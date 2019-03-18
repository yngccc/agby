/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "common.cpp"
#include "math.cpp"
#include "d3d.cpp"
#include "world.cpp"

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include <imgui/imgui.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_widgets.cpp>

struct {
	bool initialized;
	bool quit;
	window *window;
	d3d *d3d;
} window_message_channel = {};

LRESULT window_message_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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
			int window_width = LOWORD(lparam);
			int window_height = HIWORD(lparam);
			window_set_title(window, "%dx%d", window_width, window_height);
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
			window->mouse_x = LOWORD(lparam);
			window->mouse_y = HIWORD(lparam);
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

struct game {
	bool quit;

	timer timer;
	double last_frame_time_secs;

	ImGuiContext *imgui_context;

	vec3 camera_position;
	vec3 camera_view;
	float camera_fovy;
	float camera_znear;
	float camera_zfar;
	float camera_pitch;
	float camera_yaw;
	float camera_distance;
	mat4 camera_view_mat;
	mat4 camera_proj_mat;
	mat4 camera_view_proj_mat;
};

void initialize_game(game *game, d3d *d3d) {
	*game = {};

	timer_init(&game->timer);
	{
		game->imgui_context = ImGui::CreateContext();
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
		imgui_io.DisplaySize = { (float)d3d->swap_chain_desc.Width, (float)d3d->swap_chain_desc.Height };
		imgui_io.IniFilename = nullptr;
		imgui_io.MousePos = { -1, -1 };

		uint8* font_atlas_image = nullptr;
		int32 font_atlas_image_width = 0;
		int32 font_atlas_image_height = 0;
		imgui_io.Fonts->GetTexDataAsRGBA32(&font_atlas_image, &font_atlas_image_width, &font_atlas_image_height);
		imgui_io.Fonts->ClearTexData();
	}
	{
		game->camera_fovy = degree_to_radian(40);
		game->camera_znear = 0.1f;
		game->camera_zfar = 10000.0f;
		game->camera_pitch = degree_to_radian(45);
		game->camera_distance = 10;
	}
}

void common_hotkeys(game *game, world *world, window *window) {
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
	if (ImGui::IsKeyPressed(VK_F4) && ImGui::IsKeyDown(VK_MENU)) {
		game->quit = true;
	}
}

void player_input(game *game, world *world, d3d *d3d, window *window) {
	// if (ImGui::IsKeyPressed(VK_TAB)) {
	// 	world->camera_type = (world->camera_type == camera_type_player_first) ? camera_type_player_third : camera_type_player_first;
	// }

	// vec3 move_vec;
	// if (world->camera_type == camera_type_player_first) {
	// 	float x_sensitivity = 0.003f;
	// 	float y_sensitivity = 0.003f;

	// 	world->player_first_person_camera_pitch += window->raw_mouse_dy * y_sensitivity;
	// 	world->player_first_person_camera_pitch = clamp(world->player_first_person_camera_pitch, -(float)M_PI / 3, (float)M_PI / 3);

	// 	world->player_first_person_camera_yaw -= window->raw_mouse_dx * x_sensitivity;
	// 	world->player_first_person_camera_yaw = wrap_angle(world->player_first_person_camera_yaw);

	// 	quat yaw = quat_from_axis_rotate({0, 1, 0}, world->player_first_person_camera_yaw);
	// 	world->player.bt_rigid_body->getWorldTransform().setRotation(btQuaternion(m_unpack4(yaw)));

	// 	move_vec = vec3_normalize(yaw * vec3{0, 0, 1});
	// }
	// else if (world->camera_type == camera_type_player_third) {
	// 	float x_sensitivity = 0.005f;
	// 	float y_sensitivity = 0.005f;

	// 	world->player_third_person_camera_pitch += window->raw_mouse_dy * y_sensitivity;
	// 	world->player_third_person_camera_pitch = clamp(world->player_third_person_camera_pitch, -(float)M_PI / 3, (float)M_PI / 3);

	// 	world->player_third_person_camera_yaw -= window->raw_mouse_dx * x_sensitivity;
	// 	world->player_third_person_camera_yaw = wrap_angle(world->player_third_person_camera_yaw);
	// 	world->player_third_person_camera_distance = clamp(world->player_third_person_camera_distance - ImGui::GetIO().MouseWheel, 4.0f, 30.0f);

	// 	quat yaw = quat_from_axis_rotate(vec3{0, 1, 0}, world->player_third_person_camera_yaw);
	// 	move_vec = vec3_normalize(yaw * vec3{0, 0, 1});
	// }
	{
		vec3 player_displacement = vec3{ 0, 0, 0 };
		float player_speed = 1.0f;
		if (ImGui::GetIO().KeyShift) {
			player_speed = 5.0f;
		}
		if (ImGui::IsKeyDown('W')) {
			player_displacement += vec3{ 0, 0, 1 } *player_speed;
		}
		else if (ImGui::IsKeyDown('S')) {
			player_displacement += vec3{ 0, 0, -1 } *player_speed;
		}
		else if (ImGui::IsKeyDown('A')) {
			player_displacement += vec3{ 1, 0, 0 } *player_speed;
		}
		else if (ImGui::IsKeyDown('D')) {
			player_displacement += vec3{ -1, 0, 1 } *player_speed;
		}
		world->player.physx_controller->move(physx::PxVec3(m_unpack3(player_displacement)), 0, (float)game->last_frame_time_secs, physx::PxControllerFilters());
		physx::PxExtendedVec3 player_position = world->player.physx_controller->getPosition();
		world->player.transform.translate = vec3{ (float)player_position.x, (float)player_position.y, (float)player_position.z };
	}
}

