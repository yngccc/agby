/****************************************************************************************************/
/*			Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.			*/
/****************************************************************************************************/

#include "common.cpp"
#include "math.cpp"
#include "d3d.cpp"
#include "world.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include <imgui/imgui.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_widgets.cpp>
#include <imgui/ImGuizmo.cpp>

struct {
	bool initialized;
	bool quit;
	window *window;
	d3d12 *d3d12;
} window_message_channel = {};

enum edit_window_tab {
	edit_window_tab_player,
	edit_window_tab_static_object,
	edit_window_tab_dynamic_object,
	edit_window_tab_model,
	edit_window_tab_light,
	edit_window_tab_terrain,
	edit_window_tab_skybox,
};

enum selectable_object_type {
	selectable_object_none,
	selectable_object_static_object,
	selectable_object_dynamic_object,
	selectable_object_model,
	selectable_object_direct_light,
	selectable_object_sphere_light
};

const char *edit_window_tab_strs[] = { "Player", "StaticObject", "DynamicObject", "Model", "Light", "Terrain", "Skybox" };

enum tool_type {
	tool_type_select,
	tool_type_translate,
	tool_type_rotate,
	tool_type_scale,

	tool_type_terrain_begin,
	tool_type_terrain_bump,
	tool_type_terrain_raise_lower,
	tool_type_terrain_flatten,
	tool_type_terrain_height,
	tool_type_terrain_ramp,
	tool_type_terrain_smooth,
	tool_type_terrain_noise,
	tool_type_terrain_water,
	tool_type_terrain_hole,
	tool_type_terrain_paint,
	tool_type_terrain_tree,
	tool_type_terrain_road,
	tool_type_terrain_end
};

enum edit_operation_type {
	edit_operation_object_transform
};

enum transformable_type {
	transformable_type_player,
	transformable_type_static_object,
	transformable_type_dynamic_object,
	transformable_type_model
};

struct transform_operation {
	transformable_type transformable_type;
	char id[64];
	transform original_transform;
};

struct edit_operation {
	edit_operation_type type;
	union {
		transform_operation transform_operation;
	};
};

struct editor {
	bool quit;

	timer timer;
	double last_frame_time;
	double coarse_frame_time;
	ring_buffer<float> frame_time_ring_buffer;

	ImGuiContext *imgui_context;

	ID3D12RootSignature *imgui_root_signature;
	ID3D12PipelineState *imgui_pipeline_state;

	ID3D12Resource *imgui_vertex_buffer;
	D3D12_VERTEX_BUFFER_VIEW imgui_vertex_buffer_view;
	uint32 imgui_vertex_buffer_capacity;
	ID3D12Resource *imgui_index_buffer;
	D3D12_INDEX_BUFFER_VIEW imgui_index_buffer_view;
	uint32 imgui_index_buffer_capacity;

	ID3D12DescriptorHeap *imgui_descriptor_heap;

	ID3D12Resource *imgui_font_texture;

	ID3D12Resource *select_icon_texture;
	D3D12_GPU_DESCRIPTOR_HANDLE select_icon_texture_gpu_descriptor_handle;
	ID3D12Resource *translate_icon_texture;
	D3D12_GPU_DESCRIPTOR_HANDLE translate_icon_texture_gpu_descriptor_handle;
	ID3D12Resource *rotate_icon_texture;
	D3D12_GPU_DESCRIPTOR_HANDLE rotate_icon_texture_gpu_descriptor_handle;
	ID3D12Resource *scale_icon_texture;
	D3D12_GPU_DESCRIPTOR_HANDLE scale_icon_texture_gpu_descriptor_handle;

	//ID3D11Texture2D *terrain_bump_icon_texture;
	//ID3D11ShaderResourceView *terrain_bump_icon_texture_view;
	//ID3D11Texture2D *terrain_raise_lower_icon_texture;
	//ID3D11ShaderResourceView *terrain_raise_lower_icon_texture_view;
	//ID3D11Texture2D *terrain_flatten_icon_texture;
	//ID3D11ShaderResourceView *terrain_flatten_icon_texture_view;
	//ID3D11Texture2D *terrain_height_icon_texture;
	//ID3D11ShaderResourceView *terrain_height_icon_texture_view;
	//ID3D11Texture2D *terrain_ramp_icon_texture;
	//ID3D11ShaderResourceView *terrain_ramp_icon_texture_view;
	//ID3D11Texture2D *terrain_smooth_icon_texture;
	//ID3D11ShaderResourceView *terrain_smooth_icon_texture_view;
	//ID3D11Texture2D *terrain_noise_icon_texture;
	//ID3D11ShaderResourceView *terrain_noise_icon_texture_view;
	//ID3D11Texture2D *terrain_water_icon_texture;
	//ID3D11ShaderResourceView *terrain_water_icon_texture_view;
	//ID3D11Texture2D *terrain_hole_icon_texture;
	//ID3D11ShaderResourceView *terrain_hole_icon_texture_view;
	//ID3D11Texture2D *terrain_paint_icon_texture;
	//ID3D11ShaderResourceView *terrain_paint_icon_texture_view;
	//ID3D11Texture2D *terrain_tree_icon_texture;
	//ID3D11ShaderResourceView *terrain_tree_icon_texture_view;
	//ID3D11Texture2D *terrain_road_icon_texture;
	//ID3D11ShaderResourceView *terrain_road_icon_texture_view;

	ID3D11Texture2D **terrain_paint_textures;
	ID3D11ShaderResourceView **terrain_paint_texture_views;
	uint32 terrain_paint_texture_count;
	uint32 terrain_paint_texture_index;

	float top_menu_bar_height;
	ImVec4 top_menu_bar_color;
	ImVec2 objects_window_pos;
	ImVec2 objects_window_size;
	ImVec2 memory_window_pos;
	ImVec2 memory_window_size;
	ImVec2 terrain_brush_properties_window_pos;
	ImVec2 terrain_brush_properties_window_size;

	bool error_popup;
	char error_msg[256];
	bool quit_popup;
	bool add_static_object_popup;
	bool rename_static_object_popup;
	bool add_dynamic_object_popup;
	bool rename_dynamic_object_popup;
	bool new_terrain_popup;

	bool camera_active;
	uint32 camera_move_speed;
	uint32 camera_rotate_speed;
	float camera_fovy;
	float camera_znear;
	float camera_zfar;
	XMVECTOR camera_position;
	XMVECTOR camera_view;
	XMMATRIX camera_view_mat;
	XMMATRIX camera_proj_mat;
	XMMATRIX camera_view_proj_mat;
	ray camera_to_mouse_ray;

	bool render_models;
	bool render_terrain;
	bool render_skybox;
	bool render_reference_grid;
	bool render_shadow_proj_box;

	bool adjust_model;

	bool show_frame_statistic_window;
	bool show_camera_settings_window;

	int32 edit_window_tab;
	uint32 static_object_index;
	uint32 dynamic_object_index;
	uint32 model_index;
	uint32 sphere_light_index;

	selectable_object_type selected_object_type;
	uint32 selected_object_index;

	tool_type tool_type;
	bool terrain_brush_tool_active;
	vec3 terrain_brush_tool_position;
	float terrain_brush_tool_radius;
	float terrain_brush_tool_speed;

	uint64 world_frame_memory_arena_size;
	char world_save_file[256];
	world_editor_settings old_world_editor_settings;

	edit_operation undoes[256];
	uint32 undo_count;

	edit_operation *redoes[256];
	uint32 redo_count;
};

LRESULT window_message_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	LRESULT result = 0;
	if (!window_message_channel.initialized) {
		result = DefWindowProcA(hwnd, msg, wparam, lparam);
	}
	else {
		window *window = window_message_channel.window;
		d3d12 *d3d12 = window_message_channel.d3d12;
		switch (msg) {
		default: {
			result = DefWindowProcA(hwnd, msg, wparam, lparam);
		} break;
		case WM_CLOSE:
		case WM_QUIT: {
			window_message_channel.quit = true;
		} break;
		case WM_PAINT: {
			ValidateRect(hwnd, nullptr);
		} break;
		case WM_SIZE: {
			window->width = LOWORD(lparam);
			window->height = HIWORD(lparam);
			window_set_title(window, "%dx%d", window->width, window->height);
			ImGui::GetIO().DisplaySize = { (float)window->width, (float)window->height };
			ImGuizmo::SetRect(0, 0, (float)window->width, (float)window->height);
			d3d12_resize_swap_chain(d3d12, window->width, window->height);
		} break;
		case WM_SHOWWINDOW: {
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			bool down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
			ImGui::GetIO().KeysDown[wparam] = down;
			if (wparam == VK_SHIFT) {
				ImGui::GetIO().KeyShift = down;
			}
			else if (wparam == VK_CONTROL) {
				ImGui::GetIO().KeyCtrl = down;
			}
			else if (wparam == VK_MENU) {
				ImGui::GetIO().KeyAlt = down;
			}
		} break;
		case WM_CHAR:
		case WM_SYSCHAR: {
			ImGui::GetIO().AddInputCharacter((ImWchar)wparam);
		} break;
		case WM_MOUSEMOVE: {
			window->mouse_x = GET_X_LPARAM(lparam);
			window->mouse_y = GET_Y_LPARAM(lparam);
			if (!window_cursor_inside(window)) {
				ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
			}
			else {
				ImGui::GetIO().MousePos.x = window->mouse_x;
				ImGui::GetIO().MousePos.y = window->mouse_y;
			}
		} break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP: {
			ImGui::GetIO().MouseDown[0] = (msg == WM_LBUTTONDOWN);
		} break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP: {
			ImGui::GetIO().MouseDown[1] = (msg == WM_RBUTTONDOWN);
		} break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP: {
			ImGui::GetIO().MouseDown[2] = (msg == WM_MBUTTONDOWN);
		} break;
		case WM_MOUSEWHEEL: {
			ImGui::GetIO().MouseWheel = (float)WHEEL_DELTA / (float)GET_WHEEL_DELTA_WPARAM(wparam);
		} break;
		case WM_INPUT: {
			RAWINPUT raw_input;
			uint32 raw_input_size = sizeof(raw_input);
			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, &raw_input, &raw_input_size, sizeof(RAWINPUTHEADER));
			if (raw_input.header.dwType == RIM_TYPEMOUSE) {
				RAWMOUSE *raw_mouse = &raw_input.data.mouse;
				if (raw_mouse->usFlags == MOUSE_MOVE_RELATIVE) {
					window->raw_mouse_dx += raw_mouse->lLastX;
					window->raw_mouse_dy += raw_mouse->lLastY;
				}
			}
		} break;
		}
	}
	return result;
}

