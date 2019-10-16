/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#ifndef __COMMON_CPP__
#define __COMMON_CPP__

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <wtsapi32.h>
#include <shellscalingapi.h>
#include <commdlg.h>
#include <initguid.h>
#include <comdef.h>
#undef far
#undef near

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

#define m_countof(x) (sizeof(x) / sizeof(x[0]))

#define m_millions(n) (n * 1000000)
#define m_billions(n) (n * 1000000000)
#define m_kilobytes(n) (n * 1024)
#define m_megabytes(n) (n * 1024 * 1024)
#define m_gigabytes(n) (n * 1024 * 1024 * 1024)

#define m_unpack2(a) (a)[0], (a)[1]
#define m_unpack3(a) (a)[0], (a)[1], (a)[2]
#define m_unpack4(a) (a)[0], (a)[1], (a)[2], (a)[3]

#define m_concat_macros(t1, t2) t1##t2
#define m_concat_macros_2(t1, t2) m_concat_macros(t1, t2)

#define m_assert(expr) \
if (!(expr)) { \
	if (IsDebuggerPresent()) { \
		__debugbreak(); \
	} \
	else { \
		char msg[1024]; \
		snprintf(msg, sizeof(msg), "Expr: %s", #expr); \
		DWORD response; \
		char title[] = "Fatal Error"; \
		WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, title, sizeof(title) - 1, msg, (DWORD)strlen(msg), MB_OK, 0, &response, FALSE); \
	} \
	ExitProcess(1); \
}

#ifndef NO_DEBUG_ASSERT
#define m_debug_assert(expr) \
if (!(expr)) { \
	if (IsDebuggerPresent()) { \
		__debugbreak(); \
	} \
	else { \
		char msg[1024]; \
		snprintf(msg, sizeof(msg), "Expr: %s", #expr); \
		DWORD response; \
		char title[] = "Debug Error"; \
		WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, title, sizeof(title) - 1, msg, (DWORD)strlen(msg), MB_OK, 0, &response, FALSE); \
	} \
	ExitProcess(1); \
}
#else
#define m_debug_assert(expr, fmt, ...) (void(0))
#endif

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
void round_up(T* n, T multi) {
	T remainder = *n % multi;
	if (remainder != 0) {
		*n = *n + (multi - remainder);
	}
}

bool is_pow2(uint64 n) {
	return n && !(n & (n - 1));
}

uint32 next_pow2(uint32 n) {
	if (n == 0) {
		return 1;
	}
	n -= 1;
	n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16;
	n += 1;
	return n;
}

uint64 next_pow2(uint64 n) {
	if (n == 0) {
		return 1;
	}
	n -= 1;
	n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16; n |= n >> 32;
	n += 1;
	return n;
}

template <typename F>
struct scope_exit_func {
	F func;
	scope_exit_func(F f) : func(f) {}
	~scope_exit_func() { func(); }
};

template <typename F>
scope_exit_func<F> scope_exit(F f) {
	return scope_exit_func<F>(f);
}

template <typename T>
struct range {
	range(T* first, T* last) : begin_{ first }, end_{ last } {}
	range(T* first, uint64 size) : begin_{ first }, end_{ first + size } {}
	T* begin() const { return begin_; }
	T* end() const { return end_; }
private:
	T* begin_;
	T* end_;
};

template <typename T>
range<T> make_range(T* first, T* last) { return range<T>{first, last}; }

template <typename T>
range<T> make_range(T* first, uint64 size) { return range<T>{first, size}; }

template <typename T>
struct array {
	T* elems;
	uint64 size;
	uint64 capacity;

	T* begin() const { return elems; }
	T* end() const { return elems + size; }

	T& operator[](uint64 index) {
		m_debug_assert(index < size);
		return elems[index];
	}

	T* append(const T& value) {
		if (size < capacity) {
			elems[size] = value;
			size += 1;
			return &elems[size - 1];
		}
		else {
			capacity = max(capacity * 2, 64ull);
			T* new_elems = new T[capacity];
			memcpy(new_elems, elems, size * sizeof(T));
			new_elems[size] = value;
			delete[] elems;
			elems = new_elems;
			size += 1;
			return &new_elems[size - 1];
		}
	}

	void clear() {
		size = 0;
	}

	void remove(uint64 index) {
		m_debug_assert(index < size);
		memmove(elems + index, elems + index + 1, (size - index - 1) * sizeof(T));
		size -= 1;
	}
};

