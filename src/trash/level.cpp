/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#include "common.cpp"
#include "math.cpp"
#include "geometry.cpp"
#include "simd.cpp"
#include "gpk.cpp"
#include "vulkan.cpp"

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "bullet/BulletDynamics/Character/btKinematicCharacterController.h"

#include "codegen/level.flatbuffers.h"
static_assert(sizeof(flatbuffers::Transform) == sizeof(struct transform), "");

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
	uint32 diffuse_map_descriptor_index;
	uint32 metallic_map_descriptor_index;
	uint32 roughness_map_descriptor_index;
	uint32 normal_map_descriptor_index;
	vec4 diffuse_factor;
	float metallic_factor;
	float roughness_factor;
	char name[sizeof(gpk_model_material::name)];
};

struct model_image {
	uint32 index;
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
	transform transform;
	btCollisionShape *collision_shape;
	char gpk_file[32];
};

struct skybox {
	uint32 cubemap_descriptor_index;
	char gpk_file[32];
};

struct terrain_vertex {
	vec2 position;
	vec2 uv;
};
static_assert(sizeof(struct terrain_vertex) == 16, "");

struct terrain {
	uint32 width;
	uint32 height;
	float max_height;
	uint32 sample_per_meter;
	
	uint32 height_map_image_index;
	uint32 height_map_descriptor_index;
	int16 *height_map;
	btHeightfieldTerrainShape *bt_terrain_shape;
	btCollisionObject *bt_collision_object;

	uint32 diffuse_map_descriptor_index;
	uint32 diffuse_map_image_index;
	uint32 *diffuse_map;
	float diffuse_map_uv_repeat;

	uint32 vertex_buffer_offset;
	uint32 index_buffer_offset;
	uint32 index_count;

	char gpk_file[32];
};

struct ambient_light {
	vec3 color;
};

struct direct_light {
	vec3 color;
	vec3 direction;
};

struct point_light {
	vec3 color;
	vec3 position;
	float attenuation;
};

struct spot_light {
	vec3 color;
	vec3 position;
	vec3 direction;
	float solid_angle;
	float attenuation;
};

struct lights {
	ambient_light ambient_light;
	direct_light direct_light;
};

struct static_object {
	uint32 model_index;
	transform transform;
	btCollisionObject *collision_object;
	uint32 animation_index;
	double animation_time;
	char *id;
};

struct dynamic_object {
	uint32 model_index;
	transform transform;
	btRigidBody *rigid_body;
	uint32 animation_index;
	double animation_time;
	char *id;
};

struct player {
	uint32 model_index;
	transform transform;
	btCollisionObject *collision_object;
	btKinematicCharacterController *bt_character_controller;
	uint32 animation_index;
	double animation_time;
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

struct level_frame_render_data {
	model_render_data *models;
	uint32 model_count;
};

struct level_persistant_data {
	uint32 general_memory_arena_reset_offset;

	uint32 vertex_buffer_reset_offset;
	uint32 image_reset_offset;
	uint32 image_reset_index;
	uint32 combined_2d_image_samplers_reset_index;
	uint32 combined_cube_image_samplers_reset_index;

	uint32 box_vertex_buffer_offset;
	uint32 sphere_vertex_buffer_offset;
	uint32 hemisphere_vertex_buffer_offset;
	uint32 cylinder_vertex_buffer_offset;
	uint32 hollow_cylinder_vertex_buffer_offset;
	uint32 hollow_circle_vertex_buffer_offset;
	uint32 torus_vertex_buffer_offset;

	uint32 default_diffuse_map_descriptor_index;
	uint32 default_metallic_map_descriptor_index;
	uint32 default_roughness_map_descriptor_index;
	uint32 default_normal_map_descriptor_index;
	uint32 default_height_map_descriptor_index;
};

struct level {
	player player;

	static_object *static_objects;
	uint32 static_object_count;
	uint32 static_object_capacity;

	dynamic_object *dynamic_objects;
	uint32 dynamic_object_count;
	uint32 dynamic_object_capacity;

	model *models;
	uint32 model_count;
	uint32 model_capacity;

	lights lights;
	
	terrain *terrains;
	uint32 terrain_count;
	uint32 terrain_capacity;
	uint32 terrain_index;

	skybox *skyboxes;
	uint32 skybox_count;
	uint32 skybox_capacity;
	uint32 skybox_index;

	level_frame_render_data frame_render_data;

	memory_arena frame_memory_arena;
	memory_arena general_memory_arena;

