/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define _ITERATOR_DEBUG_LEVEL 0

#include <cassert>
#include <cctype>
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <cfloat>

#include <array>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef unsigned int uint;
typedef long long llong;
typedef unsigned long long ullong;

#define m_countof(x) (sizeof(x) / sizeof(x[0]))

#define m_millions(n) (n * 1000000)
#define m_billions(n) (n * 1000000000)
#define m_kilobytes(n) (n * 1024)
#define m_megabytes(n) (n * 1024 * 1024)
#define m_gigabytes(n) (n * 1024 * 1024 * 1024)

#define m_unpack3(array) (array)[0], (array)[1], (array)[2]
#define m_unpack4(array) (array)[0], (array)[1], (array)[2], (array)[3]

#define m_concat_macros(t1, t2) t1##t2
#define m_concat_macros_2(t1, t2) m_concat_macros(t1, t2)

#ifdef _WIN32
#define m_assert(expr) if (!(expr)) { _wassert(_CRT_WIDE(#expr), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)); };
#ifdef NO_DEBUG_ASSERT
#define m_debug_assert(expr) (void(0))
#else
#define m_debug_assert(expr) if (!(expr)) { _wassert(_CRT_WIDE(#expr), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)); };
#endif
#endif

template <typename F>
struct scope_exit {
	F func;
	scope_exit(F f) : func(f) {}
	~scope_exit() { func(); }
};
template <typename F> scope_exit<F> scope_exit_create(F f) { return scope_exit<F>(f); }
#define m_scope_exit(code) auto m_concat_macros_2(scope_exit_, __LINE__) = scope_exit_create([&] {code;})
#define m_scope_exit_copy(code) auto m_concat_macros_2(scope_exit_, __LINE__) = scope_exit_create([=] {code;})

template <typename T>
T max(T a, T b) {
	return (a < b) ? b : a;
}

template <typename T>
T min(T a, T b) {
	return (a < b) ? a : b;
}

template <typename T>
T clamp(T n, T min, T max) {
	return (n < min) ? min : ((n > max) ? max : n);
}

template <typename T>
T round_up(T n, T multi) {
	T remainder = n % multi;
	if (remainder == 0) {
		return n;
	}
	else {
		return n + (multi - remainder);
	}
}

template <typename T>
void round_up(T *n, T multi) {
	T remainder = *n % multi;
	if (remainder != 0) {
		*n = *n + (multi - remainder);
	}
}

bool is_pow_2(uint64 n) {
	return n && !(n & (n - 1));
}

uint32 next_pow_2(uint32 n) {
	if (n == 0) {
		return 1;
	}
	n -= 1;
	n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16;
	n += 1;
	return n;
}

uint64 next_pow_2(uint64 n) {
	if (n == 0) {
		return 1;
	}
	n -= 1;
	n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16; n |= n >> 32;
	n += 1;
	return n;
}

template <typename T>
struct range {
	range(T *first, T *last) : begin_{first}, end_{last} {}
	range(T *first, uint64 size) : begin_{first}, end_{first + size} {}
	T* begin() const noexcept { return begin_; }
	T* end() const noexcept { return end_; }
	T* begin_;
	T* end_;
};

template <typename T>
range<T> make_range(T *first, uint64 size) noexcept { return range<T>{first, size}; }

template <typename T, uint32 N>
void array_set(T (&array)[N], const T &value) {
	for (uint32 i = 0; i < N; i += 1) {
    array[i] = value;
  }
}

template <typename T, uint32 dst_size, uint32 src_size>
void array_copy(T(&dst_array)[dst_size], const T(&src_array)[src_size]) {
  static_assert(src_size == dst_size, "");
  memcpy(dst_array, src_array, src_size * sizeof(T));
}

template <typename T>
void array_remove(T *array, uint32 *array_size, uint32 index) {
	m_assert(*array_size > 0 && index < *array_size);
	memmove(array + index, array + index + 1, (*array_size - index - 1) * sizeof(T));
	*array_size -= 1;
}

template <typename T>
void array_remove_swap_end(T *array, uint32 *array_size, uint32 index) {
	m_assert(*array_size > 0 && index < *array_size);
	array[index] = array[*array_size - 1];
	*array_size -= 1;
}

template <typename T>
void array_insert(T *array, uint32 *array_size, const T &elem, uint32 insert_index) {
	m_assert(insert_index <= *array_size);
	memmove(array + insert_index + 1, array + insert_index, (*array_size - insert_index) * sizeof(T));
	array[insert_index] = elem;
	*array_size += 1;
}

template <typename T>
void list_prepend(T **list_head, T *new_head) {
	new_head->next = *list_head;
	*list_head = new_head;
}

template <typename T>
void list_append(T **list_head, T *new_tail) {
	if (!*list_head) {
		*list_head = new_tail;
	}
	else {
		T *node = *list_head;
		while (node->next) {
			node = node->next;
		}
		node->next = new_tail;
	}
	new_tail->next = nullptr;
}

