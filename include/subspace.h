/*-
 * Copyright (c) 2022 Dominique H. Li <dhli@univ-tours.fr>
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
 * $Id: subspace.h 364 2022-03-07 20:00:15Z li $
 */

#ifndef BENCH_SUBSPACE_H_
#define BENCH_SUBSPACE_H_

#include <bitset>
#include <map>
#include <vector>

#ifndef BITS
#define BITS 32
#endif

namespace subspace {

typedef std::uint_fast8_t flag_type; // Flag type.
typedef std::uint_fast32_t id_type; // ID type.
typedef std::uint_fast32_t size_type; // Size type.

typedef std::bitset<BITS> Subspace;

template<class T>
struct Tuple {
  const static flag_type pruned = 0x1;
  const static flag_type skyline = 0x2;
  id_type id = 0;
  flag_type flags = 0;
  T sum = 0;
  std::bitset<BITS> subspace;
  T *value = nullptr;
  Tuple(id_type id, T sum, std::bitset<BITS> subspace, T *value)
      : id(id), sum(sum), subspace(subspace), value(value) {}
  bool dominates(const Tuple<T> &tuple, size_type d) {
    size_t dominating = 0;
    for (size_type m = 0; m < d; ++m) {
      if (tuple.value[m] < value[m]) {
        return false;
      } else if (value[m] < tuple.value[m]) {
        ++dominating;
      }
    }
    return dominating > 0;
  }
  bool operator<(const Tuple<T> &tuple) const {
    return sum < tuple.sum;
  }
};

template<class T>
struct Data {
  Data(const std::vector<T *> &base, size_t d) : base(base), size(base.size()), width(d) {};
  const std::vector<T *> &base;
  size_type size;
  std::vector<id_type> skyline;
  std::vector<Tuple<T>> tuples;
  size_type width = 0;
};

template<class T>
size_t Superpose(Data<T> &data, size_t stable = 0) {
  size_t DT = 0;
  std::vector<id_type> ids(data.size);
  std::vector<double> scores(data.size, 0);
  std::vector<std::bitset<BITS>> subspaces(data.size);
  // Compute Euclidean distance scores.
  for (size_type i = 0; i < data.size; ++i) {
    auto *x = data.base[i] + 1;
    for (size_type m = 0; m < data.width; ++m, ++x) {
      scores[i] += *x;
    }
    ids[i] = i;
  }
  // Prune dominated tuples and compute subspaces.
  std::vector<size_type> count(data.size);
  for (;;) {
    std::vector<id_type> current;
    current.reserve(ids.size());
    // Find the stop-line.
    id_type minima = ids[0];
    for (size_type i = 1; i < ids.size(); ++i) {
      if (scores[ids[i]] < scores[minima]) {
        minima = ids[i];
      }
    }
    data.skyline.push_back(minima);
    for (auto &id: ids) {
      if (id == minima) {
        continue;
      }
      ++DT;
      auto *p1 = data.base[minima] + 1; /// Starts from 1.
      auto *p2 = data.base[id] + 1; /// Starts from 1.
      std::bitset<BITS> subspace;
      for (size_type m = 0; m < data.width; ++m) {
        if (p2[m] < p1[m]) {
          subspace.set(m);
        }
      }
      if (subspace.none()) {
        subspaces[id] = 0;
      } else {
        subspaces[id] |= subspace;
        current.push_back(id);
      }
    }
    if (current.empty()) {
      return DT;
    }
    ids.clear();
    ids.assign(current.begin(), current.end());
    std::map<size_t, size_t> probe;
    for (auto &id: ids) {
      ++probe[subspaces[id].count()];
    }
    size_type number = 0;
    auto it = probe.begin();
    for (size_type m = 0; m < data.width && it != probe.end(); ++m, ++it) {
      if (count[m] == it->second) {
        ++number;
      }
    }
    if (number >= (stable ? (stable > probe.size() ? probe.size() : stable) : data.width / 3) - 1) {
      break;
    }
    it = probe.begin();
    for (size_type m = 0; m < data.width && it != probe.end(); ++m, ++it) {
      count[m] = it->second;
    }
  }
  data.tuples.reserve(ids.size());
  for (auto &id: ids) {
    data.tuples.emplace_back(id, scores[id], subspaces[id], data.base[id] + 1);
  }
  return DT;
}

}

#endif //BENCH_SUBSPACE_H_
