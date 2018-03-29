/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#include "../vendor/include/json/json.hpp"

#include "../vendor/include/bullet/btBulletCollisionCommon.h"
#include "../vendor/include/bullet/btBulletDynamicsCommon.h"

#include "shader_type.cpp"

const uint32 level_max_entity_count = 1024;
const uint32 level_max_directional_light_count = 1;
const uint32 level_max_point_light_count = 4;
const uint32 level_max_spot_light_count = 4;

struct model_scene {
	uint32 node_indices[m_countof(gpk_model_scene::node_indices)];
	uint32 node_index_count;
	char name[sizeof(gpk_model_scene::name)];
};

struct model_node {
	uint32 mesh_index;
	transform local_transform;
	mat4 local_transform_mat;
	uint32 children[m_countof(gpk_model_node::children)];
	uint32 child_count;
};

struct model_mesh_primitive {
	uint32 material_index;
	uint32 index_count;
	uint32 index_buffer_offset;
	uint32 vertex_count;
	uint32 vertex_buffer_offset;
	bool has_joints;
	uint8 *vertices_data;
	uint8 *indices_data;
};

struct model_mesh {
	model_mesh_primitive *primitives;
	uint32 primitive_count;
	char name[sizeof(gpk_model_mesh::name)];
};

struct model_joint {
	uint32 node_index;
	mat4 inverse_bind_mat;
};

struct model_skin {
	model_joint *joints;
	uint32 joint_count;
	char name[sizeof(gpk_model_skin::name)];
};

struct model_animation_channel {
	uint32 node_index;
	uint32 channel_type;
	uint32 sampler_index;
};

struct model_animation_key_frame {
	float time;
	vec4 transform_data;
};

struct model_animation_sampler {
	uint32 interpolation_type;
	model_animation_key_frame *key_frames;
	uint32 key_frame_count;
};

struct model_animation {
	model_animation_channel *channels;
	uint32 channel_count;
	model_animation_sampler *samplers;
	uint32 sampler_count;
	char name [sizeof(gpk_model_animation::name)];
};

struct model_material {
	VkDescriptorSet textures_descriptor_set;
	vec4 diffuse_factor;
	float metallic_factor;
	float roughness_factor;
	char name[sizeof(gpk_model_material::name)];
};

struct model_image {
	vulkan_image image;
};

struct model {
	model_scene *scenes;
	uint32 scene_count;
	model_node *nodes;
	uint32 node_count;
	model_mesh *meshes;
	uint32 mesh_count;
	model_skin *skins;
	uint32 skin_count;
	model_animation *animations;
	uint32 animation_count;
	model_material *materials;
	uint32 material_count;
	model_image *images;
	uint32 image_count;
	char gpk_file[128];
};

struct skybox {
	VkDescriptorSet cubemap_descriptor_set;
	vulkan_image cubemap_image;
	char gpk_file[128];
};


struct terrain_vertex {
	vec3 position;
	vec2 uv;
};
static_assert(sizeof(struct terrain_vertex) == 20, "");

struct terrain {
	VkDescriptorSet textures_descriptor_set;
	vulkan_image height_map;
	vulkan_image diffuse_map;
	char gpk_file[128];
};

enum light_type {
	light_type_ambient,
	light_type_directional,
	light_type_point
};

struct ambient_light {
	vec3 color;
};

struct directional_light {
	vec3 color;
	vec3 direction;
};

struct point_light {
	vec3 color;
	vec3 position;
	float attenuation;
};

enum collision_shape {
	collision_shape_sphere,
	collision_shape_capsule,
	collision_shape_box
};

struct entity_info {
	char name[32];
};

enum entity_component_flag {
	entity_component_flag_render    = 1,
	entity_component_flag_collision = 2,
	entity_component_flag_physics   = 4,
	entity_component_flag_light     = 8
};

struct entity_render_component {
	uint32 model_index;
	transform adjustment_transform;
	uint32 animation_index;
	double animation_time;
	bool hide;
};

struct entity_collision_component {
	collision_shape shape;
	union {
		struct {
			float radius;
		} sphere;
		struct {
			float height, radius;
		} capsule;
		struct {
			vec3 size;
		} box;
	};
	btCollisionObject *bt_collision_object;
};

struct entity_physics_component {
	vec3 velocity;
	float mass;
	float max_speed;
	btRigidBody *bt_rigid_body;
};

struct entity_light_component {
	light_type light_type;
	union {
		ambient_light ambient_light;
		directional_light directional_light;
		point_light point_light;
	};
};

struct entity_modification {
	bool remove;
	bool remove_render_component;
	bool remove_collision_component;
	bool remove_physics_component;
	bool remove_light_component;
	entity_info *info;
	transform *transform;
	entity_render_component *render_component;
	entity_collision_component *collision_component;
	entity_physics_component *physics_component;
	entity_light_component *light_component;
};

struct entity_addition {
	uint32 flag;
	entity_info info;
	transform transform;
	entity_render_component *render_component;
	entity_collision_component *collision_component;
	entity_physics_component *physics_component;
	entity_light_component *light_component;
	entity_addition *next;
};

struct primitive_render_data {
	uint32 primitive_index;
	uint32 frame_uniform_buffer_offset;
};

struct mesh_render_data {
	uint32 mesh_index;
	uint32 frame_uniform_buffer_offset;
	primitive_render_data *primitives;
};

struct model_render_data {
	uint32 model_index;
	uint32 frame_uniform_buffer_offset;
	mesh_render_data *meshes;
	uint32 mesh_count;
};

struct level_render_data {
	uint32 frame_uniform_buffer_offset;
	model_render_data *models;
	uint32 model_count;
	uint32 text_frame_vertex_buffer_offset;
	uint32 text_frame_uniform_buffer_offset;
	uint32 text_frame_vertex_count;
	uint32 text_frame_uniform_count;
};

struct level {
	uint32 *entity_flags;
	entity_info *entity_infos;
	transform *entity_transforms;
	uint32 entity_count;

	entity_render_component *render_components;
	entity_collision_component *collision_components;
	entity_light_component *light_components;
	entity_physics_component *physics_components;
	uint32 render_component_count;
	uint32 collision_component_count;
	uint32 light_component_count;
	uint32 physics_component_count;

	entity_modification *entity_modifications;
	entity_addition *entity_addition;

	uint32 player_entity_index;

	model *models;
	uint32 model_count;
	uint32 model_capacity;

	skybox *skyboxes;
	uint32 skybox_count;
	uint32 skybox_capacity;
	uint32 skybox_index;

	terrain *terrains;
	uint32 terrain_count;
	uint32 terrain_capacity;
	uint32 terrain_index;
	uint32 terrain_common_vertex_buffer_offset;

	level_render_data render_data;

	memory_arena main_thread_frame_memory_arena;
	memory_arena render_thread_frame_memory_arena;
	memory_arena entity_components_memory_arenas[2];
	uint32 entity_components_memory_arena_index;
	memory_arena assets_memory_arena;

	char json_file[256];
};

void initialize_level(level *level, vulkan *vulkan) {
	level->main_thread_frame_memory_arena.name = "main thread frame";
	level->main_thread_frame_memory_arena.capacity = m_megabytes(4);
	level->main_thread_frame_memory_arena.memory = allocate_virtual_memory(level->main_thread_frame_memory_arena.capacity);
	m_assert(level->main_thread_frame_memory_arena.memory);

	level->render_thread_frame_memory_arena.name = "render thread frame";
	level->render_thread_frame_memory_arena.capacity = m_megabytes(4);
	level->render_thread_frame_memory_arena.memory = allocate_virtual_memory(level->render_thread_frame_memory_arena.capacity);
	m_assert(level->render_thread_frame_memory_arena.memory);

	for (uint32 i = 0; i < m_countof(level->entity_components_memory_arenas); i += 1) {
		level->entity_components_memory_arenas[i].name = "entity_components";
		level->entity_components_memory_arenas[i].capacity = m_megabytes(4);
		level->entity_components_memory_arenas[i].memory = allocate_virtual_memory(level->entity_components_memory_arenas[i].capacity);
		m_assert(level->entity_components_memory_arenas[i].memory);
	}

	level->assets_memory_arena.name = "assets";
	level->assets_memory_arena.capacity = m_megabytes(64);
	level->assets_memory_arena.memory = allocate_virtual_memory(level->assets_memory_arena.capacity);
	m_assert(level->assets_memory_arena.memory);

	level->model_capacity = 1024;
	level->models = allocate_memory<struct model>(&level->assets_memory_arena, level->model_capacity);
	level->skybox_capacity = 16;
	level->skyboxes = allocate_memory<struct skybox>(&level->assets_memory_arena, level->skybox_capacity);
	level->terrain_capacity = 16;
	level->terrains = allocate_memory<struct terrain>(&level->assets_memory_arena, level->terrain_capacity);

	{ // generate terrain vertices
		uint32 vertices_size = 128 * 128 * 6 * sizeof(struct terrain_vertex);
		m_memory_arena_undo_allocations_at_scope_exit(&level->main_thread_frame_memory_arena);
		terrain_vertex *vertices = allocate_memory<terrain_vertex>(&level->main_thread_frame_memory_arena, 128 * 128 * 6);
		const float dp = 0.5f;
		const float duv = 1.0f / 128.0f;
		vec3 position = {-32, 0, -32};
		vec2 uv = {0, 0};
		terrain_vertex *vertices_ptr = vertices;
		for (uint32 i = 0; i < 128; i += 1) {
			for (uint32 j = 0; j < 128; j += 1) {
				vec3 pa = position;
				vec3 pb = position; pb.z += dp;
				vec3 pc = position; pc.x += dp;
				vec3 pd = position; pd.x += dp; pd.z += dp;
				vec2 ua = uv;
				vec2 ub = uv; ub.y += duv;
				vec2 uc = uv; uc.x += duv;
				vec2 ud = uv; ud.x += duv; ud.y += duv;
				vertices_ptr[0] = {pa, ua};
				vertices_ptr[1] = {pb, ub};
				vertices_ptr[2] = {pc, uc};
				vertices_ptr[3] = {pc, uc};
				vertices_ptr[4] = {pb, ub};
				vertices_ptr[5] = {pd, ud};
				position += {dp, 0, 0};
				uv += {duv, 0};
				vertices_ptr += 6;
			}
			position = {-32, 0, position.z + dp};
			uv = {0, uv.y + duv};
		}
		round_up(&vulkan->buffers.common_vertex_buffer_offset, (uint32)sizeof(struct terrain_vertex));
		level->terrain_common_vertex_buffer_offset = vulkan->buffers.common_vertex_buffer_offset;
		vulkan_buffer_transfer(vulkan, vulkan->buffers.common_vertex_buffer, vulkan->buffers.common_vertex_buffer_offset, vertices, vertices_size);
		vulkan->buffers.common_vertex_buffer_offset += vertices_size;
	}
}

