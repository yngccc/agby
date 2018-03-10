/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"

#define RAPIDJSON_SSE2
#define RAPIDJSON_ASSERT(x) m_assert(x)
#include "../vendor/include/rapidjson/document.h"
#include "../vendor/include/rapidjson/prettywriter.h"
#include "../vendor/include/rapidjson/error/en.h"

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "../vendor/include/imgui/imgui_draw.cpp"
#include "../vendor/include/imgui/imgui.cpp"

#include "math.cpp"
#include "vulkan.cpp"
#include "assets.cpp"
#include "level.cpp"

struct game {
	camera player_camera;
	float player_camera_r;
	float player_camera_theta;
	float player_camera_phi;
};

bool initialize_game(game *game, vulkan *vulkan) {
	{ // imgui
		ImGuiIO *imgui_io = &ImGui::GetIO();
		imgui_io->KeyMap[ImGuiKey_Tab] = keycode_tab;
		imgui_io->KeyMap[ImGuiKey_LeftArrow] = keycode_left;
		imgui_io->KeyMap[ImGuiKey_RightArrow] = keycode_right;
		imgui_io->KeyMap[ImGuiKey_UpArrow] = keycode_up;
		imgui_io->KeyMap[ImGuiKey_DownArrow] = keycode_down;
		imgui_io->KeyMap[ImGuiKey_PageUp] = keycode_page_up;
		imgui_io->KeyMap[ImGuiKey_PageDown] = keycode_page_down;
		imgui_io->KeyMap[ImGuiKey_Home] = keycode_home;
		imgui_io->KeyMap[ImGuiKey_End] = keycode_end;
		imgui_io->KeyMap[ImGuiKey_Backspace] = keycode_backspace;
		imgui_io->KeyMap[ImGuiKey_Enter] = keycode_return;
		imgui_io->KeyMap[ImGuiKey_Escape] = keycode_esc;
		imgui_io->KeyMap[ImGuiKey_A] = 'A';
		imgui_io->KeyMap[ImGuiKey_C] = 'C';
		imgui_io->KeyMap[ImGuiKey_V] = 'V';
		imgui_io->KeyMap[ImGuiKey_X] = 'X';
		imgui_io->KeyMap[ImGuiKey_Y] = 'Y';
		imgui_io->KeyMap[ImGuiKey_Z] = 'Z';
		imgui_io->DisplaySize = {(float)vulkan->swap_chain.image_width, (float)vulkan->swap_chain.image_height};
		imgui_io->MousePos = {-1, -1};

		uint8* font_atlas_image = nullptr;
		int32 font_atlas_image_width = 0;
		int32 font_atlas_image_height = 0;
		ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&font_atlas_image, &font_atlas_image_width, &font_atlas_image_height);
		ImGui::GetIO().Fonts->ClearTexData();
	}
	{ // player
		game->player_camera_r = 10;
		game->player_camera_theta = 0.5f;
		game->player_camera_phi = 0;
	}
	return true;
}

