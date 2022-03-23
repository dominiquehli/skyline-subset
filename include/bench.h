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
 * $Id: bench.h 364 2022-03-07 20:00:15Z li $
 */

#ifndef BENCH_BENCH_H_
#define BENCH_BENCH_H_

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "dataset.h"
#include "method.h"
#include "timer.h"

#ifndef BENCH_SDI
#define BENCH_SDI
#endif

#ifndef INPUT_DEFINED
typedef double input_type;
#define INPUT_DEFINED
#endif

#ifndef VALUE_DEFINED
typedef double value_type;
#define VALUE_DEFINED
#endif

//////////////////////// Bench /////////////////////////////////////////////////

template<class METHOD, class INPUT = input_type, class VALUE = value_type>
struct Bench {
  explicit Bench(const char *);
  int run(int, char **);
private:
  std::string name_; // Method name.
};

template<class METHOD, class INPUT, class VALUE>
Bench<METHOD, INPUT, VALUE>::Bench(const char *name) {
  name_ = name;
}

template<class METHOD, class INPUT, class VALUE>
int Bench<METHOD, INPUT, VALUE>::run(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Usage: " << name_ << " DATASET DIMENSIONALITY"
              << std::endl;
    return 0;
  }
  const char *a = argv[1];
  std::ifstream in;
  if (a) {
    in.open(a);
    if (!in.good()) {
      std::cerr << "*** Error: cannot open file " << a << std::endl;
      return 0;
    }
  }
  size_t d = strtoul(argv[2], nullptr, 10);
  std::cout << "# Method: " << name_ << std::endl;
  std::cout << "# Dataset: " << a << std::endl;
  Dataset<INPUT, VALUE> dataset(d, in);
  METHOD method;
  method.assign(dataset);
  method.setup(argc, argv);
  Timer<double> timer;
  std::cout << "# Cardinality: " << dataset.size() << std::endl;
  std::cout << "# Dimensionality: " << d << std::endl;
  timer.start("build");
  method.build();
  timer.stop("build");
  std::cout << "# Build Time: " << timer.get("build") << " ms" << std::endl;
  timer.start("prepare");
  method.prepare();
  timer.stop("prepare");
  std::cout << "# Prepare Time: " << timer.get("prepare") << " ms" << std::endl;
  timer.start("query");
  method.query();
  timer.stop("query");
  method.finalize();
  double total = timer.get("build") + timer.get("query");
  std::cout << "# Query Time: " << timer.get("query") << " ms" << std::endl;
  std::cout << "# Total Time: " << total << " ms" << std::endl;
  std::cout << "# Skyline: " << method.skyline().size() << std::endl;
  std::cout << "# DT: " << method.DT << std::endl;
  std::cout << "# DT/Tuple: " << method.DT * 1.0 / dataset.size() << std::endl;
  std::cout << "# ST: " << method.ST << std::endl;
  std::cout << "# ST/Tuple: " << method.ST * 1.0 / dataset.size() << std::endl;
  std::cout << "# TT: " << method.TT << std::endl;
  std::cout << "#= " << name_ << " | "
            << dataset.size() << " | "
            << dataset.width() << " | "
            << method.skyline().size() << " | "
            << method.DT << " | "
            << method.DT * 1.0 / dataset.size() << " | "
            << method.TT << " | "
            << timer.get("build") << " | "
            << timer.get("query") << " | "
            << total
            << std::endl;
  if (argc > 5) {
    const char *o = argv[5];
    std::ofstream out(o);
    size_t n = 0;
    for (auto &x: method.skyline()) {
      out << (++n) << "\t" << x << std::endl;
    }
    out.close();
  }
  return 0;
}

#endif //BENCH_BENCH_H_
