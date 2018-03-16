/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

const uint32 gpk_model_vertex_index_size = 2;         // ushort
const uint32 gpk_model_vertex_position_size = 12;     // float/float/float
const uint32 gpk_model_vertex_uv_size = 8;            // float/float
const uint32 gpk_model_vertex_normal_size = 6;        // short_snorm/short_snorm/short_snorm
const uint32 gpk_model_vertex_tangent_size = 6;       // short_snorm/short_snorm/short_snorm
const uint32 gpk_model_vertex_joint_indices_size = 4; // u8/u8/u8/u8
const uint32 gpk_model_vertex_joint_weights_size = 8; // short_unorm/short_unorm/short_unorm/short_unorm
const uint32 gpk_model_vertex_max_joint_count = 4;
const uint32 gpk_model_mesh_max_joint_count = 256;

struct gpk_model {
	char format_str[32];
	uint32 scene_offset;
	uint32 scene_count;
	uint32 node_offset;
	uint32 node_count;
	uint32 mesh_offset;
	uint32 mesh_count;
	uint32 material_offset;
	uint32 material_count;
	uint32 image_offset;
	uint32 image_count;
};

struct gpk_model_scene {
	char name[64];
	uint32 node_indices[32];
	uint32 node_index_count;
};

struct gpk_model_node {
	uint32 mesh_index;
	mat4 local_transform_mat;
	uint32 children[32];
	uint32 child_count;
};

struct gpk_model_mesh {
	char name[64];
	uint32 material_index;
	uint32 indices_offset;
	uint32 index_count;
	uint32 vertices_offset;
	uint32 vertex_size;
	uint32 vertex_count;
};

struct gpk_model_material {
 	char name[64];

	uint32 albedo_image_index;
	uint32 albedo_image_wrap_s;
	uint32 albedo_image_wrap_t;
	vec4 albedo_factor;

	uint32 metallic_image_index;
	uint32 roughness_image_index;
	uint32 metallic_roughness_image_wrap_s;
	uint32 metallic_roughness_image_wrap_t;
	float metallic_factor;
	float roughness_factor;

	uint32 normal_image_index;
	uint32 normal_image_wrap_s;
	uint32 normal_image_wrap_t;
};

struct gpk_model_image {
	uint32 width;
	uint32 height;
	uint32 mipmap_count;
	uint32 layer_count;
	uint32 size;
	uint32 format;
	uint32 format_block_dimension;
	uint32 format_block_size;
  uint32 data_offset;
};

struct gpk_skybox {
	char format_str[32];
	uint32 cubemap_offset;
	uint32 cubemap_width;
	uint32 cubemap_height;
};

struct gpk_terrain {
	char format_str[32];
	uint32 diffuse_map_offset;
	vec2 diffuse_map_scales;
	uint32 vertices_offset;
	uint32 vertex_size;
	uint32 vertex_count;
	mat4 transform;
};