	level_persistant_data persistant_data;
};

void reset_level(level *level, vulkan *vulkan) {
	level->general_memory_arena.size = level->persistant_data.general_memory_arena_reset_offset;
	memset((uint8 *)level->general_memory_arena.memory + level->general_memory_arena.size, 0, level->general_memory_arena.capacity - level->general_memory_arena.size);

	level->player = {};
	level->player.model_index = UINT32_MAX;
	level->player.transform = transform_identity();
	level->player.animation_index = UINT32_MAX;

	level->static_object_count = 0;
	level->dynamic_object_count = 0;
	level->model_count = 0;
	level->skybox_count = 0;
	level->terrain_count = 0;

	level->lights.ambient_light = {0.1f, 0.1f, 0.1f};
	level->lights.direct_light = {{0.5f, 0.5f, 0.5f}, vec3_normalize({1, 1, 1})};
	
	vulkan->memory_regions.vertex_buffer.buffer_size = level->persistant_data.vertex_buffer_reset_offset;
	vulkan->memory_regions.images.memory_size = level->persistant_data.image_reset_offset;
	for (uint32 i = level->persistant_data.image_reset_index; i < vulkan->memory_regions.images.image_count; i += 1) {
		vkDestroyImageView(vulkan->device.device, vulkan->memory_regions.images.images[i].view, nullptr);
		vkDestroyImage(vulkan->device.device, vulkan->memory_regions.images.images[i].image, nullptr);
	}
	vulkan->memory_regions.images.image_count = level->persistant_data.image_reset_index;
	vulkan->descriptors.combined_2d_image_sampler_count = level->persistant_data.combined_2d_image_samplers_reset_index;
	vulkan->descriptors.combined_cube_image_sampler_count = level->persistant_data.combined_cube_image_samplers_reset_index;
}

void initialize_level(level *level, vulkan *vulkan) {
	*level = {};
	m_assert(initialize_memory_arena(m_megabytes(8), &level->frame_memory_arena), "");
	m_assert(initialize_memory_arena(m_megabytes(128), &level->general_memory_arena), "");
	{ // persistant memory
		level->static_object_capacity = 1024;
		level->static_objects = allocate_memory<struct static_object>(&level->general_memory_arena, level->static_object_capacity);

		level->dynamic_object_capacity = 1024;
		level->dynamic_objects = allocate_memory<struct dynamic_object>(&level->general_memory_arena, level->dynamic_object_capacity);

		level->model_capacity = 512;
		level->models = allocate_memory<struct model>(&level->general_memory_arena, level->model_capacity);

		level->terrain_capacity = 8;
		level->terrains = allocate_memory<struct terrain>(&level->general_memory_arena, level->terrain_capacity);

		level->skybox_capacity = 8;
		level->skyboxes = allocate_memory<struct skybox>(&level->general_memory_arena, level->skybox_capacity);

		level->persistant_data.general_memory_arena_reset_offset = (uint32)level->general_memory_arena.size;
	}
	{ // persistant vulkan vertex buffer
		level->persistant_data.box_vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, box_vertices, sizeof(box_vertices), 12u);
		level->persistant_data.sphere_vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, sphere_vertices, sizeof(sphere_vertices), 12u);
		level->persistant_data.hemisphere_vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, hemisphere_vertices, sizeof(hemisphere_vertices), 12u);
		level->persistant_data.cylinder_vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, cylinder_vertices, sizeof(cylinder_vertices), 12u);
		level->persistant_data.hollow_cylinder_vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, hollow_cylinder_vertices, sizeof(hollow_cylinder_vertices), 12u);
		level->persistant_data.hollow_circle_vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, hollow_circle_vertices, sizeof(hollow_circle_vertices), 12u);
		level->persistant_data.torus_vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, torus_vertices, sizeof(torus_vertices), 12u);
		level->persistant_data.vertex_buffer_reset_offset = vulkan->memory_regions.vertex_buffer.buffer_size;
	}
	{ // persistant vulkan images
		uint8 default_diffuse_map_data[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
		uint8 default_metallic_map_data[] = {255, 255, 255, 255};
		uint8 default_roughness_map_data[] = {255, 255, 255, 255};
		uint8 default_normal_map_data[] = {128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0};
		uint8 default_height_map_data[] = {0, 0, 0, 0};

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_info.extent = {2, 2, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;

		uint32 diffuse_map_index = append_vulkan_images(vulkan, image_info, image_view_info, default_diffuse_map_data, sizeof(default_diffuse_map_data), 1, 4);
		VkImageView diffuse_map_image_view = vulkan->memory_regions.images.images[diffuse_map_index].view;
		level->persistant_data.default_diffuse_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, diffuse_map_image_view, vulkan->samplers.mipmap_samplers[0]);

		image_info.format = VK_FORMAT_R8_UNORM;
		image_view_info.format = image_info.format;
		uint32 metallic_map_index = append_vulkan_images(vulkan, image_info, image_view_info, default_metallic_map_data, sizeof(default_metallic_map_data), 1, 1);
		VkImageView metallic_map_image_view = vulkan->memory_regions.images.images[metallic_map_index].view;
		level->persistant_data.default_metallic_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, metallic_map_image_view, vulkan->samplers.mipmap_samplers[0]);

		image_info.format = VK_FORMAT_R8_UNORM;
		image_view_info.format = image_info.format;
		uint32 roughness_map_index = append_vulkan_images(vulkan, image_info, image_view_info, default_roughness_map_data, sizeof(default_roughness_map_data), 1, 1);
		VkImageView roughness_map_image_view = vulkan->memory_regions.images.images[roughness_map_index].view;
		level->persistant_data.default_roughness_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, roughness_map_image_view, vulkan->samplers.mipmap_samplers[0]);

		image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_view_info.format = image_info.format;
		uint32 normal_map_index = append_vulkan_images(vulkan, image_info, image_view_info, default_normal_map_data, sizeof(default_normal_map_data), 1, 4);
		VkImageView normal_map_image_view = vulkan->memory_regions.images.images[normal_map_index].view;
		level->persistant_data.default_normal_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, normal_map_image_view, vulkan->samplers.mipmap_samplers[0]);

		image_info.format = VK_FORMAT_R8_UNORM;
		image_view_info.format = image_info.format;
		uint32 height_map_index = append_vulkan_images(vulkan, image_info, image_view_info, default_height_map_data, sizeof(default_height_map_data), 1, 1);
		VkImageView height_map_image_view = vulkan->memory_regions.images.images[height_map_index].view;
		level->persistant_data.default_height_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, height_map_image_view, vulkan->samplers.mipmap_samplers[0]);

		level->persistant_data.image_reset_offset = (uint32)vulkan->memory_regions.images.memory_size;
		level->persistant_data.image_reset_index = vulkan->memory_regions.images.image_count;
		level->persistant_data.combined_2d_image_samplers_reset_index = vulkan->descriptors.combined_2d_image_sampler_count;
		level->persistant_data.combined_cube_image_samplers_reset_index = vulkan->descriptors.combined_cube_image_sampler_count;
	}
	reset_level(level, vulkan);
}
	
bool add_model_to_level(level *level, vulkan *vulkan, const char *gpk_file, transform transform, bool store_vertices) {
	file_mapping gpk_file_mapping = {};
	if (!open_file_mapping(gpk_file, &gpk_file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(&gpk_file_mapping));

	gpk_model *gpk_model = (struct gpk_model *)gpk_file_mapping.ptr;
	if (strcmp(gpk_model->format_str, m_gpk_model_format_str)) {
		return false;
	}

	if (level->model_count >= level->model_capacity) {
		return false;
	}
	model *model = &level->models[level->model_count];
	const char *file_name = get_file_name(gpk_file);
	for (uint32 i = 0; i < level->model_count; i += 1) {
		if (!strcmp(level->models[i].gpk_file, file_name)) {
			return false;
		}
	}
	snprintf(model->gpk_file, sizeof(model->gpk_file), "%s", file_name);
	model->transform = transform;
	model->scene_count = gpk_model->scene_count;
	model->node_count = gpk_model->node_count;
	model->mesh_count = gpk_model->mesh_count;
	model->skin_count = gpk_model->skin_count;
	model->animation_count = gpk_model->animation_count;
	model->material_count = gpk_model->material_count;
	model->image_count = gpk_model->image_count;
	model->scenes = allocate_memory<struct model_scene>(&level->general_memory_arena, model->scene_count);
	model->nodes = allocate_memory<struct model_node>(&level->general_memory_arena, model->node_count);
	model->meshes = allocate_memory<struct model_mesh>(&level->general_memory_arena, model->mesh_count);
	if (model->skin_count > 0) {
		model->skins = allocate_memory<struct model_skin>(&level->general_memory_arena, model->skin_count);
	}
	if (model->animation_count > 0) {
		model->animations = allocate_memory<struct model_animation>(&level->general_memory_arena, model->animation_count);
	}
	if (model->material_count > 0) {
		model->materials = allocate_memory<struct model_material>(&level->general_memory_arena, model->material_count);
	}
	if (model->image_count > 0) {
		model->images = allocate_memory<struct model_image>(&level->general_memory_arena, model->image_count);
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
		model_mesh->primitives = allocate_memory<struct model_mesh_primitive>(&level->general_memory_arena, model_mesh->primitive_count);
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
			primitive->index_buffer_offset = append_vulkan_vertex_buffer(vulkan, indices_data, indices_size, sizeof(uint16));

			primitive->vertex_count = gpk_primitive->vertex_count;
			primitive->vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, vertices_data, vertices_size, sizeof(struct gpk_model_vertex));

			if (store_vertices) {
				primitive->indices_data = allocate_memory<uint8>(&level->general_memory_arena, indices_size);
				primitive->vertices_data = allocate_memory<uint8>(&level->general_memory_arena, vertices_size);
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
		m_assert(model_skin->joint_count > 0, "");
		model_skin->joints = allocate_memory<struct model_joint>(&level->general_memory_arena, model_skin->joint_count);
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
		animation->channels = allocate_memory<struct model_animation_channel>(&level->general_memory_arena, animation->channel_count);
		for (uint32 i = 0; i < animation->channel_count; i += 1) {
			gpk_model_animation_channel *gpk_channel = ((struct gpk_model_animation_channel*)(gpk_file_mapping.ptr + gpk_animation->channel_offset)) + i;
			model_animation_channel *channel = &animation->channels[i];
			channel->node_index = gpk_channel->node_index;
			channel->channel_type = gpk_channel->channel_type;
			channel->sampler_index = gpk_channel->sampler_index;
		}
		animation->sampler_count = gpk_animation->sampler_count;
		animation->samplers = allocate_memory<model_animation_sampler>(&level->general_memory_arena, animation->sampler_count);
		for (uint32 i = 0; i < animation->sampler_count; i += 1) {
			gpk_model_animation_sampler *gpk_sampler = ((struct gpk_model_animation_sampler*)(gpk_file_mapping.ptr + gpk_animation->sampler_offset)) + i;
			model_animation_sampler *sampler = &animation->samplers[i];
			sampler->interpolation_type = gpk_sampler->interpolation_type;
			sampler->key_frame_count = gpk_sampler->key_frame_count;
			sampler->key_frames = allocate_memory<struct model_animation_key_frame>(&level->general_memory_arena, sampler->key_frame_count);
			for (uint32 i = 0; i < sampler->key_frame_count; i += 1) {
				gpk_model_animation_key_frame *gpk_key_frame = ((struct gpk_model_animation_key_frame *)(gpk_file_mapping.ptr + gpk_sampler->key_frame_offset)) + i;
				sampler->key_frames[i].time = gpk_key_frame->time;
				sampler->key_frames[i].transform_data = gpk_key_frame->transform_data;
			}
		}
	}
	for (uint32 i = 0; i < model->image_count; i += 1) {
		gpk_model_image *gpk_model_image = ((struct gpk_model_image*)(gpk_file_mapping.ptr + gpk_model->image_offset)) + i;
		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = (VkFormat)gpk_model_image->format;
		image_info.extent = {gpk_model_image->width, gpk_model_image->height, 1};
		image_info.mipLevels = gpk_model_image->mipmap_count;
		image_info.arrayLayers = gpk_model_image->layer_count;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = image_info.mipLevels;
		image_view_info.subresourceRange.layerCount = image_info.arrayLayers;
		uint8 *image_data = gpk_file_mapping.ptr + gpk_model_image->data_offset;
		model->images[i].index = append_vulkan_images(vulkan, image_info, image_view_info, image_data, gpk_model_image->size, gpk_model_image->format_block_dimension, gpk_model_image->format_block_size);
	}
	for (uint32 i = 0; i < model->material_count; i += 1) {
		gpk_model_material *gpk_model_material = ((struct gpk_model_material*)(gpk_file_mapping.ptr + gpk_model->material_offset)) + i;
		model_material *model_material = &model->materials[i];
		array_copy(model_material->name, gpk_model_material->name);
		if (gpk_model_material->diffuse_image_index < gpk_model->image_count) {
			uint32 image_index = model->images[gpk_model_material->diffuse_image_index].index;
			vulkan_image &image = vulkan->memory_regions.images.images[image_index];
			model_material->diffuse_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image.view, vulkan->samplers.mipmap_samplers[image.mipmap_count]);
		} else {
			model_material->diffuse_map_descriptor_index = level->persistant_data.default_diffuse_map_descriptor_index;
		}
		if (gpk_model_material->metallic_image_index < gpk_model->image_count) {
			uint32 image_index = model->images[gpk_model_material->metallic_image_index].index;
			vulkan_image &image = vulkan->memory_regions.images.images[image_index];
			model_material->metallic_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image.view, vulkan->samplers.mipmap_samplers[image.mipmap_count]);
		} else {
			model_material->metallic_map_descriptor_index = level->persistant_data.default_metallic_map_descriptor_index;
		}
		if (gpk_model_material->roughness_image_index < gpk_model->image_count) {
			uint32 image_index = model->images[gpk_model_material->roughness_image_index].index;
			vulkan_image &image = vulkan->memory_regions.images.images[image_index];
			model_material->roughness_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image.view, vulkan->samplers.mipmap_samplers[image.mipmap_count]);
		} else {
			model_material->roughness_map_descriptor_index = level->persistant_data.default_roughness_map_descriptor_index;
		}
		if (gpk_model_material->normal_image_index < gpk_model->image_count) {
			uint32 image_index = model->images[gpk_model_material->normal_image_index].index;
			vulkan_image &image = vulkan->memory_regions.images.images[image_index];
			model_material->normal_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image.view, vulkan->samplers.mipmap_samplers[image.mipmap_count]);
		} else {
			model_material->normal_map_descriptor_index = level->persistant_data.default_normal_map_descriptor_index;
		}
		model_material->diffuse_factor = gpk_model_material->diffuse_factor;
		model_material->metallic_factor = gpk_model_material->metallic_factor;
		model_material->roughness_factor = gpk_model_material->roughness_factor;
	}
	level->model_count += 1;
	return true;
}

