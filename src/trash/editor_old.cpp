/***************************************************************************************************/
/*					Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.					 */
/***************************************************************************************************/

#include "level.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "imgui/imgui.cpp"
#include "imgui/imgui_draw.cpp"
#include "imgui/wip/imgui_tabs.cpp"
#include "imgui/ImGuizmo.cpp"

enum entity_type {
	entity_type_player,
	entity_type_generic,
	entity_type_model,
	entity_type_terrain,
	entity_type_skybox,
	entity_type_light
};

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

	ImGuiContext *imgui_context;

	uint32 font_atlas_image_descriptor_index;
	uint32 translate_icon_image_descriptor_index;
	uint32 rotate_icon_image_descriptor_index;
	uint32 scale_icon_image_descriptor_index;
	uint32 terrain_bump_icon_image_descriptor_index;
	uint32 terrain_raise_lower_icon_image_descriptor_index;
	uint32 terrain_flatten_icon_image_descriptor_index;
	uint32 terrain_height_icon_image_descriptor_index;
	uint32 terrain_ramp_icon_image_descriptor_index;
	uint32 terrain_smooth_icon_image_descriptor_index;
	uint32 terrain_noise_icon_image_descriptor_index;
	uint32 terrain_water_icon_image_descriptor_index;
	uint32 terrain_hole_icon_image_descriptor_index;
	uint32 terrain_paint_icon_image_descriptor_index;
	uint32 terrain_tree_icon_image_descriptor_index;
	uint32 terrain_road_icon_image_descriptor_index;

	uint32 *terrain_paint_image_descriptor_indices;
	uint32 terrain_paint_image_descriptor_index_count;
	uint32 terrain_paint_image_descriptor_index;
	
	bool error_popup;
	char error_popup_msg[256];
	bool quit_popup;
	bool add_generic_entity_popup;
	bool rename_generic_entity_popup;

	float top_menu_bar_height;
	ImVec4 top_menu_bar_color;
	ImVec2 entity_window_pos;
	ImVec2 entity_window_size;
	ImVec2 memory_window_pos;
	ImVec2 memory_window_size;
	ImVec2 terrain_brush_properties_window_pos;
	ImVec2 terrain_brush_properties_window_size;

	bool show_frame_statistic_window;
	
	camera camera;
	float camera_pitch;
	float camera_move_speed;
	float camera_rotate_speed;
	bool camera_moving;
	bool mouse_in_display_area;
	ray camera_ray;

	bool show_reference_grid;
	bool show_collision_shape;

	bool show_camera_settings_window;

	int32 entity_type;
	uint32 generic_entity_index;
	uint32 model_index;

	int32 tool_type;
	bool terrain_brush_tool_active;
	vec3 terrain_brush_tool_position;
	float terrain_brush_tool_radius;
	float terrain_brush_tool_hardness;
	float terrain_brush_tool_speed;

	char level_save_file[256];
};

void initialize_editor(editor *editor, vulkan *vulkan) {
	editor->imgui_context = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO &imgui_io = ImGui::GetIO();
	imgui_io.KeyMap[ImGuiKey_Tab] = VK_TAB;
	imgui_io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	imgui_io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	imgui_io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	imgui_io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	imgui_io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	imgui_io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	imgui_io.KeyMap[ImGuiKey_Home] = VK_HOME;
	imgui_io.KeyMap[ImGuiKey_End] = VK_END;
	imgui_io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	imgui_io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	imgui_io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	imgui_io.KeyMap[ImGuiKey_A] = 'A';
	imgui_io.KeyMap[ImGuiKey_C] = 'C';
	imgui_io.KeyMap[ImGuiKey_V] = 'V';
	imgui_io.KeyMap[ImGuiKey_X] = 'X';
	imgui_io.KeyMap[ImGuiKey_Y] = 'Y';
	imgui_io.KeyMap[ImGuiKey_Z] = 'Z';
	imgui_io.DisplaySize = {(float)vulkan->framebuffers.color_framebuffer_width, (float)vulkan->framebuffers.color_framebuffer_height};
	ImGuizmo::SetRect(0, 0, imgui_io.DisplaySize.x, imgui_io.DisplaySize.y);
	imgui_io.IniFilename = nullptr;
	imgui_io.MousePos = {-1, -1};
	imgui_io.FontGlobalScale = (float)vulkan->framebuffers.color_framebuffer_width / (float)GetSystemMetrics(SM_CXSCREEN);

	{ // font atlas image
		imgui_io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", (float)GetSystemMetrics(SM_CXSCREEN) / 100);
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
		uint32 image_index = append_vulkan_images(vulkan, image_info, image_view_info, font_atlas_image, font_atlas_image_width * font_atlas_image_height * 4, 1, 4);
		VkImageView image_view = vulkan->memory_regions.images.images[image_index].view;
		editor->font_atlas_image_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image_view, vulkan->samplers.mipmap_samplers[0]);
		imgui_io.Fonts->SetTexID((ImTextureID)(intptr_t)editor->font_atlas_image_descriptor_index);
		imgui_io.Fonts->ClearTexData();
	}
	{ // images
		auto add_image = [&](const char *file, bool srgb = true) {
			int32 width, height, channel;
			uint8 *image = stbi_load(file, &width, &height, &channel, 4);
			m_assert(image, "");

			VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = srgb ? VK_FORMAT_R8G8B8A8_SRGB : VK_FORMAT_R8G8B8A8_UNORM;
			image_info.extent = {(uint32)width, (uint32)height, 1};
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = image_info.format;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.layerCount = 1;

			uint32 image_index = append_vulkan_images(vulkan, image_info, image_view_info, image, width * height * 4, 1, 4);
			VkImageView image_view = vulkan->memory_regions.images.images[image_index].view;
			uint32 descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image_view, vulkan->samplers.mipmap_samplers[0]);
			stbi_image_free(image);
			return descriptor_index;
		};
		editor->translate_icon_image_descriptor_index = add_image("assets/images/icons/translate.png", false);
		editor->rotate_icon_image_descriptor_index = add_image("assets/images/icons/rotate.png", false);
		editor->scale_icon_image_descriptor_index = add_image("assets/images/icons/scale.png", false);
		editor->terrain_bump_icon_image_descriptor_index = add_image("assets/images/icons/terrain_bump.png", false);
		editor->terrain_raise_lower_icon_image_descriptor_index = add_image("assets/images/icons/terrain_raise_lower.png", false);
		editor->terrain_flatten_icon_image_descriptor_index = add_image("assets/images/icons/terrain_flatten.png", false);
		editor->terrain_height_icon_image_descriptor_index = add_image("assets/images/icons/terrain_height.png", false);
		editor->terrain_ramp_icon_image_descriptor_index = add_image("assets/images/icons/terrain_ramp.png", false);
		editor->terrain_smooth_icon_image_descriptor_index = add_image("assets/images/icons/terrain_smooth.png", false);
		editor->terrain_noise_icon_image_descriptor_index = add_image("assets/images/icons/terrain_noise.png", false);
		editor->terrain_water_icon_image_descriptor_index = add_image("assets/images/icons/terrain_water.png", false);
		editor->terrain_hole_icon_image_descriptor_index = add_image("assets/images/icons/terrain_hole.png", false);
		editor->terrain_paint_icon_image_descriptor_index = add_image("assets/images/icons/terrain_paint.png", false);
		editor->terrain_tree_icon_image_descriptor_index = add_image("assets/images/icons/terrain_tree.png", false);
		editor->terrain_road_icon_image_descriptor_index = add_image("assets/images/icons/terrain_road.png", false);
		{
			editor->terrain_paint_image_descriptor_index_count = 0;
			iterate_dir_files("assets/images/terrain", [editor](const char *file_name) {
				editor->terrain_paint_image_descriptor_index_count += 1;
			});
			editor->terrain_paint_image_descriptor_indices = new uint32[editor->terrain_paint_image_descriptor_index_count];
			uint32 i = 0;
			iterate_dir_files("assets/images/terrain", [editor, &i, add_image](const char *file_name) {
				char path[256];
				snprintf(path, sizeof(path), "assets/images/terrain/%s", file_name);
				editor->terrain_paint_image_descriptor_indices[i++] = add_image(path, true);
			});
			editor->terrain_paint_image_descriptor_index = UINT32_MAX;
		}
	}
	
	editor->camera.position = vec3{4, 8, 8};
	editor->camera.view = vec3_normalize(-editor->camera.position);
	editor->camera.fovy = degree_to_radian(50);
	editor->camera.aspect = (float)vulkan->framebuffers.color_framebuffer_width / (float)vulkan->framebuffers.color_framebuffer_height;
	editor->camera.znear = 0.1f;
	editor->camera.zfar = 1000;
	editor->camera_pitch = asinf(editor->camera.view.y);
	editor->camera_move_speed = 20;
	editor->camera_rotate_speed = 1;

	editor->generic_entity_index = UINT32_MAX;
	editor->model_index = UINT32_MAX;

	editor->terrain_brush_tool_radius = 5;
	editor->terrain_brush_tool_hardness = 0.5f;
	editor->terrain_brush_tool_speed = 0.5f;
}

