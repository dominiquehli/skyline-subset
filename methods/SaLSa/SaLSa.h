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
 * $Id: SaLSa.h 364 2022-03-07 20:00:15Z li $
 */

#include <algorithm>
#include <cfloat>
#include <cstring>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace salsa {

typedef std::uint_fast8_t flag_type; // Flag type.
typedef std::uint_fast32_t id_type; // ID type.
typedef std::uint_fast32_t size_type; // Size type.

template<class T>
struct Tuple {
  id_type id = 0;
  T min = 0;
  T sum = 0;
  T *value = nullptr;
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
class SaLSa {
public:
  explicit SaLSa(const std::vector<T *> &, size_type);
  ~SaLSa() = default;
  void build();
  void query();
  const std::vector<size_t> &skyline();
  size_type DT = 0;
private:
  size_type c_ = 0;
  size_type d_ = 0;
  const std::vector<T *> &data_;
  std::vector<size_t> skyline_;
  std::vector<Tuple<T>> tuples_;
};

template<class T>
SaLSa<T>::SaLSa(const std::vector<T *> &data, size_type dimensionality) : data_(data) {
  c_ = data_.size();
  d_ = dimensionality;
}

template<class T>
void SaLSa<T>::build() {
  tuples_.resize(c_);
  for (size_type i = 0; i < c_; ++i) {
    auto *t = data_[i] + 1;
    tuples_[i].id = i;
    tuples_[i].min = t[0];
    tuples_[i].sum = t[0];
    for (size_type m = 1; m < d_; ++m) {
      if (t[m] < tuples_[i].min) {
        tuples_[i].min = t[m];
      }
      tuples_[i].sum += t[m];
    }
    tuples_[i].value = t;
  }
  std::sort(tuples_.begin(), tuples_.end());
}

template<class T>
void SaLSa<T>::query() {
  std::vector<Tuple<T> *> skyline;
  double stop = DBL_MAX;
  for (auto &tuple : tuples_) {
    if (stop < tuple.min) {
      break;
    }
    bool dominated = false;
    auto it = skyline.begin();
    for (; it != skyline.end(); ++it) {
      if (tuple.sum < (*it)->sum) {
        break;
      }
      ++this->DT;
      dominated = (*it)->dominates(tuple, d_);
      if (dominated) {
        break;
      }
    }
    if (!dominated) {
      skyline.insert(it, &tuple);
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
  for (auto *x: skyline) {
    skyline_.push_back(x->id);
  }
}

template<class T>
const std::vector<size_t> &SaLSa<T>::skyline() {
  return skyline_;
}

}