template <typename T, uint32 N>
void array_fill(T(&array)[N], const T& value) {
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
void array_remove(T* array, uint32* array_size, uint32 index) {
	m_debug_assert(*array_size > 0 && index < *array_size);
	memmove(array + index, array + index + 1, (*array_size - index - 1) * sizeof(T));
	*array_size -= 1;
}

template <typename T>
void array_remove_swap_end(T* array, uint32* array_size, uint32 index) {
	m_debug_assert(*array_size > 0 && index < *array_size);
	array[index] = array[*array_size - 1];
	*array_size -= 1;
}

template <typename T>
void array_insert(T* array, uint32* array_size, const T& elem, uint32 insert_index) {
	m_debug_assert(insert_index <= *array_size);
	memmove(array + insert_index + 1, array + insert_index, (*array_size - insert_index) * sizeof(T));
	array[insert_index] = elem;
	*array_size += 1;
}

template <typename T>
void list_prepend(T** list_head, T* new_head) {
	new_head->next = *list_head;
	*list_head = new_head;
}

template <typename T>
void list_append(T** list_head, T* new_tail) {
	if (!*list_head) {
		*list_head = new_tail;
	}
	else {
		T* node = *list_head;
		while (node->next) {
			node = node->next;
		}
		node->next = new_tail;
	}
	new_tail->next = nullptr;
}

template <typename T>
void list_remove(T** list_head, T* node) {
	T* list_node = *list_head;
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

template <typename T>
struct ring_buffer {
	T* buffer;
	uint32 read_index;
	uint32 write_index;
	uint32 size;
	uint32 capacity;
};

template <typename T>
void ring_buffer_write(ring_buffer<T>* rb, T elem) {
	rb->buffer[rb->write_index] = elem;
	if (rb->size == (rb->capacity - 1)) {
		rb->read_index = (rb->read_index + 1) % rb->capacity;
	}
	rb->write_index = (rb->write_index + 1) % rb->capacity;

	if (rb->write_index > rb->read_index) {
		rb->size = rb->write_index - rb->read_index;
	}
	else if (rb->write_index < rb->read_index) {
		rb->size = rb->write_index + rb->capacity - rb->read_index;
	}
	else {
		rb->size = 0;
	}
}

struct string {
	const char* ptr;
	uint32 len;
};

bool operator==(const string& s1, const string& s2) {
	if (s1.len != s2.len) {
		return false;
	}
	else {
		return !memcmp(s1.ptr, s2.ptr, s1.len);
	}
}

bool operator!=(const string& s1, const string& s2) {
	return !(s1 == s2);
}

bool operator==(const string& s1, const char* s2) {
	uint64 s2_len = strlen(s2);
	if (s1.len != s2_len) {
		return false;
	}
	else {
		return !memcmp(s1.ptr, s2, s1.len);
	}
}

bool operator!=(const string& s1, const char* s2) {
	return !(s1 == s2);
}

uint32 murmur3_32(const void* key, uint32 len) {
	uint32 seed = 0xdeadbeef;

	const uint8* data = (const uint8*)key;
	const int nblocks = len / 4;

	uint32 h1 = seed;

	const uint32 c1 = 0xcc9e2d51;
	const uint32 c2 = 0x1b873593;

	const uint32* blocks = (const uint32*)(data + nblocks * 4ll);

	for (int i = -nblocks; i; i += 1) {
		uint32 k1 = blocks[i];
		k1 *= c1;
		k1 = _rotl(k1, 15);
		k1 *= c2;
		h1 ^= k1;
		h1 = _rotl(h1, 13);
		h1 = h1 * 5 + 0xe6546b64;
	}

	const uint8* tail = (const uint8*)(data + nblocks * 4ll);
	uint32 k1 = 0;

	switch (len & 3) {
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1: k1 ^= tail[0];
		k1 *= c1;
		k1 = _rotl(k1, 15);
		k1 *= c2;
		h1 ^= k1;
	};

	h1 ^= len;
	h1 ^= h1 >> 16;
	h1 *= 0x85ebca6b;
	h1 ^= h1 >> 13;
	h1 *= 0xc2b2ae35;
	h1 ^= h1 >> 16;
	return h1;
}

template <typename KEY, typename VALUE>
struct hash_map_elem {
	KEY key;
	VALUE value;
};

template <typename KEY, typename VALUE>
struct hash_map {
	hash_map_elem<KEY, VALUE>* elems;
	VALUE zero_elem_value;
	bool zero_elem_used;
	uint32 capacity;
	uint32 size;
};

bool hash_map_key_is_zero(string s) {
	return s.len == 0;
}

bool hash_map_key_is_zero(uint64 i) {
	return i == 0;
}

template <typename VALUE>
bool hash_map_elem_is_empty(const hash_map_elem<string, VALUE>* elem) {
	return elem->key.ptr == nullptr && elem->key.len == 0;
}

template <typename VALUE>
bool hash_map_elem_is_empty(const hash_map_elem<uint32, VALUE>* elem) {
	return elem->key == 0;
}

template <typename VALUE>
hash_map_elem<string, VALUE>* hash_map_get_first(const hash_map<string, VALUE>* map, string key) {
	uint32 hash = murmur3_32(key.ptr, key.len);
	uint32 index = hash & (map->capacity - 1);
	return map->elems + index;
}

template <typename VALUE>
hash_map_elem<uint32, VALUE>* hash_map_get_first(const hash_map<uint32, VALUE>* map, uint32 key) {
	uint32 hash = murmur3_32(&key, sizeof(key));
	uint32 index = hash & (map->capacity - 1);
	return map->elems + index;
}

template <typename KEY, typename VALUE>
hash_map_elem<KEY, VALUE>* hash_map_get_next(const hash_map<KEY, VALUE>* map, hash_map_elem<KEY, VALUE>* elem) {
	if (elem + 1 != map->elems + map->capacity) {
		return elem + 1;
	}
	else {
		return map->elems;
	}
}

template <typename KEY, typename VALUE>
uint32 hash_map_get_offset(const hash_map<KEY, VALUE>* map, hash_map_elem<KEY, VALUE>* elem1, hash_map_elem<KEY, VALUE>* elem2) {
	ptrdiff_t diff = elem2 - elem1;
	if (diff >= 0) {
		return (uint32)diff;
	}
	else {
		return (uint32)((ptrdiff_t)map->capacity + diff);
	}
}

template <typename KEY, typename VALUE>
void hash_map_initialize(hash_map<KEY, VALUE>* map, uint32 capacity) {
	m_assert(capacity >= 256 && is_pow2(capacity));
	*map = {};
	map->capacity = capacity;
	map->elems = new hash_map_elem<KEY, VALUE>[capacity]();
}

template <typename KEY, typename VALUE>
void hash_map_delete(hash_map<KEY, VALUE>* map) {
	delete[] map->elems;
	*map = {};
}

template <typename KEY, typename VALUE>
void hash_map_resize(hash_map<KEY, VALUE>* map, uint32 new_capacity) {
	m_assert(is_pow2(new_capacity));
	m_assert(map->size * 4 <= new_capacity * 3);

	hash_map_elem<KEY, VALUE>* old_elem_begin = map->elems;
	hash_map_elem<KEY, VALUE>* old_elem_end = map->elems + map->capacity;

	map->capacity = new_capacity;
	map->elems = new hash_map_elem<KEY, VALUE>[new_capacity]();

	for (hash_map_elem<KEY, VALUE>* old_elem = old_elem_begin; old_elem != old_elem_end; old_elem += 1) {
		if (!hash_map_elem_is_empty(old_elem)) {
			for (hash_map_elem<KEY, VALUE>* elem = hash_map_get_first(map, old_elem->key); ; elem = hash_map_get_next(map, elem)) {
				if (hash_map_elem_is_empty(elem)) {
					*elem = *old_elem;
					break;
				}
			}
		}
	}

	delete[] old_elem_begin;
}

template <typename KEY, typename VALUE>
bool hash_map_get(const hash_map<KEY, VALUE>* map, KEY key, VALUE* value) {
	if (hash_map_key_is_zero(key)) {
		if (!map->zero_elem_used) {
			return false;
		}
		else {
			*value = map->zero_elem_value;
			return true;
		}
	}
	else {
		for (hash_map_elem<KEY, VALUE>* elem = hash_map_get_first(map, key); ; elem = hash_map_get_next(map, elem)) {
			if (hash_map_elem_is_empty(elem)) {
				return false;
			}
			else if (elem->key == key) {
				*value = elem->value;
				return true;
			}
		}
	}
}

template <typename KEY, typename VALUE>
bool hash_map_insert(hash_map<KEY, VALUE>* map, KEY key, VALUE value) {
	if (hash_map_key_is_zero(key)) {
		if (map->zero_elem_used) {
			return false;
		}
		else {
			map->zero_elem_value = value;
			map->zero_elem_used = true;
			map->size += 1;
			return true;
		}
	}
	else {
		for (;;) {
			for (hash_map_elem<KEY, VALUE>* elem = hash_map_get_first(map, key); ; elem = hash_map_get_next(map, elem)) {
				if (hash_map_elem_is_empty(elem)) {
					if ((map->size + 1) * 4 >= map->capacity * 3) {
						hash_map_resize(map, map->capacity * 2);
						break;
					}
					else {
						elem->key = key;
						elem->value = value;
						map->size += 1;
						return true;
					}
				}
				else if (elem->key == key) {
					return false;
				}
			}
		}
	}
}

template <typename KEY, typename VALUE>
bool hash_map_remove(hash_map<KEY, VALUE>* map, KEY key) {
	if (hash_map_key_is_zero(key)) {
		if (map->zero_elem_used) {
			map->zero_elem_used = false;
			map->zero_elem_value = {};
			map->size -= 1;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		for (hash_map_elem<KEY, VALUE>* elem = hash_map_get_first(map, key); ; elem = hash_map_get_next(map, elem)) {
			if (hash_map_elem_is_empty(elem)) {
				return false;
			}
			else if (elem->key == key) {
				for (hash_map_elem<KEY, VALUE>* next_elem = hash_map_get_next(map, elem); ; next_elem = hash_map_get_next(map, next_elem)) {
					if (hash_map_elem_is_empty(next_elem)) {
						memset(elem, 0, sizeof(*elem));
						map->size -= 1;
						return true;
					}
					else {
						hash_map_elem<KEY, VALUE>* ideal_elem = hash_map_get_first(map, next_elem->key);
						if (hash_map_get_offset(map, ideal_elem, elem) < hash_map_get_offset(map, ideal_elem, next_elem)) {
							*elem = *next_elem;
							elem = next_elem;
						}
					}
				}
			}
		}
	}
}

const char* get_file_name(const char* path) {
	const char* ptr_0 = strrchr(path, '/');
	const char* ptr_1 = strrchr(path, '\\');
	if (ptr_0 && ptr_1) {
		return ptr_0 > ptr_1 ? ptr_0 : ptr_1;
	}
	else if (ptr_0) {
		return ptr_0 + 1;
	}
	else if (ptr_1) {
		return ptr_1 + 1;
	}
	else {
		return path;
	}
}

struct memory_arena {
	uint8* memory;
	uint64 size;
	uint64 capacity;
};

bool memory_arena_init(uint64 capacity, memory_arena* arena) {
	arena->memory = new uint8[capacity]();
	arena->size = 0;
	arena->capacity = capacity;
	return arena->memory != nullptr;
}

void memory_arena_destroy(memory_arena* arena) {
	delete[] arena->memory;
	*arena = {};
}

template <typename T>
T* memory_arena_alloc(memory_arena* memory_arena, uint64 count, uint64 alignment = alignof(T)) {
	m_debug_assert(is_pow2(alignment));
	if (count == 0) {
		return nullptr;
	}
	uint8* memory = (uint8*)memory_arena->memory + memory_arena->size;
	uint64 remainder = (uintptr_t)memory % alignment;
	uint64 offset = (remainder == 0) ? 0 : (alignment - remainder);
	uint64 new_arena_size = memory_arena->size + offset + count * sizeof(T);
	m_assert(new_arena_size <= memory_arena->capacity);
	memory_arena->size = new_arena_size;
	memset(memory + offset, 0, count * sizeof(T));
	return (T*)(memory + offset);
}

struct memory_arena_undo_alloc_scope_exit {
	memory_arena* arena;
	uint64 restore_size;
	memory_arena_undo_alloc_scope_exit(memory_arena* a) : arena(a), restore_size(a->size) {};
	~memory_arena_undo_alloc_scope_exit() { arena->size = restore_size; }
};

struct memory_pool {
	void* free_block;
	uint64 free_block_count;
	uint64 block_count;
	uint64 block_size;
	uint64 block_alignment;
	uint8* memory;
};

void memory_pool_clear(memory_pool* pool) {
	uint8* free_block = (uint8*)pool->memory;
	for (uint64 i = 0; i < (pool->block_count - 1); i += 1) {
		*(void**)free_block = free_block + pool->block_size;
		free_block += pool->block_size;
	}
	*(void**)free_block = nullptr;
	pool->free_block = pool->memory;
	pool->free_block_count = pool->block_count;
}

bool memory_arena_init(memory_pool* pool, uint64 block_count, uint64 block_size, uint64 block_alignment) {
	m_debug_assert(block_count > 0);
	block_alignment = max(block_alignment, sizeof(void*));
	m_debug_assert(is_pow2(block_alignment));
	round_up(&block_size, block_alignment);
	uint64 memory_size = block_size * block_count;
	pool->block_size = block_size;
	pool->block_count = block_count;
	pool->memory = new uint8[memory_size]();
	if (!pool->memory) {
		return false;
	}
	memory_pool_clear(pool);
	return true;
}

void memory_pool_destroy(memory_pool* pool) {
	delete[] pool->memory;
	*pool = {};
}

template <typename T>
T* memory_pool_alloc(memory_pool* memory_pool) {
	m_debug_assert(sizeof(T) <= memory_pool->block_size);
	T* block = (T*)memory_pool->free_block;
	if (!block) {
		return block;
	}
	memory_pool->free_block = *(T**)block;
	memory_pool->free_block_count -= 1;
	m_debug_assert((uintptr_t)block % alignof(T) == 0);
	return block;
}

void memory_pool_free(memory_pool* memory_pool, void* block) {
	m_debug_assert((uintptr_t)block >= (uintptr_t)memory_pool->memory);
	m_debug_assert((uintptr_t)block < (uintptr_t)memory_pool->memory + memory_pool->block_count * memory_pool->block_size);
	*(void**)block = memory_pool->free_block;
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
	}
	else {
		snprintf(str.data(), 16, "%.1f %s", count, suffixes[s]);
	}
	return str;
}

std::array<char, 256> get_winapi_err_str() {
	std::array<char, 256> str_buf;
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &str_buf[0], (DWORD)str_buf.max_size(), nullptr);
	return str_buf;
}

void show_console() {
	if (!GetConsoleWindow()) {
		AllocConsole();
		m_assert(freopen("CONIN$", "r", stdin));
		m_assert(freopen("CONOUT$", "w", stdout));
		m_assert(freopen("CONOUT$", "w", stderr));
	}
}

struct timer {
	LARGE_INTEGER performance_frequency;
	LARGE_INTEGER performance_counters[2];
};

void timer_init(timer* timer) {
	QueryPerformanceFrequency(&timer->performance_frequency);
}

void timer_start(timer* timer) {
	QueryPerformanceCounter(&timer->performance_counters[0]);
}

void timer_stop(timer* timer) {
	QueryPerformanceCounter(&timer->performance_counters[1]);
}

double timer_get_duration(timer timer) {
	LONGLONG ticks = timer.performance_counters[1].QuadPart - timer.performance_counters[0].QuadPart;
	return (double)ticks / (double)timer.performance_frequency.QuadPart;
}

bool get_current_dir(char* dir, uint32 dir_buf_size) {
	DWORD d = GetCurrentDirectoryA(dir_buf_size, dir);
	return d > 0;
}

bool set_current_dir(char* dir) {
	return SetCurrentDirectoryA(dir);
}

bool set_current_dir_to_exe_dir() {
	char path[512];
	DWORD n = GetModuleFileNameA(nullptr, path, sizeof(path));
	if (n == sizeof(path) && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		return false;
	}
	char* path_ptr = strrchr(path, '\\');
	if (!path_ptr) {
		return false;
	}
	*path_ptr = 0;
	if (!SetCurrentDirectoryA(path)) {
		return false;
	}
	return true;
}

bool file_exists(const char* path) {
	DWORD dwAttrib = GetFileAttributesA(path);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

struct token {
	char* ptr;
	int len;

	bool to_float(float* fp) {
		char c = ptr[len];
		ptr[len] = '\0';
		auto restore_char = scope_exit([&] { ptr[len] = c; });
		char* endptr = nullptr;
		*fp = strtof(ptr, &endptr);
		if (errno == ERANGE) {
			return false;
		}
		if (*fp == 0.0f && endptr == ptr) {
			return false;
		}
		return true;
	}
};

struct file_tokenizer {
	FILE* file;
	size_t file_len;
	char* file_data;
	size_t file_pos;

	bool get_token(token* tk) {
		while (file_pos < file_len && isspace(file_data[file_pos])) {
			file_pos += 1;
		}
		if (file_pos >= file_len) {
			return false;
		}
		size_t tk_pos = file_pos;
		int len = 0;
		while (file_pos < file_len && !isspace(file_data[file_pos])) {
			len += 1;
			file_pos += 1;
		}
		tk->ptr = file_data + tk_pos;
		tk->len = len;
		return true;
	}
};

bool file_tokenizer_init(file_tokenizer* ft, const char* file_name) {
	FILE* file = fopen(file_name, "r");
	if (!file) {
		return false;
	}
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	rewind(file);

	char* file_data = new char[file_size + 1]();

	size_t file_len = fread(file_data, 1, file_size, file);

	if (file_len != file_size && ferror(file)) {
		delete[] file_data;
		fclose(file);
		return false;
	}
	ft->file = file;
	ft->file_len = file_len;
	ft->file_data = file_data;
	ft->file_pos = 0;
	return true;
}

void file_tokenizer_delete(file_tokenizer ft) {
	delete[] ft.file_data;
	fclose(ft.file);
}

struct file_mapping {
	uint8* ptr;
	uint64 size;
	HANDLE mapping_handle;
	HANDLE file_handle;
};

bool file_mapping_create(const char* file_name, uint64 file_size, file_mapping* file_mapping) {
	HANDLE file_handle = CreateFileA(file_name, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file_handle == INVALID_HANDLE_VALUE) {
		return false;
	}
	LARGE_INTEGER size;
	size.QuadPart = file_size;
	if (!SetFilePointerEx(file_handle, size, nullptr, FILE_BEGIN)) {
		CloseHandle(file_handle);
		DeleteFileA(file_name);
		return false;
	}
	if (!SetEndOfFile(file_handle)) {
		CloseHandle(file_handle);
		DeleteFileA(file_name);
		return false;
	}
	HANDLE mapping_handle = CreateFileMappingA(file_handle, nullptr, PAGE_READWRITE, 0, 0, nullptr);
	if (!mapping_handle) {
		CloseHandle(file_handle);
		DeleteFileA(file_name);
		return false;
	}
	uint8* mapping_ptr = (uint8*)MapViewOfFile(mapping_handle, FILE_MAP_WRITE, 0, 0, 0);
	if (!mapping_ptr) {
		CloseHandle(file_handle);
		CloseHandle(mapping_handle);
		DeleteFileA(file_name);
		return false;
	}
	file_mapping->file_handle = file_handle;
	file_mapping->mapping_handle = mapping_handle;
	file_mapping->ptr = mapping_ptr;
	file_mapping->size = file_size;
	return true;
}

bool file_mapping_open(const char* file_name, file_mapping* file_mapping, bool read_only) {
	DWORD access_flags = read_only ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE);
	DWORD share_flags = read_only ? FILE_SHARE_READ : 0;
	HANDLE file_handle = CreateFileA(file_name, access_flags, share_flags, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file_handle == INVALID_HANDLE_VALUE) {
		return false;
	}
	uint64 file_size = GetFileSize(file_handle, nullptr);
	if (file_size == INVALID_FILE_SIZE) {
		CloseHandle(file_handle);
		return false;
	}
	HANDLE mapping_handle = CreateFileMappingA(file_handle, nullptr, read_only ? PAGE_READONLY : PAGE_READWRITE, 0, 0, nullptr);
	if (!mapping_handle) {
		CloseHandle(file_handle);
		return false;
	}
	uint8* mapping_ptr = (uint8*)MapViewOfFile(mapping_handle, read_only ? FILE_MAP_READ : FILE_MAP_WRITE, 0, 0, 0);
	if (!mapping_ptr) {
		CloseHandle(file_handle);
		CloseHandle(mapping_handle);
		return false;
	}
	file_mapping->file_handle = file_handle;
	file_mapping->mapping_handle = mapping_handle;
	file_mapping->ptr = mapping_ptr;
	file_mapping->size = file_size;
	return true;
}

void file_mapping_resize(file_mapping* file_mapping, uint64 file_size) {
	m_assert(UnmapViewOfFile(file_mapping->ptr));
	m_assert(CloseHandle(file_mapping->mapping_handle));

	LARGE_INTEGER size;
	size.QuadPart = file_size;
	m_assert(SetFilePointerEx(file_mapping->file_handle, size, nullptr, FILE_BEGIN));
	m_assert(SetEndOfFile(file_mapping->file_handle));

	file_mapping->mapping_handle = CreateFileMappingA(file_mapping->file_handle, nullptr, PAGE_READWRITE, 0, 0, nullptr);
	m_assert(file_mapping->mapping_handle);

	file_mapping->ptr = (uint8*)MapViewOfFile(file_mapping->mapping_handle, FILE_MAP_WRITE, 0, 0, 0);
	m_assert(file_mapping->ptr);

	file_mapping->size = file_size;
}

void file_mapping_flush(file_mapping file_mapping) {
	m_assert(FlushViewOfFile(file_mapping.ptr, 0));
	m_assert(FlushFileBuffers(file_mapping.file_handle));
}

void file_mapping_close(file_mapping file_mapping) {
	m_assert(UnmapViewOfFile(file_mapping.ptr));
	m_assert(CloseHandle(file_mapping.mapping_handle));
	m_assert(CloseHandle(file_mapping.file_handle));
}

template <typename T>
bool iterate_files_in_dir(const char* dir, T func) {
	char dir_buf[256];
	snprintf(dir_buf, sizeof(dir_buf), "%s\\*", dir);

	WIN32_FIND_DATAA find_data;
	HANDLE find_handle = FindFirstFileA(dir_buf, &find_data);
	m_scope_exit(FindClose(find_handle));
	if (find_handle == INVALID_HANDLE_VALUE) {
		return false;
	}
	do {
		if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			func(find_data.cFileName);
		}
	} while (FindNextFile(find_handle, &find_data) != 0);

	DWORD error = GetLastError();
	if (error != ERROR_NO_MORE_FILES) {
		return false;
	}
	return true;
}