bool add_skybox_to_level(level *level, vulkan *vulkan, const char *gpk_file) {
	file_mapping gpk_file_mapping = {};
	if (!open_file_mapping(gpk_file, &gpk_file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(&gpk_file_mapping));

	gpk_skybox *gpk_skybox = (struct gpk_skybox *)gpk_file_mapping.ptr;
	if (strcmp(gpk_skybox->format_str, m_gpk_skybox_format_str)) {
		return false;
	}

	if (level->skybox_count >= level->skybox_capacity) {
		return false;
	}
	skybox *skybox = &level->skyboxes[level->skybox_count];
	const char *file_name = get_file_name(gpk_file);
	for (uint32 i = 0; i < level->skybox_count; i += 1) {
		if (!strcmp(level->skyboxes[i].gpk_file, file_name)) {
			return false;
		}
	}
	snprintf(skybox->gpk_file, sizeof(skybox->gpk_file), "%s", file_name);

	VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
	image_info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.format = (VkFormat)gpk_skybox->cubemap_format;
	image_info.extent = {gpk_skybox->cubemap_width, gpk_skybox->cubemap_height, 1};
	image_info.mipLevels = gpk_skybox->cubemap_mipmap_count;
	image_info.arrayLayers = gpk_skybox->cubemap_layer_count;
	image_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
	image_view_info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	image_view_info.format = image_info.format;
	image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_info.subresourceRange.levelCount = gpk_skybox->cubemap_mipmap_count;
	image_view_info.subresourceRange.layerCount = gpk_skybox->cubemap_layer_count;
	uint8 *cubemap_ptr = (uint8 *)gpk_skybox + gpk_skybox->cubemap_offset;
	uint32 cubemap_index = append_vulkan_images(vulkan, image_info, image_view_info, cubemap_ptr, gpk_skybox->cubemap_size, gpk_skybox->cubemap_format_block_dimension, gpk_skybox->cubemap_format_block_size);
	VkImageView image_view = vulkan->memory_regions.images.images[cubemap_index].view;
	skybox->cubemap_descriptor_index = append_vulkan_combined_cube_image_samplers(vulkan, image_view, vulkan->samplers.skybox_cubemap_sampler);

	level->skybox_count += 1;
	return true;
}

bool add_terrain_to_level(level *level, vulkan *vulkan, const char *gpk_file, bool store_diffuse_map) {
	file_mapping gpk_file_mapping = {};
	if (!open_file_mapping(gpk_file, &gpk_file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(&gpk_file_mapping));

	gpk_terrain *gpk_terrain = (struct gpk_terrain *)gpk_file_mapping.ptr;
	if (strcmp(gpk_terrain->format_str, m_gpk_terrain_format_str)) {
		return false;
	}
	
	if (level->terrain_count >= level->terrain_capacity) {
		return false;
	}
	const char *file_name = get_file_name(gpk_file);
	for (uint32 i = 0; i < level->terrain_count; i += 1) {
		if (!strcmp(level->terrains[i].gpk_file, file_name)) {
			return false;
		}
	}

	terrain *terrain = &level->terrains[level->terrain_count];
	snprintf(terrain->gpk_file, sizeof(terrain->gpk_file), "%s", file_name);
	terrain->width = gpk_terrain->width;
	terrain->height = gpk_terrain->height;
	terrain->max_height = gpk_terrain->max_height;
	terrain->sample_per_meter = gpk_terrain->sample_per_meter;
	{ // height map
		uint32 height_map_width = terrain->width * terrain->sample_per_meter;
		uint32 height_map_height = terrain->height * terrain->sample_per_meter;
		uint32 height_map_size = height_map_width * height_map_height * sizeof(int16);

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R16_SNORM;
		image_info.extent = {height_map_width, height_map_width, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;

		int16 *height_map = (int16 *)(gpk_file_mapping.ptr + gpk_terrain->height_map_offset);
		terrain->height_map_image_index = append_vulkan_images(vulkan, image_info, image_view_info, (uint8 *)height_map, height_map_size, 1, 2);
		VkImageView image_view = vulkan->memory_regions.images.images[terrain->height_map_image_index].view;
		terrain->height_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image_view, vulkan->samplers.terrain_height_map_sampler);

		terrain->height_map = allocate_memory<int16>(&level->general_memory_arena, height_map_width * height_map_height);
		memcpy(terrain->height_map, height_map, height_map_size);
		terrain->bt_terrain_shape = allocate_memory<btHeightfieldTerrainShape>(&level->general_memory_arena, 1);
		new(terrain->bt_terrain_shape) btHeightfieldTerrainShape(height_map_width, height_map_height, terrain->height_map, terrain->max_height / INT16_MAX, -terrain->max_height, terrain->max_height, 1, PHY_SHORT, false);
		float scaling = 1.0f / terrain->sample_per_meter;
		terrain->bt_terrain_shape->setLocalScaling(btVector3(scaling, 1, scaling));
		terrain->bt_collision_object = allocate_memory<btCollisionObject>(&level->general_memory_arena, 1);
		new(terrain->bt_collision_object) btCollisionObject();
		terrain->bt_collision_object->setCollisionShape(terrain->bt_terrain_shape);
		terrain->bt_collision_object->setFriction(0.1f);
	}
	{ // diffuse map
		uint32 diffuse_map_width = terrain->width * terrain->sample_per_meter;
		uint32 diffuse_map_height = terrain->height * terrain->sample_per_meter;
		uint32 diffuse_map_size = diffuse_map_width * diffuse_map_height * 4;

		VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_info.extent = {diffuse_map_width, diffuse_map_height, 1};
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		image_info.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_info.format;
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.subresourceRange.layerCount = 1;

		uint32 *diffuse_map = (uint32 *)(gpk_file_mapping.ptr + gpk_terrain->diffuse_map_offset);
		terrain->diffuse_map_image_index = append_vulkan_images(vulkan, image_info, image_view_info, (uint8 *)diffuse_map, diffuse_map_size, 1, 4);
		VkImageView image_view = vulkan->memory_regions.images.images[terrain->diffuse_map_image_index].view;
		terrain->diffuse_map_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image_view, vulkan->samplers.terrain_diffuse_map_sampler);
		terrain->diffuse_map_uv_repeat = 1.0f;
		
		terrain->diffuse_map = nullptr;
		if (store_diffuse_map) {
			terrain->diffuse_map = allocate_memory<uint32>(&level->general_memory_arena, diffuse_map_width * diffuse_map_height);
			memcpy(terrain->diffuse_map, diffuse_map, diffuse_map_size);
		}
	}
	{ // vertices
		vec2 position = {terrain->width * -0.5f, terrain->height * -0.5f};
		vec2 uv = {0, 0};
		uint32 vertex_x_count = terrain->width * terrain->sample_per_meter + 1;
		uint32 vertex_y_count = terrain->height * terrain->sample_per_meter + 1;
		uint32 vertex_count = vertex_x_count * vertex_y_count;
		uint32 index_count = (vertex_x_count + 1) * 2 * (vertex_y_count - 1);
		float dpos = 1.0f / terrain->sample_per_meter;
		float duv_x = 1.0f / (vertex_x_count - 1);
		float duv_y = 1.0f / (vertex_y_count - 1);
		{
			m_memory_arena_undo_allocations_at_scope_exit(&level->general_memory_arena);
			terrain_vertex *vertices = allocate_memory<terrain_vertex>(&level->general_memory_arena, vertex_count);
			for (uint32 i = 0; i < vertex_y_count; i += 1) {
				for (uint32 j = 0; j < vertex_x_count; j += 1) {
					vertices[vertex_x_count * i + j] = {position, uv};
					position.x += dpos;
					uv.x += duv_x;
				}
				position.x = terrain->width * -0.5f;
				position.y += dpos;
				uv.x = 0;
				uv.y += duv_y;
			}
			terrain->vertex_buffer_offset = append_vulkan_vertex_buffer(vulkan, vertices, vertex_count * sizeof(struct terrain_vertex), sizeof(struct terrain_vertex));
		}
		{
			m_memory_arena_undo_allocations_at_scope_exit(&level->general_memory_arena);
			uint32 *indices = allocate_memory<uint32>(&level->general_memory_arena, index_count);
			uint32 *index = indices;
			for (uint32 i = 0; i < (vertex_y_count - 1); i += 1) {
				uint32 n = vertex_x_count * i;
				for (uint32 j = 0; j < vertex_x_count; j += 1) {
					index[0] = n;
					index[1] = n + vertex_x_count;
					index += 2;
					n += 1;
				}
				index[0] = (vertex_x_count * i) + (vertex_x_count * 2 - 1);
				index[1] = (vertex_x_count * i) + vertex_x_count;
				index += 2;
			}
			terrain->index_buffer_offset = append_vulkan_vertex_buffer(vulkan, indices, index_count * sizeof(uint32), sizeof(uint32));
			terrain->index_count = index_count;
		}
	}
	level->terrain_count += 1;
	return true;
}

template <typename T>
bool load_level(level *level, vulkan *vulkan, const char *file, bool editor, T load_editor_settings) {
	using namespace flatbuffers;

	file_mapping file_mapping = {};
	if (!open_file_mapping(file, &file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(&file_mapping));

	auto *fb_level = GetLevel(file_mapping.ptr);
	if (!fb_level) {
		return false;
	}

	reset_level(level, vulkan);

	auto load_generic_entity = [level](const GenericEntity *src_entity, generic_entity *dst_entity) {
		auto id = src_entity->id();
		m_assert(id, "");
		dst_entity->id = allocate_memory<char>(&level->general_memory_arena, id->Length() + 1);
		strcpy(dst_entity->id, id->c_str());
		
		dst_entity->model_index = src_entity->modelIndex();

		const Transform *transform = src_entity->transform();
		if (transform) {
			memcpy(&dst_entity->transform, transform, sizeof(struct transform));
		}
		else {
			dst_entity->transform = transform_identity();
		}

		const Collision *collision = src_entity->collision();
		if (!collision) {
			dst_entity->collision_shape = nullptr;
		}
		else {
			CollisionShape shape = collision->shape_type();
			if (shape == CollisionShape_Sphere) {
				const Sphere *sphere = collision->shape_as_Sphere();
				btSphereShape *bt_sphere = allocate_memory<btSphereShape>(&level->general_memory_arena, 1);
				new(bt_sphere) btSphereShape(sphere->radius());
				dst_entity->collision_shape = bt_sphere;
			}
			else if (shape == CollisionShape_Box) {
				const Box *box = collision->shape_as_Box();
				Vec3 half_extents = box->halfExtents();
				btBoxShape *bt_box = allocate_memory<btBoxShape>(&level->general_memory_arena, 1);
				new(bt_box) btBoxShape(btVector3(half_extents.x(), half_extents.y(), half_extents.z()));
				dst_entity->collision_shape = bt_box;
			}
			else if (shape == CollisionShape_CapsuleX) {
				const CapsuleX *capsule_x = collision->shape_as_CapsuleX();
				btCapsuleShapeX *bt_capsule_x = allocate_memory<btCapsuleShapeX>(&level->general_memory_arena, 1);
				new(bt_capsule_x) btCapsuleShapeX(capsule_x->radius(), capsule_x->halfHeight() * 2);
				dst_entity->collision_shape = bt_capsule_x;
			}
			else if (shape == CollisionShape_CapsuleY) {
				const CapsuleY *capsule_y = collision->shape_as_CapsuleY();
				btCapsuleShape *bt_capsule_y = allocate_memory<btCapsuleShape>(&level->general_memory_arena, 1);
				new(bt_capsule_y) btCapsuleShape(capsule_y->radius(), capsule_y->halfHeight() * 2);
				dst_entity->collision_shape = bt_capsule_y;
			}
			else if (shape == CollisionShape_CapsuleZ) {
				const CapsuleZ *capsule_z = collision->shape_as_CapsuleZ();
				btCapsuleShapeZ *bt_capsule_z = allocate_memory<btCapsuleShapeZ>(&level->general_memory_arena, 1);
				new(bt_capsule_z) btCapsuleShapeZ(capsule_z->radius(), capsule_z->halfHeight() * 2);
				dst_entity->collision_shape = bt_capsule_z;
			}
			else if (shape == CollisionShape_CylinderX) {
				const CylinderX *cylinder_x = collision->shape_as_CylinderX();
				btCylinderShapeX *bt_cylinder_x = allocate_memory<btCylinderShapeX>(&level->general_memory_arena, 1);
				new(bt_cylinder_x) btCylinderShapeX(btVector3(cylinder_x->halfHeight(), cylinder_x->radius(), cylinder_x->radius()));
				dst_entity->collision_shape = bt_cylinder_x;
			}
			else if (shape == CollisionShape_CylinderY) {
				const CylinderY *cylinder_y = collision->shape_as_CylinderY();
				btCylinderShape *bt_cylinder_y = allocate_memory<btCylinderShape>(&level->general_memory_arena, 1);
				new(bt_cylinder_y) btCylinderShape(btVector3(cylinder_y->radius(), cylinder_y->halfHeight(), cylinder_y->radius()));
				dst_entity->collision_shape = bt_cylinder_y;
			}
			else if (shape == CollisionShape_CylinderZ) {
				const CylinderZ *cylinder_z = collision->shape_as_CylinderZ();
				btCylinderShapeZ *bt_cylinder_z = allocate_memory<btCylinderShapeZ>(&level->general_memory_arena, 1);
				new(bt_cylinder_z) btCylinderShapeZ(btVector3(cylinder_z->radius(), cylinder_z->radius(), cylinder_z->halfHeight()));
				dst_entity->collision_shape = bt_cylinder_z;
			}
			else {
				m_assert(false, "");
			}
		}

		const RigidBody *rigid_body = src_entity->rigidBody();
		if (collision && rigid_body) {
			dst_entity->collision_object = allocate_memory<btRigidBody>(&level->general_memory_arena, 1);
			new(dst_entity->collision_object) btRigidBody(rigid_body->mass(), nullptr, dst_entity->collision_shape);
		}
		else if (collision) {
			dst_entity->collision_object = allocate_memory<btCollisionObject>(&level->general_memory_arena, 1);
			new(dst_entity->collision_object) btCollisionObject();
			dst_entity->collision_object->setCollisionShape(dst_entity->collision_shape);
		}
		else if (rigid_body) {
			dst_entity->collision_object = allocate_memory<btRigidBody>(&level->general_memory_arena, 1);
			new(dst_entity->collision_object) btRigidBody(rigid_body->mass(), nullptr, nullptr);
		}

		if (dst_entity->collision_object) {
			btQuaternion rotate(m_unpack4(dst_entity->transform.rotate));
			btVector3 translate(m_unpack3(dst_entity->transform.translate));
			dst_entity->collision_object->setWorldTransform(btTransform(rotate, translate));
		}
	};
	
	auto player = fb_level->player();
	if (player) {
		load_generic_entity(player->genericEntity(), &level->player);
		if (level->player.collision_object) {
			level->player.collision_object->setActivationState(DISABLE_DEACTIVATION);
		}
	}

	auto generic_entities = fb_level->genericEntities();
	if (generic_entities) {
		m_assert(generic_entities->size() <= level->generic_entity_capacity, "");
		level->generic_entity_count = generic_entities->size();
		for (uint32 i = 0; i < generic_entities->size(); i += 1) {
			load_generic_entity((*generic_entities)[i], &level->generic_entities[i]);
		}
	}

	auto lights = fb_level->lights();
	if (lights) {
		auto ambient_light = lights->ambientLight();
		if (ambient_light) {
			level->lights.ambient_light.color = {ambient_light->color().x(), ambient_light->color().y(), ambient_light->color().z()};
		}
		auto direct_light = lights->directLight();
		if (direct_light) {
			level->lights.direct_light.color = {direct_light->color().x(), direct_light->color().y(), direct_light->color().z()};
			level->lights.direct_light.direction = {direct_light->direction().x(), direct_light->direction().y(), direct_light->direction().z()};
		}
	}	
	
	auto terrains = fb_level->terrains();
	if (terrains) {
		m_assert(terrains->size() <= level->terrain_capacity, "");
		for (auto terrain : *terrains) {
			m_assert(terrain->gpkFile(), "");
			char file_path[128] = "assets/terrains/";
			m_assert(strlen(file_path) + terrain->gpkFile()->Length() < sizeof(file_path), "");
			strcat(file_path, terrain->gpkFile()->c_str());
			m_assert(add_level_terrain(level, vulkan, file_path, editor), "%s", terrain->gpkFile()->c_str());
		}
	}

	auto skyboxes = fb_level->skyboxes();
	if (skyboxes) {
		m_assert(skyboxes->size() <= level->skybox_capacity, "");
		for (auto skybox : *skyboxes) {
			m_assert(skybox->gpkFile(), "");
			char file_path[128] = "assets/skyboxes/";
			m_assert(strlen(file_path) + skybox->gpkFile()->Length() < sizeof(file_path), "");
			strcat(file_path, skybox->gpkFile()->c_str());
			m_assert(add_level_skybox(level, vulkan, file_path), "%s", skybox->gpkFile()->c_str());
		}
	}

	auto models = fb_level->models();
	if (models) {
		m_assert(models->size() <= level->model_capacity, "");
		for (auto model : *models) {
			m_assert(model->gpkFile(), "");
			char file_path[128] = "assets/models/";
			m_assert(strlen(file_path) + model->gpkFile()->Length() < sizeof(file_path), "");
			strcat(file_path, model->gpkFile()->c_str());
			transform transform = transform_identity();
			if (model->transform()) {
				memcpy(&transform, model->transform(), sizeof(mat4));
			}
			m_assert(add_level_model(level, vulkan, file_path, transform, editor), "%s", model->gpkFile()->c_str());
		}
	}

	load_editor_settings(fb_level);
	return true;
}

template <typename T>
bool save_level(level *level, const char *save_file, T save_editor_settings) {
	using namespace flatbuffers;
	FlatBufferBuilder builder;

	auto build_generic_entity = [level, &builder](generic_entity *entity) {
		m_assert(entity->id, "");
		Offset<String> id = builder.CreateString(entity->id);
		
		Transform transform;
		memcpy(&transform, &entity->transform, sizeof(struct transform));

		Offset<Collision> collision = 0;
		if (entity->collision_shape) {
			int shape = entity->collision_shape->getShapeType();
			if (shape == SPHERE_SHAPE_PROXYTYPE) {
				btSphereShape *bt_sphere = (btSphereShape *)entity->collision_shape;
				float radius = bt_sphere->getRadius();
				auto sphere = builder.CreateStruct(Sphere(radius)).Union();
				collision = CreateCollision(builder, CollisionShape_Sphere, sphere);
			}
			else if (shape == BOX_SHAPE_PROXYTYPE) {
				btBoxShape *bt_box = (btBoxShape *)entity->collision_shape;
				btVector3 half_extents = bt_box->getHalfExtentsWithMargin();
				auto box = builder.CreateStruct(Box(Vec3(m_unpack3(half_extents)))).Union();
				collision = CreateCollision(builder, CollisionShape_Box, box);
			}
			else if (shape == CAPSULE_SHAPE_PROXYTYPE) {
				btCapsuleShape *bt_capsule = (btCapsuleShape *)entity->collision_shape;
				int32 up_axis = bt_capsule->getUpAxis();
				float radius = bt_capsule->getRadius();
				float half_height = bt_capsule->getHalfHeight();
				if (up_axis == 0) {
					auto capsule = builder.CreateStruct(CapsuleX(radius, half_height)).Union();
					collision = CreateCollision(builder, CollisionShape_CapsuleX, capsule);
				}
				else if (up_axis == 1) {
					auto capsule = builder.CreateStruct(CapsuleY(radius, half_height)).Union();
					collision = CreateCollision(builder, CollisionShape_CapsuleY, capsule);
				}
				else if (up_axis == 2) {
					auto capsule = builder.CreateStruct(CapsuleZ(radius, half_height)).Union();
					collision = CreateCollision(builder, CollisionShape_CapsuleZ, capsule);
				}
				else {
					m_assert(false, "invalid player entity collision capsule axis");
				}
			}
			else if (shape == CYLINDER_SHAPE_PROXYTYPE) {
				btCylinderShape *bt_cylinder = (btCylinderShape *)entity->collision_shape;
				int32 up_axis = bt_cylinder->getUpAxis();
				if (up_axis == 0) {
					float radius = bt_cylinder->getHalfExtentsWithMargin().y();
					float half_height = bt_cylinder->getHalfExtentsWithMargin().x();
					auto cylinder = builder.CreateStruct(CylinderX(radius, half_height)).Union();
					collision = CreateCollision(builder, CollisionShape_CylinderX, cylinder);
				}
				else if (up_axis == 1) {
					float radius = bt_cylinder->getHalfExtentsWithMargin().x();
					float half_height = bt_cylinder->getHalfExtentsWithMargin().y();
					auto cylinder = builder.CreateStruct(CylinderY(radius, half_height)).Union();
					collision = CreateCollision(builder, CollisionShape_CylinderY, cylinder);
				}
				else if (up_axis == 2) {
					float radius = bt_cylinder->getHalfExtentsWithMargin().x();
					float half_height = bt_cylinder->getHalfExtentsWithMargin().z();
					auto cylinder = builder.CreateStruct(CylinderZ(radius, half_height)).Union();
					collision = CreateCollision(builder, CollisionShape_CylinderZ, cylinder);
				}
				else {
					m_assert(false, "invalid player entity collision cylinder axis");
				}
			}
			else {
				m_assert(false, "invalid player entity collision shape");
			}
		}

		Offset<RigidBody> rigid_body = 0;
		if (entity->collision_object && entity->collision_object->getInternalType() == btCollisionObject::CO_RIGID_BODY) {
			btRigidBody *bt_rigid_body = (btRigidBody *)entity->collision_object;
			float mass = 1.0f / bt_rigid_body->getInvMass();
			rigid_body = CreateRigidBody(builder, mass);
		}
		
		return CreateGenericEntity(builder, id, entity->model_index, &transform, collision, rigid_body);
	};

	Offset<Player> player = CreatePlayer(builder, build_generic_entity(&level->player));

	std::vector<Offset<GenericEntity>> generic_entities(level->generic_entity_count);
	for (uint32 i = 0; i < level->generic_entity_count; i += 1) {
		generic_entities[i] = build_generic_entity(&level->generic_entities[i]);
	}

	AmbientLight ambient_light(Vec3(m_unpack3(level->lights.ambient_light.color)));
	DirectLight direct_light(Vec3(m_unpack3(level->lights.direct_light.color)), Vec3(m_unpack3(level->lights.direct_light.direction)));
	Offset<Lights> lights = CreateLights(builder, &ambient_light, &direct_light);
	
	std::vector<Offset<Terrain>> terrain_files(level->terrain_count);
	for (uint32 i = 0; i < level->terrain_count; i += 1) {
		terrain_files[i] = CreateTerrainDirect(builder, level->terrains[i].gpk_file);
	}

	std::vector<Offset<Skybox>> skybox_files(level->skybox_count);
	for (uint32 i = 0; i < level->skybox_count; i += 1) {
		skybox_files[i] = CreateSkyboxDirect(builder, level->skyboxes[i].gpk_file);
	}

	std::vector<Offset<Model>> model_files(level->model_count);
	for (uint32 i = 0; i < level->model_count; i += 1) {
		Transform transform;
		memcpy(&transform, &level->models[i].transform, sizeof(struct transform));
		model_files[i] = CreateModelDirect(builder, level->models[i].gpk_file, &transform);
	}
	
	EditorSettings editor_settings = save_editor_settings();

	auto lvl = CreateLevelDirect(builder, player, &generic_entities, lights, &terrain_files, &skybox_files, &model_files, &editor_settings);
	FinishLevelBuffer(builder, lvl);

	file_mapping save_file_mapping;
	if (!create_file_mapping(save_file, builder.GetSize(), &save_file_mapping)) {
		return false;
	}
	memcpy(save_file_mapping.ptr, builder.GetBufferPointer(), builder.GetSize());
	flush_file_mapping(save_file_mapping);
	close_file_mapping(&save_file_mapping);
	return true;
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

void generate_entity_model_render_data(level *level, vulkan *vulkan, generic_entity *entity, uint32 animation_index, double animation_time) {
	if (entity->model_index >= level->model_count) {
		return;
	}
	model_render_data *model_render_data = &level->frame_render_data.models[level->frame_render_data.model_count++];
	model_render_data->model_index = entity->model_index;

	model model = level->models[entity->model_index];

	shader_model_info model_info = {};
	model_info.model_mat = mat4_from_transform(entity->transform) * mat4_from_transform(model.transform);
	model_render_data->frame_uniform_buffer_offset = append_vulkan_frame_uniform_buffer(vulkan, &model_info, sizeof(model_info));

	if (animation_index < model.animation_count) {
		model_animation *animation = &model.animations[animation_index];
		model_node *animated_nodes = allocate_memory<struct model_node>(&level->frame_memory_arena, model.node_count);
		memcpy(animated_nodes, model.nodes, model.node_count * sizeof(struct model_node));
		transform *animated_node_transforms = allocate_memory<struct transform>(&level->frame_memory_arena, model.node_count);
		for (uint32 i = 0; i < model.node_count; i += 1) {
			animated_node_transforms[i] = animated_nodes[i].local_transform;
		}
		for (uint32 i = 0; i < animation->channel_count; i += 1) {
			model_animation_channel *channel = &animation->channels[i];
			model_animation_sampler *sampler = &animation->samplers[channel->sampler_index];
			m_assert(sampler->interpolation_type == gpk_model_animation_linear_interpolation, "");
			float time = (float)fmod(animation_time, (double)sampler->key_frames[sampler->key_frame_count - 1].time);
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
		mat4 *joint_mats = allocate_memory<mat4>(&level->frame_memory_arena, skin->joint_count);
		traverse_model_scenes_track_global_transform(&model, [&](model_node *node, uint32 index, mat4 global_transform) {
			for (uint32 i = 0; i < skin->joint_count; i += 1) {
				if (skin->joints[i].node_index == index) {
					joint_mats[i] = global_transform * skin->joints[i].inverse_bind_mat;
					break;
				}
			}
		});
		for (uint32 i = 0; i < skin->joint_count; i += 1) {
			m_assert(joint_mats[i] != mat4{}, "");
		}
		joints_frame_uniform_buffer_offset = append_vulkan_frame_uniform_buffer(vulkan, joint_mats, skin->joint_count * sizeof(mat4));
	}

	model_render_data->mesh_count = 0;
	traverse_model_scenes(&model, [&](model_node *node, uint32 index) {
		if (node->mesh_index < model.mesh_count) {
			model_render_data->mesh_count += 1;
		}
	});
	model_render_data->meshes = allocate_memory<struct mesh_render_data>(&level->frame_memory_arena, model_render_data->mesh_count);
	uint32 mesh_render_data_index = 0;
	traverse_model_scenes_track_global_transform(&model, [&](model_node *node, uint32 index, mat4 global_transform) {
		if (node->mesh_index < model.mesh_count) {
			mesh_render_data *mesh_render_data = &model_render_data->meshes[mesh_render_data_index++];
			mesh_render_data->mesh_index = node->mesh_index;
			if (model.skin_count > 0) {
				mesh_render_data->frame_uniform_buffer_offset = joints_frame_uniform_buffer_offset;
			}
			else {
				mesh_render_data->frame_uniform_buffer_offset = append_vulkan_frame_uniform_buffer(vulkan, &global_transform, sizeof(global_transform));
			}
			model_mesh *mesh = &model.meshes[node->mesh_index];
			mesh_render_data->primitives = allocate_memory<struct primitive_render_data>(&level->frame_memory_arena, mesh->primitive_count);
			for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
				primitive_render_data *primitive_render_data = &mesh_render_data->primitives[i];
				primitive_render_data->primitive_index = i;

				model_mesh_primitive *primitive = &mesh->primitives[i];
				model_material *material = nullptr;
				if (primitive->material_index < model.material_count) {
					material = &model.materials[primitive->material_index];
				}

				shader_primitive_info primitive_info = {};
				primitive_info.diffuse_factor = material ? material->diffuse_factor : vec4{1, 1, 1, 1};
				primitive_info.metallic_factor = material ? material->metallic_factor : 1;
				primitive_info.roughness_factor = material ? material->roughness_factor: 1;
				primitive_render_data->frame_uniform_buffer_offset = append_vulkan_frame_uniform_buffer(vulkan, &primitive_info, sizeof(primitive_info));
			}
		}
	});
}

template <typename F>
void generate_level_render_data(level *level, vulkan *vulkan, camera camera, F generate_extra_render_data) {
	level->frame_render_data = {};
	{ // level
		point_light point_light = {};
		struct camera shadow_map_camera = camera;
		shadow_map_camera.zfar = 100;
		shader_level_info level_info = {};
		level_info.view_proj_mat = mat4_vulkan_clip() * camera_view_projection_mat4(camera);
		level_info.camera_position = vec4{m_unpack3(camera.position.e), 0};
		level_info.shadow_map_proj_mat = mat4_vulkan_clip() * camera_shadow_map_projection_mat4(shadow_map_camera, level->lights.direct_light.direction);
		level_info.ambient_light_color = vec4{m_unpack3(level->lights.ambient_light.color), 0};
		level_info.direct_light_color = vec4{m_unpack3(level->lights.direct_light.color), 0};
		level_info.direct_light_dir = vec4{m_unpack3(level->lights.direct_light.direction), 0};
		level_info.point_light_color = vec4{m_unpack3(point_light.color), 0};
		level_info.point_light_position = vec4{m_unpack3(point_light.position), point_light.attenuation};
		uint32 offset = append_vulkan_frame_uniform_buffer(vulkan, &level_info, sizeof(level_info));
		m_debug_assert(offset == 0, "");
	}
	{ // models
		uint32 estimate_model_count = 1 + level->generic_entity_count;
		level->frame_render_data.models = allocate_memory<struct model_render_data>(&level->frame_memory_arena, estimate_model_count);
		generate_entity_model_render_data(level, vulkan, &level->player, level->player.animation_index, level->player.animation_time);
		for (uint32 i = 0; i < level->generic_entity_count; i += 1) {
			auto *entity = &level->generic_entities[i];
			generate_entity_model_render_data(level, vulkan, entity, UINT32_MAX, 0);
		}
	}
	generate_extra_render_data();
}

template <typename F0, typename F1>
void generate_level_render_commands(level *level, vulkan *vulkan, const camera &camera, F0 extra_color_render_pass_commands, F1 extra_swap_chain_render_pass_command) {
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffers[vulkan->frame_index];
	VkDescriptorSet descriptor_sets[] = {vulkan->descriptor_sets.frame_uniform_buffers[vulkan->frame_index], vulkan->descriptor_sets.image_sampler_2d, vulkan->descriptor_sets.image_sampler_cube};
	vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.pipeline_layout, 0, m_countof(descriptor_sets), descriptor_sets, 0, nullptr);
	{ // shadow passes
		{
			VkClearValue clear_values[] = {{1, 1, 0, 0}, {1, 0}};
			VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
			render_pass_begin_info.renderPass = vulkan->render_passes.shadow_map_render_passes[0];
			render_pass_begin_info.framebuffer = vulkan->framebuffers.shadow_map_framebuffers[vulkan->frame_index].framebuffer;
			render_pass_begin_info.renderArea.offset = {0, 0};
			render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.shadow_map_framebuffer_width, vulkan->framebuffers.shadow_map_framebuffer_height};
			render_pass_begin_info.clearValueCount = m_countof(clear_values);
			render_pass_begin_info.pClearValues = clear_values;
			vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
			VkViewport viewport = {0, 0, (float)vulkan->framebuffers.shadow_map_framebuffer_width, (float)vulkan->framebuffers.shadow_map_framebuffer_height, 0, 1};
			VkRect2D scissor = {{0, 0}, vulkan->framebuffers.shadow_map_framebuffer_width, vulkan->framebuffers.shadow_map_framebuffer_height};
			vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
			vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
			if (level->frame_render_data.model_count > 0) {
				vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_shadow_map_pipeline);
				VkDeviceSize vertex_buffer_offset = 0;
				vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.vertex_buffer.buffer, &vertex_buffer_offset);
				vkCmdBindIndexBuffer(cmd_buffer, vulkan->memory_regions.vertex_buffer.buffer, 0, VK_INDEX_TYPE_UINT16);
				for (auto &model_render_data : make_range(level->frame_render_data.models, level->frame_render_data.model_count)) {
					model &model = level->models[model_render_data.model_index];
					for (auto &mesh_render_data : make_range(model_render_data.meshes, model_render_data.mesh_count)) {
						model_mesh &mesh = model.meshes[mesh_render_data.mesh_index];
						for (auto &primitive_render_data : make_range(mesh_render_data.primitives, mesh.primitive_count)) {
							model_mesh_primitive &primitive = mesh.primitives[primitive_render_data.primitive_index];
							shader_model_push_constant pc = {};
							pc.model_offset = model_render_data.frame_uniform_buffer_offset;
							pc.mesh_offset = mesh_render_data.frame_uniform_buffer_offset;
							pc.primitive_offset = primitive_render_data.frame_uniform_buffer_offset;
							vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
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
			render_pass_begin_info.framebuffer = vulkan->framebuffers.shadow_map_blur_1_framebuffers[vulkan->frame_index].framebuffer;
			render_pass_begin_info.renderArea.offset = {0, 0};
			render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.shadow_map_framebuffer_width, vulkan->framebuffers.shadow_map_framebuffer_height};
			render_pass_begin_info.clearValueCount = m_countof(clear_values);
			render_pass_begin_info.pClearValues = clear_values;
			vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[0]);
		  shader_gaussian_blur_push_constant pc;
		  pc.index = vulkan->framebuffers.shadow_map_blur_1_descriptor_indices[vulkan->frame_index];
		  pc.x_dir = 1;
		  pc.y_dir = 0;
		  vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
			vkCmdDraw(cmd_buffer, 3, 1, 0, 0);
			vkCmdEndRenderPass(cmd_buffer);
		}
		{
			VkClearValue clear_values[] = {{0, 0, 0, 0}};
			VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
			render_pass_begin_info.renderPass = vulkan->render_passes.shadow_map_render_passes[2];
			render_pass_begin_info.framebuffer = vulkan->framebuffers.shadow_map_blur_2_framebuffers[vulkan->frame_index].framebuffer;
			render_pass_begin_info.renderArea.offset = {0, 0};
			render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.shadow_map_framebuffer_width, vulkan->framebuffers.shadow_map_framebuffer_height};
			render_pass_begin_info.clearValueCount = m_countof(clear_values);
			render_pass_begin_info.pClearValues = clear_values;
			vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.shadow_map_gaussian_blur_pipelines[1]);
		  shader_gaussian_blur_push_constant pc;
		  pc.index = vulkan->framebuffers.shadow_map_blur_2_descriptor_indices[vulkan->frame_index];
		  pc.x_dir = 0;
		  pc.y_dir = 1;
		  vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
			vkCmdDraw(cmd_buffer, 3, 1, 0, 0);
			vkCmdEndRenderPass(cmd_buffer);
		}
	}
	{ // color render pass
		VkClearValue clear_values[2] = {{0, 0, 0, 1}, {0, 0}};
		VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
		render_pass_begin_info.renderPass = vulkan->render_passes.color_render_pass;
		render_pass_begin_info.framebuffer = vulkan->framebuffers.color_framebuffers[vulkan->frame_index].framebuffer;
		render_pass_begin_info.renderArea.offset = {0, 0};
		render_pass_begin_info.renderArea.extent = {vulkan->framebuffers.color_framebuffer_width, vulkan->framebuffers.color_framebuffer_height};
		render_pass_begin_info.clearValueCount = m_countof(clear_values);
		render_pass_begin_info.pClearValues = clear_values;
		vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		VkViewport viewport = {0, 0, (float)vulkan->framebuffers.color_framebuffer_width, (float)vulkan->framebuffers.color_framebuffer_height, 1, 0};
		VkRect2D scissor = {{0, 0}, vulkan->framebuffers.color_framebuffer_width, vulkan->framebuffers.color_framebuffer_height};
		vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
		vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
		if (level->frame_render_data.model_count > 0) {
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline);
			VkDeviceSize vertex_buffer_offset = 0;
			vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.vertex_buffer.buffer, &vertex_buffer_offset);
			vkCmdBindIndexBuffer(cmd_buffer, vulkan->memory_regions.vertex_buffer.buffer, 0, VK_INDEX_TYPE_UINT16);
			for (auto &model_render_data : make_range(level->frame_render_data.models, level->frame_render_data.model_count)) {
				model &model = level->models[model_render_data.model_index];
				for (auto &mesh_render_data : make_range(model_render_data.meshes, model_render_data.mesh_count)) {
					model_mesh &mesh = model.meshes[mesh_render_data.mesh_index];
					for (auto &primitive_render_data : make_range(mesh_render_data.primitives, mesh.primitive_count)) {
						model_mesh_primitive &primitive = mesh.primitives[primitive_render_data.primitive_index];
						shader_model_push_constant pc = {};
						pc.model_offset = model_render_data.frame_uniform_buffer_offset;
						pc.mesh_offset = mesh_render_data.frame_uniform_buffer_offset;
						pc.primitive_offset = primitive_render_data.frame_uniform_buffer_offset;
						pc.diffuse_map_index = primitive.material_index < model.material_count ? model.materials[primitive.material_index].diffuse_map_descriptor_index : level->persistant_data.default_diffuse_map_descriptor_index;
						pc.metallic_map_index = primitive.material_index < model.material_count ? model.materials[primitive.material_index].metallic_map_descriptor_index : level->persistant_data.default_metallic_map_descriptor_index;
						pc.roughness_map_index = primitive.material_index < model.material_count ? model.materials[primitive.material_index].roughness_map_descriptor_index : level->persistant_data.default_roughness_map_descriptor_index;
						pc.normal_map_index = primitive.material_index < model.material_count ? model.materials[primitive.material_index].normal_map_descriptor_index : level->persistant_data.default_normal_map_descriptor_index;
						pc.height_map_index = level->persistant_data.default_height_map_descriptor_index;
						pc.shadow_map_index = vulkan->framebuffers.shadow_map_blur_1_descriptor_indices[vulkan->frame_index];
						vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
						vkCmdDrawIndexed(cmd_buffer, primitive.index_count, 1, primitive.index_buffer_offset / sizeof(uint16), primitive.vertex_buffer_offset / sizeof(struct gpk_model_vertex), 0);
					}
				}
			}
		}
		if (level->terrain_index < level->terrain_count) {
			terrain *terrain = &level->terrains[level->terrain_index];
			vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.terrain_pipeline);
			VkDeviceSize vertex_buffer_offset = 0;
			vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->memory_regions.vertex_buffer.buffer, &vertex_buffer_offset);
			vkCmdBindIndexBuffer(cmd_buffer, vulkan->memory_regions.vertex_buffer.buffer, 0, VK_INDEX_TYPE_UINT32);
			shader_terrain_push_constant pc = {};
			pc.sample_per_meter = (float)terrain->sample_per_meter;
			pc.max_height = terrain->max_height;
			pc.height_map_index = terrain->height_map_descriptor_index;
			pc.diffuse_map_index = terrain->diffuse_map_descriptor_index;
			pc.diffuse_map_uv_repeat = terrain->diffuse_map_uv_repeat;
			pc.shadow_map_index = vulkan->framebuffers.shadow_map_blur_1_descriptor_indices[vulkan->frame_index];
			vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
			vkCmdDrawIndexed(cmd_buffer, terrain->index_count, 1, terrain->index_buffer_offset / sizeof(uint32), terrain->vertex_buffer_offset / sizeof(struct terrain_vertex), 0);
		}
		if (level->skybox_index < level->skybox_count) {
		  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.skybox_pipeline);
		  struct camera skybox_camera = camera;
		  skybox_camera.position = {0, 0, 0};
		  shader_skybox_push_constant pc = {};
		  pc.view_proj_mat = mat4_vulkan_clip() * camera_view_projection_mat4(skybox_camera);
		  pc.cube_map_index = level->skyboxes[level->skybox_index].cubemap_descriptor_index;
		  vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
		  vkCmdDraw(cmd_buffer, 36, 1, 0, 0);
		}
		extra_color_render_pass_commands();
		vkCmdEndRenderPass(cmd_buffer);
	}
	{ // swap chain render pass
		VkClearValue clear_value = {0, 0, 0, 1};
		VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
		render_pass_begin_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
		render_pass_begin_info.framebuffer = vulkan->framebuffers.swap_chain_framebuffers[vulkan->swap_chain_image_index];
		render_pass_begin_info.renderArea.offset = {0, 0};
		render_pass_begin_info.renderArea.extent = {vulkan->swap_chain.width, vulkan->swap_chain.height};
		render_pass_begin_info.clearValueCount = 1;
		render_pass_begin_info.pClearValues = &clear_value;
		vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.swap_chain_pipeline);
		VkViewport viewport = {0, 0, (float)vulkan->swap_chain.width, (float)vulkan->swap_chain.height, 0, 1};
		VkRect2D scissor = {{0, 0}, {vulkan->swap_chain.width, vulkan->swap_chain.height}};
		vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
		vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
		shader_swap_chain_push_constant pc = {};
		pc.x = vulkan->swap_chain_framebuffer_region[0] * 2 - 1;
		pc.y = vulkan->swap_chain_framebuffer_region[1] * 2 - 1;
		pc.width = vulkan->swap_chain_framebuffer_region[2] * 2;
		pc.height = vulkan->swap_chain_framebuffer_region[3] * 2;
		pc.texture_index = vulkan->framebuffers.color_descriptor_indices[vulkan->frame_index];
		vkCmdPushConstants(cmd_buffer, vulkan->pipelines.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pc), &pc);
		vkCmdDraw(cmd_buffer, 6, 1, 0, 0);

		extra_swap_chain_render_pass_command();
		vkCmdEndRenderPass(cmd_buffer);
	}
}	
