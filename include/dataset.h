/*-
 * Copyright (c) 2021 Dominique H. Li <dhli@univ-tours.fr>
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
 * $Id: dataset.h 364 2022-03-07 20:00:15Z li $
 */

#ifndef BENCH_DATASET_H_
#define BENCH_DATASET_H_

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#ifndef GETLINE_BUFFER
#define GETLINE_BUFFER 65536
#endif

#ifndef INPUT_DEFINED
typedef double input_type;
#define INPUT_DEFINED
#endif

#ifndef VALUE_DEFINED
typedef double value_type;
#define VALUE_DEFINED
#endif

template<class T>
struct __struct_dataset_entry;

template<class INPUT = input_type, class VALUE = value_type>
struct Dataset {
  typedef VALUE *Tuple;
  typedef std::vector<VALUE *> Vector;
  Dataset(size_t, std::istream &);
  ~Dataset();
  auto at(size_t) -> VALUE *;
  auto at(size_t) const -> const VALUE *;
  auto data() -> Vector &;
  auto data() const -> const Vector &;
  auto dominate(size_t, size_t) const -> bool;
  auto dominate(size_t, size_t, const std::vector<size_t> &) const -> bool;
  auto size() const -> size_t;
  auto width() const -> size_t;
  auto operator[](size_t) -> VALUE *;
  auto operator[](size_t) const -> const VALUE *;
private:
  size_t cardinality_ = 0; // The cardinality of data.
  size_t dimensionality_ = 0; // The dimensionality of data.
  std::vector<INPUT *> input_{}; // The original dataset.
  std::vector<VALUE *> value_{}; // The normalized dataset.
};

//////////////////////// Dataset ///////////////////////////////////////////////

template<class INPUT, class VALUE>
Dataset<INPUT, VALUE>::Dataset(size_t d, std::istream &in) {
  dimensionality_ = d;
  char buffer[GETLINE_BUFFER];
  const auto *delim = " ,";
  size_t n = 0;
  if (typeid(INPUT) == typeid(VALUE)) {
    while (in.good()) {
      in.getline(buffer, GETLINE_BUFFER);
      char *p = strtok(buffer, delim);
      auto *v = new VALUE[dimensionality_ + 1];
      v[0] = ++n;
      size_t k = 0;
      if (p != nullptr) {
        while (k++ < d) {
          VALUE x = strtod(p, nullptr);
          v[k] = x;
          p = strtok(nullptr, delim);
        }
        value_.push_back(v);
      }
    }
  } else {
    while (in.good()) {
      in.getline(buffer, GETLINE_BUFFER);
      char *p = strtok(buffer, delim);
      auto *v = new INPUT[dimensionality_ + 1];
      v[0] = ++n;
      size_t k = 0;
      if (p != nullptr) {
        while (k++ < d) {
          INPUT x = strtod(p, nullptr);
          v[k] = x;
          p = strtok(nullptr, delim);
        }
        input_.push_back(v);
      }
    }
    size_t c = input_.size();
    std::vector<std::vector<__struct_dataset_entry<INPUT>>> index;
    index.resize(dimensionality_);
    for (size_t k = 0; k < dimensionality_; ++k) {
      index[k].resize(c);
    }
    for (size_t i = 0; i < c; ++i) {
      for (size_t k = 0; k < dimensionality_; ++k) {
        index[k][i].id = i;
        index[k][i].value = input_[i][k + 1]; /// Starts from 1.
      }
    }
    for (size_t i = 0; i < c; ++i) {
      auto *v = new VALUE[dimensionality_ + 1];
      v[0] = i;
      value_.push_back(v);
    }
    for (size_t k = 0; k < dimensionality_; ++k) {
      std::sort(index[k].begin(), index[k].end());
      value_[index[k][0].id][k + 1] = 1;
      for (size_t i = 1; i < c; ++i) {
        if (index[k][i].value == index[k][i - 1].value) {
          value_[index[k][i].id][k + 1] = value_[index[k][i - 1].id][k + 1];
        } else {
          value_[index[k][i].id][k + 1] = value_[index[k][i - 1].id][k + 1] + 1;
        }
      }
    }
    for (auto &x: input_) {
      delete[] x;
    }
  }
  cardinality_ = value_.size();
}

template<class INPUT, class VALUE>
Dataset<INPUT, VALUE>::~Dataset() {
  for (auto &x: value_) {
    delete[] x;
  }
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::at(size_t index) -> VALUE * {
  return value_.at(index);
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::at(size_t index) const -> const VALUE * {
  return value_.at(index);
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::data() -> Dataset<INPUT, VALUE>::Vector & {
  return value_;
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::data() const -> const Dataset<INPUT, VALUE>::Vector & {
  return value_;
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::dominate(size_t t1, size_t t2) const -> bool {
  size_t dominating = 0;
  auto *p1 = value_[t1];
  auto *p2 = value_[t2];
  for (size_t d = 1; d <= dimensionality_; ++d) { /// Starts from 1.
    if (p1[d] > p2[d]) {
      return false;
    } else if (p1[d] < p2[d]) {
      ++dominating;
    }
  }
  return dominating > 0;
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::dominate(size_t t1, size_t t2, const std::vector<size_t> &subspace) const -> bool {
  size_t dominating = 0;
  auto *p1 = value_.at(t1).value() + 1; /// For BSkyTree data format.
  auto *p2 = value_.at(t2).value() + 1; /// For BSkyTree data format.
  for (auto d: subspace) {
    if (p1[d] > p2[d]) {
      return false;
    } else if (p1[d] < p2[d]) {
      ++dominating;
    }
  }
  return dominating > 0;
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::size() const -> size_t {
  return cardinality_;
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::width() const -> size_t {
  return dimensionality_;
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::operator[](size_t index) -> VALUE * {
  return value_[index];
}

template<class INPUT, class VALUE>
auto Dataset<INPUT, VALUE>::operator[](size_t index) const -> const VALUE * {
  return value_[index];
}

template<class T>
struct __struct_dataset_entry {
  size_t id = 0;
  T value = 0;
  __struct_dataset_entry() = default;
  __struct_dataset_entry(size_t id, T value) : id(id), value(value) {}
  bool operator<(const __struct_dataset_entry &e) const {
    return value == e.value ? id < e.id : value < e.value;
  }
  bool operator==(const __struct_dataset_entry &e) const {
    return id == e.id;
  }
};

#endif //BENCH_DATASET_H_