bool open_file_dialog(char* file, uint32 file_buf_size) {
	char cur_dir[512];
	m_assert(get_current_dir(cur_dir, sizeof(cur_dir)));
	auto restore_cur_dir = scope_exit([&] {
		set_current_dir(cur_dir);
		});

	OPENFILENAME open_file_name = {};
	open_file_name.lStructSize = sizeof(open_file_name);
	open_file_name.hwndOwner = GetActiveWindow();
	open_file_name.lpstrFile = file;
	open_file_name.nMaxFile = file_buf_size;
	return GetOpenFileNameA(&open_file_name);
}

bool save_file_dialog(char* file, uint32 file_buf_size) {
	char cur_dir[512];
	m_assert(get_current_dir(cur_dir, sizeof(cur_dir)));
	auto restore_cur_dir = scope_exit([&] {
		set_current_dir(cur_dir);
		});

	OPENFILENAME open_file_name = {};
	open_file_name.lStructSize = sizeof(open_file_name);
	open_file_name.lpstrFile = file;
	open_file_name.nMaxFile = file_buf_size;
	return GetSaveFileNameA(&open_file_name);
}

struct window {
	HWND handle;
	uint32 width;
	uint32 height;
	uint32 screen_width;
	uint32 screen_height;
	int16 mouse_x;
	int16 mouse_y;
	int32 raw_mouse_dx;
	int32 raw_mouse_dy;
};

