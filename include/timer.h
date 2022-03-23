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
 * $Id: timer.h 364 2022-03-07 20:00:15Z li $
 */

#ifndef BENCH_TIMER_H_
#define BENCH_TIMER_H_

#include <ctime>
#include <unordered_map>

template<class TYPE = double>
class Timer {
public:
  static double microtime();
  TYPE get();
  TYPE get(const char *);
  void start();
  void start(const char *);
  void stop();
  void stop(const char *);
private:
  std::unordered_map<std::string, double> interval_;
  std::unordered_map<std::string, double> start_;
};

template<class TYPE>
double Timer<TYPE>::microtime() {
  return (double) clock() / CLOCKS_PER_SEC * 1000.0;
}

template<class TYPE>
TYPE Timer<TYPE>::get() {
  return (TYPE) get("***TIMER***");
}

template<class TYPE>
TYPE Timer<TYPE>::get(const char *name) {
  return (TYPE) interval_[name];
}

template<class TYPE>
void Timer<TYPE>::start() {
  start("***TIMER***");
}

template<class TYPE>
void Timer<TYPE>::start(const char *name) {
  start_[name] = microtime();
}

template<class TYPE>
void Timer<TYPE>::stop() {
  stop("***TIMER***");
}

template<class TYPE>
void Timer<TYPE>::stop(const char *name) {
  double time = microtime();
  interval_[name] += time - start_[name];
}

#endif //BENCH_TIMER_H_
