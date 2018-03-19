/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <wtsapi32.h>
#include <shellscalingapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <io.h>
#include <fcntl.h>
#undef far
#undef near

#include "common.cpp"

#define m_line_end "\r\n"
#define m_path_sep '\\'

#define m_last_err_str(str_buf)     \
  char str_buf[512];                \
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), str_buf, sizeof(str_buf), nullptr);

void fatal(const char* fmt, ...) {
	if (IsDebuggerPresent()) {
		__debugbreak();
	}
	else {
		char buffer[2048];
		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, vl);
		va_end(vl);
		DWORD response;
		char error[] = "error";
		WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, error, sizeof(error) - 1, buffer, (DWORD)strlen(buffer), MB_OK, 0, &response, FALSE);
		ExitProcess(1);
	}
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
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, n2, &n3, nullptr);
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
		return false;
	}
	DWORD old_protect;
	if (!VirtualProtect(mem, system_info.dwPageSize, PAGE_NOACCESS, &old_protect) ||
		!VirtualProtect(mem + system_info.dwPageSize + size, system_info.dwPageSize, PAGE_NOACCESS, &old_protect)) {
		VirtualFree(mem, 0, MEM_RELEASE);
		return false;
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
	if (SetFilePointer(file_handle, size.LowPart, &size.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		CloseHandle(file_handle);
		DeleteFile(file_name);
		return false;
	}
	if (!SetEndOfFile(file_handle)) {
		CloseHandle(file_handle);
		DeleteFile(file_name);
		return false;
	}
	HANDLE mapping_handle = CreateFileMappingA(file_handle, nullptr, PAGE_READWRITE, 0, 0, nullptr);
	if (!mapping_handle) {
		CloseHandle(file_handle);
		DeleteFile(file_name);
		return false;
	}
	uint8 *mapping_ptr = (uint8 *)MapViewOfFile(mapping_handle, FILE_MAP_WRITE, 0, 0, 0);
	if (!mapping_ptr) {
		CloseHandle(file_handle);
		CloseHandle(mapping_handle);
		DeleteFile(file_name);
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

void flush_file_mapping(file_mapping file_mapping) {
	m_assert(FlushViewOfFile(file_mapping.ptr, 0));
	m_assert(FlushFileBuffers(file_mapping.file_handle));
}

void close_file_mapping(file_mapping file_mapping) {
	UnmapViewOfFile(file_mapping.ptr);
	CloseHandle(file_mapping.mapping_handle);
	CloseHandle(file_mapping.file_handle);
}

enum window_message_type {
	window_message_type_ignored,
	window_message_type_destroy,
	window_message_type_close,
	window_message_type_quit,
	window_message_type_activate_app,
	window_message_type_size,
	window_message_type_key_down,
	window_message_type_key_up,
	window_message_type_char,
	window_message_type_mouse_move,
	window_message_type_mouse_lb_down,
	window_message_type_mouse_lb_up,
	window_message_type_mouse_rb_down,
	window_message_type_mouse_rb_up,
	window_message_type_mouse_mb_down,
	window_message_type_mouse_mb_up,
	window_message_type_mouse_wheel,
	window_message_type_raw_input
};

const uint16 keycode_backspace = VK_BACK;
const uint16 keycode_tab = VK_TAB;
const uint16 keycode_return = VK_RETURN;
const uint16 keycode_shift = VK_SHIFT;
const uint16 keycode_ctrl = VK_CONTROL;
const uint16 keycode_alt = VK_MENU;
const uint16 keycode_caps = VK_CAPITAL;
const uint16 keycode_esc = VK_ESCAPE;
const uint16 keycode_space = VK_SPACE;
const uint16 keycode_left_shift = VK_LSHIFT;
const uint16 keycode_right_shift = VK_RSHIFT;
const uint16 keycode_left_ctrl = VK_LCONTROL;
const uint16 keycode_right_ctrl = VK_RCONTROL;
const uint16 keycode_left_alt = VK_LMENU;
const uint16 keycode_right_alt = VK_RMENU;
const uint16 keycode_left_win = VK_LWIN;
const uint16 keycode_right_win = VK_RWIN;

const uint16 keycode_grave_accent = VK_OEM_3;
const uint16 keycode_underscore = VK_OEM_MINUS;
const uint16 keycode_equal = VK_OEM_PLUS;
const uint16 keycode_left_bracket = VK_OEM_4;
const uint16 keycode_right_bracket = VK_OEM_6;
const uint16 keycode_backslash = VK_OEM_5;
const uint16 keycode_semi_colon = VK_OEM_1;
const uint16 keycode_apostrophe = VK_OEM_7;
const uint16 keycode_comma = VK_OEM_COMMA;
const uint16 keycode_period = VK_OEM_PERIOD;
const uint16 keycode_slash = VK_OEM_2;

const uint16 keycode_f1 = VK_F1;
const uint16 keycode_f2 = VK_F2;
const uint16 keycode_f3 = VK_F3;
const uint16 keycode_f4 = VK_F4;
const uint16 keycode_f5 = VK_F5;
const uint16 keycode_f6 = VK_F6;
const uint16 keycode_f7 = VK_F7;
const uint16 keycode_f8 = VK_F8;
const uint16 keycode_f9 = VK_F9;
const uint16 keycode_f10 = VK_F10;
const uint16 keycode_f11 = VK_F11;
const uint16 keycode_f12 = VK_F12;

const uint16 keycode_left = VK_LEFT;
const uint16 keycode_up = VK_UP;
const uint16 keycode_right = VK_RIGHT;
const uint16 keycode_down = VK_DOWN;

const uint16 keycode_page_up = VK_PRIOR;
const uint16 keycode_page_down = VK_NEXT;
const uint16 keycode_end = VK_END;
const uint16 keycode_home = VK_HOME;
const uint16 keycode_insert = VK_INSERT;
const uint16 keycode_delete = VK_DELETE;

const uint16 keycode_print_screen = VK_SNAPSHOT;
const uint16 keycode_scroll_lock = VK_SCROLL;

const uint16 keycode_numpad_0 = VK_NUMPAD0;
const uint16 keycode_numpad_1 = VK_NUMPAD1;
const uint16 keycode_numpad_2 = VK_NUMPAD2;
const uint16 keycode_numpad_3 = VK_NUMPAD3;
const uint16 keycode_numpad_4 = VK_NUMPAD4;
const uint16 keycode_numpad_5 = VK_NUMPAD5;
const uint16 keycode_numpad_6 = VK_NUMPAD6;
const uint16 keycode_numpad_7 = VK_NUMPAD7;
const uint16 keycode_numpad_8 = VK_NUMPAD8;
const uint16 keycode_numpad_9 = VK_NUMPAD9;
const uint16 keycode_numpad_add = VK_ADD;
const uint16 keycode_numpad_subtract = VK_SUBTRACT;
const uint16 keycode_numpad_multiply = VK_MULTIPLY;
const uint16 keycode_numpad_divide = VK_DIVIDE;
const uint16 keycode_numpad_decimal = VK_DECIMAL;
const uint16 keycode_numpad_lock = VK_NUMLOCK;

const uint16 keycode_volume_mute = VK_VOLUME_MUTE;
const uint16 keycode_volume_down = VK_VOLUME_DOWN;
const uint16 keycode_volume_up = VK_VOLUME_UP;
const uint16 keycode_media_next = VK_MEDIA_NEXT_TRACK;
const uint16 keycode_media_prev = VK_MEDIA_PREV_TRACK;
const uint16 keycode_media_stop = VK_MEDIA_STOP;
const uint16 keycode_media_play = VK_MEDIA_PLAY_PAUSE;

struct window {
	HWND handle;

	window_message_type msg_type;
	uint32 keycode;
	uint16 input_char;
	int16 mouse_x;
	int16 mouse_y;
	float mouse_wheel;
	int32 raw_mouse_dx;
	int32 raw_mouse_dy;

	bool fullscreen;
	WINDOWPLACEMENT placement;
};

LRESULT window_message_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	struct window *window = (struct window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (msg) {
		case WM_CREATE: {
			struct window *window = (struct window *)(((LPCREATESTRUCT)lparam)->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		} break;
		case WM_PAINT: {
			ValidateRect(hwnd, nullptr);
			window->msg_type = window_message_type_ignored;
		} break;
		case WM_DESTROY: {
			window->msg_type = window_message_type_destroy;
		} break;
		case WM_CLOSE: {
			window->msg_type = window_message_type_close;
		} break;
		case WM_QUIT: {
			window->msg_type = window_message_type_quit;
		} break;
		case WM_ACTIVATEAPP: {
			window->msg_type = window_message_type_activate_app;
		} break;
		case WM_SIZE: {
			window->msg_type = window_message_type_size;
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			window->msg_type = window_message_type_key_down;
			window->keycode = (uint16)wparam;
		} break;
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			window->msg_type = window_message_type_key_up;
			window->keycode = (uint16)wparam;
		} break;
		case WM_CHAR:
		case WM_SYSCHAR: {
			window->msg_type = window_message_type_char;
			window->input_char = (uint16)wparam;
		} break;
		case WM_MOUSEMOVE: {
			window->msg_type = window_message_type_mouse_move;
			window->mouse_x = (int16)LOWORD(lparam);
			window->mouse_y = (int16)HIWORD(lparam);
		} break;
		case WM_LBUTTONDOWN: {
			window->msg_type = window_message_type_mouse_lb_down;
		} break;
		case WM_LBUTTONUP: {
			window->msg_type = window_message_type_mouse_lb_up;
		} break;
		case WM_RBUTTONDOWN: {
			window->msg_type = window_message_type_mouse_rb_down;
		} break;
		case WM_RBUTTONUP: {
			window->msg_type = window_message_type_mouse_rb_up;
		} break;
		case WM_MBUTTONDOWN: {
			window->msg_type = window_message_type_mouse_mb_down;
		} break;
		case WM_MBUTTONUP: {
			window->msg_type = window_message_type_mouse_mb_up;
		} break;
		case WM_MOUSEWHEEL: {
			window->msg_type = window_message_type_mouse_wheel;
			window->mouse_wheel = (float)WHEEL_DELTA / (float)GET_WHEEL_DELTA_WPARAM(wparam);
		} break;
		case WM_INPUT: {
			window->msg_type = window_message_type_raw_input;
			RAWINPUT raw_input = {};
			uint32 raw_input_size = sizeof(raw_input);
			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, &raw_input, &raw_input_size, sizeof(RAWINPUTHEADER));
			if (raw_input.header.dwType == RIM_TYPEMOUSE) {
				RAWMOUSE *raw_mouse = &raw_input.data.mouse;
				if (raw_mouse->usFlags == MOUSE_MOVE_RELATIVE) {
					window->raw_mouse_dx = raw_mouse->lLastX;
					window->raw_mouse_dy = raw_mouse->lLastY;
				}
			}
		} break;
		default: {
			if (window) {
				window->msg_type = window_message_type_ignored;
			}
			// for (uint32 i = 0; i < m_countof(window_message_code_names); i += 1) {
			//   if (window_message_code_names[i].code == msg) {
			//     console("default: %s\n", window_message_code_names[i].name);
			//   }
			// }
			return DefWindowProcA(hwnd, msg, wparam, lparam);
		} break;
	}
	LRESULT result = 0;
	return result;
};

bool initialize_window(window *window) {
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

void set_window_fullscreen(window *window, bool fullscreen) {
	HWND hwnd = window->handle;
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	if (fullscreen && !window->fullscreen) {
		MONITORINFO mi = {sizeof(mi)};
		if (GetWindowPlacement(hwnd, &window->placement) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
			SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			window->fullscreen = true;
		}
	}
	else if (!fullscreen && window->fullscreen) {
		SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hwnd, &window->placement);
		SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		window->fullscreen = false;
	}
}

bool peek_window_message(window *window) {
	MSG msg;
	if (PeekMessage(&msg, window->handle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
		return true;
	}
	return false;
};

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
