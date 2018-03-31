/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"
#include "math.cpp"
#include "vulkan.cpp"
#include "gpk.cpp"
#include "level.cpp"

#include <atomic>

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "../vendor/include/imgui/imgui_draw.cpp"
#include "../vendor/include/imgui/imgui.cpp"
#undef snprintf

struct render_thread_arg {
	struct vulkan *vulkan;
	struct level *level;
	struct game *game;
};

struct game {
	std::atomic<uint32> render_done_flag;
	std::atomic<uint32> frame_done_flag;
	
	camera player_camera;
	float player_camera_r;
	float player_camera_theta;
	float player_camera_phi;
};

void initialize_game(game *game, vulkan *vulkan) {
	{ // misc
		game->render_done_flag = 0;
		game->frame_done_flag = 0;
	}
	{ // imgui
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
		imgui_io.MousePos = {-1, -1};

		uint8* font_atlas_image = nullptr;
		int32 font_atlas_image_width = 0;
		int32 font_atlas_image_height = 0;
		imgui_io.Fonts->GetTexDataAsRGBA32(&font_atlas_image, &font_atlas_image_width, &font_atlas_image_height);
		imgui_io.Fonts->ClearTexData();
	}
	{ // player
		game->player_camera_r = 8;
		game->player_camera_theta = 0.5f;
		game->player_camera_phi = 0;
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

	game *game = allocate_memory<struct game>(&general_memory_arena, 1);
	initialize_game(game, vulkan);

	level *level = allocate_memory<struct level>(&general_memory_arena, 1);
	initialize_level(level, vulkan);
	level_read_json(level, vulkan, "agby_assets\\levels\\level_save.json", [](nlohmann::json &json){}, false);
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
			}
		}
		for (uint32 i = 0; i < level->physics_component_count; i += 1) {
			if (level->physics_components[i].bt_rigid_body) {
				bt_world->addRigidBody(level->physics_components[i].bt_rigid_body);
			}
		}

		entity_get_physics_component(level, level->player_entity_index)->bt_rigid_body->setActivationState(DISABLE_DEACTIVATION);
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

	{ // rendering thread
		render_thread_arg *render_thread_arg = allocate_memory<struct render_thread_arg>(&general_memory_arena, 1);
		*render_thread_arg = {vulkan, level, game};
		auto render_thread_func = [](void *arg) -> DWORD {
			struct render_thread_arg *render_thread_arg = (struct render_thread_arg *)arg;
			struct vulkan *vulkan = render_thread_arg->vulkan;
			struct level *level = render_thread_arg->level;
			struct game *game = render_thread_arg->game;
			while (true) {
				vulkan_begin_render(vulkan);
				level_generate_render_data(level, vulkan, game->player_camera, []{});
				level_generate_render_commands(level, vulkan, game->player_camera, []{}, []{});
				vulkan_end_render(vulkan, false);

				game->render_done_flag.store(1);
				uint32 frame_done_flag = 1;
				while (!game->frame_done_flag.compare_exchange_weak(frame_done_flag, 0)) {
					frame_done_flag = 1;
				}
			}
			return 0;
		};
		CreateThread(nullptr, 0, render_thread_func, render_thread_arg, 0, nullptr);
	}
	
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
		{ // camera movement
			float x_sensitivity = 0.005f;
			float y_sensitivity = 0.005f;
			game->player_camera_theta = clamp(game->player_camera_theta + window.raw_mouse_dy * y_sensitivity, -(float)M_PI / 3, (float)M_PI / 3);
			game->player_camera_phi = wrap_angle(game->player_camera_phi - window.raw_mouse_dx * x_sensitivity);
			game->player_camera_r = clamp(game->player_camera_r - ImGui::GetIO().MouseWheel, 4.0f, 30.0f);
		}
		{ // player movement
			entity_physics_component *physics_component = entity_get_physics_component(level, level->player_entity_index);
			btVector3 linear_velocity = physics_component->bt_rigid_body->getLinearVelocity();
			bool falling = fabsf(linear_velocity.y()) > 0.0001f;

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
			if (vec3_len(move_vec) > 0.1f) {
				quat rotate = quat_from_between(vec3{0, 0, 1}, vec3_normalize(move_vec));
				btVector3 translate = physics_component->bt_rigid_body->getCenterOfMassTransform().getOrigin();
				physics_component->bt_rigid_body->setCenterOfMassTransform(btTransform(btQuaternion(rotate.x, rotate.y, rotate.z, rotate.w), translate));
				if (!falling) {
					float move_speed = 2;
					vec3 velocity = vec3_normalize(move_vec) * move_speed;
					physics_component->bt_rigid_body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
				}
			}
			if (!falling && ImGui::IsKeyPressed(' ')) {
				physics_component->bt_rigid_body->setLinearVelocity(linear_velocity + btVector3(0, 5, 0));
			}
		}
		{ // physics simulation
			bt_world->stepSimulation((float)last_frame_time_sec);
			uint32 physics_component_index = 0;
			for (uint32 i = 0; i < level->entity_count; i += 1) {
				if (level->entity_flags[i] & entity_component_flag_physics) {
					transform *transform = &level->entity_transforms[i];
					entity_physics_component *physics_component = &level->physics_components[physics_component_index++];

					const btTransform &rigid_body_transform = physics_component->bt_rigid_body->getCenterOfMassTransform();
					const btVector3 &rigid_body_velocity = physics_component->bt_rigid_body->getLinearVelocity();
					btQuaternion rigid_body_rotate = rigid_body_transform.getRotation();
					btVector3 rigid_body_translate = rigid_body_transform.getOrigin();

					struct transform *new_transform = allocate_memory<struct transform>(&level->main_thread_frame_memory_arena, 1);
					struct entity_physics_component *new_physics_component = allocate_memory<struct entity_physics_component>(&level->main_thread_frame_memory_arena, 1);

					*new_transform = *transform;
					*new_physics_component = *physics_component;
					new_transform->rotate = {rigid_body_rotate.x(), rigid_body_rotate.y(), rigid_body_rotate.z(), rigid_body_rotate.w()};
					new_transform->translate = {rigid_body_translate.x(), rigid_body_translate.y(), rigid_body_translate.z()};
					new_physics_component->velocity = {rigid_body_velocity.x(), rigid_body_velocity.y(), rigid_body_velocity.z()};
					level->entity_modifications[i].transform = new_transform;
					level->entity_modifications[i].physics_component = new_physics_component;
				}
			}
		}
		{ // animations
			uint32 render_component_index = 0;
			for (uint32 i = 0; i < level->entity_count; i += 1) {
				if (level->entity_flags[i] & entity_component_flag_render) {
					entity_render_component *render_component = &level->render_components[render_component_index++];
					if (render_component->model_index < level->model_count &&  render_component->animation_index < level->models[render_component->model_index].animation_count) {
						entity_render_component *new_render_component = allocate_memory<struct entity_render_component>(&level->main_thread_frame_memory_arena, 1);
						*new_render_component = *render_component;
						new_render_component->animation_time += last_frame_time_sec;
						level->entity_modifications[i].render_component = new_render_component;
					}
				}
			}
		}
		ImGui::EndFrame();

		uint32 render_done_flag = 1;
		while (!game->render_done_flag.compare_exchange_weak(render_done_flag, 0)) {
			render_done_flag = 1;
		}
		
		level_update_entity_components(level);
		level->main_thread_frame_memory_arena.size = 0;
		level->render_thread_frame_memory_arena.size = 0;

		game->player_camera = level_get_player_camera(level, vulkan, game->player_camera_r, game->player_camera_theta, game->player_camera_phi);
		
		game->frame_done_flag.store(1);
		
		QueryPerformanceCounter(&performance_counters[1]);
		last_frame_time_microsec = (performance_counters[1].QuadPart - performance_counters[0].QuadPart) * 1000000 / performance_frequency.QuadPart;
		last_frame_time_sec = (double)last_frame_time_microsec / 1000000;
	}
}