void simulate_physics(game *game, world *world) {
	static double accumulate = 0;
	static double time_step = 1.0f / 60.0f;

	accumulate += game->last_frame_time_secs;
	while (accumulate >= time_step) {
		accumulate -= time_step;
		world->px_scene->simulate((float)time_step);
		bool simulate_result = world->px_scene->fetchResults(true);
		m_assert(simulate_result);
	}

	world->player.physx_controller->move(physx::PxVec3(0.001f, 0, 0), 0, (float)game->last_frame_time_secs, physx::PxControllerFilters());
	physx::PxExtendedVec3 player_position = world->player.physx_controller->getPosition();
	world->player.transform.translate = vec3{ (float)player_position.x, (float)player_position.y, (float)player_position.z };
}

void update_camera(game *game, world *world, window *window) {
	{
		float x_sensitivity = 0.005f;
		float y_sensitivity = 0.005f;

		game->camera_pitch += window->raw_mouse_dy * y_sensitivity;
		game->camera_pitch = clamp(game->camera_pitch, -(float)M_PI / 3, (float)M_PI / 3);

		game->camera_yaw -= window->raw_mouse_dx * x_sensitivity;
		game->camera_yaw = wrap_angle(game->camera_yaw);
		game->camera_distance = clamp(game->camera_distance - ImGui::GetIO().MouseWheel, 4.0f, 30.0f);

		quat pitch_quat = quat_from_axis_rotate(vec3{ 1, 0, 0 }, game->camera_pitch);
		quat yaw_quat = quat_from_axis_rotate(vec3{ 0, 1, 0 }, game->camera_yaw);
		vec3 translate = yaw_quat * pitch_quat * vec3{ 0, 0, -game->camera_distance };
		vec3 camera_position = world->player.transform.translate + translate;
		vec3 camera_view = vec3_normalize(-translate);

		//XMVECTOR p = XMVectorSet(m_unpack3(camera_position), 0);
		//XMVECTOR v = XMVectorSet(m_unpack3(camera_view), 0);
		//game->camera_position = camera_position;
		//game->camera_view = camera_view;
		//game->camera_view_mat = XMMatrixLookAtRH(p, XMVectorAdd(p, v), XMVectorSet(0, 1, 0, 0));
		//game->camera_proj_mat = XMMatrixPerspectiveFovRH(game->camera_fovy, (float)window->width / (float)window->height, game->camera_zfar, game->camera_znear);
		//game->camera_view_proj_mat = XMMatrixMultiply(game->camera_view_mat, game->camera_proj_mat);
	}
	// if (world->camera_type == camera_type_player_first) {
	// 	quat pitch_quat = quat_from_axis_rotate(vec3{1, 0, 0}, world->player_first_person_camera_pitch);
	// 	quat yaw_quat = quat_from_axis_rotate(vec3{0, 1, 0}, world->player_first_person_camera_yaw);
	// 	vec3 facing = vec3_normalize(yaw_quat * vec3{0, 0, 1});
	// 	vec3 view = vec3_normalize(yaw_quat * pitch_quat * vec3{0, 0, 1});

	// 	world->player_first_person_camera.position = world->player.transform.translate + facing * 1.0f;
	// 	world->player_first_person_camera.view = view;
	// 	world->player_first_person_camera.aspect = (float)vulkan->framebuffers.color_framebuffer_width / (float)vulkan->framebuffers.color_framebuffer_height;
	// 	world->player_first_person_camera.fovy = hfov_to_vfov(degree_to_radian(80), world->player_first_person_camera.aspect);
	// 	world->player_first_person_camera.znear = 0.1f;
	// 	world->player_first_person_camera.zfar = 1000.0f;
	// }
}