void load_editor_options(const char *text, int64 len, editor *editor) {
	enum token_type {
		token_identifer,
		token_colon,
		token_integer,
		token_eof,
		token_bad
	};
	struct token {
		token_type type;
		const char *text;
		uint32 len;
	};
	auto get_token = [=](const char **current_text) -> token {
		while (true) {
			if (((*current_text) - text) >= len) {
				return token{ token_eof, nullptr, 0 };
			}
			else if (isspace((*current_text)[0])) {
				(*current_text) += 1;
			}
			else {
				break;
			}
		}
		char c = (*current_text)[0];
		if (c == ':') {
			(*current_text) += 1;
			return token{ token_colon, (*current_text) - 1, 1 };
		}
		else if (isdigit(c)) {
			uint32 l = 1;
			(*current_text) += 1;
			while (((*current_text) - text) < len && (isdigit((*current_text)[0]))) {
				(*current_text) += 1;
				l += 1;
			}
			return token{ token_integer, (*current_text) - l, l };
		}
		else if (isalpha(c)) {
			uint32 l = 1;
			(*current_text) += 1;
			while (((*current_text) - text) < len && (isalnum((*current_text)[0]) || (*current_text)[0] == '_')) {
				(*current_text) += 1;
				l += 1;
			}
			return token{ token_identifer, (*current_text) - l, l };
		}
		else {
			return token{ token_bad, nullptr, 0 };
		}
	};
	auto token_eql = [](token tk, const char *cmp_str) -> bool {
		auto len = strlen(cmp_str);
		return tk.len == len && !memcmp(tk.text, cmp_str, len);
	};
	const char *current_text = text;
	while (true) {
		token t1 = get_token(&current_text);
		token t2 = get_token(&current_text);
		token t3 = get_token(&current_text);
		if (t1.type == token_eof || t2.type == token_eof || t3.type == token_eof) {
			break;
		}
		else if (t1.type != token_identifer || t2.type != token_colon || (t3.type != token_identifer && t3.type != token_integer)) {
			continue;
		}
		else {
			if (token_eql(t1, "camera_move_speed")) {
				char *endptr = nullptr;
				uint32 value = strtoul(t3.text, &endptr, 10);
				if (endptr != t3.text) {
					editor->camera_move_speed = value;
				}
			}
			else if (token_eql(t1, "camera_rotate_speed")) {
				char *endptr = nullptr;
				uint32 value = strtoul(t3.text, &endptr, 10);
				if (endptr != t3.text) {
					editor->camera_rotate_speed = value;
				}
			}
			continue;
		}
	}
}

void save_editor_options(const char *file, editor *editor) {
	char *text = new char[m_megabytes(1)];
	uint32 offset = 0;
	offset += sprintf(text + offset, "camera_move_speed: %u\r\n", editor->camera_move_speed);
	offset += sprintf(text + offset, "camera_rotate_speed: %u\r\n", editor->camera_rotate_speed);
	file_mapping fm;
	if (file_mapping_create(file, offset, &fm)) {
		memcpy(fm.ptr, text, offset);
		file_mapping_flush(fm);
		file_mapping_close(fm);
	}
	delete[]text;
}

