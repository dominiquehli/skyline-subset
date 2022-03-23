/*-
 * Copyright (c) 2019-2020 Rui Liu and Dominique H. Li <dhli@univ-tours.fr>
 * All rights reserved.
 *
 * Redistribution and use in source and program forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in program form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: SDI-Subset.h 364 2022-03-07 20:00:15Z li $
 */

#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include "subset.h"
#include "subspace.h"

namespace subset {
namespace sdi {

typedef size_t c_type;    // Cardinality type
typedef size_t d_type;    // Dimensionality type
typedef c_type id_type;   // ID type

// Runtime flags required by the SDI algorithm.
typedef std::uint_fast8_t flag_type;
constexpr flag_type FLAG_PRUNED{0x1};
constexpr flag_type FLAG_SKYLINE{0x2};
constexpr flag_type FLAG_TESTED{0x4};

template<class T>
struct Entry {
  id_type id = 0;
  T value = 0;
  Entry() = default;
  Entry(id_type id, T value) : id(id), value(value) {}
  bool operator<(const Entry &e) const {
    return value < e.value;
  }
  bool operator==(const Entry &e) const {
    return id == e.id;
  }
};

template<class T>
struct Score {
  T max = 0;
  T min = 0;
  T sum = 0;
};

template<class T>
class SDI {
public:
  explicit SDI(const std::vector<T *> &, size_t);
  ~SDI();
  void build();
  void query();
  const std::vector<size_t> &skyline();
  size_t DT = 0;
  size_t TT = 0;
private:
  d_type best_dimension_();
  void build_index_();
  void build_score_();
  inline bool dominate_(d_type, id_type, id_type);
  inline bool incomparable_(id_type, id_type);
#ifndef WITHOUT_STOPLINE
  void locate_stopline_();
#endif
  bool query_dimension_dominance_(d_type, id_type);
  size_t query_test_block_(std::vector<const Entry<T> *> &, d_type);
  size_t query_test_tuple_(id_type, d_type);
  const std::vector<T *> &base_;
  c_type c_ = 0;
  d_type d_ = 0;
  subspace::Data<T> *data_ = nullptr;
  std::vector<std::uint_fast8_t> flags_;
  std::vector<std::vector<Entry<T>>> index_;
  std::vector<Score<T>> score_;
  std::vector<::subset::Trie<id_type>> skylines_;
  std::vector<id_type> skyline_;
#ifndef WITHOUT_STOPLINE
  id_type stopline_ = 0;
  std::vector<bool> stopped_;
#endif
  std::vector<::subspace::Tuple<T>> *tuples_ = nullptr;
};

template<class T>
SDI<T>::~SDI() {
  delete data_;
}

template<class T>
SDI<T>::SDI(const std::vector<T *> &data, size_t dimensionality) : base_(data) {
  c_ = base_.size();
  d_ = dimensionality;
  for (size_t m = 0; m < d_; ++m) {
    skylines_.emplace_back(d_);
  }
}

template<class T>
void SDI<T>::build() {
  data_ = new subspace::Data<T>(base_, d_);
  this->DT = subspace::Superpose<T>(*data_);
  skyline_.insert(skyline_.end(), data_->skyline.begin(), data_->skyline.end());
  if (data_->tuples.empty()) {
    return;
  }
  tuples_ = &data_->tuples;
  c_ = tuples_->size();
  flags_.resize(c_, 0);
  score_.resize(c_);
  build_index_();
  if (d_ > 3) {
    build_score_();
  }
}

template<class T>
void SDI<T>::query() {
  if (data_->tuples.empty()) {
    return;
  }
#ifndef WITHOUT_STOPLINE
  locate_stopline_();
  bool stop = false;
#endif
  std::vector<const Entry<T> *> block;
  std::vector<c_type> it(d_, 0);
  // The main loop.
  for (;;) {
    // Find the best dimension.
    d_type b = best_dimension_();
    bool finish = false;
    // Go ahead.
    while (it[b] < c_) {
      auto dp = it[b]++;
      if (it[b] == c_) {
        finish = true;
      }
      auto &e = index_[b][dp];
      auto id = e.id;
      bool repeat = finish ? false : index_[b][it[b]].value == e.value;
#ifndef WITHOUT_STOPLINE
      if (id == stopline_) {
        stopped_[b] = true;
        stop = true;
        for (d_type k = 0; k < d_; ++k) {
          stop &= stopped_[k];
        }
      }
#endif
      if (!(flags_[id] & FLAG_TESTED)) {
        flags_[id] |= FLAG_TESTED;
#ifdef XTT
        ++TT;
#endif
      }
      if (dp > 0 && e.value == index_[b][dp - 1].value) {
        if (!(flags_[id] & FLAG_PRUNED)) {
          block.push_back(&e);
        }
        if (!finish && repeat) {
          continue;
        }
      } else {
        if (!finish && repeat) {
          if (!(flags_[id] & FLAG_PRUNED)) {
            block.push_back(&e);
          }
          continue;
        }
      }
      int sky;
      if (block.empty()) {
        sky = query_test_tuple_(id, b);
      } else {
        if (block.size() == 1) {
          sky = query_test_tuple_(block[0]->id, b);
        } else {
          sky = query_test_block_(block, b);
        }
        block.clear();
      }
      // All dimensions reach the stop-line.
      if (stop) {
        break;
      }
      // Do dimension switching while finding any skyline tuples.
      if (sky > 0) {
        break;
      }
    }
    // All dimensions reach the stop-line.
    if (stop) {
      break;
    }
    if (finish) {
      break;
    }
  }
}

template<class T>
const std::vector<size_t> &SDI<T>::skyline() {
  return skyline_;
}

template<class T>
d_type SDI<T>::best_dimension_() {
  d_type b = 0;
  for (d_type d = 0; d < d_; ++d) {
    if (!stopped_[d]) {
      b = d;
      break;
    }
  }
  for (d_type d = 0; d < d_; ++d) {
    if (stopped_[d]) {
      continue;
    }
    if (skylines_[d].size() < skylines_[b].size()) {
      b = d;
    }
  }
  return b;
}

template<class T>
void SDI<T>::build_index_() {
  index_.resize(d_);
  for (d_type d = 0; d < d_; ++d) {
    index_[d].resize(c_);
  }
  for (c_type i = 0; i < c_; ++i) {
    for (d_type d = 0; d < d_; ++d) {
      index_[d][i].id = i;
      index_[d][i].value = (*tuples_)[i].value[d]; /// Starts from 0.
      // Important note:
      // In this implementation, Entry ID is no longer Tuple ID but Tuple Index,
      // any Tuple can be directly accessed by Entry ID.
    }
  }
  for (d_type d = 0; d < d_; ++d) {
    std::sort(index_[d].begin(), index_[d].end());
  }
}

template<class T>
void SDI<T>::build_score_() {
  for (c_type i = 0; i < c_; ++i) {
    auto *t = (*tuples_)[i].value; /// Dimension starts at 0.
    T max = INT_MIN;
    T min = INT_MAX;
    for (d_type d = 0; d < d_; ++d) {
      if (max < t[d]) {
        max = t[d];
      }
      if (min > t[d]) {
        min = t[d];
      }
    }
    score_[i].max = max;
    score_[i].min = min;
    score_[i].sum = (*tuples_)[i].sum;
  }
}

template<class T>
bool SDI<T>::dominate_(d_type skip, id_type t1, id_type t2) {
  if (d_ > 3 && incomparable_(t1, t2)) {
    return false;
  }
#ifdef XDT
  ++DT;
#endif
  size_t dominating = 0;
  auto *p1 = tuples_->at(t1).value; /// Starts from 0.
  auto *p2 = tuples_->at(t2).value; /// Starts from 0.
  for (size_t d = 0; d < d_; ++d) {
    if (d == skip) {
      continue;
    }
    if (p1[d] > p2[d]) {
      return false;
    } else if (p1[d] < p2[d]) {
      ++dominating;
    }
  }
  return dominating > 0;
}

template<class T>
bool SDI<T>::incomparable_(id_type t1, id_type t2) {
  // Returns true of a skyline r1 is incomparable with a testing tuple r2
  // w.r.t. whether r1 dominates r2 (i.e., r1 < r2).
  auto &x1 = score_[t1];
  auto &x2 = score_[t2];
  return x1.max > x2.max || x1.min > x2.min || x1.sum > x2.sum;
}

#ifndef WITHOUT_STOPLINE
template<class T>
void SDI<T>::locate_stopline_() {
  std::vector<c_type> max(c_, 0);
  stopline_ = 0;
  for (d_type d = 0; d < d_; ++d) {
    auto &column = index_[d];
    for (c_type i = 0; i < c_; ++i) {
      if (max[column[i].id] < i) {
        max[column[i].id] = i;
      }
    }
  }
  for (c_type i = 1; i < c_; ++i) {
    if (max[i] < max[stopline_]) {
      stopline_ = i;
    }
  }
  flags_[stopline_] |= FLAG_SKYLINE | FLAG_TESTED;
  skyline_.push_back(stopline_);
  stopped_.resize(d_, false);
#ifdef XTT
  ++TT;
#endif
}
#endif

template<class T>
bool SDI<T>::query_dimension_dominance_(size_t d, id_type t) {
  std::vector<id_type> skyline;
  skylines_[d].get(tuples_->at(t).subspace, skyline);
  for (auto s: skyline) {
    if (dominate_(d, s, t)) {
      return true;
    }
  }
  return false;
}

template<class T>
size_t SDI<T>::query_test_block_(std::vector<const Entry<T> *> &block, d_type d) {
  for (size_t i = 0; i < block.size() - 1; ++i) {
    if (!block[i]) {
      continue;
    }
    auto i_id = block[i]->id;
    if (flags_[i_id] & FLAG_PRUNED) {
      continue;
    }
    auto i_sky = flags_[i_id] & FLAG_SKYLINE;
    for (size_t j = i + 1; j < block.size(); ++j) {
      if (!block[j]) {
        continue;
      }
      auto j_id = block[j]->id;
      if (flags_[j_id] & FLAG_PRUNED) {
        continue;
      }
      auto j_sky = flags_[j_id] & FLAG_SKYLINE;
      if (i_sky && j_sky) {
        continue;
      } else if (!j_sky && dominate_(d, i_id, j_id)) {
        flags_[j_id] |= FLAG_PRUNED;
        block[j] = nullptr;
      } else if (!i_sky && dominate_(d, j_id, i_id)) {
        flags_[i_id] |= FLAG_PRUNED;
        block[i] = nullptr;
        break;
      }
    }
  }
  size_t sky = 0;
  for (auto &&x: block) {
    if (!x) {
      continue;
    }
    auto x_id = x->id;
    if (flags_[x_id] & FLAG_SKYLINE) {
      auto &tuple = tuples_->at(x_id);
      skylines_[d].add(tuples_->at(x_id).subspace, x_id);
      ++sky;
    } else {
      if (query_dimension_dominance_(d, x_id)) {
        flags_[x_id] |= FLAG_PRUNED;
      } else {
        flags_[x_id] |= FLAG_SKYLINE;
        skyline_.push_back(x_id);
        skylines_[d].add(tuples_->at(x_id).subspace, x_id);
        ++sky;
      }
    }
  }
  return sky;
}

template<class T>
size_t SDI<T>::query_test_tuple_(id_type id, d_type d) {
  if (flags_[id] & FLAG_SKYLINE) {
    skylines_[d].add(tuples_->at(id).subspace, id);
    return 1;
  } else {
    if (flags_[id] & FLAG_PRUNED) {
      return 0;
    }
    if (query_dimension_dominance_(d, id)) {
      flags_[id] |= FLAG_PRUNED;
      return 0;
    } else {
      flags_[id] |= FLAG_SKYLINE;
      skyline_.push_back(id);
      skylines_[d].add(tuples_->at(id).subspace, id);
      return 1;
    }
  }
}

}
}
