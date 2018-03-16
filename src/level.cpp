/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define RAPIDJSON_SSE2
#define RAPIDJSON_ASSERT(x) m_assert(x)
#include "../vendor/include/rapidjson/document.h"
#include "../vendor/include/rapidjson/prettywriter.h"
#include "../vendor/include/rapidjson/error/en.h"

#include "../vendor/include/bullet/btBulletCollisionCommon.h"
#include "../vendor/include/bullet/btBulletDynamicsCommon.h"

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
	mat4 local_transform_mat;
	uint32 children[32];
	uint32 child_count;
};

struct model_mesh {
	uint32 material_index;
	uint32 index_count;
	uint32 index_buffer_offset;
	uint32 vertex_size;
	uint32 vertex_count;
	uint32 vertex_buffer_offset;
	uint8 *vertices_data;
	uint8 *indices_data;
	char name[sizeof(gpk_model_mesh::name)];
};

struct model_material {
	VkDescriptorSet textures_descriptor_set;
	vec4 albedo_factor;
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
	model_material *materials;
	uint32 material_count;
	model_image *images;
	uint32 image_count;
	char gpk_file[128];
};

struct skybox {
	VkDescriptorSet descriptor_set;
	vulkan_image cubemap_image;
	char file_name[32];
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
	entity_component_flag_render    = 1 << 0,
	entity_component_flag_collision = 1 << 1,
	entity_component_flag_light     = 1 << 2,
	entity_component_flag_physics   = 1 << 3
};

struct entity_render_component {
	uint32 model_index;
	transform model_adjustment_transform;
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

struct entity_light_component {
	light_type light_type;
	union {
		ambient_light ambient_light;
		directional_light directional_light;
		point_light point_light;
	};
};

struct entity_physics_component {
	vec3 velocity;
	float mass;
	float max_speed;
	btRigidBody *bt_rigid_body;
};

struct entity_modification {
	bool remove;
	bool remove_render_component;
	bool remove_collision_component;
	bool remove_light_component;
	bool remove_physics_component;
	uint32 *flag;
	entity_info *info;
	transform *transform;
	entity_render_component *render_component;
	entity_collision_component *collision_component;
	entity_light_component *light_component;
	entity_physics_component *physics_component;
};

struct entity_addition {
	uint32 flag;
	entity_info info;
	transform transform;
	entity_render_component *render_component;
	entity_collision_component *collision_component;
	entity_light_component *light_component;
	entity_physics_component *physics_component;
	entity_addition *next;
};

struct mesh_render_data {
	uint32 mesh_index;
	uint32 frame_uniform_buffer_offset;
 	bool render_vertices_outline;
};

struct model_render_data {
	uint32 model_index;
	mesh_render_data *meshes;
	uint32 mesh_count;
};

struct level_render_data {
	uint32 common_data_frame_uniform_buffer_offset;
	uint32 text_frame_vertex_buffer_offset;
	uint32 text_frame_uniform_buffer_offset;
	uint32 text_frame_vertex_count;
	uint32 text_frame_uniform_count;
	model_render_data *models;
	uint32 model_count;
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

	level_render_data render_data;
	bool show_frame_stats;

	memory_arena main_thread_frame_memory_arena;
	memory_arena render_thread_frame_memory_arena;
	memory_arena entity_components_memory_arenas[2];
	uint32 entity_components_memory_arena_index;
	memory_arena assets_memory_arena;

	char *json_file;
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
		level->entity_components_memory_arenas[i].capacity = m_megabytes(8);
		level->entity_components_memory_arenas[i].memory = allocate_virtual_memory(level->entity_components_memory_arenas[i].capacity);
		m_assert(level->entity_components_memory_arenas[i].memory);
	}

	level->assets_memory_arena.name = "assets";
	level->assets_memory_arena.capacity = m_megabytes(64);
	level->assets_memory_arena.memory = allocate_virtual_memory(level->assets_memory_arena.capacity);
	m_assert(level->assets_memory_arena.memory);

	level->model_capacity = 1024;
	level->model_count = 0;
	level->models = allocate_memory<struct model>(&level->assets_memory_arena, level->model_capacity);

