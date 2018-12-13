/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#include "common.cpp"
#include "math.cpp"
#include "geometry.cpp"
#include "simd.cpp"
#include "gpk.cpp"
#include "d3d11.cpp"

#include "flatbuffers/world_generated.h"
static_assert(sizeof(flatbuffers::Transform) == sizeof(struct transform), "");
static_assert(sizeof(flatbuffers::Mat4) == 64, "");

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

#define NDEBUG
#include <physx/PxPhysicsAPI.h>
#undef NDEBUG

enum collision_type {
	collision_type_none,
	collision_type_sphere,
	collision_type_box,
	collision_type_capsule_x,
	collision_type_capsule_y,
	collision_type_capsule_z
};

const char *collision_type_strs[] = {"none", "sphere", "box", "capsule_x", "capsule_y", "capsule_z"};

struct collision_sphere {
	float radius;
};

struct collision_box {
	vec3 extents;
};

struct collision_capsule {
	float radius;
	float height;
};

struct collision {
	collision_type type;
	union {
		collision_sphere sphere;
		collision_box box;
		collision_capsule capsule;
	};
};

float collision_get_bottom(collision collision) {
	if (collision.type == collision_type_sphere) {
		return -collision.sphere.radius;
	}
	else if (collision.type == collision_type_box) {
		vec3 half_extents = collision.box.extents * 0.5f;
		return -half_extents.y;
	}
	else if (collision.type == collision_type_capsule_x) {
		return -collision.capsule.radius;
	}
	else if (collision.type == collision_type_capsule_y) {
		return -collision.capsule.height * 0.5f - collision.capsule.radius;
	}
	else if (collision.type == collision_type_capsule_z) {
		return -collision.capsule.radius;
	}
	return 0;
}

struct player {
	uint32 model_index;
	transform transform;
	uint32 animation_index;
	double animation_time;

	btCollisionShape *bt_collision_shape;
	btRigidBody *bt_rigid_body;

	float feet_translate;	
};

struct static_object {
	uint32 model_index;
	transform transform;
	uint32 animation_index;
	double animation_time;
	char *id;

	btCollisionShape *bt_collision_shape;
	btCollisionObject *bt_collision_object;
};

struct dynamic_object {
	uint32 model_index;
	transform transform;
	uint32 animation_index;
	double animation_time;
	char *id;

	btCollisionShape *bt_collision_shape;
	btRigidBody *bt_rigid_body;
};

struct model_scene {
	uint32 node_indices[m_countof(gpk_model_scene::node_indices)];
	uint32 node_index_count;
	char name[sizeof(gpk_model_scene::name)];
};

struct model_node {
	mat4 local_transform_mat;
	mat4 global_transform_mat;
	transform local_transform;
	uint32 mesh_index;
	uint32 skin_index;
	uint32 children[m_countof(gpk_model_node::children)];
	uint32 child_count;
};

struct model_mesh_primitive {
	ID3D11Buffer *vertex_buffer;
	ID3D11Buffer *index_buffer;
	uint32 vertex_count;
	uint32 index_count;
	uint32 material_index;
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
	uint32 diffuse_texture_index;
	vec4 diffuse_factor;
	uint32 roughness_texture_index;
	float roughness_factor;
	uint32 metallic_texture_index;
	float metallic_factor;
	uint32 normal_texture_index;
	char name[sizeof(gpk_model_material::name)];
};

struct model_texture {
	ID3D11Texture2D *texture;
	ID3D11ShaderResourceView *texture_view;
};

struct model {
	model_scene *scenes;
	uint32 scene_count;
	model_node *nodes;
	uint32 node_count;
	model_mesh *meshes;
	uint32 mesh_count;
	uint32 mesh_node_count;
	model_skin *skins;
	uint32 skin_count;
	model_animation *animations;
	uint32 animation_count;
	model_material *materials;
	uint32 material_count;
	model_texture *textures;
	uint32 texture_count;
	transform transform;
	collision collision;
	char file[32];
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
	
	ID3D11Texture2D *height_texture;
	ID3D11ShaderResourceView *height_texture_view;
	int16 *height_texture_data;

	btHeightfieldTerrainShape *bt_terrain_shape;
	btCollisionObject *bt_terrain_collision_object;
	
	ID3D11Texture2D *diffuse_texture;
	ID3D11ShaderResourceView *diffuse_texture_view;
	uint32 *diffuse_texture_data;
	float diffuse_texture_uv_repeat;

	ID3D11Buffer *vertex_buffer;
	ID3D11Buffer *index_buffer;
	uint32 index_count;

	char file[32];
};

struct skybox {
	ID3D11Texture2D *cube_texture;
	ID3D11ShaderResourceView *cube_texture_view;
	char file[32];
};

struct mesh_primitive_render_data {
	uint32 constant_buffer_offset;
};

struct mesh_render_data {
	model_mesh *mesh;
	mesh_primitive_render_data *primitives;
	uint32 primitive_count;
	uint32 joint_mats_constant_buffer_offset;
};

struct model_render_data {
	model *model;
	uint32 model_mat_constant_buffer_offset;
	mesh_render_data *meshes;
	uint32 mesh_count;
	bool render_collision_shapes;
	uint32 shape_count;
	ID3D11Buffer *shape_vertex_buffers[3];
	uint32 shape_vertex_counts[3];
	uint32 shape_constant_buffer_offsets[3];
	model_render_data *next;
};

struct world_render_data {
	model_render_data *model_list;
	uint32 model_count;
	uint32 terrain_constant_buffer_offset;
	uint32 terrain_brush_constant_buffer_offset;
	uint32 shadow_proj_box_constant_buffer_offset;
};

struct world {
	world_render_data render_data;	
	
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

	terrain *terrains;
	uint32 terrain_count;
	uint32 terrain_capacity;
	uint32 terrain_index;

	skybox *skyboxes;
	uint32 skybox_count;
	uint32 skybox_capacity;
	uint32 skybox_index;

	vec3 sun_light_dir;
	vec3 sun_light_color;

	btDefaultCollisionConfiguration *bt_collision_config;
	btCollisionDispatcher *bt_collision_dispatcher;
	btDbvtBroadphase *bt_broad_phase;
	btSequentialImpulseConstraintSolver *bt_constraint_solver;
	btDiscreteDynamicsWorld *bt_dynamics_world;

	memory_arena frame_memory_arena;
	memory_arena general_memory_arena;

	ID3D11Buffer *constant_buffer;
	uint8 *constant_buffer_ptr;
	uint32 constant_buffer_offset;
	uint32 constant_buffer_capacity;
	
	ID3D11Buffer *box_vertex_buffer;
	ID3D11Buffer *sphere_vertex_buffer;
	ID3D11Buffer *hemisphere_vertex_buffer;
	ID3D11Buffer *cylinder_vertex_buffer;
	ID3D11Buffer *hollow_cylinder_vertex_buffer;
	ID3D11Buffer *hollow_circle_vertex_buffer;
	ID3D11Buffer *torus_vertex_buffer;
	ID3D11Buffer *reference_grid_vertex_buffer;
	ID3D11Buffer *vertex_buffer_reset;

	ID3D11Texture2D *default_diffuse_texture;
	ID3D11ShaderResourceView *default_diffuse_texture_view;
	ID3D11Texture2D *default_roughness_texture;
	ID3D11ShaderResourceView *default_roughness_texture_view;
	ID3D11Texture2D *default_metallic_texture;
	ID3D11ShaderResourceView *default_metallic_texture_view;
	ID3D11Texture2D *default_normal_texture;
	ID3D11ShaderResourceView *default_normal_texture_view;
	ID3D11Texture2D *default_height_texture;
	ID3D11ShaderResourceView *default_height_texture_view;
};

