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
 * $Id: SaLSa-Subset.h 364 2022-03-07 20:00:15Z li $
 */

#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include "subset.h"
#include "subspace.h"

namespace subset {
namespace salsa {

typedef std::uint_fast8_t flag_type; // Flag type.
typedef std::uint_fast32_t id_type; // ID type.
typedef std::uint_fast32_t size_type; // Size type.
typedef std::uint_fast32_t uint32; // Unsigned integer type (for SubsetTrie).
typedef std::uint_fast16_t uint16; // Unsigned integer type (for SubsetTrie).

template<class T>
struct Tuple {
  id_type id = 0;
  T min = 0;
  ::subspace::Subspace subspace;
  T sum = 0;
  T *value = nullptr;
  Tuple(id_type id, T min, ::subspace::Subspace subspace, T sum, T *value)
      : id(id), min(min), subspace(subspace), sum(sum), value(value) {}
  bool dominates(const Tuple<T> &t, size_type d) const {
    size_type dominating = 0;
    for (size_type m = 0; m < d; ++m) {
      if (t.value[m] < value[m]) {
        return false;
      } else if (value[m] < t.value[m]) {
        ++dominating;
      }
    }
    return dominating > 0;
  }
  bool operator<(const Tuple<T> &t) const {
    if (min < t.min) {
      return true;
    }
    if (min == t.min && sum < t.sum) {
      return true;
    }
    return false;
  }
};

template<class T>
struct SkylineTuple {
  const Tuple<T> *tuple;
  explicit SkylineTuple(const Tuple<T> &tuple) : tuple(&tuple) {};
  bool operator<(const SkylineTuple<T> &s) const {
    return tuple->sum < s.tuple->sum;
  }
};

template<class T>
class SaLSa {
public:
  explicit SaLSa(const std::vector<T *> &, size_type);
  ~SaLSa();
  void build();
  void prepare();
  void query();
  const std::vector<size_t> &skyline();
  size_type DT = 0;
private:
  size_type c_ = 0;
  size_type d_ = 0;
  const std::vector<T *> &base_;
  ::subspace::Data<T> *data_ = nullptr;
  std::vector<size_t> skyline_;
  std::vector<Tuple<T>> tuples_;
};

template<class T>
SaLSa<T>::SaLSa(const std::vector<T *> &data, size_type dimensionality) : base_(data) {
  c_ = base_.size();
  d_ = dimensionality;
}

template<class T>
SaLSa<T>::~SaLSa() {
  delete data_;
}

template<class T>
void SaLSa<T>::build() {
  data_ = new subspace::Data<T>(base_, d_);
  this->DT = subspace::Superpose<T>(*data_);
  skyline_.insert(skyline_.end(), data_->skyline.begin(), data_->skyline.end());
}

template<class T>
void SaLSa<T>::prepare() {
  c_ = data_->tuples.size();
  tuples_.reserve(c_);
  for (auto &tuple: data_->tuples) {
    auto *value = tuple.value;
    T min = value[0];
    T sum = value[0];
    for (size_type m = 1; m < d_; ++m) {
      if (value[m] < min) {
        min = value[m];
      }
      sum += value[m];
    }
    tuples_.emplace_back(tuple.id, min, tuple.subspace, sum, value);
  }
  std::sort(tuples_.begin(), tuples_.end());
}

template<class T>
void SaLSa<T>::query() {
  auto skyline = subset::sorted::Trie<SkylineTuple<T>>
  (d_);
  double stop = DBL_MAX;
  for (auto &tuple: tuples_) {
    if (stop < tuple.min) {
      break;
    }
    bool dominated = false;
    std::vector<std::set<SkylineTuple<T>> *> subskyline;
    skyline.get(tuple.subspace, subskyline);
    for (auto *sky: subskyline) {
      for (auto &s: *sky) {
        if (tuple.sum < s.tuple->sum) {
          break;
        }
        ++this->DT;
        dominated = s.tuple->dominates(tuple, d_);
        if (dominated) {
          break;
        }
      }
      if (dominated) {
        break;
      }
    }
    if (!dominated) {
      skyline.add(tuple.subspace, SkylineTuple<T>(tuple));
      skyline_.push_back(tuple.id);
      double max = tuple.value[0];
      for (size_type m = 1; m < d_; ++m) {
        if (max < tuple.value[m])
          max = tuple.value[m];
      }
      if (max < stop) {
        stop = max;
      }
    }
  }
}

template<class T>
const std::vector<size_t> &SaLSa<T>::skyline() {
  return skyline_;
}

}
}
