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
 * $Id: BSkyTreePMethod.h 364 2022-03-07 20:00:15Z li $
 */

#include "method.h"
#include "Points.h"
#include "BSkyTreeP.h"

struct BSkyTreePMethod : public Method<double, double> {
  const char *__name() override;
  void build() override;
  void finalize() override;
  void query() override;
};

const char *BSkyTreePMethod::__name() {
  return "BSkyTreeP";
}

void BSkyTreePMethod::build() {
  nGMeasure = 0;
  for (int i = 0; i < this->dataset_->width(); i++) {
    AttList.push_back(i + 1);
  }
}

void BSkyTreePMethod::finalize() {
  DT = nGMeasure;
  for (size_t i = 0; i < Skyline.size(); ++i) {
    this->skyline_.push_back(i); // Just for counting.
  }
}

void BSkyTreePMethod::query() {
  ExecuteBSkyTreeP(AttList, this->dataset_->data(), Skyline, COS);
}