void initialize_world(world *world, d3d *d3d) {
	*world = {};
	{
		m_assert(initialize_memory_arena(m_megabytes(16), &world->frame_memory_arena), "");
		m_assert(initialize_memory_arena(m_megabytes(128), &world->general_memory_arena), "");

		world->constant_buffer_capacity = m_megabytes(4);
		D3D11_BUFFER_DESC constant_buffer_desc = {};
		constant_buffer_desc.ByteWidth = world->constant_buffer_capacity;
		constant_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		constant_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		m_d3d_assert(d3d->device->CreateBuffer(&constant_buffer_desc, nullptr, &world->constant_buffer));
	}
	{
		D3D11_BUFFER_DESC vertex_buffer_desc = {};
		vertex_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertex_buffer_data = D3D11_SUBRESOURCE_DATA{box_vertices};
		vertex_buffer_desc.ByteWidth = sizeof(box_vertices);
		m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->box_vertex_buffer));

		vertex_buffer_data = D3D11_SUBRESOURCE_DATA{sphere_vertices};
		vertex_buffer_desc.ByteWidth = sizeof(sphere_vertices);
		m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->sphere_vertex_buffer));

		vertex_buffer_data = D3D11_SUBRESOURCE_DATA{hemisphere_vertices};
		vertex_buffer_desc.ByteWidth = sizeof(hemisphere_vertices);
		m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->hemisphere_vertex_buffer));

		vertex_buffer_data = D3D11_SUBRESOURCE_DATA{cylinder_vertices};
		vertex_buffer_desc.ByteWidth = sizeof(cylinder_vertices);
		m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->cylinder_vertex_buffer));

		vertex_buffer_data = D3D11_SUBRESOURCE_DATA{hollow_cylinder_vertices};
		vertex_buffer_desc.ByteWidth = sizeof(hollow_cylinder_vertices);
		m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->hollow_cylinder_vertex_buffer));

		vertex_buffer_data = D3D11_SUBRESOURCE_DATA{hollow_circle_vertices};
		vertex_buffer_desc.ByteWidth = sizeof(hollow_circle_vertices);
		m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->hollow_circle_vertex_buffer));

		vertex_buffer_data = D3D11_SUBRESOURCE_DATA{torus_vertices};
		vertex_buffer_desc.ByteWidth = sizeof(torus_vertices);
		m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->torus_vertex_buffer));

		{ // reference grid
			undo_allocation_scope_exit(&world->frame_memory_arena);
			float size = 50;
			uint32 n = 10;
			float gap = size / n;
			uint32 vertex_count = (n + 1) * 2 * 2;
			vec3 *vertices = allocate_memory<vec3>(&world->frame_memory_arena, vertex_count);
			vec3 *vertices_ptr = vertices;
			vec3 begin = {-size / 2, 0, -size / 2};
			vec3 end = {size / 2, 0, -size / 2};
			for (uint32 i = 0; i < n + 1; i += 1) {
				vertices_ptr[0] = begin;
				vertices_ptr[1] = end;
				vertices_ptr += 2;
				begin.z += gap;
				end.z += gap;
			}
			begin = {-size / 2, 0, -size / 2};
			end = {-size / 2, 0, size / 2};
			for (uint32 i = 0; i < n + 1; i += 1) {
				vertices_ptr[0] = begin;
				vertices_ptr[1] = end;
				vertices_ptr += 2;
				begin.x += gap;
				end.x += gap;
			}
			
			vertex_buffer_desc.ByteWidth = vertex_count * 12;
			D3D11_SUBRESOURCE_DATA vertex_buffer_data = {vertices};
			m_d3d_assert(d3d->device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &world->reference_grid_vertex_buffer));
		}
	}
	{
		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = 2;
		texture_desc.Height = 2;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		D3D11_SUBRESOURCE_DATA texture_subresource_data = {};
		
		uint8 default_diffuse_map_data[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_subresource_data = {default_diffuse_map_data, 2 * 4, 0};
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, &world->default_diffuse_texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(world->default_diffuse_texture, nullptr, &world->default_diffuse_texture_view));

		uint8 default_roughness_map_data[] = {255, 255, 255, 255};
		texture_desc.Format = DXGI_FORMAT_R8_UNORM;
		texture_subresource_data = {default_roughness_map_data, 2 * 1, 0};
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, &world->default_roughness_texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(world->default_roughness_texture, nullptr, &world->default_roughness_texture_view));

		uint8 default_metallic_map_data[] = {255, 255, 255, 255};
		texture_desc.Format = DXGI_FORMAT_R8_UNORM;
		texture_subresource_data = {default_metallic_map_data, 2 * 1, 0};
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, &world->default_metallic_texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(world->default_metallic_texture, nullptr, &world->default_metallic_texture_view));

		uint8 default_normal_map_data[] = {128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0};
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_subresource_data = {default_normal_map_data, 2 * 4, 0};
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, &world->default_normal_texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(world->default_normal_texture, nullptr, &world->default_normal_texture_view));

		uint8 default_height_map_data[] = {0, 0, 0, 0};
		texture_desc.Format = DXGI_FORMAT_R8_SNORM;
		texture_subresource_data = {default_height_map_data, 2 * 1, 0};
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &texture_subresource_data, &world->default_height_texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(world->default_height_texture, nullptr, &world->default_height_texture_view));
	}
	{
		world->bt_collision_config = new btDefaultCollisionConfiguration();
		world->bt_collision_dispatcher = new btCollisionDispatcher(world->bt_collision_config);
		world->bt_broad_phase = new btDbvtBroadphase();
		world->bt_constraint_solver = new btSequentialImpulseConstraintSolver();
		world->bt_dynamics_world = new btDiscreteDynamicsWorld(world->bt_collision_dispatcher, world->bt_broad_phase, world->bt_constraint_solver, world->bt_collision_config);
		world->bt_dynamics_world->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		world->bt_dynamics_world->setGravity(btVector3(0, -9.81f, 0));
	}
	{
		static physx::PxDefaultAllocator default_allocator_callback;
		static physx::PxDefaultErrorCallback default_error_callback;

		physx::PxFoundation *foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, default_allocator_callback, default_error_callback);
		if(!foundation) {
			fatal("PxCreateFoundation failed!");
		}
	}
	void reset_world(struct world*, struct d3d*);
	reset_world(world, d3d);
}

void reset_world(world *world, d3d *d3d) {	
	world->general_memory_arena.size = 0;
	memset(world->general_memory_arena.memory, 0, world->general_memory_arena.capacity);

	world->player = {};
	world->player.model_index = UINT32_MAX;
	world->player.transform = transform_identity();
	world->player.animation_index = UINT32_MAX;

	world->static_object_count = 0;
	world->static_object_capacity = 1024;
	world->static_objects = allocate_memory<struct static_object>(&world->general_memory_arena, world->static_object_capacity);

	world->dynamic_object_count = 0;
	world->dynamic_object_capacity = 1024;
	world->dynamic_objects = allocate_memory<struct dynamic_object>(&world->general_memory_arena, world->dynamic_object_capacity);

	world->model_count = 0;
	world->model_capacity = 512;
	world->models = allocate_memory<struct model>(&world->general_memory_arena, world->model_capacity);

	world->terrain_count = 0;
	world->terrain_capacity = 8;
	world->terrains = allocate_memory<struct terrain>(&world->general_memory_arena, world->terrain_capacity);

	world->skybox_count = 0;
	world->skybox_capacity = 8;
	world->skyboxes = allocate_memory<struct skybox>(&world->general_memory_arena, world->skybox_capacity);

	world->sun_light_dir = vec3_normalize({1, 1, 1});
	world->sun_light_color = {0.5f, 0.5f, 0.5f};
}