void imgui_init(editor *editor, d3d12 *d3d12, window *window) {
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui::GetIO().KeyMap[ImGuiKey_Tab] = VK_TAB;
		ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		ImGui::GetIO().KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		ImGui::GetIO().KeyMap[ImGuiKey_UpArrow] = VK_UP;
		ImGui::GetIO().KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		ImGui::GetIO().KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		ImGui::GetIO().KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		ImGui::GetIO().KeyMap[ImGuiKey_Home] = VK_HOME;
		ImGui::GetIO().KeyMap[ImGuiKey_End] = VK_END;
		ImGui::GetIO().KeyMap[ImGuiKey_Backspace] = VK_BACK;
		ImGui::GetIO().KeyMap[ImGuiKey_Enter] = VK_RETURN;
		ImGui::GetIO().KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		ImGui::GetIO().KeyMap[ImGuiKey_A] = 'A';
		ImGui::GetIO().KeyMap[ImGuiKey_C] = 'C';
		ImGui::GetIO().KeyMap[ImGuiKey_V] = 'V';
		ImGui::GetIO().KeyMap[ImGuiKey_X] = 'X';
		ImGui::GetIO().KeyMap[ImGuiKey_Y] = 'Y';
		ImGui::GetIO().KeyMap[ImGuiKey_Z] = 'Z';
		ImGui::GetIO().IniFilename = nullptr;

		ImGui::GetIO().DisplaySize = { (float)window->width, (float)window->height };
		ImGuizmo::SetRect(0, 0, (float)window->width, (float)window->height);
	}
	{
		uint32 size = m_megabytes(1);

		editor->imgui_vertex_buffer = d3d12_create_buffer(d3d12, size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);
		editor->imgui_vertex_buffer_view.BufferLocation = editor->imgui_vertex_buffer->GetGPUVirtualAddress();
		editor->imgui_vertex_buffer_view.StrideInBytes = sizeof(ImDrawVert);
		editor->imgui_vertex_buffer_view.SizeInBytes = size;
		editor->imgui_vertex_buffer_capacity = size;

		editor->imgui_index_buffer = d3d12_create_buffer(d3d12, size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);
		editor->imgui_index_buffer_view.BufferLocation = editor->imgui_index_buffer->GetGPUVirtualAddress();
		editor->imgui_index_buffer_view.SizeInBytes = size;
		editor->imgui_index_buffer_view.Format = DXGI_FORMAT_R16_UINT;
		editor->imgui_index_buffer_capacity = size;
	}
	{
		D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
		heap_desc.NumDescriptors = 32;
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		m_d3d_assert(d3d12->device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&editor->imgui_descriptor_heap)));
	}
	{
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor_cpu_heap_handle = editor->imgui_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
		D3D12_GPU_DESCRIPTOR_HANDLE descriptor_gpu_heap_handle = editor->imgui_descriptor_heap->GetGPUDescriptorHandleForHeapStart();
		uint32 descriptor_heap_handle_size = d3d12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		{
			uint8 *texture_data;
			int32 texture_width, texture_height;
			m_assert(ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", (float)GetSystemMetrics(SM_CXSCREEN) / 128));
			ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&texture_data, &texture_width, &texture_height);
			auto clear_font_tex_data = scope_exit([] {
				ImGui::GetIO().Fonts->ClearTexData();
			});
			editor->imgui_font_texture = d3d12_create_texture_2d(d3d12, texture_width, texture_height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
			d3d12_copy_texture_2d(d3d12, editor->imgui_font_texture, texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			d3d12->device->CreateShaderResourceView(editor->imgui_font_texture, nullptr, descriptor_cpu_heap_handle);
			ImGui::GetIO().Fonts->SetTexID((ImTextureID)descriptor_gpu_heap_handle.ptr);
			descriptor_cpu_heap_handle.ptr += descriptor_heap_handle_size;
			descriptor_gpu_heap_handle.ptr += descriptor_heap_handle_size;
		}
		auto load_texture = [&](const char *file, DXGI_FORMAT fmt, ID3D12Resource **texture, D3D12_GPU_DESCRIPTOR_HANDLE *texture_gpu_descriptor_handle) {
			int32 width, height, channel;
			uint8 *texture_data = stbi_load(file, &width, &height, &channel, 4);
			m_assert(texture_data);

			*texture = d3d12_create_texture_2d(d3d12, width, height, 1, 1, fmt, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
			d3d12_copy_texture_2d(d3d12, *texture, texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			d3d12->device->CreateShaderResourceView(*texture, nullptr, descriptor_cpu_heap_handle);
			*texture_gpu_descriptor_handle = descriptor_gpu_heap_handle;
			descriptor_cpu_heap_handle.ptr += descriptor_heap_handle_size;
			descriptor_gpu_heap_handle.ptr += descriptor_heap_handle_size;

			stbi_image_free(texture_data);
		};
		load_texture("assets/icons/select.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->select_icon_texture, &editor->select_icon_texture_gpu_descriptor_handle);
		load_texture("assets/icons/translate.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->translate_icon_texture, &editor->translate_icon_texture_gpu_descriptor_handle);
		load_texture("assets/icons/rotate.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->rotate_icon_texture, &editor->rotate_icon_texture_gpu_descriptor_handle);
		load_texture("assets/icons/scale.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->scale_icon_texture, &editor->scale_icon_texture_gpu_descriptor_handle);
	}
	{
		D3D12_DESCRIPTOR_RANGE desc_range = {};
		desc_range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		desc_range.NumDescriptors = 1;
		desc_range.BaseShaderRegister = 0;
		desc_range.RegisterSpace = 0;
		desc_range.OffsetInDescriptorsFromTableStart = 0;

		D3D12_ROOT_PARAMETER param[2] = {};

		param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		param[0].Constants.ShaderRegister = 0;
		param[0].Constants.RegisterSpace = 0;
		param[0].Constants.Num32BitValues = 2;
		param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param[1].DescriptorTable.NumDescriptorRanges = 1;
		param[1].DescriptorTable.pDescriptorRanges = &desc_range;
		param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0.f;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.f;
		sampler.MaxLOD = 0.f;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC sig_desc = {};
		sig_desc.NumParameters = m_countof(param);
		sig_desc.pParameters = param;
		sig_desc.NumStaticSamplers = 1;
		sig_desc.pStaticSamplers = &sampler;
		sig_desc.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		ID3DBlob *sig_blob = nullptr;
		m_d3d_assert(D3D12SerializeRootSignature(&sig_desc, D3D_ROOT_SIGNATURE_VERSION_1, &sig_blob, nullptr));
		d3d12->device->CreateRootSignature(0, sig_blob->GetBufferPointer(), sig_blob->GetBufferSize(), IID_PPV_ARGS(&editor->imgui_root_signature));
		sig_blob->Release();

		D3D12_INPUT_ELEMENT_DESC input_element_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.pRootSignature = editor->imgui_root_signature;
		hlsl_bytecode_file vs_file("hlsl/imgui.vps.vs.bytecode");
		hlsl_bytecode_file ps_file("hlsl/imgui.vps.ps.bytecode");
		pso_desc.VS = vs_file.bytecode;
		pso_desc.PS = ps_file.bytecode;
		pso_desc.BlendState.RenderTarget[0].BlendEnable = true;
		pso_desc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		pso_desc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		pso_desc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		pso_desc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		pso_desc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		pso_desc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		pso_desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		pso_desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		pso_desc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		pso_desc.RasterizerState.DepthClipEnable = true;
		pso_desc.RasterizerState.ForcedSampleCount = 0;
		pso_desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		pso_desc.InputLayout = { input_element_descs, m_countof(input_element_descs) };
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso_desc.SampleDesc.Count = 1;
		m_d3d_assert(d3d12->device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&editor->imgui_pipeline_state)));
	}
}

void imgui_render_commands(editor *editor, d3d12 *d3d12, window *window) {
	ImDrawData *imgui_draw_data = ImGui::GetDrawData();

	uint8 *vertex_buffer = nullptr;
	uint8 *index_buffer = nullptr;
	D3D12_RANGE buffer_range = { 0, 0 };
	m_d3d_assert(editor->imgui_vertex_buffer->Map(0, &buffer_range, (void**)&vertex_buffer));
	m_d3d_assert(editor->imgui_index_buffer->Map(0, &buffer_range, (void**)&index_buffer));

	uint32 vertex_buffer_offset = 0;
	uint32 index_buffer_offset = 0;
	for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
		ImDrawList *dlist = imgui_draw_data->CmdLists[i];
		uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
		uint32 indices_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
		uint32 new_vertex_buffer_offset = vertex_buffer_offset + round_up(vertices_size, (uint32)sizeof(ImDrawVert));
		uint32 new_index_buffer_offset = index_buffer_offset + round_up(indices_size, (uint32)sizeof(ImDrawIdx));
		m_assert(new_vertex_buffer_offset < editor->imgui_vertex_buffer_capacity);
		m_assert(new_index_buffer_offset < editor->imgui_index_buffer_capacity);
		memcpy(vertex_buffer + vertex_buffer_offset, dlist->VtxBuffer.Data, vertices_size);
		memcpy(index_buffer + index_buffer_offset, dlist->IdxBuffer.Data, indices_size);
		vertex_buffer_offset = new_vertex_buffer_offset;
		index_buffer_offset = new_index_buffer_offset;
	}

	editor->imgui_vertex_buffer->Unmap(0, &buffer_range);
	editor->imgui_index_buffer->Unmap(0, &buffer_range);

	uint32 constant_buffer[] = { window->width, window->height };
	float blend_factor[] = { 0.f, 0.f, 0.f, 0.f };

	d3d12->command_list->SetPipelineState(editor->imgui_pipeline_state);
	d3d12->command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d12->command_list->IASetVertexBuffers(0, 1, &editor->imgui_vertex_buffer_view);
	d3d12->command_list->IASetIndexBuffer(&editor->imgui_index_buffer_view);
	d3d12->command_list->OMSetBlendFactor(blend_factor);
	d3d12->command_list->SetGraphicsRootSignature(editor->imgui_root_signature);
	d3d12->command_list->SetGraphicsRoot32BitConstants(0, 2, constant_buffer, 0);
	d3d12->command_list->SetDescriptorHeaps(1, &editor->imgui_descriptor_heap);

	vertex_buffer_offset = 0;
	index_buffer_offset = 0;
	for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
		ImDrawList *dlist = imgui_draw_data->CmdLists[i];
		uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
		uint32 indices_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
		uint32 vertex_index = vertex_buffer_offset / sizeof(ImDrawVert);
		uint32 indice_index = index_buffer_offset / sizeof(ImDrawIdx);
		vertex_buffer_offset += round_up(vertices_size, (uint32)sizeof(ImDrawVert));
		index_buffer_offset += round_up(indices_size, (uint32)sizeof(ImDrawIdx));
		for (int32 i = 0; i < dlist->CmdBuffer.Size; i += 1) {
			ImDrawCmd *dcmd = &dlist->CmdBuffer.Data[i];
			D3D11_RECT scissor = { (int32)dcmd->ClipRect.x, (int32)dcmd->ClipRect.y, (int32)dcmd->ClipRect.z, (int32)dcmd->ClipRect.w };
			d3d12->command_list->RSSetScissorRects(1, &scissor);
			d3d12->command_list->SetGraphicsRootDescriptorTable(1, D3D12_GPU_DESCRIPTOR_HANDLE{ (uint64)dcmd->TextureId });
			d3d12->command_list->DrawIndexedInstanced(dcmd->ElemCount, 1, indice_index, vertex_index, 0);
			indice_index += dcmd->ElemCount;
		}
	}
}

void editor_init(editor *editor, d3d12 *d3d12, window *window) {
	*editor = {};

	timer_init(&editor->timer);

	editor->frame_time_ring_buffer = {};
	editor->frame_time_ring_buffer.capacity = 200;
	editor->frame_time_ring_buffer.buffer = new float[editor->frame_time_ring_buffer.capacity];

	editor->camera_move_speed = 10;
	editor->camera_rotate_speed = 10;
	editor->camera_fovy = degree_to_radian(40);
	editor->camera_znear = 10000.0f;
	editor->camera_zfar = 0.1f;
	editor->camera_position = XMVectorSet(50, 50, 50, 0);
	editor->camera_view = -XMVector3Normalize(editor->camera_position);

	editor->static_object_index = UINT32_MAX;
	editor->dynamic_object_index = UINT32_MAX;
	editor->model_index = UINT32_MAX;

	editor->render_reference_grid = true;
	editor->render_models = true;
	editor->render_terrain = true;
	editor->render_skybox = true;
	editor->render_shadow_proj_box = false;

	editor->terrain_brush_tool_radius = 5;
	editor->terrain_brush_tool_speed = 2;

	imgui_init(editor, d3d12, window);

	{ // options
		file_mapping option_file_mapping = {};
		if (file_mapping_open("editor_options.txt", &option_file_mapping, true)) {
			load_editor_options((const char *)option_file_mapping.ptr, option_file_mapping.size, editor);
			file_mapping_close(option_file_mapping);
		}
	}
	// { // textures
	// 	auto create_texture = [&](const char *file, DXGI_FORMAT fmt, ID3D11Texture2D **texture, ID3D11ShaderResourceView **shader_resource_view) {
	// 		uint8 *texture_data;
	// 		int32 width, height, channel;
	// 		texture_data = stbi_load(file, &width, &height, &channel, 4);
	// 		m_assert(texture_data, "%s", file);
	// 		D3D11_TEXTURE2D_DESC texture_desc = {};
	// 		texture_desc.Width = width;
	// 		texture_desc.Height = height;
	// 		texture_desc.MipLevels = 1;
	// 		texture_desc.ArraySize = 1;
	// 		texture_desc.Format = fmt;
	// 		texture_desc.SampleDesc.Count = 1;
	// 		texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
	// 		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	// 		D3D11_SUBRESOURCE_DATA texture_subresource_data = {texture_data, width * 4u, 0};
	// 		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, texture));
	// 		m_d3d_assert(d3d->device->CreateShaderResourceView(*texture, nullptr, shader_resource_view));
	// 		stbi_image_free(texture_data);
	// 	};

	// 	create_texture("assets/icons/pick.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->pick_icon_texture, &editor->pick_icon_texture_view);
	// 	create_texture("assets/icons/translate.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->translate_icon_texture, &editor->translate_icon_texture_view);
	// 	create_texture("assets/icons/rotate.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->rotate_icon_texture, &editor->rotate_icon_texture_view);
	// 	create_texture("assets/icons/scale.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->scale_icon_texture, &editor->scale_icon_texture_view);
	// 	create_texture("assets/icons/terrain_bump.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_bump_icon_texture, &editor->terrain_bump_icon_texture_view);
	// 	create_texture("assets/icons/terrain_raise_lower.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_raise_lower_icon_texture, &editor->terrain_raise_lower_icon_texture_view);
	// 	create_texture("assets/icons/terrain_flatten.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_flatten_icon_texture, &editor->terrain_flatten_icon_texture_view);
	// 	create_texture("assets/icons/terrain_height.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_height_icon_texture, &editor->terrain_height_icon_texture_view);
	// 	create_texture("assets/icons/terrain_ramp.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_ramp_icon_texture, &editor->terrain_ramp_icon_texture_view);
	// 	create_texture("assets/icons/terrain_smooth.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_smooth_icon_texture, &editor->terrain_smooth_icon_texture_view);
	// 	create_texture("assets/icons/terrain_noise.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_noise_icon_texture, &editor->terrain_noise_icon_texture_view);
	// 	create_texture("assets/icons/terrain_water.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_water_icon_texture, &editor->terrain_water_icon_texture_view);
	// 	create_texture("assets/icons/terrain_hole.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_hole_icon_texture, &editor->terrain_hole_icon_texture_view);
	// 	create_texture("assets/icons/terrain_paint.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_paint_icon_texture, &editor->terrain_paint_icon_texture_view);
	// 	create_texture("assets/icons/terrain_tree.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_tree_icon_texture, &editor->terrain_tree_icon_texture_view);
	// 	create_texture("assets/icons/terrain_road.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_road_icon_texture, &editor->terrain_road_icon_texture_view);

	// 	editor->terrain_paint_texture_count = 0;
	// 	iterate_files_in_dir("assets/terrains/textures", [&](const char *file_name) {
	// 		editor->terrain_paint_texture_count += 1;
	// 	});
	// 	editor->terrain_paint_textures = new ID3D11Texture2D*[editor->terrain_paint_texture_count];
	// 	editor->terrain_paint_texture_views = new ID3D11ShaderResourceView*[editor->terrain_paint_texture_count];
	// 	uint32 index = 0;
	// 	iterate_files_in_dir("assets/terrains/textures", [&](const char *file_name) {
	// 		char path[256];
	// 		snprintf(path, sizeof(path), "assets/terrains/textures/%s", file_name);
	// 		create_texture(path, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, &editor->terrain_paint_textures[index], &editor->terrain_paint_texture_views[index]);
	// 		index += 1;
	// 	});
}

bool editor_load_world(editor *editor, world *world, d3d12 *d3d12, const char *file) {
	if (!world_load_from_file(world, d3d12, file, &editor->old_world_editor_settings)) {
		return false;
	}
	else {
		editor->camera_position = editor->old_world_editor_settings.camera_position;
		editor->camera_view = editor->old_world_editor_settings.camera_view;
		snprintf(editor->world_save_file, sizeof(editor->world_save_file), "%s", file);
		return true;
	}
}

bool editor_save_world(editor *editor, world *world, bool save_as) {
	char world_save_file[256] = {};
	if (save_as) {
		if (!open_file_dialog(world_save_file, sizeof(world_save_file))) {
			return false;
		}
	}
	else {
		if (!strcmp(editor->world_save_file, "")) {
			return false;
		}
		else {
			array_copy(world_save_file, editor->world_save_file);
		}
	}
	world_editor_settings editor_settings = { editor->camera_position, editor->camera_view };
	if (!world_save_to_file(world, world_save_file, &editor_settings)) {
		return false;
	}
	else {
		//for (auto &terrain : world->terrains) {
		//	m_assert(terrain.height_texture_data);
		//	m_assert(terrain.diffuse_texture_data);
		//	file_mapping terrain_file_mapping;
		//	char file_path[128] = "assets/terrains/";
		//	strcat(file_path, terrain.file);
		//	m_assert(file_mapping_open(file_path, &terrain_file_mapping, false));
		//	gpk_terrain *gpk_terrain = (struct gpk_terrain *)terrain_file_mapping.ptr;
		//	uint8 *height_texture_ptr = terrain_file_mapping.ptr + gpk_terrain->height_map_offset;
		//	uint8 *diffuse_texture_ptr = terrain_file_mapping.ptr + gpk_terrain->diffuse_map_offset;
		//	uint32 texture_size = terrain.width * terrain.sample_per_meter * terrain.height * terrain.sample_per_meter;
		//	memcpy(height_texture_ptr, terrain.height_texture_data, texture_size * sizeof(int16));
		//	memcpy(diffuse_texture_ptr, terrain.diffuse_texture_data, texture_size * sizeof(uint32));
		//	file_mapping_flush(terrain_file_mapping);
		//	file_mapping_close(terrain_file_mapping);
		//}
		array_copy(editor->world_save_file, world_save_file);
		return true;
	}
}

bool editor_need_saving(editor *editor, world *world) {
	if (editor->undo_count > 0) {
		return true;
	}
	else if (!XMVector3Equal(editor->camera_position, editor->old_world_editor_settings.camera_position) || !XMVector3Equal(editor->camera_view, editor->old_world_editor_settings.camera_view)) {
		return true;
	}
	else {
		return false;
	}
}

void editor_add_undo(editor *editor, edit_operation operation) {
	if (editor->undo_count >= m_countof(editor->undoes)) {
		uint32 n = 32;
		memmove(editor->undoes, editor->undoes + n, (m_countof(editor->undoes) - n) * sizeof(editor->undoes[0]));
		editor->undo_count = m_countof(editor->undoes) - n;
	}
	editor->undoes[editor->undo_count] = operation;
	editor->undo_count += 1;
}

void editor_pop_undo(editor *editor, world *world) {
	if (editor->undo_count > 0) {
		edit_operation *operation = &editor->undoes[editor->undo_count - 1];
		switch (operation->type) {
		case edit_operation_object_transform: {
			transform_operation *op = &operation->transform_operation;
			switch (op->transformable_type) {
			case transformable_type_player: {
				world->player.transform = op->original_transform;
			} break;
			case transformable_type_static_object: {
				for (auto &object : world->static_objects) {
					if (object.id == op->id) {
						object.transform = op->original_transform;
						break;
					}
				}
			} break;
			case transformable_type_dynamic_object: {
				for (auto &object : world->dynamic_objects) {
					if (object.id == op->id) {
						object.transform = op->original_transform;
						break;
					}
				}
			} break;
			case transformable_type_model: {
				for (auto &model : world->models) {
					if (!strcmp(model.file, op->id)) {
						model.transform = op->original_transform;
						break;
					}
				}
			} break;
			}
		} break;
		}
		editor->undo_count -= 1;
	}
}

void editor_check_quit(editor *editor) {
	if (window_message_channel.quit) {
		window_message_channel.quit = false;
		editor->quit_popup = true;
	}
	if (ImGui::IsKeyPressed(VK_F4) && ImGui::IsKeyDown(VK_MENU)) {
		editor->quit_popup = true;
	}
}

void editor_check_toggle_fullscreen(window *window) {
	if (ImGui::IsKeyPressed(VK_F11)) {
		static uint32 width = window->width;
		static uint32 height = window->height;
		if (window->width == window->screen_width && window->height == window->screen_height) {
			window_set_size(window, width, height);
		}
		else {
			width = window->width;
			height = window->height;
			window_set_size(window, window->screen_width, window->screen_height);
		}
	}
}

void editor_check_popups(editor *editor, world *world, d3d12 *d3d12) {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

	if (editor->error_popup) {
		editor->error_popup = false;
		ImGui::OpenPopup("##error_popup");
	}
	if (ImGui::BeginPopupModal("##error_popup", nullptr, window_flags)) {
		ImGui::Text("%s", editor->error_msg);
		if (ImGui::Button("ok")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (editor->quit_popup) {
		editor->quit_popup = false;
		//if (editor_need_saving(editor, world)) {
		//	ImGui::OpenPopup("##quit_popup");
		//}
		//else {
		//	editor->quit = true;
		//}
		editor_save_world(editor, world, false);
		editor->quit = true;
	}
	if (ImGui::BeginPopupModal("##quit_popup", nullptr, window_flags)) {
		ImGui::Text("Save changes?");
		if (ImGui::Button("Yes")) {
			if (editor_save_world(editor, world, false)) {
				ImGui::CloseCurrentPopup();
				editor->quit = true;
			}
			else {
				snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to save world to file: \"%s\"", editor->world_save_file);
				editor->error_popup = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("No")) {
			ImGui::CloseCurrentPopup();
			editor->quit = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (editor->add_static_object_popup) {
		editor->add_static_object_popup = false;
		ImGui::OpenPopup("##add_static_object_popup");
	}
	if (ImGui::BeginPopupModal("##add_static_object_popup", nullptr, window_flags)) {
		static char id[32] = {};
		static bool error = false;
		static char error_msg[128] = {};
		auto close_popup = [&] {
			id[0] = 0;
			error = false;
			error_msg[0] = 0;
			ImGui::CloseCurrentPopup();
		};
		ImGui::InputText("ID", id, sizeof(id));
		if (error) {
			ImGui::TextColored({ 1, 0, 0, 1 }, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (auto &object : world->static_objects) {
				if (object.id == id) {
					duplicate = true;
					break;
				}
			}
			if (empty_id) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID is empty");
			}
			else if (duplicate) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID already exist");
			}
			else {
				editor->static_object_index = (uint32)world->static_objects.size();
				world->static_objects.push_back(static_object{});
				static_object *static_object = &world->static_objects.back();
				static_object->model_index = UINT32_MAX;
				static_object->transform = transform_identity();
				static_object->animation_index = UINT32_MAX;
				static_object->animation_time = 0;
				char *id_cstr = new char[strlen(id) + 1]();
				strcpy(id_cstr, id);
				static_object->id = { id_cstr, (uint32)strlen(id) };
				close_popup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			close_popup();
		}
		ImGui::EndPopup();
	}

	if (editor->rename_static_object_popup) {
		editor->rename_static_object_popup = false;
		ImGui::OpenPopup("##rename_static_object_popup");
	}
	if (ImGui::BeginPopupModal("##rename_static_object_popup", nullptr, window_flags)) {
		static char id[32] = {};
		static bool error = false;
		static char error_msg[128] = {};
		ImGui::InputText("ID", id, sizeof(id));
		auto close_popup = [&] {
			id[0] = 0;
			error = false;
			error_msg[0] = 0;
			ImGui::CloseCurrentPopup();
		};
		if (error) {
			ImGui::TextColored({ 1, 0, 0, 1 }, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (uint32 i = 0; i < world->static_objects.size(); i += 1) {
				if (world->static_objects[i].id == id && i != editor->static_object_index) {
					duplicate = true;
					break;
				}
			}
			if (empty_id) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID is empty");
			}
			else if (duplicate) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID already exist");
			}
			else {
				static_object *static_object = &world->static_objects[editor->static_object_index];
				char *id_cstr = new char[strlen(id) + 1]();
				strcpy(id_cstr, id);
				static_object->id = { id_cstr, (uint32)strlen(id) };
				close_popup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			close_popup();
		}
		ImGui::EndPopup();
	}

	if (editor->add_dynamic_object_popup) {
		editor->add_dynamic_object_popup = false;
		ImGui::OpenPopup("##add_dynamic_object_popup");
	}
	if (ImGui::BeginPopupModal("##add_dynamic_object_popup", nullptr, window_flags)) {
		static char id[32] = {};
		static bool error = false;
		static char error_msg[128] = {};
		auto close_popup = [&] {
			id[0] = 0;
			error = false;
			error_msg[0] = 0;
			ImGui::CloseCurrentPopup();
		};
		ImGui::InputText("ID", id, sizeof(id));
		if (error) {
			ImGui::TextColored({ 1, 0, 0, 1 }, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (auto &object : world->dynamic_objects) {
				if (object.id == id) {
					duplicate = true;
					break;
				}
			}
			if (empty_id) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID is empty");
			}
			else if (duplicate) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID already exist");
			}
			else {
				editor->dynamic_object_index = (uint32)world->dynamic_objects.size();
				world->dynamic_objects.push_back(dynamic_object{});
				dynamic_object *dynamic_object = &world->dynamic_objects.back();
				dynamic_object->model_index = UINT32_MAX;
				dynamic_object->transform = transform_identity();
				dynamic_object->animation_index = UINT32_MAX;
				dynamic_object->animation_time = 0;
				char *id_cstr = new char[strlen(id) + 1]();
				strcpy(id_cstr, id);
				dynamic_object->id = { id_cstr, (uint32)strlen(id_cstr) };
				close_popup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			close_popup();
		}
		ImGui::EndPopup();
	}

	if (editor->rename_dynamic_object_popup) {
		editor->rename_dynamic_object_popup = false;
		ImGui::OpenPopup("##rename_dynamic_object_popup");
	}
	if (ImGui::BeginPopupModal("##rename_dynamic_object_popup", nullptr, window_flags)) {
		static char id[32] = {};
		static bool error = false;
		static char error_msg[128] = {};
		ImGui::InputText("ID", id, sizeof(id));
		auto close_popup = [&] {
			id[0] = 0;
			error = false;
			error_msg[0] = 0;
			ImGui::CloseCurrentPopup();
		};
		if (error) {
			ImGui::TextColored({ 1, 0, 0, 1 }, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (uint32 i = 0; i < world->dynamic_objects.size(); i += 1) {
				if (world->dynamic_objects[i].id == id && i != editor->dynamic_object_index) {
					duplicate = true;
					break;
				}
			}
			if (empty_id) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID is empty");
			}
			else if (duplicate) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID already exist");
			}
			else {
				dynamic_object *dynamic_object = &world->dynamic_objects[editor->dynamic_object_index];
				char *id_cstr = new char[strlen(id) + 1]();
				strcpy(id_cstr, id);
				dynamic_object->id = { id_cstr, (uint32)strlen(id_cstr) };
				close_popup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			close_popup();
		}
		ImGui::EndPopup();
	}

	if (editor->new_terrain_popup) {
		editor->new_terrain_popup = false;
		ImGui::OpenPopup("##new_terrain_popup");
	}
	if (ImGui::BeginPopupModal("##new_terrain_popup", nullptr, window_flags)) {
		static char file[256] = {};
		static bool error = false;
		static char error_msg[128] = {};
		static bool yes_to_replace_file = false;
		static int32 width = 256;
		static int32 height = 256;
		static int32 sample_per_meter = 4;
		static float max_height = 128;
		auto close_popup = [&] {
			file[0] = 0;
			error = false;
			error_msg[0] = 0;
			yes_to_replace_file = false;
			ImGui::CloseCurrentPopup();
		};
		auto new_terrain = [&] {
			gpk_terrain terrain = {};
			strcpy(terrain.format_str, m_gpk_terrain_format_str);
			terrain.width = width;
			terrain.height = height;
			terrain.max_height = max_height;
			terrain.sample_per_meter = sample_per_meter;
			uint32 texture_size = terrain.width * terrain.height * terrain.sample_per_meter * terrain.sample_per_meter;
			terrain.height_map_offset = round_up((uint32)sizeof(struct gpk_terrain), 16u);
			terrain.diffuse_map_offset = round_up(terrain.height_map_offset + texture_size * (uint32)sizeof(int16), 16u);
			terrain.total_size = terrain.diffuse_map_offset + texture_size * sizeof(uint32);
			file_mapping terrain_file_mapping;
			if (!file_mapping_create(file, terrain.total_size, &terrain_file_mapping)) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "Error: create_file_mapping failed\nFile: %s", file);
			}
			else {
				*(gpk_terrain *)terrain_file_mapping.ptr = terrain;
				int16 *height_map = (int16 *)(terrain_file_mapping.ptr + terrain.height_map_offset);
				uint8 *diffuse_map = (uint8 *)(terrain_file_mapping.ptr + terrain.diffuse_map_offset);
				memset(height_map, 0, texture_size * sizeof(int16));
				memset(diffuse_map, 150, texture_size * sizeof(uint32));
				file_mapping_flush(terrain_file_mapping);
				file_mapping_close(terrain_file_mapping);
				//add_terrain(world, d3d, file);
				close_popup();
			}
		};
		if (error) {
			ImGui::TextColored({ 1, 0, 0, 1 }, "%s", error_msg);
		}
		ImGui::InputText("File", file, sizeof(file));
		ImGui::SameLine();
		if (ImGui::Button("Browse")) {
			open_file_dialog(file, sizeof(file));
		}
		ImGui::InputInt("width", &width);
		ImGui::InputInt("height", &height);
		ImGui::InputInt("sample_per_meter", &sample_per_meter);
		ImGui::InputFloat("max_height", &max_height);
		if (ImGui::Button("Ok")) {
			if (file_exists(file)) {
				ImGui::OpenPopup("##replace_file_popup");
			}
			else {
				new_terrain();
			}
		}
		if (yes_to_replace_file) {
			yes_to_replace_file = false;
			new_terrain();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			close_popup();
		}
		if (ImGui::BeginPopupModal("##replace_file_popup", nullptr, window_flags)) {
			ImGui::Text("File alread exists, replace?");
			if (ImGui::Button("Yes")) {
				yes_to_replace_file = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No")) {
				file[0] = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}
}

void editor_top_menu(editor *editor, world *world, d3d12 *d3d12) {
	ImGui::PushID("top_menu_bar");
	if (ImGui::BeginMainMenuBar()) {
		editor->top_menu_bar_height = ImGui::GetWindowHeight();
		editor->top_menu_bar_color = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);
		ImGui::PushID("file");
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open")) {
				if (open_file_dialog(editor->world_save_file, sizeof(editor->world_save_file))) {
					if (!editor_load_world(editor, world, d3d12, editor->world_save_file)) {
						snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to load level from file: \"%s\"", editor->world_save_file);
						editor->error_popup = true;
						editor->world_save_file[0] = '\0';
					}
				}
			}
			if (ImGui::MenuItem("Save")) {
				if (!editor_save_world(editor, world, false)) {
					snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to save world to file: \"%s\"", editor->world_save_file);
					editor->error_popup = true;
				}
			}
			if (ImGui::MenuItem("Save As")) {
				if (!editor_save_world(editor, world, true)) {
					snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to save world to file");
					editor->error_popup = true;
				}
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Play")) {
				if (!editor_save_world(editor, world, false)) {
					snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to save world to file: \"%s\"", editor->world_save_file);
					editor->error_popup = true;
				}
				else {
					STARTUPINFOA startup_info = { sizeof(startup_info) };
					PROCESS_INFORMATION process_info;
					char cmd_line[256] = {};
					snprintf(cmd_line, sizeof(cmd_line), "game.exe %s", editor->world_save_file);
					if (!CreateProcessA("game.exe", cmd_line, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &process_info)) {
						snprintf(editor->error_msg, sizeof(editor->error_msg), "cannot start game.exe: %s", &get_winapi_err_str()[0]);
						editor->error_popup = true;
					}
					else {
						CloseHandle(process_info.hThread);
						CloseHandle(process_info.hProcess);
					}
				}
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit")) {
				editor->quit_popup = true;
			}
			ImGui::EndMenu();
		}
		ImGui::PopID();
		ImGui::PushID("view");
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Frame stat window", nullptr, &editor->show_frame_statistic_window);
			ImGui::Separator();
			ImGui::MenuItem("Models", nullptr, &editor->render_models);
			ImGui::MenuItem("Terrain", nullptr, &editor->render_terrain);
			ImGui::MenuItem("Skybox", nullptr, &editor->render_skybox);
			ImGui::Separator();
			ImGui::MenuItem("Reference Grid", nullptr, &editor->render_reference_grid);
			ImGui::MenuItem("ShadowProjBox", nullptr, &editor->render_shadow_proj_box);
			ImGui::EndMenu();
		}
		ImGui::PopID();
		ImGui::PushID("options");
		if (ImGui::BeginMenu("Options")) {
			ImGui::PushID("camera");
			if (ImGui::BeginMenu("Camera")) {
				uint32 min = 0;
				uint32 max = 100;
				ImGui::SliderScalar("Move Speed", ImGuiDataType_U32, &editor->camera_move_speed, &min, &max);
				ImGui::SliderScalar("Rotate Speed", ImGuiDataType_U32, &editor->camera_rotate_speed, &min, &max);
				ImGui::EndMenu();
			}
			ImGui::PopID();
			ImGui::EndMenu();
		}
		ImGui::PopID();

		ImGui::EndMainMenuBar();
	}
	ImGui::PopID();

}