entity_render_component *entity_get_render_component(level *level, uint32 entity_index) {
	m_assert(entity_index < level->entity_count);
	m_assert(level->entity_flags[entity_index] & entity_component_flag_render);
	uint32 index = 0;
	for (uint32 i = 0; i < entity_index; i += 1) {
		if (level->entity_flags[i] & entity_component_flag_render) {
			index += 1;
		}
	}
	return &level->render_components[index];
}

entity_collision_component *entity_get_collision_component(level *level, uint32 entity_index) {
	m_assert(entity_index < level->entity_count);
	m_assert(level->entity_flags[entity_index] & entity_component_flag_collision);
	uint32 index = 0;
	for (uint32 i = 0; i < entity_index; i += 1) {
		if (level->entity_flags[i] & entity_component_flag_collision) {
			index += 1;
		}
	}
	return &level->collision_components[index];
}

entity_light_component *entity_get_light_component(level *level, uint32 entity_index) {
	m_assert(entity_index < level->entity_count);
	m_assert(level->entity_flags[entity_index] & entity_component_flag_light);
	uint32 index = 0;
	for (uint32 i = 0; i < entity_index; i += 1) {
		if (level->entity_flags[i] & entity_component_flag_light) {
			index += 1;
		}
	}
	return &level->light_components[index];
}

entity_physics_component *entity_get_physics_component(level *level, uint32 entity_index) {
	m_assert(entity_index < level->entity_count);
	m_assert(level->entity_flags[entity_index] & entity_component_flag_physics);
	uint32 index = 0;
	for (uint32 i = 0; i < entity_index; i += 1) {
		if (level->entity_flags[i] & entity_component_flag_physics) {
			index += 1;
		}
	}
	return &level->physics_components[index];
}

uint32 entity_component_get_entity_index(level *level, uint32 component_index, entity_component_flag entity_component_flag) {
	uint32 index = 0;
	for (uint32 i = 0; i < level->entity_count; i += 1) {
		if (level->entity_flags[i] & entity_component_flag) {
			if (index == component_index) {
				return i;
			}
			index += 1;
		}
	}
	m_assert(false);
	return UINT32_MAX;
}

void level_update_entity_components(level *level) {
	uint32 new_entity_count = level->entity_count;
	uint32 new_render_component_count = level->render_component_count;
	uint32 new_collision_component_count = level->collision_component_count;
	uint32 new_physics_component_count = level->physics_component_count;
	uint32 new_light_component_count = level->light_component_count;
	for (uint32 i = 0; i < level->entity_count; i += 1) {
		entity_modification *mod = &level->entity_modifications[i];
		uint32 entity_flag = level->entity_flags[i];
		if (mod->remove) {
			new_entity_count -= 1;
			if (entity_flag & entity_component_flag_render) {
				new_render_component_count -= 1;
			}
			if (entity_flag & entity_component_flag_collision) {
				new_collision_component_count -= 1;
			}
			if (entity_flag & entity_component_flag_physics) {
				new_physics_component_count -= 1;
			}
			if (entity_flag & entity_component_flag_light) {
				new_light_component_count -= 1;
			}
		}
		else {
			if (mod->remove_render_component) {
				new_render_component_count -= 1;
			}
			else if (mod->render_component && !(entity_flag & entity_component_flag_render)) {
				new_render_component_count += 1;
			}
			if (mod->remove_collision_component) {
				new_collision_component_count -= 1;
			}
			else if (mod->collision_component && !(entity_flag & entity_component_flag_collision)) {
				new_collision_component_count += 1;
			}
			if (mod->remove_physics_component) {
				new_physics_component_count -= 1;
			}
			else if (mod->physics_component && !(entity_flag & entity_component_flag_physics)) {
				new_physics_component_count += 1;
			}
			if (mod->remove_light_component) {
				new_light_component_count -= 1;
			}
			else if (mod->light_component && !(entity_flag & entity_component_flag_light)) {
				new_light_component_count += 1;
			}
		}
	}
	entity_addition *addition = level->entity_addition;
	while (addition) {
		new_entity_count += 1;
		if (addition->render_component) {
			new_render_component_count += 1;
		}
		if (addition->collision_component) {
			new_collision_component_count += 1;
		}
		if (addition->physics_component) {
			new_physics_component_count += 1;
		}
		if (addition->light_component) {
			new_light_component_count += 1;
		}
		addition = addition->next;
	}

	level->entity_components_memory_arena_index = (level->entity_components_memory_arena_index + 1) % 2;
	memory_arena *entity_components_memory_arena = &level->entity_components_memory_arenas[level->entity_components_memory_arena_index];
	entity_components_memory_arena->size = 0;

	uint32 *new_entity_flags = new_entity_count > 0 ? allocate_memory<uint32>(entity_components_memory_arena, new_entity_count) : nullptr;
	entity_info *new_entity_infos = new_entity_count > 0 ? allocate_memory<entity_info>(entity_components_memory_arena, new_entity_count) : nullptr;
	transform *new_entity_transforms = new_entity_count > 0 ? allocate_memory<transform>(entity_components_memory_arena, new_entity_count) : nullptr;
	entity_modification *new_entity_modifications = new_entity_count > 0 ? allocate_memory<entity_modification>(entity_components_memory_arena, new_entity_count) : nullptr;
	entity_render_component *new_render_components = new_render_component_count > 0 ? allocate_memory<entity_render_component>(entity_components_memory_arena, new_render_component_count) : nullptr;
	entity_collision_component *new_collision_components = new_collision_component_count > 0 ? allocate_memory<entity_collision_component>(entity_components_memory_arena, new_collision_component_count) : nullptr;
	entity_physics_component *new_physics_components = new_physics_component_count > 0 ? allocate_memory<entity_physics_component>(entity_components_memory_arena, new_physics_component_count) : nullptr;
	entity_light_component *new_light_components = new_light_component_count > 0 ? allocate_memory<entity_light_component>(entity_components_memory_arena, new_light_component_count) : nullptr;

	uint32 entity_index = 0;
	uint32 render_component_index = 0;
	uint32 collision_component_index = 0;
	uint32 physics_component_index = 0;
	uint32 light_component_index = 0;
	for (uint32 i = 0; i < level->entity_count; i += 1) {
		entity_modification *mod = &level->entity_modifications[i];
		if (!mod->remove) {
			uint32 entity_flags = level->entity_flags[i];
			if (mod->remove_render_component) {
				entity_flags &= ~entity_component_flag_render;
			}
			else if (entity_flags & entity_component_flag_render || mod->render_component) {
				entity_flags |= entity_component_flag_render;
				new_render_components[render_component_index++] = mod->render_component ? *mod->render_component : *entity_get_render_component(level, i);
			}
			if (mod->remove_collision_component) {
				entity_flags &= ~entity_component_flag_collision;
			}
			else if (entity_flags & entity_component_flag_collision || mod->collision_component) {
				entity_flags |= entity_component_flag_collision;
				new_collision_components[collision_component_index++] = mod->collision_component ? *mod->collision_component : *entity_get_collision_component(level, i);
			}
			if (mod->remove_physics_component) {
				entity_flags &= ~entity_component_flag_physics;
			}
			else if (entity_flags & entity_component_flag_physics || mod->physics_component) {
				entity_flags |= entity_component_flag_physics;
				new_physics_components[physics_component_index++] = mod->physics_component ? *mod->physics_component : *entity_get_physics_component(level, i);
			}
			if (mod->remove_light_component) {
				entity_flags &= ~entity_component_flag_light;
			}
			else if (entity_flags & entity_component_flag_light || mod->light_component) {
				entity_flags |= entity_component_flag_light;
				new_light_components[light_component_index++] = mod->light_component ? *mod->light_component : *entity_get_light_component(level, i);
			}
			new_entity_flags[entity_index] = entity_flags;
			new_entity_infos[entity_index] = mod->info ? *mod->info : level->entity_infos[i];
			new_entity_transforms[entity_index] = mod->transform ? *mod->transform : level->entity_transforms[i];
			entity_index += 1;
		}
	}
	addition = level->entity_addition;
	while (addition) {
		new_entity_flags[entity_index] = addition->flag;
		new_entity_infos[entity_index] = addition->info;
		new_entity_transforms[entity_index] = addition->transform;
		if (addition->render_component) {
			new_render_components[render_component_index++] = *addition->render_component;
		}
		if (addition->collision_component) {
			new_collision_components[collision_component_index++] = *addition->collision_component;
		}
		if (addition->physics_component) {
			new_physics_components[physics_component_index++] = *addition->physics_component;
		}
		if (addition->light_component) {
			new_light_components[light_component_index++] = *addition->light_component;
		}
		entity_index += 1;
		addition = addition->next;
	}

	level->entity_flags = new_entity_flags;
	level->entity_infos = new_entity_infos;
	level->entity_transforms = new_entity_transforms;
	level->entity_count = new_entity_count;

	level->entity_modifications = new_entity_modifications;
	level->entity_addition = nullptr;

	level->render_components = new_render_components;
	level->collision_components = new_collision_components;
	level->physics_components = new_physics_components;
	level->light_components = new_light_components;

	level->render_component_count = new_render_component_count;
	level->collision_component_count = new_collision_component_count;
	level->physics_component_count = new_physics_component_count;
	level->light_component_count = new_light_component_count;
}

uint32 level_get_model_index(level *level, const char *gpk_file) {
	for (uint32 i = 0; i < level->model_count; i += 1) {
		if (!strcmp(level->models[i].gpk_file, gpk_file)) {
			return i;
		}
	}
	return UINT32_MAX;
}

