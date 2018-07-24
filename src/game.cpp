/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "common.cpp"
#include "math.cpp"
#include "d3d11.cpp"
#include "world.cpp"

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include <imgui/imgui.cpp>
#include <imgui/imgui_draw.cpp>

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

	XMVECTOR camera_position;
	XMVECTOR camera_view;
	float camera_fovy;
	float camera_znear;
	float camera_zfar;
	float camera_pitch;
	float camera_yaw;
	float camera_distance;
	XMMATRIX camera_view_mat;
	XMMATRIX camera_proj_mat;
	XMMATRIX camera_view_proj_mat;
};

void initialize_game(game *game, d3d *d3d) {
	*game = {};
	
	initialize_timer(&game->timer);
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
		imgui_io.DisplaySize = {(float)d3d->swap_chain_desc.Width, (float)d3d->swap_chain_desc.Height};
		imgui_io.IniFilename = nullptr;
		imgui_io.MousePos = {-1, -1};

		uint8* font_atlas_image = nullptr;
		int32 font_atlas_image_width = 0;
		int32 font_atlas_image_height = 0;
		imgui_io.Fonts->GetTexDataAsRGBA32(&font_atlas_image, &font_atlas_image_width, &font_atlas_image_height);
		imgui_io.Fonts->ClearTexData();
	}
	{
		game->camera_fovy = XMConvertToRadians(40);
		game->camera_znear = 0.1f;
		game->camera_zfar = 10000.0f;
		game->camera_pitch = XMConvertToRadians(45);
		game->camera_distance = 10;
	}
}

void common_hotkeys(game *game, world *world, window *window) {
	if (ImGui::IsKeyPressed(VK_F11)) {
		set_window_fullscreen(window, !window->fullscreen);
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

	// vec3 feet_position = world->player.transform.translate + vec3{0, world->player.feet_translate + 0.1f, 0};
	// btVector3 ray_from(m_unpack3(feet_position));
	// btVector3 ray_end = ray_from - btVector3(0, 0.2f, 0);
	// btCollisionWorld::ClosestRayResultCallback ray_call_back(ray_from, ray_end);
	// world->bt_dynamics_world->rayTest(ray_from, ray_end, ray_call_back);
	// bool on_ground = ray_call_back.hasHit();
		
	// vec3 velocity = {0, 0, 0};
	// float speed = 1.0f;
	// if (ImGui::GetIO().KeyShift) {
	// 	speed = 10.0f;
	// }
	// bool key_down = false;
	// if (ImGui::IsKeyDown('W')) {
	// 	velocity += move_vec * speed;
	// 	key_down = true;
	// }
	// if (ImGui::IsKeyDown('S')) {
	// 	velocity += -move_vec * speed;
	// 	key_down = true;
	// }
	// if (ImGui::IsKeyDown('A')) {
	// 	velocity += vec3{move_vec.z, 0, -move_vec.x} * speed;
	// 	key_down = true;
	// }
	// if (ImGui::IsKeyDown('D')) {
	// 	velocity += vec3{-move_vec.z, 0, move_vec.x} * speed;
	// 	key_down = true;
	// }
	// if (on_ground && key_down) {
	// 	world->player.bt_rigid_body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	// 	if (world->camera_type == camera_type_player_third) {
	// 		quat yaw = quat_from_between(vec3{0, 0, 1}, vec3_normalize(velocity));
	// 		world->player.bt_rigid_body->getWorldTransform().setRotation(btQuaternion(m_unpack4(yaw)));
	// 	}
	// }
}

void physics_step(game *game, world *world) {
	world->bt_dynamics_world->stepSimulation((float)game->last_frame_time_secs);

	if (world->player.bt_rigid_body) {
		btVector3 t = world->player.bt_rigid_body->getWorldTransform().getOrigin();
		btQuaternion q = world->player.bt_rigid_body->getWorldTransform().getRotation();
		world->player.transform.translate = {t.x(), t.y(), t.z()};
		world->player.transform.rotate = {q.x(), q.y(), q.z(), q.w()};
	}
	for (uint32 i = 0; i < world->dynamic_object_count; i += 1) {
		dynamic_object *dynamic_object = &world->dynamic_objects[i];
		if (dynamic_object->bt_rigid_body) {
			btVector3 t = dynamic_object->bt_rigid_body->getWorldTransform().getOrigin();
			btQuaternion q = dynamic_object->bt_rigid_body->getWorldTransform().getRotation();
			dynamic_object->transform.translate = {t.x(), t.y(), t.z()};
			dynamic_object->transform.rotate = {q.x(), q.y(), q.z(), q.w()};
		}
	}
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

		quat pitch_quat = quat_from_axis_rotate(vec3{1, 0, 0}, game->camera_pitch);
		quat yaw_quat = quat_from_axis_rotate(vec3{0, 1, 0}, game->camera_yaw);
		vec3 translate = yaw_quat * pitch_quat * vec3{0, 0, -game->camera_distance};
		vec3 camera_position = world->player.transform.translate + translate;
		vec3 camera_view = vec3_normalize(-translate);

		game->camera_position = XMVectorSet(m_unpack3(camera_position), 0);
		game->camera_view = XMVectorSet(m_unpack3(camera_view), 0);
		game->camera_view_mat = XMMatrixLookAtRH(game->camera_position, XMVectorAdd(game->camera_position, game->camera_view), XMVectorSet(0, 1, 0, 0));
		game->camera_proj_mat = XMMatrixPerspectiveFovRH(game->camera_fovy, (float)window->width / (float)window->height, game->camera_zfar, game->camera_znear);
		game->camera_view_proj_mat = XMMatrixMultiply(game->camera_view_mat, game->camera_proj_mat);
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
	initialize_window(window, handle_window_message);
	// set_window_fullscreen(window, true);

	d3d *d3d = new struct d3d;
	initialize_d3d(d3d, window);

	game *game = new struct game;
	initialize_game(game, d3d);

	world *world = new struct world;
	initialize_world(world, d3d);
	if (argc > 1) {
		const char *world_file = argv[1];
		m_assert(load_world(world, d3d, world_file, nullptr), "");
	}
	
	window_message_channel.window = window;
	window_message_channel.d3d = d3d;
	window_message_channel.initialized = true;
	show_window(window);
	pin_cursor(true);
	show_cursor(false);

	while (!game->quit) {
		start_timer(&game->timer);

		window->raw_mouse_dx = 0;
		window->raw_mouse_dy = 0;
		handle_window_messages(window);

		ImGui::NewFrame();
 		common_hotkeys(game, world, window);
		player_input(game, world, d3d, window);
		physics_step(game, world);
		world->player.animation_time += game->last_frame_time_secs;
		update_camera(game, world, window);
		ImGui::EndFrame();

		render_world_desc render_world_desc = {};
		render_world_desc.camera_view_proj_mat = game->camera_view_proj_mat;
		render_world_desc.camera_position = game->camera_position;
		render_world_desc.camera_view = game->camera_view;
		render_world_desc.render_models = true;
		render_world_desc.render_terrain = true;
		render_world_desc.render_skybox = true;
		render_world(world, d3d, &render_world_desc);

		world->frame_memory_arena.size = 0;
		
		stop_timer(&game->timer);
		game->last_frame_time_secs = get_timer_duration_secs(game->timer);
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

