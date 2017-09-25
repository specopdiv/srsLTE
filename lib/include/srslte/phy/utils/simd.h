/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsLTE library.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#ifndef SRSLTE_SIMD_H_H
#define SRSLTE_SIMD_H_H

#include <immintrin.h>

/*
 * SSE Macros
 */
#ifdef LV_HAVE_SSE
#define _MM_SWAP(X) ((__m128)_mm_shuffle_ps(X, X, _MM_SHUFFLE(2,3,0,1)))
#define _MM_PERM(X) ((__m128)_mm_shuffle_ps(X, X, _MM_SHUFFLE(2,1,3,0)))
#define _MM_MULJ_PS(X) _MM_SWAP(_MM_CONJ_PS(X))
#define _MM_CONJ_PS(X) (_mm_xor_ps(X, _mm_set_ps(-0.0f, 0.0f, -0.0f, 0.0f)))
#define _MM_SQMOD_PS(X) _MM_PERM(_mm_hadd_ps(_mm_mul_ps(X,X), _mm_set_ps(0.0f, 0.0f, 0.0f, 0.0f)))
#define _MM_PROD_PS(a, b) _mm_addsub_ps(_mm_mul_ps(a,_mm_moveldup_ps(b)),_mm_mul_ps(\
                            _mm_shuffle_ps(a,a,0xB1),_mm_movehdup_ps(b)))

#endif /* LV_HAVE_SSE */

/*
 * AVX Macros
 */
#ifdef LV_HAVE_AVX2

#define _MM256_MULJ_PS(X) _mm256_permute_ps(_MM256_CONJ_PS(X), 0b10110001)
#define _MM256_CONJ_PS(X) (_mm256_xor_ps(X, _mm256_set_ps(-0.0f, 0.0f, -0.0f, 0.0f, -0.0f, 0.0f, -0.0f, 0.0f)))

#ifdef LV_HAVE_FMA
#define _MM256_SQMOD_PS(A, B) _mm256_permute_ps(_mm256_hadd_ps(_mm256_fmadd_ps(A, A, _mm256_mul_ps(B,B)), \
                                           _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)), 0b11011100)
#define _MM256_PROD_PS(a, b) _mm256_fmaddsub_ps(a,_mm256_moveldup_ps(b),\
                              _mm256_mul_ps(_mm256_shuffle_ps(a,a,0xB1),_mm256_movehdup_ps(b)))
#else
#define _MM256_SQMOD_PS(A, B) _mm256_permute_ps(_mm256_hadd_ps(_mm256_add_ps(_mm256_mul_ps(A,A), _mm256_mul_ps(B,B)), \
                                           _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)), 0b11011100)
#define _MM256_PROD_PS(a, b) _mm256_addsub_ps(_mm256_mul_ps(a,_mm256_moveldup_ps(b)),\
                              _mm256_mul_ps(_mm256_shuffle_ps(a,a,0xB1),_mm256_movehdup_ps(b)))
#endif /* LV_HAVE_FMA */
#endif /* LV_HAVE_AVX2 */


/*
 * AVX extension with FMA Macros
 */
#ifdef LV_HAVE_FMA

#define _MM256_SQMOD_ADD_PS(A, B, C) _mm256_permute_ps(_mm256_hadd_ps(_mm256_fmadd_ps(A, A, _mm256_fmadd_ps(B, B, C)),\
                                           _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)), 0b11011100)

#define _MM256_PROD_ADD_PS(A, B, C) _mm256_fmaddsub_ps(A,_mm256_moveldup_ps(B),\
                              _mm256_fmaddsub_ps(_mm256_shuffle_ps(A,A,0xB1),_mm256_movehdup_ps(B), C))

#define _MM256_PROD_SUB_PS(A, B, C) _mm256_fmaddsub_ps(A,_mm256_moveldup_ps(B),\
                              _mm256_fmsubadd_ps(_mm256_shuffle_ps(A,A,0xB1),_mm256_movehdup_ps(B), C))
#endif /* LV_HAVE_FMA */



/* Memory Sizes for Single Floating Point and fixed point */
#ifdef LV_HAVE_AVX512

#define SRSLTE_SIMD_F_SIZE    16
#define SRSLTE_SIMD_CF_SIZE   16

#define SRSLTE_SIMD_S_SIZE    32
#define SRSLTE_SIMD_C16_SIZE  0

