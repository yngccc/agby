/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define _ITERATOR_DEBUG_LEVEL 0
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <wtsapi32.h>
#include <shellscalingapi.h>
#include <commdlg.h>
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
#define m_assert(expr, fmt, ...) if (!(expr)) { fatal("Assertion!\n\nExpr: %s\nFile:  %s\nLine: %d\nDetail: " fmt, #expr, __FILE__, __LINE__, __VA_ARGS__); };
#ifdef NO_DEBUG_ASSERT
#define m_debug_assert(expr, fmt, ...) (void(0))
#else
#define m_debug_assert(expr, fmt, ...) if (!(expr)) { fatal("Assertion!\n\nExpr: %s\nFile:  %s\nLine: %d\nDetail: " fmt, #expr, __FILE__, __LINE__, __VA_ARGS__); };
#endif
#endif

void fatal(const char* fmt, ...) {
	char buf[2048];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf(buf, sizeof(buf), fmt, vl);
	va_end(vl);
	if (IsDebuggerPresent()) {
		__debugbreak();
	}
	else {
		DWORD response;
		char title[] = "Fatal Error";
		WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, title, sizeof(title) - 1, buf, (DWORD)strlen(buf), MB_OK, 0, &response, FALSE);
	}
	ExitProcess(1);
}

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
	m_debug_assert(*array_size > 0 && index < *array_size, "");
	memmove(array + index, array + index + 1, (*array_size - index - 1) * sizeof(T));
	*array_size -= 1;
}

template <typename T>
void array_remove_swap_end(T *array, uint32 *array_size, uint32 index) {
	m_debug_assert(*array_size > 0 && index < *array_size, "");
	array[index] = array[*array_size - 1];
	*array_size -= 1;
}

template <typename T>
void array_insert(T *array, uint32 *array_size, const T &elem, uint32 insert_index) {
	m_debug_assert(insert_index <= *array_size, "");
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
	m_debug_assert(num_t > 0 && is_pow_2(alignment), "");
	uint8 *memory = (uint8 *)memory_arena->memory + memory_arena->size;
	uint64 remainder = (uintptr_t)memory % alignment;
	uint64 offset = (remainder == 0) ? 0 : (alignment - remainder);
	uint64 new_arena_size = memory_arena->size + offset + num_t * sizeof(T);
	m_assert(new_arena_size <= memory_arena->capacity, "");
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
	m_debug_assert(memory && (uintptr_t)memory % 16 == 0, "");
	m_debug_assert(block_size >= sizeof(void*) && is_pow_2(block_size), "");
	m_debug_assert(block_count > 0, "");
	m_debug_assert(block_size * block_count <= memory_size, "");

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
	m_debug_assert(memory_pool->free_block && memory_pool->free_block_count > 0, "");
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

std::array<char, 256> get_last_error_string() {
  std::array<char, 256> str_buf;
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &str_buf[0], (DWORD)str_buf.max_size(), nullptr);
  return str_buf;
}

void show_console() {
	if (!GetConsoleWindow()) {
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr); 
  }
}

void console(const char *fmt, ...) {
	char buffer[4096];
	va_list vl;
	va_start(vl, fmt);
	int n1 = vsnprintf(buffer, sizeof(buffer), fmt, vl);
	va_end(vl);
	int n2 = (n1 < (int)sizeof(buffer) - 1) ? n1 : ((int)sizeof(buffer) - 1);
	DWORD n3;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, n2, &n3, nullptr);
}

struct timer {
	LARGE_INTEGER performance_frequency;
	LARGE_INTEGER performance_counters[2];
};

void initialize_timer(timer *timer) {
	QueryPerformanceFrequency(&timer->performance_frequency);
}

void start_timer(timer *timer) {
	QueryPerformanceCounter(&timer->performance_counters[0]);
}

void stop_timer(timer *timer) {
	QueryPerformanceCounter(&timer->performance_counters[1]);
}

uint64 get_timer_duration_microsecs(timer timer) {
	LONGLONG ticks = timer.performance_counters[1].QuadPart - timer.performance_counters[0].QuadPart;
	return (ticks * 1000000) / timer.performance_frequency.QuadPart;
}

double get_timer_duration_secs(timer timer) {
	LONGLONG ticks = timer.performance_counters[1].QuadPart - timer.performance_counters[0].QuadPart;
	return (double)ticks / (double)timer.performance_frequency.QuadPart;
}

bool get_current_dir(char *buffer, uint32 buffer_size) {
	DWORD d = GetCurrentDirectoryA(buffer_size, buffer);
	return d > 0;
}