bool add_model(world *world, d3d *d3d, const char *model_file, transform transform, collision collision) {
	file_mapping model_file_mapping = {};
	if (!open_file_mapping(model_file, &model_file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(model_file_mapping));

	gpk_model *gpk_model = (struct gpk_model *)model_file_mapping.ptr;
	if (strcmp(gpk_model->format_str, m_gpk_model_format_str)) {
		return false;
	}

	if (world->model_count >= world->model_capacity) {
		return false;
	}
	model *model = &world->models[world->model_count];
	const char *file_name = get_file_name(model_file);
	for (uint32 i = 0; i < world->model_count; i += 1) {
		if (!strcmp(world->models[i].file, file_name)) {
			return false;
		}
	}
	snprintf(model->file, sizeof(model->file), "%s", file_name);
	model->transform = transform;
	model->collision = collision;
	model->scene_count = gpk_model->scene_count;
	model->node_count = gpk_model->node_count;
	model->mesh_count = gpk_model->mesh_count;
	model->mesh_node_count = 0;
	model->skin_count = gpk_model->skin_count;
	model->animation_count = gpk_model->animation_count;
	model->material_count = gpk_model->material_count;
	model->texture_count = gpk_model->image_count;
	model->scenes = allocate_memory<struct model_scene>(&world->general_memory_arena, model->scene_count);
	model->nodes = allocate_memory<struct model_node>(&world->general_memory_arena, model->node_count);
	model->meshes = allocate_memory<struct model_mesh>(&world->general_memory_arena, model->mesh_count);
	if (model->skin_count > 0) {
		model->skins = allocate_memory<struct model_skin>(&world->general_memory_arena, model->skin_count);
	}
	if (model->animation_count > 0) {
		model->animations = allocate_memory<struct model_animation>(&world->general_memory_arena, model->animation_count);
	}
	if (model->material_count > 0) {
		model->materials = allocate_memory<struct model_material>(&world->general_memory_arena, model->material_count);
	}
	if (model->texture_count > 0) {
		model->textures = allocate_memory<struct model_texture>(&world->general_memory_arena, model->texture_count);
	}
	for (uint32 i = 0; i < model->scene_count; i += 1) {
		gpk_model_scene *gpk_model_scene = ((struct gpk_model_scene*)(model_file_mapping.ptr + gpk_model->scene_offset)) + i;
		model_scene *model_scene = &model->scenes[i];
		array_copy(model_scene->name, gpk_model_scene->name);
		array_copy(model_scene->node_indices, gpk_model_scene->node_indices);
		model_scene->node_index_count = gpk_model_scene->node_index_count;
	}
	for (uint32 i = 0; i < model->node_count; i += 1) {
		gpk_model_node *gpk_model_node = ((struct gpk_model_node*)(model_file_mapping.ptr + gpk_model->node_offset)) + i;
		model_node *model_node = &model->nodes[i];
		model_node->mesh_index = gpk_model_node->mesh_index;
		model_node->skin_index = gpk_model_node->skin_index;
		model_node->local_transform = gpk_model_node->local_transform;
		model_node->local_transform_mat = gpk_model_node->local_transform_mat;
		model_node->global_transform_mat = gpk_model_node->global_transform_mat;
		array_copy(model_node->children, gpk_model_node->children);
		model_node->child_count = gpk_model_node->child_count;
	}

	for (uint32 i = 0; i < model->mesh_count; i += 1) {
		gpk_model_mesh *gpk_model_mesh = ((struct gpk_model_mesh*)(model_file_mapping.ptr + gpk_model->mesh_offset)) + i;
		model_mesh *model_mesh = &model->meshes[i];
		array_copy(model_mesh->name, gpk_model_mesh->name);
		model_mesh->primitive_count = gpk_model_mesh->primitive_count;
		model_mesh->primitives = allocate_memory<struct model_mesh_primitive>(&world->general_memory_arena, model_mesh->primitive_count);
		for (uint32 i = 0; i < model_mesh->primitive_count; i += 1) {
			gpk_model_mesh_primitive *gpk_primitive = ((gpk_model_mesh_primitive *)(model_file_mapping.ptr + gpk_model_mesh->primitive_offset)) + i;
			model_mesh_primitive *primitive = &model_mesh->primitives[i];

			primitive->vertex_count = gpk_primitive->vertex_count;
			primitive->index_count = gpk_primitive->index_count;
			primitive->material_index = gpk_primitive->material_index;

			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;

			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.ByteWidth = gpk_primitive->vertex_count * sizeof(struct gpk_model_vertex);
			D3D11_SUBRESOURCE_DATA vertex_buffer_data = {model_file_mapping.ptr + gpk_primitive->vertices_offset};
			m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, &vertex_buffer_data, &primitive->vertex_buffer));

			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.ByteWidth = gpk_primitive->index_count * sizeof(uint16);
			vertex_buffer_data = {model_file_mapping.ptr + gpk_primitive->indices_offset};
			m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, &vertex_buffer_data, &primitive->index_buffer));			
		}
	}

	for (uint32 i = 0; i < model->skin_count; i += 1) {
		gpk_model_skin *gpk_model_skin = ((struct gpk_model_skin*)(model_file_mapping.ptr + gpk_model->skin_offset)) + i;
		model_skin *model_skin = &model->skins[i];
		array_copy(model_skin->name, gpk_model_skin->name);
		model_skin->joint_count = gpk_model_skin->joint_count;
		m_assert(model_skin->joint_count > 0, "");
		model_skin->joints = allocate_memory<struct model_joint>(&world->general_memory_arena, model_skin->joint_count);
		gpk_model_joint *gpk_joints = (gpk_model_joint *)(model_file_mapping.ptr + gpk_model_skin->joints_offset);
		for (uint32 i = 0; i < model_skin->joint_count; i += 1) {
			model_skin->joints[i].node_index = gpk_joints[i].node_index;
			model_skin->joints[i].inverse_bind_mat = gpk_joints[i].inverse_bind_mat;
		}
	}
	for (uint32 i = 0; i < model->animation_count; i += 1) {
		gpk_model_animation *gpk_animation = ((struct gpk_model_animation*)(model_file_mapping.ptr + gpk_model->animation_offset)) + i;
		model_animation *animation = &model->animations[i];
		array_copy(animation->name, gpk_animation->name);
		animation->channel_count = gpk_animation->channel_count;
		animation->channels = allocate_memory<struct model_animation_channel>(&world->general_memory_arena, animation->channel_count);
		for (uint32 i = 0; i < animation->channel_count; i += 1) {
			gpk_model_animation_channel *gpk_channel = ((struct gpk_model_animation_channel*)(model_file_mapping.ptr + gpk_animation->channel_offset)) + i;
			model_animation_channel *channel = &animation->channels[i];
			channel->node_index = gpk_channel->node_index;
			channel->channel_type = gpk_channel->channel_type;
			channel->sampler_index = gpk_channel->sampler_index;
		}
		animation->sampler_count = gpk_animation->sampler_count;
		animation->samplers = allocate_memory<model_animation_sampler>(&world->general_memory_arena, animation->sampler_count);
		for (uint32 i = 0; i < animation->sampler_count; i += 1) {
			gpk_model_animation_sampler *gpk_sampler = ((struct gpk_model_animation_sampler*)(model_file_mapping.ptr + gpk_animation->sampler_offset)) + i;
			model_animation_sampler *sampler = &animation->samplers[i];
			sampler->interpolation_type = gpk_sampler->interpolation_type;
			sampler->key_frame_count = gpk_sampler->key_frame_count;
			sampler->key_frames = allocate_memory<struct model_animation_key_frame>(&world->general_memory_arena, sampler->key_frame_count);
			for (uint32 i = 0; i < sampler->key_frame_count; i += 1) {
				gpk_model_animation_key_frame *gpk_key_frame = ((struct gpk_model_animation_key_frame *)(model_file_mapping.ptr + gpk_sampler->key_frame_offset)) + i;
				sampler->key_frames[i].time = gpk_key_frame->time;
				sampler->key_frames[i].transform_data = gpk_key_frame->transform_data;
			}
		}
	}
	for (uint32 i = 0; i < model->texture_count; i += 1) {
		gpk_model_image *gpk_model_image = ((struct gpk_model_image*)(model_file_mapping.ptr + gpk_model->image_offset)) + i;
		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = gpk_model_image->width;
		texture_desc.Height = gpk_model_image->height;
		texture_desc.MipLevels = gpk_model_image->mipmap_count;
		texture_desc.ArraySize = 1;
		texture_desc.Format = (DXGI_FORMAT)gpk_model_image->format;
		texture_desc.SampleDesc = {1, 0};
		texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA subresource_data[16] = {};
		m_debug_assert(gpk_model_image->mipmap_count <= m_countof(subresource_data), "");
		uint8 *image_data = model_file_mapping.ptr + gpk_model_image->data_offset;
		uint32 width = gpk_model_image->width;
		uint32 height = gpk_model_image->height;
		for (uint32 i = 0; i < gpk_model_image->mipmap_count; i += 1) {
			subresource_data[i].pSysMem = image_data;
			subresource_data[i].SysMemPitch = width / gpk_model_image->format_block_dimension * gpk_model_image->format_block_size;
			image_data += width * height / (gpk_model_image->format_block_dimension * gpk_model_image->format_block_dimension) * gpk_model_image->format_block_size;
			width = max(width / 2, gpk_model_image->format_block_dimension);
			height = max(height / 2, gpk_model_image->format_block_dimension);
		}
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, subresource_data, &model->textures[i].texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(model->textures[i].texture, nullptr, &model->textures[i].texture_view));
	}
	for (uint32 i = 0; i < model->material_count; i += 1) {
		gpk_model_material *gpk_model_material = ((struct gpk_model_material*)(model_file_mapping.ptr + gpk_model->material_offset)) + i;
		model_material *model_material = &model->materials[i];
		array_copy(model_material->name, gpk_model_material->name);
		model_material->diffuse_texture_index = gpk_model_material->diffuse_image_index;
		model_material->diffuse_factor = gpk_model_material->diffuse_factor;
		model_material->roughness_texture_index = gpk_model_material->roughness_image_index;
		model_material->roughness_factor = gpk_model_material->roughness_factor;
		model_material->metallic_texture_index = gpk_model_material->metallic_image_index;
		model_material->metallic_factor = gpk_model_material->metallic_factor;
		model_material->normal_texture_index = gpk_model_material->normal_image_index;
	}
	for (uint32 i = 0; i < model->node_count; i += 1) {
		if (model->nodes[i].mesh_index < model->mesh_count) {
			model->mesh_node_count += 1;
		}
	}	
	world->model_count += 1;
	return true;
}

bool add_terrain(world *world, d3d *d3d, const char *terrain_file) {
	file_mapping terrain_file_mapping = {};
	if (!open_file_mapping(terrain_file, &terrain_file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(terrain_file_mapping));

	gpk_terrain *gpk_terrain = (struct gpk_terrain *)terrain_file_mapping.ptr;
	if (strcmp(gpk_terrain->format_str, m_gpk_terrain_format_str)) {
		return false;
	}
	
	if (world->terrain_count >= world->terrain_capacity) {
		return false;
	}
	const char *file_name = get_file_name(terrain_file);
	for (uint32 i = 0; i < world->terrain_count; i += 1) {
		if (!strcmp(world->terrains[i].file, file_name)) {
			return false;
		}
	}

	terrain *terrain = &world->terrains[world->terrain_count];
	snprintf(terrain->file, sizeof(terrain->file), "%s", file_name);
	terrain->width = gpk_terrain->width;
	terrain->height = gpk_terrain->height;
	terrain->max_height = gpk_terrain->max_height;
	terrain->sample_per_meter = gpk_terrain->sample_per_meter;
	{ // height texture
		uint32 height_texture_width = terrain->width * terrain->sample_per_meter;
		uint32 height_texture_height = terrain->height * terrain->sample_per_meter;
		uint32 height_texture_size = height_texture_width * height_texture_height * sizeof(int16);
		int16 *height_texture_data = (int16 *)(terrain_file_mapping.ptr + gpk_terrain->height_map_offset);

		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = height_texture_width;
		texture_desc.Height = height_texture_height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R16_SNORM;
		texture_desc.SampleDesc = {1, 0};
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA subresource_data = {height_texture_data, height_texture_width * 2, 0};
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &subresource_data, &terrain->height_texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(terrain->height_texture, nullptr, &terrain->height_texture_view));

		terrain->height_texture_data = allocate_memory<int16>(&world->general_memory_arena, height_texture_width * height_texture_height);
		memcpy(terrain->height_texture_data, height_texture_data, height_texture_size);
		
		terrain->bt_terrain_shape = new btHeightfieldTerrainShape(terrain->width * terrain->sample_per_meter, terrain->height * terrain->sample_per_meter, terrain->height_texture_data, terrain->max_height / INT16_MAX, -terrain->max_height, terrain->max_height, 1, PHY_SHORT, false);
		terrain->bt_terrain_shape->setLocalScaling(btVector3(1.0f / terrain->sample_per_meter, 1, 1.0f / terrain->sample_per_meter));
		terrain->bt_terrain_collision_object = new btCollisionObject();
		terrain->bt_terrain_collision_object->setCollisionShape(terrain->bt_terrain_shape);
		world->bt_dynamics_world->addCollisionObject(terrain->bt_terrain_collision_object);
	}
	{ // diffuse texture
		uint32 diffuse_texture_width = terrain->width * terrain->sample_per_meter;
		uint32 diffuse_texture_height = terrain->height * terrain->sample_per_meter;
		uint32 diffuse_texture_size = diffuse_texture_width * diffuse_texture_height * 4;
		uint32 *diffuse_texture_data = (uint32 *)(terrain_file_mapping.ptr + gpk_terrain->diffuse_map_offset);

		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = diffuse_texture_width;
		texture_desc.Height = diffuse_texture_height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		texture_desc.SampleDesc = {1, 0};
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA subresource_data = {diffuse_texture_data, diffuse_texture_width * 2, 0};
		m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &subresource_data, &terrain->diffuse_texture));
		m_d3d_assert(d3d->device->CreateShaderResourceView(terrain->diffuse_texture, nullptr, &terrain->diffuse_texture_view));

		terrain->diffuse_texture_uv_repeat = 1.0f;

		terrain->diffuse_texture_data = allocate_memory<uint32>(&world->general_memory_arena, diffuse_texture_width * diffuse_texture_height);
		memcpy(terrain->diffuse_texture_data, diffuse_texture_data, diffuse_texture_size);
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
			undo_allocation_scope_exit(&world->general_memory_arena);
			terrain_vertex *vertices = allocate_memory<terrain_vertex>(&world->general_memory_arena, vertex_count);
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
			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.ByteWidth = vertex_count * sizeof(struct terrain_vertex);
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA vertex_buffer_data = {vertices, 0, 0};
			m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, &vertex_buffer_data, &terrain->vertex_buffer));
		}
		{
			undo_allocation_scope_exit(&world->general_memory_arena);
			uint32 *indices = allocate_memory<uint32>(&world->general_memory_arena, index_count);
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
			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.ByteWidth = index_count * sizeof(uint32);
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA vertex_buffer_data = {indices, 0, 0};
			m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, &vertex_buffer_data, &terrain->index_buffer));
			terrain->index_count = index_count;
		}
	}
	world->terrain_count += 1;
	return true;
}

