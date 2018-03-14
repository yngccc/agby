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

enum gpk_texture_format {
	gpk_texture_format_b8r8g8,
	gpk_texture_format_b8r8g8_srgb,
	gpk_texture_format_b8r8g8a8,
	gpk_texture_format_b8r8g8a8_srgb,
	gpk_texture_format_bc1,
	gpk_texture_format_bc1_srgb,
	gpk_texture_format_bc5
};

struct gpk_texture {
	uint32 texture_offset;
	uint32 texture_size;
	gpk_texture_format texture_format;
	uint32 texture_pixel_size;
	uint32 texture_width;
	uint32 texture_height;
	uint32 texture_mipmap_count;
};

struct gpk_model_material {
	char name[64];
	uint32 albedo_map_offset;
	uint32 albedo_map_width;
	uint32 albedo_map_height;
	uint32 albedo_map_mipmap_count;
	uint32 albedo_map_size;
	uint32 normal_map_offset;
	uint32 normal_map_width;
	uint32 normal_map_height;
	uint32 normal_map_mipmap_count;
	uint32 normal_map_size;
	uint32 metallic_map_offset;
	uint32 metallic_map_width;
	uint32 metallic_map_height;
	uint32 metallic_map_mipmap_count;
	uint32 metallic_map_size;
	uint32 roughness_map_offset;
	uint32 roughness_map_width;
	uint32 roughness_map_height;
	uint32 roughness_map_mipmap_count;
	uint32 roughness_map_size;
	uint32 ao_map_offset;
	uint32 ao_map_width;
	uint32 ao_map_height;
	uint32 ao_map_mipmap_count;
	uint32 ao_map_size;
	uint32 height_map_offset;
	uint32 height_map_width;
	uint32 height_map_height;
	uint32 height_map_mipmap_count;
	uint32 height_map_size;
};

struct gpk_skybox {
	char format_str[32];
	uint32 cubemap_offset;
	uint32 cubemap_width;
	uint32 cubemap_height;
};

struct gpk_terrain {
	uint32 diffuse_map_offset;
	vec2 diffuse_map_scales;
	uint32 vertices_offset;
	uint32 vertex_size;
	uint32 vertex_count;
	mat4 transform;
};
