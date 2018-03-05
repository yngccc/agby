/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"

#define RAPIDJSON_SSE2
#define RAPIDJSON_ASSERT(x) m_assert(x)
#include "../vendor/include/rapidjson/document.h"
#include "../vendor/include/rapidjson/prettywriter.h"
#include "../vendor/include/rapidjson/error/en.h"

#include "../vendor/include/bullet3/btBulletCollisionCommon.h"
#include "../vendor/include/bullet3/btBulletDynamicsCommon.h"

#include "math.cpp"
#include "vulkan.cpp"
#include "assets.cpp"
#include "level.cpp"

struct game {
	bool key_shift;
	bool key_ctrl;
	bool key_alt;
	bool keys_down[512];
	float keys_down_duration[512];
	float keys_down_duration_prev[512];
	vec2 mouse_position;
	bool mouse_down[3];
	float mouse_wheel;

	camera player_camera;
	float player_camera_r;
	float player_camera_theta;
	float player_camera_phi;
};

bool initialize_game(game *game, vulkan *vulkan) {
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
	// set_window_fullscreen(&window, true);

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
						game->key_shift = (window.msg_type == window_message_type_key_down);
					}
					else if (window.keycode == keycode_ctrl) {
						game->key_ctrl = (window.msg_type == window_message_type_key_down);
					}
					else if (window.keycode == keycode_alt) {
						game->key_alt = (window.msg_type == window_message_type_key_down);
					}
					else {
						m_assert(window.keycode < m_countof(game->keys_down));
						game->keys_down[window.keycode] = (window.msg_type == window_message_type_key_down);
					}
				} break;
				case window_message_type_char: {
				} break;
				case window_message_type_mouse_move: {
				} break;
				case window_message_type_mouse_lb_down:
				case window_message_type_mouse_lb_up: {
					game->mouse_down[0] = (window.msg_type == window_message_type_mouse_lb_down);
				} break;
				case window_message_type_mouse_rb_down:
				case window_message_type_mouse_rb_up: {
					game->mouse_down[1] = (window.msg_type == window_message_type_mouse_lb_down);
				} break;
				case window_message_type_mouse_mb_down:
				case window_message_type_mouse_mb_up: {
					game->mouse_down[2] = (window.msg_type == window_message_type_mouse_lb_down);
				} break;
				case window_message_type_mouse_wheel: {
				} break;
			}
		}
			
		if (game->key_alt && game->keys_down[keycode_f4]) {
			program_running = false;
		}
		{
			vec3 camera_vec = vec3_normalize(vec3{game->player_camera.view.x, 0, game->player_camera.view.z});
			vec3 move_vec = {0, 0, 0};
			if (game->keys_down['W']) {
				move_vec += camera_vec;
			}
			if (game->keys_down['S']) {
				move_vec -= camera_vec;
			}
			if (game->keys_down['A']) {
				move_vec += vec3{camera_vec.z, 0, -camera_vec.x};
			}
			if (game->keys_down['D']) {
				move_vec -= vec3{camera_vec.z, 0, -camera_vec.x};
			}
			if (move_vec != vec3{0, 0, 0} && level->player_entity_index < level->entity_count) {
				float rotate_speed = 0;
				float move_speed = 2;
				move_vec = vec3_normalize(move_vec);
				transform *transform = &level->entity_transforms[level->player_entity_index];
				transform->rotate = quat_from_between(vec3{0, 0, 1}, move_vec);
				transform->translate += move_vec * (float)last_frame_time_sec * move_speed;
				// entity_physics_component *component = entity_get_physics_component(level, level->player_entity_index);
			}
		}
		{
			double step_time = 1.0 / 60.0;
			double sim_time_remain = last_frame_time_sec;
			while (sim_time_remain > step_time) {
				level_time_step_physics_components(level, step_time);
				sim_time_remain -= step_time;
			}
			level_time_step_physics_components(level, sim_time_remain);
		}
		{
			float x_sensitivity = 0.005f;
			float y_sensitivity = 0.005f;
			game->player_camera_theta = clamp(game->player_camera_theta + window.raw_mouse_dy * y_sensitivity, -(float)M_PI / 3, (float)M_PI / 3);
			game->player_camera_phi = wrap_angle(game->player_camera_phi - window.raw_mouse_dx * x_sensitivity);
			game->player_camera = level_get_player_camera(level, vulkan, game->player_camera_r, game->player_camera_theta, game->player_camera_phi);
		}
		
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