struct load_editor_settings {
	editor *editor;
	void operator()(const flatbuffers::Level *level) {
		auto editor_settings = level->editorSettings();
		if (editor_settings) {
			auto position = editor_settings->cameraPosition();
			auto view = editor_settings->cameraView();
			editor->camera.position = {position.x(), position.y(), position.z()};
			editor->camera.view = {view.x(), view.y(), view.z()};
			editor->camera_pitch = asinf(editor->camera.view.y);
			editor->camera_move_speed = editor_settings->cameraMoveSpeed();
			editor->camera_rotate_speed = editor_settings->cameraRotateSpeed();
		}
	}
};

struct save_editor_settings {
	editor *editor;
	flatbuffers::EditorSettings operator()() {
		flatbuffers::Vec3 camera_position = flatbuffers::Vec3(m_unpack3(editor->camera.position));
		flatbuffers::Vec3 camera_view = flatbuffers::Vec3(m_unpack3(editor->camera.view));
		return flatbuffers::EditorSettings(camera_position, camera_view, editor->camera_move_speed, editor->camera_rotate_speed);
	}
};

bool save_editor_level(editor *editor, level *level) {
	if (!strcmp(editor->level_save_file, "")) {
		if (!open_file_dialog(editor->level_save_file, sizeof(editor->level_save_file))) {
			return false;
		}
	}
	if (!save_level(level, editor->level_save_file, save_editor_settings{editor})) {
		return false;
	}
	for (uint32 i = 0; i < level->terrain_count; i += 1) {
		terrain *terrain = &level->terrains[i];
		m_assert(terrain->height_map, "");
		m_assert(terrain->diffuse_map, "");
		file_mapping gpk_file_mapping;
		char file_path[128] = "assets/terrains/";
		strcat(file_path, terrain->gpk_file);
		m_assert(open_file_mapping(file_path, &gpk_file_mapping, false), "");
		gpk_terrain *gpk_terrain = (struct gpk_terrain *)gpk_file_mapping.ptr;
		uint8 *height_map_ptr = gpk_file_mapping.ptr + gpk_terrain->height_map_offset;
		uint8 *diffuse_map_ptr = gpk_file_mapping.ptr + gpk_terrain->diffuse_map_offset;
		uint32 texture_size = terrain->width * terrain->sample_per_meter * terrain->height * terrain->sample_per_meter;
		memcpy(height_map_ptr, terrain->height_map, texture_size * sizeof(int16));
		memcpy(diffuse_map_ptr, terrain->diffuse_map, texture_size * sizeof(uint32));
    flush_file_mapping(gpk_file_mapping);
		close_file_mapping(&gpk_file_mapping);
	}
	return true;
}