int main(int argc, char **argv) {
	set_current_dir_to_exe_dir();

	window *window = new struct window;
	window_init(window, window_message_callback);
	// set_window_fullscreen(window, true);

	d3d *d3d = new struct d3d;
	initialize_d3d(d3d, window);

	d3d12 *d3d12 = new struct d3d12();
	d3d12_init(d3d12, window);

	game *game = new struct game;
	initialize_game(game, d3d);

	world *world = new struct world;
	world_init(world, d3d12);
	if (argc > 1) {
		const char *world_file = argv[1];
		m_assert(world_load_from_file(world, d3d12, world_file, nullptr));
	}

	window_message_channel.window = window;
	window_message_channel.d3d = d3d;
	window_message_channel.initialized = true;
	window_show(window);
	cursor_pin(true);
	cursor_show(false);

	while (!game->quit) {
		timer_start(&game->timer);

		window->raw_mouse_dx = 0;
		window->raw_mouse_dy = 0;
		window_handle_messages(window);

		ImGui::NewFrame();
		common_hotkeys(game, world, window);
		player_input(game, world, d3d, window);
		simulate_physics(game, world);
		world->player.animation_time += game->last_frame_time_secs;
		update_camera(game, world, window);
		ImGui::EndFrame();

		world->frame_memory_arena.size = 0;

		timer_stop(&game->timer);
		game->last_frame_time_secs = timer_get_duration(game->timer);
	}

	ImGui::DestroyContext(game->imgui_context);
}

// void player_physics_step(game *game, world *world) {
// 	vec3 player_position = world->player.transform.translate;
// 	if (world->player.model_index < world->model_count) {
// 		model *model = &world->models[world->player.model_index];
// 		if (model->collision.type == collision_type_sphere) {
// 			player_position = world->player.transform.translate - vec3{0, model->collision.sphere.radius, 0};
// 		}
// 		else if (model->collision.type == collision_type_box) {
// 			player_position = world->player.transform.translate - vec3{0, model->collision.box.extents.y * 0.5f, 0};
// 		}
// 	}
// 	if (world->terrain_index < world->terrain_count) {
// 		terrain *terrain = &world->terrains[world->terrain_index];
// 		float u = (player_position.x + terrain->width / 2) / terrain->width;
// 		float v = (player_position.z + terrain->height / 2) / terrain->height;
// 		if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
// 			uint32 x = (uint32)(u * terrain->width * terrain->sample_per_meter);
// 			uint32 y = (uint32)(v * terrain->height * terrain->sample_per_meter);
// 			int16 h = terrain->height_map[y * terrain->width * terrain->sample_per_meter + x];
// 			float height = ((float)h / (float)INT16_MAX) * terrain->max_height;
// 			double frame_time_remain = game->last_frame_time_secs;
// 			if (player_position.y > height) {
// 				double drop_distance = player_position.y - height;
// 				double drop_speed = 1;
// 				double drop_time = drop_distance / drop_speed;
// 				if (drop_time < game->last_frame_time_secs) {
// 					world->player.transform.translate.y -= (float)drop_distance;
// 					frame_time_remain = game->last_frame_time_secs - drop_time;
// 				}
// 				else {
// 					world->player.transform.translate.y -= (float)(game->last_frame_time_secs * drop_speed);
// 					frame_time_remain = 0;
// 				}
// 			}
// 			if (frame_time_remain > 0 && world->player.velocity != vec3{0, 0, 0}) {
// 				world->player.transform.rotate = quat_from_between(vec3{0, 0, 1}, vec3_normalize(world->player.velocity));

// 				float prev_height = height;
// 				double distance = vec3_len(world->player.velocity) * frame_time_remain;
// 				float epsilon = 0.01f;
// 				vec3 delta_velocity = vec3_normalize(world->player.velocity) * epsilon;
// 				for (;;) {
// 					player_position.x += delta_velocity.x;
// 					player_position.z += delta_velocity.z;
// 					float u = (player_position.x + terrain->width / 2) / terrain->width;
// 					float v = (player_position.z + terrain->height / 2) / terrain->height;
// 					if (u < 0 || u > 1 || v < 0 || v > 1) {
// 						break;
// 					}
// 					uint32 x = (uint32)(u * terrain->width * terrain->sample_per_meter);
// 					uint32 y = (uint32)(v * terrain->height * terrain->sample_per_meter);
// 					int16 h = terrain->height_map[y * terrain->width * terrain->sample_per_meter + x];
// 					float height = ((float)h / (float)INT16_MAX) * terrain->max_height;
// 					float delta_height = height - prev_height;
// 					float hypotenuse = sqrtf(epsilon * epsilon + delta_height * delta_height);
// 					world->player.transform.translate.x += delta_velocity.x;
// 					world->player.transform.translate.z += delta_velocity.z;
// 					world->player.transform.translate.y += delta_height;
// 					prev_height = height;
// 					distance -= hypotenuse;
// 					if (distance <= 0) {
// 						break;
// 					}
// 				}
// 			}
// 		}
// 	}
// }

