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
 * $Id: subset.h 364 2022-03-07 20:00:15Z li $
 */

#ifndef BENCH_SUBSET_H_
#define BENCH_SUBSET_H_

#include <bitset>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

#ifndef BITS
#define BITS 32
#endif

namespace subset {

typedef std::uint_fast16_t uint16; // Till to 65536 bits.

template<class E>
struct Node {
  Node() = default;
  virtual ~Node() {
    for (auto &pa: nodes) {
      delete pa.second;
    }
  }
  Node *get(uint16 bit) {
    auto it = nodes.find(bit);
    if (it == nodes.end()) {
      it = nodes.insert(std::make_pair(bit, new Node<E>())).first;
    }
    return it->second;
  }
  void put(E e) {
    entries.push_back(e);
  }
  std::vector<E> entries;
  std::unordered_map<uint16, Node *> nodes;
};

template<class E, size_t SIZE = BITS>
struct Trie {
  explicit Trie(uint16 d) : d_(d) {};
  void add(E e) {
    entries_.push_back(e);
    ++size_;
  }
  void add(const std::bitset<SIZE> &bits, E e) {
    auto *node = &root_;
    for (uint16 i = 0; i < d_; ++i) {
      if (!bits.test(i)) {
        node = node->get(i);
      }
    }
    node->put(e);
    ++size_;
  }
  void get(std::vector<E> &entries) {
    entries.clear();
    entries.insert(entries.end(), entries_.begin(), entries_.end());
  }
  void get(const std::bitset<SIZE> &bits, std::vector<E> &entries) {
    entries.clear();
    if (!root_.entries.empty()) {
      entries.insert(entries.end(), root_.entries.begin(), root_.entries.end());
    }
    for (auto &n: root_.nodes) {
      if (!bits.test(n.first)) {
        get_(bits, entries, n.second);
      }
    }
  }
  size_t size() const {
    return size_;
  }
private:
  void get_(const std::bitset<SIZE> &bits, std::vector<E> &entries, Node<E> *node) {
    if (!node->entries.empty()) {
      entries.insert(entries.end(), node->entries.begin(), node->entries.end());
    }
    for (auto &n: node->nodes) {
      if (!bits.test(n.first)) {
        get_(bits, entries, n.second);
      }
    }
  }
  uint16 d_ = 0;
  std::vector<E> entries_;
  Node<E> root_;
  size_t size_ = 0;
};

namespace sorted {

template<class E>
struct Node {
  Node() = default;
  virtual ~Node() {
    for (auto &pa: nodes) {
      delete pa.second;
    }
  }
  Node *get(uint16 bit) {
    auto it = nodes.find(bit);
    if (it == nodes.end()) {
      it = nodes.insert(std::make_pair(bit, new Node<E>())).first;
    }
    return it->second;
  }
  void put(E e) {
    entries.insert(e);
  }
  std::set<E> entries;
  std::unordered_map<uint16, Node *> nodes;
};

template<class E, size_t SIZE = BITS>
struct Trie {
  explicit Trie(uint16 d) : d_(d) {};
  void add(const std::bitset<SIZE> &bits, E e) {
    auto *node = &root_;
    for (uint16 i = 0; i < d_; ++i) {
      if (!bits.test(i)) {
        node = node->get(i);
      }
    }
    node->put(e);
    ++size_;
  }
  void get(const std::bitset<SIZE> &bits, std::vector<std::set<E> *> &entries) {
    entries.clear();
    if (!root_.entries.empty()) {
      entries.push_back(&root_.entries);
    }
    for (auto &n: root_.nodes) {
      if (!bits.test(n.first)) {
        get_(bits, entries, n.second);
      }
    }
  }
  size_t size() const {
    return size_;
  }
private:
  void get_(const std::bitset<SIZE> &bits, std::vector<std::set<E> *> &entries, Node<E> *node) {
    if (!node->entries.empty()) {
      entries.push_back(&node->entries);
    }
    for (auto &n: node->nodes) {
      if (!bits.test(n.first)) {
        get_(bits, entries, n.second);
      }
    }
  }
  uint16 d_ = 0;
  Node<E> root_;
  size_t size_ = 0;
};

}

}

#endif //BENCH_SUBSET_H_