void editor_bottom_menu(editor *editor) {
	float menu_height = editor->top_menu_bar_height * 1.5f;
	ImGui::SetNextWindowPos(ImVec2{ 0, ImGui::GetIO().DisplaySize.y - menu_height });
	ImGui::SetNextWindowSize(ImVec2{ ImGui::GetIO().DisplaySize.x, menu_height });
	ImGui::PushID("bottom_menu_bar");
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, editor->top_menu_bar_color);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	if (ImGui::Begin("##bottom_menu_bar", nullptr, flags)) {
		ImGui::Dummy({ ImGui::GetIO().DisplaySize.x / 2, 1 });
		ImGui::SameLine();
		float image_size = menu_height * 0.8f;
		float image_padding = menu_height * 0.1f;
		ImVec4 button_hovered_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.8f, 0.8f, 1 });

		auto render_tool_button = [&](tool_type type, D3D12_GPU_DESCRIPTOR_HANDLE texture_gpu_descriptor_handle) {
			ImGui::SameLine();
			if (editor->tool_type == type) {
				ImGui::PushStyleColor(ImGuiCol_Button, button_hovered_color);
				if (ImGui::ImageButton((ImTextureID)texture_gpu_descriptor_handle.ptr, { image_size, image_size }, { 0, 0 }, { 1, 1 }, (int32)image_padding)) {
					editor->tool_type = type;
				}
				ImGui::PopStyleColor();
			}
			else {
				if (ImGui::ImageButton((ImTextureID)texture_gpu_descriptor_handle.ptr, { image_size, image_size }, { 0, 0 }, { 1, 1 }, (int32)image_padding)) {
					editor->tool_type = type;
				}
			}
		};
		render_tool_button(tool_type_select, editor->select_icon_texture_gpu_descriptor_handle);
		render_tool_button(tool_type_translate, editor->translate_icon_texture_gpu_descriptor_handle);
		render_tool_button(tool_type_rotate, editor->rotate_icon_texture_gpu_descriptor_handle);
		render_tool_button(tool_type_scale, editor->scale_icon_texture_gpu_descriptor_handle);

		//render_tool_button(tool_type_terrain_bump, editor->terrain_bump_icon_texture_view);
		//render_tool_button(tool_type_terrain_raise_lower, editor->terrain_raise_lower_icon_texture_view);
		//render_tool_button(tool_type_terrain_flatten, editor->terrain_flatten_icon_texture_view);
		//render_tool_button(tool_type_terrain_height, editor->terrain_height_icon_texture_view);
		//render_tool_button(tool_type_terrain_ramp, editor->terrain_ramp_icon_texture_view);
		//render_tool_button(tool_type_terrain_smooth, editor->terrain_smooth_icon_texture_view);
		//render_tool_button(tool_type_terrain_noise, editor->terrain_noise_icon_texture_view);
		//render_tool_button(tool_type_terrain_water, editor->terrain_water_icon_texture_view);
		//render_tool_button(tool_type_terrain_hole, editor->terrain_hole_icon_texture_view);
		//render_tool_button(tool_type_terrain_paint, editor->terrain_paint_icon_texture_view);
		//render_tool_button(tool_type_terrain_tree, editor->terrain_tree_icon_texture_view);
		//render_tool_button(tool_type_terrain_road, editor->terrain_road_icon_texture_view);

		ImGui::PopStyleColor();
	}
	ImGui::End();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(1);
	ImGui::PopID();
}