uint32 level_add_gpk_model(level *level, vulkan *vulkan, const char *gpk_file, bool store_vertices = false) {
	m_assert(level->model_count < level->model_capacity);
	for (uint32 i = 0; i < level->model_count; i += 1) {
		m_assert(strcmp(level->models[i].gpk_file, gpk_file));
	}

	file_mapping gpk_file_mapping = {};
	m_assert(open_file_mapping(gpk_file, &gpk_file_mapping));
	m_scope_exit(close_file_mapping(gpk_file_mapping));

	model *model = &level->models[level->model_count];
	gpk_model *gpk_model = (struct gpk_model *)gpk_file_mapping.ptr;
	m_assert(!strcmp(gpk_model->format_str, m_gpk_model_format_str));
	snprintf(model->gpk_file, sizeof(model->gpk_file), "%s", gpk_file);
	model->scene_count = gpk_model->scene_count;
	model->node_count = gpk_model->node_count;
	model->mesh_count = gpk_model->mesh_count;
	model->skin_count = gpk_model->skin_count;
	model->animation_count = gpk_model->animation_count;
	model->material_count = gpk_model->material_count;
	model->image_count = gpk_model->image_count;
	model->scenes = allocate_memory<struct model_scene>(&level->assets_memory_arena, model->scene_count);
	model->nodes = allocate_memory<struct model_node>(&level->assets_memory_arena, model->node_count);
	model->meshes = allocate_memory<struct model_mesh>(&level->assets_memory_arena, model->mesh_count);
	if (model->skin_count > 0) {
		model->skins = allocate_memory<struct model_skin>(&level->assets_memory_arena, model->skin_count);
	}
	if (model->animation_count > 0) {
		model->animations = allocate_memory<struct model_animation>(&level->assets_memory_arena, model->animation_count);
	}
	if (model->material_count > 0) {
		model->materials = allocate_memory<struct model_material>(&level->assets_memory_arena, model->material_count);
	}
	if (model->image_count > 0) {
		model->images = allocate_memory<struct model_image>(&level->assets_memory_arena, model->image_count);
	}
	for (uint32 i = 0; i < model->scene_count; i += 1) {
		gpk_model_scene *gpk_model_scene = ((struct gpk_model_scene*)(gpk_file_mapping.ptr + gpk_model->scene_offset)) + i;
		model_scene *model_scene = &model->scenes[i];

		array_copy(model_scene->name, gpk_model_scene->name);
		array_copy(model_scene->node_indices, gpk_model_scene->node_indices);
		model_scene->node_index_count = gpk_model_scene->node_index_count;
	}
	for (uint32 i = 0; i < model->node_count; i += 1) {
		gpk_model_node *gpk_model_node = ((struct gpk_model_node*)(gpk_file_mapping.ptr + gpk_model->node_offset)) + i;
		model_node *model_node = &model->nodes[i];

		model_node->mesh_index = gpk_model_node->mesh_index;
		model_node->local_transform = gpk_model_node->local_transform;
		model_node->local_transform_mat = gpk_model_node->local_transform_mat;
		array_copy(model_node->children, gpk_model_node->children);
		model_node->child_count = gpk_model_node->child_count;
	}
	for (uint32 i = 0; i < model->mesh_count; i += 1) {
		gpk_model_mesh *gpk_model_mesh = ((struct gpk_model_mesh*)(gpk_file_mapping.ptr + gpk_model->mesh_offset)) + i;
		model_mesh *model_mesh = &model->meshes[i];
		array_copy(model_mesh->name, gpk_model_mesh->name);
		model_mesh->primitive_count = gpk_model_mesh->primitive_count;
		model_mesh->primitives = allocate_memory<struct model_mesh_primitive>(&level->assets_memory_arena, model_mesh->primitive_count);
		for (uint32 i = 0; i < model_mesh->primitive_count; i += 1) {
			gpk_model_mesh_primitive *gpk_primitive = ((gpk_model_mesh_primitive *)(gpk_file_mapping.ptr + gpk_model_mesh->primitive_offset)) + i;
			model_mesh_primitive *primitive = &model_mesh->primitives[i];

			primitive->material_index = gpk_primitive->material_index;
			primitive->has_joints = gpk_primitive->has_joints;

			uint32 indices_size = gpk_primitive->index_count * sizeof(uint16);
			uint32 vertices_size = gpk_primitive->vertex_count * sizeof(struct gpk_model_vertex);
			uint8 *indices_data = gpk_file_mapping.ptr + gpk_primitive->indices_offset;
			uint8 *vertices_data = gpk_file_mapping.ptr + gpk_primitive->vertices_offset;
			
			primitive->index_count = gpk_primitive->index_count;
			round_up(&vulkan->buffers.level_vertex_buffer_offset, (uint32)sizeof(uint16));
			primitive->index_buffer_offset = vulkan->buffers.level_vertex_buffer_offset;
			vulkan->buffers.level_vertex_buffer_offset += indices_size;

			primitive->vertex_count = gpk_primitive->vertex_count;
			round_up(&vulkan->buffers.level_vertex_buffer_offset, (uint32)sizeof(struct gpk_model_vertex));
			primitive->vertex_buffer_offset = vulkan->buffers.level_vertex_buffer_offset;
			vulkan->buffers.level_vertex_buffer_offset += vertices_size;

			vulkan_buffer_transfer(vulkan, vulkan->buffers.level_vertex_buffer, primitive->index_buffer_offset, indices_data, indices_size);
			vulkan_buffer_transfer(vulkan, vulkan->buffers.level_vertex_buffer, primitive->vertex_buffer_offset, vertices_data, vertices_size);
			if (store_vertices) {
				primitive->indices_data = allocate_memory<uint8>(&level->assets_memory_arena, indices_size);
				primitive->vertices_data = allocate_memory<uint8>(&level->assets_memory_arena, vertices_size);
				memcpy(primitive->indices_data, indices_data, indices_size);
				memcpy(primitive->vertices_data, vertices_data, vertices_size);
			}
		}
	}
	for (uint32 i = 0; i < model->skin_count; i += 1) {
		gpk_model_skin *gpk_model_skin = ((struct gpk_model_skin*)(gpk_file_mapping.ptr + gpk_model->skin_offset)) + i;
		model_skin *model_skin = &model->skins[i];
		array_copy(model_skin->name, gpk_model_skin->name);
		model_skin->joint_count = gpk_model_skin->joint_count;
		m_assert(model_skin->joint_count > 0);
		model_skin->joints = allocate_memory<struct model_joint>(&level->assets_memory_arena, model_skin->joint_count);
		gpk_model_joint *gpk_joints = (gpk_model_joint *)(gpk_file_mapping.ptr + gpk_model_skin->joints_offset);
		for (uint32 i = 0; i < model_skin->joint_count; i += 1) {
			model_skin->joints[i].node_index = gpk_joints[i].node_index;
			model_skin->joints[i].inverse_bind_mat = gpk_joints[i].inverse_bind_mat;
		}
	}
	for (uint32 i = 0; i < model->animation_count; i += 1) {
		gpk_model_animation *gpk_animation = ((struct gpk_model_animation*)(gpk_file_mapping.ptr + gpk_model->animation_offset)) + i;
		model_animation *animation = &model->animations[i];
		array_copy(animation->name, gpk_animation->name);
		animation->channel_count = gpk_animation->channel_count;
		animation->channels = allocate_memory<struct model_animation_channel>(&level->assets_memory_arena, animation->channel_count);
		for (uint32 i = 0; i < animation->channel_count; i += 1) {
			gpk_model_animation_channel *gpk_channel = ((struct gpk_model_animation_channel*)(gpk_file_mapping.ptr + gpk_animation->channel_offset)) + i;
			model_animation_channel *channel = &animation->channels[i];
			channel->node_index = gpk_channel->node_index;
			channel->channel_type = gpk_channel->channel_type;
			channel->sampler_index = gpk_channel->sampler_index;
		}
		animation->sampler_count = gpk_animation->sampler_count;
		animation->samplers = allocate_memory<model_animation_sampler>(&level->assets_memory_arena, animation->sampler_count);
		for (uint32 i = 0; i < animation->sampler_count; i += 1) {
			gpk_model_animation_sampler *gpk_sampler = ((struct gpk_model_animation_sampler*)(gpk_file_mapping.ptr + gpk_animation->sampler_offset)) + i;
			model_animation_sampler *sampler = &animation->samplers[i];
			sampler->interpolation_type = gpk_sampler->interpolation_type;
			sampler->key_frame_count = gpk_sampler->key_frame_count;
			sampler->key_frames = allocate_memory<struct model_animation_key_frame>(&level->assets_memory_arena, sampler->key_frame_count);
			for (uint32 i = 0; i < sampler->key_frame_count; i += 1) {
				gpk_model_animation_key_frame *gpk_key_frame = ((struct gpk_model_animation_key_frame *)(gpk_file_mapping.ptr + gpk_sampler->key_frame_offset)) + i;
				sampler->key_frames[i].time = gpk_key_frame->time;
				sampler->key_frames[i].transform_data = gpk_key_frame->transform_data;
			}
		}
	}
	for (uint32 i = 0; i < model->image_count; i += 1) {
		gpk_model_image *gpk_model_image = ((struct gpk_model_image*)(gpk_file_mapping.ptr + gpk_model->image_offset)) + i;
		VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = (VkFormat)gpk_model_image->format;
		image_create_info.extent = {gpk_model_image->width, gpk_model_image->height, 1};
		image_create_info.mipLevels = gpk_model_image->mipmap_count;
		image_create_info.arrayLayers = gpk_model_image->layer_count;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = image_create_info.format;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;
		image_view_create_info.subresourceRange.layerCount = image_create_info.arrayLayers;
		vulkan_image vulkan_image = allocate_vulkan_image(&vulkan->device, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, gpk_model_image->format_block_dimension, gpk_model_image->format_block_size);
		vulkan_image_transfer(&vulkan->device, &vulkan->cmd_buffers, &vulkan_image, gpk_file_mapping.ptr + gpk_model_image->data_offset, gpk_model_image->size);
		model->images[i].image = vulkan_image;
	}
	for (uint32 i = 0; i < model->material_count; i += 1) {
		gpk_model_material *gpk_model_material = ((struct gpk_model_material*)(gpk_file_mapping.ptr + gpk_model->material_offset)) + i;
		model_material *model_material = &model->materials[i];
		array_copy(model_material->name, gpk_model_material->name);
		model_material->diffuse_factor = gpk_model_material->diffuse_factor;
		model_material->metallic_factor = gpk_model_material->metallic_factor;
		model_material->roughness_factor = gpk_model_material->roughness_factor;

		VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
		descriptor_set_allocate_info.descriptorPool = vulkan->descriptors.pool;
		descriptor_set_allocate_info.descriptorSetCount = 1;
		descriptor_set_allocate_info.pSetLayouts = &vulkan->descriptors.textures_descriptor_set_layouts[4];
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &model_material->textures_descriptor_set));
		VkDescriptorImageInfo descriptor_image_infos[5] = {};
		if (gpk_model_material->diffuse_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->diffuse_image_index].image;
			descriptor_image_infos[0] = {vulkan->samplers.mipmap_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[0] = {vulkan->samplers.mipmap_samplers[1], vulkan->images.default_diffuse_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		if (gpk_model_material->metallic_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->metallic_image_index].image;
			descriptor_image_infos[1] = {vulkan->samplers.mipmap_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[1] = {vulkan->samplers.mipmap_samplers[1], vulkan->images.default_metallic_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		if (gpk_model_material->roughness_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->roughness_image_index].image;
			descriptor_image_infos[2] = {vulkan->samplers.mipmap_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[2] = {vulkan->samplers.mipmap_samplers[1], vulkan->images.default_roughness_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		if (gpk_model_material->normal_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->normal_image_index].image;
			descriptor_image_infos[3] = {vulkan->samplers.mipmap_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[3] = {vulkan->samplers.mipmap_samplers[1], vulkan->images.default_normal_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		// if (gpk_model_material->height_image_index < model->image_count) {
		// 	vulkan_image *image = &model->images[gpk_model_material->height_image_index].image;
		// 	descriptor_image_infos[4] = {vulkan->samplers.mipmap_image_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		// }
		// else {
		// 	descriptor_image_infos[4] = {vulkan->samplers.mipmap_image_samplers[1], vulkan->images.default_height_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		// }
		descriptor_image_infos[4] = {vulkan->samplers.mipmap_samplers[1], vulkan->images.default_height_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		VkWriteDescriptorSet write_descriptor_sets[5] = {};
		for (uint32 i = 0; i < m_countof(write_descriptor_sets); i += 1) {
			write_descriptor_sets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_sets[i].dstSet = model_material->textures_descriptor_set;
			write_descriptor_sets[i].dstBinding = i;
			write_descriptor_sets[i].descriptorCount = 1;
			write_descriptor_sets[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_descriptor_sets[i].pImageInfo = &descriptor_image_infos[i];
		}
		vkUpdateDescriptorSets(vulkan->device.device, m_countof(write_descriptor_sets), write_descriptor_sets, 0, nullptr);
	}
	return level->model_count++;
}

void level_add_skybox(level *level, vulkan *vulkan, const char *gpk_file) {
	m_assert(level->skybox_count < level->skybox_capacity);
	skybox *skybox = &level->skyboxes[level->skybox_count++];
	m_assert(strlen(gpk_file) < sizeof(skybox->gpk_file));
	strcpy(skybox->gpk_file, gpk_file);

	file_mapping gpk_file_mapping = {};
	m_assert(open_file_mapping(gpk_file, &gpk_file_mapping));
	m_scope_exit(close_file_mapping(gpk_file_mapping));
	gpk_skybox *gpk_skybox = (struct gpk_skybox *)gpk_file_mapping.ptr;
	m_assert(!strcmp(gpk_skybox->format_str, m_gpk_skybox_format_str));

	VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
	image_create_info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	image_create_info.imageType = VK_IMAGE_TYPE_2D;
	image_create_info.format = (VkFormat)gpk_skybox->cubemap_format;
	image_create_info.extent = {gpk_skybox->cubemap_width, gpk_skybox->cubemap_height, 1};
	image_create_info.mipLevels = gpk_skybox->cubemap_mipmap_count;
	image_create_info.arrayLayers = gpk_skybox->cubemap_layer_count;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	image_view_create_info.format = image_create_info.format;
	image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_create_info.subresourceRange.levelCount = gpk_skybox->cubemap_mipmap_count;
	image_view_create_info.subresourceRange.layerCount = gpk_skybox->cubemap_layer_count;

	uint8 *cubemap_ptr = (uint8 *)gpk_skybox + gpk_skybox->cubemap_offset;
	skybox->cubemap_image = allocate_vulkan_image(&vulkan->device, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, 
                                         		    gpk_skybox->cubemap_format_block_dimension, gpk_skybox->cubemap_format_block_size);
	vulkan_image_transfer(&vulkan->device, &vulkan->cmd_buffers, &skybox->cubemap_image, cubemap_ptr, gpk_skybox->cubemap_size);

	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
	descriptor_set_allocate_info.descriptorPool = vulkan->descriptors.pool;
	descriptor_set_allocate_info.descriptorSetCount = 1;
	descriptor_set_allocate_info.pSetLayouts = &vulkan->descriptors.textures_descriptor_set_layouts[0];
	m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &skybox->cubemap_descriptor_set));
	VkDescriptorImageInfo descriptor_image_info = {vulkan->samplers.skybox_cubemap_sampler, skybox->cubemap_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = skybox->cubemap_descriptor_set;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
}

void level_add_terrain(level *level, vulkan *vulkan, const char *gpk_file) {
	m_assert(level->terrain_count < level->terrain_capacity);
	terrain *terrain = &level->terrains[level->terrain_count++];
	m_assert(strlen(gpk_file) < sizeof(terrain->gpk_file));
	strcpy(terrain->gpk_file, gpk_file);

	file_mapping gpk_file_mapping = {};
	m_assert(open_file_mapping(gpk_file, &gpk_file_mapping));
	m_scope_exit(close_file_mapping(gpk_file_mapping));
	gpk_terrain *gpk_terrain = (struct gpk_terrain *)gpk_file_mapping.ptr;
	m_assert(!strcmp(gpk_terrain->format_str, m_gpk_terrain_format_str));
	{
		VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = VK_FORMAT_R16_UNORM;
		image_create_info.extent = {gpk_terrain->height_map_width, gpk_terrain->height_map_height, 1};
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = image_create_info.format;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.layerCount = 1;

		uint8 *height_map_ptr = gpk_file_mapping.ptr + gpk_terrain->height_map_offset;
		terrain->height_map = allocate_vulkan_image(&vulkan->device, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, 1, 2);
		vulkan_image_transfer(&vulkan->device, &vulkan->cmd_buffers, &terrain->height_map, height_map_ptr, gpk_terrain->height_map_size);
	}
	{
		VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_create_info.extent = {gpk_terrain->diffuse_map_width, gpk_terrain->diffuse_map_height, 1};
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = image_create_info.format;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.layerCount = 1;

		uint8 *diffuse_map_ptr = gpk_file_mapping.ptr + gpk_terrain->diffuse_map_offset;
		terrain->diffuse_map = allocate_vulkan_image(&vulkan->device, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, 1, 4);
		vulkan_image_transfer(&vulkan->device, &vulkan->cmd_buffers, &terrain->diffuse_map, diffuse_map_ptr, gpk_terrain->diffuse_map_size);
	}
	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
	descriptor_set_allocate_info.descriptorPool = vulkan->descriptors.pool;
	descriptor_set_allocate_info.descriptorSetCount = 1;
	descriptor_set_allocate_info.pSetLayouts = &vulkan->descriptors.textures_descriptor_set_layouts[1];
	m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &terrain->textures_descriptor_set));
	VkDescriptorImageInfo descriptor_image_infos[2] = {};
	descriptor_image_infos[0] = {vulkan->samplers.terrain_texture_sampler, terrain->height_map.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	descriptor_image_infos[1] = {vulkan->samplers.terrain_texture_sampler, terrain->diffuse_map.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_sets[2] = {};
	write_descriptor_sets[0] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_sets[0].dstSet = terrain->textures_descriptor_set;
	write_descriptor_sets[0].dstBinding = 0;
	write_descriptor_sets[0].descriptorCount = 1;
	write_descriptor_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_sets[0].pImageInfo = &descriptor_image_infos[0];
	write_descriptor_sets[1] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_sets[1].dstSet = terrain->textures_descriptor_set;
	write_descriptor_sets[1].dstBinding = 1;
	write_descriptor_sets[1].descriptorCount = 1;
	write_descriptor_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_sets[1].pImageInfo = &descriptor_image_infos[1];
	vkUpdateDescriptorSets(vulkan->device.device, m_countof(write_descriptor_sets), write_descriptor_sets, 0, nullptr);
}

template <typename F>
void level_read_json(level *level, vulkan *vulkan, const char *level_json_file, F extra_read, bool store_vertices) {
	file_mapping level_json_file_mapping = {};
	m_assert(open_file_mapping(level_json_file, &level_json_file_mapping));
	nlohmann::json json = nlohmann::json::parse(level_json_file_mapping.ptr, level_json_file_mapping.ptr + level_json_file_mapping.size);
	close_file_mapping(level_json_file_mapping);
	{
		level->render_data = {};
		level->entity_components_memory_arenas[0].size = 0;
		level->entity_components_memory_arenas[1].size = 0;
		level->entity_components_memory_arena_index = 0;
		level->assets_memory_arena.size = 0;
		snprintf(level->json_file, sizeof(level->json_file), "%s", level_json_file);

		for (uint32 i = 0; i < level->model_count; i += 1) {
			model *model = &level->models[i];
			for (uint32 i = 0; i < model->material_count; i += 1) {
				vkFreeDescriptorSets(vulkan->device.device, vulkan->descriptors.pool, 1, &model->materials[i].textures_descriptor_set);
			}
			for (uint32 i = 0; i < model->image_count; i += 1) {
				vkDestroyImageView(vulkan->device.device, model->images[i].image.view, nullptr);
				vkDestroyImage(vulkan->device.device, model->images[i].image.image, nullptr);
			}
		}
		level->model_count = 0;

		for (uint32 i = 0; i < level->skybox_count; i += 1) {
			skybox *skybox = &level->skyboxes[i];
			vkFreeDescriptorSets(vulkan->device.device, vulkan->descriptors.pool, 1, &skybox->cubemap_descriptor_set);
			vkDestroyImageView(vulkan->device.device, skybox->cubemap_image.view, nullptr);
			vkDestroyImage(vulkan->device.device, skybox->cubemap_image.image, nullptr);
		}
		level->skybox_count = 0;

		vulkan->buffers.level_vertex_buffer_offset = 0;
		vulkan->memories.level_images_memory.size = 0;
	}
	{ // models, skyboxes, terrains
		std::vector<std::string> model_gpk_files = json["models"];
		for (auto &m : model_gpk_files) {
			level_add_gpk_model(level, vulkan, m.c_str(), store_vertices);
		}
		std::vector<std::string> skybox_gpk_files = json["skyboxes"];
		for (auto &s : skybox_gpk_files) {
			level_add_skybox(level, vulkan, s.c_str());
		}
		level->skybox_index = json["skybox_index"];
		std::vector<std::string> terrain_gpk_files = json["terrains"];
		for (auto &s : terrain_gpk_files) {
			level_add_terrain(level, vulkan, s.c_str());
		}
		level->terrain_index = json["terrain_index"];
	}
	{ // entities
		auto &entities_json = json["entities"];
		level->entity_count = (uint32)entities_json.size();
		level->render_component_count = 0;
		level->collision_component_count = 0;
		level->physics_component_count = 0;
		level->light_component_count = 0;
		for (auto &entity : entities_json) {
			if (entity.find("render_component") != entity.end()) {
				level->render_component_count += 1;
			}
			if (entity.find("collision_component") != entity.end()) {
				level->collision_component_count += 1;
			}
			if (entity.find("physics_component") != entity.end()) {
				level->physics_component_count += 1;
			}
			if (entity.find("light_component") != entity.end()) {
				level->light_component_count += 1;
			}
		}

		level->entity_components_memory_arena_index = 0;
		memory_arena *memory_arena = &level->entity_components_memory_arenas[0];
		if (level->entity_count > 0) {
			level->entity_flags = allocate_memory<uint32>(memory_arena, level->entity_count);
			level->entity_infos = allocate_memory<entity_info>(memory_arena, level->entity_count);
			level->entity_transforms = allocate_memory<transform>(memory_arena, level->entity_count);
			level->entity_modifications = allocate_memory<entity_modification>(memory_arena, level->entity_count);
		}
		if (level->render_component_count > 0) {
			level->render_components = allocate_memory<entity_render_component>(memory_arena, level->render_component_count);
		}
		if (level->collision_component_count > 0) {
			level->collision_components = allocate_memory<entity_collision_component>(memory_arena, level->collision_component_count);
		}
		if (level->physics_component_count > 0) {
			level->physics_components = allocate_memory<entity_physics_component>(memory_arena, level->physics_component_count);
		}
		if (level->light_component_count > 0) {
			level->light_components = allocate_memory<entity_light_component>(memory_arena, level->light_component_count);
		}

		auto read_entity_info = [](const nlohmann::json &json, entity_info *info) {
			std::string name = json["name"];
			m_assert(name.length() < sizeof(info->name));
			strcpy(info->name, name.c_str());
		};
		auto read_transform = [](const nlohmann::json &json, transform *transform) {
			std::array<float, 3> scale = json["scale"];
			std::array<float, 4> rotate = json["rotate"];
			std::array<float, 3> translate = json["translate"];
			transform->scale = {scale[0], scale[1], scale[2]};
			transform->rotate = {rotate[0], rotate[1], rotate[2], rotate[3]};
			transform->translate = {translate[0], translate[1], translate[2]};
		};
    auto read_render_component = [level, read_transform](const nlohmann::json &json, entity_render_component *render_component) {
			std::string gpk_file = json["gpk_file"];
			render_component->model_index = level_get_model_index(level, gpk_file.c_str());
			auto adjustment_transform_field = json.find("adjustment_transform");
			if (adjustment_transform_field != json.end()) {
				read_transform(*adjustment_transform_field, &render_component->adjustment_transform);
			}
			else {
				render_component->adjustment_transform = transform_identity();
			}
		};
    auto read_collision_component = [](const nlohmann::json &json, entity_collision_component *collision_component) {
			std::string shape = json["shape"];
			if (shape == "sphere") {
				collision_component->shape = collision_shape_sphere;
				collision_component->sphere.radius = json["radius"];
			}
			else if (shape == "capsule") {
				collision_component->shape = collision_shape_capsule;
				collision_component->capsule.height = json["height"];
				collision_component->capsule.radius = json["radius"];
			}
			else if (shape == "box") {
				collision_component->shape = collision_shape_box;
				std::array<float, 3> size = json["size"];
				collision_component->box.size = {size[0], size[1], size[2]};
			}
			else {
				m_assert(false);
			}
		};
		auto read_physics_component = [](const nlohmann::json &json, entity_physics_component *physics_component) {
			auto velocity_field = json.find("velocity");
			if (velocity_field != json.end()) {
				std::array<float, 3> velocity = *velocity_field;
				physics_component->velocity = {velocity[0], velocity[1], velocity[2]};
			}
			auto mass_field = json.find("mass");
			if (mass_field != json.end()) {
				physics_component->mass = *mass_field;
			}
			auto max_speed_field = json.find("max_speed");
			if (max_speed_field != json.end()) {
				physics_component->max_speed = *max_speed_field;
			}
		};
		auto read_light_component = [](const nlohmann::json &json, entity_light_component *light_component) {
			std::string light_type = json["light_type"];
			if (light_type == "ambient") {
				light_component->light_type = light_type_ambient;
				std::array<float, 3> color = json["color"];
				light_component->ambient_light.color = {color[0], color[1], color[2]};
			}
			else if (light_type == "directional") {
				light_component->light_type = light_type_directional;
				std::array<float, 3> color = json["color"];
				std::array<float, 3> direction = json["direction"];
				light_component->directional_light.color = {color[0], color[1], color[2]};
				light_component->directional_light.direction = {direction[0], direction[1], direction[2]};
			}
			else if (light_type == "point") {
				light_component->light_type = light_type_point;
				std::array<float, 3> color = json["color"];
				std::array<float, 3> position = json["position"];
				light_component->point_light.color = {color[0], color[1], color[2]};
				light_component->point_light.position = {position[0], position[1], position[2]};
				light_component->point_light.attenuation = json["attenuation"];
			}
			else {
				m_assert(false);
			}
		};

		uint32 render_component_index = 0;
		uint32 collision_component_index = 0;
		uint32 physics_component_index = 0;
		uint32 light_component_index = 0;
		for (uint32 i = 0; i < level->entity_count; i += 1) {
			auto &entity_json = entities_json[i];
			uint32 &entity_flags = level->entity_flags[i];
			entity_info &entity_info = level->entity_infos[i];
			transform &entity_transform = level->entity_transforms[i];
			entity_flags = 0;
			read_entity_info(entity_json, &entity_info);
			read_transform(entity_json["transform"], &entity_transform);

			auto render_component_field = entity_json.find("render_component");
			if (render_component_field != entity_json.end()) {
				entity_flags |= entity_component_flag_render;
				read_render_component(*render_component_field, &level->render_components[render_component_index++]);
			}
			auto collision_component_field = entity_json.find("collision_component");
			if (collision_component_field != entity_json.end()) {
				entity_flags |= entity_component_flag_collision;
				read_collision_component(*collision_component_field, &level->collision_components[collision_component_index++]);
			}
			auto physics_component_field = entity_json.find("physics_component");
			if (physics_component_field != entity_json.end()) {
				entity_flags |= entity_component_flag_physics;
				read_physics_component(*physics_component_field, &level->physics_components[physics_component_index++]);
			}
			auto light_component_field = entity_json.find("light_component");
			if (light_component_field != entity_json.end()) {
				entity_flags |= entity_component_flag_light;
				read_light_component(*light_component_field, &level->light_components[light_component_index++]);
			}
			if (collision_component_field != entity_json.end() && physics_component_field != entity_json.end()) {
				entity_collision_component *collision_component = &level->collision_components[collision_component_index - 1];
				entity_physics_component *physics_component = &level->physics_components[physics_component_index - 1];
				btRigidBody *rigid_body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(physics_component->mass, nullptr, nullptr));
				if (collision_component->shape == collision_shape_sphere) {
					rigid_body->setCollisionShape(new btSphereShape(collision_component->sphere.radius));
				}
				else if (collision_component->shape == collision_shape_capsule) {
					rigid_body->setCollisionShape(new btCapsuleShape(collision_component->capsule.radius, collision_component->capsule.height));
				}
				else if (collision_component->shape == collision_shape_box) {
					rigid_body->setCollisionShape(new btBoxShape(btVector3(collision_component->box.size.x / 2, collision_component->box.size.y / 2, collision_component->box.size.z / 2)));
				}
				btQuaternion rotate(entity_transform.rotate.x, entity_transform.rotate.y, entity_transform.rotate.z, entity_transform.rotate.w);
				btVector3 translate(entity_transform.translate.x, entity_transform.translate.y, entity_transform.translate.z);
				rigid_body->setWorldTransform(btTransform(rotate, translate));
				rigid_body->setLinearVelocity(btVector3(physics_component->velocity.x, physics_component->velocity.y, physics_component->velocity.z));
				physics_component->bt_rigid_body = rigid_body;
			}
			else if (collision_component_field != entity_json.end()) {
				entity_collision_component *collision_component = &level->collision_components[collision_component_index - 1];
				btCollisionObject *collision_object = new btCollisionObject();
				if (collision_component->shape == collision_shape_sphere) {
					collision_object->setCollisionShape(new btSphereShape(collision_component->sphere.radius));
				}
				else if (collision_component->shape == collision_shape_capsule) {
					collision_object->setCollisionShape(new btCapsuleShape(collision_component->capsule.radius, collision_component->capsule.height));
				}
				else if (collision_component->shape == collision_shape_box) {
					collision_object->setCollisionShape(new btBoxShape(btVector3(collision_component->box.size.x / 2, collision_component->box.size.y / 2, collision_component->box.size.z / 2)));
				}
				btQuaternion rotate(entity_transform.rotate.x, entity_transform.rotate.y, entity_transform.rotate.z, entity_transform.rotate.w);
				btVector3 translate(entity_transform.translate.x, entity_transform.translate.y, entity_transform.translate.z);
				collision_object->setWorldTransform(btTransform(rotate, translate));
				collision_component->bt_collision_object = collision_object;
			}
			else if (physics_component_field != entity_json.end()) {
				entity_physics_component *physics_component = &level->physics_components[physics_component_index - 1];
				btRigidBody *rigid_body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(physics_component->mass, nullptr, new btEmptyShape()));
				btQuaternion rotate(entity_transform.rotate.x, entity_transform.rotate.y, entity_transform.rotate.z, entity_transform.rotate.w);
				btVector3 translate(entity_transform.translate.x, entity_transform.translate.y, entity_transform.translate.z);
				rigid_body->setWorldTransform(btTransform(rotate, translate));
				rigid_body->setLinearVelocity(btVector3(physics_component->velocity.x, physics_component->velocity.y, physics_component->velocity.z));
				physics_component->bt_rigid_body = rigid_body;
			}
		}
	}
  { // player
		level->player_entity_index = UINT32_MAX;
		auto &player = json["player"];
		auto entity_name = player.find("entity_name");
		if (entity_name != player.end()) {
			std::string player_entity_name = *entity_name;
			for (uint32 i = 0; i < level->entity_count; i += 1) {
				if (!strcmp(player_entity_name.c_str(), level->entity_infos[i].name)) {
					level->player_entity_index = i;
					break;
				}
			}
		}
	}
	extra_read(json);
}

template <typename F>
void level_write_json(level *level, const char *json_file_path, F extra_write) {
	nlohmann::json json;
	{ // models, skyboxes, terrains
		auto &models = json["models"];
		models = nlohmann::json::array();
		for (uint32 i = 0; i < level->model_count; i += 1) {
			models.push_back(level->models[i].gpk_file);
		}
		auto &skyboxes = json["skyboxes"];
		skyboxes = nlohmann::json::array();
		for (uint32 i = 0; i < level->skybox_count; i += 1) {
			skyboxes.push_back(level->skyboxes[i].gpk_file);
		}
		json["skybox_index"] = level->skybox_index;
		auto &terrains = json["terrains"];
		terrains = nlohmann::json::array();
		for (uint32 i = 0; i < level->terrain_count; i += 1) {
			terrains.push_back(level->terrains[i].gpk_file);
		}
		json["terrain_index"] = level->terrain_index;
	}
	{ // entities
		auto &entities = json["entities"];
		entities = nlohmann::json::array();
		for (uint32 i = 0; i < level->entity_count; i += 1) {
			uint32 &flags = level->entity_flags[i];
			entity_info &info = level->entity_infos[i];
			transform &tfm = level->entity_transforms[i];
			nlohmann::json entity_json = {
				{"name", level->entity_infos[i].name},
				{"transform", {
					{"scale", {m_unpack3(tfm.scale)}}, 
					{"rotate", {m_unpack4(tfm.rotate)}},
					{"translate", {m_unpack3(tfm.translate)}}}
				}
			};
			if (flags & entity_component_flag_render) {
				entity_render_component *render_component = entity_get_render_component(level, i);
				transform &tfm = render_component->adjustment_transform;
				entity_json["render_component"] = {
					{"gpk_file", render_component->model_index < level->model_count ? level->models[render_component->model_index].gpk_file : ""},
					{"adjustment_transform", {
						{"scale", {m_unpack3(tfm.scale)}}, 
						{"rotate", {m_unpack4(tfm.rotate)}},
						{"translate", {m_unpack3(tfm.translate)}}}
					}
				};
			}
			if (flags & entity_component_flag_collision) {
				entity_collision_component *collision_component = entity_get_collision_component(level, i);
				if (collision_component->shape == collision_shape_sphere) {
					entity_json["collision_component"] = {
						{"shape", "sphere"},
						{"radius", collision_component->sphere.radius}
					};
				}
				else if (collision_component->shape == collision_shape_capsule) {
					entity_json["collision_component"] = {
						{"shape", "capsule"},
						{"height", collision_component->capsule.height},
						{"radius", collision_component->capsule.radius}
					};
				}
				else if (collision_component->shape == collision_shape_box) {
					entity_json["collision_component"] = {
						{"shape", "box"},
						{"size", {m_unpack3(collision_component->box.size)}}
					};
				}
				else {
					m_assert(false);
				}
			}
			if (flags & entity_component_flag_physics) {
				entity_physics_component *physics_component = entity_get_physics_component(level, i);
				entity_json["physics_component"] = {
					{"velocity", {m_unpack3(physics_component->velocity)}},
					{"mass", physics_component->mass},
					{"max_speed", physics_component->max_speed}
				};
			}
			if (flags & entity_component_flag_light) {
				entity_light_component *light_component = entity_get_light_component(level, i);
				if (light_component->light_type == light_type_ambient) {
					entity_json["light_component"] = {
						{"light_type", "ambient"},
						{"color", {m_unpack3(light_component->ambient_light.color)}}
					};
				}				
				else if (light_component->light_type == light_type_directional) {
					entity_json["light_component"] = {
						{"light_type", "directional"},
						{"color", {m_unpack3(light_component->directional_light.color)}},
						{"direction", {m_unpack3(light_component->directional_light.direction)}}
					};
				}				
				else if (light_component->light_type == light_type_point) {
					entity_json["light_component"] = {
						{"light_type", "point"},
						{"color", {m_unpack3(light_component->point_light.color)}},
						{"position", {m_unpack3(light_component->point_light.position)}},
						{"attenuation", light_component->point_light.attenuation}
					};
				}				
			}
			entities.push_back(entity_json);
		}
	}
	{ // player
		auto &player = json["player"];
		player = nlohmann::json::object();
		if (level->player_entity_index < level->entity_count) {
			player = {
				{"entity_name", level->entity_infos[level->player_entity_index].name}
			};
		}
	}
	extra_write(json);
	std::string json_string = json.dump(2);

	file_mapping file_mapping = {};
	create_file_mapping(json_file_path, json_string.length(), &file_mapping);
	memcpy(file_mapping.ptr, json_string.c_str(), json_string.length());
	close_file_mapping(file_mapping);
}

camera level_get_player_camera(level *level, vulkan *vulkan, float r, float theta, float phi) {
	vec3 center = {};
	vec3 translate = {};
	if (level->player_entity_index < level->entity_count) {
		transform *transform = &level->entity_transforms[level->player_entity_index];
		center = transform->translate;
		quat rotate_0 = quat_from_rotation(vec3{1, 0, 0}, theta);
		quat rotate_1 = quat_from_rotation(vec3{0, 1, 0}, phi);
		translate = rotate_1 * rotate_0 * vec3{0, 0, -r};
	}
	else {
		center = {0, 0, 0};
		translate = vec3_normalize({0, 1, -1}) * r;
	}
	camera camera = {};
	camera.position = center + translate;
	camera.view = vec3_normalize(-translate);
	camera.up = vec3_cross(vec3_cross(camera.view, vec3{0, 1, 0}), camera.view);
	camera.fovy = degree_to_radian(50);
	camera.aspect = (float)vulkan->swap_chain.image_width / (float)vulkan->swap_chain.image_height;
	camera.znear = 0.1f;
	camera.zfar = 1000;
	return camera;
}

template <typename F>
void traverse_model_node_hierarchy(model_node *nodes, uint32 index, uint32 level, F f) {
	model_node *node = &nodes[index];
	f(node, index);
	for (uint32 i = 0; i < node->child_count; i += 1) {
		traverse_model_node_hierarchy(nodes, node->children[i], level + 1, f);
	}
}

template <typename F>
void traverse_model_scenes(model *model, F f) {
	for (uint32 i = 0; i < model->scene_count; i += 1) {
		model_scene *scene = &model->scenes[i];
		for (uint32 i = 0; i < scene->node_index_count; i += 1) {
			traverse_model_node_hierarchy(model->nodes, scene->node_indices[i], 0, f);
		}
	}
}

template <typename F>
void traverse_model_node_hierarchy_track_global_transform(model_node *nodes, uint32 index, mat4 global_transform_mat, F f) {
	model_node *node = &nodes[index];
	global_transform_mat = global_transform_mat * node->local_transform_mat;
	f(node, index, global_transform_mat);
	for (uint32 i = 0; i < node->child_count; i += 1) {
		traverse_model_node_hierarchy_track_global_transform(nodes, node->children[i], global_transform_mat, f);
	}
}

template <typename F>
void traverse_model_scenes_track_global_transform(model *model, F f) {
	for (uint32 i = 0; i < model->scene_count; i += 1) {
		model_scene *scene = &model->scenes[i];
		for (uint32 i = 0; i < scene->node_index_count; i += 1) {
			traverse_model_node_hierarchy_track_global_transform(model->nodes, scene->node_indices[i], mat4_identity(), f);
		}
	}
}

template <typename F>
void level_generate_render_data(level *level, vulkan *vulkan, camera camera, F generate_extra_render_data) {
	level->render_data = {};

	uint32 *frame_vertex_buffer_offset = &vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index];
	uint32 *frame_uniform_buffer_offset = &vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index];
	uint8 *frame_uniform_buffer_ptr = vulkan->buffers.frame_uniform_buffer_ptrs[vulkan->frame_index];

	*frame_vertex_buffer_offset = 0;
	*frame_uniform_buffer_offset = 0;

	uint32 uniform_alignment = (uint32)vulkan->device.physical_device_properties.limits.minUniformBufferOffsetAlignment;

	{ // scene
		ambient_light ambient_light = {{0, 0, 0}};
		directional_light directional_light = {{0, 0, 0}, {1, 0, 0}};
		point_light point_light = {};
		for (uint32 i = 0; i < level->light_component_count; i += 1) {
			switch (level->light_components[i].light_type) {
				case light_type_ambient: {
					ambient_light = level->light_components[i].ambient_light;
				} break;
				case light_type_directional: {
					directional_light = level->light_components[i].directional_light;
				} break;
				case light_type_point: {
					point_light = level->light_components[i].point_light;
				} break;
			}
		}

		round_up(frame_uniform_buffer_offset, uniform_alignment);
		level->render_data.frame_uniform_buffer_offset = *frame_uniform_buffer_offset;
		shader_level_info *level_info = (struct shader_level_info *)(frame_uniform_buffer_ptr + *frame_uniform_buffer_offset);
		level_info->view_proj_mat = mat4_vulkan_clip() * camera_view_projection_mat4(camera);
		level_info->camera_position = vec4{m_unpack3(camera.position.e), 0};
		struct camera shadow_map_camera = camera;
		shadow_map_camera.zfar = 100;
		level_info->shadow_map_proj_mat = mat4_vulkan_clip() * camera_shadow_map_projection_mat4(shadow_map_camera, directional_light.direction);
		level_info->ambient_light_color = vec4{m_unpack3(ambient_light.color), 0};
		level_info->directional_light_color = vec4{m_unpack3(directional_light.color), 0};
		level_info->directional_light_dir = vec4{m_unpack3(directional_light.direction), 0};
		level_info->point_light_color = vec4{m_unpack3(point_light.color), 0};
		level_info->point_light_position = vec4{m_unpack3(point_light.position), point_light.attenuation};
		*frame_uniform_buffer_offset += sizeof(struct shader_level_info);
	}
	{ // models
		if (level->render_component_count > 0) {
			round_up(frame_uniform_buffer_offset, uniform_alignment);
			level->render_data.models = allocate_memory<struct model_render_data>(&level->render_thread_frame_memory_arena, level->render_component_count);
			level->render_data.model_count = 0;
			for (uint32 i = 0; i < level->entity_count; i += 1) {
				if (level->entity_flags[i] & entity_component_flag_render) {
					entity_render_component *render_component = entity_get_render_component(level, i);
					if (render_component->model_index < level->model_count && !render_component->hide) {
						model_render_data *model_render_data = &level->render_data.models[level->render_data.model_count++];

						model_render_data->model_index = render_component->model_index;
						round_up(frame_uniform_buffer_offset, uniform_alignment);
						model_render_data->frame_uniform_buffer_offset = *frame_uniform_buffer_offset;
						shader_model_info *model_info = (struct shader_model_info *)(frame_uniform_buffer_ptr + *frame_uniform_buffer_offset);
						model_info->model_mat = mat4_from_transform(level->entity_transforms[i]) * mat4_from_transform(render_component->adjustment_transform);
						*frame_uniform_buffer_offset += sizeof(struct shader_model_info);

						model model = level->models[render_component->model_index];
						if (render_component->animation_index < model.animation_count) {
							model_animation *animation = &model.animations[render_component->animation_index];
							model_node *animated_nodes = allocate_memory<struct model_node>(&level->render_thread_frame_memory_arena, model.node_count);
							memcpy(animated_nodes, model.nodes, model.node_count * sizeof(struct model_node));
							transform *animated_node_transforms = allocate_memory<struct transform>(&level->render_thread_frame_memory_arena, model.node_count);
							for (uint32 i = 0; i < model.node_count; i += 1) {
								animated_node_transforms[i] = animated_nodes[i].local_transform;
							}
							for (uint32 i = 0; i < animation->channel_count; i += 1) {
								model_animation_channel *channel = &animation->channels[i];
								model_animation_sampler *sampler = &animation->samplers[channel->sampler_index];
								m_assert(sampler->interpolation_type == gpk_model_animation_linear_interpolation);
								float time = (float)fmod(render_component->animation_time, (double)sampler->key_frames[sampler->key_frame_count - 1].time);
								for (uint32 i = 0; i < sampler->key_frame_count; i += 1) {
									model_animation_key_frame *key_frame = &sampler->key_frames[i];
									if (time <= key_frame->time) {
										if (channel->channel_type == gpk_model_animation_translate_channel) {
											vec3 translate = {};
											if (i == 0) {
												translate = vec3_lerp({0, 0, 0}, {m_unpack3(key_frame->transform_data)}, key_frame->time == 0 ? 1 : time / key_frame->time);
											}
											else {
												model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
												translate = vec3_lerp({m_unpack3(prev_key_frame->transform_data)}, {m_unpack3(key_frame->transform_data)}, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
											}
											animated_node_transforms[channel->node_index].translate = translate;
										}
										else if (channel->channel_type == gpk_model_animation_rotate_channel) {
											quat rotate = {};
											if (i == 0) {
												rotate = quat_slerp({0, 0, 0, 1}, {m_unpack4(key_frame->transform_data)}, key_frame->time == 0 ? 1 : time / key_frame->time);
											}
											else {
												model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
												rotate = quat_slerp({m_unpack4(prev_key_frame->transform_data)}, {m_unpack4(key_frame->transform_data)}, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
											}
											animated_node_transforms[channel->node_index].rotate = rotate;
										}
										else if (channel->channel_type == gpk_model_animation_scale_channel) {
											vec3 scale = {};
											if (i == 0) {
												scale = vec3_lerp({1, 1, 1}, {m_unpack3(key_frame->transform_data)}, key_frame->time == 0 ? 1 : time / key_frame->time);
											}
											else {
												model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
												scale = vec3_lerp({m_unpack3(prev_key_frame->transform_data)}, {m_unpack3(key_frame->transform_data)}, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
											}
											animated_node_transforms[channel->node_index].scale = scale;
										}
										break;
									}
								}
							}
							for (uint32 i = 0; i < model.node_count; i += 1) {
								animated_nodes[i].local_transform_mat = mat4_from_transform(animated_node_transforms[i]);
							}
							model.nodes = animated_nodes;
						}

						uint32 joints_frame_uniform_buffer_offset = 0;
						if (model.skin_count > 0) {
							model_skin *skin = &model.skins[0];
							round_up(frame_uniform_buffer_offset, uniform_alignment);
							joints_frame_uniform_buffer_offset = *frame_uniform_buffer_offset;
							mat4 *joint_mats = (mat4 *)(frame_uniform_buffer_ptr + *frame_uniform_buffer_offset);
							*frame_uniform_buffer_offset += skin->joint_count * sizeof(mat4);
							traverse_model_scenes_track_global_transform(&model, [&](model_node *node, uint32 index, mat4 global_transform) {
								for (uint32 i = 0; i < skin->joint_count; i += 1) {
									if (skin->joints[i].node_index == index) {
										joint_mats[i] = global_transform * skin->joints[i].inverse_bind_mat;
										break;
									}
								}
							});
							for (uint32 i = 0; i < skin->joint_count; i += 1) {
								m_assert(joint_mats[i] != mat4{});
							}
						}

						model_render_data->mesh_count = 0;
						traverse_model_scenes(&model, [&](model_node *node, uint32 index) {
							if (node->mesh_index < model.mesh_count) {
								model_render_data->mesh_count += 1;
							}
						});
						model_render_data->meshes = allocate_memory<struct mesh_render_data>(&level->render_thread_frame_memory_arena, model_render_data->mesh_count);
						uint32 mesh_render_data_index = 0;
						traverse_model_scenes_track_global_transform(&model, [&](model_node *node, uint32 index, mat4 global_transform) {
							if (node->mesh_index < model.mesh_count) {
								mesh_render_data *mesh_render_data = &model_render_data->meshes[mesh_render_data_index++];
								mesh_render_data->mesh_index = node->mesh_index;
								if (model.skin_count > 0) {
									mesh_render_data->frame_uniform_buffer_offset = joints_frame_uniform_buffer_offset;
								}
								else {
									round_up(frame_uniform_buffer_offset, uniform_alignment);
									mesh_render_data->frame_uniform_buffer_offset = *frame_uniform_buffer_offset;
									*(mat4 *)(frame_uniform_buffer_ptr + *frame_uniform_buffer_offset) = global_transform;
									*frame_uniform_buffer_offset += sizeof(mat4);
								}
								model_mesh *mesh = &model.meshes[node->mesh_index];
								mesh_render_data->primitives = allocate_memory<struct primitive_render_data>(&level->render_thread_frame_memory_arena, mesh->primitive_count);
								for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
									primitive_render_data *primitive_render_data = &mesh_render_data->primitives[i];
									primitive_render_data->primitive_index = i;

									model_mesh_primitive *primitive = &mesh->primitives[i];
									model_material *material = &model.materials[primitive->material_index];

									round_up(frame_uniform_buffer_offset, uniform_alignment);
									primitive_render_data->frame_uniform_buffer_offset = *frame_uniform_buffer_offset;
									shader_primitive_info *primitive_info = (shader_primitive_info *)(frame_uniform_buffer_ptr + *frame_uniform_buffer_offset);
									primitive_info->diffuse_factor = material->diffuse_factor;
									primitive_info->metallic_factor = material->metallic_factor;
									primitive_info->roughness_factor = material->roughness_factor;
									*frame_uniform_buffer_offset += sizeof(struct shader_primitive_info);
								}
							}
						});
					}
				}
			}
		}
	}
	generate_extra_render_data();
}

template <typename F0, typename F1>
void level_generate_render_commands(level *level, vulkan *vulkan, const camera &camera, F0 extra_main_render_pass_render_commands, F1 extra_swap_chain_render_commands) {
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];
	{ // shadow passes
		{
			VkClearValue clear_values[] = {{1, 1, 0, 0}, {1, 0}};
			VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
			render_pass_begin_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
			render_pass_begin_info.framebuffer = vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].framebuffers[0];
			render_pass_begin_info.renderArea.offset = {0, 0};
			render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].width, vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].height};
			render_pass_begin_info.clearValueCount = m_countof(clear_values);
			render_pass_begin_info.pClearValues = clear_values;
			vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
			VkViewport viewport = {0, 0, (float)vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].width, (float)vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].height, 0, 1};
			VkRect2D scissor = {{0, 0}, vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].width, vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].height};
			vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
			vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
			if (level->render_data.model_count > 0) {
				vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_shadow_map_pipeline.pipeline);
				VkDeviceSize vertex_buffer_offset = 0;
				vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.level_vertex_buffer.buffer, &vertex_buffer_offset);
				vkCmdBindIndexBuffer(cmd_buffer, vulkan->buffers.level_vertex_buffer.buffer, 0, VK_INDEX_TYPE_UINT16);
				for (auto &model_render_data : make_range(level->render_data.models, level->render_data.model_count)) {
					model &model = level->models[model_render_data.model_index];
					for (auto &mesh_render_data : make_range(model_render_data.meshes, model_render_data.mesh_count)) {
						model_mesh &mesh = model.meshes[mesh_render_data.mesh_index];
						for (auto &primitive_render_data : make_range(mesh_render_data.primitives, mesh.primitive_count)) {
							model_mesh_primitive &primitive = mesh.primitives[primitive_render_data.primitive_index];
							uint32 offsets[4] = {
								level->render_data.frame_uniform_buffer_offset, model_render_data.frame_uniform_buffer_offset, 
								mesh_render_data.frame_uniform_buffer_offset, primitive_render_data.frame_uniform_buffer_offset
							};
							vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(offsets), offsets);
							vkCmdDrawIndexed(cmd_buffer, primitive.index_count, 1, primitive.index_buffer_offset / sizeof(uint16), primitive.vertex_buffer_offset / sizeof(struct gpk_model_vertex), 0);
						}
					}
				}
			}
			vkCmdEndRenderPass(cmd_buffer);
		}
		{
			VkClearValue clear_values[] = {{0, 0, 0, 0}};
			VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
			render_pass_begin_info.renderPass = vulkan->render_passes.shadow_map_render_passes[1];
			render_pass_begin_info.framebuffer = vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].framebuffers[1];
			render_pass_begin_info.renderArea.offset = {0, 0};
			render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].width, vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].height};
			render_pass_begin_info.clearValueCount = m_countof(clear_values);
			render_pass_begin_info.pClearValues = clear_values;
			vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0].pipeline);
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0].layout, 0, 1, &vulkan->descriptors.shadow_map_framebuffer_textures[vulkan->frame_index][0], 0, nullptr);
		  struct {
		  	float x_dir, y_dir;
		  } push_const = {1, 0};
		  vkCmdPushConstants(cmd_buffer, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0].layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(push_const), &push_const);
			vkCmdDraw(cmd_buffer, 3, 1, 0, 0);
			vkCmdEndRenderPass(cmd_buffer);
		}
		{
			VkClearValue clear_values[] = {{0, 0, 0, 0}};
			VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
			render_pass_begin_info.renderPass = vulkan->render_passes.shadow_map_render_passes[2];
			render_pass_begin_info.framebuffer = vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].framebuffers[2];
			render_pass_begin_info.renderArea.offset = {0, 0};
			render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].width, vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].height};
			render_pass_begin_info.clearValueCount = m_countof(clear_values);
			render_pass_begin_info.pClearValues = clear_values;
			vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[1].pipeline);
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[1].layout, 0, 1, &vulkan->descriptors.shadow_map_framebuffer_textures[vulkan->frame_index][1], 0, nullptr);
		  struct {
		  	float x_dir, y_dir;
		  } push_const = {0, 1};
		  vkCmdPushConstants(cmd_buffer, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[1].layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(push_const), &push_const);
			vkCmdDraw(cmd_buffer, 3, 1, 0, 0);
			vkCmdEndRenderPass(cmd_buffer);
		}
	}
	{ // main render pass
		VkClearValue clear_values[2] = {{0, 0, 0, 1}, {0, 0}};
		VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
		render_pass_begin_info.renderPass = vulkan->render_passes.main_render_pass;
		render_pass_begin_info.framebuffer = vulkan->framebuffers.main_framebuffers[vulkan->frame_index].framebuffer;
		render_pass_begin_info.renderArea.offset = {0, 0};
		render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.main_framebuffers[vulkan->frame_index].width, vulkan->framebuffers.main_framebuffers[vulkan->frame_index].height};
		render_pass_begin_info.clearValueCount = m_countof(clear_values);
		render_pass_begin_info.pClearValues = clear_values;
		vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		VkViewport viewport = {0, 0, (float)vulkan->framebuffers.main_framebuffers[vulkan->frame_index].width, (float)vulkan->framebuffers.main_framebuffers[vulkan->frame_index].height, 1, 0};
		VkRect2D scissor = {{0, 0}, vulkan->framebuffers.main_framebuffers[vulkan->frame_index].width, vulkan->framebuffers.main_framebuffers[vulkan->frame_index].height};
		vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
		vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
		if (level->render_data.model_count > 0) {
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.pipeline);
			VkDeviceSize vertex_buffer_offset = 0;
			vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.level_vertex_buffer.buffer, &vertex_buffer_offset);
			vkCmdBindIndexBuffer(cmd_buffer, vulkan->buffers.level_vertex_buffer.buffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 2, 1, &vulkan->descriptors.shadow_map_framebuffer_textures[vulkan->frame_index][2], 0, nullptr);
			for (auto &model_render_data : make_range(level->render_data.models, level->render_data.model_count)) {
				model &model = level->models[model_render_data.model_index];
				for (auto &mesh_render_data : make_range(model_render_data.meshes, model_render_data.mesh_count)) {
					model_mesh &mesh = model.meshes[mesh_render_data.mesh_index];
					for (auto &primitive_render_data : make_range(mesh_render_data.primitives, mesh.primitive_count)) {
						model_mesh_primitive &primitive = mesh.primitives[primitive_render_data.primitive_index];
						uint32 offsets[4] = {
							level->render_data.frame_uniform_buffer_offset, model_render_data.frame_uniform_buffer_offset, 
							mesh_render_data.frame_uniform_buffer_offset, primitive_render_data.frame_uniform_buffer_offset
						};
						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(offsets), offsets);
						if (primitive.material_index < model.material_count) {
							vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 1, 1, &model.materials[primitive.material_index].textures_descriptor_set, 0, nullptr);
						}
						else {
							vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 1, 1, &vulkan->descriptors.model_default_material_textures, 0, nullptr);
						}
						vkCmdDrawIndexed(cmd_buffer, primitive.index_count, 1, primitive.index_buffer_offset / sizeof(uint16), primitive.vertex_buffer_offset / sizeof(struct gpk_model_vertex), 0);
					}
				}
			}
		}
		if (level->terrain_index < level->terrain_count) {
			terrain *terrain = &level->terrains[level->terrain_index];
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.terrain_pipeline.pipeline);
			VkDeviceSize vertex_buffer_offset = 0;
			vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.common_vertex_buffer.buffer, &vertex_buffer_offset);
			uint32 offsets[4] = {level->render_data.frame_uniform_buffer_offset, 0, 0, 0};
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.terrain_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(offsets), offsets);
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.terrain_pipeline.layout, 1, 1, &terrain->textures_descriptor_set, 0, nullptr);
			vkCmdDraw(cmd_buffer, 128 * 128 * 6, 1, level->terrain_common_vertex_buffer_offset / sizeof(struct terrain_vertex), 0);
		}
		if (level->skybox_index < level->skybox_count) {
		  skybox *skybox = &level->skyboxes[level->skybox_index];
		  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.skybox_pipeline.pipeline);
		  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.skybox_pipeline.layout, 0, 1, &skybox->cubemap_descriptor_set, 0, nullptr);
		  struct camera skybox_camera = camera;
		  skybox_camera.position = {0, 0, 0};
		  struct {
		  	mat4 camera_view_proj_mat;
		  } push_const = {mat4_vulkan_clip() * camera_view_projection_mat4(skybox_camera)};
		  vkCmdPushConstants(cmd_buffer, vulkan->pipelines.skybox_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(push_const), &push_const);
		  vkCmdDraw(cmd_buffer, 36, 1, 0, 0);
		}
		extra_main_render_pass_render_commands();
		vkCmdEndRenderPass(cmd_buffer);
	}
	{ // swap chain render pass
		VkClearValue clear_value = {0, 0, 0, 1};
		VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
		render_pass_begin_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
		render_pass_begin_info.framebuffer = vulkan->framebuffers.swap_chain_framebuffers[vulkan->swap_chain_image_index];
		render_pass_begin_info.renderArea.offset = {0, 0};
		render_pass_begin_info.renderArea.extent = {vulkan->swap_chain.image_width, vulkan->swap_chain.image_height};
		render_pass_begin_info.clearValueCount = 1;
		render_pass_begin_info.pClearValues = &clear_value;
		vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		VkViewport viewport = {0, 0, (float)vulkan->swap_chain.image_width, (float)vulkan->swap_chain.image_height, 0, 1};
		VkRect2D scissor = {{0, 0}, {vulkan->swap_chain.image_width, vulkan->swap_chain.image_height}};
		vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
		vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
		{
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.swap_chain_pipeline.pipeline);
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.swap_chain_pipeline.layout, 0, 1, &vulkan->descriptors.main_framebuffer_textures[vulkan->frame_index], 0, nullptr);
			vkCmdDraw(cmd_buffer, 3, 1, 0, 0);
		}
		extra_swap_chain_render_commands();
		vkCmdEndRenderPass(cmd_buffer);
	}
}	