bool set_exe_dir_as_current() {
	char path[2048];
	DWORD n = GetModuleFileNameA(nullptr, path, sizeof(path));
	if (n == sizeof(path) && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		return false;
	}
	char *path_ptr = strrchr(path, '\\');
	if (!path_ptr) {
		return false;
	}
	*path_ptr = 0;
	if (!SetCurrentDirectoryA(path)) {
		return false;
	}
	return true;
}

void *allocate_virtual_memory(uint64 size) {
	static SYSTEM_INFO system_info = [] {
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		return system_info;
	}();
	size = round_up(size, (uint64)system_info.dwPageSize);
	char *mem = (char *)VirtualAlloc(nullptr, size + 2 * system_info.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!mem) {
		return nullptr;
	}
	DWORD old_protect;
	if (!VirtualProtect(mem, system_info.dwPageSize, PAGE_NOACCESS, &old_protect) ||
		  !VirtualProtect(mem + system_info.dwPageSize + size, system_info.dwPageSize, PAGE_NOACCESS, &old_protect)) {
		VirtualFree(mem, 0, MEM_RELEASE);
		return nullptr;
	}
	return mem + system_info.dwPageSize;
}

void free_virtual_memory(void *memory) {
	static SYSTEM_INFO system_info = [] {
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		return system_info;
	}();
	VirtualFree((char *)memory - system_info.dwPageSize, 0, MEM_RELEASE);
}

