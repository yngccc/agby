/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "level.cpp"

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "../vendor/include/imgui/imgui_draw.cpp"
#include "../vendor/include/imgui/imgui.cpp"

#include <atomic>

struct game {
	ImGuiContext *imgui_context;
	ImGuiIO *imgui_io;
	
	camera player_camera;
	float player_camera_r;
	float player_camera_theta;
	float player_camera_phi;
};

void initialize_game(game *game, vulkan *vulkan) {
	{ // imgui
		game->imgui_context = ImGui::CreateContext();
		game->imgui_io = &ImGui::GetIO();
		game->imgui_io->KeyMap[ImGuiKey_Tab] = VK_TAB;
		game->imgui_io->KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		game->imgui_io->KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		game->imgui_io->KeyMap[ImGuiKey_UpArrow] = VK_UP;
		game->imgui_io->KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		game->imgui_io->KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		game->imgui_io->KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		game->imgui_io->KeyMap[ImGuiKey_Home] = VK_HOME;
		game->imgui_io->KeyMap[ImGuiKey_End] = VK_END;
		game->imgui_io->KeyMap[ImGuiKey_Backspace] = VK_BACK;
		game->imgui_io->KeyMap[ImGuiKey_Enter] = VK_RETURN;
		game->imgui_io->KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		game->imgui_io->KeyMap[ImGuiKey_A] = 'A';
		game->imgui_io->KeyMap[ImGuiKey_C] = 'C';
		game->imgui_io->KeyMap[ImGuiKey_V] = 'V';
		game->imgui_io->KeyMap[ImGuiKey_X] = 'X';
		game->imgui_io->KeyMap[ImGuiKey_Y] = 'Y';
		game->imgui_io->KeyMap[ImGuiKey_Z] = 'Z';
		game->imgui_io->DisplaySize = {(float)vulkan->swap_chain.width, (float)vulkan->swap_chain.height};
		game->imgui_io->MousePos = {-1, -1};

		uint8* font_atlas_image = nullptr;
		int32 font_atlas_image_width = 0;
		int32 font_atlas_image_height = 0;
		game->imgui_io->Fonts->GetTexDataAsRGBA32(&font_atlas_image, &font_atlas_image_width, &font_atlas_image_height);
		game->imgui_io->Fonts->ClearTexData();
	}
	{ // player
		game->player_camera_r = 8;
		game->player_camera_theta = 0.5f;
		game->player_camera_phi = 0;
	}
}

struct {
	bool initialized;
	bool quit;
	window *window;
	game *game;
} window_message_channel = {};

