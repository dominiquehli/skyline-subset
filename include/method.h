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
 * $Id: method.h 364 2022-03-07 20:00:15Z li $
 */

#ifndef BENCH_METHOD_H_
#define BENCH_METHOD_H_

#include <bitset>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "dataset.h"

#ifndef INPUT_DEFINED
typedef double input_type;
#define INPUT_DEFINED
#endif

#ifndef VALUE_DEFINED
typedef double value_type;
#define VALUE_DEFINED
#endif

//////////////////////// Method ////////////////////////////////////////////////

template<class INPUT = input_type, class VALUE = value_type>
struct Method {
  size_t DT = 0; // Dominance test counter
  size_t ST = 0; // Subset test counter
  size_t TT = 0; // Tested tuple counter
  Method() = default;
  virtual auto __name() -> const char * = 0;
  virtual auto assign(Dataset<INPUT, VALUE> &) -> void;
  virtual auto build() -> void;
  virtual auto finalize() -> void;
  virtual auto prepare() -> void;
  virtual auto query() -> void = 0;
  virtual auto setup(int, char **) -> void;
  virtual auto skyline() -> const std::vector<size_t> &;
protected:
  Dataset<INPUT, VALUE> *dataset_ = nullptr;
  std::vector<size_t> skyline_;
};

template<class INPUT, class VALUE>
auto Method<INPUT, VALUE>::assign(Dataset<INPUT, VALUE> &dataset) -> void {
  dataset_ = &dataset;
}

template<class INPUT, class VALUE>
auto Method<INPUT, VALUE>::build() -> void {
}

template<class INPUT, class VALUE>
auto Method<INPUT, VALUE>::finalize() -> void {
}

template<class INPUT, class VALUE>
auto Method<INPUT, VALUE>::prepare() -> void {
}

template<class INPUT, class VALUE>
auto Method<INPUT, VALUE>::setup(int, char **) -> void {
}

template<class INPUT, class VALUE>
auto Method<INPUT, VALUE>::skyline() -> const std::vector<size_t> & {
  return skyline_;
}

#endif //BENCH_METHOD_H_