bool file_exists(const char *path) {
	DWORD dwAttrib = GetFileAttributesA(path);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

struct file_mapping {
	uint8 *ptr;
	uint64 size;
	HANDLE mapping_handle;
	HANDLE file_handle;
};

bool create_file_mapping(const char *file_name, uint64 file_size, file_mapping *file_mapping) {
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
	uint8 *mapping_ptr = (uint8 *)MapViewOfFile(mapping_handle, FILE_MAP_WRITE, 0, 0, 0);
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

bool open_file_mapping(const char *file_name, file_mapping *file_mapping) {
	HANDLE file_handle = CreateFileA(file_name, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file_handle == INVALID_HANDLE_VALUE) {
		return false;
	}
	uint64 file_size = GetFileSize(file_handle, nullptr);
	if (file_size == INVALID_FILE_SIZE) {
		CloseHandle(file_handle);
		return false;
	}
	HANDLE mapping_handle = CreateFileMappingA(file_handle, nullptr, PAGE_READWRITE, 0, 0, nullptr);
	if (!mapping_handle) {
		CloseHandle(file_handle);
		return false;
	}
	uint8 *mapping_ptr = (uint8 *)MapViewOfFile(mapping_handle, FILE_MAP_WRITE, 0, 0, 0);
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

void resize_file_mapping(file_mapping *file_mapping, uint64 file_size) {
	m_assert(UnmapViewOfFile(file_mapping->ptr), "");
	m_assert(CloseHandle(file_mapping->mapping_handle), "");

	LARGE_INTEGER size;
	size.QuadPart = file_size;
	m_assert(SetFilePointerEx(file_mapping->file_handle, size, nullptr, FILE_BEGIN), "");
	m_assert(SetEndOfFile(file_mapping->file_handle), "");

	file_mapping->mapping_handle = CreateFileMappingA(file_mapping->file_handle, nullptr, PAGE_READWRITE, 0, 0, nullptr);
	m_assert(file_mapping->mapping_handle, "");

	file_mapping->ptr = (uint8 *)MapViewOfFile(file_mapping->mapping_handle, FILE_MAP_WRITE, 0, 0, 0);
	m_assert(file_mapping->ptr, "");
  
  file_mapping->size = file_size;
}

void flush_file_mapping(file_mapping file_mapping) {
	m_assert(FlushViewOfFile(file_mapping.ptr, 0), "");
	m_assert(FlushFileBuffers(file_mapping.file_handle), "");
}

void close_file_mapping(file_mapping *file_mapping) {
	m_assert(UnmapViewOfFile(file_mapping->ptr), "");
	m_assert(CloseHandle(file_mapping->mapping_handle), "");
	m_assert(CloseHandle(file_mapping->file_handle), "");
	*file_mapping = {};
}

bool open_file_dialog(char *file_name_buffer, uint32 file_name_buffer_size) {
	memset(file_name_buffer, 0, file_name_buffer_size);
	OPENFILENAME open_file_name = {};
	open_file_name.lStructSize = sizeof(open_file_name);
	open_file_name.lpstrFile = file_name_buffer;
	open_file_name.nMaxFile = file_name_buffer_size;
	BOOL b = GetOpenFileNameA(&open_file_name);
	set_exe_dir_as_current();
	return b;
}

bool save_file_dialog(char *file_name_buffer, uint32 file_name_buffer_size) {
	memset(file_name_buffer, 0, file_name_buffer_size);
	OPENFILENAME open_file_name = {};
	open_file_name.lStructSize = sizeof(open_file_name);
	open_file_name.lpstrFile = file_name_buffer;
	open_file_name.nMaxFile = file_name_buffer_size;
	BOOL b = GetSaveFileNameA(&open_file_name);
	return b;
}

struct window {
	HWND handle;
	bool fullscreen;
	WINDOWPLACEMENT placement;
	uint32 width;
	uint32 height;
	int16 mouse_x;
	int16 mouse_y;
	int32 raw_mouse_dx;
	int32 raw_mouse_dy;

	// uint32 keycode;
	// uint16 input_char;
	// float mouse_wheel;
};

bool initialize_window(window *window, LRESULT (*message_handle_func)(HWND, UINT, WPARAM, LPARAM)) {
	SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	HMODULE instance_handle = GetModuleHandle(nullptr);
	WNDCLASSA window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = message_handle_func;
	window_class.hInstance = instance_handle;
	window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszClassName = "agby_window_class";
	if (!RegisterClassA(&window_class)) {
		return false;
	}

	int32 window_x = (int32)(GetSystemMetrics(SM_CXSCREEN) * 0.18);
	int32 window_y = (int32)(GetSystemMetrics(SM_CYSCREEN) * 0.15);
	int32 window_width = (int32)(GetSystemMetrics(SM_CXSCREEN) * 0.64);
	int32 window_height = (int32)(GetSystemMetrics(SM_CYSCREEN) * 0.7);
	DWORD window_style = WS_OVERLAPPEDWINDOW;
	char window_title[128];
	snprintf(window_title, sizeof(window_title), "AGBY %dx%d", window_width, window_height);
	HWND window_handle = CreateWindowExA(0, window_class.lpszClassName, window_title, window_style, window_x, window_y, window_width, window_height, nullptr, nullptr, instance_handle, window);
	if (!window_handle) {
		return false;
	}

	RAWINPUTDEVICE raw_input_device;
	raw_input_device.usUsagePage = 0x01;
	raw_input_device.usUsage = 0x02;
	raw_input_device.dwFlags = RIDEV_INPUTSINK;
	raw_input_device.hwndTarget = window_handle;
	RegisterRawInputDevices(&raw_input_device, 1, sizeof(raw_input_device));

	*window = {};
	window->handle = window_handle;
	window->placement = {sizeof(WINDOWPLACEMENT)};
	return true;
};

void show_window(window *window) {
	ShowWindow(window->handle, SW_SHOW);
}

void handle_window_messages(window *window) {
	MSG msg;
	while (PeekMessageA(&msg, window->handle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

void set_window_fullscreen(window *window, bool fullscreen) {
	HWND hwnd = window->handle;
	DWORD dwStyle = GetWindowLongA(hwnd, GWL_STYLE);
	if (fullscreen) {
		MONITORINFO mi = {sizeof(mi)};
		if (GetWindowPlacement(hwnd, &window->placement) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
			SetWindowLongA(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else {
		SetWindowLongA(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hwnd, &window->placement);
		SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	window->fullscreen = fullscreen;
}

void show_cursor(bool show) {
	ShowCursor(show);
}

void pin_cursor(bool pin) {
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

bool rgba_image_to_bmp_file(const char *file_name, uint8 *image, uint32 image_width, uint32 image_height) {
	uint32 image_size = image_width * image_height * 4;

	BITMAPFILEHEADER bmp_file_header = {};
	bmp_file_header.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	bmp_file_header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + image_size;
	bmp_file_header.bfType = 0x4D42;

	BITMAPINFOHEADER bmp_info_header = {};
	bmp_info_header.biSize = sizeof(BITMAPINFOHEADER);
	bmp_info_header.biWidth = image_width;
	bmp_info_header.biHeight = image_height;
	bmp_info_header.biPlanes = 1; 
	bmp_info_header.biBitCount = 32;
	bmp_info_header.biCompression = BI_RGB;
	bmp_info_header.biSizeImage = 0;
	bmp_info_header.biXPelsPerMeter = 0;
	bmp_info_header.biYPelsPerMeter = 0;    
	bmp_info_header.biClrUsed = 0;
	bmp_info_header.biClrImportant = 0;

	HANDLE file_handle = CreateFileA(file_name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!file_handle) {
		return false;
	}
	DWORD bytes_written = 0;
	WriteFile(file_handle, (LPSTR)&bmp_file_header, sizeof(BITMAPFILEHEADER), &bytes_written, nullptr);
	WriteFile(file_handle, (LPSTR)&bmp_info_header, sizeof(BITMAPINFOHEADER), &bytes_written, nullptr);
	WriteFile(file_handle, (LPSTR)image, image_size, &bytes_written, NULL);
	CloseHandle(file_handle);
	return true;
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