#else
#ifdef LV_HAVE_AVX2

#define SRSLTE_SIMD_F_SIZE    8
#define SRSLTE_SIMD_CF_SIZE   8

#define SRSLTE_SIMD_S_SIZE    16
#define SRSLTE_SIMD_C16_SIZE  16

#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE

#define SRSLTE_SIMD_F_SIZE    4
#define SRSLTE_SIMD_CF_SIZE   4

#define SRSLTE_SIMD_S_SIZE    8
#define SRSLTE_SIMD_C16_SIZE  8

#else /* LV_HAVE_SSE */

#define SRSLTE_SIMD_F_SIZE    0
#define SRSLTE_SIMD_CF_SIZE   0

#define SRSLTE_SIMD_S_SIZE    0
#define SRSLTE_SIMD_C16_SIZE  0

#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */



#if SRSLTE_SIMD_F_SIZE

/* Data types */
#ifdef LV_HAVE_AVX512
typedef __m512 simd_f_t;
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
typedef __m256 simd_f_t;
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
typedef __m128 simd_f_t;
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */

/* Single precision Floating point functions */
static inline simd_f_t srslte_simd_f_load(float *ptr) {
#ifdef LV_HAVE_AVX512
  return _mm512_load_ps(ptr);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_load_ps(ptr);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_load_ps(ptr);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_loadu(float *ptr) {
#ifdef LV_HAVE_AVX512
  return _mm512_loadu_ps(ptr);
#else /* LV_HAVE_AVX512 */
  #ifdef LV_HAVE_AVX2
  return _mm256_loadu_ps(ptr);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_loadu_ps(ptr);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline void srslte_simd_f_store(float *ptr, simd_f_t simdreg) {
#ifdef LV_HAVE_AVX512
  _mm512_store_ps(ptr, simdreg);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  _mm256_store_ps(ptr, simdreg);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  _mm_store_ps(ptr, simdreg);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline void srslte_simd_f_storeu(float *ptr, simd_f_t simdreg) {
#ifdef LV_HAVE_AVX512
  _mm512_storeu_ps(ptr, simdreg);
#else /* LV_HAVE_AVX512 */
  #ifdef LV_HAVE_AVX2
  _mm256_storeu_ps(ptr, simdreg);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  _mm_storeu_ps(ptr, simdreg);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_set1(float x) {
#ifdef LV_HAVE_AVX512
  return _mm512_set1_ps(x);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_set1_ps(x);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_set1_ps(x);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_mul(simd_f_t a, simd_f_t b) {
#ifdef LV_HAVE_AVX512
  return _mm512_mul_ps(a, b);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_mul_ps(a, b);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_mul_ps(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_addsub(simd_f_t a, simd_f_t b) {
#ifdef LV_HAVE_AVX512
  __m512 r = _mm512_add_ps(a, b);
  return _mm512_mask_sub_ps(r, 0b1010101010101010, a, b);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_addsub_ps(a, b);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_addsub_ps(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_sub(simd_f_t a, simd_f_t b) {
#ifdef LV_HAVE_AVX512
  return _mm512_sub_ps(a, b);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_sub_ps(a, b);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_sub_ps(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_add(simd_f_t a, simd_f_t b) {
#ifdef LV_HAVE_AVX512
  return _mm512_add_ps(a, b);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_add_ps(a, b);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_add_ps(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_zero (void) {
#ifdef LV_HAVE_AVX512
  return _mm512_setzero_ps();
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
 return _mm256_setzero_ps();
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
 return _mm_setzero_ps();
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_swap(simd_f_t a) {
#ifdef LV_HAVE_AVX512
  return _mm512_permute_ps(a, 0b10110001);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_permute_ps(a, 0b10110001);
#else /* LV_HAVE_AVX2 */
  #ifdef LV_HAVE_SSE
  return _mm_shuffle_ps(a, a, 0b10110001);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_hadd(simd_f_t a, simd_f_t b) {
#ifdef LV_HAVE_AVX512
  const __m512i idx1 = _mm512_setr_epi32((0b00000), (0b00010),
                                         (0b00100), (0b00110),
                                         (0b01000), (0b01010),
                                         (0b01100), (0b01110),
                                         (0b10000), (0b10010),
                                         (0b10100), (0b10110),
                                         (0b11000), (0b11010),
                                         (0b11100), (0b11110));
  const __m512i idx2 = _mm512_or_epi32(idx1, _mm512_set1_epi32(1));

  simd_f_t a1 = _mm512_permutex2var_ps(a, idx1, b);
  simd_f_t b1 = _mm512_permutex2var_ps(a, idx2, b);
  return _mm512_add_ps(a1, b1);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  simd_f_t a1 = _mm256_permute2f128_ps(a, b, 0b00100000);
  simd_f_t b1 = _mm256_permute2f128_ps(a, b, 0b00110001);
  return _mm256_hadd_ps(a1, b1);
#else /* LV_HAVE_AVX2 */
  #ifdef LV_HAVE_SSE
  return _mm_hadd_ps(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_f_t srslte_simd_f_sqrt(simd_f_t a) {
#ifdef LV_HAVE_AVX512
  return _mm512_sqrt_ps(a);
#else /* LV_HAVE_AVX512 */
  #ifdef LV_HAVE_AVX2
  return _mm256_sqrt_ps(a);
#else /* LV_HAVE_AVX2 */
  #ifdef LV_HAVE_SSE
  return _mm_sqrt_ps(a);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

#endif /* SRSLTE_SIMD_F_SIZE */


#if SRSLTE_SIMD_CF_SIZE

typedef struct {
  simd_f_t re;
  simd_f_t im;
} simd_cf_t;

/* Complex Single precission Floating point functions */
static inline simd_cf_t srslte_simd_cfi_load(cf_t *ptr) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  __m512 in1 = _mm512_permute_ps(_mm512_load_ps((float*)(ptr)), 0b11011000);
  __m512 in2 = _mm512_permute_ps(_mm512_load_ps((float*)(ptr + 8)), 0b11011000);
  ret.re = _mm512_unpacklo_ps(in1, in2);
  ret.im = _mm512_unpackhi_ps(in1, in2);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  __m256 in1 = _mm256_permute_ps(_mm256_load_ps((float*)(ptr)), 0b11011000);
  __m256 in2 = _mm256_permute_ps(_mm256_load_ps((float*)(ptr + 4)), 0b11011000);
  ret.re = _mm256_unpacklo_ps(in1, in2);
  ret.im = _mm256_unpackhi_ps(in1, in2);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  __m128 i1 = _mm_load_ps((float*)(ptr));
  __m128 i2 = _mm_load_ps((float*)(ptr + 2));
  ret.re = _mm_shuffle_ps(i1, i2, _MM_SHUFFLE(2,0,2,0));
  ret.im = _mm_shuffle_ps(i1, i2, _MM_SHUFFLE(3,1,3,1));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

/* Complex Single precission Floating point functions */
static inline simd_cf_t srslte_simd_cfi_loadu(cf_t *ptr) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  __m512 in1 = _mm512_permute_ps(_mm512_loadu_ps((float*)(ptr)), 0b11011000);
  __m512 in2 = _mm512_permute_ps(_mm512_loadu_ps((float*)(ptr + 8)), 0b11011000);
  ret.re = _mm512_unpacklo_ps(in1, in2);
  ret.im = _mm512_unpackhi_ps(in1, in2);
#else /* LV_HAVE_AVX512 */
  #ifdef LV_HAVE_AVX2
  __m256 in1 = _mm256_permute_ps(_mm256_loadu_ps((float*)(ptr)), 0b11011000);
  __m256 in2 = _mm256_permute_ps(_mm256_loadu_ps((float*)(ptr + 4)), 0b11011000);
  ret.re = _mm256_unpacklo_ps(in1, in2);
  ret.im = _mm256_unpackhi_ps(in1, in2);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  __m128 i1 = _mm_loadu_ps((float*)(ptr));
  __m128 i2 = _mm_loadu_ps((float*)(ptr + 2));
  ret.re = _mm_shuffle_ps(i1, i2, _MM_SHUFFLE(2,0,2,0));
  ret.im = _mm_shuffle_ps(i1, i2, _MM_SHUFFLE(3,1,3,1));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline simd_cf_t srslte_simd_cf_load(float *re, float *im) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  ret.re = _mm512_load_ps(re);
  ret.im = _mm512_load_ps(im);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  ret.re = _mm256_load_ps(re);
  ret.im = _mm256_load_ps(im);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  ret.re = _mm_load_ps(re);
  ret.im = _mm_load_ps(im);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline simd_cf_t srslte_simd_cf_loadu(float *re, float *im) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  ret.re = _mm512_loadu_ps(re);
  ret.im = _mm512_loadu_ps(im);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  ret.re = _mm256_loadu_ps(re);
  ret.im = _mm256_loadu_ps(im);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  ret.re = _mm_loadu_ps(re);
  ret.im = _mm_loadu_ps(im);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline void srslte_simd_cfi_store(cf_t *ptr, simd_cf_t simdreg) {
#ifdef LV_HAVE_AVX512
  __m512 out1 = _mm512_permute_ps(simdreg.re, 0b11011000);
  __m512 out2 = _mm512_permute_ps(simdreg.im, 0b11011000);
  _mm512_store_ps((float*)(ptr), _mm512_unpacklo_ps(out1, out2));
  _mm512_store_ps((float*)(ptr + 8), _mm512_unpackhi_ps(out1, out2));
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  __m256 out1 = _mm256_permute_ps(simdreg.re, 0b11011000);
  __m256 out2 = _mm256_permute_ps(simdreg.im, 0b11011000);
  _mm256_store_ps((float*)(ptr), _mm256_unpacklo_ps(out1, out2));
  _mm256_store_ps((float*)(ptr + 4), _mm256_unpackhi_ps(out1, out2));
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  _mm_store_ps((float*)(ptr), _mm_unpacklo_ps(simdreg.re, simdreg.im));
  _mm_store_ps((float*)(ptr + 2), _mm_unpackhi_ps(simdreg.re, simdreg.im));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline void srslte_simd_cfi_storeu(cf_t *ptr, simd_cf_t simdreg) {
#ifdef LV_HAVE_AVX512
  __m512 out1 = _mm512_permute_ps(simdreg.re, 0b11011000);
  __m512 out2 = _mm512_permute_ps(simdreg.im, 0b11011000);
  _mm512_storeu_ps((float*)(ptr), _mm512_unpacklo_ps(out1, out2));
  _mm512_storeu_ps((float*)(ptr + 8), _mm512_unpackhi_ps(out1, out2));
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  __m256 out1 = _mm256_permute_ps(simdreg.re, 0b11011000);
  __m256 out2 = _mm256_permute_ps(simdreg.im, 0b11011000);
  _mm256_storeu_ps((float*)(ptr), _mm256_unpacklo_ps(out1, out2));
  _mm256_storeu_ps((float*)(ptr + 4), _mm256_unpackhi_ps(out1, out2));
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  _mm_storeu_ps((float*)(ptr), _mm_unpacklo_ps(simdreg.re, simdreg.im));
  _mm_storeu_ps((float*)(ptr + 2), _mm_unpackhi_ps(simdreg.re, simdreg.im));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline void srslte_simd_cf_store(float *re, float *im, simd_cf_t simdreg) {
#ifdef LV_HAVE_AVX512
  _mm512_store_ps(re, simdreg.re);
  _mm512_store_ps(im, simdreg.im);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  _mm256_store_ps((float *) re, simdreg.re);
  _mm256_store_ps((float *) im, simdreg.im);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_SSE
  _mm_store_ps((float *) re, simdreg.re);
  _mm_store_ps((float *) im, simdreg.im);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline void srslte_simd_cf_storeu(float *re, float *im, simd_cf_t simdreg) {
#ifdef LV_HAVE_AVX512
  _mm512_storeu_ps(re, simdreg.re);
  _mm512_storeu_ps(im, simdreg.im);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  _mm256_storeu_ps((float *) re, simdreg.re);
  _mm256_storeu_ps((float *) im, simdreg.im);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_SSE
  _mm_storeu_ps((float *) re, simdreg.re);
  _mm_storeu_ps((float *) im, simdreg.im);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_cf_t srslte_simd_cf_set1 (cf_t x) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  ret.re = _mm512_set1_ps(__real__ x);
  ret.im = _mm512_set1_ps(__imag__ x);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  ret.re = _mm256_set1_ps(__real__ x);
  ret.im = _mm256_set1_ps(__imag__ x);
#else
#ifdef LV_HAVE_SSE
  ret.re = _mm_set1_ps(__real__ x);
  ret.im = _mm_set1_ps(__imag__ x);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline simd_cf_t srslte_simd_cf_prod (simd_cf_t a, simd_cf_t b) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  ret.re = _mm512_sub_ps(_mm512_mul_ps(a.re, b.re),
                         _mm512_mul_ps(a.im, b.im));
  ret.im = _mm512_add_ps(_mm512_mul_ps(a.re, b.im),
                         _mm512_mul_ps(a.im, b.re));
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  ret.re = _mm256_sub_ps(_mm256_mul_ps(a.re, b.re),
                         _mm256_mul_ps(a.im, b.im));
  ret.im = _mm256_add_ps(_mm256_mul_ps(a.re, b.im),
                         _mm256_mul_ps(a.im, b.re));
#else
#ifdef LV_HAVE_SSE
  ret.re = _mm_sub_ps(_mm_mul_ps(a.re, b.re),
                      _mm_mul_ps(a.im, b.im));
  ret.im = _mm_add_ps(_mm_mul_ps(a.re, b.im),
                      _mm_mul_ps(a.im, b.re));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline simd_cf_t srslte_simd_cf_conjprod (simd_cf_t a, simd_cf_t b) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  ret.re = _mm512_add_ps(_mm512_mul_ps(a.re, b.re),
                         _mm512_mul_ps(a.im, b.im));
  ret.im = _mm512_sub_ps(_mm512_mul_ps(a.im, b.re),
                         _mm512_mul_ps(a.re, b.im));
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  ret.re = _mm256_add_ps(_mm256_mul_ps(a.re, b.re),
                         _mm256_mul_ps(a.im, b.im));
  ret.im = _mm256_sub_ps(_mm256_mul_ps(a.im, b.re),
                         _mm256_mul_ps(a.re, b.im));
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  ret.re = _mm_add_ps(_mm_mul_ps(a.re, b.re),
                      _mm_mul_ps(a.im, b.im));
  ret.im = _mm_sub_ps(_mm_mul_ps(a.im, b.re),
                      _mm_mul_ps(a.re, b.im));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline simd_cf_t srslte_simd_cf_add (simd_cf_t a, simd_cf_t b) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  ret.re = _mm512_add_ps(a.re, b.re);
  ret.im = _mm512_add_ps(a.im, b.im);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  ret.re = _mm256_add_ps(a.re, b.re);
  ret.im = _mm256_add_ps(a.im, b.im);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  ret.re = _mm_add_ps(a.re, b.re);
  ret.im = _mm_add_ps(a.im, b.im);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline simd_cf_t srslte_simd_cf_zero (void) {
  simd_cf_t ret;
#ifdef LV_HAVE_AVX512
  ret.re = _mm512_setzero_ps();
  ret.im = _mm512_setzero_ps();
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  ret.re = _mm256_setzero_ps();
  ret.im = _mm256_setzero_ps();
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  ret.re = _mm_setzero_ps();
  ret.im = _mm_setzero_ps();
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

#endif /* SRSLTE_SIMD_CF_SIZE */


#if SRSLTE_SIMD_S_SIZE


#ifdef LV_HAVE_AVX512
typedef __m512i simd_s_t;
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
typedef __m256i simd_s_t;
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
typedef __m128i simd_s_t;
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */

static inline simd_s_t srslte_simd_s_load(int16_t *ptr) {
#ifdef LV_HAVE_AVX512
  return _mm512_load_si512(ptr);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_load_si256(ptr);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_load_si128(ptr);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_s_t srslte_simd_s_loadu(int16_t *ptr) {
#ifdef LV_HAVE_AVX512
  return _mm512_load_si512(ptr);
#else /* LV_HAVE_AVX512 */
  #ifdef LV_HAVE_AVX2
  return _mm256_load_si256(ptr);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_load_si128(ptr);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline void srslte_simd_s_store(int16_t *ptr, simd_s_t simdreg) {
#ifdef LV_HAVE_AVX512
  _mm512_store_si512(ptr, simdreg);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  _mm256_store_si256(ptr, simdreg);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  _mm_store_si128(ptr, simdreg);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline void srslte_simd_s_storeu(int16_t *ptr, simd_s_t simdreg) {
#ifdef LV_HAVE_AVX512
  _mm512_storeu_si512(ptr, simdreg);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  _mm256_storeu_si256(ptr, simdreg);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  _mm_storeu_si128(ptr, simdreg);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_s_t srslte_simd_s_zero(void) {
#ifdef LV_HAVE_AVX512
  return _mm512_setzero_si512();
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_setzero_si256();
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_setzero_si128();
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_s_t srslte_simd_s_mul(simd_s_t a, simd_s_t b) {
#ifdef LV_HAVE_AVX512
  return _mm512_mullo_epi16(a, b);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_mullo_epi16(a, b);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_mullo_epi16(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_s_t srslte_simd_s_add(simd_s_t a, simd_s_t b) {
#ifdef LV_HAVE_AVX512
  return _mm512_add_epi16(a, b);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_add_epi16(a, b);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_add_epi16(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

static inline simd_s_t srslte_simd_s_sub(simd_s_t a, simd_s_t b) {
#ifdef LV_HAVE_AVX512
  return _mm512_sub_epi16(a, b);
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  return _mm256_sub_epi16(a, b);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  return _mm_sub_epi16(a, b);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
}

#endif /* SRSLTE_SIMD_S_SIZE */


#if SRSLTE_SIMD_C16_SIZE

typedef struct {
#ifdef LV_HAVE_AVX512
    union {
        __m512i m512;
        int16_t i16[32];
    } re;
    union {
        __m512i m512;
        int16_t i16[32];
    } im;
#else /* LV_HAVE_AVX512 */
#ifdef LV_HAVE_AVX2
  union {
    __m256i m256;
    int16_t i16[16];
  } re;
  union {
    __m256i m256;
    int16_t i16[16];
  } im;
#else
#ifdef LV_HAVE_SSE
  union {
    __m128i m128;
    int16_t i16[8];
  } re;
  union {
    __m128i m128;
    int16_t i16[8];
  } im;
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
} simd_c16_t;

/* Fixed point precision (16-bit) functions */
static inline simd_c16_t srslte_simd_c16i_load(c16_t *ptr) {
  simd_c16_t ret;
#ifdef LV_HAVE_AVX512
  __m512i in1 = _mm512_load_si512((__m512i*)(ptr));
  __m512i in2 = _mm512_load_si512((__m512i*)(ptr + 8));
  ret.re.m512 = _mm512_mask_blend_epi16(0xAAAAAAAA, in1,_mm512_shufflelo_epi16(_mm512_shufflehi_epi16(in2, 0b10100000), 0b10100000));
  ret.im.m512 = _mm512_mask_blend_epi16(0xAAAAAAAA, _mm512_shufflelo_epi16(_mm512_shufflehi_epi16(in1, 0b11110101), 0b11110101),in2);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_AVX2
  __m256i in1 = _mm256_load_si256((__m256i*)(ptr));
  __m256i in2 = _mm256_load_si256((__m256i*)(ptr + 8));
  ret.re.m256 = _mm256_blend_epi16(in1,_mm256_shufflelo_epi16(_mm256_shufflehi_epi16(in2, 0b10100000), 0b10100000), 0b10101010);
  ret.im.m256 = _mm256_blend_epi16(_mm256_shufflelo_epi16(_mm256_shufflehi_epi16(in1, 0b11110101), 0b11110101),in2, 0b10101010);
#else /* LV_HAVE_AVX2 */
#ifdef LV_HAVE_SSE
  __m128i in1 = _mm_load_si128((__m128i*)(ptr));
  __m128i in2 = _mm_load_si128((__m128i*)(ptr + 8));
  ret.re.m128 = _mm_blend_epi16(in1,_mm_shufflelo_epi16(_mm_shufflehi_epi16(in2, 0b10100000), 0b10100000), 0b10101010);
  ret.im.m128 = _mm_blend_epi16(_mm_shufflelo_epi16(_mm_shufflehi_epi16(in1, 0b11110101), 0b11110101),in2, 0b10101010);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
#endif /* LV_HAVE_AVX512 */
  return ret;
}

static inline simd_c16_t srslte_simd_c16_load(int16_t *re, int16_t *im) {
  simd_c16_t ret;
#ifdef LV_HAVE_AVX2
  ret.re.m256 = _mm256_load_si256((__m256i*)(re));
  ret.im.m256 = _mm256_load_si256((__m256i*)(im));
#else
#ifdef LV_HAVE_SSE
  ret.re.m128 = _mm_load_si128((__m128i*)(re));
  ret.im.m128 = _mm_load_si128((__m128i*)(im));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
  return ret;
}

static inline void srslte_simd_c16i_store(c16_t *ptr, simd_c16_t simdreg) {
#ifdef LV_HAVE_AVX2
  __m256i re_sw = _mm256_shufflelo_epi16(_mm256_shufflehi_epi16(simdreg.re.m256, 0b10110001), 0b10110001);
  __m256i im_sw = _mm256_shufflelo_epi16(_mm256_shufflehi_epi16(simdreg.im.m256, 0b10110001), 0b10110001);
  _mm256_store_si256((__m256i *) (ptr), _mm256_blend_epi16(simdreg.re.m256, im_sw, 0b10101010));
  _mm256_store_si256((__m256i *) (ptr + 8), _mm256_blend_epi16(re_sw, simdreg.im.m256, 0b10101010));
#else
#ifdef LV_HAVE_SSE
  __m128i re_sw = _mm_shufflelo_epi16(_mm_shufflehi_epi16(simdreg.re.m128, 0b10110001), 0b10110001);
  __m128i im_sw = _mm_shufflelo_epi16(_mm_shufflehi_epi16(simdreg.im.m128, 0b10110001), 0b10110001);
  _mm_store_si128((__m128i *) (ptr), _mm_blend_epi16(simdreg.re.m128, im_sw, 0b10101010));
  _mm_store_si128((__m128i *) (ptr + 8), _mm_blend_epi16(re_sw, simdreg.im.m128, 0b10101010));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
}

static inline void srslte_simd_c16_store(int16_t *re, int16_t *im, simd_c16_t simdreg) {
#ifdef LV_HAVE_AVX2
  _mm256_store_si256((__m256i *) re, simdreg.re.m256);
  _mm256_store_si256((__m256i *) im, simdreg.im.m256);
#else
#ifdef LV_HAVE_SSE
  _mm_store_si128((__m128i *) re, simdreg.re.m128);
  _mm_store_si128((__m128i *) im, simdreg.im.m128);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
}

static inline simd_c16_t srslte_simd_c16_prod (simd_c16_t a, simd_c16_t b) {
  simd_c16_t ret;
#ifdef LV_HAVE_AVX2
  ret.re.m256 = _mm256_sub_epi16(_mm256_mulhrs_epi16(a.re.m256, _mm256_slli_epi16(b.re.m256, 1)),
                                 _mm256_mulhrs_epi16(a.im.m256, _mm256_slli_epi16(b.im.m256, 1)));
  ret.im.m256 = _mm256_add_epi16(_mm256_mulhrs_epi16(a.re.m256, _mm256_slli_epi16(b.im.m256, 1)),
                                 _mm256_mulhrs_epi16(a.im.m256, _mm256_slli_epi16(b.re.m256, 1)));
#else
#ifdef LV_HAVE_SSE
  ret.re.m128 = _mm_sub_epi16(_mm_mulhrs_epi16(a.re.m128, _mm_slli_epi16(b.re.m128, 1)),
                              _mm_mulhrs_epi16(a.im.m128, _mm_slli_epi16(b.im.m128, 1)));
  ret.im.m128 = _mm_add_epi16(_mm_mulhrs_epi16(a.re.m128, _mm_slli_epi16(b.im.m128, 1)),
                              _mm_mulhrs_epi16(a.im.m128, _mm_slli_epi16(b.re.m128, 1)));
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
  return ret;
}

static inline simd_c16_t srslte_simd_c16_add (simd_c16_t a, simd_c16_t b) {
  simd_c16_t ret;
#ifdef LV_HAVE_AVX2
  ret.re.m256 = _mm256_add_epi16(a.re.m256, b.re.m256);
  ret.im.m256 = _mm256_add_epi16(a.im.m256, b.im.m256);
#else
#ifdef LV_HAVE_SSE
  ret.re.m128 = _mm_add_epi16(a.re.m128, b.re.m128);
  ret.im.m128 = _mm_add_epi16(a.im.m128, b.im.m128);
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
  return ret;
}

static inline simd_c16_t srslte_simd_c16_zero (void) {
  simd_c16_t ret;
#ifdef LV_HAVE_AVX2
  ret.re.m256 = _mm256_setzero_si256();
  ret.im.m256 = _mm256_setzero_si256();
#else
#ifdef LV_HAVE_SSE
  ret.re.m128 = _mm_setzero_si128();
  ret.im.m128 = _mm_setzero_si128();
#endif /* LV_HAVE_SSE */
#endif /* LV_HAVE_AVX2 */
  return ret;
}

#endif /* SRSLTE_SIMD_C16_SIZE */



#endif //SRSLTE_SIMD_H_H