bool remove_terrain(world *world, uint32 index) {
	if (index < world->terrain_count) {
		terrain *terrain = &world->terrains[index];
		terrain->height_texture->Release();
		terrain->height_texture_view->Release();
		delete terrain->bt_terrain_shape;
		delete terrain->bt_terrain_collision_object;
		terrain->diffuse_texture->Release();
		terrain->diffuse_texture_view->Release();
		terrain->vertex_buffer->Release();
		terrain->index_buffer->Release();
		array_remove(world->terrains, &world->terrain_count, index);
		return true;
	}
	else {
		return false;
	}
}

bool add_skybox(world *world, d3d *d3d, const char *skybox_file) {
	file_mapping skybox_file_mapping = {};
	if (!open_file_mapping(skybox_file, &skybox_file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(skybox_file_mapping));

	gpk_skybox *gpk_skybox = (struct gpk_skybox *)skybox_file_mapping.ptr;
	if (strcmp(gpk_skybox->format_str, m_gpk_skybox_format_str)) {
		return false;
	}

	if (world->skybox_count >= world->skybox_capacity) {
		return false;
	}
	skybox *skybox = &world->skyboxes[world->skybox_count];
	const char *file_name = get_file_name(skybox_file);
	for (uint32 i = 0; i < world->skybox_count; i += 1) {
		if (!strcmp(world->skyboxes[i].file, file_name)) {
			return false;
		}
	}
	snprintf(skybox->file, sizeof(skybox->file), "%s", file_name);

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = gpk_skybox->cubemap_width;
	texture_desc.Height = gpk_skybox->cubemap_height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 6;
	texture_desc.Format = (DXGI_FORMAT)gpk_skybox->cubemap_format;
	texture_desc.SampleDesc = {1, 0};
	texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	D3D11_SUBRESOURCE_DATA subresource_data[6] = {};
	uint8 *cubemap_data = (uint8 *)gpk_skybox + gpk_skybox->cubemap_offset;
	uint32 cubemap_face_size = gpk_skybox->cubemap_size / 6;
	for (uint32 i = 0; i < 6; i += 1) {
		subresource_data[i] = {cubemap_data + i * cubemap_face_size, gpk_skybox->cubemap_width / gpk_skybox->cubemap_format_block_dimension * gpk_skybox->cubemap_format_block_size, 0};
	}
	m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, subresource_data, &skybox->cube_texture));
	m_d3d_assert(d3d->device->CreateShaderResourceView(skybox->cube_texture, nullptr, &skybox->cube_texture_view));

	world->skybox_count += 1;
	return true;
}

collision fb_collision_to_collision(flatbuffers::CollisionShape fb_collision_type, const void *fb_collision) {
	using namespace flatbuffers;
	collision collision;
	collision.type = collision_type_none;
	if (fb_collision_type == CollisionShape_Sphere) {
		Sphere *fb_sphere = (Sphere *)fb_collision;
		collision.type = collision_type_sphere;
		collision.sphere = collision_sphere{fb_sphere->radius()};
	}
	else if (fb_collision_type == CollisionShape_Box) {
		Box *fb_box = (Box *)fb_collision;
		Vec3 fb_extents = fb_box->extents();
		collision.type = collision_type_box;
		collision.box = collision_box{vec3{fb_extents.x(), fb_extents.y(), fb_extents.z()}};
	}
	else if (fb_collision_type == CollisionShape_CapsuleX) {
		CapsuleX *fb_capsule = (CapsuleX *)fb_collision;
		collision.type = collision_type_capsule_x;
		collision.capsule = collision_capsule{fb_capsule->radius(), fb_capsule->height()};
	}
	else if (fb_collision_type == CollisionShape_CapsuleY) {
		CapsuleY *fb_capsule = (CapsuleY *)fb_collision;
		collision.type = collision_type_capsule_y;
		collision.capsule = collision_capsule{fb_capsule->radius(), fb_capsule->height()};
	}
	else if (fb_collision_type == CollisionShape_CapsuleZ) {
		CapsuleZ *fb_capsule = (CapsuleZ *)fb_collision;
		collision.type = collision_type_capsule_z;
		collision.capsule = collision_capsule{fb_capsule->radius(), fb_capsule->height()};
	}
	return collision;
}

btCollisionShape *collision_to_bt_collision(collision collision) {
	if (collision.type == collision_type_sphere) {
		return new btSphereShape(collision.sphere.radius);
	}
	else if (collision.type == collision_type_box) {
		vec3 half_extents = collision.box.extents * 0.5f;
		btVector3 bt_half_extents(m_unpack3(half_extents));
		return new btBoxShape(bt_half_extents);
	}
	else if (collision.type == collision_type_capsule_x) {
		return new btCapsuleShapeX(collision.capsule.radius, collision.capsule.height);
	}
	else if (collision.type == collision_type_capsule_y) {
		return new btCapsuleShape(collision.capsule.radius, collision.capsule.height);
	}
	else if (collision.type == collision_type_capsule_z) {
		return new btCapsuleShapeZ(collision.capsule.radius, collision.capsule.height);
	}
	return new btEmptyShape();
}

struct world_editor_settings {
	vec3 camera_position;
	vec3 camera_view;
};

