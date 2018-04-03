/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"
#include "math.cpp"
#include "vulkan.cpp"
#include "gpk.cpp"
#include "level.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/include/stb/stb_image.h"

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "../vendor/include/imgui/imgui_draw.cpp"
#include "../vendor/include/imgui/imgui.cpp"
#include "../vendor/include/imgui/ImGuizmo.cpp"
#undef snprintf

struct imgui_error_popup {
	bool error;
	char msg[256];
};

void check_imgui_error_popup(imgui_error_popup *popup) {
	if (popup->error) {
		ImGui::OpenPopup("##error_popup_modal");
	}
	if (ImGui::BeginPopupModal("##error_popup_modal", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("%s", popup->msg);
		if (ImGui::Button("ok")) {
			popup->error = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void open_imgui_error_popup(imgui_error_popup *popup, const char *msg) {
	popup->error = true;
	strcpy(popup->msg, msg);
}

enum selection_mode {
	selection_mode_entity,
	selection_mode_mesh
};

enum transform_mode {
	transform_mode_entity,
	transform_mode_render_adjustment
};

enum gizmo_mode {
	gizmo_mode_transform_translate,
	gizmo_mode_transform_rotate,
	gizmo_mode_transform_scale,
	gizmo_mode_directional_light_rotate,
	gizmo_mode_point_light_translate,
	gizmo_mode_collision_sphere_scale,
	gizmo_mode_collision_capsule_scale,
	gizmo_mode_collision_capsule_translate,
	gizmo_mode_collision_box_scale,
	gizmo_mode_terrain_brush
};

struct collision_shape_render_data {
	mat4 transform;
	vec4 color;
	mat4 capsule_sphere_transforms[2];
};

struct editor_render_data {
	collision_shape_render_data *collision_spheres;
	uint32 collision_sphere_count;
	collision_shape_render_data *collision_capsules;
	uint32 collision_capsule_count;
	collision_shape_render_data *collision_bounds;
	uint32 collision_bound_count;

	bool terrain_brush;
	mat4 terrain_brush_transform_mat;
	uint32 terrain_brush_height_map_descriptor_index;

	uint32 imgui_dynamic_vertex_region_buffer_offset;
};

enum undoable_type {
};

struct undoable {
	undoable_type shape;
};

struct editor {
	editor_render_data render_data;

	uint32 imgui_font_atlas_descriptor_index;
	
	camera camera;
	float camera_pitch;
	float camera_move_speed;
	bool camera_moving;

	float menu_bar_height;
	float entity_window_height;
	float skybox_window_height;

	bool show_reference_grid;
	bool show_collision_shape;

	selection_mode selection_mode;
	transform_mode transform_mode;
	gizmo_mode gizmo_mode;
	
	uint32 entity_index;

	undoable undoables[256];
	uint32 undoable_count;

	char file_name[256];
};

void initialize_editor(editor *editor, vulkan *vulkan) {
	{ // imgui
		ImGui::StyleColorsDark();
		ImGuiIO &imgui_io = ImGui::GetIO();
		imgui_io.KeyMap[ImGuiKey_Tab] = keycode_tab;
		imgui_io.KeyMap[ImGuiKey_LeftArrow] = keycode_left;
		imgui_io.KeyMap[ImGuiKey_RightArrow] = keycode_right;
		imgui_io.KeyMap[ImGuiKey_UpArrow] = keycode_up;
		imgui_io.KeyMap[ImGuiKey_DownArrow] = keycode_down;
		imgui_io.KeyMap[ImGuiKey_PageUp] = keycode_page_up;
		imgui_io.KeyMap[ImGuiKey_PageDown] = keycode_page_down;
		imgui_io.KeyMap[ImGuiKey_Home] = keycode_home;
		imgui_io.KeyMap[ImGuiKey_End] = keycode_end;
		imgui_io.KeyMap[ImGuiKey_Backspace] = keycode_backspace;
		imgui_io.KeyMap[ImGuiKey_Enter] = keycode_return;
		imgui_io.KeyMap[ImGuiKey_Escape] = keycode_esc;
		imgui_io.KeyMap[ImGuiKey_A] = 'A';
		imgui_io.KeyMap[ImGuiKey_C] = 'C';
		imgui_io.KeyMap[ImGuiKey_V] = 'V';
		imgui_io.KeyMap[ImGuiKey_X] = 'X';
		imgui_io.KeyMap[ImGuiKey_Y] = 'Y';
		imgui_io.KeyMap[ImGuiKey_Z] = 'Z';
		imgui_io.DisplaySize = {(float)vulkan->swap_chain.image_width, (float)vulkan->swap_chain.image_height};
		ImGuizmo::SetRect(0, 0, imgui_io.DisplaySize.x, imgui_io.DisplaySize.y);
		imgui_io.IniFilename = nullptr;
		imgui_io.MousePos = {-1, -1};
		imgui_io.FontGlobalScale = (float)vulkan->swap_chain.image_width / (float)GetSystemMetrics(SM_CXSCREEN);

		m_assert(imgui_io.Fonts->AddFontFromFileTTF("assets\\fonts\\Roboto-Medium.ttf", (float)GetSystemMetrics(SM_CXSCREEN) / 110.0f));
		uint8* font_atlas_image = nullptr;
		int32 font_atlas_image_width = 0;
		int32 font_atlas_image_height = 0;
		imgui_io.Fonts->GetTexDataAsRGBA32(&font_atlas_image, &font_atlas_image_width, &font_atlas_image_height);

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_info.extent = {(uint32)font_atlas_image_width, (uint32)font_atlas_image_height, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;
		uint32 image_index = append_vulkan_image_region(vulkan, image_info, image_view_info, font_atlas_image, font_atlas_image_width * font_atlas_image_height * 4, 1, 4);
		editor->imgui_font_atlas_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image_index, vulkan->samplers.mipmap_samplers[0]);
		imgui_io.Fonts->SetTexID((ImTextureID)(intptr_t)editor->imgui_font_atlas_descriptor_index);
		imgui_io.Fonts->ClearTexData();
	}
	editor->camera.position = vec3{4, 8, 8};
	editor->camera.view = vec3_normalize(-editor->camera.position);
	editor->camera.fovy = degree_to_radian(50);
	editor->camera.aspect = (float)vulkan->swap_chain.image_width / (float)vulkan->swap_chain.image_height;
	editor->camera.znear = 0.1f;
	editor->camera.zfar = 1000;
	editor->camera_pitch = asinf(editor->camera.view.y);
	editor->camera_move_speed = 10;

	editor->show_reference_grid = true;

	editor->entity_index = UINT32_MAX;
}

struct read_editor_settings {
	editor *editor;
	void operator()(nlohmann::json &json) {
		if (json.find("editor_settings") != json.end()) {
			auto &camera = json["editor_settings"]["camera"];
			std::array<float, 3> position = camera["position"];
			std::array<float, 3> view = camera["view"];
			editor->camera.position = {position[0], position[1], position[2]};
			editor->camera.view = {view[0], view[1], view[2]};
			editor->camera.znear = camera["znear"];
			editor->camera.zfar = camera["zfar"];
			editor->camera_move_speed = camera["move_speed"];
			editor->camera_pitch = asinf(editor->camera.view.y);
		}
	}
};

struct write_editor_settings {
	editor *editor;
	void operator()(nlohmann::json &json) {
		json["editor_settings"] = {
			{"camera", {
				{"position", {m_unpack3(editor->camera.position)}},
				{"view", {m_unpack3(editor->camera.view)}},
				{"znear", editor->camera.znear},
				{"zfar", editor->camera.zfar},
				{"move_speed", editor->camera_move_speed}}
			}
		};
	}
};

bool ray_intersect_mesh(ray ray, model_mesh *mesh, mat4 transform, float *distance) {
	float min_distance = ray.len;
	for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
		model_mesh_primitive *primitive = &mesh->primitives[i];
		for (uint32 i = 0; i < primitive->index_count / 3; i += 1) {
			vec3 a = *(vec3 *)(primitive->vertices_data + ((uint16 *)primitive->indices_data)[i * 3 + 0] * sizeof(struct gpk_model_vertex));
			vec3 b = *(vec3 *)(primitive->vertices_data + ((uint16 *)primitive->indices_data)[i * 3 + 1] * sizeof(struct gpk_model_vertex));
			vec3 c = *(vec3 *)(primitive->vertices_data + ((uint16 *)primitive->indices_data)[i * 3 + 2] * sizeof(struct gpk_model_vertex));
			a = transform * a;
			b = transform * b;
			c = transform * c;
			float d = 0;
			if (ray_intersect_triangle(ray, a, b, c, &d) && d < min_distance) {
				min_distance = d;
			}
		}
	}
	if (min_distance != ray.len) {
		*distance = min_distance;
		return true;
	}
	else {
		return false;
	}
}

int main(int argc, char **argv) {
	set_exe_dir_as_current();

	memory_arena general_memory_arena = {};
	general_memory_arena.name = "general";
	general_memory_arena.capacity = m_megabytes(16);
	general_memory_arena.memory = allocate_virtual_memory(general_memory_arena.capacity);
	m_assert(general_memory_arena.memory);
	
	window window = {};
	m_assert(initialize_window(&window));
	set_window_fullscreen(&window, true);

	vulkan *vulkan = allocate_memory<struct vulkan>(&general_memory_arena, 1);
	initialize_vulkan(vulkan, window);

	editor *editor = allocate_memory<struct editor>(&general_memory_arena, 1);
	initialize_editor(editor, vulkan);

	level *level = allocate_memory<struct level>(&general_memory_arena, 1);
	initialize_level(level, vulkan);
	// level_read_json(level, vulkan, "agby_assets\\levels\\level_save.json", read_editor_settings{editor}, true);

	LARGE_INTEGER performance_frequency = {};
	QueryPerformanceFrequency(&performance_frequency);
	LARGE_INTEGER performance_counters[2] = {};
	uint64 last_frame_time_microsec = 0;
	double last_frame_time_sec = 0;

	uint64 entity_components_memory_arena_size = 0;
	uint64 main_thread_frame_memory_arena_size = 0;
	uint64 render_thread_frame_memory_arena_size = 0;
	uint64 vulkan_uniform_region_size = 0;
	uint64 vulkan_dynamic_vertex_region_size = 0;

	bool program_running = true;

	show_window(&window);

	while (program_running) {
		QueryPerformanceCounter(&performance_counters[0]);

		ImGui::GetIO().DeltaTime = (float)last_frame_time_sec;
		window.raw_mouse_dx = 0;
		window.raw_mouse_dy = 0;

		while (peek_window_message(&window)) {
			switch (window.msg_type) {
				case window_message_type_destroy:
				case window_message_type_close:
				case window_message_type_quit: {
					program_running = false;
				} break;
				case window_message_type_key_down:
				case window_message_type_key_up: {
					if (window.keycode == keycode_shift) {
						ImGui::GetIO().KeyShift = (window.msg_type == window_message_type_key_down);
					}
					else if (window.keycode == keycode_ctrl) {
						ImGui::GetIO().KeyCtrl = (window.msg_type == window_message_type_key_down);
					}
					else if (window.keycode == keycode_alt) {
						ImGui::GetIO().KeyAlt = (window.msg_type == window_message_type_key_down);
					}
					else {
						m_assert(window.keycode < sizeof(ImGui::GetIO().KeysDown));
						ImGui::GetIO().KeysDown[window.keycode] = (window.msg_type == window_message_type_key_down);
						if (window.keycode == keycode_print_screen && window.msg_type != window_message_type_key_down) {
							// simulate print screen key down event since windows does not send it for some reason
							ImGui::GetIO().KeysDownDuration[keycode_print_screen] = 0.1f;
						}
					}
				} break;
				case window_message_type_char: {
					ImGui::GetIO().AddInputCharacter(window.input_char);
				} break;
				case window_message_type_mouse_move: {
					ImGui::GetIO().MousePos.x = (float)window.mouse_x;
					ImGui::GetIO().MousePos.y = (float)window.mouse_y;
				} break;
				case window_message_type_mouse_lb_down:
				case window_message_type_mouse_lb_up: {
					ImGui::GetIO().MouseDown[0] = (window.msg_type == window_message_type_mouse_lb_down);
				} break;
				case window_message_type_mouse_rb_down:
				case window_message_type_mouse_rb_up: {
					ImGui::GetIO().MouseDown[1] = (window.msg_type == window_message_type_mouse_rb_down);
				} break;
				case window_message_type_mouse_mb_down:
				case window_message_type_mouse_mb_up: {
					ImGui::GetIO().MouseDown[2] = (window.msg_type == window_message_type_mouse_mb_down);
				} break;
				case window_message_type_mouse_wheel: {
					ImGui::GetIO().MouseWheel = window.mouse_wheel;
				} break;
			}
		}

		ImGui::NewFrame();
	  { // miscs
	  	if (ImGui::GetIO().KeyAlt && ImGui::IsKeyPressed(keycode_f4)) {
	  		program_running = false;
	  	}
	  }
		{ // move camera
			if (ImGui::IsMouseClicked(2) && !ImGui::GetIO().WantCaptureMouse) {
				pin_cursor(true);
				editor->camera_moving = true;
			}
			if (ImGui::IsMouseReleased(2)) {
				pin_cursor(false);
				editor->camera_moving = false;
			}
			if (editor->camera_moving) {
				if (ImGui::IsKeyDown('W')) {
					editor->camera.position = editor->camera.position + editor->camera.view * (float)last_frame_time_sec * editor->camera_move_speed;
				}
				if (ImGui::IsKeyDown('S')) {
					editor->camera.position = editor->camera.position - editor->camera.view * (float)last_frame_time_sec * editor->camera_move_speed;
				}
				if (ImGui::IsKeyDown('A')) {
					vec3 move_direction = vec3_normalize(vec3_cross(editor->camera.view, vec3{0, 1, 0}));
					editor->camera.position = editor->camera.position - move_direction * (float)last_frame_time_sec * editor->camera_move_speed;
				}
				if (ImGui::IsKeyDown('D')) {
					vec3 move_direction = vec3_normalize(vec3_cross(editor->camera.view, vec3{0, 1, 0}));
					editor->camera.position = editor->camera.position + move_direction * (float)last_frame_time_sec * editor->camera_move_speed;
				}
				float camera_rotation_speed = 2.0f;
				float max_pitch = degree_to_radian(75.0f);
				float yaw = -window.raw_mouse_dx * camera_rotation_speed * ImGui::GetIO().DeltaTime;
				editor->camera.view = vec3_normalize(mat3_from_mat4(mat4_from_rotate(vec3{0, 1, 0}, yaw)) * editor->camera.view);
				float pitch = -window.raw_mouse_dy * camera_rotation_speed * ImGui::GetIO().DeltaTime;
				if (editor->camera_pitch + pitch > -max_pitch && editor->camera_pitch + pitch < max_pitch) {
					vec3 view_cross_up = vec3_normalize(vec3_cross(editor->camera.view, vec3{0, 1, 0}));
					mat4 rotate_mat = mat4_from_rotate(view_cross_up, pitch);
					editor->camera.view = vec3_normalize(mat3_from_mat4(rotate_mat) * editor->camera.view);
					editor->camera_pitch += pitch;
				}
			}
			{
				static bool camera_move_speed_popup = false;
				if (!ImGui::GetIO().WantCaptureMouse && editor->camera_moving) {
					if (ImGui::GetIO().MouseWheel != 0 && ImGui::GetIO().KeyShift) {
						ImGui::OpenPopup("##camera_speed_popup");
						camera_move_speed_popup = true;
					}
				}
				if (ImGui::BeginPopupModal("##camera_speed_popup", &camera_move_speed_popup, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
					float min_speed = 0.1f;
					float max_speed = 100;
					editor->camera_move_speed += ImGui::GetIO().MouseWheel;
					ImGui::SliderFloat("camera speed##camera_speed_slider", &editor->camera_move_speed, min_speed, max_speed);
					ImGui::EndPopup();
				}
			}
		}
		{ // selection modes, transform mode, gizmo modes
			ImGui::PushID("selection_transform_gizmo_mode_popup");
			if ((ImGui::IsMouseClicked(1) && !ImGui::GetIO().WantCaptureMouse && !ImGuizmo::IsOver()) || ImGui::IsKeyPressed('X') && ImGui::GetIO().KeyCtrl) {
				ImGui::OpenPopup("##popup");
			}
			if (ImGui::BeginPopup("##popup")) {
				ImGui::TextColored(ImVec4{1, 1, 0, 1}, "selection mode");
				ImGui::Separator();
				if (ImGui::MenuItem("entity##selection_mode_entity", nullptr, editor->selection_mode == selection_mode_entity)) {
					editor->selection_mode = selection_mode_entity;
				}
				if (ImGui::MenuItem("mesh##selection_mode_mesh", nullptr, editor->selection_mode == selection_mode_mesh)) {
					editor->selection_mode = selection_mode_mesh;
				}
				ImGui::Dummy(ImVec2{0, 10});
				ImGui::TextColored(ImVec4{1, 1, 0, 1}, "transform mode");
				ImGui::Separator();
				if (editor->entity_index < level->entity_count) {
					uint32 entity_flags = level->entity_flags[editor->entity_index];
					if (ImGui::MenuItem("entity##transform_mode_entity", nullptr, editor->transform_mode == transform_mode_entity)) {
						editor->transform_mode = transform_mode_entity;
					}
					if (entity_flags & entity_component_flag_render) {
						if (ImGui::MenuItem("render adjustment##transform_mode_render_adjustment", nullptr, editor->transform_mode == transform_mode_render_adjustment)) {
							editor->transform_mode = transform_mode_render_adjustment;
						}
					}
					ImGui::Dummy(ImVec2{0, 10});
					ImGui::TextColored(ImVec4{1, 1, 0, 1}, "gizmo mode");
					ImGui::Separator();
					if (ImGui::MenuItem("transform translate##gizmo_mode_transform_translate", nullptr, editor->gizmo_mode == gizmo_mode_transform_translate)) {
						editor->gizmo_mode = gizmo_mode_transform_translate;
					}
					if (ImGui::MenuItem("transform rotate##gizmo_mode_transform_rotate", nullptr, editor->gizmo_mode == gizmo_mode_transform_rotate)) {
						editor->gizmo_mode = gizmo_mode_transform_rotate;
					}
					if (ImGui::MenuItem("transform scale##gizmo_mode_transform_scale", nullptr, editor->gizmo_mode == gizmo_mode_transform_scale)) {
						editor->gizmo_mode = gizmo_mode_transform_scale;
					}
					if (entity_flags & entity_component_flag_light) {
						entity_light_component *light_component = entity_get_light_component(level, editor->entity_index);
						if (light_component->light_type == light_type_directional) {
							if (ImGui::MenuItem("dir light rotate##gizmo_mode_dir_light_rotate", nullptr, editor->gizmo_mode == gizmo_mode_directional_light_rotate)) {
								editor->gizmo_mode = gizmo_mode_directional_light_rotate;
							}
						}
						else if (light_component->light_type == light_type_point) {
							if (ImGui::MenuItem("point light translate##gizmo_mode_point_light_translate", nullptr, editor->gizmo_mode == gizmo_mode_point_light_translate)) {
								editor->gizmo_mode = gizmo_mode_point_light_translate;
							}
						}
					}
					if (entity_flags & entity_component_flag_collision) {
						entity_collision_component *collision_component = entity_get_collision_component(level, editor->entity_index);
						if (collision_component->shape == collision_shape_sphere) {
							if (ImGui::MenuItem("sphere scale##gizmo_mode_collision_sphere_scale", nullptr, editor->gizmo_mode == gizmo_mode_collision_sphere_scale)) {
								editor->gizmo_mode = gizmo_mode_collision_sphere_scale;
							}
						}
						else if (collision_component->shape == collision_shape_capsule) {
							if (ImGui::MenuItem("capsule scale##gizmo_mode_collision_capsule_scale", nullptr, editor->gizmo_mode == gizmo_mode_collision_capsule_scale)) {
								editor->gizmo_mode = gizmo_mode_collision_capsule_scale;
							}
							if (ImGui::MenuItem("capsule translate##gizmo_mode_collision_capsule_translate", nullptr, editor->gizmo_mode == gizmo_mode_collision_capsule_translate)) {
								editor->gizmo_mode = gizmo_mode_collision_capsule_translate;
							}
						}
						else if (collision_component->shape == collision_shape_box) {
							if (ImGui::MenuItem("box scale##gizmo_mode_collision_box_scale", nullptr, editor->gizmo_mode == gizmo_mode_collision_box_scale)) {
								editor->gizmo_mode = gizmo_mode_collision_box_scale;
							}
						}
					}
					if (entity_flags & entity_component_flag_terrain) {
						entity_terrain_component *terrain_component = entity_get_terrain_component(level, editor->entity_index);
						if (terrain_component->terrain_index < level->terrain_count) {
							if (ImGui::MenuItem("terrain brush##gizmo_mode_terrain_brush", nullptr, editor->gizmo_mode == gizmo_mode_terrain_brush)) {
								editor->gizmo_mode = gizmo_mode_terrain_brush;
							}
						}
					}
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		ray camera_ray = {};
		{
			vec3 window_pos = vec3{ImGui::GetMousePos().x, ImGui::GetIO().DisplaySize.y - ImGui::GetMousePos().y, 0.1f};
			vec4 viewport = vec4{0, 0, ImGui::GetIO().DisplaySize.x , ImGui::GetIO().DisplaySize.y};
			vec3 mouse_world_position = mat4_unproject(window_pos, camera_view_mat4(editor->camera), camera_projection_mat4(editor->camera), viewport);
			camera_ray = {editor->camera.position, vec3_normalize(mouse_world_position - editor->camera.position), editor->camera.zfar};
		}
		{ // selection
			if (ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyShift && !ImGui::GetIO().WantCaptureMouse && !ImGuizmo::IsOver()) {
				if (editor->selection_mode == selection_mode_entity) {
					float entity_min_distance = camera_ray.len;
					uint32 entity_index = UINT32_MAX;
					for (uint32 i = 0; i < level->entity_count; i += 1) {
						if (level->entity_flags[i] & entity_component_flag_render) {
							entity_render_component *render_component = entity_get_render_component(level, i);
							if (render_component->model_index < level->model_count) {
								model *model = &level->models[render_component->model_index];
								mat4 transform_mat = mat4_from_transform(level->entity_transforms[i]) * mat4_from_transform(render_component->adjustment_transform);
								float model_min_distance = camera_ray.len;
								traverse_model_scenes_track_global_transform(model, [&](model_node *node, uint32 index, mat4 global_transform_mat) {
									if (node->mesh_index < model->mesh_count) {
										float distance = 0;
										if (ray_intersect_mesh(camera_ray, &model->meshes[node->mesh_index], transform_mat * global_transform_mat, &distance) && distance < model_min_distance) {
											model_min_distance = distance;
										}
									}
								});
								if (model_min_distance < entity_min_distance) {
									entity_min_distance = model_min_distance;
									entity_index = i;
								}
							}
						}
					}
					if (editor->entity_index != entity_index) {
						editor->entity_index = entity_index;
					}
				}
				else if (editor->selection_mode == selection_mode_mesh) {
				}
			}
		}
		{ // main menu
			ImGui::PushID("main_menu_bar");
			if (ImGui::BeginMainMenuBar()) {
				editor->menu_bar_height = ImGui::GetWindowHeight();
				if (ImGui::BeginMenu("File##file")) {
					ImGui::PushID("file");
					if (ImGui::MenuItem("New Level##new_level")) {
					}
					if (ImGui::MenuItem("Open Level##open_level")) {
						if (open_file_dialog(editor->file_name, sizeof(editor->file_name))) {
							set_exe_dir_as_current();
							level_read_json(level, vulkan, editor->file_name, read_editor_settings{editor}, true);
						}
					}
					if (ImGui::MenuItem("Save Level##save_level")) {
						if (save_file_dialog(editor->file_name, sizeof(editor->file_name))) {
							set_exe_dir_as_current();
							level_write_json(level, editor->file_name, write_editor_settings{editor});
						}
					}
					ImGui::Separator();
					ImGui::PopID();
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("View##view")) {
					ImGui::PushID("view");
					ImGui::MenuItem("reference grid##show_entity_reference_grid", nullptr, &editor->show_reference_grid);
					ImGui::MenuItem("collision shape##show_collision_shape", nullptr, &editor->show_collision_shape);
					ImGui::PopID();
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			ImGui::PopID();
		}
		{ // entity window
			ImGui::SetNextWindowPos(ImVec2{0, editor->menu_bar_height});
			ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.5f});
			ImGui::PushID("entitiy_window");
			if (ImGui::Begin("Entity##window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
				editor->entity_window_height = ImGui::GetWindowHeight();
				{ // entity list
					const char *entity_combo_name = editor->entity_index < level->entity_count ? level->entity_infos[editor->entity_index].name : nullptr;
					if (ImGui::BeginCombo("##entities_combo", entity_combo_name)) {
						if (ImGui::Selectable("", editor->entity_index >= level->entity_count)) {
							editor->entity_index = UINT32_MAX;
						}
						for (uint32 i = 0; i < level->entity_count; i += 1) {
							if (ImGui::Selectable(level->entity_infos[i].name, editor->entity_index == i)) {
								if (editor->entity_index != i) {
									editor->entity_index = i;
								}
							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();
					if (ImGui::Button("New Entity##new_entity_button")) {
						ImGui::OpenPopup("##new_entity_popup");
					}
					if (ImGui::BeginPopupModal("##new_entity_popup", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
						static imgui_error_popup error_popup = {};
						check_imgui_error_popup(&error_popup);
						static char entity_name[sizeof(entity_info::name)] = {};
						ImGui::InputText("name##new_entity_popup_name", entity_name, sizeof(entity_name));
						if (ImGui::Button("ok##new_entity_popup_ok")) {
							if (strcmp(entity_name, "")) {
								for (uint32 i = 0; i < level->entity_count; i += 1) {
									if (!strcmp(level->entity_infos[i].name, entity_name)) {
										open_imgui_error_popup(&error_popup, "entity name already exist");
										break;
									}
								}
								if (!error_popup.error) {
									entity_addition *entity_addition = allocate_memory<struct entity_addition>(&level->main_thread_frame_memory_arena, 1);
									array_copy(entity_addition->info.name, entity_name);
									entity_addition->transform = transform_identity();
									list_prepend(&level->entity_addition, entity_addition);
									ImGui::CloseCurrentPopup();
								}
							}
						}
						ImGui::SameLine();
						if (ImGui::Button("cancel##new_entity_popup_cancel")) {
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
				}
				ImGui::Separator();
				if (editor->entity_index >= level->entity_count) goto entity_window_skip_components_label;
				{ // basic properties
					ImGui::PushID("basic_properties");
					if (ImGui::CollapsingHeader("Basic Properties"), ImGuiTreeNodeFlags_DefaultOpen) {
						transform *old_transform = &level->entity_transforms[editor->entity_index];
						transform *transform = allocate_memory<struct transform>(&level->main_thread_frame_memory_arena, 1);
						memcpy(transform, old_transform, sizeof(struct transform));
						ImGui::InputFloat3("translate##transform_translate_field", transform->translate.e, 3);
						ImGui::InputFloat4("rotate##transform_rotate_field", transform->rotate.e, 3);
						ImGui::InputFloat3("scale##transform_scale_field", transform->scale.e, 3);
						if (ImGui::Button("reset##transform_reset_button")) {
							*transform = transform_identity();
						}
						if (memcmp(transform, old_transform, sizeof(struct transform))) {
							transform->rotate = quat_normalize(transform->rotate);
							level->entity_modifications[editor->entity_index].transform = transform;
						}
					}
					ImGui::PopID();
				}
				uint32 entity_flags = level->entity_flags[editor->entity_index];
				if (entity_flags & entity_component_flag_render) {
					ImGui::PushID("render_component");
					if (ImGui::CollapsingHeader("Render Component"), ImGuiTreeNodeFlags_DefaultOpen) {
						entity_render_component *old_render_component = entity_get_render_component(level, editor->entity_index);
						entity_render_component *render_component = allocate_memory<struct entity_render_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(render_component, old_render_component, sizeof(struct entity_render_component));
						const char *model_file_combo_name = (render_component->model_index < level->model_count) ? level->models[render_component->model_index].gpk_file : nullptr;
						if (ImGui::BeginCombo("models##models_combo", model_file_combo_name)) {
							for (uint32 i = 0; i < level->model_count; i += 1) {
								if (ImGui::Selectable(level->models[i].gpk_file, render_component->model_index == i)) {
									render_component->model_index = i;
								}
							}
							ImGui::EndCombo();
						}
						ImGui::Separator();
						ImGui::InputFloat3("translate##transform_translate_field", render_component->adjustment_transform.translate.e, 3);
						if (ImGui::InputFloat4("rotate##transform_rotate_field", render_component->adjustment_transform.rotate.e, 3)) {
							render_component->adjustment_transform.rotate = quat_normalize(render_component->adjustment_transform.rotate);
						}
						ImGui::InputFloat3("scale##transform_scale_field", render_component->adjustment_transform.scale.e, 3);
						if (ImGui::Button("reset##transform_reset_button")) {
							render_component->adjustment_transform = transform_identity();
						}
						ImGui::SameLine();
						ImGui::Checkbox("hide##hide_checkbox", &render_component->hide);
						if (memcmp(render_component, old_render_component, sizeof(struct entity_render_component))) {
							level->entity_modifications[editor->entity_index].render_component = render_component;
						}
						if (ImGui::Button("delete##delete_button")) {
							level->entity_modifications[editor->entity_index].remove_render_component = true;
						}
					}
					ImGui::PopID();
				}
				if (entity_flags & entity_component_flag_collision) {
					ImGui::PushID("collision_component");
					if (ImGui::CollapsingHeader("Collision Component"), ImGuiTreeNodeFlags_DefaultOpen) {
						entity_collision_component *old_collision_component = entity_get_collision_component(level, editor->entity_index);
						entity_collision_component *collision_component = allocate_memory<struct entity_collision_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(collision_component, old_collision_component, sizeof(struct entity_collision_component));
						if (collision_component->shape == collision_shape_sphere) {
							auto *sphere = &collision_component->sphere;
							ImGui::Text("sphere:");
							ImGui::InputFloat("radius##sphere_radius_field", &sphere->radius, 3);
						}
						else if (collision_component->shape == collision_shape_capsule) {
							auto *capsule = &collision_component->capsule;
							ImGui::Text("capsule:");
							ImGui::InputFloat("height##capsule_height_field", &capsule->height, 3);
							ImGui::InputFloat("radius##capsule_radius_field", &capsule->radius, 3);
						}
						else if (collision_component->shape == collision_shape_box) {
							auto *box = &collision_component->box;
							ImGui::Text("box:");
							ImGui::InputFloat3("size##box_size", box->size.e, 3);
						}
						if (ImGui::Button("change##change_collision_shape_button")) {
							ImGui::OpenPopup("##change_collision_shape_popup");
						}
						if (ImGui::BeginPopupModal("##change_collision_shape_popup", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
							static uint32 shape_index = UINT32_MAX;
							const char *shape_strings[] = {"sphere", "capsule", "box"};
							const char *shape_str = shape_index < m_countof(shape_strings) ? shape_strings[shape_index] : nullptr;
							if (ImGui::BeginCombo("shapes##change_collision_shape_combo", shape_str)) {
								for (uint32 i = 0; i < m_countof(shape_strings); i += 1) {
									if (collision_component->shape != i) {
										if (ImGui::Selectable(shape_strings[i], shape_index == i)) {
											shape_index = i;
										}
									}
								}
								ImGui::EndCombo();
							}
							if (ImGui::Button("ok##change_collision_shape_ok")) {
								collision_component->shape = (collision_shape)shape_index;
								shape_index = UINT32_MAX;
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();
							if (ImGui::Button("cancel##change_collision_shape_cancel")) {
								shape_index = UINT32_MAX;
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						if (memcmp(collision_component, old_collision_component, sizeof(struct entity_collision_component))) {
							level->entity_modifications[editor->entity_index].collision_component = collision_component;
						}
						if (ImGui::Button("delete##delete_button")) {
							level->entity_modifications[editor->entity_index].remove_collision_component = true;
						}
					}
					ImGui::PopID();
				}
				if (entity_flags & entity_component_flag_physics) {
					ImGui::PushID("physics_component");
					if (ImGui::CollapsingHeader("Physics Component"), ImGuiTreeNodeFlags_DefaultOpen) {
						entity_physics_component *old_physics_component = entity_get_physics_component(level, editor->entity_index);
						entity_physics_component *physics_component = allocate_memory<struct entity_physics_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(physics_component, old_physics_component, sizeof(struct entity_physics_component));
						ImGui::InputFloat3("velocity##velocity_field", physics_component->velocity.e);
						ImGui::InputFloat("mass##mass_field", &physics_component->mass);
						ImGui::InputFloat("max speed##max_speed_field", &physics_component->max_speed);
						if (memcmp(physics_component, old_physics_component, sizeof(struct entity_physics_component))) {
							level->entity_modifications[editor->entity_index].physics_component = physics_component;
						}
						if (ImGui::Button("delete##delete_button")) {
							level->entity_modifications[editor->entity_index].remove_physics_component = true;
						}
					}
					ImGui::PopID();
				}
				if (entity_flags & entity_component_flag_light) {
					ImGui::PushID("light_component");
					if (ImGui::CollapsingHeader("Light Component"), ImGuiTreeNodeFlags_DefaultOpen) {
						entity_light_component *old_light_component = entity_get_light_component(level, editor->entity_index);
						entity_light_component *light_component = allocate_memory<struct entity_light_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(light_component, old_light_component, sizeof(struct entity_light_component));
						if (light_component->light_type == light_type_ambient) {
							ImGui::ColorEdit3("color##ambient_light_color", light_component->ambient_light.color.e);
						}
						else if (light_component->light_type == light_type_directional) {
							if (ImGui::InputFloat3("direction##directional_light_direction_field", light_component->directional_light.direction.e, 3)) {
								light_component->directional_light.direction = vec3_normalize(light_component->directional_light.direction);
							}
							ImGui::ColorEdit3("color##directional_light_color", light_component->directional_light.color.e);
						}
						else if (light_component->light_type == light_type_point) {
							ImGui::InputFloat3("position##point_light_position_field", light_component->point_light.position.e, 3);
							ImGui::InputFloat("attenuation##point_light_attenuation_field", &light_component->point_light.attenuation);
							ImGui::ColorEdit3("color##point_light_color_field", light_component->point_light.color.e);
						}
						if (memcmp(light_component, old_light_component, sizeof(struct entity_light_component))) {
							level->entity_modifications[editor->entity_index].light_component = light_component;
						}
						if (ImGui::Button("delete##delete_button")) {
							level->entity_modifications[editor->entity_index].remove_light_component = true;
						}
					}
					ImGui::PopID();
				}
				if (entity_flags & entity_component_flag_terrain) {
					ImGui::PushID("terrain_component");
					if (ImGui::CollapsingHeader("Terrain Component"), ImGuiTreeNodeFlags_DefaultOpen) {
						entity_terrain_component *old_terrain_component = entity_get_terrain_component(level, editor->entity_index);
						entity_terrain_component *terrain_component = allocate_memory<struct entity_terrain_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(terrain_component, old_terrain_component, sizeof(struct entity_terrain_component));
						const char *terrain_file_combo_name = (terrain_component->terrain_index < level->terrain_count) ? level->terrains[terrain_component->terrain_index].gpk_file : nullptr;
						if (ImGui::BeginCombo("terrains##terrains_combo", terrain_file_combo_name)) {
							for (uint32 i = 0; i < level->terrain_count; i += 1) {
								if (ImGui::Selectable(level->terrains[i].gpk_file, terrain_component->terrain_index == i)) {
									terrain_component->terrain_index = i;
								}
							}
							ImGui::EndCombo();
						}
						ImGui::Separator();
						if (memcmp(terrain_component, old_terrain_component, sizeof(struct entity_terrain_component))) {
							level->entity_modifications[editor->entity_index].terrain_component = terrain_component;
						}
						if (ImGui::Button("delete##delete_button")) {
							level->entity_modifications[editor->entity_index].remove_terrain_component = true;
						}
					}
					ImGui::PopID();
				}
				ImGui::Dummy({0, 16});
				if (ImGui::Button("New Component##new_component_button")) {
					ImGui::OpenPopup("##new_component_popup");
				}
				if (ImGui::BeginPopupModal("##new_component_popup", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
					static imgui_error_popup error_popup = {};
					check_imgui_error_popup(&error_popup);
					static const char *component_types[] = {"render", "collision", "physics", "light", "terrain"};
					static const char *light_types[] = {"ambient", "directional", "point"};
					static uint32 component_type_index = 0;
					static uint32 light_type_index = 0;
					const char *component_combo_name = component_types[component_type_index];
					if (ImGui::BeginCombo("component##new_component_popup_component_combo", component_combo_name)) {
						for (uint32 i = 0; i < m_countof(component_types); i += 1) {
							if (ImGui::Selectable(component_types[i], component_type_index == i)) {
								component_type_index = i;
							}
						}
						ImGui::EndCombo();
					}
					if (component_type_index == 3) {
						const char *light_combo_type = light_types[light_type_index];
						if (ImGui::BeginCombo("type##new_component_popup_light_type_combo", light_combo_type)) {
							for (uint32 i = 0; i < m_countof(light_types); i += 1) {
								if (ImGui::Selectable(light_types[i], light_type_index == i)) {
									light_type_index = i;
								}
							}
							ImGui::EndCombo();
						}
					}
					if (ImGui::Button("ok##new_component_popup_ok")) {
						if ((component_type_index == 0 && entity_flags & entity_component_flag_render) ||
								(component_type_index == 1 && entity_flags & entity_component_flag_collision) ||
								(component_type_index == 2 && entity_flags & entity_component_flag_physics) ||
								(component_type_index == 3 && entity_flags & entity_component_flag_light) ||
							  (component_type_index == 4 && entity_flags & entity_component_flag_terrain)) {
							open_imgui_error_popup(&error_popup, "component already exist");
						}
						else {
							if (component_type_index == 0) {
								entity_render_component *new_render_component = allocate_memory<struct entity_render_component>(&level->main_thread_frame_memory_arena, 1);
								new_render_component->model_index = UINT32_MAX;
								new_render_component->adjustment_transform = transform_identity();
								level->entity_modifications[editor->entity_index].render_component = new_render_component;
							}
							else if (component_type_index == 1) {
								entity_collision_component *new_collision_component = allocate_memory<struct entity_collision_component>(&level->main_thread_frame_memory_arena, 1);
								level->entity_modifications[editor->entity_index].collision_component = new_collision_component;
							}
							else if (component_type_index == 2) {
								entity_physics_component *new_physics_component = allocate_memory<struct entity_physics_component>(&level->main_thread_frame_memory_arena, 1);
								level->entity_modifications[editor->entity_index].physics_component = new_physics_component;
							}
							else if (component_type_index == 3) {
								entity_light_component *new_light_component = allocate_memory<struct entity_light_component>(&level->main_thread_frame_memory_arena, 1);
								if (light_type_index == 0) {
									new_light_component->light_type = {light_type_ambient};
									new_light_component->ambient_light = ambient_light{{0.1f, 0.1f, 0.1f}};
								}
								else if (light_type_index == 1) {
									new_light_component->light_type = {light_type_directional};
									new_light_component->directional_light = directional_light{{0.1f, 0.1f, 0.1f}, {0, -1, 0}};
								}
								else if (light_type_index == 2) {
									new_light_component->light_type = {light_type_point};
									new_light_component->point_light = point_light{{0.1f, 0.1f, 0.1f}, {0, 0, 0}, 2};
								}
								level->entity_modifications[editor->entity_index].light_component = new_light_component;
							} else if (component_type_index == 4) {
								entity_terrain_component *new_terrain_component = allocate_memory<struct entity_terrain_component>(&level->main_thread_frame_memory_arena, 1);
								new_terrain_component->terrain_index = UINT32_MAX;
								level->entity_modifications[editor->entity_index].terrain_component = new_terrain_component;
							}
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("cancel##new_component_popup_cancel")) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Rename##rename_button")) {
					ImGui::OpenPopup("##rename_popup");
				}
				if (ImGui::BeginPopupModal("##rename_popup", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
					static imgui_error_popup error_popup = {};
					check_imgui_error_popup(&error_popup);
					static char name_buf[sizeof(entity_info::name)] = {};
					ImGui::InputText("new name##rename_pop_up_new_name", name_buf, sizeof(name_buf));
					if (ImGui::Button("ok##rename_popup_ok")) {
						for (uint32 i = 0; i < level->entity_count; i += 1) {
							if (!strcmp(level->entity_infos[i].name, name_buf)) {
								open_imgui_error_popup(&error_popup, "entity name already exist");
								break;
							}
						}
						if (!error_popup.error) {
							entity_info *new_entity_info = allocate_memory<struct entity_info>(&level->main_thread_frame_memory_arena, 1);
							array_copy(new_entity_info->name, name_buf);
							array_set(name_buf, '\0');
							level->entity_modifications[editor->entity_index].info = new_entity_info;
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("cancel##rename_popup_cancel")) {
						array_set(name_buf, '\0');
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Delete##delete_button")) {
					level->entity_modifications[editor->entity_index].remove = true;
				}
			}
		entity_window_skip_components_label:
			ImGui::End();
			ImGui::PopID();
		}
		{ // skyboxes window
			ImGui::SetNextWindowPos(ImVec2{0, editor->menu_bar_height + editor->entity_window_height});
			ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.5f * 0.2f});
			ImGui::PushID("skyboxes_window");
			if (ImGui::Begin("Skyboxes##window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
				editor->skybox_window_height = ImGui::GetWindowHeight();
				const char *skybox_combo_name = (level->skybox_index < level->skybox_count) ? level->skyboxes[level->skybox_index].gpk_file : nullptr;
				if (ImGui::BeginCombo("skyboxes##skyboxes_combo", skybox_combo_name)) {
					for (uint32 i = 0; i < level->skybox_count; i += 1) {
						if (ImGui::Selectable(level->skyboxes[i].gpk_file, level->skybox_index == i)) {
							level->skybox_index = i;
						}
					}
					ImGui::EndCombo();
				}
			}
			ImGui::End();
			ImGui::PopID();
		}
		{ // terrains window
			ImGui::SetNextWindowPos(ImVec2{0, editor->menu_bar_height + editor->entity_window_height + editor->skybox_window_height});
			ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.5f * 0.4f});
			ImGui::PushID("terrains_window");
			if (ImGui::Begin("Terrains##window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
				static uint32 terrain_index = 0;
				const char *terrain_combo_name = (terrain_index < level->terrain_count) ? level->terrains[terrain_index].gpk_file : nullptr;
				if (ImGui::BeginCombo("terrains##terraines_combo", terrain_combo_name)) {
					for (uint32 i = 0; i < level->terrain_count; i += 1) {
						if (ImGui::Selectable(level->terrains[i].gpk_file, terrain_index == i)) {
							terrain_index = i;
						}
					}
					ImGui::EndCombo();
				}
				if (terrain_index < level->terrain_count) {
					static imgui_error_popup error_popup = {};
					check_imgui_error_popup(&error_popup);
					terrain *terrain = &level->terrains[terrain_index];
					ImGui::ImageButton((ImTextureID)(intptr_t)terrain->height_map_descriptor_index, ImVec2{50, 50});
					ImGui::SameLine();
					ImGui::ImageButton((ImTextureID)(intptr_t)terrain->diffuse_map_descriptor_index, ImVec2{50, 50});
					ImGui::Text("Height map");
					ImGui::SameLine();
					ImGui::Text("Diffuse map");
				}
				if (ImGui::Button("new##new_terrain")) {
					ImGui::OpenPopup("##new_terrain_popup");
				}
				if (ImGui::BeginPopupModal("##new_terrain_popup", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
					static imgui_error_popup error_popup = {};
					check_imgui_error_popup(&error_popup);
					static char gpk_file[256] = {};
					ImGui::InputText("gpk file##gpk_file", gpk_file, sizeof(gpk_file));
					ImGui::SameLine();
					if (ImGui::Button("browse##browse_button")) {
						open_file_dialog(gpk_file, sizeof(gpk_file));
					}
					if (ImGui::Button("ok##ok")) {
						if (!file_exists(gpk_file)) {
							gpk_terrain terrain = {};
							strcpy(terrain.format_str, m_gpk_terrain_format_str);
							terrain.height_map_width = 128;
							terrain.height_map_height = 128;
							terrain.height_map_size = terrain.height_map_width * terrain.height_map_height * 2;
							terrain.height_map_offset = round_up((uint32)sizeof(struct gpk_terrain), 16u);
							terrain.diffuse_map_width = 128;
							terrain.diffuse_map_height = 128;
							terrain.diffuse_map_size = terrain.diffuse_map_width * terrain.diffuse_map_height * 4;
							terrain.diffuse_map_offset = round_up(terrain.height_map_offset + terrain.height_map_size, 16u);
							uint32 file_size = terrain.diffuse_map_offset + terrain.diffuse_map_size;
							file_mapping gpk_file_mapping;
							if (!create_file_mapping(gpk_file, file_size, &gpk_file_mapping)) {
								open_imgui_error_popup(&error_popup, "cannot create new gpk file");
							} else {
								*(gpk_terrain *)gpk_file_mapping.ptr = terrain;
								uint8 *height_map_ptr = gpk_file_mapping.ptr + terrain.height_map_offset;
								memset(height_map_ptr, 0, terrain.height_map_size);
								uint8 *diffuse_map_ptr = gpk_file_mapping.ptr + terrain.diffuse_map_offset;
								memset(diffuse_map_ptr, 128, terrain.diffuse_map_size);
								flush_file_mapping(gpk_file_mapping);
								close_file_mapping(&gpk_file_mapping);
								level_add_terrain(level, vulkan, gpk_file);
								array_set(gpk_file, '\0');
								ImGui::CloseCurrentPopup();
							}
						} else {
							level_add_terrain(level, vulkan, gpk_file);
							array_set(gpk_file, '\0');
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("cancel##cancel")) {
						array_set(gpk_file, '\0');
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			ImGui::End();
			ImGui::PopID();
		}
		{ // memory window
			ImGui::SetNextWindowPos(ImVec2{ImGui::GetIO().DisplaySize.x * 0.8f, editor->menu_bar_height});
			ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.5f * 0.8f});
			ImGui::PushID("memory_usage_window");
			if (ImGui::Begin("Memory Usage##window", nullptr, ImGuiWindowFlags_NoCollapse)) {
				auto imgui_render_memory = [](uint64 memory_size, uint64 memory_capacity, const char *memory_name) {
					char overlay[64] = {};
					snprintf(overlay, sizeof(overlay), "%s / %s", pretty_print_bytes(memory_size).data(), pretty_print_bytes(memory_capacity).data());
					ImGui::ProgressBar((float)((double)memory_size / (double)memory_capacity), ImVec2{ImGui::GetWindowContentRegionWidth() * 0.5f, 0}, overlay);
					ImGui::SameLine();
					ImGui::Text(memory_name);
				};
				ImGui::Text("Memory Arenas");
				memory_arena *entity_components_memory_arena = &level->entity_components_memory_arenas[level->entity_components_memory_arena_index];
				imgui_render_memory(entity_components_memory_arena_size, entity_components_memory_arena->capacity, "Entity Component");
				imgui_render_memory(level->assets_memory_arena.size, level->assets_memory_arena.capacity, "Assets");
				imgui_render_memory(main_thread_frame_memory_arena_size, level->main_thread_frame_memory_arena.capacity, "Main Thread Frame");
				imgui_render_memory(render_thread_frame_memory_arena_size, level->render_thread_frame_memory_arena.capacity, "Render Thread Frame");
				ImGui::Text("Vulkan Memories");
				imgui_render_memory(vulkan->memory_regions.image_region_size, vulkan->memory_regions.image_region_capacity, "Image Region");
				imgui_render_memory(vulkan->memory_regions.vertex_region_size, vulkan->memory_regions.vertex_region_capacity, "Vertex Region");
				imgui_render_memory(vulkan_uniform_region_size, vulkan->memory_regions.uniform_region_capacities[vulkan->frame_index], "Uniform Region");
				imgui_render_memory(vulkan_dynamic_vertex_region_size, vulkan->memory_regions.dynamic_vertex_region_capacities[vulkan->frame_index], "Dynamic Vertex Region");
			}
			ImGui::End();
			ImGui::PopID();
		}
		{ // gizmo
			if (editor->entity_index < level->entity_count) {
				uint32 entity_flags = level->entity_flags[editor->entity_index];
				mat4 camera_view_mat = camera_view_mat4(editor->camera);
				mat4 camera_proj_mat = camera_projection_mat4(editor->camera);
				auto transform_gizmo = [&](ImGuizmo::OPERATION imguizmo_op) {
					transform entity_transform = level->entity_transforms[editor->entity_index];
					transform adjustment_transform = transform_identity();
					entity_render_component *render_component = nullptr;
					if (entity_flags & entity_component_flag_render) {
						render_component = entity_get_render_component(level, editor->entity_index);
						adjustment_transform = render_component->adjustment_transform;
					}
					mat4 transform_mat = mat4_from_translate(entity_transform.translate + adjustment_transform.translate);
					if (editor->transform_mode == transform_mode_entity) {
						transform *new_entity_transform = allocate_memory<struct transform>(&level->main_thread_frame_memory_arena, 1);
						*new_entity_transform = entity_transform;
						ImGuizmo::BeginFrame();
						if (imguizmo_op == ImGuizmo::TRANSLATE) {
							ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, (float *)transform_mat);
							new_entity_transform->translate = mat4_get_translate(transform_mat) - adjustment_transform.translate;
						}
						else if (imguizmo_op == ImGuizmo::ROTATE) {
 							transform_mat = transform_mat * mat4_from_rotate(new_entity_transform->rotate);
							ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::ROTATE, ImGuizmo::LOCAL, (float *)transform_mat);
							new_entity_transform->rotate = mat4_get_rotate(transform_mat);
						}
						else if (imguizmo_op == ImGuizmo::SCALE) {
 							transform_mat = transform_mat * mat4_from_scale(new_entity_transform->scale);
							ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::SCALE, ImGuizmo::LOCAL, (float *)transform_mat);
							new_entity_transform->scale = mat4_get_scale(transform_mat);
						}
						if (!level->entity_modifications[editor->entity_index].transform) {
							level->entity_modifications[editor->entity_index].transform = new_entity_transform;
						}
					}
					else if (editor->transform_mode == transform_mode_render_adjustment && render_component) {
						entity_render_component *new_render_component = allocate_memory<struct entity_render_component>(&level->main_thread_frame_memory_arena, 1);
						*new_render_component = *render_component;
						ImGuizmo::BeginFrame();
						if (imguizmo_op == ImGuizmo::TRANSLATE) {
							ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, (float *)transform_mat);
							new_render_component->adjustment_transform.translate = mat4_get_translate(transform_mat) - entity_transform.translate;
						}
						if (imguizmo_op == ImGuizmo::ROTATE) {
 							transform_mat = transform_mat * mat4_from_rotate(adjustment_transform.rotate);
							ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::ROTATE, ImGuizmo::LOCAL, (float *)transform_mat);
							new_render_component->adjustment_transform.rotate = mat4_get_rotate(transform_mat);
						}
						else if (imguizmo_op == ImGuizmo::SCALE) {
 							transform_mat = transform_mat * mat4_from_scale(adjustment_transform.scale);
							ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::SCALE, ImGuizmo::LOCAL, (float *)transform_mat);
							new_render_component->adjustment_transform.scale = mat4_get_scale(transform_mat);
						}
						if (!level->entity_modifications[editor->entity_index].render_component) {
							level->entity_modifications[editor->entity_index].render_component = new_render_component;
						}
					}
				};
				if (editor->gizmo_mode == gizmo_mode_transform_translate) {
					transform_gizmo(ImGuizmo::TRANSLATE);
				}
				else if (editor->gizmo_mode == gizmo_mode_transform_rotate) {
					transform_gizmo(ImGuizmo::ROTATE);
				}
				else if (editor->gizmo_mode == gizmo_mode_transform_scale) {
					transform_gizmo(ImGuizmo::SCALE);
				}
				else if (editor->gizmo_mode == gizmo_mode_directional_light_rotate && entity_flags & entity_component_flag_light) {
					entity_light_component *old_light_component = entity_get_light_component(level, editor->entity_index);
					if (old_light_component->light_type == light_type_directional) {
						entity_light_component *light_component = allocate_memory<struct entity_light_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(light_component, old_light_component, sizeof(struct entity_light_component));
						transform transform = transform_identity();
						transform.translate = editor->camera.position + editor->camera.view * 16;
						mat4 transform_mat = mat4_from_transform(transform);
						ImGuizmo::BeginFrame();
						ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::ROTATE, ImGuizmo::LOCAL, (float *)transform_mat);
						light_component->directional_light.direction = vec3_normalize(mat4_get_rotate(transform_mat) * light_component->directional_light.direction);
						if (!level->entity_modifications[editor->entity_index].light_component) {
							level->entity_modifications[editor->entity_index].light_component = light_component;
						}

						ImDrawList *draw_list = ImGuizmo::gContext.mDrawList;
						vec3 line_begin_world = transform.translate;
						vec3 line_end_world = line_begin_world + light_component->directional_light.direction * 1.25f;
						vec4 line_begin = camera_proj_mat * camera_view_mat * vec4{line_begin_world.x, line_begin_world.y, line_begin_world.z, 1};
						vec4 line_end = camera_proj_mat * camera_view_mat * vec4{line_end_world.x, line_end_world.y, line_end_world.z, 1};
						line_begin /= line_begin.w;
						line_end /= line_end.w;
						line_begin.x = line_begin.x * 0.5f + 0.5f;
						line_begin.y = -line_begin.y * 0.5f + 0.5f;
						line_end.x = line_end.x * 0.5f + 0.5f;
						line_end.y = -line_end.y * 0.5f + 0.5f;

						ImVec2 line_begin_imgui = {ImGui::GetIO().DisplaySize.x * line_begin.x, ImGui::GetIO().DisplaySize.y * line_begin.y};
						ImVec2 line_end_imgui = {ImGui::GetIO().DisplaySize.x * line_end.x, ImGui::GetIO().DisplaySize.y * line_end.y};

						draw_list->AddLine(line_begin_imgui, line_end_imgui, 0xffffffff, 6);
						draw_list->AddCircleFilled(line_end_imgui, 12, 0xff00ffff);
					}
				}
				else if (editor->gizmo_mode == gizmo_mode_point_light_translate && entity_flags & entity_component_flag_light) {
					entity_light_component *old_light_component = entity_get_light_component(level, editor->entity_index);
					if (old_light_component->light_type == light_type_point) {
						entity_light_component *light_component = allocate_memory<struct entity_light_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(light_component, old_light_component, sizeof(struct entity_light_component));
						transform transform = transform_identity();
						transform.translate = light_component->point_light.position;
						mat4 transform_mat = mat4_from_transform(transform);
						ImGuizmo::BeginFrame();
						ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, (float *)transform_mat);
						light_component->point_light.position = mat4_get_translate(transform_mat);
						if (!level->entity_modifications[editor->entity_index].light_component) {
							level->entity_modifications[editor->entity_index].light_component = light_component;
						}
					}
				}
				else if (editor->gizmo_mode == gizmo_mode_collision_sphere_scale && entity_flags & entity_component_flag_collision) {
					entity_collision_component *old_collision_component = entity_get_collision_component(level, editor->entity_index);
					if (old_collision_component->shape == collision_shape_sphere) {
						entity_collision_component *collision_component = allocate_memory<struct entity_collision_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(collision_component, old_collision_component, sizeof(struct entity_collision_component));

						auto *sphere = &collision_component->sphere;
						mat4 transform_mat = mat4_from_translate(level->entity_transforms[editor->entity_index].translate);
						ImGuizmo::BeginFrame();
						ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::SCALE, ImGuizmo::LOCAL, (float *)transform_mat);
						static float radius = 0;
						vec3 scale = mat4_get_scale(transform_mat);
						if (scale == vec3{1, 1, 1}) {
							radius = sphere->radius;
						}
						else {
							sphere->radius = radius * max(max(scale.x, scale.y), scale.z);
						}
						if (!level->entity_modifications[editor->entity_index].collision_component) {
							level->entity_modifications[editor->entity_index].collision_component = collision_component;
						}
					}					
				}
				else if (editor->gizmo_mode == gizmo_mode_collision_box_scale && entity_flags & entity_component_flag_collision) {
					entity_collision_component *old_collision_component = entity_get_collision_component(level, editor->entity_index);
					if (old_collision_component->shape == collision_shape_box) {
						entity_collision_component *collision_component = allocate_memory<struct entity_collision_component>(&level->main_thread_frame_memory_arena, 1);
						memcpy(collision_component, old_collision_component, sizeof(struct entity_collision_component));
						auto *box = &collision_component->box;
						mat4 transform_mat = mat4_from_translate(level->entity_transforms[editor->entity_index].translate);
						ImGuizmo::BeginFrame();
						ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, ImGuizmo::SCALE, ImGuizmo::LOCAL, (float *)transform_mat);
						static vec3 size = {};
						vec3 scale = mat4_get_scale(transform_mat);
						if (scale == vec3{1, 1, 1}) {
							size = box->size;
						}
						else {
							box->size = size * scale;
						}
						if (!level->entity_modifications[editor->entity_index].collision_component) {
							level->entity_modifications[editor->entity_index].collision_component = collision_component;
						}
					}
				}
			}
		}
		ImGui::Render();

		auto generate_editor_render_data = [&] {
			editor->render_data = {};
			// terrain brush
			if (editor->gizmo_mode == gizmo_mode_terrain_brush && editor->entity_index < level->entity_count && level->entity_flags[editor->entity_index] & entity_component_flag_terrain) {
				entity_terrain_component *terrain_component = entity_get_terrain_component(level, editor->entity_index);
				if (terrain_component->terrain_index < level->terrain_count) {
					vec3 intersection = {};
					if (ray_interect_plane(camera_ray, plane{{0, 1, 0}, 0}, &intersection)) {
						intersection.y += 0.0005f;
						editor->render_data.terrain_brush = true;
						editor->render_data.terrain_brush_transform_mat = mat4_from_translate(intersection);
						editor->render_data.terrain_brush_height_map_descriptor_index = level->terrains[terrain_component->terrain_index].height_map_descriptor_index;
					}
				}
			}
	    // collision objects
			if (editor->show_collision_shape && editor->entity_index < level->entity_count && level->entity_flags[editor->entity_index] & entity_component_flag_collision) {
				entity_collision_component *entity_collision_component = entity_get_collision_component(level, editor->entity_index);
				if (entity_collision_component->shape == collision_shape_sphere) {
					editor->render_data.collision_sphere_count = 1;
					editor->render_data.collision_spheres = allocate_memory<collision_shape_render_data>(&level->main_thread_frame_memory_arena, editor->render_data.collision_sphere_count);

					auto sphere = entity_collision_component->sphere;
					transform transform = level->entity_transforms[editor->entity_index];
					editor->render_data.collision_spheres[0].transform = mat4_from_translate(transform.translate) * mat4_from_scale({sphere.radius, sphere.radius, sphere.radius});
					editor->render_data.collision_spheres[0].color = vec4{0, 1, 0, 0.25f};
				}
				if (entity_collision_component->shape == collision_shape_capsule) {
					editor->render_data.collision_capsule_count = 1;
					editor->render_data.collision_capsules = allocate_memory<collision_shape_render_data>(&level->main_thread_frame_memory_arena, editor->render_data.collision_capsule_count);

					transform transform = level->entity_transforms[editor->entity_index];
					capsule capsule = {};
					capsule.radius = entity_collision_component->capsule.radius;
					capsule.begin = {0, -entity_collision_component->capsule.height / 2, 0};
					capsule.end = {0, entity_collision_component->capsule.height / 2, 0};
					capsule.begin = transform.rotate * capsule.begin;
					capsule.end = transform.rotate * capsule.end;
					mat4 cylinder_scale_mat = mat4_from_scale({capsule.radius, entity_collision_component->capsule.height, capsule.radius});
					mat4 cylinder_rotate_mat = mat4_from_rotate(quat_from_between({0, 1, 0}, vec3_normalize(capsule.end - (capsule.begin + capsule.end) / 2)));
					mat4 cylinder_translate_mat = mat4_from_translate(transform.translate);
					mat4 sphere_scale_mat = mat4_from_scale({capsule.radius, capsule.radius, capsule.radius});
					mat4 sphere_translate_mats[2] = {mat4_from_translate(transform.translate + capsule.begin), mat4_from_translate(transform.translate + capsule.end)};
					editor->render_data.collision_capsules[0].transform = cylinder_translate_mat * cylinder_rotate_mat * cylinder_scale_mat;
					editor->render_data.collision_capsules[0].capsule_sphere_transforms[0] = sphere_translate_mats[0] * sphere_scale_mat;
					editor->render_data.collision_capsules[0].capsule_sphere_transforms[1] = sphere_translate_mats[1] * sphere_scale_mat;
					editor->render_data.collision_capsules[0].color = vec4{0, 1, 0, 0.25f};
				}
				if (entity_collision_component->shape == collision_shape_box) {
					editor->render_data.collision_bound_count = 1;
					editor->render_data.collision_bounds = allocate_memory<collision_shape_render_data>(&level->main_thread_frame_memory_arena, editor->render_data.collision_bound_count);

					transform transform = level->entity_transforms[editor->entity_index];
					auto box = entity_collision_component->box;
					editor->render_data.collision_bounds[0].transform = mat4_from_translate(transform.translate) * mat4_from_scale(box.size);
					editor->render_data.collision_bounds[0].color = vec4{0, 1, 0, 0.25f};
				}
			}
	    // imgui
			ImDrawData *imgui_draw_data = ImGui::GetDrawData();
			for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
				ImDrawList *dlist = imgui_draw_data->CmdLists[i];
				uint32 offset = append_vulkan_dynamic_vertex_region(vulkan, dlist->VtxBuffer.Data, dlist->VtxBuffer.Size * sizeof(ImDrawVert), sizeof(ImDrawVert));
				append_vulkan_dynamic_vertex_region(vulkan, dlist->IdxBuffer.Data, dlist->IdxBuffer.Size * sizeof(ImDrawIdx), sizeof(ImDrawVert));
				if (i == 0) {
					editor->render_data.imgui_dynamic_vertex_region_buffer_offset = offset;
				}
			}
		};
   	auto extra_color_render_pass_commands = [&] {
			VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];
			if (editor->render_data.terrain_brush) {
				vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.color_vertex_pipeline);
				VkDeviceSize offset = 0;
				vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.vertex_region_buffer, &offset);
				shader_color_vertex_push_constant pc = {};
				pc.transform_mat = editor->render_data.terrain_brush_transform_mat;
				pc.color = vec4{1, 0, 0, 1};
				pc.height_map_index = editor->render_data.terrain_brush_height_map_descriptor_index;
				vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
				vkCmdDraw(cmd_buffer, m_countof(hollow_circle_vertices), 1, level->persistant_data.hollow_circle_vertex_region_buffer_offset / sizeof(vec3), 0);
			}
			if (editor->render_data.collision_sphere_count + editor->render_data.collision_capsule_count + editor->render_data.collision_bound_count > 0) {
				vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.color_vertex_pipeline);
				VkDeviceSize vertices_offset = 0;
				vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.vertex_region_buffer, &vertices_offset);
				for (uint32 i = 0; i < editor->render_data.collision_sphere_count; i += 1) {
					shader_color_vertex_push_constant pc = {editor->render_data.collision_spheres[i].transform, editor->render_data.collision_spheres[i].color, level->persistant_data.default_height_map_descriptor_index};
					vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
					vkCmdDraw(cmd_buffer, m_countof(sphere_vertices), 1, level->persistant_data.sphere_vertex_region_buffer_offset / sizeof(vec3), 0);
				}
				for (uint32 i = 0; i < editor->render_data.collision_capsule_count; i += 1) {
					shader_color_vertex_push_constant pc = {editor->render_data.collision_capsules[i].transform, editor->render_data.collision_capsules[i].color, level->persistant_data.default_height_map_descriptor_index};
					vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
					vkCmdDraw(cmd_buffer, m_countof(cylinder_vertices), 1, level->persistant_data.cylinder_vertex_region_buffer_offset / sizeof(vec3), 0);

					pc = {editor->render_data.collision_capsules[i].capsule_sphere_transforms[0], editor->render_data.collision_capsules[i].color};
					vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
					vkCmdDraw(cmd_buffer, m_countof(sphere_vertices), 1, level->persistant_data.sphere_vertex_region_buffer_offset / sizeof(vec3), 0);

					pc = {editor->render_data.collision_capsules[i].capsule_sphere_transforms[1], editor->render_data.collision_capsules[i].color};
					vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
					vkCmdDraw(cmd_buffer, m_countof(sphere_vertices), 1, level->persistant_data.sphere_vertex_region_buffer_offset / sizeof(vec3), 0);
				}
				for (uint32 i = 0; i < editor->render_data.collision_bound_count; i += 1) {
					shader_color_vertex_push_constant pc = {editor->render_data.collision_bounds[i].transform, editor->render_data.collision_bounds[i].color, level->persistant_data.default_height_map_descriptor_index};
					vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
					vkCmdDraw(cmd_buffer, m_countof(bound_vertices), 1, level->persistant_data.bound_vertex_region_buffer_offset / sizeof(vec3), 0);
				}
			}
		};
		auto extra_swap_chain_render_pass_commands = [&] {
			VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];
			ImDrawData *imgui_draw_data = ImGui::GetDrawData();
			if (imgui_draw_data->CmdListsCount > 0) {
				vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.imgui_pipeline);
				VkDeviceSize vertices_offset = 0;
				vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.dynamic_vertex_region_buffers[vulkan->frame_index], &vertices_offset);
				vkCmdBindIndexBuffer(cmd_buffer, vulkan->memory_regions.dynamic_vertex_region_buffers[vulkan->frame_index], vertices_offset, VK_INDEX_TYPE_UINT16);
				uint32 buffer_offset = editor->render_data.imgui_dynamic_vertex_region_buffer_offset;
				for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
					ImDrawList *dlist = imgui_draw_data->CmdLists[i];
					uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
					uint32 indices_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
					uint32 vertex_index = buffer_offset / sizeof(ImDrawVert);
					buffer_offset += vertices_size;
					uint32 element_index = buffer_offset / sizeof(ImDrawIdx);
					buffer_offset += round_up(indices_size, (uint32)sizeof(ImDrawVert));
					for (int32 i = 0; i < dlist->CmdBuffer.Size; i += 1) {
						ImDrawCmd *dcmd = &dlist->CmdBuffer.Data[i];
						VkRect2D scissor = {{(int32)dcmd->ClipRect.x, (int32)dcmd->ClipRect.y}, {(uint32)(dcmd->ClipRect.z - dcmd->ClipRect.x), (uint32)(dcmd->ClipRect.w - dcmd->ClipRect.y)}};
						vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
						shader_imgui_push_constant pc = {};
						pc.viewport = {(float)vulkan->swap_chain.image_width, (float)vulkan->swap_chain.image_height};
						pc.texture_index = (uint32)(intptr_t)dcmd->TextureId;
						pc.texture_is_grayscale = false;
						vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
						vkCmdDrawIndexed(cmd_buffer, dcmd->ElemCount, 1, element_index, vertex_index, 0);
						element_index += dcmd->ElemCount;
					}
				}
			}
		};
		vulkan_begin_render(vulkan);
		level_generate_render_data(level, vulkan, editor->camera, generate_editor_render_data);
		level_generate_render_commands(level, vulkan, editor->camera, extra_color_render_pass_commands, extra_swap_chain_render_pass_commands);
		bool screen_shot = ImGui::IsKeyReleased(keycode_print_screen);
		vulkan_uniform_region_size = vulkan->memory_regions.uniform_region_sizes[vulkan->frame_index];
		vulkan_dynamic_vertex_region_size = vulkan->memory_regions.dynamic_vertex_region_sizes[vulkan->frame_index];
		vulkan_end_render(vulkan, screen_shot);

		{
			entity_addition *addition = level->entity_addition;
			uint32 addition_count = 0;
			while (addition) {
				addition_count += 1;
				addition = addition->next;
			}
			if (addition_count > 0) {
				editor->entity_index = level->entity_count + addition_count - 1;
			}
		}

		entity_components_memory_arena_size = level->entity_components_memory_arenas[level->entity_components_memory_arena_index].size;
		level_update_entity_components(level);

		main_thread_frame_memory_arena_size = level->main_thread_frame_memory_arena.size;
		render_thread_frame_memory_arena_size = level->render_thread_frame_memory_arena.size;
		level->main_thread_frame_memory_arena.size = 0;
		level->render_thread_frame_memory_arena.size = 0;
	
		QueryPerformanceCounter(&performance_counters[1]);
		last_frame_time_microsec = (performance_counters[1].QuadPart - performance_counters[0].QuadPart) * 1000000 / performance_frequency.QuadPart;
		last_frame_time_sec = (double)last_frame_time_microsec / 1000000;
	}
	ImGui::Shutdown();
}