#if 0
{ // texts
	struct vertex {
		vec4 position_uv;
		u8vec4 color;
		uint32 transform_mat_index;
	};
	static_assert(sizeof(struct vertex) == 24, "");
	round_up(&vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index], (uint32)sizeof(struct vertex));
	round_up(&vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], uniform_alignment);
	level->render_data.text_frame_vertex_buffer_offset = vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index];
	level->render_data.text_frame_uniform_buffer_offset = vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index];
	level->render_data.text_frame_vertex_count = 0;
	level->render_data.text_frame_uniform_count = 0;
	auto append_text_render_data = [level, vulkan, font, camera](const char *text, aa_bound bound) {
		vec4 text_quad = {};
		float xpos = 0;
		float ypos = 0;
		uint32 text_len = (uint32)strlen(text);
		for (uint32 i = 0; i < text_len; i += 1) {
			stbtt_aligned_quad quad = {};
			stbtt_GetPackedQuad(font->stbtt_packed_chars, font->packed_bitmap_width, font->packed_bitmap_height, text[i] - ' ', &xpos, &ypos, &quad, 0);
			quad.y0 = -quad.y0;
			quad.y1 = -quad.y1;
			text_quad.x0 = min(text_quad.x0, quad.x0);
			text_quad.y0 = max(text_quad.y0, quad.y0);
			text_quad.x1 = max(text_quad.x1, quad.x1);
			text_quad.y1 = min(text_quad.y1, quad.y1);
			vertex *vertices = (vertex *)(vulkan->buffers.frame_vertex_buffer_ptrs[vulkan->frame_index] + vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] + sizeof(struct vertex) * 6 * i);
			vertices[0] = {{quad.x0, quad.y0, quad.s0, quad.t0}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
			vertices[1] = {{quad.x0, quad.y1, quad.s0, quad.t1}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
			vertices[2] = {{quad.x1, quad.y1, quad.s1, quad.t1}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
			vertices[3] = vertices[0];
			vertices[4] = vertices[2];
			vertices[5] = {{quad.x1, quad.y0, quad.s1, quad.t0}, {0, 255, 0, 255}, level->render_data.text_frame_uniform_count};
		}
		vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] += sizeof(struct vertex) * 6 * text_len;

		vec2 text_quad_center = {};
		text_quad_center.x = text_quad.x0 + (text_quad.x1 - text_quad.x0) / 2;
		text_quad_center.y = text_quad.y0 + (text_quad.y1 - text_quad.y0) / 2;

		float text_quad_scale = 1 / font->font_size;
		float text_quad_height = text_quad.y0 - text_quad.y1;
		vec3 text_quad_translate = {0, text_quad_height / 2 * text_quad_scale, 0};
		text_quad_translate = text_quad_translate + aa_bound_center(bound);
		text_quad_translate.y += (bound.max.y - bound.min.y) / 2;

		mat4 transform_mat = mat4_vulkan_clip() * camera_view_projection_mat4(camera) * mat4_from_translation(text_quad_translate) * camera_billboard_mat4(camera) * mat4_from_scaling(text_quad_scale) * mat4_from_translation({(-text_quad_center).x, (-text_quad_center).y, 0});
		*(mat4 *)(vulkan->buffers.frame_uniform_buffer_ptrs[vulkan->frame_index] + vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index]) = transform_mat;
		vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index] += sizeof(mat4);

		level->render_data.text_frame_vertex_count += text_len * 6;
		level->render_data.text_frame_uniform_count += 1;
	};
	for (uint32 i = 0; i < level->entity_count; i += 1) {
		append_text_render_data(level->entity_infos[i].name, aa_bound{{-1, -1, -1}, {1, 1, 1}});
	}
}
#endif