int WinMain(HINSTANCE instance_handle, HINSTANCE prev_instance_handle, LPSTR cmd_line_str, int cmd_show) {
	set_exe_dir_as_current();
	show_command_prompt();

	memory_arena general_memory_arena = {};
	general_memory_arena.name = "general";
	general_memory_arena.capacity = m_megabytes(64);
	general_memory_arena.memory = allocate_virtual_memory(general_memory_arena.capacity);
	m_assert(general_memory_arena.memory);
	
	window window = {};
	m_assert(initialize_window(&window));
	set_window_fullscreen(&window, true);

	vulkan *vulkan = {};
	vulkan = memory_arena_allocate<struct vulkan>(&general_memory_arena, 1);
	initialize_vulkan(vulkan, window);

	game *game = {};
	game = memory_arena_allocate<struct game>(&general_memory_arena, 1);
	initialize_game(game, vulkan);

	level *level = {};
	level = memory_arena_allocate<struct level>(&general_memory_arena, 1);
	initialize_level(level, vulkan);
	auto extra_level_load = [](rapidjson::Document *json_doc) {};
	level_read_json(level, vulkan, "agby_assets\\levels\\level_save.json", extra_level_load);
	game->player_camera = level_get_player_camera(level, vulkan, game->player_camera_r, game->player_camera_theta, game->player_camera_phi);

	btDiscreteDynamicsWorld *bt_world = nullptr;
	{
		auto *bt_collision_config = new btDefaultCollisionConfiguration();
		auto *bt_dispatcher = new btCollisionDispatcher(bt_collision_config);
		auto *bt_overlapping_pair_cache = new btDbvtBroadphase();
		auto *bt_solver = new btSequentialImpulseConstraintSolver();
		bt_world = new btDiscreteDynamicsWorld(bt_dispatcher, bt_overlapping_pair_cache, bt_solver, bt_collision_config);
		bt_world->setGravity({0, -9, 0});

		for (uint32 i = 0; i < level->collision_component_count; i += 1) {
			if (level->collision_components[i].bt_collision_object) {
				bt_world->addCollisionObject(level->collision_components[i].bt_collision_object);
				console("friction %f\n", level->collision_components[i].bt_collision_object->getFriction());
			}
		}
		for (uint32 i = 0; i < level->physics_component_count; i += 1) {
			if (level->physics_components[i].bt_rigid_body) {
				bt_world->addRigidBody(level->physics_components[i].bt_rigid_body);
				console("damping %f\n", level->physics_components[i].bt_rigid_body->getLinearDamping());
			}
		}
	}

	LARGE_INTEGER performance_frequency = {};
	QueryPerformanceFrequency(&performance_frequency);
	LARGE_INTEGER performance_counters[2] = {};
	uint64 last_frame_time_microsec = 0;
	double last_frame_time_sec = 0;
	bool program_running = true;

	show_window(&window);
	pin_cursor(true);
	show_cursor(false);

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
		if (ImGui::GetIO().KeyAlt && ImGui::IsKeyDown(keycode_f4)) {
			program_running = false;
		}
		{
			entity_physics_component *physics_component = entity_get_physics_component(level, level->player_entity_index);
			vec3 camera_vec = vec3_normalize(vec3{game->player_camera.view.x, 0, game->player_camera.view.z});
			vec3 move_vec = {0, 0, 0};
			if (ImGui::IsKeyDown('W')) {
				move_vec += camera_vec;
			}
			if (ImGui::IsKeyDown('S')) {
				move_vec -= camera_vec;
			}
			if (ImGui::IsKeyDown('A')) {
				move_vec += vec3{camera_vec.z, 0, -camera_vec.x};
			}
			if (ImGui::IsKeyDown('D')) {
				move_vec -= vec3{camera_vec.z, 0, -camera_vec.x};
			}
			if (move_vec != vec3{0, 0, 0}) {
				move_vec = vec3_normalize(move_vec) * 2;
				btVector3 translate = physics_component->bt_rigid_body->getCenterOfMassTransform().getOrigin();
				quat rotate = quat_from_between(vec3{0, 0, 1}, vec3_normalize(move_vec));
				physics_component->velocity = move_vec;
				physics_component->bt_rigid_body->setCenterOfMassTransform(btTransform(btQuaternion(rotate.x, rotate.y, rotate.z, rotate.w), translate));
				physics_component->bt_rigid_body->setLinearVelocity(btVector3(move_vec.x, move_vec.y, move_vec.z));
			}
		}
		{
			bt_world->stepSimulation((float)last_frame_time_sec);
			uint32 physics_component_index = 0;
			for (uint32 i = 0; i < level->entity_count; i += 1) {
				if (level->entity_flags[i] & entity_component_flag_physics) {
					transform *entity_transform = &level->entity_transforms[i];
					entity_physics_component *physics_component = &level->physics_components[physics_component_index++];
					const btTransform &transform = physics_component->bt_rigid_body->getCenterOfMassTransform();
					const btVector3 &velocity = physics_component->bt_rigid_body->getLinearVelocity();
					btQuaternion rotate = transform.getRotation();
					btVector3 translate = transform.getOrigin();
					entity_transform->rotate = {rotate.x(), rotate.y(), rotate.z(), rotate.w()};
					entity_transform->translate = {translate.x(), translate.y(), translate.z()};
					physics_component->velocity = {velocity.x(), velocity.y(), velocity.z()};
				}
			}
		}
		{
			float x_sensitivity = 0.005f;
			float y_sensitivity = 0.005f;
			game->player_camera_theta = clamp(game->player_camera_theta + window.raw_mouse_dy * y_sensitivity, -(float)M_PI / 3, (float)M_PI / 3);
			game->player_camera_phi = wrap_angle(game->player_camera_phi - window.raw_mouse_dx * x_sensitivity);
			game->player_camera = level_get_player_camera(level, vulkan, game->player_camera_r, game->player_camera_theta, game->player_camera_phi);
		}
		ImGui::EndFrame();
		
		vulkan_begin_render(vulkan);
		level_generate_render_data(level, vulkan, game->player_camera, []{});
		level_generate_render_commands(level, vulkan, game->player_camera, []{}, []{});
		vulkan_end_render(vulkan);

		level_process_entity_modifications_additions(level);
		level->frame_memory_arena.size = 0;

		QueryPerformanceCounter(&performance_counters[1]);
		last_frame_time_microsec = (performance_counters[1].QuadPart - performance_counters[0].QuadPart) * 1000000 / performance_frequency.QuadPart;
		last_frame_time_sec = (double)last_frame_time_microsec / 1000000;
	}
}

// { // start rendering thread
// 	struct render_thread_data {
// 		struct vulkan *vulkan;
// 		struct level *level;
// 		struct game *game;
// 	};
// 	render_thread_data *render_thread_data = memory_arena_allocate<struct render_thread_data>(&game->general_memory_arena, 1);
// 	*render_thread_data = {&vulkan, &level, &game};
// 	auto render_thread_func = [](void *data) -> DWORD {
// 		auto *render_thread_data = (struct render_thread_data *)data;
// 		struct vulkan *vulkan = render_thread_data->vulkan;
// 		struct level *level = render_thread_data->level;
// 		struct game *game = render_thread_data->game;
// 		while (true) {
// 			vulkan_begin_render(vulkan);
// 			level_generate_render_data(level, vulkan, game->camera, []{});
// 			level_generate_render_commands(level, vulkan, game->camera, []{}, []{});
// 			vulkan_end_render(vulkan, game->screen_shot);
// 		}
// 		return 0;
// 	};
// 	CreateThread(nullptr, 0, render_thread_func, render_thread_data, 0, nullptr);
// }