	level->skybox_capacity = 16;
	level->skybox_count = 0;
	level->skyboxes = allocate_memory<struct skybox>(&level->assets_memory_arena, level->skybox_capacity);
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

void level_process_entity_modifications_additions(level *level) {
	uint32 new_entity_count = level->entity_count;
	uint32 new_render_component_count = level->render_component_count;
	uint32 new_collision_component_count = level->collision_component_count;
	uint32 new_light_component_count = level->light_component_count;
	uint32 new_physics_component_count = level->physics_component_count;
	{
		for (uint32 i = 0; i < level->entity_count; i += 1) {
			uint32 entity_flag = level->entity_flags[i];
			entity_modification *em = &level->entity_modifications[i];
			if (em->remove) {
				new_entity_count -= 1;
				if (entity_flag & entity_component_flag_render) {
					new_render_component_count -= 1;
				}
				if (entity_flag & entity_component_flag_collision) {
					new_collision_component_count -= 1;
				}
				if (entity_flag & entity_component_flag_light) {
					new_light_component_count -= 1;
				}
				if (entity_flag & entity_component_flag_physics) {
					new_physics_component_count -= 1;
				}
			}
			else {
				if (em->remove_render_component) {
					new_render_component_count -= 1;
				}
				else if (em->render_component && !(entity_flag & entity_component_flag_render)) {
					new_render_component_count += 1;
				}
				if (em->remove_collision_component) {
					new_collision_component_count -= 1;
				}
				else if (em->collision_component && !(entity_flag & entity_component_flag_collision)) {
					new_collision_component_count += 1;
				}
				if (em->remove_light_component) {
					new_light_component_count -= 1;
				}
				else if (em->light_component && !(entity_flag & entity_component_flag_light)) {
					new_light_component_count += 1;
				}
				if (em->remove_physics_component) {
					new_physics_component_count -= 1;
				}
				else if (em->physics_component && !(entity_flag & entity_component_flag_physics)) {
					new_physics_component_count += 1;
				}
			}
		}
		entity_addition *ea = level->entity_addition;
		while (ea) {
			new_entity_count += 1;
			if (ea->render_component) {
				new_render_component_count += 1;
			}
			if (ea->collision_component) {
				new_collision_component_count += 1;
			}
			if (ea->light_component) {
				new_light_component_count += 1;
			}
			if (ea->physics_component) {
				new_physics_component_count += 1;
			}
			ea = ea->next;
		}
	}

	level->entity_components_memory_arena_index = (level->entity_components_memory_arena_index + 1) % 2;
	memory_arena *entity_components_memory_arena = &level->entity_components_memory_arenas[level->entity_components_memory_arena_index];
	entity_components_memory_arena->size = 0;

	uint32 *new_entity_flags = allocate_memory<uint32>(entity_components_memory_arena, new_entity_count);
	entity_info *new_entity_infos = allocate_memory<entity_info>(entity_components_memory_arena, new_entity_count);
	transform *new_entity_transforms = allocate_memory<transform>(entity_components_memory_arena, new_entity_count);
	entity_modification *new_entity_modifications = allocate_memory<entity_modification>(entity_components_memory_arena, new_entity_count);
	entity_render_component *new_render_components = allocate_memory<entity_render_component>(entity_components_memory_arena, new_render_component_count);
	entity_collision_component *new_collision_components = allocate_memory<entity_collision_component>(entity_components_memory_arena, new_collision_component_count);
	entity_light_component *new_light_components = allocate_memory<entity_light_component>(entity_components_memory_arena, new_light_component_count);
	entity_physics_component *new_physics_components = allocate_memory<entity_physics_component>(entity_components_memory_arena, new_physics_component_count);

	uint32 entity_index = 0;
	uint32 render_component_index = 0;
	uint32 collision_component_index = 0;
	uint32 light_component_index = 0;
	uint32 physics_component_index = 0;
	for (uint32 i = 0; i < level->entity_count; i += 1) {
		entity_modification *em = &level->entity_modifications[i];
		if (!em->remove) {
			new_entity_flags[entity_index] = em->flag ? *em->flag : level->entity_flags[i];
			new_entity_infos[entity_index] = em->info ? *em->info : level->entity_infos[i];
			new_entity_transforms[entity_index] = em->transform ? *em->transform : level->entity_transforms[i];
			if (!em->remove_render_component) {
				if (level->entity_flags[i] & entity_component_flag_render || em->render_component) {
					new_render_components[render_component_index++] = em->render_component ? *em->render_component : *entity_get_render_component(level, i);
				}
			}
			if (!em->remove_collision_component) {
				if (level->entity_flags[i] & entity_component_flag_collision || em->collision_component) {
					new_collision_components[collision_component_index++] = em->collision_component ? *em->collision_component : *entity_get_collision_component(level, i);
				}
			}
			if (!em->remove_light_component) {
				if (level->entity_flags[i] & entity_component_flag_light || em->light_component) {
					new_light_components[light_component_index++] = em->light_component ? *em->light_component : *entity_get_light_component(level, i);
				}
			}
			if (!em->remove_physics_component) {
				if (level->entity_flags[i] & entity_component_flag_physics || em->physics_component) {
					new_physics_components[physics_component_index++] = em->physics_component ? *em->physics_component : *entity_get_physics_component(level, i);
				}
			}
			entity_index += 1;
		}
	}
	entity_addition *ea = level->entity_addition;
	while (ea) {
		new_entity_flags[entity_index] = ea->flag;
		new_entity_infos[entity_index] = ea->info;
		new_entity_transforms[entity_index] = ea->transform;
		if (ea->render_component) {
			new_render_components[render_component_index++] = *ea->render_component;
		}
		if (ea->collision_component) {
			new_collision_components[collision_component_index++] = *ea->collision_component;
		}
		if (ea->light_component) {
			new_light_components[light_component_index++] = *ea->light_component;
		}
		if (ea->physics_component) {
			new_physics_components[physics_component_index++] = *ea->physics_component;
		}
		entity_index += 1;
		ea = ea->next;
	}

	level->entity_flags = new_entity_flags;
	level->entity_infos = new_entity_infos;
	level->entity_transforms = new_entity_transforms;
	level->entity_count = new_entity_count;

	level->entity_modifications = new_entity_modifications;
	level->entity_addition = nullptr;

	level->render_components = new_render_components;
	level->collision_components = new_collision_components;
	level->light_components = new_light_components;
	level->physics_components = new_physics_components;

	level->render_component_count = new_render_component_count;
	level->collision_component_count = new_collision_component_count;
	level->light_component_count = new_light_component_count;
	level->physics_component_count = new_physics_component_count;
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
	snprintf(model->gpk_file, sizeof(model->gpk_file), "%s", gpk_file);
	model->scene_count = gpk_model->scene_count;
	model->node_count = gpk_model->node_count;
	model->mesh_count = gpk_model->mesh_count;
	model->material_count = gpk_model->material_count;
	model->image_count = gpk_model->image_count;
	model->scenes = allocate_memory<struct model_scene>(&level->assets_memory_arena, model->scene_count);
	model->nodes = allocate_memory<struct model_node>(&level->assets_memory_arena, model->node_count);
	model->meshes = allocate_memory<struct model_mesh>(&level->assets_memory_arena, model->mesh_count);
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
		model_node->local_transform_mat = gpk_model_node->local_transform_mat;
		array_copy(model_node->children, gpk_model_node->children);
		model_node->child_count = gpk_model_node->child_count;
	}
	for (uint32 i = 0; i < model->mesh_count; i += 1) {
		gpk_model_mesh *gpk_model_mesh = ((struct gpk_model_mesh*)(gpk_file_mapping.ptr + gpk_model->mesh_offset)) + i;
		model_mesh *model_mesh = &model->meshes[i];
		model_mesh->material_index = gpk_model_mesh->material_index;
		array_copy(model_mesh->name, gpk_model_mesh->name);

		uint32 indices_size = gpk_model_mesh->index_count * sizeof(uint16);
		uint32 vertices_size = gpk_model_mesh->vertex_size * gpk_model_mesh->vertex_count;
		uint8 *indices_data = gpk_file_mapping.ptr + gpk_model_mesh->indices_offset;
		uint8 *vertices_data = gpk_file_mapping.ptr + gpk_model_mesh->vertices_offset;
			
		model_mesh->index_count = gpk_model_mesh->index_count;
		round_up(&vulkan->buffers.level_vertex_buffer_offset, (uint32)sizeof(uint16));
		model_mesh->index_buffer_offset = vulkan->buffers.level_vertex_buffer_offset;
		vulkan->buffers.level_vertex_buffer_offset += indices_size;

		model_mesh->vertex_count = gpk_model_mesh->vertex_count;
		model_mesh->vertex_size = gpk_model_mesh->vertex_size;
		round_up(&vulkan->buffers.level_vertex_buffer_offset, model_mesh->vertex_size);
		model_mesh->vertex_buffer_offset = vulkan->buffers.level_vertex_buffer_offset;
		vulkan->buffers.level_vertex_buffer_offset += vertices_size;

		vulkan_buffer_transfer(vulkan, &vulkan->buffers.level_vertex_buffer, model_mesh->index_buffer_offset, indices_data, indices_size);
		vulkan_buffer_transfer(vulkan, &vulkan->buffers.level_vertex_buffer, model_mesh->vertex_buffer_offset, vertices_data, vertices_size);
		if (store_vertices) {
			model_mesh->indices_data = allocate_memory<uint8>(&level->assets_memory_arena, indices_size);
			model_mesh->vertices_data = allocate_memory<uint8>(&level->assets_memory_arena, vertices_size);
			memcpy(model_mesh->indices_data, indices_data, indices_size);
			memcpy(model_mesh->vertices_data, vertices_data, vertices_size);
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
		model_material->albedo_factor = gpk_model_material->albedo_factor;
		model_material->metallic_factor = gpk_model_material->metallic_factor;
		model_material->roughness_factor = gpk_model_material->roughness_factor;

		VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
		descriptor_set_allocate_info.descriptorPool = vulkan->descriptors.pool;
		descriptor_set_allocate_info.descriptorSetCount = 1;
		descriptor_set_allocate_info.pSetLayouts = &vulkan->descriptors.sampled_images_set_layouts[4];
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &model_material->textures_descriptor_set));
		VkDescriptorImageInfo descriptor_image_infos[5] = {};
		if (gpk_model_material->albedo_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->albedo_image_index].image;
			descriptor_image_infos[0] = {vulkan->samplers.mipmap_image_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[0] = {vulkan->samplers.mipmap_image_samplers[1], vulkan->images.default_albedo_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		if (gpk_model_material->metallic_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->metallic_image_index].image;
			descriptor_image_infos[1] = {vulkan->samplers.mipmap_image_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[1] = {vulkan->samplers.mipmap_image_samplers[1], vulkan->images.default_metallic_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		if (gpk_model_material->roughness_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->roughness_image_index].image;
			descriptor_image_infos[2] = {vulkan->samplers.mipmap_image_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[2] = {vulkan->samplers.mipmap_image_samplers[1], vulkan->images.default_roughness_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		if (gpk_model_material->normal_image_index < model->image_count) {
			vulkan_image *image = &model->images[gpk_model_material->normal_image_index].image;
			descriptor_image_infos[3] = {vulkan->samplers.mipmap_image_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		else {
			descriptor_image_infos[3] = {vulkan->samplers.mipmap_image_samplers[1], vulkan->images.default_normal_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		}
		// if (gpk_model_material->height_image_index < model->image_count) {
		// 	vulkan_image *image = &model->images[gpk_model_material->height_image_index].image;
		// 	descriptor_image_infos[4] = {vulkan->samplers.mipmap_image_samplers[image->mipmap_count], image->view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		// }
		// else {
		// 	descriptor_image_infos[4] = {vulkan->samplers.mipmap_image_samplers[1], vulkan->images.default_height_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		// }
		descriptor_image_infos[4] = {vulkan->samplers.mipmap_image_samplers[1], vulkan->images.default_height_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
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

void level_add_skybox(level *level, vulkan *vulkan, const char *file_name) {
	m_assert(level->skybox_count < level->skybox_capacity);
	skybox *skybox = &level->skyboxes[level->skybox_count++];
	snprintf(skybox->file_name, sizeof(skybox->file_name), "%s", file_name);

	file_mapping file_mapping = {};
	m_assert(open_file_mapping(file_name, &file_mapping));
	m_scope_exit(close_file_mapping(file_mapping));
	gpk_skybox *gpk_skybox = (struct gpk_skybox *)file_mapping.ptr;

	VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
	image_create_info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	image_create_info.imageType = VK_IMAGE_TYPE_2D;
	image_create_info.format = VK_FORMAT_R8G8B8A8_SRGB;
	image_create_info.extent = {gpk_skybox->cubemap_width, gpk_skybox->cubemap_height, 1};
	image_create_info.mipLevels = 1;
	image_create_info.arrayLayers = 6;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	image_view_create_info.format = image_create_info.format;
	image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_create_info.subresourceRange.levelCount = 1;
	image_view_create_info.subresourceRange.layerCount = 6;
	uint8 *cubemap = (uint8 *)gpk_skybox + gpk_skybox->cubemap_offset;
	uint32 cubemap_size = gpk_skybox->cubemap_width * gpk_skybox->cubemap_height * 4 * 6;
	skybox->cubemap_image = allocate_vulkan_image(&vulkan->device, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, 1, 4);
	vulkan_image_transfer(&vulkan->device, &vulkan->cmd_buffers, &skybox->cubemap_image, cubemap, cubemap_size);

	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
	descriptor_set_allocate_info.descriptorPool = vulkan->descriptors.pool;
	descriptor_set_allocate_info.descriptorSetCount = 1;
	descriptor_set_allocate_info.pSetLayouts = &vulkan->descriptors.sampled_images_set_layouts[0];
	m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &skybox->descriptor_set));
	VkDescriptorImageInfo descriptor_image_info = {vulkan->samplers.skybox_cubemap_sampler, skybox->cubemap_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
	VkWriteDescriptorSet write_descriptor_set = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
	write_descriptor_set.dstSet = skybox->descriptor_set;
	write_descriptor_set.dstBinding = 0;
	write_descriptor_set.descriptorCount = 1;
	write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write_descriptor_set.pImageInfo = &descriptor_image_info;
	vkUpdateDescriptorSets(vulkan->device.device, 1, &write_descriptor_set, 0, nullptr);
}

template <typename T>
void level_read_json(level *level, vulkan *vulkan, const char *level_json_file, T extra_load, bool store_vertices = false) {
	file_mapping level_json_file_mapping = {};
	m_assert(open_file_mapping(level_json_file, &level_json_file_mapping));
	m_scope_exit(close_file_mapping(level_json_file_mapping));
	level->json_file = allocate_memory<char>(&level->assets_memory_arena, strlen(level_json_file) + 1);
	strcpy(level->json_file, level_json_file);

	rapidjson::Document json_doc;
	rapidjson::ParseResult json_parse_result = json_doc.Parse((const char *)level_json_file_mapping.ptr);
	if (!json_parse_result) {
    fatal("JSON parse error: %s (%u)", rapidjson::GetParseError_En(json_parse_result.Code()), json_parse_result.Offset());		
	}

	auto read_json_transform_object = [](rapidjson::Value::Object json_transform, transform *transform) {
		rapidjson::Value::Array json_transform_scaling = json_transform["scale"].GetArray();
		rapidjson::Value::Array json_transform_rotation = json_transform["rotate"].GetArray();
		rapidjson::Value::Array json_transform_translation = json_transform["translate"].GetArray();
		transform->scale.x = json_transform_scaling[0].GetFloat();
		transform->scale.y = json_transform_scaling[1].GetFloat();
		transform->scale.z = json_transform_scaling[2].GetFloat();
		transform->rotate.x = json_transform_rotation[0].GetFloat();
		transform->rotate.y = json_transform_rotation[1].GetFloat();
		transform->rotate.z = json_transform_rotation[2].GetFloat();
		transform->rotate.w = json_transform_rotation[3].GetFloat();
		transform->translate.x = json_transform_translation[0].GetFloat();
		transform->translate.y = json_transform_translation[1].GetFloat();
		transform->translate.z = json_transform_translation[2].GetFloat();
	};
	auto read_json_render_component = [&](rapidjson::Value::Object render_component_json, entity_render_component *render_component) {
		render_component->model_index = level_get_model_index(level, render_component_json["model_file_name"].GetString());
		auto model_adjustment_transform_member = render_component_json.FindMember("model_adjustment_transform");
		if (model_adjustment_transform_member != render_component_json.MemberEnd()) {
			read_json_transform_object(model_adjustment_transform_member->value.GetObject(), &render_component->model_adjustment_transform);
		}
		else {
			render_component->model_adjustment_transform = transform_identity();
		}
	};
	auto read_json_collision_component = [&](rapidjson::Value::Object collision_component_json, entity_collision_component *entity_collision_component) {
		memory_arena *memory_arena = &level->entity_components_memory_arenas[level->entity_components_memory_arena_index];
		const char *shape = collision_component_json["type"].GetString();
		if (!strcmp(shape, "sphere")) {
			entity_collision_component->shape = collision_shape_sphere;
			float radius = collision_component_json["radius"].GetFloat();
			entity_collision_component->sphere = {radius};
		}
		else if (!strcmp(shape, "capsule")) {
			entity_collision_component->shape = collision_shape_capsule;
			float height = collision_component_json["height"].GetFloat();
			float radius = collision_component_json["radius"].GetFloat();
			entity_collision_component->capsule.height = height;
			entity_collision_component->capsule.radius = radius;
		}
		else if (!strcmp(shape, "box")) {
			entity_collision_component->shape = collision_shape_box;
			rapidjson::Value::Array size = collision_component_json["size"].GetArray();
			entity_collision_component->box.size = {size[0].GetFloat(), size[1].GetFloat(), size[2].GetFloat()};
		}
		else {
			m_assert(false);
		}
	};
	auto read_json_light_component = [](rapidjson::Value::Object light_component_json, entity_light_component *entity_light_component) {
		const char *light_type = light_component_json["light_type"].GetString();
		if (!strcmp(light_type, "ambient")) {
			entity_light_component->light_type = light_type_ambient;
			rapidjson::Value::Array color = light_component_json["color"].GetArray();
			entity_light_component->ambient_light.color.r = color[0].GetFloat();
			entity_light_component->ambient_light.color.g = color[1].GetFloat();
			entity_light_component->ambient_light.color.b = color[2].GetFloat();
		}
		else if (!strcmp(light_type, "directional")) {
			entity_light_component->light_type = light_type_directional;
			rapidjson::Value::Array color = light_component_json["color"].GetArray();
			rapidjson::Value::Array direction = light_component_json["direction"].GetArray();
			entity_light_component->directional_light.color.r = color[0].GetFloat();
			entity_light_component->directional_light.color.g = color[1].GetFloat();
			entity_light_component->directional_light.color.b = color[2].GetFloat();
			entity_light_component->directional_light.direction.x = direction[0].GetFloat();
			entity_light_component->directional_light.direction.y = direction[1].GetFloat();
			entity_light_component->directional_light.direction.z = direction[2].GetFloat();
		}
		else if (!strcmp(light_type, "point")) {
			entity_light_component->light_type = light_type_point;
			rapidjson::Value::Array color = light_component_json["color"].GetArray();
			rapidjson::Value::Array position = light_component_json["position"].GetArray();
			float attenuation = light_component_json["attenuation"].GetFloat();
			entity_light_component->point_light.color.r = color[0].GetFloat();
			entity_light_component->point_light.color.g = color[1].GetFloat();
			entity_light_component->point_light.color.b = color[2].GetFloat();
			entity_light_component->point_light.position.x = position[0].GetFloat();
			entity_light_component->point_light.position.y = position[1].GetFloat();
			entity_light_component->point_light.position.z = position[2].GetFloat();
			entity_light_component->point_light.attenuation = attenuation;
		}
		else {
			fatal("invalid json light type");
		}
	};
	auto read_json_physics_component = [level](rapidjson::Value::Object physics_component_json, entity_physics_component *entity_physics_component) {
		auto velocity_member = physics_component_json.FindMember("velocity");
		if (velocity_member != physics_component_json.MemberEnd()) {
			rapidjson::Value::Array velocity = velocity_member->value.GetArray();
			entity_physics_component->velocity.x = velocity[0].GetFloat();
			entity_physics_component->velocity.y = velocity[1].GetFloat();
			entity_physics_component->velocity.z = velocity[2].GetFloat();
		}
		auto mass_member = physics_component_json.FindMember("mass");
		if (mass_member != physics_component_json.MemberEnd()) {
			entity_physics_component->mass = mass_member->value.GetFloat();
		}
		auto max_speed_member = physics_component_json.FindMember("max_speed");
		if (max_speed_member != physics_component_json.MemberEnd()) {
			entity_physics_component->max_speed = max_speed_member->value.GetFloat();
		}
	};

	rapidjson::Value::Array models = json_doc["models"].GetArray();
	for (uint32 i = 0; i < models.Size(); i += 1) {
		level_add_gpk_model(level, vulkan, models[i].GetString(), store_vertices);
	}
	rapidjson::Value::Array skyboxes = json_doc["skyboxes"].GetArray();
	for (uint32 i = 0; i < skyboxes.Size(); i += 1) {
		level_add_skybox(level, vulkan, skyboxes[i].GetString());
	}
	level->skybox_index = json_doc["skybox_index"].GetUint();
	rapidjson::Value::Array entities_json = json_doc["entities"].GetArray();
	level->entity_count = entities_json.Size();
	level->render_component_count = 0;
	level->collision_component_count = 0;
	level->light_component_count = 0;
	level->physics_component_count = 0;
	for (uint32 i = 0; i < entities_json.Size(); i += 1) {
		rapidjson::Value::Object entity_json = entities_json[i].GetObject();
		if (entity_json.HasMember("render_component")) {
			level->render_component_count += 1;
		}
		if (entity_json.HasMember("light_component")) {
			level->light_component_count += 1;
		}
		if (entity_json.HasMember("collision_component")) {
			level->collision_component_count += 1;
		}
		if (entity_json.HasMember("physics_component")) {
			level->physics_component_count += 1;
		}
	}
	memory_arena *entity_components_memory_arena = &level->entity_components_memory_arenas[level->entity_components_memory_arena_index];
	level->entity_flags = allocate_memory<uint32>(entity_components_memory_arena, level->entity_count);
	level->entity_infos = allocate_memory<entity_info>(entity_components_memory_arena, level->entity_count);
	level->entity_transforms = allocate_memory<transform>(entity_components_memory_arena, level->entity_count);
	level->entity_modifications = allocate_memory<entity_modification>(entity_components_memory_arena, level->entity_count);
	level->render_components = allocate_memory<entity_render_component>(entity_components_memory_arena, level->render_component_count);
	level->collision_components = allocate_memory<entity_collision_component>(entity_components_memory_arena, level->collision_component_count);
	level->light_components = allocate_memory<entity_light_component>(entity_components_memory_arena, level->light_component_count);
	level->physics_components = allocate_memory<entity_physics_component>(entity_components_memory_arena, level->physics_component_count);

	uint32 render_component_index = 0;
	uint32 collision_component_index = 0;
	uint32 light_component_index = 0;
	uint32 physics_component_index = 0;
	for (uint32 i = 0; i < entities_json.Size(); i += 1) {
		rapidjson::Value::Object entity_json = entities_json[i].GetObject();
		uint32 *entity_flag = &level->entity_flags[i];
		entity_info *entity_info = &level->entity_infos[i];
		transform *entity_transform = &level->entity_transforms[i];
		*entity_flag = 0;
		*entity_transform = transform_identity();
		m_assert(snprintf(entity_info->name, sizeof(entity_info->name), "%s", entity_json["name"].GetString()) < sizeof(entity_info->name));
		auto transform_member = entity_json.FindMember("transform");
		if (transform_member != entity_json.MemberEnd()) {
			read_json_transform_object(transform_member->value.GetObject(), entity_transform);
		}
		auto render_component_member = entity_json.FindMember("render_component");
		if (render_component_member != entity_json.MemberEnd()) {
			*entity_flag = *entity_flag | entity_component_flag_render;
			read_json_render_component(render_component_member->value.GetObject(), &level->render_components[render_component_index++]);
		}
		auto collision_component_member = entity_json.FindMember("collision_component");
		if (collision_component_member != entity_json.MemberEnd()) {
			*entity_flag = *entity_flag | entity_component_flag_collision;
			read_json_collision_component(collision_component_member->value.GetObject(), &level->collision_components[collision_component_index++]);
		}
		auto light_component_member = entity_json.FindMember("light_component");
		if (light_component_member != entity_json.MemberEnd()) {
			*entity_flag = *entity_flag | entity_component_flag_light;
			read_json_light_component(light_component_member->value.GetObject(), &level->light_components[light_component_index++]);
		}
		auto physics_component_member = entity_json.FindMember("physics_component");
		if (physics_component_member != entity_json.MemberEnd()) {
			*entity_flag = *entity_flag | entity_component_flag_physics;
			read_json_physics_component(physics_component_member->value.GetObject(), &level->physics_components[physics_component_index++]);
		}

		if (collision_component_member != entity_json.MemberEnd() && physics_component_member != entity_json.MemberEnd()) {
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
			btQuaternion rotate(entity_transform->rotate.x, entity_transform->rotate.y, entity_transform->rotate.z, entity_transform->rotate.w);
			btVector3 translate(entity_transform->translate.x, entity_transform->translate.y, entity_transform->translate.z);
			rigid_body->setWorldTransform(btTransform(rotate, translate));
			rigid_body->setLinearVelocity(btVector3(physics_component->velocity.x, physics_component->velocity.y, physics_component->velocity.z));
			physics_component->bt_rigid_body = rigid_body;
		}
		else if (collision_component_member != entity_json.MemberEnd()) {
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
			btQuaternion rotate(entity_transform->rotate.x, entity_transform->rotate.y, entity_transform->rotate.z, entity_transform->rotate.w);
			btVector3 translate(entity_transform->translate.x, entity_transform->translate.y, entity_transform->translate.z);
			collision_object->setWorldTransform(btTransform(rotate, translate));
			collision_component->bt_collision_object = collision_object;
		}
		else if (physics_component_member != entity_json.MemberEnd()) {
			entity_physics_component *physics_component = &level->physics_components[physics_component_index - 1];
			btRigidBody *rigid_body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(physics_component->mass, nullptr, new btEmptyShape()));
			btQuaternion rotate(entity_transform->rotate.x, entity_transform->rotate.y, entity_transform->rotate.z, entity_transform->rotate.w);
			btVector3 translate(entity_transform->translate.x, entity_transform->translate.y, entity_transform->translate.z);
			rigid_body->setWorldTransform(btTransform(rotate, translate));
			rigid_body->setLinearVelocity(btVector3(physics_component->velocity.x, physics_component->velocity.y, physics_component->velocity.z));
			physics_component->bt_rigid_body = rigid_body;
		}
	}

	level->player_entity_index = UINT32_MAX;
	rapidjson::Value::Object player = json_doc["player"].GetObject();
	const char *player_entity_name = player["entity_name"].GetString();
	for (uint32 i = 0; i < level->entity_count; i += 1) {
		if (!strcmp(player_entity_name, level->entity_infos[i].name)) {
			level->player_entity_index = i;
			break;
		}
	}
	m_assert(level->player_entity_index != UINT32_MAX);

	extra_load(&json_doc);
}

template <typename T>
void level_write_json(level *level, const char *json_file_path, T extra_dump) {
	rapidjson::StringBuffer json_string;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(json_string);
	writer.SetIndent('\t', 1);
	writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
	writer.SetMaxDecimalPlaces(4);

	auto write_json_transform_object = [&](transform *transform) {
		writer.StartObject();
		writer.Key("scale");
		writer.StartArray();
		writer.Double(transform->scale.x);
		writer.Double(transform->scale.y);
		writer.Double(transform->scale.z);
		writer.EndArray();
		writer.Key("rotate");
		writer.StartArray();
		writer.Double(transform->rotate.x);
		writer.Double(transform->rotate.y);
		writer.Double(transform->rotate.z);
		writer.Double(transform->rotate.w);
		writer.EndArray();
		writer.Key("translate");
		writer.StartArray();
		writer.Double(transform->translate.x);
		writer.Double(transform->translate.y);
		writer.Double(transform->translate.z);
		writer.EndArray();
		writer.EndObject();
	};
	auto write_json_render_component = [&](entity_render_component *component) {
		writer.Key("render_component");
		writer.StartObject();
		writer.Key("model_file_name");
		writer.String(level->models[component->model_index].gpk_file);
		writer.Key("model_adjustment_transform");
		write_json_transform_object(&component->model_adjustment_transform);
		writer.EndObject();
	};
	auto write_json_collision_component = [&](entity_collision_component *component) {
		writer.Key("collision_component");
		writer.StartObject();
		if (component->shape == collision_shape_sphere) {
			auto *sphere = &component->sphere;
			writer.Key("type");
			writer.String("sphere");
			writer.Key("radius");
			writer.Double(sphere->radius);
		}
		else if (component->shape == collision_shape_capsule) {
			auto *capsule = &component->capsule;
			writer.Key("type");
			writer.String("capsule");
			writer.Key("height");
			writer.Double(capsule->height);
			writer.Key("radius");
			writer.Double(capsule->radius);
		}
		else if (component->shape == collision_shape_box) {
			auto *box = &component->box;
			writer.Key("type");
			writer.String("box");
			writer.Key("size");
			writer.StartArray();
			writer.Double(box->size.x);
			writer.Double(box->size.y);
			writer.Double(box->size.z);
			writer.EndArray();			
		}
		writer.EndObject();
	};
	auto write_json_light_component = [&](entity_light_component *component) {
		writer.Key("light_component");
		writer.StartObject();
		writer.Key("light_type");
		if (component->light_type == light_type_ambient) {
			writer.String("ambient");
			writer.Key("color");
			writer.StartArray();
			writer.Double(component->ambient_light.color.r);
			writer.Double(component->ambient_light.color.g);
			writer.Double(component->ambient_light.color.b);
			writer.EndArray();
		}
		else if (component->light_type == light_type_directional) {
			writer.String("directional");
			writer.Key("color");
			writer.StartArray();
			writer.Double(component->directional_light.color.r);
			writer.Double(component->directional_light.color.g);
			writer.Double(component->directional_light.color.b);
			writer.EndArray();
			writer.Key("direction");
			writer.StartArray();
			writer.Double(component->directional_light.direction.x);
			writer.Double(component->directional_light.direction.y);
			writer.Double(component->directional_light.direction.z);
			writer.EndArray();
		}
		else if (component->light_type == light_type_point) {
			writer.String("point");
			writer.Key("color");
			writer.StartArray();
			writer.Double(component->point_light.color.r);
			writer.Double(component->point_light.color.g);
			writer.Double(component->point_light.color.b);
			writer.EndArray();
			writer.Key("position");
			writer.StartArray();
			writer.Double(component->point_light.position.x);
			writer.Double(component->point_light.position.y);
			writer.Double(component->point_light.position.z);
			writer.EndArray();
			writer.Key("attenuation");
			writer.Double(component->point_light.attenuation);
		}
		writer.EndObject();
	};
	auto write_json_physics_component = [&](entity_physics_component *component) {
		writer.Key("physics_component");
		writer.StartObject();
		writer.Key("velocity");
		writer.StartArray();
		writer.Double(component->velocity.x);
		writer.Double(component->velocity.y);
		writer.Double(component->velocity.z);
		writer.EndArray();
		writer.Key("mass");
		writer.Double(component->mass);
		writer.Key("max_speed");
		writer.Double(component->max_speed);
		writer.EndObject();
	};

	writer.StartObject();
	{
		writer.Key("models");
		writer.StartArray();
		for (uint32 i = 0; i < level->model_count; i += 1) {
			writer.String(level->models[i].gpk_file);
		}
		writer.EndArray();
	}
	{
		writer.Key("skyboxes");
		writer.StartArray();
		for (uint32 i = 0; i < level->skybox_count; i += 1) {
			writer.String(level->skyboxes[i].file_name);
		}
		writer.EndArray();
		writer.Key("skybox_index");
		writer.Uint(level->skybox_index);
	}
	{
		writer.Key("entities");
		writer.StartArray();
		for (uint32 i = 0; i < level->entity_count; i += 1) {
			uint32 entity_flag = level->entity_flags[i];
			transform entity_transform = level->entity_transforms[i];
			const char *entity_name = level->entity_infos[i].name;
			writer.StartObject();
			writer.Key("name");
			writer.String(entity_name);
			writer.Key("transform");
			write_json_transform_object(&entity_transform);
			if (entity_flag & entity_component_flag_render) {
				write_json_render_component(entity_get_render_component(level, i));
			}
			if (entity_flag & entity_component_flag_collision) {
				write_json_collision_component(entity_get_collision_component(level, i));
			}
			if (entity_flag & entity_component_flag_light) {
				write_json_light_component(entity_get_light_component(level, i));
			}
			if (entity_flag & entity_component_flag_physics) {
				write_json_physics_component(entity_get_physics_component(level, i));
			}
			writer.EndObject();
		}
		writer.EndArray();
	}
	{
		writer.Key("player");
		writer.StartObject();
		writer.Key("entity_name");
		writer.String(level->entity_infos[level->player_entity_index].name);
		writer.EndObject();
	}	
	extra_dump(&writer);
	writer.EndObject();

	file_mapping file_mapping = {};
	create_file_mapping(json_file_path, json_string.GetSize(), &file_mapping);
	memcpy(file_mapping.ptr, json_string.GetString(), json_string.GetSize());
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
void traverse_model_node_hierarchy(model_node *nodes, uint32 index, F f) {
	model_node *node = &nodes[index];
	f(node);
	for (uint32 i = 0; i < node->child_count; i += 1) {
		traverse_model_node_hierarchy(nodes, node->children[i], f);
	}
}

template <typename F>
void traverse_model_scenes(model *model, F f) {
	for (uint32 i = 0; i < model->scene_count; i += 1) {
		model_scene *scene = &model->scenes[i];
		for (uint32 i = 0; i < scene->node_index_count; i += 1) {
			traverse_model_node_hierarchy(model->nodes, scene->node_indices[i], f);
		}
	}
}

template <typename F>
void traverse_model_node_hierarchy_track_global_transform(model_node *nodes, uint32 index, mat4 global_transform_mat, F f) {
	model_node *node = &nodes[index];
	global_transform_mat = global_transform_mat * node->local_transform_mat;
	f(node, global_transform_mat);
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

void level_generate_model_render_data(level *level, vulkan *vulkan, uint32 model_index, mat4 transform_mat) {
	m_assert(model_index < level->model_count);
	uint32 uniform_alignment = (uint32)vulkan->device.physical_device_properties.limits.minUniformBufferOffsetAlignment;
	model_render_data *model_render_data = &level->render_data.models[level->render_data.model_count++];
	model_render_data->model_index = model_index;
	model *model = &level->models[model_index];
	model_render_data->mesh_count = 0;
	traverse_model_scenes(model, [&](model_node *node) {
		if (node->mesh_index < model->mesh_count) {
			model_render_data->mesh_count += 1;
		}
	});
	model_render_data->meshes = allocate_memory<mesh_render_data>(&level->render_thread_frame_memory_arena, model_render_data->mesh_count);
	uint32 mesh_render_data_index = 0;
	traverse_model_scenes_track_global_transform(model, [&](model_node *node, mat4 global_transform) {
		if (node->mesh_index < model->mesh_count) {
			mesh_render_data *mesh = &model_render_data->meshes[mesh_render_data_index++];
			mesh->mesh_index = node->mesh_index;
			struct {
				mat4 model_mat;
				mat4 normal_mat;
				vec2 uv_scale;
				float roughness;
				float metallic;
				float height_map_scale;
 			} uniforms = {};
			uniforms.model_mat = transform_mat * global_transform;
			uniforms.normal_mat = mat4_transpose(mat4_inverse(uniforms.model_mat));
			uniforms.uv_scale = {1, 1};
			uniforms.roughness = 1;
			uniforms.metallic = 0;
			uniforms.height_map_scale = 0;
			uint8 *uniform_buffer_ptr = vulkan->buffers.frame_uniform_buffer_ptrs[vulkan->frame_index];
			uint32 *frame_uniform_buffer_offset = &vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index];
			round_up(frame_uniform_buffer_offset, uniform_alignment);
			mesh->frame_uniform_buffer_offset = *frame_uniform_buffer_offset;
			memcpy(uniform_buffer_ptr + *frame_uniform_buffer_offset, &uniforms, sizeof(uniforms));
			*frame_uniform_buffer_offset += sizeof(uniforms);
		}
	});
};

template <typename F>
void level_generate_render_data(level *level, vulkan *vulkan, camera camera, F generate_extra_render_data) {
	level->render_data = {};
	vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] = 0;
	vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index] = 0;

	uint32 uniform_alignment = (uint32)vulkan->device.physical_device_properties.limits.minUniformBufferOffsetAlignment;

	ambient_light ambient_light = {{0, 0, 0}};
	directional_light directional_light = {{0, 0, 0}, {1, 0, 0}};
	point_light point_lights[level_max_point_light_count] = {};
	uint32 point_light_count = 0;
	for (uint32 i = 0; i < level->light_component_count; i += 1) {
		switch (level->light_components[i].light_type) {
			case light_type_ambient: {
				ambient_light = level->light_components[i].ambient_light;
			} break;
			case light_type_directional: {
				directional_light = level->light_components[i].directional_light;
			} break;
			case light_type_point: {
				m_assert(point_light_count < m_countof(point_lights));
				point_lights[point_light_count++] = level->light_components[i].point_light;
			} break;
		}
	}
	{ // common uniform
		struct common_uniform {
			mat4 camera_view_proj_mat;
			vec4 camera_position;
			mat4 shadow_map_proj_mat;
			struct {
				vec4 color;
			} ambient_light;
			struct {
				vec4 color;
				vec4 direction;
			} directional_lights[level_max_directional_light_count];
			struct {
				vec4 color;
				vec4 position_attenuation;
			} point_lights[level_max_point_light_count];
			struct {
				vec4 color;
				vec4 position_attenuation;
				vec4 direction_angle;
			} spot_lights[level_max_spot_light_count];
			uint32 directional_light_count;
			uint32 point_light_count;
			uint32 spot_light_count;
		};
		round_up(&vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], uniform_alignment);
		level->render_data.common_data_frame_uniform_buffer_offset = vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index];
		common_uniform *uniform = (struct common_uniform *)(vulkan->buffers.frame_uniform_buffer_ptrs[vulkan->frame_index] + vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index]);
		uniform->camera_view_proj_mat = mat4_vulkan_clip() * camera_view_projection_mat4(camera);
		uniform->camera_position = vec4{m_unpack3(camera.position.e), 0};
		struct camera shadow_map_camera = camera;
		shadow_map_camera.zfar = 100;
		uniform->shadow_map_proj_mat = mat4_vulkan_clip() * camera_shadow_map_projection_mat4(shadow_map_camera, directional_light.direction);
		uniform->ambient_light = {vec4{ambient_light.color.r, ambient_light.color.g, ambient_light.color.b, 0}};
		uniform->directional_lights[0].color = vec4{directional_light.color.r, directional_light.color.g, directional_light.color.b, 0};
		uniform->directional_lights[0].direction = vec4{directional_light.direction.x, directional_light.direction.y, directional_light.direction.z, 0};
		uniform->directional_light_count = 1;
		for (uint32 i = 0; i < m_countof(point_lights); i += 1) {
			uniform->point_lights[i].color = {point_lights[i].color.r, point_lights[i].color.g, point_lights[i].color.b, 0};
			uniform->point_lights[i].position_attenuation = {point_lights[i].position.x, point_lights[i].position.y, point_lights[i].position.z, point_lights[i].attenuation};
		}
		uniform->point_light_count = point_light_count;
		uniform->spot_light_count = 0;
		vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index] += sizeof(struct common_uniform);
	}
	{ // models
		round_up(&vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], uniform_alignment);
		level->render_data.models = allocate_memory<struct model_render_data>(&level->render_thread_frame_memory_arena, level->render_component_count);
		level->render_data.model_count = 0;
		for (uint32 i = 0; i < level->entity_count; i += 1) {
			if (level->entity_flags[i] & entity_component_flag_render) {
				entity_render_component *render_component = entity_get_render_component(level, i);
				mat4 transform_mat = transform_to_mat4(level->entity_transforms[i]) * transform_to_mat4(render_component->model_adjustment_transform);
				level_generate_model_render_data(level, vulkan, render_component->model_index, transform_mat);
			}
		}
	}
	// { // texts
		// struct vertex {
		//   vec4 position_uv;
		//   u8vec4 color;
		//   uint32 transform_mat_index;
		// };
		// static_assert(sizeof(struct vertex) == 24, "");
	  // round_up(&vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index], (uint32)sizeof(struct vertex));
	  // round_up(&vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], uniform_alignment);
		// level->render_data.text_frame_vertex_buffer_offset = vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index];
		// level->render_data.text_frame_uniform_buffer_offset = vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index];
		// level->render_data.text_frame_vertex_count = 0;
		// level->render_data.text_frame_uniform_count = 0;
		// auto append_text_render_data = [level, vulkan, font, camera](const char *text, aa_bound bound) {
		//   vec4 text_quad = {};
		//   float xpos = 0;
		//   float ypos = 0;
		//   uint32 text_len = (uint32)strlen(text);
		//   for (uint32 i = 0; i < text_len; i += 1) {
		//     stbtt_aligned_quad quad = {};
		//     stbtt_GetPackedQuad(font->stbtt_packed_chars, font->packed_bitmap_width, font->packed_bitmap_height, text[i] - ' ', &xpos, &ypos, &quad, 0);
		//     quad.y0 = -quad.y0;
		//     quad.y1 = -quad.y1;
		//     text_quad.x0 = min(text_quad.x0, quad.x0);
		//     text_quad.y0 = max(text_quad.y0, quad.y0);
		//     text_quad.x1 = max(text_quad.x1, quad.x1);
		//     text_quad.y1 = min(text_quad.y1, quad.y1);
		//     vertex *vertices = (vertex *)(vulkan->buffers.frame_vertex_buffer_ptrs[vulkan->frame_index] + vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] + sizeof(struct vertex) * 6 * i);
		//     vertices[0] = {{quad.x0, quad.y0, quad.s0, quad.t0}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
		//     vertices[1] = {{quad.x0, quad.y1, quad.s0, quad.t1}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
		//     vertices[2] = {{quad.x1, quad.y1, quad.s1, quad.t1}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
		//     vertices[3] = vertices[0];
		//     vertices[4] = vertices[2];
		//     vertices[5] = {{quad.x1, quad.y0, quad.s1, quad.t0}, {0, 255, 0, 255}, level->render_data.text_frame_uniform_count};
		//  }
		//   vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] += sizeof(struct vertex) * 6 * text_len;

		//   vec2 text_quad_center = {};
		//   text_quad_center.x = text_quad.x0 + (text_quad.x1 - text_quad.x0) / 2;
		//   text_quad_center.y = text_quad.y0 + (text_quad.y1 - text_quad.y0) / 2;

		//   float text_quad_scale = 1 / font->font_size;
		//   float text_quad_height = text_quad.y0 - text_quad.y1;
		//   vec3 text_quad_translate = {0, text_quad_height / 2 * text_quad_scale, 0};
		//   text_quad_translate = text_quad_translate + aa_bound_center(bound);
		//   text_quad_translate.y += (bound.max.y - bound.min.y) / 2;

		//   mat4 transform_mat = mat4_vulkan_clip() * camera_view_projection_mat4(camera) * mat4_from_translation(text_quad_translate) * camera_billboard_mat4(camera) * mat4_from_scaling(text_quad_scale) * mat4_from_translation({(-text_quad_center).x, (-text_quad_center).y, 0});
		//   *(mat4 *)(vulkan->buffers.frame_uniform_buffer_ptrs[vulkan->frame_index] + vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index]) = transform_mat;
		//   vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index] += sizeof(mat4);

		//   level->render_data.text_frame_vertex_count += text_len * 6;
		//   level->render_data.text_frame_uniform_count += 1;
		// };
		// for (uint32 i = 0; i < level->entity_count; i += 1) {
		//   append_text_render_data(level->entity_infos[i].name, aa_bound{{-1, -1, -1}, {1, 1, 1}});
		// }
	// }
	generate_extra_render_data();
}

template <typename F0, typename F1>
void level_generate_render_commands(level *level, vulkan *vulkan, camera camera, F0 extra_main_render_pass_render_commands, F1 extra_swap_chain_render_commands) {
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
				for (uint32 i = 0; i < level->render_data.model_count; i += 1) {
					model_render_data *model_render_data = &level->render_data.models[i];
					model *model = &level->models[model_render_data->model_index];
					for (uint32 i = 0; i < model_render_data->mesh_count; i += 1) {
						model_mesh *mesh = &model->meshes[model_render_data->meshes[i].mesh_index];
						uint32 offsets[3] = {level->render_data.common_data_frame_uniform_buffer_offset, model_render_data->meshes[i].frame_uniform_buffer_offset, 0};
						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_shadow_map_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(offsets), offsets);
						vkCmdDrawIndexed(cmd_buffer, mesh->index_count, 1, mesh->index_buffer_offset / sizeof(uint16), mesh->vertex_buffer_offset / mesh->vertex_size, 0);
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
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0].layout, 0, 1, &vulkan->descriptors.shadow_map_framebuffer_images[vulkan->frame_index][0], 0, nullptr);
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
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[1].layout, 0, 1, &vulkan->descriptors.shadow_map_framebuffer_images[vulkan->frame_index][1], 0, nullptr);
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
		VkDeviceSize vertex_buffer_offset = 0;
		vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.level_vertex_buffer.buffer, &vertex_buffer_offset);
		vkCmdBindIndexBuffer(cmd_buffer, vulkan->buffers.level_vertex_buffer.buffer, 0, VK_INDEX_TYPE_UINT16);
		if (level->render_data.model_count > 0) {
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.pipeline);
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 2, 1, &vulkan->descriptors.shadow_map_framebuffer_images[vulkan->frame_index][2], 0, nullptr);
			for (uint32 i = 0; i < level->render_data.model_count; i += 1) {
				model_render_data *model_render_data = &level->render_data.models[i];
				model *model = &level->models[model_render_data->model_index];
				for (uint32 i = 0; i < model_render_data->mesh_count; i += 1) {
					model_mesh *mesh = &model->meshes[model_render_data->meshes[i].mesh_index];
					uint32 frame_uniform_buffer_offsets[3] = {level->render_data.common_data_frame_uniform_buffer_offset, model_render_data->meshes[i].frame_uniform_buffer_offset, 0};
					vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(frame_uniform_buffer_offsets), frame_uniform_buffer_offsets);
					if (mesh->material_index < model->material_count) {
 						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 1, 1, &model->materials[mesh->material_index].textures_descriptor_set, 0, nullptr);
					}
					else {
						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 1, 1, &vulkan->descriptors.model_default_material_images, 0, nullptr);
					}
					vkCmdDrawIndexed(cmd_buffer, mesh->index_count, 1, mesh->index_buffer_offset / sizeof(uint16), mesh->vertex_buffer_offset / mesh->vertex_size, 0);
					if (model_render_data->meshes[i].render_vertices_outline) {
						vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_wireframe_pipeline.pipeline);
						uint32 frame_uniform_buffer_offsets[3] = {level->render_data.common_data_frame_uniform_buffer_offset, model_render_data->meshes[i].frame_uniform_buffer_offset, 0};
						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_wireframe_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(frame_uniform_buffer_offsets), frame_uniform_buffer_offsets);
						vkCmdDrawIndexed(cmd_buffer, mesh->index_count, 1, mesh->index_buffer_offset / sizeof(uint16), mesh->vertex_buffer_offset / mesh->vertex_size, 0);
						vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.pipeline);
					}
				}
			}
		}
		if (level->skybox_index < level->skybox_count) {
		  skybox *skybox = &level->skyboxes[level->skybox_index];
		  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.skybox_pipeline.pipeline);
		  vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.skybox_pipeline.layout, 0, 1, &skybox->descriptor_set, 0, nullptr);
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
			vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.swap_chain_pipeline.layout, 0, 1, &vulkan->descriptors.main_framebuffer_images[vulkan->frame_index], 0, nullptr);
			vkCmdDraw(cmd_buffer, 3, 1, 0, 0);
		}
		extra_swap_chain_render_commands();
		vkCmdEndRenderPass(cmd_buffer);
	}
}	