bool load_world(world *world, d3d *d3d, const char *file, world_editor_settings *editor_settings) {
	file_mapping file_mapping = {};
	if (!open_file_mapping(file, &file_mapping, true)) {
		return false;
	}
	m_scope_exit(close_file_mapping(file_mapping));

	using namespace flatbuffers;
	auto *fb_world = GetWorld(file_mapping.ptr);
	if (!fb_world) {
		return false;
	}

	reset_world(world, d3d);

	auto fb_models = fb_world->models();
	if (fb_models) {
		m_assert(fb_models->size() <= world->model_capacity, "");
		for (uint32 i = 0; i < fb_models->size(); i += 1) {
			auto fb_model = (*fb_models)[i];
			transform transform = transform_identity();
			if (fb_model->transform()) {
				memcpy(&transform, fb_model->transform(), sizeof(struct transform));
			}
			collision collision = fb_collision_to_collision(fb_model->collisionShape_type(), fb_model->collisionShape());
			m_assert(fb_model->file(), "");
			char model_file[256];
			snprintf(model_file, sizeof(model_file), "assets/models/%s", fb_model->file()->c_str());
			m_assert(add_model(world, d3d, model_file, transform, collision), "");
		}
	}

	auto fb_player = fb_world->player();
	if (fb_player) {
		world->player.model_index = fb_player->modelIndex();
		if (fb_player->transform()) {
			memcpy(&world->player.transform, fb_player->transform(), sizeof(struct transform));
		}
		else {
			world->player.transform = transform_identity();
		}
		if (world->player.model_index < world->model_count) {
			model *model = &world->models[world->player.model_index];
			world->player.bt_collision_shape = collision_to_bt_collision(model->collision);
			world->player.feet_translate = collision_get_bottom(model->collision);
		}
		else {
			world->player.bt_collision_shape = new btSphereShape(0.1f);
			world->player.feet_translate = -0.1f;
		}
		world->player.bt_rigid_body = new btRigidBody(10, nullptr, world->player.bt_collision_shape);
		btTransform bt_transform(btQuaternion(m_unpack4(world->player.transform.rotate)), btVector3(m_unpack3(world->player.transform.translate)));
		world->player.bt_rigid_body->setWorldTransform(bt_transform);
		world->player.bt_rigid_body->setActivationState(DISABLE_DEACTIVATION);
		world->bt_dynamics_world->addRigidBody(world->player.bt_rigid_body);
	}

	auto fb_static_objects = fb_world->staticObjects();
	if (fb_static_objects) {
		m_assert(fb_static_objects->size() < world->static_object_capacity, "");
		world->static_object_count = fb_static_objects->size();
		for (uint32 i = 0; i < world->static_object_count; i += 1) {
			auto fb_static_object = (*fb_static_objects)[i];
			auto static_object = &world->static_objects[i];
			*static_object = {};
			static_object->model_index = fb_static_object->modelIndex();
			if (fb_static_object->transform()) {
				memcpy(&static_object->transform, fb_static_object->transform(), sizeof(struct transform));
			}
			else {
				static_object->transform = transform_identity();
			}
			m_assert(fb_static_object->id(), "");
			static_object->id = allocate_memory<char>(&world->general_memory_arena, fb_static_object->id()->size() + 1);
			strcpy(static_object->id, fb_static_object->id()->c_str());

			if (static_object->model_index < world->model_count) {
				model *model = &world->models[static_object->model_index];
				static_object->bt_collision_shape = collision_to_bt_collision(model->collision);
			}
			else {
				static_object->bt_collision_shape = new btSphereShape(0.1f);
			}
			static_object->bt_collision_object = new btCollisionObject();
			static_object->bt_collision_object->setCollisionShape(static_object->bt_collision_shape);
			btTransform bt_transform(btQuaternion(m_unpack4(static_object->transform.rotate)), btVector3(m_unpack3(static_object->transform.translate)));
			static_object->bt_collision_object->setWorldTransform(bt_transform);
			world->bt_dynamics_world->addCollisionObject(static_object->bt_collision_object);
		}
	}

	auto fb_dynamic_objects = fb_world->dynamicObjects();
	if (fb_dynamic_objects) {
		m_assert(fb_dynamic_objects->size() < world->dynamic_object_capacity, "");
		world->dynamic_object_count = fb_dynamic_objects->size();
		for (uint32 i = 0; i < world->dynamic_object_count; i += 1) {
			auto fb_dynamic_object = (*fb_dynamic_objects)[i];
			auto dynamic_object = &world->dynamic_objects[i];
			*dynamic_object = {};
			dynamic_object->model_index = fb_dynamic_object->modelIndex();
			if (fb_dynamic_object->transform()) {
				memcpy(&dynamic_object->transform, fb_dynamic_object->transform(), sizeof(struct transform));
			}
			else {
				dynamic_object->transform = transform_identity();
			}
			m_assert(fb_dynamic_object->id(), "");
			dynamic_object->id = allocate_memory<char>(&world->general_memory_arena, fb_dynamic_object->id()->size() + 1);
			strcpy(dynamic_object->id, fb_dynamic_object->id()->c_str());

			if (dynamic_object->model_index < world->model_count) {
				model *model = &world->models[dynamic_object->model_index];
				dynamic_object->bt_collision_shape = collision_to_bt_collision(model->collision);
			}
			else {
				dynamic_object->bt_collision_shape = new btSphereShape(0.1f);
			}
			dynamic_object->bt_rigid_body = new btRigidBody(10, nullptr, dynamic_object->bt_collision_shape);
			btTransform bt_transform(btQuaternion(m_unpack4(dynamic_object->transform.rotate)), btVector3(m_unpack3(dynamic_object->transform.translate)));
			dynamic_object->bt_rigid_body->setWorldTransform(bt_transform);
			world->bt_dynamics_world->addRigidBody(dynamic_object->bt_rigid_body);
		}
	}

	auto fb_terrains = fb_world->terrains();
	if (fb_terrains) {
		m_assert(fb_terrains->size() < world->terrain_capacity, "");
		for (uint32 i = 0; i < fb_terrains->size(); i += 1) {
			auto fb_terrain = (*fb_terrains)[i];
			// auto terrain = &world->terrains[i];
			m_assert(fb_terrain->file(), "");
			char terrain_file[256];
			snprintf(terrain_file, sizeof(terrain_file), "assets/terrains/%s", fb_terrain->file()->c_str());
			m_assert(add_terrain(world, d3d, terrain_file), "");
		}
	}

	auto fb_skyboxes = fb_world->skyboxes();
	if (fb_skyboxes) {
		m_assert(fb_skyboxes->size() < world->skybox_capacity, "");
		for (uint32 i = 0; i < fb_skyboxes->size(); i += 1) {
			auto fb_skybox = (*fb_skyboxes)[i];
			// auto skybox = &world->skyboxes[i];
			m_assert(fb_skybox->file(), "");
			char skybox_file[256];
			snprintf(skybox_file, sizeof(skybox_file), "assets/skyboxes/%s", fb_skybox->file()->c_str());
			m_assert(add_skybox(world, d3d, skybox_file), "");
		}
	}

	auto sun_light_dir = fb_world->sunLightDir();
	if (sun_light_dir) {
		world->sun_light_dir = {sun_light_dir->x(), sun_light_dir->y(), sun_light_dir->z()};
	}
	else {
		world->sun_light_dir = vec3_normalize({1, 1, 1});
	}
	auto sun_light_color = fb_world->sunLightDir();
	if (sun_light_color) {
		world->sun_light_color = {sun_light_color->x(), sun_light_color->y(), sun_light_color->z()};
	}
	else {
		world->sun_light_color = {0.5f, 0.5f, 0.5f};
	}

	auto editor_camera = fb_world->editorCamera();
	if (editor_camera && editor_settings) {
		editor_settings->camera_position = vec3{editor_camera->position().x(), editor_camera->position().y(), editor_camera->position().z()};
		editor_settings->camera_view = vec3{editor_camera->view().x(), editor_camera->view().y(), editor_camera->view().z()};
	}
	else if (editor_settings) {
		editor_settings->camera_position = vec3{20, 20, 20};
		editor_settings->camera_view = vec3_normalize(-editor_settings->camera_position);
	}
	
	return true;
}

bool save_world(world *world, const char *file, world_editor_settings *editor_settings) {
	using namespace flatbuffers;

	FlatBufferBuilder fb_builder;

	Offset<Player> fb_player = 0;
	{
		Transform transform;
		memcpy(&transform, &world->player.transform, sizeof(struct transform));
		fb_player = CreatePlayer(fb_builder, world->player.model_index, &transform);
	}
	Offset<Vector<Offset<StaticObject>>> fb_static_objects = 0;
	{
		std::vector<Offset<StaticObject>> static_objects(world->static_object_count);
		for (uint32 i = 0; i < world->static_object_count; i += 1) {
			auto static_object = &world->static_objects[i];
			auto id = fb_builder.CreateString(static_object->id);
			Transform transform;
			memcpy(&transform, &static_object->transform, sizeof(struct transform));
			static_objects[i] = CreateStaticObject(fb_builder, id, static_object->model_index, &transform);
		}
		fb_static_objects = fb_builder.CreateVector<Offset<StaticObject>>(static_objects);
	}
	Offset<Vector<Offset<DynamicObject>>> fb_dynamic_objects = 0;
	{
		std::vector<Offset<DynamicObject>> dynamic_objects(world->dynamic_object_count);
		for (uint32 i = 0; i < world->dynamic_object_count; i += 1) {
			auto dynamic_object = &world->dynamic_objects[i];
			auto id = fb_builder.CreateString(dynamic_object->id);
			Transform transform;
			memcpy(&transform, &dynamic_object->transform, sizeof(struct transform));
			float mass = 0;
			dynamic_objects[i] = CreateDynamicObject(fb_builder, id, dynamic_object->model_index, &transform, mass);
		}
		fb_dynamic_objects = fb_builder.CreateVector<Offset<DynamicObject>>(dynamic_objects);
	}
	Offset<Vector<Offset<Model>>> fb_models = 0;
	{
		std::vector<Offset<Model>> models(world->model_count);
		for (uint32 i = 0; i < world->model_count; i += 1) {
			auto model = &world->models[i];
			auto file = fb_builder.CreateString(model->file);
			Transform transform;
			memcpy(&transform, &model->transform, sizeof(struct transform));
			CollisionShape collision_shape_type = CollisionShape_NONE;
			Offset<void> collision_shape = 0;
			if (model->collision.type == collision_type_sphere) {
				collision_shape_type = CollisionShape_Sphere;
				collision_shape = fb_builder.CreateStruct(Sphere(model->collision.sphere.radius)).Union();
			}
			else if (model->collision.type == collision_type_box) {
				collision_shape_type = CollisionShape_Box;
				collision_shape = fb_builder.CreateStruct(Box(Vec3(m_unpack3(model->collision.box.extents)))).Union();
			}
			else if (model->collision.type == collision_type_capsule_x) {
				collision_shape_type = CollisionShape_CapsuleX;
				collision_shape = fb_builder.CreateStruct(CapsuleX(model->collision.capsule.radius, model->collision.capsule.height)).Union();
			}
			else if (model->collision.type == collision_type_capsule_y) {
				collision_shape_type = CollisionShape_CapsuleY;
				collision_shape = fb_builder.CreateStruct(CapsuleY(model->collision.capsule.radius, model->collision.capsule.height)).Union();
			}
			else if (model->collision.type == collision_type_capsule_z) {
				collision_shape_type = CollisionShape_CapsuleZ;
				collision_shape = fb_builder.CreateStruct(CapsuleZ(model->collision.capsule.radius, model->collision.capsule.height)).Union();
			}
			models[i] = CreateModel(fb_builder, file, &transform, collision_shape_type, collision_shape);
		}
		fb_models = fb_builder.CreateVector<Offset<Model>>(models);
	}
	Offset<Vector<Offset<Terrain>>> fb_terrains = 0;
	{
		std::vector<Offset<Terrain>> terrains(world->terrain_count);
		for (uint32 i = 0; i < world->terrain_count; i += 1) {
			auto terrain = &world->terrains[i];
			auto file = fb_builder.CreateString(terrain->file);
			terrains[i] = CreateTerrain(fb_builder, file);
		}
		fb_terrains = fb_builder.CreateVector<Offset<Terrain>>(terrains);
	}
	Offset<Vector<Offset<Skybox>>> fb_skyboxes = 0;
	{
		std::vector<Offset<Skybox>> skyboxes(world->skybox_count);
		for (uint32 i = 0; i < world->skybox_count; i += 1) {
			auto skybox = &world->skyboxes[i];
			auto file = fb_builder.CreateString(skybox->file);
			skyboxes[i] = CreateSkybox(fb_builder, file);
		}
		fb_skyboxes = fb_builder.CreateVector<Offset<Skybox>>(skyboxes);
	}

	Vec3 fb_sun_light_dir(world->sun_light_dir.x, world->sun_light_dir.y, world->sun_light_dir.z);
	Vec3 fb_sun_light_color(world->sun_light_color.x, world->sun_light_color.y, world->sun_light_color.z);

	Camera fb_editor_camera(Vec3(m_unpack3(editor_settings->camera_position)), Vec3(m_unpack3(editor_settings->camera_view)));
	
	auto fb_world = CreateWorld(fb_builder, fb_player, fb_static_objects, fb_dynamic_objects, fb_models, fb_terrains, fb_skyboxes, &fb_sun_light_dir, &fb_sun_light_color, &fb_editor_camera);
	FinishWorldBuffer(fb_builder, fb_world);

	file_mapping save_file_mapping;
	if (!create_file_mapping(file, fb_builder.GetSize(), &save_file_mapping)) {
		return false;
	}
	memcpy(save_file_mapping.ptr, fb_builder.GetBufferPointer(), fb_builder.GetSize());
	flush_file_mapping(save_file_mapping);
	close_file_mapping(save_file_mapping);

	return true;
}

