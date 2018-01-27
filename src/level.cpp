/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

const uint32 level_max_entity_count = 1024;
const uint32 level_max_directional_light_count = 1;
const uint32 level_max_point_light_count = 4;
const uint32 level_max_spot_light_count = 4;

typedef gpk_model_mesh_instance model_mesh_instance;

struct model_mesh {
	uint32 vertex_size;
	uint32 vertex_count;
	uint32 vertex_buffer_offset;
	uint8 *vertices_data;
	uint32 index_size;
	uint32 index_count;
	uint32 index_buffer_offset;
	uint8 *indices_data;
	uint32 material_index;
	model_mesh_instance *instances;
	uint32 instance_count;
	char name[sizeof(gpk_model_mesh_header::name)];
};

struct model_material {
	VkDescriptorSet descriptor_set;
	vulkan_image albedo_map_image;
	vulkan_image normal_map_image;
	vulkan_image metallic_map_image;
	vulkan_image roughness_map_image;
	vulkan_image ao_map_image;
	vulkan_image height_map_image;
	char name[sizeof(gpk_model_material_header::name)];
};

struct model {
	model_mesh *meshes;
	uint32 mesh_count;
	model_material *materials;
	uint32 material_count;
	char file_name[128];
};

struct skybox {
	VkDescriptorSet descriptor_set;
	vulkan_image cubemap_image;
	char file_name[32];
};

struct entity_info {
	char name[32];
};

enum component_flag {
	component_flag_render = 1,
	component_flag_collision = 2,
	component_flag_light = 4,
};

struct render_component {
	uint32 model_index;
	vec2 uv_scale;
	float height_map_scale;
};

struct collision_component {
	aa_bound bound;
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

struct light_component {
	light_type light_type;
	union {
		ambient_light ambient_light;
		directional_light directional_light;
		point_light point_light;
	};
};

struct mesh_render_data {
	uint32 frame_uniforms_buffer_offset;
 	bool render_vertices_outline;
};

struct model_render_data {
	uint32 model_index;
	mesh_render_data *meshes_render_data;
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
	render_component *render_components;
	collision_component *collision_components;
	light_component *light_components;
	uint32 entity_count;
	uint32 render_component_count;
	uint32 collision_component_count;
	uint32 light_component_count;

	uint32 *new_entity_flags;
	entity_info *new_entity_infos;
	transform *new_entity_transforms;
	render_component *new_render_components;
	collision_component *new_collision_components;
	light_component *new_light_components;
	uint32 new_entity_count;
	uint32 new_render_component_count;
	uint32 new_collision_component_count;
	uint32 new_light_component_count;

	model *models;
	uint32 model_count;
	uint32 model_capacity;

	skybox *skyboxes;
	uint32 skybox_count;
	uint32 skybox_capacity;
	uint32 skybox_index;

	camera camera;
	float camera_zoom;
	float camera_rotates[2];
	bool camera_moving;

	level_render_data render_data;
	bool show_frame_stats;

	memory_arena frame_memory_arena;
	memory_arena entity_components_memory_arena;
	memory_arena new_entity_components_memory_arena;
	memory_arena assets_memory_arena;

