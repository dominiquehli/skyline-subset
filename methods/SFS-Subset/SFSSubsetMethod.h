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
 * $Id: SFSSubsetMethod.h 364 2022-03-07 20:00:15Z li $
 */

#include <cfloat>
#include <numeric>
#include <vector>
#include "method.h"
#include "SFS-Subset.h"

template<class I = input_type, class V = value_type>
class SFSSubsetMethod : public Method<I, V> {
public:
  SFSSubsetMethod() = default;
  ~SFSSubsetMethod() = default;
  const char *__name() override;
  void assign(Dataset<I, V> &) override;
  void build() override;
  void finalize() override;
  void prepare() override;
  void query() override;
private:
  subset::sfs::SFS<V> *x_ = nullptr;
};

template<class I, class V>
const char *SFSSubsetMethod<I, V>::__name() {
  return "SFS-Subset";
}

template<class I, class V>
void SFSSubsetMethod<I, V>::assign(Dataset<I, V> &dataset) {
  Method<I, V>::assign(dataset);
  x_ = new subset::sfs::SFS<V>(this->dataset_->data(), this->dataset_->width());
}

template<class I, class V>
void SFSSubsetMethod<I, V>::build() {
  x_->build();
}

template<class I, class V>
void SFSSubsetMethod<I, V>::prepare() {
  x_->prepare();
}

template<class I, class V>
void SFSSubsetMethod<I, V>::finalize() {
  this->skyline_ = x_->skyline();
  this->DT = x_->DT;
}

template<class I, class V>
void SFSSubsetMethod<I, V>::query() {
  x_->query();
}
