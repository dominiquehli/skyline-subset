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
 * $Id: SFS.h 364 2022-03-07 20:00:15Z li $
 */

#include <algorithm>
#include <cfloat>
#include <cstring>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace sfs {

typedef std::uint_fast8_t flag_type; // Flag type.
typedef std::uint_fast32_t id_type; // ID type.
typedef std::uint_fast32_t size_type; // Size type.

template<class T>
struct Tuple {
  id_type id = 0;
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
    return sum < t.sum;
  }
};

template<class T>
class SFS {
public:
  explicit SFS(const std::vector<T *> &, size_type);
  ~SFS() = default;
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
SFS<T>::SFS(const std::vector<T *> &data, size_type dimensionality) : data_(data) {
  c_ = data_.size();
  d_ = dimensionality;
}

template<class T>
void SFS<T>::build() {
  tuples_.resize(c_);
  for (size_type i = 0; i < c_; ++i) {
    auto *t = data_[i] + 1;
    tuples_[i].id = i;
    tuples_[i].sum = t[0];
    for (size_type m = 1; m < d_; ++m) {
      tuples_[i].sum += t[m];
    }
    tuples_[i].value = t;
  }
  std::sort(tuples_.begin(), tuples_.end());
}

template<class T>
void SFS<T>::query() {
  std::vector<Tuple<T> *> skyline;
  for (auto &tuple : tuples_) {
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
    }
  }
  for (auto *x: skyline) {
    skyline_.push_back(x->id);
  }
}

template<class T>
const std::vector<size_t> &SFS<T>::skyline() {
  return skyline_;
}

}
