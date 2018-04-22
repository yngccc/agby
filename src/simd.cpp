/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <tmmintrin.h> 

#include <intrin.h>

#define x (char)(0x80)
alignas(16) static const __m128i simd_left_pack_shuffle_table[16] = {
	_mm_setr_epi8(x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x),        // 0000 - ....
	_mm_setr_epi8(0, 1, 2, 3, x, x, x, x, x, x, x, x, x, x, x, x),        // 1000 - x...
	_mm_setr_epi8(4, 5, 6, 7, x, x, x, x, x, x, x, x, x, x, x, x),        // 0100 - y...
	_mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, x, x, x, x, x, x, x, x),        // 1100 - xy..
	_mm_setr_epi8(8, 9, 10, 11, x, x, x, x, x, x, x, x, x, x, x, x),      // 0010 - z...
	_mm_setr_epi8(0, 1, 2, 3, 8, 9, 10, 11, x, x, x, x, x, x, x, x),      // 1010 - xz..
	_mm_setr_epi8(4, 5, 6, 7, 8, 9, 10, 11, x, x, x, x, x, x, x, x),      // 0110 - yz..
	_mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, x, x, x, x),      // 1110 - xyz.
	_mm_setr_epi8(12, 13, 14, 15, x, x, x, x, x, x, x, x, x, x, x, x),    // 0001 - w...
	_mm_setr_epi8(0, 1, 2, 3, 12, 13, 14, 15, x, x, x, x, x, x, x, x),    // 1001 - xw..
	_mm_setr_epi8(4, 5, 6, 7, 12, 13, 14, 15, x, x, x, x, x, x, x, x),    // 0101 - yw..
	_mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15, x, x, x, x),    // 1101 - xyw.
	_mm_setr_epi8(8, 9, 10, 11, 12, 13, 14, 15, x, x, x, x, x, x, x, x),  // 0011 - zw..
	_mm_setr_epi8(0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15, x, x, x, x),  // 1011 - xzw.
	_mm_setr_epi8(4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, x, x, x, x),  // 0111 - yzw. 
	_mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)   // 1111 - xyzw
};
#undef x

__m128 simd_left_pack(__m128 val, __m128 mask) {
	int index = _mm_movemask_ps(mask);
	__m128i shuffle = _mm_load_si128(&simd_left_pack_shuffle_table[index]);
	__m128i shuffled = _mm_shuffle_epi8(_mm_castps_si128(val), shuffle);
	return _mm_castsi128_ps(shuffled);
}

enum compare_op {
	compare_op_gt,
	compare_op_ge,
	compare_op_eq,
	compare_op_le,
	compare_op_lt
};

uint32 simd_filter_floats(const float *in, float *out, uint32 count, float limit, compare_op cmp) {
  m_debug_assert(count % 4 == 0);
  __m128 limits = _mm_set1_ps(limit);
  uint32 out_offset = 0;
	for (uint32 i = 0; i < count; i += 4) {
		__m128 val = _mm_load_ps(in + i);
		__m128 mask;
		switch (cmp) {
			case compare_op_gt: mask = _mm_cmpgt_ps(val, limits); break;
			case compare_op_ge: mask = _mm_cmpge_ps(val, limits); break;
			case compare_op_eq: mask = _mm_cmpeq_ps(val, limits); break;
			case compare_op_le: mask = _mm_cmple_ps(val, limits); break;
			case compare_op_lt: mask = _mm_cmplt_ps(val, limits); break;
		}
		__m128 result = simd_left_pack(val, mask);
		_mm_storeu_ps(out + out_offset, result);
		out_offset += __popcnt(_mm_movemask_ps(mask));
	}
	return out_offset;
}