uint32 append_world_constant_buffer(world *world, void *data, uint32 data_size) {
	round_up(&world->constant_buffer_offset, 256u);
	uint32 offset = world->constant_buffer_offset;
	memcpy(world->constant_buffer_ptr + world->constant_buffer_offset, data, data_size);
	world->constant_buffer_offset += data_size;
	m_debug_assert(world->constant_buffer_offset <= world->constant_buffer_capacity, "");
	return offset;
}

void add_model_render_data(world *world, d3d *d3d, uint32 model_index, transform transform, uint32 animation_index, double animation_time, bool render_collision_shapes) {
	model *model = &world->models[model_index];

	struct {
		XMMATRIX model_mat;
	} model_constant_buffer = {
		XMMatrixMultiply(xmmatrix_transform(&model->transform), xmmatrix_transform(&transform))
	};

	model_render_data *model_render_data = allocate_memory<struct model_render_data>(&world->frame_memory_arena, 1);
	model_render_data->model = model;
	model_render_data->model_mat_constant_buffer_offset = append_world_constant_buffer(world, &model_constant_buffer, sizeof(model_constant_buffer));
	model_render_data->meshes = allocate_memory<struct mesh_render_data>(&world->frame_memory_arena, model->mesh_node_count);
	model_render_data->mesh_count = model->mesh_node_count;
	model_render_data->render_collision_shapes = render_collision_shapes;
	model_render_data->next = world->render_data.model_list;

	world->render_data.model_list = model_render_data;
	world->render_data.model_count += 1;

	uint32 *joint_mats_offsets = allocate_memory<uint32>(&world->frame_memory_arena, model->skin_count);
	{
		undo_allocation_scope_exit(&world->frame_memory_arena);

		model_node *model_nodes = model->nodes;
		if (animation_index < model->animation_count) {
			model_animation *animation = &model->animations[animation_index];
		
			model_nodes = allocate_memory<struct model_node>(&world->frame_memory_arena, model->node_count);
			memcpy(model_nodes, model->nodes, model->node_count * sizeof(struct model_node));

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
								translate = vec3_lerp(model_nodes[channel->node_index].local_transform.translate, {m_unpack3(key_frame->transform_data)}, key_frame->time == 0 ? 1 : time / key_frame->time);
							}
							else {
								model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
								translate = vec3_lerp({m_unpack3(prev_key_frame->transform_data)}, {m_unpack3(key_frame->transform_data)}, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
							}
							model_nodes[channel->node_index].local_transform.translate = translate;
						}
						else if (channel->channel_type == gpk_model_animation_rotate_channel) {
							quat rotate = {};
							if (i == 0) {
								rotate = quat_slerp(model_nodes[channel->node_index].local_transform.rotate, {m_unpack4(key_frame->transform_data)}, key_frame->time == 0 ? 1 : time / key_frame->time);
							}
							else {
								model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
								rotate = quat_slerp({m_unpack4(prev_key_frame->transform_data)}, {m_unpack4(key_frame->transform_data)}, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
							}
							model_nodes[channel->node_index].local_transform.rotate = rotate;
						}
						else if (channel->channel_type == gpk_model_animation_scale_channel) {
							vec3 scale = {};
							if (i == 0) {
								scale = vec3_lerp(model_nodes[channel->node_index].local_transform.scale, {m_unpack3(key_frame->transform_data)}, key_frame->time == 0 ? 1 : time / key_frame->time);
							}
							else {
								model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
								scale = vec3_lerp({m_unpack3(prev_key_frame->transform_data)}, {m_unpack3(key_frame->transform_data)}, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
							}
							model_nodes[channel->node_index].local_transform.scale = scale;
						}
						break;
					}
				}
			}
			for (uint32 i = 0; i < model->node_count; i += 1) {
				model_nodes[i].local_transform_mat = mat4_from_transform(model_nodes[i].local_transform);
			}
			for (auto &scene : make_range(model->scenes, model->scene_count)) {
				for (auto &node_index : make_range(scene.node_indices, scene.node_index_count)) {
					model_node *node = &model_nodes[node_index];
					node->global_transform_mat = node->local_transform_mat;
					std::stack<model_node *> node_stack;
					node_stack.push(node);
					while (!node_stack.empty()) {
						model_node *parent_node = node_stack.top();
						node_stack.pop();
						for (uint32 i = 0; i < parent_node->child_count; i += 1) {
							model_node *child_node = &model_nodes[parent_node->children[i]];
							child_node->global_transform_mat = parent_node->global_transform_mat * child_node->local_transform_mat;
							node_stack.push(child_node);
						}
					}
				}
			}
		}
		for (uint32 i = 0; i < model->skin_count; i += 1) {
			model_skin *skin = &model->skins[i];
			mat4 *joint_mats = allocate_memory<mat4>(&world->frame_memory_arena, skin->joint_count);
			for (uint32 i = 0; i < skin->joint_count; i += 1) {
				joint_mats[i] = model_nodes[skin->joints[i].node_index].global_transform_mat * skin->joints[i].inverse_bind_mat;
			}
			joint_mats_offsets[i] = append_world_constant_buffer(world, joint_mats, skin->joint_count * sizeof(mat4));
		}
	}
	uint32 mesh_index = 0;
	for (uint32 i = 0; i < model->node_count; i += 1) {
		model_node *node = &model->nodes[i];
		if (node->mesh_index < model->mesh_count) {
			mesh_render_data *mesh_render_data = &model_render_data->meshes[mesh_index++];
			mesh_render_data->mesh = &model->meshes[node->mesh_index];
			if (node->skin_index < model->skin_count) {
				mesh_render_data->joint_mats_constant_buffer_offset = joint_mats_offsets[node->skin_index];
			}
			else {
				mesh_render_data->joint_mats_constant_buffer_offset = append_world_constant_buffer(world, &node->global_transform_mat, sizeof(mat4));
			}
			mesh_render_data->primitive_count = mesh_render_data->mesh->primitive_count;
			mesh_render_data->primitives = allocate_memory<mesh_primitive_render_data>(&world->frame_memory_arena, mesh_render_data->primitive_count);
			for (uint32 i = 0; i < mesh_render_data->primitive_count; i += 1) {
				model_mesh_primitive *primitive = &mesh_render_data->mesh->primitives[i];
				mesh_primitive_render_data *primitive_render_data = &mesh_render_data->primitives[i];
				model_material *material = primitive->material_index < model->material_count ? &model->materials[primitive->material_index] : nullptr;
				struct {
					vec4 diffuse_factor;
					float metallic_factor;
					float roughness_factor;
				} constant_buffer = {
					material ? material->diffuse_factor : vec4{1, 1, 1, 1},
					material ? material->metallic_factor : 1,
					material ? material->roughness_factor : 1
				};
				primitive_render_data->constant_buffer_offset = append_world_constant_buffer(world, &constant_buffer, sizeof(constant_buffer));
			}
		}
	}

	if (render_collision_shapes) {
		if (model->collision.type == collision_type_sphere) {
			model_render_data->shape_count = 1;
			model_render_data->shape_vertex_buffers[0] = world->sphere_vertex_buffer;
			model_render_data->shape_vertex_counts[0] = m_countof(sphere_vertices);
			float radius = model->collision.sphere.radius;
			XMMATRIX transform_mat = XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), xmmatrix_transform(&transform));
			model_render_data->shape_constant_buffer_offsets[0] = append_world_constant_buffer(world, &transform_mat, sizeof(transform_mat));
		}
		else if (model->collision.type == collision_type_box) {
			model_render_data->shape_count = 1;
			model_render_data->shape_vertex_buffers[0] = world->box_vertex_buffer;
			model_render_data->shape_vertex_counts[0] = m_countof(box_vertices);
			vec3 extents = model->collision.box.extents;
			XMMATRIX transform_mat = XMMatrixMultiply(XMMatrixScaling(extents.x, extents.y, extents.z), xmmatrix_transform(&transform));
			model_render_data->shape_constant_buffer_offsets[0] = append_world_constant_buffer(world, &transform_mat, sizeof(transform_mat));
		}
		else if (model->collision.type == collision_type_capsule_x || model->collision.type == collision_type_capsule_y || model->collision.type == collision_type_capsule_z) {
			model_render_data->shape_count = 3;
			model_render_data->shape_vertex_buffers[0] = world->hollow_cylinder_vertex_buffer;
			model_render_data->shape_vertex_buffers[1] = world->hemisphere_vertex_buffer;
			model_render_data->shape_vertex_buffers[2] = world->hemisphere_vertex_buffer;
			model_render_data->shape_vertex_counts[0] = sizeof(hollow_cylinder_vertices);
			model_render_data->shape_vertex_counts[1] = sizeof(hemisphere_vertices);
			model_render_data->shape_vertex_counts[2] = sizeof(hemisphere_vertices);
			
			XMMATRIX transform_mat = xmmatrix_transform(&transform);
			if (model->collision.type == collision_type_capsule_x) {
				transform_mat = XMMatrixMultiply(XMMatrixRotationAxis(XMVectorSet(0, 0, 1, 0), (float)M_PI_2), transform_mat);
			}
			else if (model->collision.type == collision_type_capsule_z) {
				transform_mat = XMMatrixMultiply(XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), (float)M_PI_2), transform_mat);
			}
			float radius = model->collision.capsule.radius;
			float height = model->collision.capsule.height * 0.5f;
			XMMATRIX transform_mats[3];
			transform_mats[0] = XMMatrixMultiply(XMMatrixScaling(radius, height, radius), transform_mat);
			transform_mats[1] = XMMatrixMultiply(XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), XMMatrixTranslation(0, height, 0)), transform_mat);
			transform_mats[2] = XMMatrixMultiply(XMMatrixMultiply(XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), XMMatrixRotationAxis(XMVectorSet(0, 0, 1, 0), (float)M_PI)), XMMatrixTranslation(0, -height, 0)), transform_mat);
			model_render_data->shape_constant_buffer_offsets[0] = append_world_constant_buffer(world, &transform_mats[0], sizeof(transform_mats[0]));
			model_render_data->shape_constant_buffer_offsets[1] = append_world_constant_buffer(world, &transform_mats[1], sizeof(transform_mats[1]));
			model_render_data->shape_constant_buffer_offsets[2] = append_world_constant_buffer(world, &transform_mats[2], sizeof(transform_mats[2]));
		}
		else {
			model_render_data->shape_count = 0;
		}
	}
}