#if 0
void imgui_render_popups(editor *editor, level *level) {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::PushID("error_popup");
	if (editor->error_popup) {
		editor->error_popup = false;
		ImGui::OpenPopup("##popup");
	}
	if (ImGui::BeginPopupModal("##popup", nullptr, window_flags)) {
		ImGui::Text("%s", editor->error_popup_msg);
		if (ImGui::Button("ok")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();

	ImGui::PushID("quit_popup");
	if (editor->quit_popup) {
		editor->quit_popup = false;
		ImGui::OpenPopup("##popup");
	}
	if (ImGui::BeginPopupModal("##popup", nullptr, window_flags)) {
		ImGui::Text("Save changes?");
		if (ImGui::Button("Yes")) {
			save_editor_level_status status = save_editor_level(editor, level);
			if (status == save_editor_level_error) {
				ImGui::OpenPopup("##save_failure_popup");
			}
			else if (status == save_editor_level_done) {
				ImGui::CloseCurrentPopup();
				editor->quit = true;
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
		if (ImGui::BeginPopupModal("##save_failure_popup", nullptr, window_flags)) {
			ImGui::Text("failed to save level to file: \"%s\"", editor->level_save_file);
			if (ImGui::Button("Ok")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();

	ImGui::PushID("add_generic_entity_popup");
	if (editor->add_generic_entity_popup) {
		editor->add_generic_entity_popup = false;
		ImGui::OpenPopup("##popup");
	}
	if (ImGui::BeginPopupModal("##popup", nullptr, window_flags)) {
		static char id[32] = {};
		ImGui::InputText("id", id, sizeof(id));
		if (ImGui::Button("Ok")) {
			if (generic_entity_id_exist(level, id)) {
				ImGui::OpenPopup("##id_exist_popup");
			}
			else {
				add_level_generic_entity(level, id);
				editor->generic_entity_index = level->generic_entity_count - 1;
				id[0] = '\0';
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			id[0] = '\0';
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::BeginPopupModal("##id_exist_popup", nullptr, window_flags)) {
			ImGui::TextColored({1, 0, 0, 1}, "Error: id %s already exist", id);
			if (ImGui::Button("Ok")) {
				id[0] = '\0';
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();

	ImGui::PushID("rename_generic_entity_popup");
	if (editor->rename_generic_entity_popup) {
		editor->rename_generic_entity_popup = false;
		ImGui::OpenPopup("##popup");
	}
	if (ImGui::BeginPopupModal("##popup", nullptr, window_flags)) {
		static char id[32] = {};
		ImGui::InputText("id", id, sizeof(id));
		if (ImGui::Button("Ok")) {
			if (generic_entity_id_exist(level, id)) {
				ImGui::OpenPopup("##id_exist_popup");
			}
			else {
				auto *entity = &level->generic_entities[editor->generic_entity_index];
				if (strlen(id) > strlen(entity->id)) {
					entity->id = allocate_memory<char>(&level->general_memory_arena, strlen(id) + 1);
				}
				strcpy(entity->id, id);
				id[0] = '\0';
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			id[0] = '\0';
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::BeginPopupModal("##id_exist_popup", nullptr, window_flags)) {
			ImGui::TextColored({1, 0, 0, 1}, "Error: id %s already exist", id);
			if (ImGui::Button("Ok")) {
				id[0] = '\0';
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopID();
}
#endif

#if 0
void imgui_render_generic_entity(editor *editor, level *level, generic_entity *entity) {
	const char *model_file = entity->model_index < level->model_count ? level->models[entity->model_index].gpk_file : nullptr;
	if (ImGui::BeginCombo("model", model_file)) {
		for (uint32 i = 0; i < level->model_count; i += 1) {
			if (ImGui::Selectable(level->models[i].gpk_file, entity->model_index == i)) {
				entity->model_index = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PushID("transform");
	ImGui::InputFloat3("T", &entity->transform.translate[0]);
	vec3 rotate = quat_get_euler_angles(entity->transform.rotate);
	ImGui::InputFloat3("R", &rotate[0], "%.3f", ImGuiInputTextFlags_ReadOnly);
	float scale = entity->transform.scale[0];
	if (ImGui::InputFloat("S", &scale) && scale > 0) {
		entity->transform.scale = {scale, scale, scale};
	}
	if (ImGui::Button("Reset Transform")) {
		entity->transform = transform_identity();
	}
	ImGui::PopID();

	ImGui::Separator();
	ImGui::PushID("collision");
	if (!entity->collision_shape) {
		if (ImGui::Button("Add Collision")) {
			entity->collision_shape = allocate_memory<btSphereShape>(&level->general_memory_arena, 1);
			new(entity->collision_shape) btSphereShape(1);
		}
	}
	else {
		auto bt_shape_type = entity->collision_shape->getShapeType();
		const char *shapes[] = {"sphere", "box", "capsule_x", "capsule_y", "capsule_z", "cylinder_x", "cylinder_y", "cylinder_z"};
		uint32 shape_index;
		if (bt_shape_type == SPHERE_SHAPE_PROXYTYPE) {
			shape_index = 0;
		}
		else if (bt_shape_type == BOX_SHAPE_PROXYTYPE) {
			shape_index = 1;
		}
		else if (bt_shape_type == CAPSULE_SHAPE_PROXYTYPE) {
			btCapsuleShape *bt_capsule = (btCapsuleShape *)entity->collision_shape;
			int32 up_axis = bt_capsule->getUpAxis();
			m_assert(up_axis >= 0 && up_axis < 3, "invalid entity collision capsule axis %d", up_axis);
			uint32 indices[] = {2, 3, 4};
			shape_index = indices[up_axis];
		}
		else if (bt_shape_type == CYLINDER_SHAPE_PROXYTYPE) {
			btCylinderShape *bt_cylinder = (btCylinderShape *)entity->collision_shape;
			int32 up_axis = bt_cylinder->getUpAxis();
			m_assert(up_axis >= 0 && up_axis < 3, "invalid entity collision cylinder axis %d", up_axis);
			uint32 indices[] = {5, 6, 7};
			shape_index = indices[up_axis];
		}
		else {
			m_assert(false, "invalid entity collision shape");
		}
		if (ImGui::BeginCombo("shapes", shapes[shape_index])) {
			for (uint32 i = 0; i < m_countof(shapes); i += 1) {
				if (ImGui::Selectable(shapes[i], shape_index == i)) {
					if (shape_index != i) {
						shape_index = i;
						if (shape_index == 0) {
							entity->collision_shape = allocate_memory<btSphereShape>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btSphereShape(1);
						}
						else if (shape_index == 1) {
							entity->collision_shape = allocate_memory<btBoxShape>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btBoxShape(btVector3(1, 1, 1));
						}
						else if (shape_index == 2) {
							entity->collision_shape = allocate_memory<btCapsuleShapeX>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btCapsuleShapeX(1, 2);
						}
						else if (shape_index == 3) {
							entity->collision_shape = allocate_memory<btCapsuleShape>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btCapsuleShape(1, 2);
						}
						else if (shape_index == 4) {
							entity->collision_shape = allocate_memory<btCapsuleShapeZ>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btCapsuleShapeZ(1, 2);
						}
						else if (shape_index == 5) {
							entity->collision_shape = allocate_memory<btCylinderShapeX>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btCylinderShapeX(btVector3(1, 1, 1));
						}
						else if (shape_index == 6) {
							entity->collision_shape = allocate_memory<btCylinderShape>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btCylinderShape(btVector3(1, 1, 1));
						}
						else if (shape_index == 7) {
							entity->collision_shape = allocate_memory<btCylinderShapeZ>(&level->general_memory_arena, 1);
							new(entity->collision_shape) btCylinderShapeZ(btVector3(1, 1, 1));
						}
						else {
							m_assert(false, "");
						}
					}
				}
			}
			ImGui::EndCombo();
		}
		
		if (bt_shape_type == SPHERE_SHAPE_PROXYTYPE) {
			btSphereShape *bt_sphere = (btSphereShape *)entity->collision_shape;
			float radius = bt_sphere->getRadius();
			if (ImGui::InputFloat("radius", &radius)) {
				new(bt_sphere) btSphereShape(radius);
			}
		}
		else if (bt_shape_type == BOX_SHAPE_PROXYTYPE) {
			btBoxShape *bt_box = (btBoxShape *)entity->collision_shape;
			btVector3 half_extents = bt_box->getHalfExtentsWithMargin();
			if (ImGui::InputFloat3("half extents", (float *)half_extents)) {
				new(bt_box) btBoxShape(half_extents);
			}
		}
		else if (bt_shape_type == CAPSULE_SHAPE_PROXYTYPE) {
			btCapsuleShape *bt_capsule = (btCapsuleShape *)entity->collision_shape;
			int up_axis = bt_capsule->getUpAxis();
			float radius = bt_capsule->getRadius();
			float half_height = bt_capsule->getHalfHeight();
			int n = ImGui::InputFloat("radius", &radius) + ImGui::InputFloat("half height", &half_height);
			if (n > 0) {
				if (up_axis == 0) {
					new(bt_capsule) btCapsuleShapeX(radius, half_height * 2);
				}
				else if (up_axis == 1) {
					new(bt_capsule) btCapsuleShape(radius, half_height * 2);
				}
				else if (up_axis == 2) {
					new(bt_capsule) btCapsuleShapeZ(radius, half_height * 2);
				}
			}
		}
		else if (bt_shape_type == CYLINDER_SHAPE_PROXYTYPE) {
			btCylinderShape *bt_cylinder = (btCylinderShape *)entity->collision_shape;
			int up_axis = bt_cylinder->getUpAxis();
			if (up_axis == 0) {
				float radius = bt_cylinder->getHalfExtentsWithMargin().y();
				float half_height = bt_cylinder->getHalfExtentsWithMargin().x();
				if (ImGui::InputFloat("radius", &radius) + ImGui::InputFloat("half height", &half_height) > 0) {
					new(bt_cylinder) btCylinderShapeX(btVector3(half_height, radius, radius));
				}
			}
			else if (up_axis == 1) {
				float radius = bt_cylinder->getHalfExtentsWithMargin().x();
				float half_height = bt_cylinder->getHalfExtentsWithMargin().y();
				if (ImGui::InputFloat("radius", &radius) + ImGui::InputFloat("half height", &half_height) > 0) {
					new(bt_cylinder) btCylinderShape(btVector3(radius, half_height, radius));
				}
			}
			else if (up_axis == 2) {
				float radius = bt_cylinder->getHalfExtentsWithMargin().x();
				float half_height = bt_cylinder->getHalfExtentsWithMargin().z();
				if (ImGui::InputFloat("radius", &radius) + ImGui::InputFloat("half height", &half_height) > 0) {
					new(bt_cylinder) btCylinderShapeZ(btVector3(radius, radius, half_height));
				}
			}
		}
		else {
			m_assert(false, "");
		}
		if (ImGui::Button("Remove Collision")) {
			entity->collision_shape = nullptr;
		}
	}
	ImGui::PopID();

	ImGui::Separator();
	ImGui::PushID("physics");
	if (entity->collision_object && entity->collision_object->getInternalType() == btCollisionObject::CO_RIGID_BODY) {
		float mass = 1.0f / ((btRigidBody *)entity->collision_object)->getInvMass();
		if (ImGui::InputFloat("mass", &mass)) {
			((btRigidBody *)entity->collision_object)->setMassProps(mass, btVector3(0, 0, 0));
		}
		if (ImGui::Button("Remove Physics")) {
			if (entity->collision_shape) {
				entity->collision_object = allocate_memory<btCollisionObject>(&level->general_memory_arena, 1);
				new(entity->collision_object) btCollisionObject();
				entity->collision_object->setCollisionShape(entity->collision_shape);
			}
			else {
				entity->collision_object = nullptr;
			}
		}
	}
	else {
		if (ImGui::Button("Add Physics")) {
			entity->collision_object = allocate_memory<btRigidBody>(&level->general_memory_arena, 1);
			new(entity->collision_object) btRigidBody(1, nullptr, entity->collision_shape);
		}
	}
	ImGui::PopID();
}
#endif

#if 0
void imgui_render_generic_entities(editor *editor, level *level) {
	generic_entity *entity = editor->generic_entity_index < level->generic_entity_count ? &level->generic_entities[editor->generic_entity_index] : nullptr;
	if (ImGui::Button("Add")) {
		editor->add_generic_entity_popup = true;
	}
	if (entity) {
		ImGui::SameLine();
		if (ImGui::Button("Remove")) {
			remove_level_generic_entity(level, editor->generic_entity_index);
			editor->generic_entity_index = UINT32_MAX;
		}
		ImGui::SameLine();
		if (ImGui::Button("Rename")) {
			editor->rename_generic_entity_popup = true;
		}
	}
	char *entity_id = entity ? entity->id : nullptr;
	if (ImGui::BeginCombo("IDs", entity_id)) {
		for (uint32 i = 0; i < level->generic_entity_count; i += 1) {
			if (ImGui::Selectable(level->generic_entities[i].id, editor->generic_entity_index == i)) {
				editor->generic_entity_index = i;
			}
		}
		ImGui::EndCombo();
	}
	if (entity) {
		imgui_render_generic_entity(editor, level, entity);
	}
}
#endif

#if 0
void imgui_render_models(editor *editor, level *level, vulkan *vulkan) {
	model *model = editor->model_index < level->model_count ? &level->models[editor->model_index] : nullptr;
	if (ImGui::Button("Add")) {
		char gpk_file[256] = {};
		if (open_file_dialog(gpk_file, sizeof(gpk_file))) {
			if (!add_level_model(level, vulkan, gpk_file, transform_identity(), true)) {
				snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "Err: add_level_model\nFile: %s", gpk_file);
				editor->error_popup = true;
			}
		}
	}
	ImGui::Separator();
	const char *model_file = model ? model->gpk_file : nullptr;
	if (ImGui::BeginCombo("models", model_file)) {
		for (uint32 i = 0; i < level->model_count; i += 1) {
			if (ImGui::Selectable(level->models[i].gpk_file, editor->model_index == i)) {
				editor->model_index = i;
			}
		}
		ImGui::EndCombo();
	}
	if (model) {
		ImGui::PushID("transform");
		vec3 rotate = quat_get_euler_angles(model->transform.rotate);
		float scale = model->transform.scale[0];
		ImGui::InputFloat3("T", &model->transform.translate[0]);
		ImGui::InputFloat3("R", &rotate[0], "%.3f", ImGuiInputTextFlags_ReadOnly);
		if (ImGui::InputFloat("S", &scale) && scale > 0) {
			model->transform.scale = {scale, scale, scale};
		}
		if (ImGui::Button("Reset Transform")) {
			model->transform = transform_identity();
		}
		ImGui::PopID();
	}
}
#endif

#if 0
void imgui_render_terrains(editor *editor, level *level, vulkan *vulkan) {
	terrain *terrain = level->terrain_index < level->terrain_count ? &level->terrains[level->terrain_index] : nullptr;
	if (ImGui::Button("Add")) {
		char gpk_file[256] = {};
		if (open_file_dialog(gpk_file, sizeof(gpk_file))) {
			if (file_exists(gpk_file)) {
				if (!add_level_terrain(level, vulkan, gpk_file, true)) {
					snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "Err: add_level_terrain\nFile: %s", gpk_file);
					editor->error_popup = true;
				}
			}
			else {
				gpk_terrain terrain = {};
				strcpy(terrain.format_str, m_gpk_terrain_format_str);
				terrain.width = 256;
				terrain.height = 256;
				terrain.max_height = 128;
				terrain.sample_per_meter = 4;
				uint32 texture_size = terrain.width * terrain.height * terrain.sample_per_meter * terrain.sample_per_meter;
				terrain.height_map_offset = round_up((uint32)sizeof(struct gpk_terrain), 16u);
				terrain.diffuse_map_offset = round_up(terrain.height_map_offset + texture_size * (uint32)sizeof(int16), 16u);
				terrain.total_size = terrain.diffuse_map_offset + texture_size * sizeof(uint32);
				file_mapping gpk_file_mapping;
				if (!create_file_mapping(gpk_file, terrain.total_size, &gpk_file_mapping)) {
					snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "cannot create file %s\n", gpk_file);
					editor->error_popup = true;
				}
				else {
					*(gpk_terrain *)gpk_file_mapping.ptr = terrain;
					int16 *height_map = (int16 *)(gpk_file_mapping.ptr + terrain.height_map_offset);
					uint8 *diffuse_map = (uint8 *)(gpk_file_mapping.ptr + terrain.diffuse_map_offset);
					memset(height_map, 0, texture_size * sizeof(int16));
					memset(diffuse_map, 150, texture_size * sizeof(uint32));
					flush_file_mapping(gpk_file_mapping);
					close_file_mapping(&gpk_file_mapping);
					add_level_terrain(level, vulkan, gpk_file, true);
					ImGui::CloseCurrentPopup();
				}
			}
		}
	}
	if (terrain) {
		ImGui::SameLine();
		if (ImGui::Button("Remove")) {
			array_remove_swap_end(level->terrains, &level->terrain_count, level->terrain_index);
			level->terrain_index = UINT32_MAX;
		}
	}
	ImGui::Separator();
	char *terrain_id = terrain ? terrain->gpk_file : nullptr;
	if (ImGui::BeginCombo("terrains", terrain_id)) {
		for (uint32 i = 0; i < level->terrain_count; i += 1) {
			if (ImGui::Selectable(level->terrains[i].gpk_file, level->terrain_index == i)) {
				level->terrain_index = i;
			}
		}
		ImGui::EndCombo();
	}
	if (terrain) {
		ImGui::SliderFloat("diffuse map uv repeat", &terrain->diffuse_map_uv_repeat, 1.0f, 100.0f);
	}
}
#endif

#if 0
void imgui_render_skyboxes(editor *editor, level *level, vulkan *vulkan) {
	skybox *skybox = level->skybox_index < level->skybox_count ? &level->skyboxes[level->skybox_index] : nullptr;
	if (ImGui::Button("Add")) {
		char gpk_file[256] = {};
		if (open_file_dialog(gpk_file, sizeof(gpk_file))) {
			if (!add_level_skybox(level, vulkan, gpk_file)) {
				snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "Err: add_level_skybox\nFile: %s", gpk_file);
				editor->error_popup = true;
			}
		}
	}
	if (skybox) {
		ImGui::SameLine();
		if (ImGui::Button("Remove")) {
			array_remove_swap_end(level->skyboxes, &level->skybox_count, level->skybox_index);
			level->skybox_index = UINT32_MAX;
		}
	}
	ImGui::Separator();
	char *skybox_id = skybox ? skybox->gpk_file : nullptr;
	if (ImGui::BeginCombo("skyboxes", skybox_id)) {
		for (uint32 i = 0; i < level->skybox_count; i += 1) {
			if (ImGui::Selectable(level->skyboxes[i].gpk_file, level->skybox_index == i)) {
				level->skybox_index = i;
			}
		}
		ImGui::EndCombo();
	}
}
#endif

#if 0
void imgui_render_lights(editor *editor, level *level) {
	ImGui::ColorEdit3("ambient color", &level->lights.ambient_light.color[0]);
	ImGui::ColorEdit3("direct color", &level->lights.direct_light.color[0]);
	float extra = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x * 2 + ImGui::CalcTextSize("direct color").x;
	ImGui::PushItemWidth(ImGui::GetItemRectSize().x - extra);
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::DragFloat3("", &level->lights.direct_light.direction[0]);
	ImGui::PopItemFlag();
	ImGui::PopItemWidth();
}
#endif

int main(int argc, char **argv) {
	set_exe_dir_as_current();

	window *window = new struct window;
	m_assert(initialize_window(window, window_message_handle_func), "");
	set_window_fullscreen(window, true);

	vulkan *vulkan = new struct vulkan;
	initialize_vulkan(vulkan, window);

	editor *editor = new struct editor;
	initialize_editor(editor, vulkan);

	level *level = new struct level;
	initialize_level(level, vulkan);
	if (argc > 1) {
		const char *level_file = argv[1];
		m_assert(load_level(level, vulkan, level_file, true, load_editor_settings{editor}), "");
		snprintf(editor->level_save_file, sizeof(editor->level_save_file), "%s", level_file);
	}

	window_message_channel.window = window;
	window_message_channel.initialized = true;
	show_window(window);

	timer timer = {};
	initialize_timer(&timer);
	uint64 last_frame_time_microsecs = 0;
	double last_frame_time_millisecs = 0;
	double last_frame_time_secs = 0;

	uint64 level_frame_memory_arena_size = 0;
	uint64 vulkan_frame_uniform_buffer_size = 0;
	uint64 vulkan_frame_vertex_buffer_size = 0;

	while (!editor->quit) {
		start_timer(&timer);

		window->raw_mouse_dx = 0;
		window->raw_mouse_dy = 0;
		handle_window_messages(window);

		ImGui::GetIO().DeltaTime = (float)last_frame_time_secs;
		{ // window mouse position to imgui
			float px = (float)window->mouse_x / vulkan->swap_chain.width;
			float py = (float)window->mouse_y / vulkan->swap_chain.height;
			vec4 region = vulkan->swap_chain_framebuffer_region;
			if (px >= region.x && px <= (region.x + region.z) && py >= region.y && py <= (region.y + region.w)) {
				ImGui::GetIO().MousePos.x = ImGui::GetIO().DisplaySize.x * (px - region.x) / region.z;
				ImGui::GetIO().MousePos.y = ImGui::GetIO().DisplaySize.y * (py - region.y) / region.w;
				editor->mouse_in_display_area = true;
			}
			else {
				ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
				editor->mouse_in_display_area = false;
			}
		}
		ImGui::NewFrame();
		{ // miscs
			if (ImGui::IsKeyPressed(VK_F11)) {
				set_window_fullscreen(window, !window->fullscreen);
			}
			if (window_message_channel.quit) {
				window_message_channel.quit = false;
				editor->quit_popup = true;
			}
			if (ImGui::IsKeyPressed(VK_F4) && ImGui::IsKeyDown(VK_MENU)) {
				editor->quit_popup = true;
			}
		}
		{ // camera
			if (ImGui::IsMouseClicked(2)) {
				pin_cursor(true);
				editor->camera_moving = true;
			}
			if (ImGui::IsMouseReleased(2)) {
				pin_cursor(false);
				editor->camera_moving = false;
			}
			if (editor->camera_moving) {
				if (ImGui::IsKeyDown('W')) {
					editor->camera.position = editor->camera.position + editor->camera.view * (float)last_frame_time_secs * editor->camera_move_speed;
				}
				if (ImGui::IsKeyDown('S')) {
					editor->camera.position = editor->camera.position - editor->camera.view * (float)last_frame_time_secs * editor->camera_move_speed;
				}
				if (ImGui::IsKeyDown('A')) {
					vec3 move_direction = vec3_normalize(vec3_cross(editor->camera.view, vec3{0, 1, 0}));
					editor->camera.position = editor->camera.position - move_direction * (float)last_frame_time_secs * editor->camera_move_speed;
				}
				if (ImGui::IsKeyDown('D')) {
					vec3 move_direction = vec3_normalize(vec3_cross(editor->camera.view, vec3{0, 1, 0}));
					editor->camera.position = editor->camera.position + move_direction * (float)last_frame_time_secs * editor->camera_move_speed;
				}
				float max_pitch = degree_to_radian(80.0f);
				float rotate = editor->camera_rotate_speed * ImGui::GetIO().DeltaTime;
				float yaw = -window->raw_mouse_dx * rotate;
				editor->camera.view = vec3_normalize(mat3_from_mat4(mat4_from_axis_rotate(vec3{0, 1, 0}, yaw)) * editor->camera.view);
				float pitch = -window->raw_mouse_dy * rotate;
				if (editor->camera_pitch + pitch > -max_pitch && editor->camera_pitch + pitch < max_pitch) {
					vec3 view_cross_up = vec3_normalize(vec3_cross(editor->camera.view, vec3{0, 1, 0}));
					mat4 rotate_mat = mat4_from_axis_rotate(view_cross_up, pitch);
					editor->camera.view = vec3_normalize(mat3_from_mat4(rotate_mat) * editor->camera.view);
					editor->camera_pitch += pitch;
				}
			}
			if (editor->mouse_in_display_area) {
				vec3 window_pos = vec3{ImGui::GetMousePos().x, ImGui::GetIO().DisplaySize.y - ImGui::GetMousePos().y, 0.1f};
				vec4 viewport = vec4{0, 0, ImGui::GetIO().DisplaySize.x , ImGui::GetIO().DisplaySize.y};
				vec3 mouse_world_position = mat4_unproject(window_pos, camera_view_mat4(editor->camera), camera_projection_mat4(editor->camera), viewport);
				editor->camera_ray = {editor->camera.position, vec3_normalize(mouse_world_position - editor->camera.position), editor->camera.zfar};
			}
		}
		{ // top menu bar
			ImGui::PushID("top_menu_bar");
			if (ImGui::BeginMainMenuBar()) {
				editor->top_menu_bar_height = ImGui::GetWindowHeight();
				editor->top_menu_bar_color = ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg);
				ImGui::PushID("file");
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open Level")) {
						if (open_file_dialog(editor->level_save_file, sizeof(editor->level_save_file))) {
							if (!load_level(level, vulkan, editor->level_save_file, true, load_editor_settings{editor})) {
								snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "failed to load level from file: \"%s\"", editor->level_save_file);
								editor->error_popup = true;
								editor->level_save_file[0] = '\0';
							}
						}
					}
					if (ImGui::MenuItem("Save Level")) {
						if (save_editor_level(editor, level) == save_editor_level_error) {
							snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "failed to save level to file: \"%s\"", editor->level_save_file);
							editor->error_popup = true;
						}
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Play")) {
						if (save_editor_level(editor, level) == save_editor_level_error) {
							snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "failed to save level to file: \"%s\"", editor->level_save_file);
							editor->error_popup = true;
						}
						else {
							STARTUPINFOA startup_info = {sizeof(startup_info)};
							PROCESS_INFORMATION process_info;
							char cmd_line[256] = {};
							snprintf(cmd_line, sizeof(cmd_line), "game.exe %s", editor->level_save_file);
							if (!CreateProcessA("game.exe", cmd_line, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &process_info)) {
								snprintf(editor->error_popup_msg, sizeof(editor->error_popup_msg), "cannot start game.exe: %s", &get_winapi_err_str()[0]);
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
					ImGui::MenuItem("Reference grid", nullptr, &editor->show_reference_grid);
					ImGui::MenuItem("Collision shape", nullptr, &editor->show_collision_shape);
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
		{ // bottom menu bar
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
				auto render_tool_button = [&](tool_type type, uint32 image_descriptor_index) {
					ImGui::SameLine();
					if (editor->tool_type == type) {
						ImGui::PushStyleColor(ImGuiCol_Button, button_hovered_color);
						if (ImGui::ImageButton((ImTextureID)(uintptr_t)image_descriptor_index, {size, size}, {0, 0},  {1, 1}, (int32)padding)) {
							editor->tool_type = type;
						}
						ImGui::PopStyleColor();
					}
					else {
						if (ImGui::ImageButton((ImTextureID)(uintptr_t)image_descriptor_index, {size, size}, {0, 0},  {1, 1}, (int32)padding)) {
							editor->tool_type = type;
						}
					}
				};
				for (auto type : {entity_type_player, entity_type_generic, entity_type_model}) {
					if (editor->entity_type == type) {
						render_tool_button(tool_type_translate, editor->translate_icon_image_descriptor_index);
						render_tool_button(tool_type_rotate, editor->rotate_icon_image_descriptor_index);
						render_tool_button(tool_type_scale, editor->scale_icon_image_descriptor_index);
						break;
					}
				}
				if (editor->entity_type == entity_type_terrain) {
					render_tool_button(tool_type_terrain_bump, editor->terrain_bump_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_raise_lower, editor->terrain_raise_lower_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_flatten, editor->terrain_flatten_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_height, editor->terrain_height_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_ramp, editor->terrain_ramp_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_smooth, editor->terrain_smooth_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_noise, editor->terrain_noise_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_water, editor->terrain_water_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_hole, editor->terrain_hole_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_paint, editor->terrain_paint_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_tree, editor->terrain_tree_icon_image_descriptor_index);
					render_tool_button(tool_type_terrain_road, editor->terrain_road_icon_image_descriptor_index);
				}
				ImGui::PopStyleColor();
			}
			ImGui::End();
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(1);
			ImGui::PopID();
		}
		{ // entity window
			ImGui::SetNextWindowPos(ImVec2{0, editor->top_menu_bar_height}, ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.8f}, ImGuiCond_Once);
			ImGui::PushID("entity_window");
			if (ImGui::Begin("Entity", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
				editor->entity_window_pos = ImGui::GetWindowPos();
				editor->entity_window_size = ImGui::GetWindowSize();
				const char *entity_types[] = {"Player", "Generic", "Model", "Terrain", "Skybox", "Light"};
				ImGui::BeginTabBar("entity_type");
				for (int32 i = 0; i < m_countof(entity_types); i += 1) {
					if (ImGui::TabItem(entity_types[i])) {
						editor->entity_type = i;
					}
				}
				ImGui::EndTabBar();
				ImGui::Separator();
				switch (editor->entity_type) {
					case entity_type_player: {
						ImGui::PushID("player");
						imgui_render_generic_entity(editor, level, &level->player);
						ImGui::PopID();
					} break;
					case entity_type_generic: {
						ImGui::PushID("generic");
						imgui_render_generic_entities(editor, level);
						ImGui::PopID();
					} break;
					case entity_type_model: {
						ImGui::PushID("model");
						imgui_render_models(editor, level, vulkan);
						ImGui::PopID();
					} break;
					case entity_type_terrain: {
						ImGui::PushID("terrain");
						imgui_render_terrains(editor, level, vulkan);
						ImGui::PopID();
					} break;
					case entity_type_skybox: {
						ImGui::PushID("skybox");
						imgui_render_skyboxes(editor, level, vulkan);
						ImGui::PopID();
					} break;
					case entity_type_light: {
						ImGui::PushID("light");
						imgui_render_lights(editor, level);
						ImGui::PopID();
					} break;
				}
			}
			ImGui::End();
			ImGui::PopID();
		}
		{ // memory window
			ImGui::SetNextWindowPos(ImVec2{ImGui::GetIO().DisplaySize.x * 0.8f, editor->top_menu_bar_height}, ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y * 0.5f * 0.8f}, ImGuiCond_Once);
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
				imgui_render_memory(level->general_memory_arena.size, level->general_memory_arena.capacity, "Level General");
				imgui_render_memory(level_frame_memory_arena_size, level->frame_memory_arena.capacity, "Level Frame");
				ImGui::Text("Vulkan Memories");
				imgui_render_memory(vulkan->memory_regions.images.memory_size, vulkan->memory_regions.images.memory_capacity, "Level Images");
				imgui_render_memory(vulkan->memory_regions.vertex_buffer.buffer_size, vulkan->memory_regions.vertex_buffer.buffer_capacity, "Level Vertex");
				imgui_render_memory(vulkan_frame_uniform_buffer_size, vulkan->memory_regions.frame_uniform_buffers[vulkan->frame_index].buffer_capacity, "Frame Uniform");
				imgui_render_memory(vulkan_frame_vertex_buffer_size, vulkan->memory_regions.frame_vertex_buffers[vulkan->frame_index].buffer_capacity, "Frame Vertex");
			}
			ImGui::End();
			ImGui::PopID();
		}
		{ // frame statistic window
			ImGui::PushID("frame_statistic_window");
			if (editor->show_frame_statistic_window) {
				if (ImGui::Begin("Frame statistic", &editor->show_frame_statistic_window)) {
					ImGui::Text("Frame time: %.3f", last_frame_time_millisecs);
				}
				ImGui::End();
			}
			ImGui::PopID();
		}
		{ // tool gizmo
			auto render_transform_gizmo = [editor](transform *transform) {
				for (auto tool_type : {tool_type_translate, tool_type_rotate, tool_type_scale}) {
					if (editor->tool_type == tool_type) {
						mat4 camera_view_mat = camera_view_mat4(editor->camera);
						mat4 camera_proj_mat = camera_projection_mat4(editor->camera);
						mat4 transform_mat = mat4_from_transform(*transform);
						ImGuizmo::OPERATION operations[] = {ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE};
						ImGuizmo::MODE modes[] = {ImGuizmo::WORLD, ImGuizmo::LOCAL, ImGuizmo::LOCAL};
						ImGuizmo::BeginFrame();
						ImGuizmo::Manipulate((float *)camera_view_mat, (float *)camera_proj_mat, operations[editor->tool_type], modes[editor->tool_type], (float *)transform_mat);
						*transform = mat4_get_transform(transform_mat);
						break;
					}
				}
			};
			if (editor->entity_type == entity_type_player) {
				render_transform_gizmo(&level->player.transform);
			}
			else if (editor->entity_type == entity_type_generic && editor->generic_entity_index < level->generic_entity_count) {
				render_transform_gizmo(&level->generic_entities[editor->generic_entity_index].transform);
			}
			else if (editor->entity_type == entity_type_model && editor->model_index < level->model_count) {
				render_transform_gizmo(&level->models[editor->model_index].transform);
			}
			else if (editor->entity_type == entity_type_terrain && level->terrain_index < level->terrain_count) {
				terrain *terrain = &level->terrains[level->terrain_index];
				float half_width = terrain->width * 0.5f;
				float half_height = terrain->height * 0.5f;
				bool tool_selected = editor->tool_type > tool_type_terrain_begin && editor->tool_type < tool_type_terrain_end;
				if (tool_selected) {
					ImGui::PushID("terrain_brush_properties_window");
					ImGui::SetNextWindowPos(editor->entity_window_pos + ImVec2(editor->entity_window_size.x, 0), ImGuiCond_Once);
					ImGui::SetNextWindowSize(ImVec2(editor->entity_window_size.x, 0), ImGuiCond_Once);
					if (ImGui::Begin("Terrain Brush")) {
						ImGui::SliderFloat("Radius", &editor->terrain_brush_tool_radius, 1, 10);
						ImGui::SliderFloat("Hardness", &editor->terrain_brush_tool_hardness, 0, 1);
						ImGui::SliderFloat("Speed", &editor->terrain_brush_tool_speed, 0, 1);
					}
					ImGui::End();
					ImGui::PopID();
					if (editor->tool_type == tool_type_terrain_paint) {
						ImGui::PushID("terrain_texture_window");
						ImGui::SetNextWindowPos(editor->entity_window_pos + ImVec2(editor->entity_window_size.x * 2, 0), ImGuiCond_Once);
						ImGui::SetNextWindowSize(ImVec2(editor->entity_window_size.x, 0), ImGuiCond_Once);
						if (ImGui::Begin("Terrain Textures")) {
							for (uint32 i = 0; i < editor->terrain_paint_image_descriptor_index_count; i += 1) {
								ImGui::SameLine();
								if (ImGui::ImageButton((ImTextureID)(intptr_t)editor->terrain_paint_image_descriptor_indices[i], ImVec2(32, 32))) {
									editor->terrain_paint_image_descriptor_index = editor->terrain_paint_image_descriptor_indices[i];
									terrain->diffuse_map_descriptor_index = editor->terrain_paint_image_descriptor_indices[i];
								}
							}
						}
						ImGui::End();
						ImGui::PopID();
					}
				}
				if (tool_selected && editor->mouse_in_display_area && ray_interect_plane(editor->camera_ray, plane{{0, 1, 0}, 0}, &editor->terrain_brush_tool_position)) {
					if (editor->terrain_brush_tool_position.x >= -half_width && editor->terrain_brush_tool_position.x <= half_width &&
							editor->terrain_brush_tool_position.z >= -half_height && editor->terrain_brush_tool_position.z <= half_height) {
						editor->terrain_brush_tool_active = true;
					}
				}
				if (editor->terrain_brush_tool_active && ImGui::IsMouseDown(0) && !ImGui::GetIO().WantCaptureMouse) {
					if (editor->tool_type == tool_type_terrain_bump) {
						vec2 center = editor->terrain_brush_tool_position.xz();
						vec2 begin = center - vec2{editor->terrain_brush_tool_radius, editor->terrain_brush_tool_radius};
						vec2 end = center + vec2{editor->terrain_brush_tool_radius, editor->terrain_brush_tool_radius};

						vec2 center_uv = center / vec2{half_width, half_height};
						vec2 begin_uv = begin / vec2{half_width, half_height};
						vec2 end_uv = end / vec2{half_width, half_height};

						begin_uv.x = clamp(begin_uv.x, -1.0f, 1.0f);
						begin_uv.y = clamp(begin_uv.y, -1.0f, 1.0f);
						end_uv.x = clamp(end_uv.x, -1.0f, 1.0f);
						end_uv.y = clamp(end_uv.y, -1.0f, 1.0f);

						center_uv = (center_uv + 1.0f) * 0.5f;
						begin_uv = (begin_uv + 1.0f) * 0.5f;
						end_uv = (end_uv + 1.0f) * 0.5f;

						uint32 height_map_width = terrain->width * terrain->sample_per_meter;
						uint32 height_map_height = terrain->height * terrain->sample_per_meter;
							
						uint32 begin_x = (uint32)(height_map_width * begin_uv.x);
						uint32 begin_y = (uint32)(height_map_height * begin_uv.y);

						uint32 end_x = (uint32)(height_map_width * end_uv.x);
						uint32 end_y = (uint32)(height_map_height * end_uv.y);
						
						vec2 uv = begin_uv;
						float duv_x = 1.0f / height_map_width;
						float duv_y = 1.0f / height_map_height;

						float radius_uv = editor->terrain_brush_tool_radius / terrain->width;

						for (uint32 i = begin_y; i <= end_y; i += 1) {
							for (uint32 j = begin_x; j <= end_x; j += 1) {
								float distance_uv = vec2_len(uv - center_uv);
								if (distance_uv <= radius_uv) {
									float variance = 1.0f;
									float x2 = (distance_uv / radius_uv) * (distance_uv / radius_uv) * 8;
									float fade_factor = expf(-x2 / 2 / variance) / sqrtf(2 * (float)M_PI * variance);
									terrain->height_map[height_map_width * i + j] += (int16)(fade_factor * last_frame_time_secs * terrain->max_height * 10 * editor->terrain_brush_tool_speed);
								}
								uv.x += duv_x;
							}
							uv.x = begin_uv.x;
							uv.y += duv_y;
						}

						uint32 height_map_size = terrain->width * terrain->sample_per_meter * terrain->height * terrain->sample_per_meter * sizeof(int16);
						update_vulkan_images(vulkan, terrain->height_map_image_index, (uint8 *)terrain->height_map, height_map_size);
					}
				}
			}
		}
		ImGui::Render();

		auto extra_color_render_pass_commands = [&] {
			VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];
			{ // collision shape
				auto render_collision_shape = [&](btCollisionShape *collision_shape, transform transform) {
					uint32 shape_count = 0;
					mat4 transform_mats[3];
					uint32 shape_vertex_counts[3];
					uint32 shape_vertex_buffer_offsets[3];
					int32 shape_type = collision_shape->getShapeType();
					if (shape_type == SPHERE_SHAPE_PROXYTYPE) {
						float radius = ((btSphereShape *)collision_shape)->getRadius();
						shape_count = 1;
						transform_mats[0] = mat4_from_transform(transform) * mat4_from_scale(radius);
						shape_vertex_counts[0] = m_countof(sphere_vertices);
						shape_vertex_buffer_offsets[0] = level->persistant_data.sphere_vertex_buffer_offset / sizeof(vec3);
					}
					else if (shape_type == BOX_SHAPE_PROXYTYPE) {
						btVector3 half_extents = ((btBoxShape *)collision_shape)->getHalfExtentsWithMargin();
						btVector3 extents = half_extents * 2;
						shape_count = 1;
						transform_mats[0] = mat4_from_transform(transform) * mat4_from_scale(vec3{m_unpack3(extents)});
						shape_vertex_counts[0] = m_countof(box_vertices);
						shape_vertex_buffer_offsets[0] = level->persistant_data.box_vertex_buffer_offset / sizeof(vec3);
					}
					else if (shape_type == CAPSULE_SHAPE_PROXYTYPE) {
						int32 up_axis = ((btCapsuleShape *)collision_shape)->getUpAxis();
						float radius = ((btCapsuleShape *)collision_shape)->getRadius();
						float half_height = ((btCapsuleShape *)collision_shape)->getHalfHeight();
						if (up_axis == 0) {
							transform.rotate = transform.rotate * quat_from_axis_rotate({0, 0, 1}, (float)M_PI / 2);
						}
						else if (up_axis == 2) {
							transform.rotate = transform.rotate * quat_from_axis_rotate({1, 0, 0}, (float)M_PI / 2);
						}
						shape_count = 3;
						transform_mats[0] = mat4_from_transform(transform) * mat4_from_scale(vec3{radius, half_height, radius});
						shape_vertex_counts[0] = m_countof(hollow_cylinder_vertices);
						shape_vertex_buffer_offsets[0] = level->persistant_data.hollow_cylinder_vertex_buffer_offset / sizeof(vec3);

						transform_mats[1] = mat4_from_transform(transform) * mat4_from_translate({0, half_height, 0}) * mat4_from_scale(radius);
						shape_vertex_counts[1] = m_countof(hemisphere_vertices);
						shape_vertex_buffer_offsets[1] = level->persistant_data.hemisphere_vertex_buffer_offset / sizeof(vec3);

						transform_mats[2] = mat4_from_transform(transform) * mat4_from_translate({0, -half_height, 0}) * mat4_from_axis_rotate({1, 0, 0}, (float)M_PI) * mat4_from_scale(radius);
						shape_vertex_counts[2] = m_countof(hemisphere_vertices);
						shape_vertex_buffer_offsets[2] = level->persistant_data.hemisphere_vertex_buffer_offset / sizeof(vec3);
					}
					else if (shape_type == CYLINDER_SHAPE_PROXYTYPE) {
						int32 up_axis = ((btCylinderShape *)collision_shape)->getUpAxis();
						float radius = 0;
						float half_height = 0;
						if (up_axis == 0) {
							transform.rotate = transform.rotate * quat_from_axis_rotate({0, 0, 1}, (float)M_PI / 2);
							radius = ((btCylinderShape *)collision_shape)->getHalfExtentsWithMargin().y();
							half_height = ((btCylinderShape *)collision_shape)->getHalfExtentsWithMargin().x();
						}
						else if (up_axis == 1) {
							radius = ((btCylinderShape *)collision_shape)->getHalfExtentsWithMargin().x();
							half_height = ((btCylinderShape *)collision_shape)->getHalfExtentsWithMargin().y();
						}
						else if (up_axis == 2) {
							transform.rotate = transform.rotate * quat_from_axis_rotate({1, 0, 0}, (float)M_PI / 2);
							radius = ((btCylinderShape *)collision_shape)->getHalfExtentsWithMargin().x();
							half_height = ((btCylinderShape *)collision_shape)->getHalfExtentsWithMargin().z();
						}
						shape_count = 1;
						transform_mats[0] = mat4_from_transform(transform) * mat4_from_scale(vec3{radius, half_height, radius});
						shape_vertex_counts[0] = m_countof(cylinder_vertices);
						shape_vertex_buffer_offsets[0] = level->persistant_data.cylinder_vertex_buffer_offset / sizeof(vec3);
					}
					else {
						m_assert(false, "");
					}
					if (shape_count > 0) {
						vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.collision_shape_pipeline);
						VkDeviceSize vertices_offset = 0;
						vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.vertex_buffer.buffer, &vertices_offset);
						for (uint32 i = 0; i < shape_count; i += 1) {
							shader_collision_shape_push_constant pc = {transform_mats[i], {0, 0.8f, 0, 0.2f}};
							vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
							vkCmdDraw(cmd_buffer, shape_vertex_counts[i], 1, shape_vertex_buffer_offsets[i], 0);
						}
					}
				};
				if (level->player.collision_shape) {
					transform transform = level->player.transform;
					transform.scale = {1, 1, 1};
					render_collision_shape(level->player.collision_shape, transform);
				}
				if (editor->generic_entity_index < level->generic_entity_count) {
					generic_entity *entity = &level->generic_entities[editor->generic_entity_index];
					if (entity->collision_shape) {
						transform transform = entity->transform;
						transform.scale = {1, 1, 1};
						render_collision_shape(entity->collision_shape, transform);
					}
				}
			}
			{ // terrain brush
				if (editor->terrain_brush_tool_active) {
					editor->terrain_brush_tool_active = false;
					m_debug_assert(level->terrain_index < level->terrain_count, "");
					terrain *terrain = &level->terrains[level->terrain_index];
					vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.terrain_brush_pipeline);
					VkDeviceSize offset = 0;
					vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.vertex_buffer.buffer, &offset);
					shader_terrain_brush_push_constant pc = {};
					pc.transform_mat = mat4_from_translate(editor->terrain_brush_tool_position) * mat4_from_scale(vec3{editor->terrain_brush_tool_radius, 1, editor->terrain_brush_tool_radius});
					pc.color = {0.8f, 0, 0, 1};
					pc.height_map_index = terrain->height_map_descriptor_index;
					pc.width = terrain->width;
					pc.height = terrain->height;
					pc.max_height = terrain->max_height;
					vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
					vkCmdDraw(cmd_buffer, m_countof(hollow_circle_vertices), 1, level->persistant_data.hollow_circle_vertex_buffer_offset / sizeof(vec3), 0);
				}
			}
		};
		auto extra_swap_chain_render_pass_commands = [&] {
			VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];
			ImDrawData *imgui_draw_data = ImGui::GetDrawData();
			for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
				if (i == 0) {
					vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.imgui_pipeline);
					VkDeviceSize vertices_offset = 0;
					vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.frame_vertex_buffers[vulkan->frame_index].buffer, &vertices_offset);
					vkCmdBindIndexBuffer(cmd_buffer, vulkan->memory_regions.frame_vertex_buffers[vulkan->frame_index].buffer, vertices_offset, VK_INDEX_TYPE_UINT16);
				}
				ImDrawList *dlist = imgui_draw_data->CmdLists[i];
				uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
				uint32 elements_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
				uint32 vertices_offset = append_vulkan_frame_vertex_buffer(vulkan, dlist->VtxBuffer.Data, vertices_size, sizeof(ImDrawVert));
				uint32 elements_offset = append_vulkan_frame_vertex_buffer(vulkan, dlist->IdxBuffer.Data, elements_size, sizeof(ImDrawVert));
				uint32 vertex_index = vertices_offset / sizeof(ImDrawVert);
				uint32 element_index = elements_offset / sizeof(ImDrawIdx);
				for (int32 i = 0; i < dlist->CmdBuffer.Size; i += 1) {
					ImDrawCmd *dcmd = &dlist->CmdBuffer.Data[i];
					vec2 clip_rect[2] = {{dcmd->ClipRect.x, dcmd->ClipRect.y}, {dcmd->ClipRect.z, dcmd->ClipRect.w}};
					for (auto &point : clip_rect) {
						float px = point.x / ImGui::GetIO().DisplaySize.x;
						float py = point.y / ImGui::GetIO().DisplaySize.y;
						px = vulkan->swap_chain_framebuffer_region.x + vulkan->swap_chain_framebuffer_region.z * px;
						py = vulkan->swap_chain_framebuffer_region.y + vulkan->swap_chain_framebuffer_region.w * py;
						point.x = vulkan->swap_chain.width * px;
						point.y = vulkan->swap_chain.height * py;
					}
					VkRect2D scissor = {{(int32)clip_rect[0].x, (int32)clip_rect[0].y}, {(uint32)(clip_rect[1].x - clip_rect[0].x), (uint32)(clip_rect[1].y - clip_rect[0].y)}};
					vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
					shader_imgui_push_constant pc = {};
					pc.swap_chain_framebuffer_region = vulkan->swap_chain_framebuffer_region;
					pc.swap_chain_width = vulkan->swap_chain.width;
					pc.swap_chain_height = vulkan->swap_chain.height;
					pc.framebuffer_width = vulkan->framebuffers.color_framebuffer_width;
					pc.framebuffer_height = vulkan->framebuffers.color_framebuffer_height;
					pc.texture_index = (uint32)(intptr_t)dcmd->TextureId;
					vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
					vkCmdDrawIndexed(cmd_buffer, dcmd->ElemCount, 1, element_index, vertex_index, 0);
					element_index += dcmd->ElemCount;
				}
			}
		};

		vulkan_begin_render(vulkan);
		generate_level_render_data(level, vulkan, editor->camera, [](){});
		generate_level_render_commands(level, vulkan, editor->camera, extra_color_render_pass_commands, extra_swap_chain_render_pass_commands);
		bool screen_shot = ImGui::IsKeyReleased(VK_SNAPSHOT);
		vulkan_frame_uniform_buffer_size = vulkan->memory_regions.frame_uniform_buffers[vulkan->frame_index].buffer_size;
		vulkan_frame_vertex_buffer_size = vulkan->memory_regions.frame_vertex_buffers[vulkan->frame_index].buffer_size;
		vulkan_end_render(vulkan, screen_shot);

		level_frame_memory_arena_size = level->frame_memory_arena.size;
		level->frame_memory_arena.size = 0;
	
		stop_timer(&timer);
		last_frame_time_microsecs = get_timer_duration_microsecs(timer);
		last_frame_time_millisecs = last_frame_time_microsecs / 1000.0;
		last_frame_time_secs = last_frame_time_microsecs / 1000000.0;
	}
	ImGui::DestroyContext(editor->imgui_context);
}