void window_init(window* window, LRESULT(*window_message_callback)(HWND, UINT, WPARAM, LPARAM)) {
	SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

	HMODULE instance_handle = GetModuleHandle(nullptr);
	WNDCLASSA window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = window_message_callback;
	window_class.hInstance = instance_handle;
	window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszClassName = "agby_window_class";
	m_assert(RegisterClassA(&window_class));

	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	int window_width = (int)(screen_width * 0.7f);
	int window_height = (int)(screen_height * 0.8f);
	int window_x = (int)((screen_width - window_width) * 0.5f);
	int window_y = (int)((screen_height - window_height) * 0.5f);
	DWORD window_style = WS_OVERLAPPEDWINDOW;
	char window_title[128];
	snprintf(window_title, sizeof(window_title), "%dx%d", window_width, window_height);
	HWND window_handle = CreateWindowExA(0, window_class.lpszClassName, window_title, window_style, window_x, window_y, window_width, window_height, nullptr, nullptr, instance_handle, nullptr);
	m_assert(window_handle);

	RAWINPUTDEVICE raw_input_device;
	raw_input_device.usUsagePage = 0x01;
	raw_input_device.usUsage = 0x02;
	raw_input_device.dwFlags = RIDEV_INPUTSINK;
	raw_input_device.hwndTarget = window_handle;
	RegisterRawInputDevices(&raw_input_device, 1, sizeof(raw_input_device));

	RECT window_rect;
	GetClientRect(window_handle, &window_rect);

	*window = {};
	window->handle = window_handle;
	window->width = window_rect.right - window_rect.left;
	window->height = window_rect.bottom - window_rect.top;
	window->screen_width = screen_width;
	window->screen_height = screen_height;
};