LRESULT window_message_handle_func(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	LRESULT result = 0;
	if (!window_message_channel.initialized) {
		result = DefWindowProcA(hwnd, msg, wparam, lparam);
	}
	else {
		window *window = window_message_channel.window;
		ImGuiIO *imgui_io = window_message_channel.game->imgui_io;
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
			} break;
			case WM_SHOWWINDOW : {
			} break;
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN: 
			case WM_KEYUP:
			case WM_SYSKEYUP: {
				bool down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
				imgui_io->KeysDown[wparam] = down;
				if (wparam == VK_SHIFT) {
					imgui_io->KeyShift = down;
				}
				else if (wparam == VK_CONTROL) {
					imgui_io->KeyCtrl = down;
				}
				else if (wparam == VK_MENU) {
					imgui_io->KeyAlt = down;
				}
			} break;
			case WM_CHAR:
			case WM_SYSCHAR: {
				imgui_io->AddInputCharacter((ImWchar)wparam);
			} break;
			case WM_MOUSEMOVE: {
				window->mouse_x = LOWORD(lparam);
				window->mouse_y = HIWORD(lparam);
			} break;
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP: {
				imgui_io->MouseDown[0] = (msg == WM_LBUTTONDOWN);
			} break;
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP: {
				imgui_io->MouseDown[1] = (msg == WM_RBUTTONDOWN);
			} break;
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP: {
				imgui_io->MouseDown[2] = (msg == WM_MBUTTONDOWN);
			} break;
			case WM_MOUSEWHEEL: {
				imgui_io->MouseWheel = (float)WHEEL_DELTA / (float)GET_WHEEL_DELTA_WPARAM(wparam);
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

int main(int argc, char **argv) {
	set_exe_dir_as_current();

	memory_arena general_memory_arena = {};
	general_memory_arena.name = "general";
	general_memory_arena.capacity = m_megabytes(16);
	general_memory_arena.memory = allocate_virtual_memory(general_memory_arena.capacity);
	m_assert(general_memory_arena.memory, "");
	
	window *window = allocate_memory<struct window>(&general_memory_arena, 1);
	m_assert(initialize_window(window, window_message_handle_func), "");

	vulkan *vulkan = allocate_memory<struct vulkan>(&general_memory_arena, 1);
	initialize_vulkan(vulkan, window);

	game *game = allocate_memory<struct game>(&general_memory_arena, 1);
	initialize_game(game, vulkan);

	level *level = allocate_memory<struct level>(&general_memory_arena, 1);
	initialize_level(level, vulkan);
	level_read_json(level, vulkan, "assets/levels/level_save.json", [](nlohmann::json &json){}, false);
	game->player_camera = level_get_player_camera(level, vulkan, game->player_camera_r, game->player_camera_theta, game->player_camera_phi);

	btDiscreteDynamicsWorld *bt_world = nullptr;
	{
		auto *bt_collision_config = allocate_memory<btDefaultCollisionConfiguration>(&general_memory_arena, 1);
		auto *bt_dispatcher = allocate_memory<btCollisionDispatcher>(&general_memory_arena, 1);
		auto *bt_overlapping_pair_cache = allocate_memory<btDbvtBroadphase>(&general_memory_arena, 1);
		auto *bt_solver = allocate_memory<btSequentialImpulseConstraintSolver>(&general_memory_arena, 1);
		bt_world = allocate_memory<btDiscreteDynamicsWorld>(&general_memory_arena, 1);

		new(bt_collision_config) btDefaultCollisionConfiguration();
		new(bt_dispatcher) btCollisionDispatcher(bt_collision_config);
		new(bt_overlapping_pair_cache) btDbvtBroadphase();
		new(bt_solver) btSequentialImpulseConstraintSolver();
		new(bt_world) btDiscreteDynamicsWorld(bt_dispatcher, bt_overlapping_pair_cache, bt_solver, bt_collision_config);

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

	window_message_channel.window = window;
	window_message_channel.game = game;
	window_message_channel.initialized = true;
	show_window(window);
	pin_cursor(true);
	show_cursor(false);

	timer timer = {};
	initialize_timer(&timer);
	uint64 last_frame_time_microsec = 0;
	double last_frame_time_sec = 0;

	bool quit_game = false;
	
	while (!quit_game) {
		start_timer(&timer);

		window->raw_mouse_dx = 0;
		window->raw_mouse_dy = 0;
		handle_window_messages(window);

		game->imgui_io->DeltaTime = (float)last_frame_time_sec;
		{ // mouse position
			float px = (float)window->mouse_x / vulkan->swap_chain.width;
			float py = (float)window->mouse_y / vulkan->swap_chain.height;
			vec4 region = vulkan->swap_chain_framebuffer_region;
			if (px >= region.x && px <= (region.x + region.z) && py >= region.y && py <= (region.y + region.w)) {
				game->imgui_io->MousePos.x = game->imgui_io->DisplaySize.x * (px - region.x) / region.z;
				game->imgui_io->MousePos.y = game->imgui_io->DisplaySize.y * (py - region.y) / region.w;
			}
			else {
				game->imgui_io->MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
			}
		}
		ImGui::NewFrame();
		{ // miscs
			if (ImGui::IsKeyPressed(VK_RETURN) && ImGui::IsKeyDown(VK_MENU)) {
				set_window_fullscreen(window, !window->fullscreen);
			}
			if (ImGui::IsKeyPressed(VK_F4) && ImGui::IsKeyDown(VK_MENU)) {
				quit_game = true;
			}
		}
		{ // camera movement
			float x_sensitivity = 0.005f;
			float y_sensitivity = 0.005f;
			game->player_camera_theta = clamp(game->player_camera_theta + window->raw_mouse_dy * y_sensitivity, -(float)M_PI / 3, (float)M_PI / 3);
			game->player_camera_phi = wrap_angle(game->player_camera_phi - window->raw_mouse_dx * x_sensitivity);
			game->player_camera_r = clamp(game->player_camera_r - game->imgui_io->MouseWheel, 4.0f, 30.0f);
			game->player_camera = level_get_player_camera(level, vulkan, game->player_camera_r, game->player_camera_theta, game->player_camera_phi);
		}
		{ // player movement
			entity_physics_component *physics_component = entity_get_physics_component(level, level->player_entity_index);
			btVector3 linear_velocity = physics_component->bt_rigid_body->getLinearVelocity();
			bool falling = fabsf(linear_velocity.y()) > 0.1f;

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
				physics_component->bt_rigid_body->setLinearVelocity(linear_velocity + btVector3(0, 10, 0));
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

					struct transform *new_transform = allocate_memory<struct transform>(&level->frame_memory_arena, 1);
					struct entity_physics_component *new_physics_component = allocate_memory<struct entity_physics_component>(&level->frame_memory_arena, 1);

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
			uint32 model_component_index = 0;
			for (uint32 i = 0; i < level->entity_count; i += 1) {
				if (level->entity_flags[i] & entity_component_flag_model) {
					entity_model_component *model_component = &level->model_components[model_component_index++];
					if (model_component->model_index < level->model_count &&  model_component->animation_index < level->models[model_component->model_index].animation_count) {
						entity_model_component *new_model_component = allocate_memory<struct entity_model_component>(&level->frame_memory_arena, 1);
						*new_model_component = *model_component;
						new_model_component->animation_time += last_frame_time_sec;
						level->entity_modifications[i].model_component = new_model_component;
					}
				}
			}
		}
		ImGui::EndFrame();

		vulkan_begin_render(vulkan, window->width, window->height);
		level_generate_render_data(level, vulkan, game->player_camera, []{});
		level_generate_render_commands(level, vulkan, game->player_camera, []{}, []{});
		vulkan_end_render(vulkan, false);

		level_update_entity_components(level);
		level->frame_memory_arena.size = 0;
		vulkan->frame_memory_arena.size = 0;
		
		stop_timer(&timer);
		last_frame_time_microsec = get_timer_duration_microsecs(timer);
		last_frame_time_sec = get_timer_duration_secs(timer);
	}
	ImGui::DestroyContext(game->imgui_context);
}