template <typename T>
void list_remove(T **list_head, T *node) {
	T *list_node = *list_head;
	if (list_node == node) {
		*list_head = node->next;
	}
	else {
		while (list_node->next != node) {
			list_node = list_node->next;
			if (!list_node) {
				return;
			}
		}
		list_node->next = node->next;
	}
}

struct string {
	char *buf;
	uint64 len;
	uint64 capacity;
};

template <uint32 N>
string string_from_array(char (&array)[N]) {
	return string{array, (uint32)strlen(array), N};
}

void string_cat(string *str, char c) {
	if ((str->capacity - str->len) > 1) {
		str->buf[str->len] = c;
		str->buf[str->len + 1] = '\0';
		str->len += 1;
	}
}

void string_cat(string *str, const char *str2, uint64 str2_len) {
	uint64 n = min(str->capacity - str->len - 1, str2_len);
	memcpy(str->buf + str->len, str2, n);
	str->len += n;
	str->buf[str->len] = '\0';
}

void string_catf(string *str, const char *fmt, ...) {
	uint64 capacity_left = str->capacity - str->len;
	if (capacity_left <= 1) {
		return;
	}
	va_list args;
	va_start(args, fmt);
	int n = vsnprintf(str->buf + str->len, capacity_left, fmt, args);
	va_end(args);
	if (n >= 0) {
		if ((uint32)n < capacity_left) {
			str->len += n;
		}
		else {
			str->len += (capacity_left - 1);
		}
	}
}

void string_pop_back_n(string *str, uint64 n) {
	n = min(str->len, n);
	str->len -= n;
	str->buf[str->len] = '\0';
}

void string_pop_back(string *str, char c) {
	uint64 len = str->len;
	while (len > 0) {
		uint64 i = len - 1;
		if (str->buf[i] == c) {
			str->buf[i + 1] = '\0';
			str->len = len;
			break;
		}
		len -= 1;
	}
}

void string_pop_back(string *str, char c1, char c2) {
	uint64 len = str->len;
	while (len > 0) {
		uint64 i = len - 1;
		if (str->buf[i] == c1 || str->buf[i] == c2) {
			str->buf[i + 1] = '\0';
			str->len = len;
			break;
		}
		len -= 1;
	}
}

struct memory_arena {
	void *memory;
	uint64 size;
	uint64 capacity;
	const char *name;
};

template <typename T>
T *allocate_memory(memory_arena *memory_arena, uint64 num_t, uint64 alignment = alignof(T)) {
	m_debug_assert(num_t > 0 && is_pow_2(alignment));
	uint8 *memory = (uint8 *)memory_arena->memory + memory_arena->size;
	uint64 remainder = (uintptr_t)memory % alignment;
	uint64 offset = (remainder == 0) ? 0 : (alignment - remainder);
	uint64 new_arena_size = memory_arena->size + offset + num_t * sizeof(T);
	m_assert(new_arena_size <= memory_arena->capacity);
	memory_arena->size = new_arena_size;
	memset(memory + offset, 0, num_t * sizeof(T));
	return (T *)(memory + offset);
}

#define m_memory_arena_undo_allocations_at_scope_exit(memory_arena)         \
  const uint64 memory_arena_size_to_restore__ = (memory_arena)->size;       \
  m_scope_exit_copy((memory_arena)->size = memory_arena_size_to_restore__);

struct memory_pool {
	void *free_block;
	uint64 free_block_count;
	uint64 block_count;
	uint64 block_size;
	void *memory;
	const char *name;
};

bool initialize_memory_pool(memory_pool *pool, void *memory, uint64 memory_size, uint64 block_size, uint64 block_count, const char *name) {
	m_assert(memory && (uintptr_t)memory % 16 == 0);
	m_assert(block_size >= sizeof(void*) && is_pow_2(block_size));
	m_assert(block_count > 0);
	m_assert(block_size * block_count <= memory_size);

	char *free_block = (char *)memory;
	for (uint64 i = 0; i < (block_count - 1); i += 1) {
		*(void **)free_block = free_block + block_size;
		free_block = free_block + block_size;
	}
	*(void **)free_block = nullptr;
	pool->free_block = memory;
	pool->free_block_count = block_count;
	pool->block_count = block_count;
	pool->block_size = block_size;
	pool->memory = memory;
	pool->name = name;
	return true;
}

void *allocate_memory(memory_pool *memory_pool) {
	m_assert(memory_pool->free_block && memory_pool->free_block_count > 0);
	void *block = memory_pool->free_block;
	memory_pool->free_block = *(void **)memory_pool->free_block;
	memory_pool->free_block_count -= 1;
	return block;
}