void window_handle_messages(window* window) {
	MSG msg;
	while (PeekMessageA(&msg, window->handle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

void window_show(window* window) {
	ShowWindow(window->handle, SW_SHOW);
}

void window_set_title(window* window, const char* fmt, ...) {
	char buf[256];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf(buf, sizeof(buf), fmt, vl);
	va_end(vl);
	SetWindowText(window->handle, buf);
}

void window_set_size(window* window, uint32 width, uint32 height) {
	width = clamp(width, 256u, window->screen_width);
	height = clamp(height, 256u, window->screen_height);
	if (window->width != width || window->height != height) {
		HWND hwnd = window->handle;
		DWORD dw_style = GetWindowLongA(hwnd, GWL_STYLE);
		if (width == window->screen_width && height == window->screen_height) {
			MONITORINFO mi = { sizeof(mi) };
			if (GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
				SetWindowLongA(hwnd, GWL_STYLE, dw_style & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
				window->width = width;
				window->height = height;
			}
		}
		else {
			int window_x = (int)((window->screen_width - width) * 0.5f);
			int window_y = (int)((window->screen_height - height) * 0.5f);
			SetWindowLongA(hwnd, GWL_STYLE, dw_style | WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, window_x, window_y, width, height, SWP_FRAMECHANGED);
			window->width = width;
			window->height = height;
		}
	}
}

bool window_cursor_inside(window* window) {
	return
		(window->mouse_x >= 0) &&
		(window->mouse_x <= (int16)window->width) &&
		(window->mouse_y >= 0) &&
		(window->mouse_y <= (int16)window->height);
}

void cursor_show(bool show) {
	ShowCursor(show);
}

void cursor_pin(bool pin) {
	HWND hwnd = GetActiveWindow();
	if (!hwnd) {
		return;
	}
	if (!pin) {
		ClipCursor(0);
	}
	else {
		POINT cursor_point = {};
		GetCursorPos(&cursor_point);
		RECT rect = {};
		rect.left = cursor_point.x;
		rect.top = cursor_point.y;
		rect.right = rect.left;
		rect.bottom = rect.top;
		ClipCursor(&rect);
	}
}

void flip_image(uint8* image, uint64 w, uint64 h) {
	for (uint64 i = 0; i < (h / 2); i += 1) {
		uint32* row_1 = (uint32*)image + i * w;
		uint32* row_2 = (uint32*)image + (h - 1 - i) * w;
		for (uint64 i = 0; i < w; i += 1) {
			uint32 tmp_pixel = row_1[i];
			row_1[i] = row_2[i];
			row_2[i] = tmp_pixel;
		}
	}
}

void convert_rgba_image_to_bgra(uint8* image, uint64 width, uint64 height) {
	for (uint64 i = 0; i < height; i += 1) {
		uint8* row = image + i * width * 4;
		for (uint64 i = 0; i < width; i += 1) {
			uint8* pixel = row + i * 4;
			uint8 r = pixel[0];
			pixel[0] = pixel[2];
			pixel[2] = r;
		}
	}
}

bool rgba_image_to_bmp_file(void* image, uint32 image_width, uint32 image_height, const char* bmp_file) {
	uint32 image_size = image_width * image_height * 4;

	BITMAPV4HEADER bmp_info_header = {};
	bmp_info_header.bV4Size = sizeof(bmp_info_header);
	bmp_info_header.bV4Width = image_width;
	bmp_info_header.bV4Height = image_height;
	bmp_info_header.bV4Planes = 1;
	bmp_info_header.bV4BitCount = 32;
	bmp_info_header.bV4V4Compression = BI_BITFIELDS;
	bmp_info_header.bV4RedMask = 0x000000ff;
	bmp_info_header.bV4GreenMask = 0x0000ff00;
	bmp_info_header.bV4BlueMask = 0x00ff0000;
	bmp_info_header.bV4AlphaMask = 0xff000000;

	BITMAPFILEHEADER bmp_file_header = {};
	bmp_file_header.bfType = 0x4D42;
	bmp_file_header.bfSize = (DWORD)(sizeof(bmp_file_header) + sizeof(bmp_info_header) + image_size);
	bmp_file_header.bfOffBits = (DWORD)(sizeof(bmp_file_header) + sizeof(bmp_info_header));

	HANDLE file_handle = CreateFileA(bmp_file, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!file_handle) {
		return false;
	}
	else {
		DWORD bytes_written = 0;
		WriteFile(file_handle, (LPSTR)&bmp_file_header, sizeof(bmp_file_header), &bytes_written, nullptr);
		WriteFile(file_handle, (LPSTR)&bmp_info_header, sizeof(bmp_info_header), &bytes_written, nullptr);
		WriteFile(file_handle, (LPSTR)image, image_size, &bytes_written, NULL);
		CloseHandle(file_handle);
		return true;
	}
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

#endif // __COMMON_CPP__