void editor_edit_window_model_transform(world *world, uint32 *model_index, transform *transform) {
	const char *model_file = *model_index < world->models.size() ? world->models[*model_index].file : nullptr;
	if (ImGui::BeginCombo("model", model_file)) {
		for (uint32 i = 0; i < world->models.size(); i += 1) {
			if (ImGui::Selectable(world->models[i].file, *model_index == i)) {
				*model_index = i;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::InputFloat3("T", &transform->translate[0]);
	ImGui::InputFloat3("R", &transform->rotate[0], "%.3f", ImGuiInputTextFlags_ReadOnly);
	float scale = transform->scale[0];
	if (ImGui::InputFloat("S", &scale) && scale > 0) {
		transform->scale = { scale, scale, scale };
	}
	if (ImGui::Button("Reset Transform")) {
		*transform = transform_identity();
	}
}

void editor_edit_window_player_tab(editor *editor, world *world) {
	ImGui::PushID("player_tab");
	editor_edit_window_model_transform(world, &world->player.model_index, &world->player.transform);
	ImGui::PopID();
}

void editor_edit_window_static_object_tab(editor *editor, world *world) {
	ImGui::PushID("static_object_tab");
	if (ImGui::Button("Add")) {
		editor->add_static_object_popup = true;
	}
	static_object *static_object = editor->static_object_index < world->static_objects.size() ? &world->static_objects[editor->static_object_index] : nullptr;
	const char *id = static_object ? static_object->id.ptr : nullptr;
	if (ImGui::BeginCombo("static objects", id)) {
		for (uint32 i = 0; i < world->static_objects.size(); i += 1) {
			if (ImGui::Selectable(world->static_objects[i].id.ptr, editor->static_object_index == i)) {
				editor->static_object_index = i;
				static_object = &world->static_objects[i];
			}
		}
		ImGui::EndCombo();
	}
	if (static_object) {
		if (ImGui::Button("Rename")) {
			editor->rename_static_object_popup = true;
		}
	}
	if (static_object) {
		editor_edit_window_model_transform(world, &static_object->model_index, &static_object->transform);
	}
	ImGui::PopID();
}

void editor_edit_window_dynamic_object_tab(editor *editor, world *world) {
	ImGui::PushID("dynamic_object_tab");
	if (ImGui::Button("Add")) {
		editor->add_dynamic_object_popup = true;
	}
	dynamic_object *dynamic_object = editor->dynamic_object_index < world->dynamic_objects.size() ? &world->dynamic_objects[editor->dynamic_object_index] : nullptr;
	const char *id = dynamic_object ? dynamic_object->id.ptr : nullptr;
	if (ImGui::BeginCombo("dynamic objects", id)) {
		for (uint32 i = 0; i < world->dynamic_objects.size(); i += 1) {
			if (ImGui::Selectable(world->dynamic_objects[i].id.ptr, editor->dynamic_object_index == i)) {
				editor->dynamic_object_index = i;
				dynamic_object = &world->dynamic_objects[i];
			}
		}
		ImGui::EndCombo();
	}
	if (dynamic_object) {
		if (ImGui::Button("Rename")) {
			editor->rename_dynamic_object_popup = true;
		}
	}
	if (dynamic_object) {
		editor_edit_window_model_transform(world, &dynamic_object->model_index, &dynamic_object->transform);
	}
	ImGui::PopID();
}

void editor_edit_window_model_tab(editor *editor, world *world, d3d12 *d3d12) {
	ImGui::PushID("model_tab");
	if (ImGui::Button("Import")) {
		char file[256] = {};
		if (open_file_dialog(file, sizeof(file))) {
			if (world_add_model(world, d3d12, file, transform_identity(), collision{ collision_type_none })) {
				editor->model_index = (uint32)world->models.size() - 1;
			}
			else {
				snprintf(editor->error_msg, sizeof(editor->error_msg), "Add model failed\nFile: %s", file);
				editor->error_popup = true;
			}
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox("Adjust", &editor->adjust_model);
	ImGui::Separator();
	model *model = editor->model_index < world->models.size() ? &world->models[editor->model_index] : nullptr;
	const char *file = model ? model->file : nullptr;
	if (ImGui::BeginCombo("models", file)) {
		for (uint32 i = 0; i < world->models.size(); i += 1) {
			if (ImGui::Selectable(world->models[i].file, editor->model_index == i)) {
				editor->model_index = i;
				model = &world->models[i];
			}
		}
		ImGui::EndCombo();
	}
	if (model) {
		ImGui::InputFloat3("T", &model->transform.translate[0]);
		ImGui::InputFloat3("R", &model->transform.rotate[0], "%.3f", ImGuiInputTextFlags_ReadOnly);
		float scale = model->transform.scale[0];
		if (ImGui::InputFloat("S", &scale) && scale > 0) {
			model->transform.scale = { scale, scale, scale };
		}
		if (ImGui::Button("Reset Transform")) {
			model->transform = transform_identity();
		}
		if (model->collision.type == collision_type_none) {
			if (ImGui::Button("Add Collision")) {
				model->collision.type = collision_type_sphere;
				model->collision.sphere = collision_sphere{ 1 };
			}
		}
		else {
			const char *preview_str = collision_type_strs[model->collision.type];
			if (ImGui::BeginCombo("shapes", preview_str)) {
				if (ImGui::Selectable("sphere", model->collision.type == collision_type_sphere)) {
					if (model->collision.type != collision_type_sphere) {
						model->collision.type = collision_type_sphere;
						model->collision.sphere = collision_sphere{ 1 };
					}
				}
				if (ImGui::Selectable("box", model->collision.type == collision_type_box)) {
					if (model->collision.type != collision_type_box) {
						model->collision.type = collision_type_box;
						model->collision.box = collision_box{ 1, 1, 1 };
					}
				}
				if (ImGui::Selectable("capsule", model->collision.type == collision_type_capsule)) {
					if (model->collision.type != collision_type_capsule) {
						model->collision.type = collision_type_capsule;
						model->collision.capsule = collision_capsule{ 1, 2 };
					}
				}
				ImGui::EndCombo();
			}
			if (model->collision.type == collision_type_sphere) {
				ImGui::InputFloat("radius", &model->collision.sphere.radius);
			}
			if (model->collision.type == collision_type_box) {
				ImGui::InputFloat3("extents", &model->collision.box.extents[0]);
			}
			if (model->collision.type == collision_type_capsule) {
				ImGui::InputFloat("radius", &model->collision.capsule.radius);
				ImGui::InputFloat("height", &model->collision.capsule.height);
			}
			if (ImGui::Button("Remove Collision")) {
				model->collision.type = collision_type_none;
			}
		}
	}
	ImGui::PopID();
}

void editor_direct_light_properties(editor *editor, direct_light *direct_light) {
	ImGui::Text("Properties:");
	ImGui::ColorEdit3("color", &direct_light->color.x);
	float extra = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x * 2 + ImGui::CalcTextSize("direct color").x;
	ImGui::PushItemWidth(ImGui::GetItemRectSize().x - extra);
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::DragFloat3("position", &direct_light->position.x);
	ImGui::DragFloat3("direction", &direct_light->dir.x);
	ImGui::PopItemFlag();
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGui::Button("reset")) {
		direct_light->dir = { 0, 1, 0 };
	}
}

void editor_sphere_light_properties(editor *editor, sphere_light *sphere_light) {
	ImGui::Text("Properties:");
	ImGui::InputFloat3("position", &sphere_light->position.x);
}

void editor_edit_window_terrain_tab(editor *editor, world *world) {
	ImGui::PushID("terrain_tab");
	if (ImGui::Button("New")) {
		editor->new_terrain_popup = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Import")) {
		char file[256] = {};
		if (open_file_dialog(file, sizeof(file))) {
			if (file_exists(file)) {
				//if (!add_terrain(world, d3d, file)) {
				//	editor->error_popup = true;
				//	snprintf(editor->error_msg, sizeof(editor->error_msg), "Add terrain failed\nFile: %s", file);
				//}
			}
			else {
				editor->error_popup = true;
				snprintf(editor->error_msg, sizeof(editor->error_msg), "File does not exist\nFile: %s", file);
			}
		}
	}
	terrain *terrain = world->terrain_index < world->terrains.size() ? &world->terrains[world->terrain_index] : nullptr;
	const char *file = terrain ? terrain->file : nullptr;
	if (ImGui::BeginCombo("terrains", file)) {
		for (uint32 i = 0; i < world->terrains.size(); i += 1) {
			if (ImGui::Selectable(world->terrains[i].file, world->terrain_index == i)) {
				world->terrain_index = i;
				terrain = &world->terrains[i];
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}

void editor_edit_window_skybox_tab(editor *editor, world *world) {
	ImGui::PushID("skybox_tab");
	if (ImGui::Button("Import")) {
		char file[256] = {};
		if (open_file_dialog(file, sizeof(file))) {
			//if (!add_skybox(world, d3d, file)) {
			//	snprintf(editor->error_msg, sizeof(editor->error_msg), "Add Skybox Failed\nFile: %s", file);
			//	editor->error_popup = true;
			//}
		}
	}
	skybox *skybox = world->skybox_index < world->skyboxes.size() ? &world->skyboxes[world->skybox_index] : nullptr;
	const char *file = skybox ? skybox->file : nullptr;
	if (ImGui::BeginCombo("skyboxes", file)) {
		for (uint32 i = 0; i < world->skyboxes.size(); i += 1) {
			if (ImGui::Selectable(world->skyboxes[i].file, world->skybox_index == i)) {
				world->skybox_index = i;
				skybox = &world->skyboxes[i];
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}

void editor_objects_window(editor *editor, world *world, d3d12 *d3d12) {
	ImGui::SetNextWindowPos(ImVec2{ 0, editor->top_menu_bar_height }, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2{ ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.85f }, ImGuiCond_Always);
	ImGui::PushID("objects_window");
	if (ImGui::Begin("Objects", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		editor->objects_window_pos = ImGui::GetWindowPos();
		editor->objects_window_size = ImGui::GetWindowSize();

		if (ImGui::BeginChild("object_lists", ImVec2(0, editor->objects_window_size.y * 0.4f))) {
			if (ImGui::TreeNodeEx("direct lights", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::OpenPopupOnItemClick("direct lights popup");
				if (ImGui::BeginPopup("direct lights popup")) {
					if (ImGui::Button("New")) {
						direct_light direct_light = { {0, 0, 0}, vec3_normalize({1, 1, 1}), {1, 1, 1} };
						world->direct_lights.push_back(direct_light);
					}
					ImGui::EndPopup();
				}
				for (uint32 i = 0; i < world->direct_lights.size(); i += 1) {
					bool selected = editor->selected_object_type == selectable_object_direct_light && editor->selected_object_index == i;
					char index_str[8] = "";
					itoa(i, index_str, 10);
					if (ImGui::Selectable(index_str, selected)) {
						editor->selected_object_type = selectable_object_direct_light;
						editor->selected_object_index = i;
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("sphere lights", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::OpenPopupOnItemClick("sphere lights popup");
				if (ImGui::BeginPopup("sphere lights popup")) {
					if (ImGui::Selectable("New")) {
						sphere_light sphere_light = { {0, 0, 0}, {1, 1, 1}, 1, 10 };
						world->sphere_lights.push_back(sphere_light);
						editor->selected_object_type = selectable_object_sphere_light;
						editor->selected_object_index = (uint32)world->sphere_lights.size() - 1;
					}
					ImGui::EndPopup();
				}
				for (uint32 i = 0; i < world->sphere_lights.size(); i += 1) {
					bool selected = editor->selected_object_type == selectable_object_sphere_light && editor->selected_object_index == i;
					char index_str[8] = "";
					itoa(i, index_str, 10);
					if (ImGui::Selectable(index_str, selected)) {
						editor->selected_object_type = selectable_object_sphere_light;
						editor->selected_object_index = i;
					}
					char popup_id_str[32] = {};
					snprintf(popup_id_str, sizeof(popup_id_str), "sphere light popup %u", i);
					ImGui::OpenPopupOnItemClick(popup_id_str);
					if (ImGui::BeginPopup(popup_id_str)) {
						if (ImGui::Selectable("Delete")) {
							world->sphere_lights.erase(world->sphere_lights.begin() + i);
							editor->selected_object_type = selectable_object_none;
						}
						ImGui::EndPopup();
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("models", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::OpenPopupOnItemClick("models popup");
				if (ImGui::BeginPopup("models popup")) {
					if (ImGui::Selectable("New")) {
						char file[256] = {};
						if (open_file_dialog(file, sizeof(file))) {
							if (world_add_model(world, d3d12, file, transform_identity(), collision{ collision_type_none })) {
								editor->model_index = (uint32)world->models.size() - 1;
								dxr_build_acceleration_buffers(world, d3d12);
							}
							else {
								snprintf(editor->error_msg, sizeof(editor->error_msg), "Import model failed\nFile: %s", file);
								editor->error_popup = true;
							}
						}
					}
					ImGui::EndPopup();
				}
				for (uint32 i = 0; i < world->models.size(); i += 1) {
					bool selected = editor->selected_object_type == selectable_object_model && editor->selected_object_index == i;
					if (ImGui::Selectable(world->models[i].file, selected)) {
						editor->selected_object_type = selectable_object_model;
						editor->selected_object_index = i;
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

		ImGui::PushID("properties");
		if (ImGui::BeginChild("object_properties")) {
			if (editor->selected_object_type == selectable_object_direct_light) {
				direct_light *direct_light = &world->direct_lights[editor->selected_object_index];
				editor_direct_light_properties(editor, direct_light);
			}
			else if (editor->selected_object_type == selectable_object_sphere_light) {
				sphere_light *sphere_light = &world->sphere_lights[editor->selected_object_index];
				editor_sphere_light_properties(editor, sphere_light);
			}
		}
		ImGui::EndChild();
		ImGui::PopID();
	}
	ImGui::End();
	ImGui::PopID();
}

void editor_memories_window(editor *editor, world *world) {
	ImGui::SetNextWindowPos(ImVec2{ ImGui::GetIO().DisplaySize.x * 0.85f, editor->top_menu_bar_height }, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2{ ImGui::GetIO().DisplaySize.x * 0.15f, ImGui::GetIO().DisplaySize.y * 0.2f }, ImGuiCond_Always);
	ImGui::PushID("memory_usage_window");
	if (ImGui::Begin("Memory Usage", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		editor->memory_window_pos = ImGui::GetWindowPos();
		editor->memory_window_size = ImGui::GetWindowSize();
		auto imgui_render_memory = [](uint64 memory_size, uint64 memory_capacity, const char *memory_name) {
			char overlay[64] = {};
			snprintf(overlay, sizeof(overlay), "%s / %s", pretty_print_bytes(memory_size).data(), pretty_print_bytes(memory_capacity).data());
			ImGui::ProgressBar((float)((double)memory_size / (double)memory_capacity), ImVec2{ ImGui::GetWindowContentRegionWidth() * 0.5f, 0 }, overlay);
			ImGui::SameLine();
			ImGui::Text("%s", memory_name);
		};
		ImGui::Text("System Memory");
		imgui_render_memory(editor->world_frame_memory_arena_size, world->frame_memory_arena.capacity, "world frame arena");
		ImGui::Text("GPU Memory");
		imgui_render_memory(world->frame_constants_buffer_size, world->frame_constants_buffer_capacity, "world frame constants");
	}
	ImGui::End();
	ImGui::PopID();
}

float editor_frame_time_ring_buffer_values_getter(void *data, int index) {
	editor *editor = (struct editor *)data;
	return editor->frame_time_ring_buffer.buffer[(editor->frame_time_ring_buffer.read_index + index) % editor->frame_time_ring_buffer.capacity];
}

void editor_frame_statistic_window(editor *editor, window *window) {
	if (editor->show_frame_statistic_window) {
		ImGui::PushID("frame_statistic_window");
		ImGui::SetNextWindowPos(ImVec2((float)window->width / 2, (float)window->height / 2), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		if (ImGui::Begin("Frame statistic", &editor->show_frame_statistic_window)) {
			ImGui::Text("Frame time: %.3f ms", editor->coarse_frame_time * 1000);
			ImGui::PlotLines("frame_time_plot", editor_frame_time_ring_buffer_values_getter, editor, editor->frame_time_ring_buffer.size);
		}
		ImGui::End();
		ImGui::PopID();
	}
}

void editor_update_camera(editor *editor, window *window) {
	if (ImGui::IsMouseClicked(2)) {
		cursor_pin(true);
		editor->camera_active = true;
	}
	if (ImGui::IsMouseReleased(2)) {
		cursor_pin(false);
		editor->camera_active = false;
	}
	if (editor->camera_active) {
		float move_distance = (float)editor->last_frame_time * editor->camera_move_speed;
		if (ImGui::IsKeyDown(VK_SHIFT)) {
			move_distance *= 10;
		}
		if (ImGui::IsKeyDown('W')) {
			editor->camera_position += editor->camera_view * move_distance;
		}
		if (ImGui::IsKeyDown('S')) {
			editor->camera_position -= editor->camera_view * move_distance;
		}
		if (ImGui::IsKeyDown('A')) {
			editor->camera_position -= XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0)) * move_distance;
		}
		if (ImGui::IsKeyDown('D')) {
			editor->camera_position += XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0)) * move_distance;
		}
		float rotate_distance = editor->camera_rotate_speed * 0.1f * (float)editor->last_frame_time;
		float dyaw = window->raw_mouse_dx * rotate_distance;
		editor->camera_view = XMVector3Normalize(XMVector3Transform(editor->camera_view, XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), -dyaw)));

		float max_pitch = degree_to_radian(80.0f);
		float pitch = asinf(XMVectorGetY(editor->camera_view));
		float dpitch = -window->raw_mouse_dy * rotate_distance;
		if ((dpitch + pitch) > -max_pitch && (pitch + dpitch) < max_pitch) {
			XMVECTOR axis = XMVector3Normalize(XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0)));
			editor->camera_view = XMVector3Normalize(XMVector3Transform(editor->camera_view, XMMatrixRotationAxis(axis, dpitch)));
		}
	}
	else {
		float move_distance = (float)editor->last_frame_time * 20;
		if (ImGui::IsKeyDown(VK_UP) && ImGui::IsKeyDown(VK_CONTROL)) {
			editor->camera_position += editor->camera_view * move_distance;
		}
		if (ImGui::IsKeyDown(VK_DOWN) && ImGui::IsKeyDown(VK_CONTROL)) {
			editor->camera_position -= editor->camera_view * move_distance;
		}
		if (ImGui::IsKeyDown(VK_LEFT) && ImGui::IsKeyDown(VK_CONTROL)) {
			editor->camera_position -= XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0)) * move_distance;
		}
		if (ImGui::IsKeyDown(VK_RIGHT) && ImGui::IsKeyDown(VK_CONTROL)) {
			editor->camera_position += XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0)) * move_distance;
		}
	}
	{
		editor->camera_view_mat = XMMatrixLookToRH(editor->camera_position, editor->camera_view, XMVectorSet(0, 1, 0, 0));
		editor->camera_proj_mat = XMMatrixPerspectiveFovRH(editor->camera_fovy, (float)window->width / (float)window->height, editor->camera_znear, editor->camera_zfar);
		editor->camera_view_proj_mat = editor->camera_view_mat * editor->camera_proj_mat;
		//if (window_cursor_inside(window)) {
		//	vec3 world_position = mat4_unproject(vec3{ ImGui::GetMousePos().x, ImGui::GetMousePos().y, 0 }, editor->camera_view_mat, editor->camera_proj_mat, vec4{ 0, 0, (float)window->width, (float)window->height });
		//	vec3 ray_dir = vec3_normalize(world_position - editor->camera_position);
		//	editor->camera_to_mouse_ray = ray{ editor->camera_position, ray_dir, editor->camera_zfar };
		//}
	}
}

void editor_tool_gizmo(editor *editor, world *world, window *window) {
	float aspect = (float)window->width / (float)window->height;
	float znear = min(editor->camera_znear, editor->camera_zfar);
	float zfar = max(editor->camera_znear, editor->camera_zfar);
	mat4 camera_view_mat = mat4_look_at(vec3_from_xmvector(editor->camera_position), vec3_from_xmvector(editor->camera_position + editor->camera_view));
	mat4 camera_proj_mat = mat4_project(editor->camera_fovy, aspect, znear, zfar);

	auto render_direct_light = [&](direct_light *dir_light) {
		vec3 line_verts[2] = { {0, 0, 0}, {0, 0.1f, 0} };
		vec3 tri_verts[3] = { {-0.02f, 0.1f, 0}, {0.02f, 0.1f, 0}, {0, 0.11f, 0} };
		vec3 translation = dir_light->position;
		quat rotation = quat_from_between({ 0, 1, 0 }, dir_light->dir);
		mat4 transform_mat = mat4_from_translate(translation) * mat4_from_rotate(rotation);
		mat4 camera_mat = camera_proj_mat * camera_view_mat;
		auto transform_to_screen_space = [&](vec3 v) {
			vec4 v2 = camera_mat * transform_mat * vec4{ v.x, v.y, v.z, 1 };
			v2 /= v2.w;
			float x = (v2.x + 1) * 0.5f;
			float y = (-v2.y + 1) * 0.5f;
			ImVec2 vec;
			vec.x = ImGui::GetIO().DisplaySize.x * x;
			vec.y = ImGui::GetIO().DisplaySize.y * y;
			return vec;
		};
		ImVec2 line_screen_space_verts[2] = {};
		ImVec2 tri_screen_space_verts[3] = {};
		for (uint32 i = 0; i < m_countof(line_verts); i += 1) {
			line_screen_space_verts[i] = transform_to_screen_space(line_verts[i]);
		}
		for (uint32 i = 0; i < m_countof(tri_verts); i += 1) {
			tri_screen_space_verts[i] = transform_to_screen_space(tri_verts[i]);
		}
		ImDrawList *draw_list = ImGuizmo::gContext.mDrawList;
		draw_list->AddLine(line_screen_space_verts[0], line_screen_space_verts[1], 0xffffffff);
		draw_list->AddTriangleFilled(tri_screen_space_verts[0], tri_screen_space_verts[1], tri_screen_space_verts[2], 0xffffffff);
	};

	if (editor->selected_object_type == selectable_object_direct_light) {
		direct_light *direct_light = &world->direct_lights[editor->selected_object_index];
		if (editor->tool_type == tool_type_translate) {
			mat4 transform_mat = mat4_from_translate(direct_light->position);
			ImGuizmo::BeginFrame();
			ImGuizmo::Manipulate((const float *)camera_view_mat, (const float *)camera_proj_mat, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, (float *)transform_mat);
			direct_light->position = mat4_get_translate(transform_mat);
			// render_direct_light(direct_light);
		}
		else if (editor->tool_type == tool_type_rotate) {
			vec3 default_dir = { 0, 1, 0 };
			mat4 transform_mat = mat4_from_translate(direct_light->position) * mat4_from_rotate(quat_from_between(default_dir, direct_light->dir));
			ImGuizmo::BeginFrame();
			ImGuizmo::Manipulate((const float *)camera_view_mat, (const float *)camera_proj_mat, ImGuizmo::ROTATE, ImGuizmo::LOCAL, (float *)transform_mat);
			direct_light->dir = vec3_normalize(mat4_get_rotate(transform_mat) * default_dir);
			// render_direct_light(direct_light);
		}
	}
	else if (editor->selected_object_type == selectable_object_sphere_light) {
		sphere_light *sphere_light = &world->sphere_lights[editor->selected_object_index];
		if (editor->tool_type == tool_type_translate) {
			mat4 transform_mat = mat4_from_translate(sphere_light->position);
			ImGuizmo::BeginFrame();
			ImGuizmo::Manipulate((const float *)camera_view_mat, (const float *)camera_proj_mat, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, (float *)transform_mat);
			sphere_light->position = mat4_get_translate(transform_mat);
		}
	}

	//auto transform_gizmo = [&](transformable_type type) {
	//	for (auto tool_type : { tool_type_translate, tool_type_rotate, tool_type_scale }) {
	//		if (editor->tool_type == tool_type) {
	//			mat4 camera_view_mat = mat4_from_xmmatrix(editor->camera_view_mat);
	//			mat4 camera_proj_mat = mat4_from_xmmatrix(editor->camera_proj_mat);
	//			transform *transform = nullptr;
	//			switch (type) {
	//			case transformable_type_player: transform = &world->player.transform; break;
	//			case transformable_type_static_object: transform = &world->static_objects[editor->static_object_index].transform; break;
	//			case transformable_type_dynamic_object: transform = &world->dynamic_objects[editor->dynamic_object_index].transform; break;
	//			case transformable_type_model: transform = &world->models[editor->model_index].transform; break;
	//			}
	//			mat4 transform_mat = mat4_from_transform(*transform);
	//			mat4 old_transform_mat = transform_mat;
	//			ImGuizmo::OPERATION operations[] = { ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE };
	//			ImGuizmo::MODE modes[] = { ImGuizmo::WORLD, ImGuizmo::LOCAL, ImGuizmo::LOCAL };
	//			ImGuizmo::BeginFrame();
	//			ImGuizmo::Manipulate((const float *)camera_view_mat, (const float *)camera_proj_mat, operations[editor->tool_type], modes[editor->tool_type], (float *)transform_mat);
	//			*transform = mat4_get_transform(transform_mat);
	//			static bool transforming = false;
	//			static struct transform original_transform = transform_identity();
	//			if (ImGuizmo::IsUsing()) {
	//				if (!transforming && (transform_mat != old_transform_mat)) {
	//					transforming = true;
	//					original_transform = mat4_get_transform(old_transform_mat);
	//				}
	//			}
	//			else {
	//				if (transforming) {
	//					transforming = false;
	//					edit_operation operation = { edit_operation_object_transform };
	//					operation.transform_operation.original_transform = original_transform;
	//					switch (type) {
	//					case transformable_type_player: {
	//						operation.transform_operation.transformable_type = transformable_type_player;
	//					} break;
	//					case transformable_type_static_object: {
	//						operation.transform_operation.transformable_type = transformable_type_static_object;
	//						strcpy(operation.transform_operation.id, world->static_objects[editor->static_object_index].id.ptr);
	//					} break;
	//					case transformable_type_dynamic_object: {
	//						operation.transform_operation.transformable_type = transformable_type_dynamic_object;
	//						strcpy(operation.transform_operation.id, world->dynamic_objects[editor->dynamic_object_index].id.ptr);
	//					} break;
	//					case transformable_type_model: {
	//						operation.transform_operation.transformable_type = transformable_type_model;
	//						strcpy(operation.transform_operation.id, world->models[editor->model_index].file);
	//					} break;
	//					}
	//					editor_add_undo(editor, operation);
	//				}
	//			}
	//			break;
	//		}
	//	}
	//};
	//if (editor->edit_window_tab == edit_window_tab_player) {
	//	transform_gizmo(transformable_type_player);
	//}
	//else if (editor->edit_window_tab == edit_window_tab_static_object && editor->static_object_index < world->static_objects.size()) {
	//	transform_gizmo(transformable_type_static_object);
	//}
	//else if (editor->edit_window_tab == edit_window_tab_dynamic_object && editor->dynamic_object_index < world->dynamic_objects.size()) {
	//	transform_gizmo(transformable_type_dynamic_object);
	//}
	//else if (editor->edit_window_tab == edit_window_tab_model && editor->model_index < world->models.size()) {
	//	transform_gizmo(transformable_type_model);
	//}
	//else if (editor->edit_window_tab == edit_window_tab_terrain && world->terrain_index < world->terrains.size()) {
	//	terrain *terrain = &world->terrains[world->terrain_index];
	//	float half_width = terrain->width * 0.5f;
	//	float half_height = terrain->height * 0.5f;
	//	bool tool_selected = editor->tool_type > tool_type_terrain_begin && editor->tool_type < tool_type_terrain_end;
	//	if (tool_selected) {
	//		ImGui::PushID("terrain_brush_properties_window");
	//		ImGui::SetNextWindowPos(editor->objects_window_pos + ImVec2(editor->objects_window_size.x, 0), ImGuiCond_Once);
	//		ImGui::SetNextWindowSize(ImVec2(editor->objects_window_size.x, 0), ImGuiCond_Once);
	//		if (ImGui::Begin("Terrain Brush")) {
	//			ImGui::SliderFloat("Radius", &editor->terrain_brush_tool_radius, 1, 10);
	//			ImGui::SliderFloat("Speed", &editor->terrain_brush_tool_speed, 1, 10);
	//		}
	//		ImGui::End();
	//		ImGui::PopID();
	//		if (editor->tool_type == tool_type_terrain_paint) {
	//			ImGui::PushID("terrain_texture_window");
	//			ImGui::SetNextWindowPos(editor->objects_window_pos + ImVec2(editor->objects_window_size.x * 2, 0), ImGuiCond_Once);
	//			ImGui::SetNextWindowSize(ImVec2(editor->objects_window_size.x, 0), ImGuiCond_Once);
	//			if (ImGui::Begin("Terrain Textures")) {
	//				for (uint32 i = 0; i < editor->terrain_paint_texture_count; i += 1) {
	//					ImGui::SameLine();
	//					if (ImGui::ImageButton((ImTextureID)editor->terrain_paint_texture_views[i], ImVec2(32, 32))) {
	//						editor->terrain_paint_texture_index = i;
	//					}
	//				}
	//			}
	//			ImGui::End();
	//			ImGui::PopID();
	//		}
	//	}
	//	if (tool_selected && window_cursor_inside(window)) {
	//		float t;
	//		if (ray_hit_plane(editor->camera_to_mouse_ray, plane{ {0, 1, 0}, 0 }, &t)) {
	//			vec3 p = editor->camera_to_mouse_ray.origin + editor->camera_to_mouse_ray.dir * t;
	//			if (p.x >= -half_width && p.x <= half_width && p.z >= -half_height && p.z <= half_height) {
	//				editor->terrain_brush_tool_active = true;
	//				editor->terrain_brush_tool_position = { p.x, 0, p.z };
	//			}
	//		}
	//	}
	//	if (editor->terrain_brush_tool_active && ImGui::IsMouseDown(0) && !ImGui::GetIO().WantCaptureMouse) {
	//		static double accumulate_frame_time = 0;
	//		accumulate_frame_time += editor->last_frame_time;
	//		if (accumulate_frame_time >= 1.0 / 15.0) {
	//			accumulate_frame_time = 0;

	//			if (editor->tool_type == tool_type_terrain_bump) {
	//				uint32 height_texture_width = terrain->width * terrain->sample_per_meter;
	//				uint32 height_texture_height = terrain->height * terrain->sample_per_meter;

	//				vec2 center = editor->terrain_brush_tool_position.xz() + vec2{ half_width, half_height };
	//				vec2 begin = center - vec2{ editor->terrain_brush_tool_radius, editor->terrain_brush_tool_radius };
	//				vec2 end = center + vec2{ editor->terrain_brush_tool_radius, editor->terrain_brush_tool_radius };
	//				vec2 center_uv = center / vec2{ (float)terrain->width, (float)terrain->height };
	//				vec2 begin_uv = begin / vec2{ (float)terrain->width, (float)terrain->height };
	//				vec2 end_uv = end / vec2{ (float)terrain->width, (float)terrain->height };
	//				int32 center_texel[2] = { (int32)(height_texture_width * center_uv.x), (int32)(height_texture_height * center_uv.y) };
	//				int32 begin_texel[2] = { (int32)(height_texture_width * begin_uv.x), (int32)(height_texture_height * begin_uv.y) };
	//				int32 end_texel[2] = { (int32)(height_texture_width * end_uv.x), (int32)(height_texture_height * end_uv.y) };
	//				double radius = (double)abs(begin_texel[0] - center_texel[0]);
	//				double bump_step = 128.0 * editor->terrain_brush_tool_speed;

	//				for (int32 y = begin_texel[1]; y <= end_texel[1]; y += 1) {
	//					for (int32 x = begin_texel[0]; x <= end_texel[0]; x += 1) {
	//						double dx = (double)abs(x - center_texel[0]);
	//						double dy = (double)abs(y - center_texel[1]);
	//						double distance = sqrt(dx * dx + dy * dy);
	//						if (distance < radius) {
	//							double p = distance / radius;
	//							double bump_p = exp(-1.0 / (1.0 - p * p));
	//							int16 *pixel = &terrain->height_texture_data[height_texture_width * y + x];
	//							int16 bump = (int16)(bump_step * bump_p);
	//							if (*pixel > 0 && bump > (INT16_MAX - *pixel)) {
	//								*pixel = INT16_MAX;
	//							}
	//							else {
	//								*pixel += bump;
	//							}
	//						}
	//					}
	//				}
	//				//D3D11_BOX box = { 0, 0, 0, terrain->width * terrain->sample_per_meter, terrain->height * terrain->sample_per_meter, 1 };
	//				//d3d->context->UpdateSubresource(terrain->height_texture, 0, &box, terrain->height_texture_data, terrain->width * terrain->sample_per_meter * sizeof(int16), 0);
	//			}
	//		}
	//	}
	//}
}

void editor_check_undo(editor *editor, world *world) {
	if (ImGui::IsKeyPressed('Z') && ImGui::IsKeyDown(VK_CONTROL)) {
		editor_pop_undo(editor, world);
	}
}

void append_extra_model_constants(editor *editor, world *world, d3d *d3d) {
	//if (editor->adjust_model && editor->model_index < world->model_count) {
	//	world_add_model_render_data(world, d3d, editor->model_index, transform_identity(), 0, 0, true);
	//}
}

void append_terrain_brush_constants(editor *editor, world *world, d3d *d3d) {
	//if (editor->terrain_brush_tool_active) {
	//	m_debug_assert(world->terrain_index < world->terrain_count);
	//	terrain *terrain = &world->terrains[world->terrain_index];
	//	struct {
	//		XMMATRIX transform_mat;
	//		uint32 width;
	//		uint32 height;
	//		float max_height;
	//	} terrain_brush_constants = {
	//		XMMatrixMultiply(XMMatrixScaling(editor->terrain_brush_tool_radius, 1, editor->terrain_brush_tool_radius), XMMatrixTranslation(m_unpack3(editor->terrain_brush_tool_position))),
	//		terrain->width, terrain->height, terrain->max_height
	//	};
	//	uint32 offset = world_append_constant_buffer(world, &terrain_brush_constants, sizeof(terrain_brush_constants));
	//	world->render_data.terrain_brush_constant_buffer_offset = offset / 16;
	//}
}

void render_terrain_brush(editor *editor, world *world, d3d *d3d) {
	//if (editor->terrain_brush_tool_active) {
	//	editor->terrain_brush_tool_active = false;
	//	m_debug_assert(world->terrain_index < world->terrain_count);
	//	terrain *terrain = &world->terrains[world->terrain_index];
	//	d3d->context->VSSetShader(d3d->terrain_brush_vs, nullptr, 0);
	//	d3d->context->PSSetShader(d3d->terrain_brush_ps, nullptr, 0);
	//	d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	d3d->context->IASetInputLayout(d3d->terrain_brush_input_layout);
	//	d3d->context->OMSetDepthStencilState(d3d->terrain_brush_depth_stencil_state, 0);
	//	d3d->context->VSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
	//	d3d->context->VSSetShaderResources(0, 1, &terrain->height_texture_view);
	//	uint32 constant_count = 16;
	//	d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &world->render_data.terrain_brush_constant_buffer_offset, &constant_count);
	//	uint32 vertex_buffer_stride = 12;
	//	uint32 vertex_buffer_offset = 0;
	//	d3d->context->IASetVertexBuffers(0, 1, &world->hollow_circle_vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
	//	d3d->context->Draw(m_countof(hollow_circle_vertices), 0);
	//}
}

int main(int argc, char **argv) {
	set_current_dir_to_exe_dir();

	window *window = new struct window();
	window_init(window, window_message_callback);
	// set_window_fullscreen(window, true);

	d3d12 *d3d12 = new struct d3d12();
	d3d12_init(d3d12, window);

	editor *editor = new struct editor();
	editor_init(editor, d3d12, window);

	world *world = new struct world;
	world_init(world, d3d12);
	if (argc > 1) {
		const char *world_file = argv[1];
		m_assert(editor_load_world(editor, world, d3d12, world_file));
	}

	dxr_init_pipeline_state(world, d3d12);
	dxr_init_shader_resources(world, d3d12, window);
	dxr_init_shader_table(world, d3d12);
	dxr_build_acceleration_buffers(world, d3d12);

	window_message_channel.window = window;
	window_message_channel.d3d12 = d3d12;
	window_message_channel.initialized = true;
	window_show(window);

	while (!editor->quit) {
		timer_start(&editor->timer);
		auto record_frame_timing = scope_exit([&] {
			timer_stop(&editor->timer);
			editor->last_frame_time = timer_get_duration(editor->timer);
			static uint32 frame_count = 0;
			static double frame_time = 0;
			frame_count += 1;
			frame_time += editor->last_frame_time;
			if (frame_time >= 1.0) {
				editor->coarse_frame_time = frame_time / frame_count;
				ring_buffer_write(&editor->frame_time_ring_buffer, (float)editor->coarse_frame_time);
				frame_count = 0;
				frame_time = 0;
			}
		});

		window->raw_mouse_dx = 0;
		window->raw_mouse_dy = 0;
		window_handle_messages(window);

		ImGui::GetIO().DeltaTime = (float)editor->last_frame_time;
		ImGui::NewFrame();
		editor_check_quit(editor);
		editor_check_toggle_fullscreen(window);
		editor_check_popups(editor, world, d3d12);
		editor_top_menu(editor, world, d3d12);
		editor_bottom_menu(editor);
		editor_objects_window(editor, world, d3d12);
		editor_memories_window(editor, world);
		editor_frame_statistic_window(editor, window);
		editor_update_camera(editor, window);
		editor_tool_gizmo(editor, world, window);
		editor_check_undo(editor, world);
		ImGui::Render();

		m_d3d_assert(d3d12->command_allocator->Reset());
		m_d3d_assert(d3d12->command_list->Reset(d3d12->command_allocator, nullptr));

		world_render_params world_render_params = {
			editor->camera_view_proj_mat, editor->camera_position, editor->camera_view, editor->camera_fovy
		};
		world_render_commands(world, d3d12, &world_render_params);

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.pResource = d3d12->swap_chain_render_targets[d3d12->swap_chain_buffer_index];
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3d12->command_list->ResourceBarrier(1, &barrier);

		D3D12_CPU_DESCRIPTOR_HANDLE swap_chain_render_target_handle = d3d12->swap_chain_render_targets_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
		swap_chain_render_target_handle.ptr += d3d12->swap_chain_buffer_index * d3d12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		float swap_chain_clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		d3d12->command_list->ClearRenderTargetView(swap_chain_render_target_handle, swap_chain_clear_color, 0, nullptr);
		d3d12->command_list->OMSetRenderTargets(1, &swap_chain_render_target_handle, false, nullptr);

		D3D12_VIEWPORT viewport = { 0.0f, 0.0f, (float)window->width, (float)window->height, 0.0f, 1.0f };
		RECT scissor = { 0, 0, (LONG)window->width, (LONG)window->height };
		d3d12->command_list->RSSetViewports(1, &viewport);
		d3d12->command_list->RSSetScissorRects(1, &scissor);

		uint32 world_frame_descriptor_handle_size = d3d12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CPU_DESCRIPTOR_HANDLE world_frame_cpu_descriptor_handle = { world->frame_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + world->frame_descriptor_handle_count * world_frame_descriptor_handle_size };
		D3D12_GPU_DESCRIPTOR_HANDLE world_frame_gpu_descriptor_handle = { world->frame_descriptor_heap->GetGPUDescriptorHandleForHeapStart().ptr + world->frame_descriptor_handle_count * world_frame_descriptor_handle_size };
		d3d12->device->CreateShaderResourceView(world->render_target, nullptr, world_frame_cpu_descriptor_handle);
		d3d12->device->CreateShaderResourceView(d3d12->dither_texture, nullptr, { world_frame_cpu_descriptor_handle.ptr + world_frame_descriptor_handle_size });
		world->frame_descriptor_handle_count += 2;

		d3d12->command_list->SetPipelineState(d3d12->blit_to_swap_chain_pipeline_state);
		d3d12->command_list->SetGraphicsRootSignature(d3d12->blit_to_swap_chain_root_signature);
		d3d12->command_list->SetDescriptorHeaps(1, &world->frame_descriptor_heap);
		d3d12->command_list->SetGraphicsRootDescriptorTable(0, world_frame_gpu_descriptor_handle);
		d3d12->command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d12->command_list->DrawInstanced(3, 1, 0, 0);

		imgui_render_commands(editor, d3d12, window);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		d3d12->command_list->ResourceBarrier(1, &barrier);

		d3d12->command_list->Close();

		d3d12->command_queue->ExecuteCommandLists(1, (ID3D12CommandList **)&d3d12->command_list);
		m_d3d_assert(d3d12->swap_chain->Present(1, 0));

		d3d12_wait_command_list(d3d12);
		d3d12->swap_chain_buffer_index = d3d12->swap_chain->GetCurrentBackBufferIndex();
	}
	save_editor_options("editor_options.txt", editor);
	ImGui::DestroyContext(editor->imgui_context);

	// d3d12->dxgi_debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL);
	d3d12->dxgi_info_queue->SetMuteDebugOutput(DXGI_DEBUG_ALL, true);
}