	char *json_file;
};

void level_initialize(level *level, vulkan *vulkan) {
	level->frame_memory_arena = {};
	level->frame_memory_arena.name = "frame";
	level->frame_memory_arena.capacity = m_megabytes(4);
	m_assert(allocate_virtual_memory(level->frame_memory_arena.capacity, &level->frame_memory_arena.memory));

	level->entity_components_memory_arena = {};
	level->entity_components_memory_arena.name = "entity_components";
	level->entity_components_memory_arena.capacity = m_megabytes(8);
	m_assert(allocate_virtual_memory(level->entity_components_memory_arena.capacity, &level->entity_components_memory_arena.memory));

	level->new_entity_components_memory_arena = {};
	level->new_entity_components_memory_arena.name = "new_entity_components";
	level->new_entity_components_memory_arena.capacity = m_megabytes(8);
	m_assert(allocate_virtual_memory(level->new_entity_components_memory_arena.capacity, &level->new_entity_components_memory_arena.memory));

	level->assets_memory_arena = {};
	level->assets_memory_arena.name = "assets";
	level->assets_memory_arena.capacity = m_megabytes(16);
	m_assert(allocate_virtual_memory(level->assets_memory_arena.capacity, &level->assets_memory_arena.memory));

	level->model_capacity = 1024;
	level->model_count = 0;
	level->models = memory_arena_allocate<struct model>(&level->assets_memory_arena, level->model_capacity);

	level->skybox_capacity = 16;
	level->skybox_count = 0;
	level->skyboxes = memory_arena_allocate<struct skybox>(&level->assets_memory_arena, level->skybox_capacity);

	level->camera.position = vec3{10, 20, 20};
	level->camera.view = vec3_normalize(-level->camera.position);
	level->camera.up = vec3_normalize(vec3_cross(vec3_cross(level->camera.view, vec3{0, 1, 0}), level->camera.view));
	level->camera.fovy = degree_to_radian(58);
	level->camera.aspect = (float)vulkan->framebuffers.main_framebuffers[0].width / (float)vulkan->framebuffers.main_framebuffers[0].height;
	level->camera.znear = 0.1f;
	level->camera.zfar = 2500;
	level->camera_zoom = 3;
	level->camera_rotates[0] = degree_to_radian(30);
	level->camera_rotates[1] = degree_to_radian(0);

	vulkan->buffers.level_vertex_buffer_offset = 0;
	vulkan->memories.level_images_memory.size = 0;
}

render_component *entity_get_render_component(level *level, uint32 entity_index) {
	m_assert(entity_index < level->entity_count);
	m_assert(level->entity_flags[entity_index] & component_flag_render);
	uint32 index = 0;
	for (uint32 i = 0; i < entity_index; i += 1) {
		if (level->entity_flags[i] & component_flag_render) {
			index += 1;
		}
	}
	return &level->render_components[index];
}

collision_component *entity_get_collision_component(level *level, uint32 entity_index) {
	m_assert(entity_index < level->entity_count);
	m_assert(level->entity_flags[entity_index] & component_flag_collision);
	uint32 index = 0;
	for (uint32 i = 0; i < entity_index; i += 1) {
		if (level->entity_flags[i] & component_flag_collision) {
			index += 1;
		}
	}
	return &level->collision_components[index];
}

light_component *entity_get_light_component(level *level, uint32 entity_index) {
	m_assert(entity_index < level->entity_count);
	m_assert(level->entity_flags[entity_index] & component_flag_light);
	uint32 index = 0;
	for (uint32 i = 0; i < entity_index; i += 1) {
		if (level->entity_flags[i] & component_flag_light) {
			index += 1;
		}
	}
	return &level->light_components[index];
}

uint32 component_get_entity_index(level *level, uint32 component_index, component_flag component_flag) {
	uint32 index = 0;
	for (uint32 i = 0; i < level->entity_count; i += 1) {
		if (level->entity_flags[i] & component_flag) {
			if (index == component_index) {
				return i;
			}
			index += 1;
		}
	}
	m_assert(false);
	return UINT32_MAX;
}

uint32 level_get_model_index(level *level, const char *model_file_name) {
	for (uint32 i = 0; i < level->model_count; i += 1) {
		if (!strcmp(level->models[i].file_name, model_file_name)) {
			return i;
		}
	}
	return UINT32_MAX;
}

uint32 level_add_model(level *level, vulkan *vulkan, const char *file_name, bool store_vertices = false) {
	for (uint32 i = 0; i < level->model_count; i += 1) {
		if (!strcmp(level->models[i].file_name, file_name)) {
			return i;
		}
	}
	m_assert(level->model_count < level->model_capacity);

	file_mapping file_mapping = {};
	m_assert(open_file_mapping(file_name, &file_mapping));
	m_scope_exit(close_file_mapping(file_mapping));

	gpk_model_header *gpk_model_header = (struct gpk_model_header *)file_mapping.ptr;
	model *model = &level->models[level->model_count];
	snprintf(model->file_name, sizeof(model->file_name), "%s", file_name);
	model->mesh_count = gpk_model_header->mesh_count;
	model->meshes = memory_arena_allocate<struct model_mesh>(&level->assets_memory_arena, model->mesh_count);
	m_assert(model->meshes);
	for (uint32 i = 0; i < model->mesh_count; i += 1) {
		gpk_model_mesh_header *gpk_model_mesh_header = (struct gpk_model_mesh_header *)((uint8 *)gpk_model_header + gpk_model_header->mesh_offsets[i]);
		gpk_model_mesh_instance *gpk_model_mesh_instances = (struct gpk_model_mesh_instance *)((uint8 *)gpk_model_mesh_header + gpk_model_mesh_header->instances_offset);
		uint8 *gpk_model_mesh_vertices = ((uint8 *)gpk_model_mesh_header) + gpk_model_mesh_header->vertices_offset;
		uint8 *gpk_model_mesh_indices = ((uint8 *)gpk_model_mesh_header) + gpk_model_mesh_header->indices_offset;
		model_mesh *model_mesh = &model->meshes[i];
		*model_mesh = {};
		m_array_copy(model_mesh->name, gpk_model_mesh_header->name);
		model_mesh->material_index = gpk_model_mesh_header->material_index;
		model_mesh->instance_count = gpk_model_mesh_header->instance_count;
		model_mesh->instances = memory_arena_allocate<model_mesh_instance>(&level->assets_memory_arena, model_mesh->instance_count);
		memcpy(model_mesh->instances, gpk_model_mesh_instances, sizeof(struct gpk_model_mesh_instance) * gpk_model_mesh_header->instance_count);
		model_mesh->vertex_size = gpk_model_mesh_header->vertex_size;
		model_mesh->vertex_count = gpk_model_mesh_header->vertex_count;
		vulkan->buffers.level_vertex_buffer_offset = round_up(vulkan->buffers.level_vertex_buffer_offset, model_mesh->vertex_size);
		model_mesh->vertex_buffer_offset = vulkan->buffers.level_vertex_buffer_offset;
		model_mesh->index_size = 2;
		model_mesh->index_count = gpk_model_mesh_header->index_count;
		uint32 vertices_data_size = model_mesh->vertex_size * model_mesh->vertex_count;
		vulkan->buffers.level_vertex_buffer_offset = round_up(vulkan->buffers.level_vertex_buffer_offset + vertices_data_size, 4);
		model_mesh->index_buffer_offset = vulkan->buffers.level_vertex_buffer_offset;
		uint32 indices_data_size = model_mesh->index_size * model_mesh->index_count;
		vulkan->buffers.level_vertex_buffer_offset += indices_data_size;
		vulkan_buffer_transfer(vulkan, &vulkan->buffers.level_vertex_buffer, model_mesh->vertex_buffer_offset, gpk_model_mesh_vertices, vertices_data_size);
		vulkan_buffer_transfer(vulkan, &vulkan->buffers.level_vertex_buffer, model_mesh->index_buffer_offset, gpk_model_mesh_indices, indices_data_size);
		if (store_vertices) {
			model_mesh->vertices_data = memory_arena_allocate<uint8>(&level->assets_memory_arena, vertices_data_size);
			model_mesh->indices_data = memory_arena_allocate<uint8>(&level->assets_memory_arena, indices_data_size);
			memcpy(model_mesh->vertices_data, gpk_model_mesh_vertices, vertices_data_size);
			memcpy(model_mesh->indices_data, gpk_model_mesh_indices, indices_data_size);
		}
	}
	model->material_count = gpk_model_header->material_count;
	model->materials = memory_arena_allocate<struct model_material>(&level->assets_memory_arena, model->material_count);
	m_assert(model->materials);
	for (uint32 i = 0; i < model->material_count; i += 1) {
		gpk_model_material_header *gpk_model_material_header = (struct gpk_model_material_header *)((uint8 *)gpk_model_header + gpk_model_header->material_offsets[i]);
		model_material *model_material = &model->materials[i];
		m_array_copy(model_material->name, gpk_model_material_header->name);
		model_material->albedo_map_image = vulkan->images.default_albedo_map_image;
		model_material->normal_map_image = vulkan->images.default_normal_map_image;
		model_material->metallic_map_image = vulkan->images.default_metallic_map_image;
		model_material->roughness_map_image = vulkan->images.default_roughness_map_image;
		model_material->ao_map_image = vulkan->images.default_ao_map_image;
		model_material->height_map_image = vulkan->images.default_height_map_image;
		if (gpk_model_material_header->albedo_map_mipmap_count > 0) {
			VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_create_info.imageType = VK_IMAGE_TYPE_2D;
			image_create_info.format = VK_FORMAT_BC1_RGB_SRGB_BLOCK;
			image_create_info.extent = {gpk_model_material_header->albedo_map_width, gpk_model_material_header->albedo_map_height, 1};
			image_create_info.mipLevels = gpk_model_material_header->albedo_map_mipmap_count;
			image_create_info.arrayLayers = 1;
			image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = image_create_info.format;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;
			image_view_create_info.subresourceRange.layerCount = 1;
			vulkan_image_create(vulkan, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, &model_material->albedo_map_image);
			uint8 *albedo_map_image_data = ((uint8 *)gpk_model_material_header) + gpk_model_material_header->albedo_map_offset;
			vulkan_image_transfer(vulkan, &model_material->albedo_map_image, albedo_map_image_data, gpk_model_material_header->albedo_map_size, 4, 4);
		}
		if (gpk_model_material_header->normal_map_mipmap_count > 0) {
			VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_create_info.imageType = VK_IMAGE_TYPE_2D;
			image_create_info.format = VK_FORMAT_BC5_UNORM_BLOCK;
			image_create_info.extent = {gpk_model_material_header->normal_map_width, gpk_model_material_header->normal_map_height, 1};
			image_create_info.mipLevels = gpk_model_material_header->normal_map_mipmap_count;
			image_create_info.arrayLayers = 1;
			image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = image_create_info.format;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;
			image_view_create_info.subresourceRange.layerCount = 1;
			vulkan_image_create(vulkan, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, &model_material->normal_map_image);
			uint8 *normal_map_image_data = ((uint8 *)gpk_model_material_header) + gpk_model_material_header->normal_map_offset;
			vulkan_image_transfer(vulkan, &model_material->normal_map_image, normal_map_image_data, gpk_model_material_header->normal_map_size, 8, 4);
		}
		if (gpk_model_material_header->metallic_map_mipmap_count > 0) {
			VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_create_info.imageType = VK_IMAGE_TYPE_2D;
			image_create_info.format = VK_FORMAT_BC4_UNORM_BLOCK;
			image_create_info.extent = {gpk_model_material_header->metallic_map_width, gpk_model_material_header->metallic_map_height, 1};
			image_create_info.mipLevels = gpk_model_material_header->metallic_map_mipmap_count;
			image_create_info.arrayLayers = 1;
			image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = image_create_info.format;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;
			image_view_create_info.subresourceRange.layerCount = 1;
			vulkan_image_create(vulkan, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, &model_material->metallic_map_image);
			uint8 *metallic_map_image_data = ((uint8 *)gpk_model_material_header) + gpk_model_material_header->metallic_map_offset;
			vulkan_image_transfer(vulkan, &model_material->metallic_map_image, metallic_map_image_data, gpk_model_material_header->metallic_map_size, 4, 4);
		}
		if (gpk_model_material_header->roughness_map_mipmap_count > 0) {
			VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_create_info.imageType = VK_IMAGE_TYPE_2D;
			image_create_info.format = VK_FORMAT_BC4_UNORM_BLOCK;
			image_create_info.extent = {gpk_model_material_header->roughness_map_width, gpk_model_material_header->roughness_map_height, 1};
			image_create_info.mipLevels = gpk_model_material_header->roughness_map_mipmap_count;
			image_create_info.arrayLayers = 1;
			image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = image_create_info.format;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;
			image_view_create_info.subresourceRange.layerCount = 1;
			vulkan_image_create(vulkan, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, &model_material->roughness_map_image);
			uint8 *roughness_map_image_data = ((uint8 *)gpk_model_material_header) + gpk_model_material_header->roughness_map_offset;
			vulkan_image_transfer(vulkan, &model_material->roughness_map_image, roughness_map_image_data, gpk_model_material_header->roughness_map_size, 4, 4);
		}
		if (gpk_model_material_header->height_map_mipmap_count > 0) {
			VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			image_create_info.imageType = VK_IMAGE_TYPE_2D;
			image_create_info.format = VK_FORMAT_BC4_UNORM_BLOCK;
			image_create_info.extent = {gpk_model_material_header->height_map_width, gpk_model_material_header->height_map_height, 1};
			image_create_info.mipLevels = gpk_model_material_header->height_map_mipmap_count;
			image_create_info.arrayLayers = 1;
			image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = image_create_info.format;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;
			image_view_create_info.subresourceRange.layerCount = 1;
			vulkan_image_create(vulkan, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, &model_material->height_map_image);
			uint8 *height_map_image_data = ((uint8 *)gpk_model_material_header) + gpk_model_material_header->height_map_offset;
			vulkan_image_transfer(vulkan, &model_material->height_map_image, height_map_image_data, gpk_model_material_header->height_map_size, 4, 4);
		}
		VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
		descriptor_set_allocate_info.descriptorPool = vulkan->descriptors.pool;
		descriptor_set_allocate_info.descriptorSetCount = 1;
		descriptor_set_allocate_info.pSetLayouts = &vulkan->descriptors.sampled_images_set_layouts[5];
		m_vk_assert(vkAllocateDescriptorSets(vulkan->device.device, &descriptor_set_allocate_info, &model_material->descriptor_set));
		VkDescriptorImageInfo descriptor_image_infos[6] = {};
		descriptor_image_infos[0] = {vulkan->samplers.mipmap_image_samplers[model_material->albedo_map_image.mipmap_count], model_material->albedo_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[1] = {vulkan->samplers.mipmap_image_samplers[model_material->normal_map_image.mipmap_count], model_material->normal_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[2] = {vulkan->samplers.mipmap_image_samplers[model_material->metallic_map_image.mipmap_count], model_material->metallic_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[3] = {vulkan->samplers.mipmap_image_samplers[model_material->roughness_map_image.mipmap_count], model_material->roughness_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[4] = {vulkan->samplers.mipmap_image_samplers[model_material->ao_map_image.mipmap_count], model_material->ao_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		descriptor_image_infos[5] = {vulkan->samplers.mipmap_image_samplers[model_material->height_map_image.mipmap_count], model_material->height_map_image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		VkWriteDescriptorSet write_descriptor_sets[6] = {};
		for (uint32 i = 0; i < m_countof(write_descriptor_sets); i += 1) {
			write_descriptor_sets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_sets[i].dstSet = model_material->descriptor_set;
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
	gpk_skybox_header *gpk_skybox_header = (struct gpk_skybox_header *)file_mapping.ptr;

	VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
	image_create_info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	image_create_info.imageType = VK_IMAGE_TYPE_2D;
	image_create_info.format = VK_FORMAT_R8G8B8A8_SRGB;
	image_create_info.extent = {gpk_skybox_header->cubemap_width, gpk_skybox_header->cubemap_height, 1};
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
	vulkan_image_create(vulkan, &vulkan->memories.level_images_memory, image_create_info, image_view_create_info, &skybox->cubemap_image);
	uint8 *cubemap = (uint8 *)gpk_skybox_header + gpk_skybox_header->cubemap_offset;
	uint32 cubemap_size = gpk_skybox_header->cubemap_width * gpk_skybox_header->cubemap_height * 4 * 6;
	vulkan_image_transfer(vulkan, &skybox->cubemap_image, cubemap, cubemap_size, 32, 1);

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
void level_load_json(level *level, vulkan *vulkan, const char *level_json_file, T extra_load, bool store_vertices = false) {
	file_mapping level_json_file_mapping = {};
	m_assert(open_file_mapping(level_json_file, &level_json_file_mapping));
	m_scope_exit(close_file_mapping(level_json_file_mapping));
	level->json_file = memory_arena_allocate<char>(&level->assets_memory_arena, strlen(level_json_file) + 1);
	strcpy(level->json_file, level_json_file);

	rapidjson::Document json_doc;
	rapidjson::ParseResult json_parse_result = json_doc.Parse((const char *)level_json_file_mapping.ptr);
	m_assert(json_parse_result);
	m_assert(!json_doc.HasParseError());

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
	auto read_json_collision_component = [](rapidjson::Value::Object collision_component_json, collision_component *collision_component) {
		rapidjson::Value::Array bound_min = collision_component_json["bound_min"].GetArray();
		rapidjson::Value::Array bound_max = collision_component_json["bound_max"].GetArray();
		collision_component->bound.min.x = bound_min[0].GetFloat();
		collision_component->bound.min.y = bound_min[1].GetFloat();
		collision_component->bound.min.z = bound_min[2].GetFloat();
		collision_component->bound.max.x = bound_max[0].GetFloat();
		collision_component->bound.max.y = bound_max[1].GetFloat();
		collision_component->bound.max.z = bound_max[2].GetFloat();
	};
	auto read_json_light_component = [](rapidjson::Value::Object light_component_json, light_component *light_component) {
		const char *light_type = light_component_json["light_type"].GetString();
		if (!strcmp(light_type, "ambient")) {
			light_component->light_type = light_type_ambient;
			rapidjson::Value::Array color = light_component_json["color"].GetArray();
			light_component->ambient_light.color.r = color[0].GetFloat();
			light_component->ambient_light.color.g = color[1].GetFloat();
			light_component->ambient_light.color.b = color[2].GetFloat();
		}
		else if (!strcmp(light_type, "directional")) {
			light_component->light_type = light_type_directional;
			rapidjson::Value::Array color = light_component_json["color"].GetArray();
			rapidjson::Value::Array direction = light_component_json["direction"].GetArray();
			light_component->directional_light.color.r = color[0].GetFloat();
			light_component->directional_light.color.g = color[1].GetFloat();
			light_component->directional_light.color.b = color[2].GetFloat();
			light_component->directional_light.direction.x = direction[0].GetFloat();
			light_component->directional_light.direction.y = direction[1].GetFloat();
			light_component->directional_light.direction.z = direction[2].GetFloat();
		}
		else if (!strcmp(light_type, "point")) {
			light_component->light_type = light_type_point;
			rapidjson::Value::Array color = light_component_json["color"].GetArray();
			rapidjson::Value::Array position = light_component_json["position"].GetArray();
			float attenuation = light_component_json["attenuation"].GetFloat();
			light_component->point_light.color.r = color[0].GetFloat();
			light_component->point_light.color.g = color[1].GetFloat();
			light_component->point_light.color.b = color[2].GetFloat();
			light_component->point_light.position.x = position[0].GetFloat();
			light_component->point_light.position.y = position[1].GetFloat();
			light_component->point_light.position.z = position[2].GetFloat();
			light_component->point_light.attenuation = attenuation;
		}
		else {
			fatal("invalid json light type");
		}
	};
	auto read_json_render_component = [level](rapidjson::Value::Object render_component_json, render_component *render_component) {
		render_component->model_index = level_get_model_index(level, render_component_json["model_file_name"].GetString());
		if (render_component_json.HasMember("uv_scale")) {
			rapidjson::Value::Array uv_scale = render_component_json["uv_scale"].GetArray();
			render_component->uv_scale[0] = uv_scale[0].GetFloat();
			render_component->uv_scale[1] = uv_scale[1].GetFloat();
		}
		else {
			render_component->uv_scale[0] = 1;
			render_component->uv_scale[1] = 1;
		}
		if (render_component_json.HasMember("height_map_scale")) {
			render_component->height_map_scale = render_component_json["height_map_scale"].GetFloat();
		}
		else {
			render_component->height_map_scale = 0;
		}
	};

	rapidjson::Value::Array models = json_doc["models"].GetArray();
	for (uint32 i = 0; i < models.Size(); i += 1) {
		level_add_model(level, vulkan, models[i].GetString(), store_vertices);
	}
	rapidjson::Value::Array skyboxes = json_doc["skyboxes"].GetArray();
	for (uint32 i = 0; i < skyboxes.Size(); i += 1) {
		level_add_skybox(level, vulkan, skyboxes[i].GetString());
	}
	rapidjson::Value::Array entities_json = json_doc["entities"].GetArray();
	level->entity_count = entities_json.Size();
	level->render_component_count = 0;
	level->collision_component_count = 0;
	level->light_component_count = 0;
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
	}
	level->entity_flags = memory_arena_allocate<uint32>(&level->entity_components_memory_arena, level->entity_count);
	level->entity_infos = memory_arena_allocate<struct entity_info>(&level->entity_components_memory_arena, level->entity_count);
	level->entity_transforms = memory_arena_allocate<struct transform>(&level->entity_components_memory_arena, level->entity_count);
	level->render_components = memory_arena_allocate<struct render_component>(&level->entity_components_memory_arena, level->render_component_count);
	level->collision_components = memory_arena_allocate<struct collision_component>(&level->entity_components_memory_arena, level->collision_component_count);
	level->light_components = memory_arena_allocate<struct light_component>(&level->entity_components_memory_arena, level->light_component_count);

	uint32 render_component_index = 0;
	uint32 collision_component_index = 0;
	uint32 light_component_index = 0;
	for (uint32 i = 0; i < entities_json.Size(); i += 1) {
		rapidjson::Value::Object entity_json = entities_json[i].GetObject();
		uint32 *entity_flag = &level->entity_flags[i];
		entity_info *entity_info = &level->entity_infos[i];
		transform *entity_transform = &level->entity_transforms[i];
		*entity_flag = 0;
		*entity_transform = transform_identity();
		m_assert(snprintf(entity_info->name, sizeof(entity_info->name), "%s", entity_json["name"].GetString()) < sizeof(entity_info->name));
		if (entity_json.HasMember("transform")) {
			read_json_transform_object(entity_json["transform"].GetObject(), entity_transform);
		}
		if (entity_json.HasMember("render_component")) {
			*entity_flag = *entity_flag | component_flag_render;
			read_json_render_component(entity_json["render_component"].GetObject(), &level->render_components[render_component_index++]);
		}
		if (entity_json.HasMember("collision_component")) {
			*entity_flag = *entity_flag | component_flag_collision;
			read_json_collision_component(entity_json["collision_component"].GetObject(), &level->collision_components[collision_component_index++]);
		}
		if (entity_json.HasMember("light_component")) {
			*entity_flag = *entity_flag | component_flag_light;
			read_json_light_component(entity_json["light_component"].GetObject(), &level->light_components[light_component_index++]);
		}
	}
	extra_load(&json_doc);
}

template <typename T>
void level_dump_json(level *level, const char *json_file_path, T extra_dump) {
	rapidjson::StringBuffer json_string;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(json_string);
	writer.SetIndent('\t', 1);
	writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);

	auto write_json_transform_object = [&writer](transform transform) {
		writer.Key("transform");
		writer.StartObject();
		writer.Key("scale");
		writer.StartArray();
		writer.Double(transform.scale.x);
		writer.Double(transform.scale.y);
		writer.Double(transform.scale.z);
		writer.EndArray();
		writer.Key("rotate");
		writer.StartArray();
		writer.Double(transform.rotate.x);
		writer.Double(transform.rotate.y);
		writer.Double(transform.rotate.z);
		writer.Double(transform.rotate.w);
		writer.EndArray();
		writer.Key("translate");
		writer.StartArray();
		writer.Double(transform.translate.x);
		writer.Double(transform.translate.y);
		writer.Double(transform.translate.z);
		writer.EndArray();
		writer.EndObject();
	};
	auto write_json_bound_object = [&writer](aa_bound bound) {
		writer.Key("bound");
		writer.StartObject();
		writer.Key("min");
		writer.StartArray();
		writer.Double(bound.min.x);
		writer.Double(bound.min.y);
		writer.Double(bound.min.z);
		writer.EndArray();
		writer.Key("max");
		writer.StartArray();
		writer.Double(bound.max.x);
		writer.Double(bound.max.y);
		writer.Double(bound.max.z);
		writer.EndArray();
		writer.EndObject();
	};
	auto write_json_collision_component = [&writer](collision_component *component) {
		writer.StartObject();
		writer.Key("bound_min");
		writer.StartArray();
		writer.Double(component->bound.min.x);
		writer.Double(component->bound.min.y);
		writer.Double(component->bound.min.z);
		writer.EndArray();
		writer.Key("bound_max");
		writer.StartArray();
		writer.Double(component->bound.max.x);
		writer.Double(component->bound.max.y);
		writer.Double(component->bound.max.z);
		writer.EndArray();
		writer.EndObject();
	};
	auto write_json_light_component = [&writer](light_component *component) {
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
	auto write_json_render_component = [&writer, level](render_component *component) {
		writer.StartObject();
		writer.Key("model_file_name");
		writer.String(level->models[component->model_index].file_name);
		writer.Key("uv_scale");
		writer.StartArray();
		writer.Double(component->uv_scale[0]);
		writer.Double(component->uv_scale[1]);
		writer.EndArray();
		writer.Key("height_map_scale");
		writer.Double(component->height_map_scale);
		writer.EndObject();
	};

	writer.StartObject();
	writer.Key("models");
	writer.StartArray();
	for (uint32 i = 0; i < level->model_count; i += 1) {
		writer.String(level->models[i].file_name);
	}
	writer.EndArray();
	writer.Key("skyboxes");
	writer.StartArray();
	for (uint32 i = 0; i < level->skybox_count; i += 1) {
		writer.String(level->skyboxes[i].file_name);
	}
	writer.EndArray();
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
		write_json_transform_object(entity_transform);
		if (entity_flag & component_flag_collision) {
			writer.Key("collision_component");
			write_json_collision_component(entity_get_collision_component(level, i));
		}
		if (entity_flag & component_flag_light) {
			writer.Key("light_component");
			write_json_light_component(entity_get_light_component(level, i));
		}
		if (entity_flag & component_flag_render) {
			writer.Key("render_component");
			write_json_render_component(entity_get_render_component(level, i));
		}
		writer.EndObject();
	}
	writer.EndArray();
	extra_dump(&writer);
	writer.EndObject();

	file_mapping file_mapping = {};
	create_file_mapping(json_file_path, json_string.GetSize(), &file_mapping);
	memcpy(file_mapping.ptr, json_string.GetString(), json_string.GetSize());
	close_file_mapping(file_mapping);
}

void level_update_player_movement(level *level, double last_frame_time) {
	// player *player = &level->players[level->current_player_index];
	// vec3 player_facing = quat_to_mat3(player->transform.rotation) * vec3{0, 0, 1};
	// vec3 player_facing_left = vec3_normalize(vec3_cross(vec3{0, 1, 0}, player_facing));
	// bool player_jumping = player->current_jump_time > 0;
	// bool player_falling = false;
	// float player_fall_speed = 10.0;
	// double player_fall_time = 0;
	// {
	//   terrain *terrain = &level->terrains[level->current_terrain_index];
	//   vec3 player_bound_bottom_center = aa_bound_bottom_center(aa_bound_transform(player->bound, player->transform));
	//   double px = clamp((double)player_bound_bottom_center.x / (double)terrain->size + 0.5, 0.0, 1.0);
	//   double py = clamp((double)player_bound_bottom_center.z / (double)terrain->size + 0.5, 0.0, 1.0);
	//   uint32 x = (uint32)(terrain->height_map_size * px);
	//   uint32 y = (uint32)(terrain->height_map_size * py);
	//   float terrain_height = terrain->height_map[terrain->height_map_size * y + x];
	//   float player_terrain_delta_height = player_bound_bottom_center.y - terrain_height;
	//   player_fall_time = (double)player_terrain_delta_height / (double)player_fall_speed;
	//   player_falling = !player_jumping && player_fall_time > last_frame_time;
	//   if (!player_jumping && player_fall_time <= last_frame_time) {
	//     player->transform.translation.y -= player_terrain_delta_height;
	//  }
	//}
	// if (player_falling) {
	//   player->transform.translation.y -= (float)last_frame_time * player_fall_speed;
	//}
	// else if (player_jumping) {
	//   player->transform.translation += player->velocity * (float)last_frame_time;
	//   double player_new_jump_time = max(player->current_jump_time - last_frame_time, 0.0);
	//   player->transform.translation.y += (float)(sin(player_new_jump_time / player->jump_time * M_PI) - sin(player->current_jump_time / player->jump_time * M_PI)) * player->jump_height;
	//   player->current_jump_time = player_new_jump_time;
	//}
	// else {
	//   player->velocity = {0, 0, 0};
	//   if (ImGui::IsKeyDown('W')) {
	//     player->velocity += player_facing * player->walk_speed;
	//  }
	//   if (ImGui::IsKeyDown('S')) {
	//     player->velocity -= player_facing * player->walk_speed;
	//  }
	//   if (ImGui::IsKeyDown('A')) {
	//     player->velocity += player_facing_left * player->walk_speed;
	//  }
	//   if (ImGui::IsKeyDown('D')) {
	//     player->velocity -= player_facing_left * player->walk_speed;        
	//  }
	//   if (ImGui::IsKeyPressed(' ')) {
	//     player->current_jump_time = player->jump_time;
	//  }
	//   player->transform.translation += player->velocity * (float)last_frame_time;
	// double player_new_jump_time = max(player->current_jump_time - last_frame_time, 0.0);
	// player->transform.translation.y += (float)(sin(player_new_jump_time / player->jump_time * M_PI) - sin(player->current_jump_time / player->jump_time * M_PI)) * player->jump_height;
	// player->current_jump_time = player_new_jump_time;
	//}
}

void level_update_player_camera(level *level, double last_frame_time, int32 raw_mouse_dx, int32 raw_mouse_dy) {
	// player *player = &level->players[level->current_player_index];

	// if (ImGui::GetIO().KeyCtrl && ImGui::GetIO().MouseWheel != 0) {
	//   level->camera_zoom -= ImGui::GetIO().MouseWheel * 0.1f;
	//   level->camera_zoom = clamp(level->camera_zoom, 1.0f, 16.0f);
	//}
	// else if (ImGui::GetIO().KeyCtrl && (ImGui::IsKeyDown(VK_UP) || ImGui::IsKeyDown(VK_DOWN))) {
	//   level->camera_zoom -=  (ImGui::IsKeyDown(VK_UP) ? (float)last_frame_time : -(float)last_frame_time) * 3.0f;
	//   level->camera_zoom = clamp(level->camera_zoom, 1.0f, 16.0f);
	//}

	// if (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) {
	//   pin_cursor(true);
	//   level->camera_moving = true;
	//}
	// if (level->camera_moving) {
	//   if ((ImGui::IsMouseReleased(0) && !ImGui::IsMouseDown(1)) || (ImGui::IsMouseReleased(1) && !ImGui::IsMouseDown(0))) {
	//     pin_cursor(false);
	//     level->camera_moving = false;
	//  }
	//   else {
	//     float rotate_sensitivity = 0.003f;
	//     level->camera_rotates[0] += raw_mouse_dy * rotate_sensitivity;
	//     level->camera_rotates[0] = clamp(level->camera_rotates[0], degree_to_radian(0), degree_to_radian(80));
	//     if (ImGui::IsMouseDown(0)) {
	//       level->camera_rotates[1] += raw_mouse_dx * rotate_sensitivity;
	//       level->camera_rotates[1] = wrap_angle(level->camera_rotates[1]);
	//    }
	//     else if (ImGui::IsMouseDown(1)) {
	//       if (raw_mouse_dx != 0 || raw_mouse_dy != 0) {
	//         player->transform.rotation = quat_compose(player->transform.rotation, quat_from_rotation(vec3{0, -1, 0}, level->camera_rotates[1]));
	//         level->camera_rotates[1] = 0;
	//         player->transform.rotation = quat_compose(player->transform.rotation, quat_from_rotation(vec3{0, -1, 0}, raw_mouse_dx * rotate_sensitivity));
	//      }
	//    }
	//  }
	//}

	// vec3 player_facing = quat_to_mat3(player->transform.rotation) * vec3{0, 0, 1};
	// vec3 player_facing_left = vec3_normalize(vec3_cross(vec3{0, 1, 0}, player_facing));
	// aa_bound player_bound = aa_bound_transform(player->bound, player->transform);

	// vec3 camera_translate = -player_facing * (player_bound.max.z - player_bound.min.z) * level->camera_zoom;
	// camera_translate = mat3_from_rotation(vec3{player_facing_left}, level->camera_rotates[0]) * camera_translate;
	// camera_translate = mat3_from_rotation(vec3{0, -1, 0}, level->camera_rotates[1]) * camera_translate;

	// level->camera.position = aa_bound_center(player_bound);
	// level->camera.position += camera_translate;
	// level->camera.view = vec3_normalize(-camera_translate);
	// level->camera.up = vec3_normalize(vec3_cross(vec3_cross(level->camera.view, vec3{0, 1, 0}), level->camera.view));
}

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
	{// common uniform
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
		vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index]= round_up(vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], uniform_alignment);
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
		vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index] = round_up(vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], uniform_alignment);
		level->render_data.models = memory_arena_allocate<struct model_render_data>(&level->frame_memory_arena, level->render_component_count);
		level->render_data.model_count = 0;
		auto add_model_render_data = [level, vulkan, uniform_alignment](render_component *render_component, mat4 transform) {
			model_render_data *model_render_data = &level->render_data.models[level->render_data.model_count++];
			model_render_data->model_index = render_component->model_index;
			m_assert(render_component->model_index < level->model_count);
			model *model = &level->models[render_component->model_index];
			model_render_data->meshes_render_data = memory_arena_allocate<mesh_render_data>(&level->frame_memory_arena, model->mesh_count);
			for (uint32 i = 0; i < model->mesh_count; i += 1) {
				model_mesh *model_mesh = &model->meshes[i];
				struct uniforms_t {
					mat4 model_mat;
					mat4 normal_mat;
					vec2 uv_scale;
					float roughness;
					float metallic;
					float height_map_scale;
				} *uniforms = (struct uniforms_t *)(vulkan->buffers.frame_uniform_buffer_ptrs[vulkan->frame_index] + vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index]);
				uniforms->model_mat = transform * model_mesh->instances[0].transform;
				uniforms->normal_mat = mat4_transpose(mat4_inverse(uniforms->model_mat));
				uniforms->uv_scale = render_component->uv_scale;
				uniforms->roughness = 1.0f;
				uniforms->metallic = 0.0f;
				uniforms->height_map_scale = render_component->height_map_scale;
				model_render_data->meshes_render_data[i].frame_uniforms_buffer_offset = vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index];
				uint32 uniforms_size = round_up((uint32)sizeof(struct uniforms_t), uniform_alignment);
				vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index] += uniforms_size;
			}
		};
		for (uint32 i = 0; i < level->entity_count; i += 1) {
			uint32 entity_flag = level->entity_flags[i];
			if (entity_flag & component_flag_render) {
				render_component *render_component = entity_get_render_component(level, i);
				add_model_render_data(render_component, transform_to_mat4(level->entity_transforms[i]));
			}
		}
	}
	// {// texts
		// struct vertex {
		//   vec4 position_uv;
		//   u8vec4 color;
		//   uint32 transform_mat_index;
		// };
		// static_assert(sizeof(struct vertex) == 24, "");
		// vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] = round_up(vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index], (uint32)sizeof(struct vertex));
		// vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index] = round_up(vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], uniform_alignment);
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
					for (uint32 i = 0; i < model->mesh_count; i += 1) {
						model_mesh *model_mesh = &model->meshes[i];
						uint32 offsets[3] = {level->render_data.common_data_frame_uniform_buffer_offset, model_render_data->meshes_render_data[i].frame_uniforms_buffer_offset, 0};
						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_shadow_map_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(offsets), offsets);
						vkCmdDrawIndexed(cmd_buffer, model_mesh->index_count, 1, model_mesh->index_buffer_offset / sizeof(uint16), model_mesh->vertex_buffer_offset / model_mesh->vertex_size, 0);
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
				for (uint32 i = 0; i < model->mesh_count; i += 1) {
					model_mesh *model_mesh = &model->meshes[i];
					uint32 frame_uniform_buffer_offsets[3] = {level->render_data.common_data_frame_uniform_buffer_offset, model_render_data->meshes_render_data[i].frame_uniforms_buffer_offset, 0};
					vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(frame_uniform_buffer_offsets), frame_uniform_buffer_offsets);
					if (model_mesh->material_index < model->material_count) {
 						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 1, 1, &model->materials[model_mesh->material_index].descriptor_set, 0, nullptr);
					}
					else {
						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_pipeline.layout, 1, 1, &vulkan->descriptors.model_default_material_images, 0, nullptr);
					}
					vkCmdDrawIndexed(cmd_buffer, model_mesh->index_count, 1, model_mesh->index_buffer_offset / sizeof(uint16), model_mesh->vertex_buffer_offset / model_mesh->vertex_size, 0);
					if (model_render_data->meshes_render_data[i].render_vertices_outline) {
						vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_line_pipeline.pipeline);
						uint32 frame_uniform_buffer_offsets[3] = {level->render_data.common_data_frame_uniform_buffer_offset, model_render_data->meshes_render_data[i].frame_uniforms_buffer_offset, 0};
						vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.static_model_line_pipeline.layout, 0, 1, &(vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index]), m_countof(frame_uniform_buffer_offsets), frame_uniform_buffer_offsets);
						vkCmdDrawIndexed(cmd_buffer, model_mesh->index_count, 1, model_mesh->index_buffer_offset / sizeof(uint16), model_mesh->vertex_buffer_offset / model_mesh->vertex_size, 0);
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
		// if (level->render_data.text_frame_vertex_count > 0) {
		// 	vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.text_pipeline.pipeline);
		// 	VkDeviceSize vertices_offset = 0;
		// 	vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.frame_vertices_buffer.buffer, &vertices_offset);
		// 	VkDescriptorSet descriptor_sets[2] = {vulkan->descriptor_sets.text_uniform_descriptor_set, vulkan->descriptor_sets.text_image_descriptor_set};
		// 	vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.text_pipeline.layout, 0, m_countof(descriptor_sets), descriptor_sets, 1, &level->render_data.text_frame_uniforms_buffer_offset);
		// 	vkCmdDraw(cmd_buffer, level->render_data.text_frame_vertex_count, 1, level->render_data.text_frame_vertices_buffer_offset / 24, 0);
		// }
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
