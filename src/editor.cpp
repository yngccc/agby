/***************************************************************************************************/
/*					Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.					 */
/***************************************************************************************************/

#include "common.cpp"
#include "math.cpp"
#include "d3d11.cpp"
#include "world.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include <imgui/imgui.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/wip/imgui_tabs.cpp>
#include <imgui/ImGuizmo.cpp>

struct {
	bool initialized;
	bool quit;
	window *window;
	d3d *d3d;
} window_message_channel = {};

LRESULT handle_window_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	LRESULT result = 0;
	if (!window_message_channel.initialized) {
		result = DefWindowProcA(hwnd, msg, wparam, lparam);
	}
	else {
		window *window = window_message_channel.window;
		d3d *d3d = window_message_channel.d3d;
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
			set_window_title(window, "%dx%d", window->width, window->height);
			resize_d3d_swap_chain(d3d, window->width, window->height);
			ImGui::GetIO().DisplaySize = {(float)d3d->swap_chain_desc.Width, (float)d3d->swap_chain_desc.Height};
			ImGui::GetIO().FontGlobalScale = (float)d3d->swap_chain_desc.Width / (float)GetSystemMetrics(SM_CXSCREEN);
			ImGuizmo::SetRect(0, 0, (float)d3d->swap_chain_desc.Width, (float)d3d->swap_chain_desc.Height);
		} break;
		case WM_SHOWWINDOW : {
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
			if (!cursor_inside_window(window)) {
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

enum edit_window_tab {
	edit_window_tab_player,
	edit_window_tab_static_object,
	edit_window_tab_dynamic_object,
	edit_window_tab_model,
	edit_window_tab_light,
	edit_window_tab_terrain,
	edit_window_tab_skybox,
};

const char *edit_window_tab_strs[] = {"Player", "StaticObject", "DynamicObject", "Model", "Light", "Terrain", "Skybox"};

enum tool_type {
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

struct editor {
	bool quit;

	timer timer;
	double last_frame_time_secs;

	ImGuiContext *imgui_context;

	ID3D11Texture2D *imgui_font_atlas_texture;
	ID3D11ShaderResourceView *imgui_font_atlas_texture_view;
	ID3D11Buffer *imgui_vertex_buffer;

	ID3D11Texture2D *translate_icon_texture;
	ID3D11ShaderResourceView *translate_icon_texture_view;
	ID3D11Texture2D *rotate_icon_texture;
	ID3D11ShaderResourceView *rotate_icon_texture_view;
	ID3D11Texture2D *scale_icon_texture;
	ID3D11ShaderResourceView *scale_icon_texture_view;
	ID3D11Texture2D *terrain_bump_icon_texture;
	ID3D11ShaderResourceView *terrain_bump_icon_texture_view;
	ID3D11Texture2D *terrain_raise_lower_icon_texture;
	ID3D11ShaderResourceView *terrain_raise_lower_icon_texture_view;
	ID3D11Texture2D *terrain_flatten_icon_texture;
	ID3D11ShaderResourceView *terrain_flatten_icon_texture_view;
	ID3D11Texture2D *terrain_height_icon_texture;
	ID3D11ShaderResourceView *terrain_height_icon_texture_view;
	ID3D11Texture2D *terrain_ramp_icon_texture;
	ID3D11ShaderResourceView *terrain_ramp_icon_texture_view;
	ID3D11Texture2D *terrain_smooth_icon_texture;
	ID3D11ShaderResourceView *terrain_smooth_icon_texture_view;
	ID3D11Texture2D *terrain_noise_icon_texture;
	ID3D11ShaderResourceView *terrain_noise_icon_texture_view;
	ID3D11Texture2D *terrain_water_icon_texture;
	ID3D11ShaderResourceView *terrain_water_icon_texture_view;
	ID3D11Texture2D *terrain_hole_icon_texture;
	ID3D11ShaderResourceView *terrain_hole_icon_texture_view;
	ID3D11Texture2D *terrain_paint_icon_texture;
	ID3D11ShaderResourceView *terrain_paint_icon_texture_view;
	ID3D11Texture2D *terrain_tree_icon_texture;
	ID3D11ShaderResourceView *terrain_tree_icon_texture_view;
	ID3D11Texture2D *terrain_road_icon_texture;
	ID3D11ShaderResourceView *terrain_road_icon_texture_view;

	ID3D11Texture2D **terrain_paint_textures;
	ID3D11ShaderResourceView **terrain_paint_texture_views;
	uint32 terrain_paint_texture_count;
	uint32 terrain_paint_texture_index;
	
	float top_menu_bar_height;
	ImVec4 top_menu_bar_color;
	ImVec2 edit_window_pos;
	ImVec2 edit_window_size;
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
	XMVECTOR camera_position;
	XMVECTOR camera_view;
	float camera_fovy;
	float camera_znear;
	float camera_zfar;
	float camera_move_speed;
	float camera_rotate_speed;
	XMMATRIX camera_view_mat;
	XMMATRIX camera_proj_mat;
	XMMATRIX camera_view_proj_mat;
	ray camera_to_mouse_ray;

	bool render_reference_grid;
	bool render_models;
	bool render_terrain;
	bool render_skybox;
	bool render_shadow_proj_box;

	bool focus_model;

	bool show_frame_statistic_window;
	bool show_camera_settings_window;

	int32 edit_window_tab;
	uint32 static_object_index;
	uint32 dynamic_object_index;
	uint32 model_index;

	int32 tool_type;
	bool terrain_brush_tool_active;
	vec3 terrain_brush_tool_position;
	float terrain_brush_tool_radius;
	float terrain_brush_tool_speed;

	uint64 world_frame_memory_arena_size;
	char world_save_file[256];
};

void initialize_editor(editor *editor, d3d *d3d) {
	*editor = {};
	initialize_timer(&editor->timer);
	{ // imgui
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

			ImGui::GetIO().DisplaySize = {(float)d3d->swap_chain_desc.Width, (float)d3d->swap_chain_desc.Height};
			ImGui::GetIO().FontGlobalScale = (float)d3d->swap_chain_desc.Width / (float)GetSystemMetrics(SM_CXSCREEN);
			ImGuizmo::SetRect(0, 0, (float)d3d->swap_chain_desc.Width, (float)d3d->swap_chain_desc.Height);
		}
		{
			uint8 *texture;
			int32 texture_width, texture_height;
			m_assert(ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", (float)GetSystemMetrics(SM_CXSCREEN) / 100), "");
			ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&texture, &texture_width, &texture_height);
			D3D11_TEXTURE2D_DESC texture_desc = {};
			texture_desc.Width = texture_width;
			texture_desc.Height = texture_height;
			texture_desc.MipLevels = 1;
			texture_desc.ArraySize = 1;
			texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			texture_desc.SampleDesc.Count = 1;
			texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
			texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			D3D11_SUBRESOURCE_DATA texture_subresource_data = {texture, texture_width * 4u, 0};
			m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, &editor->imgui_font_atlas_texture));
			m_d3d_assert(d3d->device->CreateShaderResourceView(editor->imgui_font_atlas_texture, nullptr, &editor->imgui_font_atlas_texture_view));
			ImGui::GetIO().Fonts->SetTexID((ImTextureID)editor->imgui_font_atlas_texture_view);
			ImGui::GetIO().Fonts->ClearTexData();
		}
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.ByteWidth = m_megabytes(2);
			buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
			buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_INDEX_BUFFER;
			m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, nullptr, &editor->imgui_vertex_buffer));
		}
	}
	{ // textures
		auto create_texture = [&](const char *file, DXGI_FORMAT fmt, ID3D11Texture2D **texture, ID3D11ShaderResourceView **shader_resource_view) {
			uint8* texture_data;
			int32 width, height, channel;
			texture_data = stbi_load(file, &width, &height, &channel, 4);
			m_assert(texture_data, "");
			D3D11_TEXTURE2D_DESC texture_desc = {};
			texture_desc.Width = width;
			texture_desc.Height = height;
			texture_desc.MipLevels = 1;
			texture_desc.ArraySize = 1;
			texture_desc.Format = fmt;
			texture_desc.SampleDesc.Count = 1;
			texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
			texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			D3D11_SUBRESOURCE_DATA texture_subresource_data = {texture_data, width * 4u, 0};
			m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, texture));
			m_d3d_assert(d3d->device->CreateShaderResourceView(*texture, nullptr, shader_resource_view));
			stbi_image_free(texture_data);
		};
	
		create_texture("assets/images/icons/translate.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->translate_icon_texture, &editor->translate_icon_texture_view);
		create_texture("assets/images/icons/rotate.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->rotate_icon_texture, &editor->rotate_icon_texture_view);
		create_texture("assets/images/icons/scale.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->scale_icon_texture, &editor->scale_icon_texture_view);
		create_texture("assets/images/icons/terrain_bump.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_bump_icon_texture, &editor->terrain_bump_icon_texture_view);
		create_texture("assets/images/icons/terrain_raise_lower.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_raise_lower_icon_texture, &editor->terrain_raise_lower_icon_texture_view);
		create_texture("assets/images/icons/terrain_flatten.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_flatten_icon_texture, &editor->terrain_flatten_icon_texture_view);
		create_texture("assets/images/icons/terrain_height.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_height_icon_texture, &editor->terrain_height_icon_texture_view);
		create_texture("assets/images/icons/terrain_ramp.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_ramp_icon_texture, &editor->terrain_ramp_icon_texture_view);
		create_texture("assets/images/icons/terrain_smooth.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_smooth_icon_texture, &editor->terrain_smooth_icon_texture_view);
		create_texture("assets/images/icons/terrain_noise.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_noise_icon_texture, &editor->terrain_noise_icon_texture_view);
		create_texture("assets/images/icons/terrain_water.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_water_icon_texture, &editor->terrain_water_icon_texture_view);
		create_texture("assets/images/icons/terrain_hole.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_hole_icon_texture, &editor->terrain_hole_icon_texture_view);
		create_texture("assets/images/icons/terrain_paint.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_paint_icon_texture, &editor->terrain_paint_icon_texture_view);
		create_texture("assets/images/icons/terrain_tree.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_tree_icon_texture, &editor->terrain_tree_icon_texture_view);
		create_texture("assets/images/icons/terrain_road.png", DXGI_FORMAT_R8G8B8A8_UNORM, &editor->terrain_road_icon_texture, &editor->terrain_road_icon_texture_view);
	
		editor->terrain_paint_texture_count = 0;
		iterate_files_in_dir("assets/images/terrain", [&](const char *file_name) {
			editor->terrain_paint_texture_count += 1;
		});
		editor->terrain_paint_textures = new ID3D11Texture2D*[editor->terrain_paint_texture_count];
		editor->terrain_paint_texture_views = new ID3D11ShaderResourceView*[editor->terrain_paint_texture_count];
		uint32 index = 0;
		iterate_files_in_dir("assets/images/terrain", [&](const char *file_name) {
			char path[256];
			snprintf(path, sizeof(path), "assets/images/terrain/%s", file_name);
			create_texture(path, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, &editor->terrain_paint_textures[index], &editor->terrain_paint_texture_views[index]);
			index += 1;
		});
	}
	{ // misc
		editor->camera_position = XMVectorSet(20, 20, 20, 0);
		editor->camera_view = XMVector3Normalize(XMVectorNegate(editor->camera_position));
		editor->camera_fovy = XMConvertToRadians(40);
		editor->camera_znear = 0.1f;
		editor->camera_zfar = 10000.0f;
		editor->camera_move_speed = 10;
		editor->camera_rotate_speed = 5;
		
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
	}
}

bool save_editor_world(editor *editor, world *world, bool save_as) {
	char world_save_file[256] = {};
	if (save_as) {
		if (!open_file_dialog(world_save_file, sizeof(world_save_file))) {
			return false;
		}
	}
	else {
		if (!strcmp(editor->world_save_file, "")) {
			if (!open_file_dialog(world_save_file, sizeof(world_save_file))) {
				return false;
			}
		}
		else {
			array_copy(world_save_file, editor->world_save_file);
		}
	}
	if (!save_world(world, world_save_file)) {
		return false;
	}
	for (uint32 i = 0; i < world->terrain_count; i += 1) {
		terrain *terrain = &world->terrains[i];
		m_assert(terrain->height_texture_data, "");
		m_assert(terrain->diffuse_texture_data, "");
		file_mapping terrain_file_mapping;
		char file_path[128] = "assets/terrains/";
		strcat(file_path, terrain->file);
		m_assert(open_file_mapping(file_path, &terrain_file_mapping, false), "");
		gpk_terrain *gpk_terrain = (struct gpk_terrain *)terrain_file_mapping.ptr;
		uint8 *height_texture_ptr = terrain_file_mapping.ptr + gpk_terrain->height_map_offset;
		uint8 *diffuse_texture_ptr = terrain_file_mapping.ptr + gpk_terrain->diffuse_map_offset;
		uint32 texture_size = terrain->width * terrain->sample_per_meter * terrain->height * terrain->sample_per_meter;
		memcpy(height_texture_ptr, terrain->height_texture_data, texture_size * sizeof(int16));
		memcpy(diffuse_texture_ptr, terrain->diffuse_texture_data, texture_size * sizeof(uint32));
    flush_file_mapping(terrain_file_mapping);
		close_file_mapping(terrain_file_mapping);
	}
	if (!save_as) {
		array_copy(editor->world_save_file, world_save_file);
	}
	return true;
}

void check_quit(editor *editor) {
	if (window_message_channel.quit) {
		window_message_channel.quit = false;
		editor->quit_popup = true;
	}
	if (ImGui::IsKeyPressed(VK_F4) && ImGui::IsKeyDown(VK_MENU)) {
		editor->quit_popup = true;
	}
}

void check_toggle_fullscreen(window *window) {
	if (ImGui::IsKeyPressed(VK_F11)) {
		set_window_fullscreen(window, !window->fullscreen);
	}
}

void check_popups(editor *editor, world *world, d3d *d3d) {
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
		ImGui::OpenPopup("##quit_popup");
	}
	if (ImGui::BeginPopupModal("##quit_popup", nullptr, window_flags)) {
		ImGui::Text("Save changes?");
		if (ImGui::Button("Yes")) {
			if (save_editor_world(editor, world, false)) {
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
			ImGui::TextColored({1, 0, 0, 1}, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool full_capacity = world->static_object_count >= world->static_object_capacity;
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (uint32 i = 0; i < world->static_object_count; i += 1) {
				if (!strcmp(id, world->static_objects[i].id)) {
					duplicate = true;
					break;
				}
			}
			if (full_capacity) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: object at full capacity");
			}
			else if (empty_id) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID is empty");
			}
			else if (duplicate) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID already exist");
			}
			else {
				editor->static_object_index = world->static_object_count;
				static_object *static_object = &world->static_objects[world->static_object_count++];
				static_object->model_index = UINT32_MAX;
				static_object->transform = transform_identity();
				static_object->animation_index = UINT32_MAX;
				static_object->animation_time = 0;
				static_object->id = allocate_memory<char>(&world->general_memory_arena, strlen(id) + 1);
				strcpy(static_object->id, id);
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
			ImGui::TextColored({1, 0, 0, 1}, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (uint32 i = 0; i < world->static_object_count; i += 1) {
				if (!strcmp(id, world->static_objects[i].id) && i != editor->static_object_index) {
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
				static_object->id = allocate_memory<char>(&world->general_memory_arena, strlen(id) + 1);
				strcpy(static_object->id, id);
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
			ImGui::TextColored({1, 0, 0, 1}, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool full_capacity = world->dynamic_object_count >= world->dynamic_object_capacity;
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (uint32 i = 0; i < world->dynamic_object_count; i += 1) {
				if (!strcmp(id, world->dynamic_objects[i].id)) {
					duplicate = true;
					break;
				}
			}
			if (full_capacity) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: object at full capacity");
			}
			else if (empty_id) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID is empty");
			}
			else if (duplicate) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "error: ID already exist");
			}
			else {
				editor->dynamic_object_index = world->dynamic_object_count;
				dynamic_object *dynamic_object = &world->dynamic_objects[world->dynamic_object_count++];
				dynamic_object->model_index = UINT32_MAX;
				dynamic_object->transform = transform_identity();
				dynamic_object->animation_index = UINT32_MAX;
				dynamic_object->animation_time = 0;
				dynamic_object->id = allocate_memory<char>(&world->general_memory_arena, strlen(id) + 1);
				strcpy(dynamic_object->id, id);
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
			ImGui::TextColored({1, 0, 0, 1}, "%s", error_msg);
		}
		if (ImGui::Button("Ok")) {
			bool empty_id = id[0] == 0;
			bool duplicate = false;
			for (uint32 i = 0; i < world->dynamic_object_count; i += 1) {
				if (!strcmp(id, world->dynamic_objects[i].id) && i != editor->dynamic_object_index) {
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
				dynamic_object->id = allocate_memory<char>(&world->general_memory_arena, strlen(id) + 1);
				strcpy(dynamic_object->id, id);
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
			if (!create_file_mapping(file, terrain.total_size, &terrain_file_mapping)) {
				error = true;
				snprintf(error_msg, sizeof(error_msg), "Error: create_file_mapping failed\nFile: %s", file);
			}
			else {
				*(gpk_terrain *)terrain_file_mapping.ptr = terrain;
				int16 *height_map = (int16 *)(terrain_file_mapping.ptr + terrain.height_map_offset);
				uint8 *diffuse_map = (uint8 *)(terrain_file_mapping.ptr + terrain.diffuse_map_offset);
				memset(height_map, 0, texture_size * sizeof(int16));
				memset(diffuse_map, 150, texture_size * sizeof(uint32));
				flush_file_mapping(terrain_file_mapping);
				close_file_mapping(terrain_file_mapping);
				add_terrain(world, d3d, file);
				close_popup();
			}
		};
		if (error) {
			ImGui::TextColored({1, 0, 0, 1}, "%s", error_msg);
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

void top_menu(editor *editor, world *world, d3d *d3d) {
	ImGui::PushID("top_menu_bar");
	if (ImGui::BeginMainMenuBar()) {
		editor->top_menu_bar_height = ImGui::GetWindowHeight();
		editor->top_menu_bar_color = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);
		ImGui::PushID("file");
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open")) {
				if (open_file_dialog(editor->world_save_file, sizeof(editor->world_save_file))) {
					if (!load_world(world, d3d, editor->world_save_file)) {
						snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to load level from file: \"%s\"", editor->world_save_file);
						editor->error_popup = true;
						editor->world_save_file[0] = '\0';
					}
				}
			}
			if (ImGui::MenuItem("Save")) {
				if (!save_editor_world(editor, world, false)) {
					snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to save world to file: \"%s\"", editor->world_save_file);
					editor->error_popup = true;
				}
			}
			if (ImGui::MenuItem("Save As")) {
				if (!save_editor_world(editor, world, true)) {
					snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to save world to file");
					editor->error_popup = true;
				}
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Play")) {
				if (!save_editor_world(editor, world, false)) {
					snprintf(editor->error_msg, sizeof(editor->error_msg), "failed to save world to file: \"%s\"", editor->world_save_file);
					editor->error_popup = true;
				}
				else {
					STARTUPINFOA startup_info = {sizeof(startup_info)};
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
			ImGui::MenuItem("Reference Grid", nullptr, &editor->render_reference_grid);
			ImGui::MenuItem("Models", nullptr, &editor->render_models);
			ImGui::MenuItem("Terrain", nullptr, &editor->render_terrain);
			ImGui::MenuItem("Skybox", nullptr, &editor->render_skybox);
			ImGui::Separator();
			ImGui::MenuItem("ShadowProjBox", nullptr, &editor->render_shadow_proj_box);
			ImGui::EndMenu();
		}
		ImGui::PopID();
		ImGui::PushID("settings");
		if (ImGui::BeginMenu("Settings")) {
			ImGui::PushID("camera");
			if (ImGui::BeginMenu("Camera")) {
				ImGui::SliderFloat("Move Speed", &editor->camera_move_speed, 0, 100);
				ImGui::SliderFloat("Rotate Speed", &editor->camera_rotate_speed, 0, 1);
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

void bottom_menu(editor *editor) {
	ImGui::SetNextWindowPos(ImVec2{0, ImGui::GetIO().DisplaySize.y - editor->top_menu_bar_height * 1.5f});
	ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x, editor->top_menu_bar_height * 1.5f});
	ImGui::PushID("bottom_menu_bar");
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, editor->top_menu_bar_color);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	if (ImGui::Begin("##bottom_menu_bar", nullptr, flags)) {
		ImGui::Dummy({ImGui::GetIO().DisplaySize.x / 2, 1});
		ImGui::SameLine();
		float size = ImGui::GetWindowHeight() / 8 * 6;
		float padding = ImGui::GetWindowHeight() / 8;
		ImVec4 button_hovered_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
		ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.8f, 0.8f, 1});
		auto render_tool_button = [&](tool_type type, ID3D11ShaderResourceView *texture_view) {
			ImGui::SameLine();
			if (editor->tool_type == type) {
				ImGui::PushStyleColor(ImGuiCol_Button, button_hovered_color);
				if (ImGui::ImageButton((ImTextureID)texture_view, {size, size}, {0, 0},  {1, 1}, (int32)padding)) {
					editor->tool_type = type;
				}
				ImGui::PopStyleColor();
			}
			else {
				if (ImGui::ImageButton((ImTextureID)texture_view, {size, size}, {0, 0},  {1, 1}, (int32)padding)) {
					editor->tool_type = type;
				}
			}
		};
		for (auto type : {edit_window_tab_player, edit_window_tab_static_object, edit_window_tab_dynamic_object, edit_window_tab_model}) {
			if (editor->edit_window_tab == type) {
				render_tool_button(tool_type_translate, editor->translate_icon_texture_view);
				render_tool_button(tool_type_rotate, editor->rotate_icon_texture_view);
				render_tool_button(tool_type_scale, editor->scale_icon_texture_view);
				break;
			}
		}
		if (editor->edit_window_tab == edit_window_tab_terrain) {
			render_tool_button(tool_type_terrain_bump, editor->terrain_bump_icon_texture_view);
			render_tool_button(tool_type_terrain_raise_lower, editor->terrain_raise_lower_icon_texture_view);
			render_tool_button(tool_type_terrain_flatten, editor->terrain_flatten_icon_texture_view);
			render_tool_button(tool_type_terrain_height, editor->terrain_height_icon_texture_view);
			render_tool_button(tool_type_terrain_ramp, editor->terrain_ramp_icon_texture_view);
			render_tool_button(tool_type_terrain_smooth, editor->terrain_smooth_icon_texture_view);
			render_tool_button(tool_type_terrain_noise, editor->terrain_noise_icon_texture_view);
			render_tool_button(tool_type_terrain_water, editor->terrain_water_icon_texture_view);
			render_tool_button(tool_type_terrain_hole, editor->terrain_hole_icon_texture_view);
			render_tool_button(tool_type_terrain_paint, editor->terrain_paint_icon_texture_view);
			render_tool_button(tool_type_terrain_tree, editor->terrain_tree_icon_texture_view);
			render_tool_button(tool_type_terrain_road, editor->terrain_road_icon_texture_view);
		}
		ImGui::PopStyleColor();
	}
	ImGui::End();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(1);
	ImGui::PopID();
}

void edit_window_model_transform(world *world, uint32 *model_index, transform *transform) {
	const char *model_file = *model_index < world->model_count ? world->models[*model_index].file : nullptr;
	if (ImGui::BeginCombo("model", model_file)) {
		for (uint32 i = 0; i < world->model_count; i += 1) {
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
		transform->scale = {scale, scale, scale};
	}
	if (ImGui::Button("Reset Transform")) {
		*transform = transform_identity();
	}
}

void edit_window_player_tab(editor *editor, world *world) {
	ImGui::PushID("player_tab");
	edit_window_model_transform(world, &world->player.model_index, &world->player.transform);
	ImGui::PopID();
}

void edit_window_static_object_tab(editor *editor, world *world) {
	ImGui::PushID("static_object_tab");
	if (ImGui::Button("Add")) {
		editor->add_static_object_popup = true;
	}
	static_object *static_object = editor->static_object_index < world->static_object_count ? &world->static_objects[editor->static_object_index] : nullptr;
	const char *id = static_object ? static_object->id : nullptr;
	if (ImGui::BeginCombo("static objects", id)) {
		for (uint32 i = 0; i < world->static_object_count; i += 1) {
			if (ImGui::Selectable(world->static_objects[i].id, editor->static_object_index == i)) {
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
		if (ImGui::Button("Delete")) {
			array_remove(world->static_objects, &world->static_object_count, editor->static_object_index);
			static_object = nullptr;
		}
	}
	if (static_object) {
		edit_window_model_transform(world, &static_object->model_index, &static_object->transform);
	}
	ImGui::PopID();
}

void edit_window_dynamic_object_tab(editor *editor, world *world) {
	ImGui::PushID("dynamic_object_tab");
	if (ImGui::Button("Add")) {
		editor->add_dynamic_object_popup = true;
	}
	dynamic_object *dynamic_object = editor->dynamic_object_index < world->dynamic_object_count ? &world->dynamic_objects[editor->dynamic_object_index] : nullptr;
	const char *id = dynamic_object ? dynamic_object->id : nullptr;
	if (ImGui::BeginCombo("dynamic objects", id)) {
		for (uint32 i = 0; i < world->dynamic_object_count; i += 1) {
			if (ImGui::Selectable(world->dynamic_objects[i].id, editor->dynamic_object_index == i)) {
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
		if (ImGui::Button("Delete")) {
			array_remove(world->dynamic_objects, &world->dynamic_object_count, editor->dynamic_object_index);
			dynamic_object = nullptr;
		}
	}
	if (dynamic_object) {
		edit_window_model_transform(world, &dynamic_object->model_index, &dynamic_object->transform);
	}
	ImGui::PopID();
}

void edit_window_model_tab(editor *editor, world *world, d3d *d3d) {
	ImGui::PushID("model_tab");
	if (ImGui::Button("Import")) {
		char file[256] = {};
		if (open_file_dialog(file, sizeof(file))) {
			if (add_model(world, d3d, file, transform_identity(), collision{collision_type_none})) {
				editor->model_index = world->model_count - 1;
			}
			else {
				snprintf(editor->error_msg, sizeof(editor->error_msg), "Add model failed\nFile: %s", file);
				editor->error_popup = true;
			}
		}
	}
	ImGui::Separator();
	model *model = editor->model_index < world->model_count ? &world->models[editor->model_index] : nullptr;
	const char *file = model ? model->file : nullptr;
	if (ImGui::BeginCombo("models", file)) {
		for (uint32 i = 0; i < world->model_count; i += 1) {
			if (ImGui::Selectable(world->models[i].file, editor->model_index == i)) {
				editor->model_index = i;
				model = &world->models[i];
			}
		}
		ImGui::EndCombo();
	}
	if (model) {
		ImGui::SameLine();
		ImGui::Checkbox("Focus", &editor->focus_model);
		ImGui::InputFloat3("T", &model->transform.translate[0]);
		ImGui::InputFloat3("R", &model->transform.rotate[0], "%.3f", ImGuiInputTextFlags_ReadOnly);
		float scale = model->transform.scale[0];
		if (ImGui::InputFloat("S", &scale) && scale > 0) {
			model->transform.scale = {scale, scale, scale};
		}
		if (ImGui::Button("Reset Transform")) {
			model->transform = transform_identity();
		}
		if (model->collision.type == collision_type_none) {
			if (ImGui::Button("Add Collision")) {
				model->collision.type = collision_type_sphere;
				model->collision.sphere = collision_sphere{1};
			}
		}
		else {
			const char *preview_str = collision_type_strs[model->collision.type];
			if (ImGui::BeginCombo("shapes", preview_str)) {
				if (ImGui::Selectable("sphere", model->collision.type == collision_type_sphere)) {
					if (model->collision.type != collision_type_sphere) {
						model->collision.type = collision_type_sphere;
						model->collision.sphere = collision_sphere{1};
					}
				}
				if (ImGui::Selectable("box", model->collision.type == collision_type_box)) {
					if (model->collision.type != collision_type_box) {
						model->collision.type = collision_type_box;
						model->collision.box = collision_box{1, 1, 1};
					}
				}
				if (ImGui::Selectable("capsule_x", model->collision.type == collision_type_capsule_x)) {
					if (model->collision.type != collision_type_capsule_x) {
						model->collision.type = collision_type_capsule_x;
						model->collision.capsule = collision_capsule{1, 2};
					}
				}
				if (ImGui::Selectable("capsule_y", model->collision.type == collision_type_capsule_y)) {
					if (model->collision.type != collision_type_capsule_y) {
						model->collision.type = collision_type_capsule_y;
						model->collision.capsule = collision_capsule{1, 2};
					}
				}
				if (ImGui::Selectable("capsule_z", model->collision.type == collision_type_capsule_z)) {
					if (model->collision.type != collision_type_capsule_z) {
						model->collision.type = collision_type_capsule_z;
						model->collision.capsule = collision_capsule{1, 2};
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
			if (model->collision.type == collision_type_capsule_x || model->collision.type == collision_type_capsule_y || model->collision.type == collision_type_capsule_z) {
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

void edit_window_light_tab(editor *editor, world *world) {
	ImGui::PushID("light_tab");
	ImGui::ColorEdit3("sun light color", &world->sun_light_color.x);
	float extra = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x * 2 + ImGui::CalcTextSize("direct color").x;
	ImGui::PushItemWidth(ImGui::GetItemRectSize().x - extra);
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::DragFloat3("", &world->sun_light_dir.x);
	ImGui::PopItemFlag();
	ImGui::PopItemWidth();
	ImGui::PopID();
}

void edit_window_terrain_tab(editor *editor, world *world, d3d *d3d) {
	ImGui::PushID("terrain_tab");
	if (ImGui::Button("New")) {
		editor->new_terrain_popup = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Import")) {
		char file[256] = {};
		if (open_file_dialog(file, sizeof(file))) {
			if (file_exists(file)) {
				if (!add_terrain(world, d3d, file)) {
					editor->error_popup = true;
					snprintf(editor->error_msg, sizeof(editor->error_msg), "Add terrain failed\nFile: %s", file);
				}
			}
			else {
				editor->error_popup = true;
				snprintf(editor->error_msg, sizeof(editor->error_msg), "File does not exist\nFile: %s", file);
			}
		}
	}
	terrain *terrain = world->terrain_index < world->terrain_count ? &world->terrains[world->terrain_index] : nullptr;
	const char *file = terrain ? terrain->file : nullptr;
	if (ImGui::BeginCombo("terrains", file)) {
		for (uint32 i = 0; i < world->terrain_count; i += 1) {
			if (ImGui::Selectable(world->terrains[i].file, world->terrain_index == i)) {
				world->terrain_index = i;
				terrain = &world->terrains[i];
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}

void edit_window_skybox_tab(editor *editor, world *world, d3d *d3d) {
	ImGui::PushID("skybox_tab");
	if (ImGui::Button("Import")) {
		char file[256] = {};
		if (open_file_dialog(file, sizeof(file))) {
			if (!add_skybox(world, d3d, file)) {
				snprintf(editor->error_msg, sizeof(editor->error_msg), "Add Skybox Failed\nFile: %s", file);
				editor->error_popup = true;
			}
		}
	}
	skybox *skybox = world->skybox_index < world->skybox_count ? &world->skyboxes[world->skybox_index] : nullptr;
	const char *file = skybox ? skybox->file : nullptr;
	if (ImGui::BeginCombo("skyboxes", file)) {
		for (uint32 i = 0; i < world->skybox_count; i += 1) {
			if (ImGui::Selectable(world->skyboxes[i].file, world->skybox_index == i)) {
				world->skybox_index = i;
				skybox = &world->skyboxes[i];
			}
		}
		ImGui::EndCombo();
	}
	if (skybox) {
		if (ImGui::Button("Delete")) {
			skybox->cube_texture_view->Release();
			skybox->cube_texture->Release();
			array_remove(world->skyboxes, &world->skybox_count, world->skybox_index);
			world->skybox_index = UINT32_MAX;
		}
	}
	ImGui::PopID();
}

void edit_window(editor *editor, world *world, d3d *d3d) {
	ImGui::SetNextWindowPos(ImVec2{0, editor->top_menu_bar_height}, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.4f}, ImGuiCond_Once);
	ImGui::PushID("edit_window");
	if (ImGui::Begin("Edit", nullptr, ImGuiWindowFlags_NoCollapse)) {
		editor->edit_window_pos = ImGui::GetWindowPos();
		editor->edit_window_size = ImGui::GetWindowSize();
		ImGui::BeginTabBar("edit_window_tab_type", ImGuiTabBarFlags_NoReorder);
		for (int32 i = 0; i < m_countof(edit_window_tab_strs); i += 1) {
			if (ImGui::TabItem(edit_window_tab_strs[i])) {
				editor->edit_window_tab = i;
			}
		}
		ImGui::EndTabBar();
		ImGui::Separator();
		switch (editor->edit_window_tab) {
		case edit_window_tab_player: {
			edit_window_player_tab(editor, world);
		} break;
		case edit_window_tab_static_object: {
			edit_window_static_object_tab(editor, world);
		} break;
		case edit_window_tab_dynamic_object: {
			edit_window_dynamic_object_tab(editor, world);
		} break;
		case edit_window_tab_model: {
			edit_window_model_tab(editor, world, d3d);
		} break;
		case edit_window_tab_light: {
			edit_window_light_tab(editor, world);
		} break;
		case edit_window_tab_terrain: {
			edit_window_terrain_tab(editor, world, d3d);
		} break;
		case edit_window_tab_skybox: {
			edit_window_skybox_tab(editor, world, d3d);
		} break;
		}
	}
	ImGui::End();
	ImGui::PopID();
}

void memories_window(editor *editor, world *world, d3d *d3d) {
	ImGui::SetNextWindowPos(ImVec2{0, editor->edit_window_pos.y + editor->edit_window_size.y}, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.2f}, ImGuiCond_Once);
	ImGui::PushID("memory_usage_window");
	if (ImGui::Begin("Memory Usage", nullptr, ImGuiWindowFlags_NoCollapse)) {
		editor->memory_window_pos = ImGui::GetWindowPos();
		editor->memory_window_size = ImGui::GetWindowSize();
		auto imgui_render_memory = [](uint64 memory_size, uint64 memory_capacity, const char *memory_name) {
			char overlay[64] = {};
			snprintf(overlay, sizeof(overlay), "%s / %s", pretty_print_bytes(memory_size).data(), pretty_print_bytes(memory_capacity).data());
			ImGui::ProgressBar((float)((double)memory_size / (double)memory_capacity), ImVec2{ImGui::GetWindowContentRegionWidth() * 0.5f, 0}, overlay);
			ImGui::SameLine();
			ImGui::Text("%s", memory_name);
		};
		ImGui::Text("Memory Arenas");
		imgui_render_memory(world->general_memory_arena.size, world->general_memory_arena.capacity, "World General");
		imgui_render_memory(editor->world_frame_memory_arena_size, world->frame_memory_arena.capacity, "World Frame");
		// ImGui::Text("Vulkan Memories");
		// imgui_render_memory(vulkan->memory_regions.images.memory_size, vulkan->memory_regions.images.memory_capacity, "Images");
		// imgui_render_memory(vulkan->memory_regions.vertex_buffer.buffer_size, vulkan->memory_regions.vertex_buffer.buffer_capacity, "Vertex Buffer");
		// auto frame_uniform_buffer = &vulkan->memory_regions.frame_uniform_buffers[vulkan->buffering_index];
		// auto frame_vertex_buffer = &vulkan->memory_regions.frame_vertex_buffers[vulkan->buffering_index];
		// imgui_render_memory(frame_uniform_buffer->buffer_size, frame_uniform_buffer->buffer_capacity, "Frame Uniform Buffer");
		// imgui_render_memory(frame_vertex_buffer->buffer_size, frame_vertex_buffer->buffer_capacity, "Frame Vertex Buffer");
	}
	ImGui::End();
	ImGui::PopID();
}

void frame_statistic_window(editor *editor) {
	if (editor->show_frame_statistic_window) {
		ImGui::PushID("frame_statistic_window");
		if (ImGui::Begin("Frame statistic", &editor->show_frame_statistic_window)) {
			ImGui::Text("Frame time: %.3f", editor->last_frame_time_secs);
		}
		ImGui::End();
		ImGui::PopID();
	}
}

void update_camera(editor *editor, window *window) {
	if (ImGui::IsMouseClicked(2)) {
		pin_cursor(true);
		editor->camera_active = true;
	}
	if (ImGui::IsMouseReleased(2)) {
		pin_cursor(false);
		editor->camera_active = false;
	}
	if (ImGui::IsKeyDown(VK_UP) && ImGui::IsKeyDown(VK_CONTROL)) {
		editor->camera_position = XMVectorAdd(editor->camera_position, XMVectorScale(editor->camera_view, (float)editor->last_frame_time_secs * 20));
	}
	if (ImGui::IsKeyDown(VK_DOWN) && ImGui::IsKeyDown(VK_CONTROL)) {
		editor->camera_position = XMVectorSubtract(editor->camera_position, XMVectorScale(editor->camera_view, (float)editor->last_frame_time_secs * 20));
	}
	if (editor->camera_active) {
		float move_distance = editor->camera_move_speed * (float)editor->last_frame_time_secs;
		if (ImGui::IsKeyDown(VK_SHIFT)) {
			move_distance *= 8;
		}
		if (ImGui::IsKeyDown('W')) {
			XMVECTOR move_vec = XMVectorScale(editor->camera_view, move_distance);
			editor->camera_position = XMVectorAdd(editor->camera_position, move_vec);
		}
		if (ImGui::IsKeyDown('S')) {
			XMVECTOR move_vec = XMVectorScale(editor->camera_view, -move_distance);
			editor->camera_position = XMVectorAdd(editor->camera_position, move_vec);
		}
		if (ImGui::IsKeyDown('A')) {
			XMVECTOR move_vec = XMVectorScale(XMVector3Normalize(XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0))), -move_distance);
			editor->camera_position = XMVectorAdd(editor->camera_position, move_vec);
		}
		if (ImGui::IsKeyDown('D')) {
			XMVECTOR move_vec = XMVectorScale(XMVector3Normalize(XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0))), move_distance);
			editor->camera_position = XMVectorAdd(editor->camera_position, move_vec);
		}
		float rotate_distance = editor->camera_rotate_speed * (float)editor->last_frame_time_secs;
		float yaw = window->raw_mouse_dx * rotate_distance;
		editor->camera_view = XMVector3Normalize(XMVector3Rotate(editor->camera_view, XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), -yaw)));
		float max_pitch = XMConvertToRadians(80.0f);
		float pitch = asinf(XMVectorGetY(editor->camera_view));
		float dpitch = -window->raw_mouse_dy * rotate_distance;
		if ((dpitch + pitch) > -max_pitch && (pitch + dpitch) < max_pitch) {
			XMVECTOR axis = XMVector3Normalize(XMVector3Cross(editor->camera_view, XMVectorSet(0, 1, 0, 0)));
			editor->camera_view = XMVector3Normalize(XMVector3Rotate(editor->camera_view, XMQuaternionRotationAxis(axis, dpitch)));
		}
	}
	{
		editor->camera_view_mat = XMMatrixLookAtRH(editor->camera_position, XMVectorAdd(editor->camera_position, editor->camera_view), XMVectorSet(0, 1, 0, 0));
		editor->camera_proj_mat = XMMatrixPerspectiveFovRH(editor->camera_fovy, (float)window->width / (float)window->height, editor->camera_zfar, editor->camera_znear);
		editor->camera_view_proj_mat = XMMatrixMultiply(editor->camera_view_mat, editor->camera_proj_mat);
	}
	if (cursor_inside_window(window)) {
		XMVECTOR world_position = XMVector3Unproject(XMVectorSet(ImGui::GetMousePos().x, ImGui::GetMousePos().y, 1, 0), 0, 0, (float)window->width, (float)window->height, 0, 1, editor->camera_proj_mat, editor->camera_view_mat, XMMatrixIdentity());
		XMVECTOR ray_dir = XMVector3Normalize(XMVectorSubtract(world_position, editor->camera_position));
		editor->camera_to_mouse_ray = {{XMVectorGetX(editor->camera_position), XMVectorGetY(editor->camera_position), XMVectorGetZ(editor->camera_position)}, {XMVectorGetX(ray_dir), XMVectorGetY(ray_dir), XMVectorGetZ(ray_dir)}, editor->camera_zfar};
	}
}

void tool_gizmo(editor *editor, world *world, d3d *d3d, window *window) {
	auto transform_gizmo = [&](transform *transform) {
		for (auto tool_type : {tool_type_translate, tool_type_rotate, tool_type_scale}) {
			if (editor->tool_type == tool_type) {
				vec3 camera_position = {XMVectorGetX(editor->camera_position), XMVectorGetY(editor->camera_position), XMVectorGetZ(editor->camera_position)};
				vec3 camera_view = {XMVectorGetX(editor->camera_view), XMVectorGetY(editor->camera_view), XMVectorGetZ(editor->camera_view)};
				mat4 camera_view_mat = mat4_look_at(camera_position, camera_position + camera_view);
				mat4 camera_proj_mat = mat4_perspective_project(editor->camera_fovy, window->width / (float)window->height, editor->camera_znear, editor->camera_zfar);
				mat4 transform_mat = mat4_from_transform(*transform);
				ImGuizmo::OPERATION operations[] = {ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE};
				ImGuizmo::MODE modes[] = {ImGuizmo::WORLD, ImGuizmo::LOCAL, ImGuizmo::LOCAL};
				ImGuizmo::BeginFrame();
				ImGuizmo::Manipulate((const float *)camera_view_mat, (const float *)camera_proj_mat, operations[editor->tool_type], modes[editor->tool_type], (float *)transform_mat);
				*transform = mat4_get_transform(transform_mat);
				break;
			}
		}
	};
	if (editor->edit_window_tab == edit_window_tab_player) {
		transform_gizmo(&world->player.transform);
	}
	else if (editor->edit_window_tab == edit_window_tab_static_object && editor->static_object_index < world->static_object_count) {
		transform_gizmo(&world->static_objects[editor->static_object_index].transform);
	}
	else if (editor->edit_window_tab == edit_window_tab_dynamic_object && editor->dynamic_object_index < world->dynamic_object_count) {
		transform_gizmo(&world->dynamic_objects[editor->dynamic_object_index].transform);
	}
	else if (editor->edit_window_tab == edit_window_tab_model && editor->model_index < world->model_count) {
		transform_gizmo(&world->models[editor->model_index].transform);
	}
	else if (editor->edit_window_tab == edit_window_tab_terrain && world->terrain_index < world->terrain_count) {
		terrain *terrain = &world->terrains[world->terrain_index];
		float half_width = terrain->width * 0.5f;
		float half_height = terrain->height * 0.5f;
		bool tool_selected = editor->tool_type > tool_type_terrain_begin && editor->tool_type < tool_type_terrain_end;
		if (tool_selected) {
			ImGui::PushID("terrain_brush_properties_window");
			ImGui::SetNextWindowPos(editor->edit_window_pos + ImVec2(editor->edit_window_size.x, 0), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(editor->edit_window_size.x, 0), ImGuiCond_Once);
			if (ImGui::Begin("Terrain Brush")) {
				ImGui::SliderFloat("Radius", &editor->terrain_brush_tool_radius, 1, 10);
				ImGui::SliderFloat("Speed", &editor->terrain_brush_tool_speed, 1, 10);
			}
			ImGui::End();
			ImGui::PopID();
			if (editor->tool_type == tool_type_terrain_paint) {
				ImGui::PushID("terrain_texture_window");
				ImGui::SetNextWindowPos(editor->edit_window_pos + ImVec2(editor->edit_window_size.x * 2, 0), ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2(editor->edit_window_size.x, 0), ImGuiCond_Once);
				if (ImGui::Begin("Terrain Textures")) {
					for (uint32 i = 0; i < editor->terrain_paint_texture_count; i += 1) {
						ImGui::SameLine();
						if (ImGui::ImageButton((ImTextureID)editor->terrain_paint_texture_views[i], ImVec2(32, 32))) {
							editor->terrain_paint_texture_index = i;
						}
					}
				}
				ImGui::End();
				ImGui::PopID();
			}
		}
		if (tool_selected && cursor_inside_window(window) && ray_hit_plane(editor->camera_to_mouse_ray, plane{{0, 1, 0}, 0}, nullptr, &editor->terrain_brush_tool_position) &&
				editor->terrain_brush_tool_position.x >= -half_width && editor->terrain_brush_tool_position.x <= half_width && editor->terrain_brush_tool_position.z >= -half_height && editor->terrain_brush_tool_position.z <= half_height) {
			editor->terrain_brush_tool_active = true;
		}
		if (editor->terrain_brush_tool_active && ImGui::IsMouseDown(0) && !ImGui::GetIO().WantCaptureMouse) {
			static double accumulate_frame_time = 0;
			accumulate_frame_time += editor->last_frame_time_secs;
			if (accumulate_frame_time >= 1.0 / 15.0) {
				accumulate_frame_time = 0;

				if (editor->tool_type == tool_type_terrain_bump) {
					uint32 height_texture_width = terrain->width * terrain->sample_per_meter;
					uint32 height_texture_height = terrain->height * terrain->sample_per_meter;

					vec2 center = editor->terrain_brush_tool_position.xz() + vec2{half_width, half_height};
					vec2 begin = center - vec2{editor->terrain_brush_tool_radius, editor->terrain_brush_tool_radius};
					vec2 end = center + vec2{editor->terrain_brush_tool_radius, editor->terrain_brush_tool_radius};
					vec2 center_uv = center / vec2{(float)terrain->width, (float)terrain->height};
					vec2 begin_uv = begin / vec2{(float)terrain->width, (float)terrain->height};
					vec2 end_uv = end / vec2{(float)terrain->width, (float)terrain->height};
					int32 center_texel[2] = {(int32)(height_texture_width * center_uv.x), (int32)(height_texture_height * center_uv.y)};
					int32 begin_texel[2] = {(int32)(height_texture_width * begin_uv.x), (int32)(height_texture_height * begin_uv.y)};
					int32 end_texel[2] = {(int32)(height_texture_width * end_uv.x), (int32)(height_texture_height * end_uv.y)};
					double radius = (double)abs(begin_texel[0] - center_texel[0]);
					double bump_step = 128.0 * editor->terrain_brush_tool_speed;
						
					for (int32 y = begin_texel[1]; y <= end_texel[1]; y += 1) {
						for (int32 x = begin_texel[0]; x <= end_texel[0]; x += 1) {
							double dx = (double)abs(x - center_texel[0]);
							double dy = (double)abs(y - center_texel[1]);
							double distance = sqrt(dx * dx + dy * dy);
							if (distance < radius) {
								double p = distance / radius;
								double bump_p = exp(-1.0 / (1.0 - p * p));
								int16 *pixel = &terrain->height_texture_data[height_texture_width * y + x];
								int16 bump = (int16)(bump_step * bump_p);
								if (*pixel > 0 && bump > (INT16_MAX - *pixel)) {
									*pixel = INT16_MAX;
								}
								else {
									*pixel += bump;
								}
							}
						}
					}

					D3D11_BOX box = {0, 0, 0, terrain->width * terrain->sample_per_meter, terrain->height * terrain->sample_per_meter, 1};
					d3d->context->UpdateSubresource(terrain->height_texture, 0, &box, terrain->height_texture_data, terrain->width * terrain->sample_per_meter * sizeof(int16), 0);
				}
			}
		}
	}
}

void append_extra_model_constants(editor *editor, world *world, d3d *d3d) {
	if (editor->model_index < world->model_count && editor->focus_model) {
		add_model_render_data(world, d3d, editor->model_index, transform_identity(), 0, 0, true);
	}
}

void render_reference_grid(editor *editor, world *world, d3d *d3d) {
	uint32 vertex_buffer_stride = 12;
	uint32 vertex_buffer_offset = 0;
	d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	d3d->context->IASetInputLayout(d3d->reference_grid_input_layout);
	d3d->context->IASetVertexBuffers(0, 1, &world->reference_grid_vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
	d3d->context->VSSetShader(d3d->reference_grid_vs, nullptr, 0);
	d3d->context->PSSetShader(d3d->reference_grid_ps, nullptr, 0);
	d3d->context->OMSetDepthStencilState(d3d->reference_grid_depth_stencil_state, 0);
	d3d->context->Draw(11 * 2 * 2, 0);
}

void append_terrain_brush_constants(editor *editor, world *world, d3d *d3d) {
	if (editor->terrain_brush_tool_active) {
		m_debug_assert(world->terrain_index < world->terrain_count, "");
		terrain *terrain = &world->terrains[world->terrain_index];
		struct {
			XMMATRIX transform_mat;
			uint32 width;
			uint32 height;
			float max_height;
		} terrain_brush_constants = {
			XMMatrixMultiply(XMMatrixScaling(editor->terrain_brush_tool_radius, 1, editor->terrain_brush_tool_radius), XMMatrixTranslation(m_unpack3(editor->terrain_brush_tool_position))),
			terrain->width, terrain->height, terrain->max_height
		};
		uint32 offset = append_world_constant_buffer(world, &terrain_brush_constants, sizeof(terrain_brush_constants));
		world->render_data.terrain_brush_constant_buffer_offset = offset / 16;
	}
}

void render_terrain_brush(editor *editor, world *world, d3d *d3d) {
 	if (editor->terrain_brush_tool_active) {
		editor->terrain_brush_tool_active = false;
		m_debug_assert(world->terrain_index < world->terrain_count, "");
		terrain *terrain = &world->terrains[world->terrain_index];
		d3d->context->VSSetShader(d3d->terrain_brush_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->terrain_brush_ps, nullptr, 0);
		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->context->IASetInputLayout(d3d->terrain_brush_input_layout);
		d3d->context->OMSetDepthStencilState(d3d->terrain_brush_depth_stencil_state, 0);
		d3d->context->VSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
		d3d->context->VSSetShaderResources(0, 1, &terrain->height_texture_view);
		uint32 constant_count = 16;
		d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &world->render_data.terrain_brush_constant_buffer_offset, &constant_count);
		uint32 vertex_buffer_stride = 12;
		uint32 vertex_buffer_offset = 0;
		d3d->context->IASetVertexBuffers(0, 1, &world->hollow_circle_vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
		d3d->context->Draw(m_countof(hollow_circle_vertices), 0);
	}
}

void render_imgui(editor *editor, world *world, d3d *d3d) {
	ImDrawData *imgui_draw_data = ImGui::GetDrawData();
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	m_d3d_assert(d3d->context->Map(editor->imgui_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource));
	uint8 *buffer = (uint8 *)mapped_subresource.pData;
	uint32 offset = 0;
	for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
		ImDrawList *dlist = imgui_draw_data->CmdLists[i];
		uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
		uint32 elements_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
		memcpy(buffer + offset, dlist->VtxBuffer.Data, vertices_size);
		offset += round_up(vertices_size, (uint32)sizeof(ImDrawVert));
		memcpy(buffer + offset, dlist->IdxBuffer.Data, elements_size);
		offset += round_up(elements_size, (uint32)sizeof(ImDrawVert));
	}
	d3d->context->Unmap(editor->imgui_vertex_buffer, 0);
		
	d3d->context->VSSetShader(d3d->imgui_vs, nullptr, 0);
	d3d->context->PSSetShader(d3d->imgui_ps, nullptr, 0);
	uint32 vertex_buffer_stride = 20;
	uint32 vertex_buffer_offset = 0;
	d3d->context->IASetVertexBuffers(0, 1, &editor->imgui_vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
	d3d->context->IASetIndexBuffer(editor->imgui_vertex_buffer, DXGI_FORMAT_R16_UINT, 0);
	d3d->context->IASetInputLayout(d3d->imgui_input_layout);
	d3d->context->RSSetState(d3d->imgui_rasterizer_state);
	d3d->context->OMSetBlendState(d3d->imgui_blend_state, nullptr, 0xffffffff);
	d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);

	offset = 0;
	for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
		ImDrawList *dlist = imgui_draw_data->CmdLists[i];
		uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
		uint32 elements_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
		uint32 vertex_index = offset / sizeof(ImDrawVert);
		offset += round_up(vertices_size, (uint32)sizeof(ImDrawVert));
		uint32 element_index = offset / sizeof(ImDrawIdx);
		offset += round_up(elements_size, (uint32)sizeof(ImDrawVert));
		for (int32 i = 0; i < dlist->CmdBuffer.Size; i += 1) {
			ImDrawCmd *dcmd = &dlist->CmdBuffer.Data[i];
			D3D11_RECT scissor_rect = {(int32)dcmd->ClipRect.x, (int32)dcmd->ClipRect.y, (int32)dcmd->ClipRect.z, (int32)dcmd->ClipRect.w};
			d3d->context->RSSetScissorRects(1, &scissor_rect);
			d3d->context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView **)&dcmd->TextureId);
			d3d->context->DrawIndexed(dcmd->ElemCount, element_index, vertex_index);
			element_index += dcmd->ElemCount;
		}
	}
}

int main(int argc, char **argv) {
	set_current_dir_to_exe_dir();

	window *window = new struct window;
	initialize_window(window, handle_window_message);
	// set_window_fullscreen(window, true);

	d3d *d3d = new struct d3d;
	initialize_d3d(d3d, window);

	editor *editor = new struct editor;
	initialize_editor(editor, d3d);

	world *world = new struct world;
	initialize_world(world, d3d);
	if (argc > 1) {
		const char *world_file = argv[1];
		m_assert(load_world(world, d3d, world_file), "");
		snprintf(editor->world_save_file, sizeof(editor->world_save_file), "%s", world_file);
	}
	
	window_message_channel.window = window;
	window_message_channel.d3d = d3d;
	window_message_channel.initialized = true;
	show_window(window);

	while (!editor->quit) {
		start_timer(&editor->timer);

		window->raw_mouse_dx = 0;
		window->raw_mouse_dy = 0;
		handle_window_messages(window);

		ImGui::GetIO().DeltaTime = (float)editor->last_frame_time_secs;
		ImGui::NewFrame();
		check_quit(editor);
		check_toggle_fullscreen(window);
		check_popups(editor, world, d3d);
		top_menu(editor, world, d3d);
		bottom_menu(editor);
		edit_window(editor, world, d3d);
		memories_window(editor, world, d3d);
		frame_statistic_window(editor);
		update_camera(editor, window);
		tool_gizmo(editor, world, d3d, window);
		ImGui::Render();

		render_world_desc render_world_desc = {};
		render_world_desc.camera_view_proj_mat = editor->camera_view_proj_mat;
		render_world_desc.camera_position = editor->camera_position;
		render_world_desc.camera_view = editor->camera_view;
		render_world_desc.render_models = true;
		render_world_desc.render_terrain = true;
		render_world_desc.render_skybox = true;
		render_world_desc.render_shadow_proj_box = editor->render_shadow_proj_box;
		render_world_desc.editor = editor;
		render_world_desc.append_extra_model_constants = append_extra_model_constants;
		render_world_desc.render_reference_grid = render_reference_grid;
		render_world_desc.append_terrain_brush_constants = append_terrain_brush_constants;
		render_world_desc.render_terrain_brush = render_terrain_brush;
		render_world_desc.render_imgui = render_imgui;
		if (editor->model_index < world->model_count && editor->focus_model) {
			render_world_desc.render_models = false;
			render_world_desc.render_terrain = false;
			render_world_desc.render_skybox = false;
		}
		render_world(world, d3d, &render_world_desc);

		editor->world_frame_memory_arena_size = world->frame_memory_arena.size;
		world->frame_memory_arena.size = 0;
		
		stop_timer(&editor->timer);
		editor->last_frame_time_secs = get_timer_duration_secs(editor->timer);
	}
	ImGui::DestroyContext(editor->imgui_context);
}