void free_memory(memory_pool *memory_pool, void *block) {
	*(void **)block = memory_pool->free_block;
	memory_pool->free_block = block;
	memory_pool->free_block_count += 1;
}

std::array<char, 16> pretty_print_bytes(uint64 bytes) {
	const char* suffixes[7];
	suffixes[0] = "B";
	suffixes[1] = "KB";
	suffixes[2] = "MB";
	suffixes[3] = "GB";
	suffixes[4] = "TB";
	suffixes[5] = "PB";
	suffixes[6] = "EB";

  uint32 s = 0;
  double count = (double)bytes;
  while (count >= 1024 && s < 7) {
  	s += 1;
  	count /= 1024;
  }
  std::array<char, 16> str;
  if (count - floor(count) == 0.0) {
  	snprintf(str.data(), 16, "%d %s", (int32)count, suffixes[s]);
  } else {
  	snprintf(str.data(), 16, "%.1f %s", count, suffixes[s]);
  }
  return str;
}

// struct profiler_code_frame {
//   const char *name;
//   uint32 level;
//   uint64 per_frame_num_calls[2];
//   uint64 per_frame_call_time_microsec[2];
//   profiler_code_frame *parent;
//   profiler_code_frame *next;
//   profiler_code_frame *children;
// };

// struct profiler {
//   uint32 current_frame_index;
//   profiler_code_frame *current_code_frame;
//   profiler_code_frame *root_code_frame;
//   LARGE_INTEGER performance_frequency;
// };

// template <typename F>
// void profiler_traverse_code_frames(profiler_code_frame *code_frame, F f) {
//   f(code_frame);
//   profiler_code_frame *child_code_frame = code_frame->children;
//   while (child_code_frame) {
//     profiler_traverse_code_frames(child_code_frame, f);
//     child_code_frame = child_code_frame->next;
//   }
// };

// void profiler_swap_frame(profiler *profiler) {
//   uint32 last_frame_index = profiler->current_frame_index == 0 ? 1 : 0;
//   profiler->current_frame_index = last_frame_index;
//   profiler_traverse_code_frames(profiler->current_code_frame, [last_frame_index] (profiler_code_frame *code_frame) {
//     code_frame->per_frame_num_calls[last_frame_index] = 0;
//     code_frame->per_frame_call_time_microsec[last_frame_index] = 0;
//   });
// }

// uint64 profiler_get_last_frame_call_time(profiler *profiler) {
//   return profiler->current_code_frame->per_frame_call_time_microsec[profiler->current_frame_index == 0 ? 1 :0];
// }

// #define m_profiler_begin_code_frame(profiler__, frame_name__)           \
//   {                                                                     \
//     static profiler_code_frame code_frame = {};                         \
//     static int32 code_frame_init = [] (struct profiler *profiler, struct profiler_code_frame *code_frame) { \
//       code_frame->name = frame_name__;                                  \
//       code_frame->level = profiler->current_code_frame->level + 1;      \
//       code_frame->parent = profiler->current_code_frame;                \
//       profiler_code_frame *child_frame = profiler->current_code_frame->children; \
//       if (!child_frame) {                                               \
//         profiler->current_code_frame->children = code_frame;            \
//       }                                                                 \
//       else {                                                            \
//         while (child_frame->next) {                                     \
//           child_frame = child_frame->next;                              \
//         }                                                               \
//         child_frame->next = code_frame;                                 \
//       }                                                                 \
//       return 0;                                                         \
//     }(profiler__, &code_frame);                                         \
//     (profiler__)->current_code_frame = &code_frame;                     \
//   }                                                                     \

// #define m_profiler_end_code_frame(profiler__, frame_time__)                      \
//   {                                                                              \
//     uint32 frame_index = (profiler__)->current_frame_index;                      \
//     (profiler__)->current_code_frame->per_frame_num_calls[frame_index] += 1;     \
//     (profiler__)->current_code_frame->per_frame_call_time_microsec[frame_index] += frame_time__; \
//     (profiler__)->current_code_frame = (profiler__)->current_code_frame->parent; \
//   }

// struct profiler_scope_exit {
//   profiler *profiler;
//   LARGE_INTEGER performance_counters[2];
//   ~profiler_scope_exit() {
//     QueryPerformanceCounter(&performance_counters[1]);
//     uint64 frame_time = (performance_counters[1].QuadPart - performance_counters[0].QuadPart) * 1000000 / profiler->performance_frequency.QuadPart;
//     m_profiler_end_code_frame(profiler, frame_time);
//   }
// };

// #define m_profile_scope(profiler__, frame_name__)                           \
//   m_profiler_begin_code_frame(profiler__, frame_name__)                     \
//   struct profiler_scope_exit profiler_scope_exit = {};                      \
//   profiler_scope_exit.profiler = profiler__;                                \
//   QueryPerformanceCounter(&profiler_scope_exit.performance_counters[0]);