struct render_world_desc {
	XMMATRIX camera_view_proj_mat;
	vec3 camera_position;
	vec3 camera_view;

	bool render_models;
	bool render_terrain;
	bool render_skybox;
	bool render_shadow_proj_box;
	bool screenshot;
	struct editor *editor;
	void (*append_extra_model_constants)(struct editor*, world*, d3d*);
	void (*render_reference_grid)(struct editor*, world*, d3d*);
	void (*append_terrain_brush_constants)(struct editor*, world*, d3d*);
	void (*render_terrain_brush)(struct editor*, world*, d3d*);
	void (*render_imgui)(struct editor*, world*, d3d*);
};

void render_world(world *world, d3d *d3d, render_world_desc *render_world_desc) {
	{ // update constant buffer
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		m_d3d_assert(d3d->context->Map(world->constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource));
		world->constant_buffer_ptr = (uint8 *)mapped_subresource.pData;
		world->constant_buffer_offset = 0;

		vec3 shadow_proj_box_size = {128, 128, 128};
		vec3 shadow_proj_eye = world->sun_light_dir * shadow_proj_box_size.z * 0.5f;
		vec3 shadow_proj_box_translate = shadow_proj_eye - (vec3_normalize(shadow_proj_eye) * shadow_proj_box_size.z * 0.5f);
		struct {
			uint32 swap_chain_width;
			uint32 swap_chain_height;
			uint32 padding0[2];
			XMMATRIX camera_view_proj_mat;
			vec4 camera_position;
			vec4 camera_view;
			vec4 sun_light_dir;
			vec4 sun_light_color;
			XMMATRIX shadow_proj_mat;
		} common_constants = {
			d3d->swap_chain_desc.Width,
			d3d->swap_chain_desc.Height,
			{},
			render_world_desc->camera_view_proj_mat,
			vec4{m_unpack3(render_world_desc->camera_position), 0},
			vec4{m_unpack3(render_world_desc->camera_view), 0},
			vec4{world->sun_light_dir.x, world->sun_light_dir.y, world->sun_light_dir.z, 0},
			vec4{world->sun_light_color.x, world->sun_light_color.y, world->sun_light_color.z, 0},
			XMMatrixMultiply(XMMatrixLookAtRH(XMVectorSet(m_unpack3(shadow_proj_eye), 0), XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0)), XMMatrixOrthographicRH(m_unpack3(shadow_proj_box_size), 0))
		};
		append_world_constant_buffer(world, &common_constants, sizeof(common_constants));

		world->render_data.model_list = nullptr;
		world->render_data.model_count = 0;
		if (render_world_desc->render_models) {
			if (world->player.model_index < world->model_count) {
				add_model_render_data(world, d3d, world->player.model_index, world->player.transform, world->player.animation_index, world->player.animation_time, false);
			}
		}

		if (render_world_desc->append_extra_model_constants) {
			render_world_desc->append_extra_model_constants(render_world_desc->editor, world, d3d);
		}

		if (render_world_desc->render_terrain && world->terrain_index < world->terrain_count) {
			terrain *terrain = &world->terrains[world->terrain_index];
			struct {
				float max_height;
				float sample_per_meter;
			} terrain_constant_buffer = {
				terrain->max_height,
				(float)terrain->sample_per_meter
			};
			uint32 offset = append_world_constant_buffer(world, &terrain_constant_buffer, sizeof(terrain_constant_buffer));
			world->render_data.terrain_constant_buffer_offset = offset / 16;
		}

		if (render_world_desc->append_terrain_brush_constants) {
			render_world_desc->append_terrain_brush_constants(render_world_desc->editor, world, d3d);
		}

		if (render_world_desc->render_shadow_proj_box) {
			quat q = quat_from_between({0, 0, 1}, vec3_normalize(shadow_proj_eye));
			XMMATRIX transform_mat = XMMatrixScaling(m_unpack3(shadow_proj_box_size));
			transform_mat = XMMatrixMultiply(transform_mat, XMMatrixRotationQuaternion(XMVectorSet(m_unpack4(q))));
			transform_mat = XMMatrixMultiply(transform_mat, XMMatrixTranslation(m_unpack3(shadow_proj_box_translate)));
			world->render_data.shadow_proj_box_constant_buffer_offset = append_world_constant_buffer(world, &transform_mat, sizeof(transform_mat));
		}
				
		d3d->context->Unmap(world->constant_buffer, 0);

		uint32 offset = 0;
		uint32 constant_count = 16;
		d3d->context->VSSetConstantBuffers1(0, 1, &world->constant_buffer, &offset, &constant_count);
		d3d->context->PSSetConstantBuffers1(0, 1, &world->constant_buffer, &offset, &constant_count);
	}
	{ // prepare shadow framebuffer
		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)d3d->shadow_framebuffer_width;
		viewport.Height = (float)d3d->shadow_framebuffer_height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		d3d->context->RSSetViewports(1, &viewport);
		
		d3d->context->ClearRenderTargetView(d3d->shadow_framebuffer_render_target_views[0], Colors::Black);
		d3d->context->ClearRenderTargetView(d3d->shadow_framebuffer_render_target_views[1], Colors::Black);
		d3d->context->ClearDepthStencilView(d3d->shadow_framebuffer_depth_view, D3D11_CLEAR_DEPTH, 0, 0);
		d3d->context->OMSetRenderTargets(1, &d3d->shadow_framebuffer_render_target_views[0], d3d->shadow_framebuffer_depth_view);
	}
	{ // render shadows
		d3d->context->VSSetShader(d3d->mesh_shadow_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->mesh_shadow_ps, nullptr, 0);
		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->context->IASetInputLayout(d3d->mesh_input_layout);
		d3d->context->RSSetState(d3d->mesh_rasterizer_state);
		d3d->context->OMSetDepthStencilState(d3d->mesh_depth_stencil_state, 0);
		d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		model_render_data *model_render_data = world->render_data.model_list;
		while (model_render_data) {
			// model *model = model_render_data->model;
			uint32 model_constant_buffer_offset = model_render_data->model_mat_constant_buffer_offset / 16;
			uint32 model_constant_count = 16;
			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &model_constant_buffer_offset, &model_constant_count);
			for (uint32 i = 0; i < model_render_data->mesh_count; i += 1) {
				mesh_render_data *mesh_render_data = &model_render_data->meshes[i];
				model_mesh *mesh = mesh_render_data->mesh;
				uint32 mesh_constant_buffer_offset = mesh_render_data->joint_mats_constant_buffer_offset / 16;
				uint32 mesh_constant_count = 256 * 4;
				d3d->context->VSSetConstantBuffers1(2, 1, &world->constant_buffer, &mesh_constant_buffer_offset, &mesh_constant_count);
				for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
					model_mesh_primitive *primitive = &mesh->primitives[i];
					uint32 vertex_buffer_stride = sizeof(gpk_model_vertex);
					uint32 vertex_buffer_offset = 0;
					d3d->context->IASetVertexBuffers(0, 1, &primitive->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
					d3d->context->IASetIndexBuffer(primitive->index_buffer, DXGI_FORMAT_R16_UINT, 0);
					d3d->context->DrawIndexed(primitive->index_count, 0, 0);
				}
			}
			model_render_data = model_render_data->next;
		}

		if (render_world_desc->render_terrain && world->terrain_index < world->terrain_count) {
			terrain *terrain = &world->terrains[world->terrain_index];
			d3d->context->VSSetShader(d3d->terrain_shadow_vs, nullptr, 0);
			d3d->context->PSSetShader(d3d->terrain_shadow_ps, nullptr, 0);
			d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			d3d->context->IASetInputLayout(d3d->terrain_input_layout);
			d3d->context->VSSetShaderResources(0, 1, &terrain->height_texture_view);
			d3d->context->VSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
			uint32 constant_count = 16;
			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &world->render_data.terrain_constant_buffer_offset, &constant_count);
			uint32 vertex_buffer_stride = sizeof(terrain_vertex);
			uint32 vertex_buffer_offset = 0;
			d3d->context->IASetVertexBuffers(0, 1, &terrain->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
			d3d->context->IASetIndexBuffer(terrain->index_buffer, DXGI_FORMAT_R32_UINT, 0);
			d3d->context->DrawIndexed(terrain->index_count, 0, 0);
		}
	}
	{ // gaussian blur shadow
		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->context->IASetInputLayout(d3d->terrain_input_layout);
		d3d->context->RSSetState(nullptr);
		d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
		d3d->context->OMSetDepthStencilState(nullptr, 0);

		d3d->context->OMSetRenderTargets(1, &d3d->shadow_framebuffer_render_target_views[1], nullptr);
		d3d->context->VSSetShader(d3d->gaussian_blur_x_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->gaussian_blur_x_ps, nullptr, 0);
		d3d->context->PSSetShaderResources(0, 1, &d3d->shadow_framebuffer_texture_views[0]);
		d3d->context->Draw(3, 0);

		ID3D11ShaderResourceView *null_view = nullptr;
		d3d->context->PSSetShaderResources(0, 1, &null_view);
		d3d->context->OMSetRenderTargets(1, &d3d->shadow_framebuffer_render_target_views[0], nullptr);
		d3d->context->PSSetShaderResources(0, 1, &d3d->shadow_framebuffer_texture_views[1]);
		d3d->context->VSSetShader(d3d->gaussian_blur_y_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->gaussian_blur_y_ps, nullptr, 0);
		d3d->context->Draw(3, 0);
	}
	{ // prepare final framebuffer
		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)d3d->final_framebuffer_width;
		viewport.Height = (float)d3d->final_framebuffer_height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		d3d->context->RSSetViewports(1, &viewport);

		d3d->context->ClearRenderTargetView(d3d->final_framebuffer_render_target_view, Colors::Black);
		d3d->context->ClearDepthStencilView(d3d->final_framebuffer_depth_view, D3D11_CLEAR_DEPTH, 0, 0);
		d3d->context->OMSetRenderTargets(1, &d3d->final_framebuffer_render_target_view, d3d->final_framebuffer_depth_view);
	}
	{ // render models
		d3d->context->VSSetShader(d3d->mesh_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->mesh_ps, nullptr, 0);
		d3d->context->PSSetSamplers(0, 1, &d3d->repeat_sampler_state);
		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->context->IASetInputLayout(d3d->mesh_input_layout);
		d3d->context->RSSetState(d3d->mesh_rasterizer_state);
		d3d->context->OMSetDepthStencilState(d3d->mesh_depth_stencil_state, 0);
		d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		
		model_render_data *model_render_data = world->render_data.model_list;
		while (model_render_data) {
			model *model = model_render_data->model;
			uint32 model_constant_buffer_offset = model_render_data->model_mat_constant_buffer_offset / 16;
			uint32 model_constant_count = 16;
			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &model_constant_buffer_offset, &model_constant_count);
			for (uint32 i = 0; i < model_render_data->mesh_count; i += 1) {
				mesh_render_data *mesh_render_data = &model_render_data->meshes[i];
				model_mesh *mesh = mesh_render_data->mesh;
				uint32 mesh_constant_buffer_offset = mesh_render_data->joint_mats_constant_buffer_offset / 16;
				uint32 mesh_constant_count = 256 * 4;
				d3d->context->VSSetConstantBuffers1(2, 1, &world->constant_buffer, &mesh_constant_buffer_offset, &mesh_constant_count);
				for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
					model_mesh_primitive *primitive = &mesh->primitives[i];
					mesh_primitive_render_data *primitive_render_data = &mesh_render_data->primitives[i];
					uint32 primitive_constant_buffer_offset = primitive_render_data->constant_buffer_offset / 16;
					uint32 primitive_constant_count = 16;
					d3d->context->PSSetConstantBuffers1(3, 1, &world->constant_buffer, &primitive_constant_buffer_offset, &primitive_constant_count);
					model_material *material = primitive->material_index < model->material_count ? &model->materials[primitive->material_index] : nullptr;
					ID3D11ShaderResourceView *texture_views[4] = {world->default_diffuse_texture_view, world->default_roughness_texture_view,
																												world->default_metallic_texture_view, world->default_normal_texture_view};
					if (material && material->diffuse_texture_index < model->texture_count) {
						texture_views[0] = model->textures[material->diffuse_texture_index].texture_view;
					}
					if (material && material->roughness_texture_index < model->texture_count) {
						texture_views[1] = model->textures[material->roughness_texture_index].texture_view;
					}
					if (material && material->metallic_texture_index < model->texture_count) {
						texture_views[2] = model->textures[material->metallic_texture_index].texture_view;
					}
					if (material && material->normal_texture_index < model->texture_count) {
						texture_views[3] = model->textures[material->normal_texture_index].texture_view;
					}
					d3d->context->PSSetShaderResources(0, m_countof(texture_views), texture_views);
					uint32 vertex_buffer_stride = sizeof(gpk_model_vertex);
					uint32 vertex_buffer_offset = 0;
					d3d->context->IASetVertexBuffers(0, 1, &primitive->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
					d3d->context->IASetIndexBuffer(primitive->index_buffer, DXGI_FORMAT_R16_UINT, 0);
					d3d->context->DrawIndexed(primitive->index_count, 0, 0);
				}
			}
			model_render_data = model_render_data->next;
		}

		d3d->context->VSSetShader(d3d->collision_shape_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->collision_shape_ps, nullptr, 0);
		d3d->context->IASetInputLayout(d3d->collision_shape_input_layout);
		d3d->context->OMSetDepthStencilState(d3d->collision_shape_depth_stencil_state, 0);
		d3d->context->OMSetBlendState(d3d->collision_shape_blend_state, nullptr, 0xffffffff);

		model_render_data = world->render_data.model_list;
		while (model_render_data) {
			for (uint32 i = 0; i < model_render_data->shape_count; i += 1) {
				uint32 vertex_buffer_stride = 12;
				uint32 vertex_buffer_offset = 0;
				d3d->context->IASetVertexBuffers(0, 1, &model_render_data->shape_vertex_buffers[i], &vertex_buffer_stride, &vertex_buffer_offset);
				uint32 constant_buffer_offset = model_render_data->shape_constant_buffer_offsets[i] / 16;
				uint32 constant_count = 16;
				d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &constant_buffer_offset, &constant_count);
				d3d->context->Draw(model_render_data->shape_vertex_counts[i], 0);
			}
			model_render_data = model_render_data->next;
		}		
	}
	{ // render terrain
		if (render_world_desc->render_terrain && world->terrain_index < world->terrain_count) {
			terrain *terrain = &world->terrains[world->terrain_index];
			d3d->context->VSSetShader(d3d->terrain_vs, nullptr, 0);
			d3d->context->PSSetShader(d3d->terrain_ps, nullptr, 0);
			d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			d3d->context->IASetInputLayout(d3d->terrain_input_layout);
			d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
			d3d->context->OMSetDepthStencilState(d3d->terrain_depth_stencil_state, 0);
			d3d->context->VSSetShaderResources(0, 1, &terrain->height_texture_view);
			d3d->context->PSSetShaderResources(1, 1, &terrain->diffuse_texture_view);
			d3d->context->PSSetShaderResources(2, 1, &d3d->shadow_framebuffer_texture_views[0]);
			d3d->context->VSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
			d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
			d3d->context->PSSetSamplers(1, 1, &d3d->shadow_framebuffer_sampler_state);
			uint32 constant_count = 16;
			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &world->render_data.terrain_constant_buffer_offset, &constant_count);
			uint32 vertex_buffer_stride = sizeof(terrain_vertex);
			uint32 vertex_buffer_offset = 0;
			d3d->context->IASetVertexBuffers(0, 1, &terrain->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
			d3d->context->IASetIndexBuffer(terrain->index_buffer, DXGI_FORMAT_R32_UINT, 0);
			d3d->context->DrawIndexed(terrain->index_count, 0, 0);

			ID3D11ShaderResourceView *null_view = nullptr;
			d3d->context->PSSetShaderResources(2, 1, &null_view);
		}
	}
	{ // render skybox
		if (render_world_desc->render_skybox && world->skybox_index < world->skybox_count) {
			skybox *skybox = &world->skyboxes[world->skybox_index];
			d3d->context->VSSetShader(d3d->skybox_vs, nullptr, 0);
			d3d->context->PSSetShader(d3d->skybox_ps, nullptr, 0);
			d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			d3d->context->RSSetState(d3d->skybox_rasterizer_state);
			d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
			d3d->context->OMSetDepthStencilState(d3d->skybox_depth_stencil_state, 0);
			d3d->context->PSSetShaderResources(0, 1, &skybox->cube_texture_view);
			d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
			d3d->context->Draw(36, 0);
		}
	}
	if (render_world_desc->render_shadow_proj_box) {
		d3d->context->VSSetShader(d3d->collision_shape_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->collision_shape_ps, nullptr, 0);
		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->context->IASetInputLayout(d3d->collision_shape_input_layout);
		d3d->context->OMSetDepthStencilState(d3d->collision_shape_depth_stencil_state, 0);
		d3d->context->OMSetBlendState(d3d->collision_shape_blend_state, nullptr, 0xffffffff);

		uint32 vertex_buffer_stride = 12;
		uint32 vertex_buffer_offset = 0;
		d3d->context->IASetVertexBuffers(0, 1, &world->box_vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
		uint32 constant_buffer_offset = world->render_data.shadow_proj_box_constant_buffer_offset / 16;
		uint32 constant_count = 16;
		d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &constant_buffer_offset, &constant_count);
		d3d->context->Draw(m_countof(box_vertices), 0);
	}
	if (render_world_desc->render_reference_grid) {
		render_world_desc->render_reference_grid(render_world_desc->editor, world, d3d);
	}
	if (render_world_desc->render_terrain && render_world_desc->render_terrain_brush) {
		render_world_desc->render_terrain_brush(render_world_desc->editor, world, d3d);
	}
	{ // prepare swap chain
		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)d3d->swap_chain_desc.Width;
		viewport.Height = (float)d3d->swap_chain_desc.Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		d3d->context->RSSetViewports(1, &viewport);

		d3d->context->ClearRenderTargetView(d3d->swap_chain_render_target_view, Colors::Black);
		d3d->context->OMSetRenderTargets(1, &d3d->swap_chain_render_target_view, nullptr);
	}
	{ // final framebuffer to screen
		d3d->context->VSSetShader(d3d->final_framebuffer_to_screen_vs, nullptr, 0);
		d3d->context->PSSetShader(d3d->final_framebuffer_to_screen_ps, nullptr, 0);
		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3d->context->RSSetState(nullptr);
		d3d->context->PSSetShaderResources(0, 1, &d3d->final_framebuffer_texture_view);
		d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
		d3d->context->Draw(3, 0);
	}
	if (render_world_desc->render_imgui) {
		render_world_desc->render_imgui(render_world_desc->editor, world, d3d);
	}

	m_d3d_assert(d3d->swap_chain->Present(0, 0));
}
