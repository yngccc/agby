/****************************************************************************************************/
/*			Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.			*/
/****************************************************************************************************/

#ifndef __WORLD_CPP__
#define __WORLD_CPP__

#include "common.cpp"
#include "math.cpp"
#include "geometry.cpp"
#include "simd.cpp"
#include "gpk.cpp"
#include "d3d.cpp"

#include <stack>

#include <physx/PxPhysicsAPI.h>

enum collision_type {
	collision_type_none,
	collision_type_sphere,
	collision_type_box,
	collision_type_capsule
};

const char* collision_type_strs[] = { "none", "sphere", "box", "capsule" };

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

struct player {
	uint32 model_index;
	transform transform;
	uint32 animation_index;
	double animation_time;

	physx::PxController* physx_controller;
};

struct static_object {
	string id;
	uint32 model_index;
	transform transform;
	uint32 animation_index;
	double animation_time;

	physx::PxRigidStatic* physx_rigid_staic;
};

struct dynamic_object {
	string id;
	uint32 model_index;
	transform transform;
	uint32 animation_index;
	double animation_time;

	physx::PxRigidDynamic* physx_rigid_dynamic;
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
	ID3D12Resource* vertex_buffer;
	ID3D12Resource* index_buffer;
	uint32 vertex_count;
	uint32 index_count;
	uint32 material_index;
};

struct model_mesh {
	model_mesh_primitive* primitives;
	uint32 primitive_count;
	char name[sizeof(gpk_model_mesh::name)];
};

struct model_joint {
	uint32 node_index;
	mat4 inverse_bind_mat;
};

struct model_skin {
	model_joint* joints;
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
	model_animation_key_frame* key_frames;
	uint32 key_frame_count;
};

struct model_animation {
	model_animation_channel* channels;
	uint32 channel_count;
	model_animation_sampler* samplers;
	uint32 sampler_count;
	char name[sizeof(gpk_model_animation::name)];
};

struct model_material {
	uint32 diffuse_texture_index;
	uint32 normal_texture_index;
	uint32 roughness_texture_index;
	uint32 metallic_texture_index;
	uint32 emissive_texture_index;
	D3D12_GPU_DESCRIPTOR_HANDLE texture_srv_descriptors;
	vec4 diffuse_factor;
	float roughness_factor;
	float metallic_factor;
	vec3 emissive_factor;
	float transparency;
	float index_of_refraction;
	char name[sizeof(gpk_model_material::name)];
};

struct model_texture {
	ID3D12Resource* texture;
};

struct model {
	model_scene* scenes;
	uint32 scene_count;
	model_node* nodes;
	uint32 node_count;
	model_mesh* meshes;
	uint32 mesh_count;
	uint32 mesh_node_count;
	model_skin* skins;
	uint32 skin_count;
	model_animation* animations;
	uint32 animation_count;
	model_material* materials;
	uint32 material_count;
	model_texture* textures;
	uint32 texture_count;
	transform transform;
	collision collision;
	physx::PxGeometryHolder px_geometry_holder;
	char file_name[64];
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

	ID3D11Texture2D* height_texture;
	ID3D11ShaderResourceView* height_texture_view;
	int16* height_texture_data;

	ID3D11Texture2D* diffuse_texture;
	ID3D11ShaderResourceView* diffuse_texture_view;
	uint32* diffuse_texture_data;
	float diffuse_texture_uv_repeat;

	ID3D11Buffer* vertex_buffer;
	ID3D11Buffer* index_buffer;
	uint32 index_count;

	char file[32];
};

struct skybox {
	ID3D11Texture2D* cube_texture;
	ID3D11ShaderResourceView* cube_texture_view;
	char file[32];
};

struct mesh_primitive_render_data {
	uint32 constant_buffer_offset;
};

struct mesh_render_data {
	model_mesh* mesh;
	mesh_primitive_render_data* primitives;
	uint32 primitive_count;
	uint32 joint_mats_constant_buffer_offset;
};

struct model_render_data {
	model* model;
	uint32 model_mat_constant_buffer_offset;
	mesh_render_data* meshes;
	uint32 mesh_count;
	bool render_collision_shapes;
	uint32 shape_count;
	ID3D11Buffer* shape_vertex_buffers[3];
	uint32 shape_vertex_counts[3];
	uint32 shape_constant_buffer_offsets[3];
	model_render_data* next;
};

struct world_render_data {
	model_render_data* model_list;
	uint32 model_count;
	uint32 terrain_constant_buffer_offset;
	uint32 terrain_brush_constant_buffer_offset;
	uint32 shadow_proj_box_constant_buffer_offset;
};

struct direct_light {
	vec3 position;
	vec3 dir;
	vec3 color;
};

struct sphere_light {
	vec3 position;
	vec3 color;
	float radius;
};

struct world {
	memory_arena frame_memory_arena;

	world_render_data render_data;

	array<static_object> static_objects;
	array<dynamic_object> dynamic_objects;
	array<terrain> terrains;
	uint32 terrain_index;
	array<skybox> skyboxes;
	uint32 skybox_index;
	player player;

	array<model> models;
	array<direct_light> direct_lights;
	array<sphere_light> sphere_lights;

	ID3D12Resource* box_vertex_buffer;
	ID3D12Resource* sphere_vertex_buffer;
	ID3D12Resource* hemisphere_vertex_buffer;
	ID3D12Resource* cylinder_vertex_buffer;
	ID3D12Resource* hollow_cylinder_vertex_buffer;
	ID3D12Resource* hollow_circle_vertex_buffer;
	ID3D12Resource* torus_vertex_buffer;
	ID3D12Resource* reference_grid_vertex_buffer;
	ID3D12Resource* vertex_buffer_reset;

	ID3D12Resource* default_diffuse_texture;
	ID3D12Resource* default_normal_texture;
	ID3D12Resource* default_roughness_texture;
	ID3D12Resource* default_metallic_texture;
	ID3D12Resource* default_emissive_texture;
	ID3D12Resource* default_height_texture;
	D3D12_GPU_DESCRIPTOR_HANDLE default_material_srv_descriptors;

	physx::PxFoundation* px_foundation;
	physx::PxPvd* px_pvd;
	physx::PxPvdTransport* px_pvd_transport;
	physx::PxPhysics* px_physics;
	physx::PxScene* px_scene;
	physx::PxControllerManager* px_controller_manager;
};

struct world_render_params {
	XMMATRIX camera_view_project_mat;
	XMVECTOR camera_position;
	XMVECTOR camera_view;
	float camera_fovy;
};

void world_init(world* world, d3d12* d3d12) {
	m_assert(memory_arena_init(m_megabytes(16), &world->frame_memory_arena));

	world->box_vertex_buffer = d3d12_create_buffer(d3d12, sizeof(box_vertices), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	d3d12_copy_buffer(d3d12, world->box_vertex_buffer, box_vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	world->sphere_vertex_buffer = d3d12_create_buffer(d3d12, sizeof(sphere_vertices), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	d3d12_copy_buffer(d3d12, world->sphere_vertex_buffer, sphere_vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	world->hemisphere_vertex_buffer = d3d12_create_buffer(d3d12, sizeof(hemisphere_vertices), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	d3d12_copy_buffer(d3d12, world->hemisphere_vertex_buffer, hemisphere_vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	world->cylinder_vertex_buffer = d3d12_create_buffer(d3d12, sizeof(cylinder_vertices), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	d3d12_copy_buffer(d3d12, world->cylinder_vertex_buffer, cylinder_vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	world->hollow_cylinder_vertex_buffer = d3d12_create_buffer(d3d12, sizeof(hollow_cylinder_vertices), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	d3d12_copy_buffer(d3d12, world->hollow_cylinder_vertex_buffer, hollow_cylinder_vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	world->torus_vertex_buffer = d3d12_create_buffer(d3d12, sizeof(torus_vertices), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	d3d12_copy_buffer(d3d12, world->torus_vertex_buffer, torus_vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	{ // reference grid
		memory_arena_undo_alloc_scope_exit undo_frame_alloc(&world->frame_memory_arena);
		float size = 50;
		uint32 n = 50;
		float gap = size / n;
		uint32 vertex_count = (n + 1) * 2 * 2;
		vec3* vertices = memory_arena_alloc<vec3>(&world->frame_memory_arena, vertex_count);
		vec3* vertices_ptr = vertices;
		vec3 begin = { -size / 2, 0, -size / 2 };
		vec3 end = { size / 2, 0, -size / 2 };
		for (uint32 i = 0; i < n + 1; i += 1) {
			vertices_ptr[0] = begin;
			vertices_ptr[1] = end;
			vertices_ptr += 2;
			begin.z += gap;
			end.z += gap;
		}
		begin = { -size / 2, 0, -size / 2 };
		end = { -size / 2, 0, size / 2 };
		for (uint32 i = 0; i < n + 1; i += 1) {
			vertices_ptr[0] = begin;
			vertices_ptr[1] = end;
			vertices_ptr += 2;
			begin.x += gap;
			end.x += gap;
		}
		world->reference_grid_vertex_buffer = d3d12_create_buffer(d3d12, vertex_count * 12, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		d3d12_copy_buffer(d3d12, world->reference_grid_vertex_buffer, vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	}
	{
		world->default_diffuse_texture = d3d12_create_texture_2d(d3d12, 2, 2, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		world->default_normal_texture = d3d12_create_texture_2d(d3d12, 2, 2, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		world->default_roughness_texture = d3d12_create_texture_2d(d3d12, 2, 2, 1, 1, DXGI_FORMAT_R8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		world->default_metallic_texture = d3d12_create_texture_2d(d3d12, 2, 2, 1, 1, DXGI_FORMAT_R8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		world->default_emissive_texture = d3d12_create_texture_2d(d3d12, 2, 2, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		world->default_height_texture = d3d12_create_texture_2d(d3d12, 2, 2, 1, 1, DXGI_FORMAT_R8_UNORM, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);

		world->default_diffuse_texture->SetName(L"default_diffuse_texture");
		world->default_normal_texture->SetName(L"default_normal_texture");
		world->default_roughness_texture->SetName(L"default_roughness_texture");
		world->default_metallic_texture->SetName(L"default_metallic_texture");
		world->default_emissive_texture->SetName(L"default_emissive_texture");
		world->default_height_texture->SetName(L"default_height_texture");

		uint8 default_diffuse_texture_data[] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
		uint8 default_normal_texture_data[] = { 128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0, 128, 128, 255, 0 };
		uint8 default_roughness_texture_data[] = { 255, 255, 255, 255 };
		uint8 default_metallic_texture_data[] = { 255, 255, 255, 255 };
		float default_emissive_texture_data[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		uint8 default_height_texture_data[] = { 0, 0, 0, 0 };
		d3d12_copy_texture_2d(d3d12, world->default_diffuse_texture, default_diffuse_texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		d3d12_copy_texture_2d(d3d12, world->default_normal_texture, default_normal_texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		d3d12_copy_texture_2d(d3d12, world->default_roughness_texture, default_roughness_texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		d3d12_copy_texture_2d(d3d12, world->default_metallic_texture, default_metallic_texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		d3d12_copy_texture_2d(d3d12, world->default_emissive_texture, (uint8*)default_emissive_texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		d3d12_copy_texture_2d(d3d12, world->default_height_texture, default_height_texture_data, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		d3d12_append_srv_descriptor(d3d12, world->default_diffuse_texture, nullptr, &world->default_material_srv_descriptors);
		d3d12_append_srv_descriptor(d3d12, world->default_normal_texture, nullptr, nullptr);
		d3d12_append_srv_descriptor(d3d12, world->default_roughness_texture, nullptr, nullptr);
		d3d12_append_srv_descriptor(d3d12, world->default_metallic_texture, nullptr, nullptr);
		d3d12_append_srv_descriptor(d3d12, world->default_emissive_texture, nullptr, nullptr);
	}
	{
		static physx::PxDefaultAllocator allocator_callback;
		static physx::PxDefaultErrorCallback error_callback;

		world->px_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator_callback, error_callback);
		m_assert(world->px_foundation);

		bool record_mem_allocation = true;
		world->px_pvd = nullptr;
		world->px_pvd_transport = nullptr;
		// world->px_pvd = PxCreatePvd(*world->px_foundation);
		// world->px_pvd_transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		// world->px_pvd->connect(*world->px_pvd_transport, physx::PxPvdInstrumentationFlag::eALL);

		physx::PxTolerancesScale tolerance_scale = physx::PxTolerancesScale();
		tolerance_scale.length = 1;
		tolerance_scale.speed = 10;
		world->px_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *world->px_foundation, tolerance_scale, record_mem_allocation, world->px_pvd);
		m_assert(world->px_physics);

		physx::PxSceneDesc scene_desc = physx::PxSceneDesc(tolerance_scale);
		scene_desc.gravity = physx::PxVec3(0, -10, 0);
		scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;
		scene_desc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);

		world->px_scene = world->px_physics->createScene(scene_desc);
		m_assert(world->px_scene);

		world->px_controller_manager = PxCreateControllerManager(*world->px_scene);
		m_assert(world->px_controller_manager);
	}
	{
		world->static_objects = array<static_object>{new static_object[256], 0, 256};
		world->dynamic_objects = array<dynamic_object>{new dynamic_object[256], 0, 256};
		world->terrains = array<terrain>{new terrain[16], 0, 16};
		world->skyboxes = array<skybox>{new skybox[16], 0, 16};

		world->player = {};
		world->player.model_index = UINT32_MAX;
		world->player.transform = transform_identity();
		world->player.animation_index = UINT32_MAX;

		world->models = array<model>{new model[256], 0, 256};

		world->direct_lights = array<direct_light>{new direct_light[16], 0, 16};

		world->sphere_lights = array<sphere_light>{new sphere_light[16], 0, 16};
	}
}

bool world_add_model(world* world, d3d12* d3d12, const char* file_name, transform transform, collision collision) {
	for (auto& model : world->models) {
		if (!strcmp(model.file_name, file_name)) {
			return false;
		}
	}

	file_mapping model_file_mapping = {};
	if (!file_mapping_open(file_name, &model_file_mapping, true)) {
		return false;
	}
	auto close_model_file_mapping = scope_exit([&] { file_mapping_close(model_file_mapping); });

	gpk_model* gpk_model = (struct gpk_model*)model_file_mapping.ptr;
	if (strcmp(gpk_model->format_str, m_gpk_model_format_str)) {
		return false;
	}

	model* model = world->models.append({});

	snprintf(model->file_name, sizeof(model->file_name), "%s", file_name);
	model->transform = transform;
	model->collision = collision;
	model->px_geometry_holder = physx::PxGeometryHolder();
	if (collision.type == collision_type_sphere) {
		model->px_geometry_holder = physx::PxGeometryHolder(physx::PxSphereGeometry(collision.sphere.radius));
	}
	else if (collision.type == collision_type_box) {
		model->px_geometry_holder = physx::PxGeometryHolder(physx::PxBoxGeometry(m_unpack3(collision.box.extents * 0.5f)));
	}
	else if (collision.type == collision_type_capsule) {
		model->px_geometry_holder = physx::PxGeometryHolder(physx::PxCapsuleGeometry(collision.capsule.radius, collision.capsule.height * 0.5f));
	}
	model->scene_count = gpk_model->scene_count;
	model->node_count = gpk_model->node_count;
	model->mesh_count = gpk_model->mesh_count;
	model->mesh_node_count = 0;
	model->skin_count = gpk_model->skin_count;
	model->animation_count = gpk_model->animation_count;
	model->material_count = gpk_model->material_count;
	model->texture_count = gpk_model->image_count;
	model->scenes = new model_scene[model->scene_count]();
	model->nodes = new model_node[model->node_count]();
	model->meshes = new model_mesh[model->mesh_count]();
	if (model->skin_count > 0) {
		model->skins = new model_skin[model->skin_count]();
	}
	if (model->animation_count > 0) {
		model->animations = new model_animation[model->animation_count]();
	}
	if (model->material_count > 0) {
		model->materials = new model_material[model->material_count]();
	}
	if (model->texture_count > 0) {
		model->textures = new model_texture[model->texture_count]();
	}
	for (uint32 i = 0; i < model->scene_count; i += 1) {
		gpk_model_scene* gpk_model_scene = ((struct gpk_model_scene*)(model_file_mapping.ptr + gpk_model->scene_offset)) + i;
		model_scene* model_scene = &model->scenes[i];
		array_copy(model_scene->name, gpk_model_scene->name);
		array_copy(model_scene->node_indices, gpk_model_scene->node_indices);
		model_scene->node_index_count = gpk_model_scene->node_index_count;
	}
	for (uint32 i = 0; i < model->node_count; i += 1) {
		gpk_model_node* gpk_model_node = ((struct gpk_model_node*)(model_file_mapping.ptr + gpk_model->node_offset)) + i;
		model_node* model_node = &model->nodes[i];
		model_node->mesh_index = gpk_model_node->mesh_index;
		model_node->skin_index = gpk_model_node->skin_index;
		model_node->local_transform = gpk_model_node->local_transform;
		model_node->local_transform_mat = gpk_model_node->local_transform_mat;
		model_node->global_transform_mat = gpk_model_node->global_transform_mat;
		array_copy(model_node->children, gpk_model_node->children);
		model_node->child_count = gpk_model_node->child_count;
	}

	for (uint32 i = 0; i < model->mesh_count; i += 1) {
		gpk_model_mesh* gpk_model_mesh = ((struct gpk_model_mesh*)(model_file_mapping.ptr + gpk_model->mesh_offset)) + i;
		model_mesh* model_mesh = &model->meshes[i];
		array_copy(model_mesh->name, gpk_model_mesh->name);
		model_mesh->primitive_count = gpk_model_mesh->primitive_count;
		model_mesh->primitives = new model_mesh_primitive[model_mesh->primitive_count]();
		for (uint32 i = 0; i < model_mesh->primitive_count; i += 1) {
			gpk_model_mesh_primitive* gpk_primitive = ((gpk_model_mesh_primitive*)(model_file_mapping.ptr + gpk_model_mesh->primitive_offset)) + i;
			model_mesh_primitive* primitive = &model_mesh->primitives[i];

			primitive->vertex_count = gpk_primitive->vertex_count;
			primitive->index_count = gpk_primitive->index_count;
			primitive->material_index = gpk_primitive->material_index;

			m_assert(primitive->vertex_count > 0);
			primitive->vertex_buffer = d3d12_create_buffer(d3d12, primitive->vertex_count * sizeof(struct gpk_model_vertex), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
			d3d12_copy_buffer(d3d12, primitive->vertex_buffer, model_file_mapping.ptr + gpk_primitive->vertices_offset, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

			if (primitive->index_count > 0) {
				primitive->index_buffer = d3d12_create_buffer(d3d12, primitive->index_count * sizeof(uint16), D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
				d3d12_copy_buffer(d3d12, primitive->index_buffer, model_file_mapping.ptr + gpk_primitive->indices_offset, D3D12_RESOURCE_STATE_INDEX_BUFFER);
			}
		}
	}

	for (uint32 i = 0; i < model->skin_count; i += 1) {
		gpk_model_skin* gpk_model_skin = ((struct gpk_model_skin*)(model_file_mapping.ptr + gpk_model->skin_offset)) + i;
		model_skin* model_skin = &model->skins[i];
		array_copy(model_skin->name, gpk_model_skin->name);
		model_skin->joint_count = gpk_model_skin->joint_count;
		m_assert(model_skin->joint_count > 0);
		model_skin->joints = new model_joint[model_skin->joint_count]();
		gpk_model_joint* gpk_joints = (gpk_model_joint*)(model_file_mapping.ptr + gpk_model_skin->joints_offset);
		for (uint32 i = 0; i < model_skin->joint_count; i += 1) {
			model_skin->joints[i].node_index = gpk_joints[i].node_index;
			model_skin->joints[i].inverse_bind_mat = gpk_joints[i].inverse_bind_mat;
		}
	}
	for (uint32 i = 0; i < model->animation_count; i += 1) {
		gpk_model_animation* gpk_animation = ((struct gpk_model_animation*)(model_file_mapping.ptr + gpk_model->animation_offset)) + i;
		model_animation* animation = &model->animations[i];
		array_copy(animation->name, gpk_animation->name);
		animation->channel_count = gpk_animation->channel_count;
		animation->channels = new model_animation_channel[animation->channel_count]();
		for (uint32 i = 0; i < animation->channel_count; i += 1) {
			gpk_model_animation_channel* gpk_channel = ((struct gpk_model_animation_channel*)(model_file_mapping.ptr + gpk_animation->channel_offset)) + i;
			model_animation_channel* channel = &animation->channels[i];
			channel->node_index = gpk_channel->node_index;
			channel->channel_type = gpk_channel->channel_type;
			channel->sampler_index = gpk_channel->sampler_index;
		}
		animation->sampler_count = gpk_animation->sampler_count;
		animation->samplers = new model_animation_sampler[animation->sampler_count]();
		for (uint32 i = 0; i < animation->sampler_count; i += 1) {
			gpk_model_animation_sampler* gpk_sampler = ((struct gpk_model_animation_sampler*)(model_file_mapping.ptr + gpk_animation->sampler_offset)) + i;
			model_animation_sampler* sampler = &animation->samplers[i];
			sampler->interpolation_type = gpk_sampler->interpolation_type;
			sampler->key_frame_count = gpk_sampler->key_frame_count;
			sampler->key_frames = new model_animation_key_frame[sampler->key_frame_count]();
			for (uint32 i = 0; i < sampler->key_frame_count; i += 1) {
				gpk_model_animation_key_frame* gpk_key_frame = ((struct gpk_model_animation_key_frame*)(model_file_mapping.ptr + gpk_sampler->key_frame_offset)) + i;
				sampler->key_frames[i].time = gpk_key_frame->time;
				sampler->key_frames[i].transform_data = gpk_key_frame->transform_data;
			}
		}
	}
	for (uint32 i = 0; i < model->texture_count; i += 1) {
		gpk_model_image* gpk_model_image = ((struct gpk_model_image*)(model_file_mapping.ptr + gpk_model->image_offset)) + i;
		model->textures[i].texture = d3d12_create_texture_2d(d3d12, gpk_model_image->width, gpk_model_image->height, 1, gpk_model_image->mips, (DXGI_FORMAT)gpk_model_image->format, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
		d3d12_copy_texture_2d(d3d12, model->textures[i].texture, model_file_mapping.ptr + gpk_model_image->data_offset, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
	for (uint32 i = 0; i < model->material_count; i += 1) {
		gpk_model_material* gpk_model_material = ((struct gpk_model_material*)(model_file_mapping.ptr + gpk_model->material_offset)) + i;
		model_material* model_material = &model->materials[i];
		array_copy(model_material->name, gpk_model_material->name);
		model_material->diffuse_texture_index = gpk_model_material->diffuse_image_index;
		model_material->normal_texture_index = gpk_model_material->normal_image_index;
		model_material->roughness_texture_index = gpk_model_material->roughness_image_index;
		model_material->metallic_texture_index = gpk_model_material->metallic_image_index;
		model_material->emissive_texture_index = gpk_model_material->emissive_image_index;
		model_material->diffuse_factor = gpk_model_material->diffuse_factor;
		model_material->roughness_factor = gpk_model_material->roughness_factor;
		model_material->metallic_factor = gpk_model_material->metallic_factor;
		model_material->emissive_factor = gpk_model_material->emissive_factor;
		model_material->transparency = gpk_model_material->transparency;
		model_material->index_of_refraction = gpk_model_material->index_of_refraction;

		ID3D12Resource* diffuse_texture = model_material->diffuse_texture_index < model->texture_count ? model->textures[model_material->diffuse_texture_index].texture : world->default_diffuse_texture;
		ID3D12Resource* normal_texture = model_material->normal_texture_index < model->texture_count ? model->textures[model_material->normal_texture_index].texture : world->default_normal_texture;
		ID3D12Resource* roughness_texture = model_material->roughness_texture_index < model->texture_count ? model->textures[model_material->roughness_texture_index].texture : world->default_roughness_texture;
		ID3D12Resource* metallic_texture = model_material->metallic_texture_index < model->texture_count ? model->textures[model_material->metallic_texture_index].texture : world->default_metallic_texture;
		ID3D12Resource* emissive_texture = model_material->emissive_texture_index < model->texture_count ? model->textures[model_material->emissive_texture_index].texture : world->default_emissive_texture;
		d3d12_append_srv_descriptor(d3d12, diffuse_texture, nullptr, &model_material->texture_srv_descriptors);
		d3d12_append_srv_descriptor(d3d12, normal_texture, nullptr, nullptr);
		d3d12_append_srv_descriptor(d3d12, roughness_texture, nullptr, nullptr);
		d3d12_append_srv_descriptor(d3d12, metallic_texture, nullptr, nullptr);
		d3d12_append_srv_descriptor(d3d12, emissive_texture, nullptr, nullptr);
	}
	for (uint32 i = 0; i < model->node_count; i += 1) {
		if (model->nodes[i].mesh_index < model->mesh_count) {
			model->mesh_node_count += 1;
		}
	}
	return true;
}

bool world_add_terrain(world* world, d3d12* d3d12, const char* terrain_file) {
	const char* file_name = get_file_name(terrain_file);
	for (auto& terrain : world->terrains) {
		if (!strcmp(terrain.file, file_name)) {
			return false;
		}
	}

	file_mapping terrain_file_mapping = {};
	if (!file_mapping_open(terrain_file, &terrain_file_mapping, true)) {
		return false;
	}
	auto close_terrain_file_mapping = scope_exit([&] { file_mapping_close(terrain_file_mapping); });

	gpk_terrain* gpk_terrain = (struct gpk_terrain*)terrain_file_mapping.ptr;
	if (strcmp(gpk_terrain->format_str, m_gpk_terrain_format_str)) {
		return false;
	}

	terrain* terrain = world->terrains.append({});

	snprintf(terrain->file, sizeof(terrain->file), "%s", file_name);
	terrain->width = gpk_terrain->width;
	terrain->height = gpk_terrain->height;
	terrain->max_height = gpk_terrain->max_height;
	terrain->sample_per_meter = gpk_terrain->sample_per_meter;
	{ // height texture
		uint32 height_texture_width = terrain->width * terrain->sample_per_meter;
		uint32 height_texture_height = terrain->height * terrain->sample_per_meter;
		uint32 height_texture_size = height_texture_width * height_texture_height * sizeof(int16);
		int16* height_texture_data = (int16*)(terrain_file_mapping.ptr + gpk_terrain->height_map_offset);

		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = height_texture_width;
		texture_desc.Height = height_texture_height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R16_SNORM;
		texture_desc.SampleDesc = { 1, 0 };
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA subresource_data = { height_texture_data, height_texture_width * 2, 0 };
		//m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &subresource_data, &terrain->height_texture));
		//m_d3d_assert(d3d->device->CreateShaderResourceView(terrain->height_texture, nullptr, &terrain->height_texture_view));

		terrain->height_texture_data = new int16[height_texture_width * height_texture_height]();
		memcpy(terrain->height_texture_data, height_texture_data, height_texture_size);
	}
	{ // diffuse texture
		uint32 diffuse_texture_width = terrain->width * terrain->sample_per_meter;
		uint32 diffuse_texture_height = terrain->height * terrain->sample_per_meter;
		uint32 diffuse_texture_size = diffuse_texture_width * diffuse_texture_height * 4;
		uint32* diffuse_texture_data = (uint32*)(terrain_file_mapping.ptr + gpk_terrain->diffuse_map_offset);

		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = diffuse_texture_width;
		texture_desc.Height = diffuse_texture_height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		texture_desc.SampleDesc = { 1, 0 };
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA subresource_data = { diffuse_texture_data, diffuse_texture_width * 2, 0 };
		//m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, &subresource_data, &terrain->diffuse_texture));
		//m_d3d_assert(d3d->device->CreateShaderResourceView(terrain->diffuse_texture, nullptr, &terrain->diffuse_texture_view));

		terrain->diffuse_texture_uv_repeat = 1.0f;

		terrain->diffuse_texture_data = new uint32[diffuse_texture_width * diffuse_texture_height]();
		memcpy(terrain->diffuse_texture_data, diffuse_texture_data, diffuse_texture_size);
	}
	{ // vertices
		vec2 position = { terrain->width * -0.5f, terrain->height * -0.5f };
		vec2 uv = { 0, 0 };
		uint32 vertex_x_count = terrain->width * terrain->sample_per_meter + 1;
		uint32 vertex_y_count = terrain->height * terrain->sample_per_meter + 1;
		uint32 vertex_count = vertex_x_count * vertex_y_count;
		uint32 index_count = (vertex_x_count + 1) * 2 * (vertex_y_count - 1);
		float dpos = 1.0f / terrain->sample_per_meter;
		float duv_x = 1.0f / (vertex_x_count - 1);
		float duv_y = 1.0f / (vertex_y_count - 1);
		{
			terrain_vertex* vertices = new terrain_vertex[vertex_count]();
			auto delete_vertices = scope_exit([&] { delete[]vertices; });
			for (uint32 i = 0; i < vertex_y_count; i += 1) {
				for (uint32 j = 0; j < vertex_x_count; j += 1) {
					vertices[vertex_x_count * i + j] = { position, uv };
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
			D3D11_SUBRESOURCE_DATA vertex_buffer_data = { vertices, 0, 0 };
			//m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, &vertex_buffer_data, &terrain->vertex_buffer));
		}
		{
			uint32* indices = new uint32[index_count]();
			auto delete_indices = scope_exit([&] { delete[]indices; });
			uint32* index = indices;
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
			D3D11_SUBRESOURCE_DATA vertex_buffer_data = { indices, 0, 0 };
			//m_d3d_assert(d3d->device->CreateBuffer(&buffer_desc, &vertex_buffer_data, &terrain->index_buffer));
			terrain->index_count = index_count;
		}
	}
	return true;
}

bool world_add_skybox(world* world, d3d12* d3d12, const char* skybox_file) {
	const char* file_name = get_file_name(skybox_file);
	for (auto& skybox : world->skyboxes) {
		if (!strcmp(skybox.file, file_name)) {
			return false;
		}
	}

	file_mapping skybox_file_mapping = {};
	if (!file_mapping_open(skybox_file, &skybox_file_mapping, true)) {
		return false;
	}
	auto close_skybyx_file_mapping = scope_exit([&] { file_mapping_close(skybox_file_mapping); });

	gpk_skybox* gpk_skybox = (struct gpk_skybox*)skybox_file_mapping.ptr;
	if (strcmp(gpk_skybox->format_str, m_gpk_skybox_format_str)) {
		return false;
	}

	skybox* skybox = world->skyboxes.append({});
	snprintf(skybox->file, sizeof(skybox->file), "%s", file_name);

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = gpk_skybox->cubemap_width;
	texture_desc.Height = gpk_skybox->cubemap_height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 6;
	texture_desc.Format = (DXGI_FORMAT)gpk_skybox->cubemap_format;
	texture_desc.SampleDesc = { 1, 0 };
	texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	D3D11_SUBRESOURCE_DATA subresource_data[6] = {};
	uint8* cubemap_data = (uint8*)gpk_skybox + gpk_skybox->cubemap_offset;
	uint32 cubemap_face_size = gpk_skybox->cubemap_size / 6;
	for (uint32 i = 0; i < 6; i += 1) {
		subresource_data[i] = { cubemap_data + i * cubemap_face_size, gpk_skybox->cubemap_width / gpk_skybox->cubemap_format_block_dimension * gpk_skybox->cubemap_format_block_size, 0 };
	}
	//m_d3d_assert(d3d->device->CreateTexture2D(&texture_desc, subresource_data, &skybox->cube_texture));
	//m_d3d_assert(d3d->device->CreateShaderResourceView(skybox->cube_texture, nullptr, &skybox->cube_texture_view));

	return true;
}

void world_build_dxr_acceleration_buffers(world* world, d3d12* d3d12) {
	for (auto& buffer : d3d12->dxr_bottom_acceleration_buffers) buffer->Release();
	if (d3d12->dxr_top_acceleration_buffer) d3d12->dxr_top_acceleration_buffer->Release();

	m_d3d_assert(d3d12->command_allocator->Reset());
	m_d3d_assert(d3d12->command_list->Reset(d3d12->command_allocator, nullptr));

	array<array<D3D12_RAYTRACING_GEOMETRY_DESC>> model_geom_descs{ new array<D3D12_RAYTRACING_GEOMETRY_DESC>[world->models.size], 0, world->models.size };
	auto delete_descs = scope_exit([&]
		{
			for (auto descs : model_geom_descs) {
				delete[] descs.elems;
			}
			delete[] model_geom_descs.elems;
		}
	);

	uint32 node_transform_mats_offset = 0;
	uint32 node_transform_mats_capacity = m_megabytes(16);
	ID3D12Resource* node_transform_mats = d3d12_create_buffer(d3d12, node_transform_mats_capacity, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);
	D3D12_GPU_VIRTUAL_ADDRESS node_transform_mats_gpu_virtual_adress = node_transform_mats->GetGPUVirtualAddress();

	uint8* node_transform_mats_ptr = nullptr;
	node_transform_mats->Map(0, nullptr, (void**)&node_transform_mats_ptr);
	for (uint32 i = 0; i < world->models.size; i += 1) {
		model* model = &world->models[i];
		auto* geom_descs = model_geom_descs.append(array<D3D12_RAYTRACING_GEOMETRY_DESC>{new D3D12_RAYTRACING_GEOMETRY_DESC[32], 0, 32});
		for (uint32 i = 0; i < model->scene_count; i += 1) {
			model_scene* scene = &model->scenes[i];
			for (uint32 i = 0; i < scene->node_index_count; i += 1) {
				std::stack<model_node*> node_stack;
				node_stack.push(&model->nodes[scene->node_indices[i]]);
				while (!node_stack.empty()) {
					model_node* node = node_stack.top();
					node_stack.pop();
					for (uint32 i = 0; i < node->child_count; i += 1) {
						model_node* child_node = &model->nodes[node->children[i]];
						node_stack.push(child_node);
					}
					if (node->mesh_index < model->mesh_count) {
						mat4 transform_mat = mat4_transpose(node->global_transform_mat);
						memcpy(node_transform_mats_ptr + node_transform_mats_offset, &transform_mat, sizeof(float[3][4]));
						model_mesh* mesh = &model->meshes[node->mesh_index];
						for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
							model_mesh_primitive* primitive = &mesh->primitives[i];
							D3D12_RAYTRACING_GEOMETRY_DESC geom_desc = {};
							geom_desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
							geom_desc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
							if (primitive->material_index < model->material_count) {
								if (model->materials[primitive->material_index].transparency > 0.5f) {
									geom_desc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_NONE;
								}
							}
							geom_desc.Triangles.Transform3x4 = node_transform_mats_gpu_virtual_adress + node_transform_mats_offset;
							geom_desc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
							geom_desc.Triangles.VertexCount = primitive->vertex_count;
							geom_desc.Triangles.VertexBuffer = { primitive->vertex_buffer->GetGPUVirtualAddress(), sizeof(gpk_model_vertex) };
							if (primitive->index_count > 0) {
								geom_desc.Triangles.IndexFormat = DXGI_FORMAT_R16_UINT;
								geom_desc.Triangles.IndexCount = primitive->index_count;
								geom_desc.Triangles.IndexBuffer = primitive->index_buffer->GetGPUVirtualAddress();
							}
							else {
								geom_desc.Triangles.IndexFormat = DXGI_FORMAT_UNKNOWN;
								geom_desc.Triangles.IndexCount = 0;
								geom_desc.Triangles.IndexBuffer = 0;
							}
							geom_descs->append(geom_desc);
						}
						node_transform_mats_offset += sizeof(float[3][4]);
						m_assert(node_transform_mats_offset < node_transform_mats_capacity);
					}
				}
			}
		}
	}
	node_transform_mats->Unmap(0, nullptr);

	auto bottom_acceleration_scratch_buffers = array<ID3D12Resource*>{new ID3D12Resource*[world->models.size], world->models.size, world->models.size};
	auto bottom_acceleration_buffers = array<ID3D12Resource*>{new ID3D12Resource*[world->models.size], world->models.size, world->models.size};
	auto delete_buffers = scope_exit([&] { delete[] bottom_acceleration_scratch_buffers.elems; });
	for (uint32 i = 0; i < world->models.size; i += 1) {
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
		inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
		inputs.Flags =
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION |
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
		inputs.NumDescs = (uint32)model_geom_descs[i].size;
		inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
		inputs.pGeometryDescs = model_geom_descs[i].elems;

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info = {};
		d3d12->device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

		bottom_acceleration_scratch_buffers[i] = d3d12_create_buffer(d3d12, info.ScratchDataSizeInBytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		bottom_acceleration_buffers[i] = d3d12_create_buffer(d3d12, info.ResultDataMaxSizeInBytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC as_desc = {};
		as_desc.Inputs = inputs;
		as_desc.DestAccelerationStructureData = bottom_acceleration_buffers[i]->GetGPUVirtualAddress();
		as_desc.ScratchAccelerationStructureData = bottom_acceleration_scratch_buffers[i]->GetGPUVirtualAddress();

		d3d12->command_list->BuildRaytracingAccelerationStructure(&as_desc, 0, nullptr);

		D3D12_RESOURCE_BARRIER uav_barrier = {};
		uav_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		uav_barrier.UAV.pResource = bottom_acceleration_buffers[i];
		d3d12->command_list->ResourceBarrier(1, &uav_barrier);
	}

	ID3D12Resource* model_instance_descs_buffer = nullptr;
	ID3D12Resource* top_acceleration_scratch_buffer = nullptr;
	ID3D12Resource* top_acceleration_buffer = nullptr;
	if (world->models.size > 0) {
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
		inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
		inputs.NumDescs = (uint32)world->models.size;
		inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info = {};
		d3d12->device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

		top_acceleration_scratch_buffer = d3d12_create_buffer(d3d12, info.ScratchDataSizeInBytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		top_acceleration_buffer = d3d12_create_buffer(d3d12, info.ResultDataMaxSizeInBytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);
		model_instance_descs_buffer = d3d12_create_buffer(d3d12, world->models.size * sizeof(D3D12_RAYTRACING_INSTANCE_DESC), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ);

		D3D12_RAYTRACING_INSTANCE_DESC* model_instance_descs = nullptr;
		model_instance_descs_buffer->Map(0, nullptr, (void**)&model_instance_descs);
		mat4 model_mat = mat4_identity();
		for (uint32 i = 0; i < world->models.size; i += 1) {
			memcpy(model_instance_descs[i].Transform, &model_mat, sizeof(float[3][4]));
			model_instance_descs[i].InstanceID = i;
			model_instance_descs[i].InstanceMask = 0xff;
			model_instance_descs[i].AccelerationStructure = bottom_acceleration_buffers[i]->GetGPUVirtualAddress();
		}
		model_instance_descs_buffer->Unmap(0, nullptr);

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC as_desc = {};
		as_desc.Inputs = inputs;
		as_desc.Inputs.InstanceDescs = model_instance_descs_buffer->GetGPUVirtualAddress();
		as_desc.DestAccelerationStructureData = top_acceleration_buffer->GetGPUVirtualAddress();
		as_desc.ScratchAccelerationStructureData = top_acceleration_scratch_buffer->GetGPUVirtualAddress();

		d3d12->command_list->BuildRaytracingAccelerationStructure(&as_desc, 0, nullptr);

		D3D12_RESOURCE_BARRIER uav_barrier = {};
		uav_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		uav_barrier.UAV.pResource = top_acceleration_buffer;
		d3d12->command_list->ResourceBarrier(1, &uav_barrier);
	}

	d3d12->command_list->Close();
	d3d12->command_queue->ExecuteCommandLists(1, (ID3D12CommandList**)&d3d12->command_list);
	d3d12_wait_command_list(d3d12);

	for (auto b : bottom_acceleration_scratch_buffers) b->Release();
	if (top_acceleration_scratch_buffer) top_acceleration_scratch_buffer->Release();
	if (model_instance_descs_buffer) model_instance_descs_buffer->Release();
	if (node_transform_mats) node_transform_mats->Release();

	d3d12->dxr_bottom_acceleration_buffers = bottom_acceleration_buffers;
	d3d12->dxr_top_acceleration_buffer = top_acceleration_buffer;

	if (d3d12->dxr_top_acceleration_buffer) {
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.RaytracingAccelerationStructure.Location = d3d12->dxr_top_acceleration_buffer->GetGPUVirtualAddress();
		D3D12_CPU_DESCRIPTOR_HANDLE dxr_cpu_descriptor_handle = d3d12->dxr_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
		uint32 dxr_cpu_descriptor_handle_size = d3d12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		d3d12->device->CreateShaderResourceView(nullptr, &srv_desc, { dxr_cpu_descriptor_handle.ptr + dxr_cpu_descriptor_handle_size * 0 });
	}
}

void world_render_commands(world* world, d3d12* d3d12, world_render_params* params) {
	d3d12->frame_constants_buffer_size = 0;
	uint8* frame_constants_buffer = nullptr;
	d3d12->frame_constants_buffer->Map(0, nullptr, (void**)&frame_constants_buffer);
	auto unmap_frame_constants_buffer = scope_exit([&] { d3d12->frame_constants_buffer->Unmap(0, nullptr); });
	D3D12_GPU_VIRTUAL_ADDRESS frame_constants_buffer_gpu_address = d3d12->frame_constants_buffer->GetGPUVirtualAddress();
	auto frame_constants_buffer_append = [&](void* data, uint32 data_size, uint32* offset) {
		if (offset) {
			*offset = d3d12->frame_constants_buffer_size;
		}
		memcpy(frame_constants_buffer + d3d12->frame_constants_buffer_size, data, data_size);
		d3d12->frame_constants_buffer_size += round_up(data_size, 256u);
	};

	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
	d3d12->swap_chain->GetDesc(&swap_chain_desc);
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f, (float)swap_chain_desc.BufferDesc.Width , (float)swap_chain_desc.BufferDesc.Height, 0.0f, 1.0f };
	RECT scissor = { 0, 0, (LONG)swap_chain_desc.BufferDesc.Width, (LONG)swap_chain_desc.BufferDesc.Height };
	d3d12->command_list->RSSetViewports(1, &viewport);
	d3d12->command_list->RSSetScissorRects(1, &scissor);

	d3d12->command_list->ClearDepthStencilView(d3d12->depth_texture_dsv_descriptor, D3D12_CLEAR_FLAG_DEPTH, 0, 0, 0, nullptr);

	d3d12->command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d12->command_list->SetDescriptorHeaps(1, &d3d12->cbv_srv_uav_descriptor_heap);
	{
		D3D12_RESOURCE_BARRIER barriers[m_countof(d3d12->gbuffer_textures)] = {};
		for (uint32 i = 0; i < m_countof(barriers); i += 1) {
			barriers[i].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barriers[i].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barriers[i].Transition.pResource = d3d12->gbuffer_textures[i];
			barriers[i].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			barriers[i].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		}
		d3d12->command_list->ResourceBarrier(m_countof(barriers), barriers);

		d3d12->command_list->SetPipelineState(d3d12->gbuffer_pipeline_state);
		d3d12->command_list->OMSetRenderTargets(m_countof(d3d12->gbuffer_textures), d3d12->gbuffer_texture_rtv_descriptors, false, &d3d12->depth_texture_dsv_descriptor);
		d3d12->command_list->SetGraphicsRootSignature(d3d12->gbuffer_root_signature);

		float clear_color[4] = { 0, 0, 0, 0 };
		for (auto& gbuffer : d3d12->gbuffer_texture_rtv_descriptors) {
			d3d12->command_list->ClearRenderTargetView(gbuffer, clear_color, 0, nullptr);
		}

		struct world_constants {
			XMMATRIX camera_view_proj_mat;
		} world_constants = {
			params->camera_view_project_mat,
		};
		uint32 world_constants_offset = 0;
		frame_constants_buffer_append(&world_constants, sizeof(world_constants), &world_constants_offset);

		struct material_constants {
			vec4 diffuse_factor;
			vec4 emissive_factor;
			float metallic_factor;
			float roughness_factor;
		};
		uint32 material_constants_increment_size = round_up((uint32)sizeof(struct material_constants), 256u);
		material_constants default_material_constants = { {1, 1, 1, 1}, {0, 0, 0, 0}, 1, 1 };
		uint32 default_material_constants_offset = 0;
		frame_constants_buffer_append(&default_material_constants, sizeof(default_material_constants), &default_material_constants_offset);

		for (uint32 i = 0; i < world->models.size; i += 1) {
			model* model = &world->models[i];
			mat4 model_mat = mat4_identity();
			uint32 model_mat_offset = 0;
			frame_constants_buffer_append(&model_mat, sizeof(model_mat), &model_mat_offset);

			uint32 first_material_constants_offset = d3d12->frame_constants_buffer_size;
			for (uint32 i = 0; i < model->material_count; i += 1) {
				model_material* material = &model->materials[i];
				material_constants material_constants = {
					material->diffuse_factor,
					{m_unpack3(material->emissive_factor), 0},
					material->metallic_factor,
					material->roughness_factor
				};
				frame_constants_buffer_append(&material_constants, sizeof(material_constants), nullptr);
			}
			for (uint32 i = 0; i < model->scene_count; i += 1) {
				model_scene* scene = &model->scenes[i];
				for (uint32 i = 0; i < scene->node_index_count; i += 1) {
					std::stack<model_node*> node_stack;
					node_stack.push(&model->nodes[scene->node_indices[i]]);
					while (!node_stack.empty()) {
						model_node* node = node_stack.top();
						node_stack.pop();
						for (uint32 i = 0; i < node->child_count; i += 1) {
							model_node* child_node = &model->nodes[node->children[i]];
							node_stack.push(child_node);
						}
						if (node->mesh_index < model->mesh_count) {
							uint32 node_mat_offset = 0;
							frame_constants_buffer_append(&node->global_transform_mat, sizeof(node->global_transform_mat), &node_mat_offset);
							model_mesh* mesh = &model->meshes[node->mesh_index];
							for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
								model_mesh_primitive* primitive = &mesh->primitives[i];
								uint32 primitive_constants_offset = default_material_constants_offset;
								D3D12_GPU_DESCRIPTOR_HANDLE material_gpu_descriptor_handle = world->default_material_srv_descriptors;
								if (primitive->material_index < model->material_count) {
									primitive_constants_offset = first_material_constants_offset + primitive->material_index * material_constants_increment_size;
									material_gpu_descriptor_handle = model->materials[primitive->material_index].texture_srv_descriptors;
								}

								d3d12->command_list->SetGraphicsRootConstantBufferView(0, frame_constants_buffer_gpu_address + world_constants_offset);
								d3d12->command_list->SetGraphicsRootConstantBufferView(1, frame_constants_buffer_gpu_address + model_mat_offset);
								d3d12->command_list->SetGraphicsRootConstantBufferView(2, frame_constants_buffer_gpu_address + node_mat_offset);
								d3d12->command_list->SetGraphicsRootConstantBufferView(3, frame_constants_buffer_gpu_address + primitive_constants_offset);
								d3d12->command_list->SetGraphicsRootDescriptorTable(4, material_gpu_descriptor_handle);

								D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view = {};
								vertex_buffer_view.BufferLocation = primitive->vertex_buffer->GetGPUVirtualAddress();
								vertex_buffer_view.SizeInBytes = primitive->vertex_count * sizeof(gpk_model_vertex);
								vertex_buffer_view.StrideInBytes = sizeof(gpk_model_vertex);
								d3d12->command_list->IASetVertexBuffers(0, 1, &vertex_buffer_view);

								if (primitive->index_count == 0) {
									d3d12->command_list->DrawInstanced(primitive->vertex_count, 1, 0, 0);
								}
								else {
									D3D12_INDEX_BUFFER_VIEW index_buffer_view = {};
									index_buffer_view.BufferLocation = primitive->index_buffer->GetGPUVirtualAddress();
									index_buffer_view.SizeInBytes = primitive->index_count * sizeof(uint16);
									index_buffer_view.Format = DXGI_FORMAT_R16_UINT;
									d3d12->command_list->IASetIndexBuffer(&index_buffer_view);
									d3d12->command_list->DrawIndexedInstanced(primitive->index_count, 1, 0, 0, 0);
								}
							}
						}
					}
				}
			}
		}

		for (uint32 i = 0; i < m_countof(barriers); i += 1) {
			barriers[i].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barriers[i].Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;;
		}
		d3d12->command_list->ResourceBarrier(m_countof(barriers), barriers);
	}
	const uint32 direct_light_type = 0;
	const uint32 sphere_light_type = 1;
	struct light {
		uint32 type;
		float radius;
		float padding0;
		float padding1;
		vec4 color;
		vec4 position;
		vec4 dir;
	};
	struct lights_info {
		light lights[4];
		uint32 light_count;
	} lights_info = {};
	for (auto& light : world->sphere_lights) {
		lights_info.lights[lights_info.light_count].type = sphere_light_type;
		lights_info.lights[lights_info.light_count].color = { 1, 1, 1, 0 };
		lights_info.lights[lights_info.light_count].position = { m_unpack3(light.position), 0 };
		lights_info.light_count += 1;
		if (lights_info.light_count == m_countof(lights_info.lights)) {
			break;
		}
	}
	if (d3d12->dxr_enabled && d3d12->dxr_top_acceleration_buffer) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = d3d12->dxr_shadow_output_texture_array;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		d3d12->command_list->ResourceBarrier(1, &barrier);

		d3d12->command_list->SetPipelineState1(d3d12->dxr_shadow_pipeline_state);
		d3d12->command_list->SetDescriptorHeaps(1, &d3d12->dxr_descriptor_heap);
		d3d12->command_list->SetComputeRootSignature(d3d12->dxr_shadow_global_root_sig);

		uint32 constants_offset = 0;
		frame_constants_buffer_append(&lights_info, sizeof(lights_info), &constants_offset);
		d3d12->command_list->SetComputeRootConstantBufferView(0, frame_constants_buffer_gpu_address + constants_offset);

		D3D12_DISPATCH_RAYS_DESC rays_desc = {};
		DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
		m_d3d_assert(d3d12->swap_chain->GetDesc(&swap_chain_desc));
		rays_desc.Width = swap_chain_desc.BufferDesc.Width;
		rays_desc.Height = swap_chain_desc.BufferDesc.Height;
		rays_desc.Depth = 1;
		rays_desc.RayGenerationShaderRecord.StartAddress = d3d12->dxr_shadow_shader_table->GetGPUVirtualAddress();
		rays_desc.RayGenerationShaderRecord.SizeInBytes = d3d12->dxr_shadow_shader_table_entry_size;
		rays_desc.MissShaderTable.StartAddress = d3d12->dxr_shadow_shader_table->GetGPUVirtualAddress() + d3d12->dxr_shadow_shader_table_entry_size;
		rays_desc.MissShaderTable.StrideInBytes = d3d12->dxr_shadow_shader_table_entry_size;
		rays_desc.MissShaderTable.SizeInBytes = d3d12->dxr_shadow_shader_table_entry_size;
		rays_desc.HitGroupTable.StartAddress = d3d12->dxr_shadow_shader_table->GetGPUVirtualAddress() + 2 * d3d12->dxr_shadow_shader_table_entry_size;
		rays_desc.HitGroupTable.StrideInBytes = d3d12->dxr_shadow_shader_table_entry_size;
		rays_desc.HitGroupTable.SizeInBytes = d3d12->dxr_shadow_shader_table_entry_size;

		d3d12->command_list->DispatchRays(&rays_desc);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		d3d12->command_list->ResourceBarrier(1, &barrier);
	}
	if (d3d12->dxr_enabled && d3d12->dxr_top_acceleration_buffer) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = d3d12->dxr_ao_output_texture;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		d3d12->command_list->ResourceBarrier(1, &barrier);

		d3d12->command_list->SetPipelineState1(d3d12->dxr_ao_pipeline_state);
		d3d12->command_list->SetDescriptorHeaps(1, &d3d12->dxr_descriptor_heap);
		d3d12->command_list->SetComputeRootSignature(d3d12->dxr_ao_global_root_sig);

		D3D12_DISPATCH_RAYS_DESC rays_desc = {};
		DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
		m_d3d_assert(d3d12->swap_chain->GetDesc(&swap_chain_desc));
		rays_desc.Width = swap_chain_desc.BufferDesc.Width;
		rays_desc.Height = swap_chain_desc.BufferDesc.Height;
		rays_desc.Depth = 1;
		rays_desc.RayGenerationShaderRecord.StartAddress = d3d12->dxr_ao_shader_table->GetGPUVirtualAddress();
		rays_desc.RayGenerationShaderRecord.SizeInBytes = d3d12->dxr_ao_shader_table_entry_size;
		rays_desc.MissShaderTable.StartAddress = d3d12->dxr_ao_shader_table->GetGPUVirtualAddress() + d3d12->dxr_ao_shader_table_entry_size;
		rays_desc.MissShaderTable.StrideInBytes = d3d12->dxr_ao_shader_table_entry_size;
		rays_desc.MissShaderTable.SizeInBytes = d3d12->dxr_ao_shader_table_entry_size;
		rays_desc.HitGroupTable.StartAddress = d3d12->dxr_ao_shader_table->GetGPUVirtualAddress() + 2 * d3d12->dxr_ao_shader_table_entry_size;
		rays_desc.HitGroupTable.StrideInBytes = d3d12->dxr_ao_shader_table_entry_size;
		rays_desc.HitGroupTable.SizeInBytes = d3d12->dxr_ao_shader_table_entry_size;

		d3d12->command_list->DispatchRays(&rays_desc);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		d3d12->command_list->ResourceBarrier(1, &barrier);
	}
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.pResource = d3d12->render_texture;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3d12->command_list->ResourceBarrier(1, &barrier);

		d3d12->command_list->SetPipelineState(d3d12->gbuffer_direct_lit_pipeline_state);
		d3d12->command_list->OMSetRenderTargets(1, &d3d12->render_texture_rtv_descriptor, false, nullptr);
		d3d12->command_list->SetGraphicsRootSignature(d3d12->gbuffer_direct_lit_root_signature);
		d3d12->command_list->SetDescriptorHeaps(1, &d3d12->cbv_srv_uav_descriptor_heap);

		if (d3d12->dxr_enabled) {
			//d3d12->device->CreateShaderResourceView(d3d12->dxr_shadow_output_texture_array, nullptr, { render_target_cpu_descriptor_handle.ptr + m_countof(d3d12->gbuffer_textures) * frame_descriptor_handle_size });
			//d3d12->cbv_srv_uav_descriptor_count += 1;
		}

		struct constants {
			XMVECTOR camera_position;
			struct lights_info lights_info;
		} constants = {
			params->camera_position,
			lights_info
		};
		uint32 constants_offset = 0;
		frame_constants_buffer_append(&constants, sizeof(constants), &constants_offset);
		d3d12->command_list->SetGraphicsRootConstantBufferView(0, frame_constants_buffer_gpu_address + constants_offset);
		d3d12->command_list->SetGraphicsRootDescriptorTable(1, d3d12->render_texture_srv_descriptor);

		d3d12->command_list->DrawInstanced(3, 1, 0, 0);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		d3d12->command_list->ResourceBarrier(1, &barrier);
	}
}

bool world_load_from_file(world* world, d3d12* d3d12, const char* file_name) {
	file_tokenizer ft;
	if (!file_tokenizer_init(&ft, file_name)) {
		return false;
	}
	auto delete_ft = scope_exit([&] { file_tokenizer_delete(ft); });

	token object;
	while (ft.get_token(&object)) {
		if (!strncmp(object.ptr, "directional_light:", object.len)) {
			token x, y, z, r, g, b;
			if (!ft.get_token(&x) || !ft.get_token(&y) || !ft.get_token(&z) ||
				!ft.get_token(&r) || !ft.get_token(&g) || !ft.get_token(&b)) {
				return false;
			}
			float fx, fy, fz, fr, fg, fb;
			if (!x.to_float(&fx) || !y.to_float(&fy) || !z.to_float(&fz) ||
				!r.to_float(&fr) || !g.to_float(&fg) || !b.to_float(&fb)) {
				return false;
			}
			printf("directional_light: %f %f %f %f %f %f\n", fx, fy, fz, fr, fg, fb);
			direct_light l;
			l.position = { 0, 0, 0 };
			l.dir = vec3_normalize({ fx, fy, fz });
			l.color = { fr, fg, fb };
			world->direct_lights.append(l);
		}
		else if (!strncmp(object.ptr, "spherical_light:", object.len)) {
			token x, y, z, r, g, b, radius;
			if (!ft.get_token(&x) || !ft.get_token(&y) || !ft.get_token(&z) ||
				!ft.get_token(&r) || !ft.get_token(&g) || !ft.get_token(&b) ||
				!ft.get_token(&radius)) {
				return false;
			}
			float fx, fy, fz, fr, fg, fb, fradius;
			if (!x.to_float(&fx) || !y.to_float(&fy) || !z.to_float(&fz) ||
				!r.to_float(&fr) || !g.to_float(&fg) || !b.to_float(&fb) ||
				!radius.to_float(&fradius)) {
				return false;
			}
			printf("spherical_light: %f %f %f %f %f %f %f\n", fx, fy, fz, fr, fg, fb, fradius);
			sphere_light l;
			l.position = { fx, fy, fz };
			l.color = { fr, fg, fb };
			l.radius = fradius;
			world->sphere_lights.append(l);
		}
		else if (!strncmp(object.ptr, "model:", object.len)) {
			token file;
			if (!ft.get_token(&file)) {
				return false;
			}
			printf("model: %.*s\n", file.len, file.ptr);
			char c = file.ptr[file.len];
			file.ptr[file.len] = '\0';
			auto restore_char = scope_exit([&] { file.ptr[file.len] = c; });
			if (!world_add_model(world, d3d12, file.ptr, transform_identity(), collision{ collision_type_none })) {
				return false;
			}
		}
	}

	if (d3d12->dxr_enabled) {
		world_build_dxr_acceleration_buffers(world, d3d12);
	}

	return true;
}

bool world_save_to_file(world* world, const char* file_name) {
	string file_data = { new char[512], 0, 512 };
	auto delete_file_data = scope_exit([&] { delete[] file_data.ptr; });

	char buffer[1024] = {};

	for (auto& l : world->direct_lights) {
		int len = snprintf(buffer, sizeof(buffer), "directional_light: %f %f %f %f %f %f\n", l.dir[0], l.dir[1], l.dir[2], l.color[0], l.color[1], l.color[2]);
		if (len < 0 || len >= sizeof(buffer)) {
			return false;
		}
		file_data.append(buffer, len);
	}
	for (auto& l : world->sphere_lights) {
		int len = snprintf(buffer, sizeof(buffer), "spherical_light: %f %f %f %f %f %f %f\n", l.position[0], l.position[1], l.position[2], l.color[0], l.color[1], l.color[2], l.radius);
		if (len < 0 || len >= sizeof(buffer)) {
			return false;
		}
		file_data.append(buffer, len);
	}
	for (auto& m : world->models) {
		int len = snprintf(buffer, sizeof(buffer), "model: %s\n", m.file_name);
		if (len < 0 || len >= sizeof(buffer)) {
			return false;
		}
		file_data.append(buffer, len);
	}

	FILE *file = fopen(file_name, "w");
	if (!file) {
		return false;
	}
	auto close_file = scope_exit([&] { fclose(file); });

	size_t n = fwrite(file_data.ptr, 1, file_data.len, file);
	if (n < file_data.len) {
		return false;
	}

	return true;
}

//uint32 world_append_constant_buffer(world *world, void *data, uint32 data_size) {
//	round_up(&world->constant_buffer_offset, 256u);
//	uint32 offset = world->constant_buffer_offset;
//	memcpy(world->constant_buffer_ptr + world->constant_buffer_offset, data, data_size);
//	world->constant_buffer_offset += data_size;
//	m_debug_assert(world->constant_buffer_offset <= world->constant_buffer_capacity);
//	return offset;
//}

//void world_add_model_render_data(world *world, d3d *d3d, uint32 model_index, transform transform, uint32 animation_index, double animation_time, bool render_collision_shapes) {
//	model *model = &world->models[model_index];
//
//	struct {
//		XMMATRIX model_mat;
//	} model_constant_buffer = {
//		XMMatrixMultiply(xmmatrix_transform(&model->transform), xmmatrix_transform(&transform))
//	};
//
//	model_render_data *model_render_data = memory_arena_alloc<struct model_render_data>(&world->frame_memory_arena, 1);
//	model_render_data->model = model;
//	model_render_data->model_mat_constant_buffer_offset = world_append_constant_buffer(world, &model_constant_buffer, sizeof(model_constant_buffer));
//	model_render_data->meshes = memory_arena_alloc<struct mesh_render_data>(&world->frame_memory_arena, model->mesh_node_count);
//	model_render_data->mesh_count = model->mesh_node_count;
//	model_render_data->render_collision_shapes = render_collision_shapes;
//	model_render_data->next = world->render_data.model_list;
//
//	world->render_data.model_list = model_render_data;
//	world->render_data.model_count += 1;
//
//	uint32 *joint_mats_offsets = memory_arena_alloc<uint32>(&world->frame_memory_arena, model->skin_count);
//	{
//		memory_arena_undo_alloc_scope_exit(&world->frame_memory_arena);
//
//		model_node *model_nodes = model->nodes;
//		if (animation_index < model->animation_count) {
//			model_animation *animation = &model->animations[animation_index];
//
//			model_nodes = memory_arena_alloc<struct model_node>(&world->frame_memory_arena, model->node_count);
//			memcpy(model_nodes, model->nodes, model->node_count * sizeof(struct model_node));
//
//			for (uint32 i = 0; i < animation->channel_count; i += 1) {
//				model_animation_channel *channel = &animation->channels[i];
//				model_animation_sampler *sampler = &animation->samplers[channel->sampler_index];
//				m_assert(sampler->interpolation_type == gpk_model_animation_linear_interpolation);
//				float time = (float)fmod(animation_time, (double)sampler->key_frames[sampler->key_frame_count - 1].time);
//				for (uint32 i = 0; i < sampler->key_frame_count; i += 1) {
//					model_animation_key_frame *key_frame = &sampler->key_frames[i];
//					if (time <= key_frame->time) {
//						if (channel->channel_type == gpk_model_animation_translate_channel) {
//							vec3 translate = {};
//							if (i == 0) {
//								translate = vec3_lerp(model_nodes[channel->node_index].local_transform.translate, { m_unpack3(key_frame->transform_data) }, key_frame->time == 0 ? 1 : time / key_frame->time);
//							}
//							else {
//								model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
//								translate = vec3_lerp({ m_unpack3(prev_key_frame->transform_data) }, { m_unpack3(key_frame->transform_data) }, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
//							}
//							model_nodes[channel->node_index].local_transform.translate = translate;
//						}
//						else if (channel->channel_type == gpk_model_animation_rotate_channel) {
//							quat rotate = {};
//							if (i == 0) {
//								rotate = quat_slerp(model_nodes[channel->node_index].local_transform.rotate, { m_unpack4(key_frame->transform_data) }, key_frame->time == 0 ? 1 : time / key_frame->time);
//							}
//							else {
//								model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
//								rotate = quat_slerp({ m_unpack4(prev_key_frame->transform_data) }, { m_unpack4(key_frame->transform_data) }, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
//							}
//							model_nodes[channel->node_index].local_transform.rotate = rotate;
//						}
//						else if (channel->channel_type == gpk_model_animation_scale_channel) {
//							vec3 scale = {};
//							if (i == 0) {
//								scale = vec3_lerp(model_nodes[channel->node_index].local_transform.scale, { m_unpack3(key_frame->transform_data) }, key_frame->time == 0 ? 1 : time / key_frame->time);
//							}
//							else {
//								model_animation_key_frame *prev_key_frame = &sampler->key_frames[i - 1];
//								scale = vec3_lerp({ m_unpack3(prev_key_frame->transform_data) }, { m_unpack3(key_frame->transform_data) }, (time - prev_key_frame->time) / (key_frame->time - prev_key_frame->time));
//							}
//							model_nodes[channel->node_index].local_transform.scale = scale;
//						}
//						break;
//					}
//				}
//			}
//			for (uint32 i = 0; i < model->node_count; i += 1) {
//				model_nodes[i].local_transform_mat = mat4_from_transform(model_nodes[i].local_transform);
//			}
//			for (auto &scene : make_range(model->scenes, model->scene_count)) {
//				for (auto &node_index : make_range(scene.node_indices, scene.node_index_count)) {
//					model_node *node = &model_nodes[node_index];
//					node->global_transform_mat = node->local_transform_mat;
//					std::stack<model_node *> node_stack;
//					node_stack.push(node);
//					while (!node_stack.empty()) {
//						model_node *parent_node = node_stack.top();
//						node_stack.pop();
//						for (uint32 i = 0; i < parent_node->child_count; i += 1) {
//							model_node *child_node = &model_nodes[parent_node->children[i]];
//							child_node->global_transform_mat = parent_node->global_transform_mat * child_node->local_transform_mat;
//							node_stack.push(child_node);
//						}
//					}
//				}
//			}
//		}
//		for (uint32 i = 0; i < model->skin_count; i += 1) {
//			model_skin *skin = &model->skins[i];
//			mat4 *joint_mats = memory_arena_alloc<mat4>(&world->frame_memory_arena, skin->joint_count);
//			for (uint32 i = 0; i < skin->joint_count; i += 1) {
//				joint_mats[i] = model_nodes[skin->joints[i].node_index].global_transform_mat * skin->joints[i].inverse_bind_mat;
//			}
//			joint_mats_offsets[i] = world_append_constant_buffer(world, joint_mats, skin->joint_count * sizeof(mat4));
//		}
//	}
//	uint32 mesh_index = 0;
//	for (uint32 i = 0; i < model->node_count; i += 1) {
//		model_node *node = &model->nodes[i];
//		if (node->mesh_index < model->mesh_count) {
//			mesh_render_data *mesh_render_data = &model_render_data->meshes[mesh_index++];
//			mesh_render_data->mesh = &model->meshes[node->mesh_index];
//			if (node->skin_index < model->skin_count) {
//				mesh_render_data->joint_mats_constant_buffer_offset = joint_mats_offsets[node->skin_index];
//			}
//			else {
//				mesh_render_data->joint_mats_constant_buffer_offset = world_append_constant_buffer(world, &node->global_transform_mat, sizeof(mat4));
//			}
//			mesh_render_data->primitive_count = mesh_render_data->mesh->primitive_count;
//			mesh_render_data->primitives = memory_arena_alloc<mesh_primitive_render_data>(&world->frame_memory_arena, mesh_render_data->primitive_count);
//			for (uint32 i = 0; i < mesh_render_data->primitive_count; i += 1) {
//				model_mesh_primitive *primitive = &mesh_render_data->mesh->primitives[i];
//				mesh_primitive_render_data *primitive_render_data = &mesh_render_data->primitives[i];
//				model_material *material = primitive->material_index < model->material_count ? &model->materials[primitive->material_index] : nullptr;
//				struct {
//					vec4 diffuse_factor;
//					float metallic_factor;
//					float roughness_factor;
//				} constant_buffer = {
//					material ? material->diffuse_factor : vec4{1, 1, 1, 1},
//					material ? material->metallic_factor : 1,
//					material ? material->roughness_factor : 1
//				};
//				primitive_render_data->constant_buffer_offset = world_append_constant_buffer(world, &constant_buffer, sizeof(constant_buffer));
//			}
//		}
//	}
//
//	if (render_collision_shapes) {
//		if (model->collision.type == collision_type_sphere) {
//			model_render_data->shape_count = 1;
//			model_render_data->shape_vertex_buffers[0] = world->sphere_vertex_buffer;
//			model_render_data->shape_vertex_counts[0] = m_countof(sphere_vertices);
//			float radius = model->collision.sphere.radius;
//			XMMATRIX transform_mat = XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), xmmatrix_transform(&transform));
//			model_render_data->shape_constant_buffer_offsets[0] = world_append_constant_buffer(world, &transform_mat, sizeof(transform_mat));
//		}
//		else if (model->collision.type == collision_type_box) {
//			model_render_data->shape_count = 1;
//			model_render_data->shape_vertex_buffers[0] = world->box_vertex_buffer;
//			model_render_data->shape_vertex_counts[0] = m_countof(box_vertices);
//			vec3 extents = model->collision.box.extents;
//			XMMATRIX transform_mat = XMMatrixMultiply(XMMatrixScaling(extents.x, extents.y, extents.z), xmmatrix_transform(&transform));
//			model_render_data->shape_constant_buffer_offsets[0] = world_append_constant_buffer(world, &transform_mat, sizeof(transform_mat));
//		}
//		else if (model->collision.type == collision_type_capsule) {
//			model_render_data->shape_count = 3;
//			model_render_data->shape_vertex_buffers[0] = world->hollow_cylinder_vertex_buffer;
//			model_render_data->shape_vertex_buffers[1] = world->hemisphere_vertex_buffer;
//			model_render_data->shape_vertex_buffers[2] = world->hemisphere_vertex_buffer;
//			model_render_data->shape_vertex_counts[0] = sizeof(hollow_cylinder_vertices);
//			model_render_data->shape_vertex_counts[1] = sizeof(hemisphere_vertices);
//			model_render_data->shape_vertex_counts[2] = sizeof(hemisphere_vertices);
//
//			XMMATRIX transform_mat = xmmatrix_transform(&transform);
//			float radius = model->collision.capsule.radius;
//			float height = model->collision.capsule.height * 0.5f;
//			XMMATRIX transform_mats[3];
//			transform_mats[0] = XMMatrixMultiply(XMMatrixScaling(radius, height, radius), transform_mat);
//			transform_mats[1] = XMMatrixMultiply(XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), XMMatrixTranslation(0, height, 0)), transform_mat);
//			transform_mats[2] = XMMatrixMultiply(XMMatrixMultiply(XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), XMMatrixRotationAxis(XMVectorSet(0, 0, 1, 0), (float)M_PI)), XMMatrixTranslation(0, -height, 0)), transform_mat);
//			model_render_data->shape_constant_buffer_offsets[0] = world_append_constant_buffer(world, &transform_mats[0], sizeof(transform_mats[0]));
//			model_render_data->shape_constant_buffer_offsets[1] = world_append_constant_buffer(world, &transform_mats[1], sizeof(transform_mats[1]));
//			model_render_data->shape_constant_buffer_offsets[2] = world_append_constant_buffer(world, &transform_mats[2], sizeof(transform_mats[2]));
//		}
//		else {
//			model_render_data->shape_count = 0;
//		}
//	}
//}

//struct world_render_desc {
//	XMMATRIX camera_view_proj_mat;
//	vec3 camera_position;
//	vec3 camera_view;
//
//	bool render_models;
//	bool render_terrain;
//	bool render_skybox;
//	bool render_shadow_proj_box;
//	bool screenshot;
//	struct editor *editor;
//	void(*append_extra_model_constants)(struct editor*, world*, d3d*);
//	void(*render_reference_grid)(struct editor*, world*, d3d*);
//	void(*append_terrain_brush_constants)(struct editor*, world*, d3d*);
//	void(*render_terrain_brush)(struct editor*, world*, d3d*);
//	void(*render_imgui)(struct editor*, world*, d3d*);
//};

//void render_world(world *world, d3d *d3d, render_world_desc *render_world_desc) {
//	{ // update constant buffer
//		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
//		m_d3d_assert(d3d->context->Map(world->constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource));
//		world->constant_buffer_ptr = (uint8 *)mapped_subresource.pData;
//		world->constant_buffer_offset = 0;
//
//		vec3 shadow_proj_box_size = { 128, 128, 128 };
//		vec3 shadow_proj_eye = world->sun_light_dir * shadow_proj_box_size.z * 0.5f;
//		vec3 shadow_proj_box_translate = shadow_proj_eye - (vec3_normalize(shadow_proj_eye) * shadow_proj_box_size.z * 0.5f);
//		struct {
//			uint32 swap_chain_width;
//			uint32 swap_chain_height;
//			uint32 padding0[2];
//			XMMATRIX camera_view_proj_mat;
//			vec4 camera_position;
//			vec4 camera_view;
//			vec4 sun_light_dir;
//			vec4 sun_light_color;
//			XMMATRIX shadow_proj_mat;
//		} common_constants = {
//			d3d->swap_chain_desc.Width,
//			d3d->swap_chain_desc.Height,
//			{},
//			render_world_desc->camera_view_proj_mat,
//			vec4{m_unpack3(render_world_desc->camera_position), 0},
//			vec4{m_unpack3(render_world_desc->camera_view), 0},
//			vec4{world->sun_light_dir.x, world->sun_light_dir.y, world->sun_light_dir.z, 0},
//			vec4{world->sun_light_color.x, world->sun_light_color.y, world->sun_light_color.z, 0},
//			XMMatrixMultiply(XMMatrixLookAtRH(XMVectorSet(m_unpack3(shadow_proj_eye), 0), XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0)), XMMatrixOrthographicRH(m_unpack3(shadow_proj_box_size), 0))
//		};
//		append_world_constant_buffer(world, &common_constants, sizeof(common_constants));
//
//		world->render_data.model_list = nullptr;
//		world->render_data.model_count = 0;
//		if (render_world_desc->render_models) {
//			if (world->player.model_index < world->model_count) {
//				add_model_render_data(world, d3d, world->player.model_index, world->player.transform, world->player.animation_index, world->player.animation_time, false);
//			}
//		}
//
//		if (render_world_desc->append_extra_model_constants) {
//			render_world_desc->append_extra_model_constants(render_world_desc->editor, world, d3d);
//		}
//
//		if (render_world_desc->render_terrain && world->terrain_index < world->terrain_count) {
//			terrain *terrain = &world->terrains[world->terrain_index];
//			struct {
//				float max_height;
//				float sample_per_meter;
//			} terrain_constant_buffer = {
//				terrain->max_height,
//				(float)terrain->sample_per_meter
//			};
//			uint32 offset = append_world_constant_buffer(world, &terrain_constant_buffer, sizeof(terrain_constant_buffer));
//			world->render_data.terrain_constant_buffer_offset = offset / 16;
//		}
//
//		if (render_world_desc->append_terrain_brush_constants) {
//			render_world_desc->append_terrain_brush_constants(render_world_desc->editor, world, d3d);
//		}
//
//		if (render_world_desc->render_shadow_proj_box) {
//			quat q = quat_from_between({ 0, 0, 1 }, vec3_normalize(shadow_proj_eye));
//			XMMATRIX transform_mat = XMMatrixScaling(m_unpack3(shadow_proj_box_size));
//			transform_mat = XMMatrixMultiply(transform_mat, XMMatrixRotationQuaternion(XMVectorSet(m_unpack4(q))));
//			transform_mat = XMMatrixMultiply(transform_mat, XMMatrixTranslation(m_unpack3(shadow_proj_box_translate)));
//			world->render_data.shadow_proj_box_constant_buffer_offset = append_world_constant_buffer(world, &transform_mat, sizeof(transform_mat));
//		}
//
//		d3d->context->Unmap(world->constant_buffer, 0);
//
//		uint32 offset = 0;
//		uint32 constant_count = 16;
//		d3d->context->VSSetConstantBuffers1(0, 1, &world->constant_buffer, &offset, &constant_count);
//		d3d->context->PSSetConstantBuffers1(0, 1, &world->constant_buffer, &offset, &constant_count);
//	}
//	{ // prepare shadow framebuffer
//		D3D11_VIEWPORT viewport = {};
//		viewport.Width = (float)d3d->shadow_framebuffer_width;
//		viewport.Height = (float)d3d->shadow_framebuffer_height;
//		viewport.MinDepth = 0.0f;
//		viewport.MaxDepth = 1.0f;
//		d3d->context->RSSetViewports(1, &viewport);
//
//		d3d->context->ClearRenderTargetView(d3d->shadow_framebuffer_render_target_views[0], Colors::Black);
//		d3d->context->ClearRenderTargetView(d3d->shadow_framebuffer_render_target_views[1], Colors::Black);
//		d3d->context->ClearDepthStencilView(d3d->shadow_framebuffer_depth_view, D3D11_CLEAR_DEPTH, 0, 0);
//		d3d->context->OMSetRenderTargets(1, &d3d->shadow_framebuffer_render_target_views[0], d3d->shadow_framebuffer_depth_view);
//	}
//	{ // render shadows
//		d3d->context->VSSetShader(d3d->mesh_shadow_vs, nullptr, 0);
//		d3d->context->PSSetShader(d3d->mesh_shadow_ps, nullptr, 0);
//		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		d3d->context->IASetInputLayout(d3d->mesh_input_layout);
//		d3d->context->RSSetState(d3d->mesh_rasterizer_state);
//		d3d->context->OMSetDepthStencilState(d3d->mesh_depth_stencil_state, 0);
//		d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
//
//		model_render_data *model_render_data = world->render_data.model_list;
//		while (model_render_data) {
//			uint32 model_constant_buffer_offset = model_render_data->model_mat_constant_buffer_offset / 16;
//			uint32 model_constant_count = 16;
//			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &model_constant_buffer_offset, &model_constant_count);
//			for (uint32 i = 0; i < model_render_data->mesh_count; i += 1) {
//				mesh_render_data *mesh_render_data = &model_render_data->meshes[i];
//				model_mesh *mesh = mesh_render_data->mesh;
//				uint32 mesh_constant_buffer_offset = mesh_render_data->joint_mats_constant_buffer_offset / 16;
//				uint32 mesh_constant_count = 256 * 4;
//				d3d->context->VSSetConstantBuffers1(2, 1, &world->constant_buffer, &mesh_constant_buffer_offset, &mesh_constant_count);
//				for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
//					model_mesh_primitive *primitive = &mesh->primitives[i];
//					uint32 vertex_buffer_stride = sizeof(gpk_model_vertex);
//					uint32 vertex_buffer_offset = 0;
//					//d3d->context->IASetVertexBuffers(0, 1, &primitive->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
//					//d3d->context->IASetIndexBuffer(primitive->index_buffer, DXGI_FORMAT_R16_UINT, 0);
//					//d3d->context->DrawIndexed(primitive->index_count, 0, 0);
//				}
//			}
//			model_render_data = model_render_data->next;
//		}
//
//		if (render_world_desc->render_terrain && world->terrain_index < world->terrain_count) {
//			terrain *terrain = &world->terrains[world->terrain_index];
//			d3d->context->VSSetShader(d3d->terrain_shadow_vs, nullptr, 0);
//			d3d->context->PSSetShader(d3d->terrain_shadow_ps, nullptr, 0);
//			d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//			d3d->context->IASetInputLayout(d3d->terrain_input_layout);
//			d3d->context->VSSetShaderResources(0, 1, &terrain->height_texture_view);
//			d3d->context->VSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
//			uint32 constant_count = 16;
//			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &world->render_data.terrain_constant_buffer_offset, &constant_count);
//			uint32 vertex_buffer_stride = sizeof(terrain_vertex);
//			uint32 vertex_buffer_offset = 0;
//			d3d->context->IASetVertexBuffers(0, 1, &terrain->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
//			d3d->context->IASetIndexBuffer(terrain->index_buffer, DXGI_FORMAT_R32_UINT, 0);
//			d3d->context->DrawIndexed(terrain->index_count, 0, 0);
//		}
//	}
//	{ // gaussian blur shadow
//		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		d3d->context->IASetInputLayout(d3d->terrain_input_layout);
//		d3d->context->RSSetState(nullptr);
//		d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
//		d3d->context->OMSetDepthStencilState(nullptr, 0);
//
//		d3d->context->OMSetRenderTargets(1, &d3d->shadow_framebuffer_render_target_views[1], nullptr);
//		d3d->context->VSSetShader(d3d->gaussian_blur_x_vs, nullptr, 0);
//		d3d->context->PSSetShader(d3d->gaussian_blur_x_ps, nullptr, 0);
//		d3d->context->PSSetShaderResources(0, 1, &d3d->shadow_framebuffer_texture_views[0]);
//		d3d->context->Draw(3, 0);
//
//		ID3D11ShaderResourceView *null_view = nullptr;
//		d3d->context->PSSetShaderResources(0, 1, &null_view);
//		d3d->context->OMSetRenderTargets(1, &d3d->shadow_framebuffer_render_target_views[0], nullptr);
//		d3d->context->PSSetShaderResources(0, 1, &d3d->shadow_framebuffer_texture_views[1]);
//		d3d->context->VSSetShader(d3d->gaussian_blur_y_vs, nullptr, 0);
//		d3d->context->PSSetShader(d3d->gaussian_blur_y_ps, nullptr, 0);
//		d3d->context->Draw(3, 0);
//	}
//	{ // prepare final framebuffer
//		D3D11_VIEWPORT viewport = {};
//		viewport.Width = (float)d3d->final_framebuffer_width;
//		viewport.Height = (float)d3d->final_framebuffer_height;
//		viewport.MinDepth = 0.0f;
//		viewport.MaxDepth = 1.0f;
//		d3d->context->RSSetViewports(1, &viewport);
//
//		d3d->context->ClearRenderTargetView(d3d->final_framebuffer_render_target_view, Colors::Black);
//		d3d->context->ClearDepthStencilView(d3d->final_framebuffer_depth_view, D3D11_CLEAR_DEPTH, 0, 0);
//		d3d->context->OMSetRenderTargets(1, &d3d->final_framebuffer_render_target_view, d3d->final_framebuffer_depth_view);
//	}
//	{ // render models
//		d3d->context->VSSetShader(d3d->mesh_vs, nullptr, 0);
//		d3d->context->PSSetShader(d3d->mesh_ps, nullptr, 0);
//		d3d->context->PSSetSamplers(0, 1, &d3d->repeat_sampler_state);
//		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		d3d->context->IASetInputLayout(d3d->mesh_input_layout);
//		d3d->context->RSSetState(d3d->mesh_rasterizer_state);
//		d3d->context->OMSetDepthStencilState(d3d->mesh_depth_stencil_state, 0);
//		d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
//
//		model_render_data *model_render_data = world->render_data.model_list;
//		while (model_render_data) {
//			model *model = model_render_data->model;
//			uint32 model_constant_buffer_offset = model_render_data->model_mat_constant_buffer_offset / 16;
//			uint32 model_constant_count = 16;
//			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &model_constant_buffer_offset, &model_constant_count);
//			for (uint32 i = 0; i < model_render_data->mesh_count; i += 1) {
//				mesh_render_data *mesh_render_data = &model_render_data->meshes[i];
//				model_mesh *mesh = mesh_render_data->mesh;
//				uint32 mesh_constant_buffer_offset = mesh_render_data->joint_mats_constant_buffer_offset / 16;
//				uint32 mesh_constant_count = 256 * 4;
//				d3d->context->VSSetConstantBuffers1(2, 1, &world->constant_buffer, &mesh_constant_buffer_offset, &mesh_constant_count);
//				for (uint32 i = 0; i < mesh->primitive_count; i += 1) {
//					model_mesh_primitive *primitive = &mesh->primitives[i];
//					mesh_primitive_render_data *primitive_render_data = &mesh_render_data->primitives[i];
//					uint32 primitive_constant_buffer_offset = primitive_render_data->constant_buffer_offset / 16;
//					uint32 primitive_constant_count = 16;
//					d3d->context->PSSetConstantBuffers1(3, 1, &world->constant_buffer, &primitive_constant_buffer_offset, &primitive_constant_count);
//					model_material *material = primitive->material_index < model->material_count ? &model->materials[primitive->material_index] : nullptr;
//					//ID3D11ShaderResourceView *texture_views[4] = { world->default_diffuse_texture_view, world->default_roughness_texture_view, world->default_metallic_texture_view, world->default_normal_texture_view };
//					//if (material && material->diffuse_texture_index < model->texture_count) {
//					//	texture_views[0] = model->textures[material->diffuse_texture_index].texture_view;
//					//}
//					//if (material && material->roughness_texture_index < model->texture_count) {
//					//	texture_views[1] = model->textures[material->roughness_texture_index].texture_view;
//					//}
//					//if (material && material->metallic_texture_index < model->texture_count) {
//					//	texture_views[2] = model->textures[material->metallic_texture_index].texture_view;
//					//}
//					//if (material && material->normal_texture_index < model->texture_count) {
//					//	texture_views[3] = model->textures[material->normal_texture_index].texture_view;
//					//}
//					//d3d->context->PSSetShaderResources(0, m_countof(texture_views), texture_views);
//					//uint32 vertex_buffer_stride = sizeof(gpk_model_vertex);
//					//uint32 vertex_buffer_offset = 0;
//					//d3d->context->IASetVertexBuffers(0, 1, &primitive->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
//					//d3d->context->IASetIndexBuffer(primitive->index_buffer, DXGI_FORMAT_R16_UINT, 0);
//					//d3d->context->DrawIndexed(primitive->index_count, 0, 0);
//				}
//			}
//			model_render_data = model_render_data->next;
//		}
//
//		d3d->context->VSSetShader(d3d->collision_shape_vs, nullptr, 0);
//		d3d->context->PSSetShader(d3d->collision_shape_ps, nullptr, 0);
//		d3d->context->IASetInputLayout(d3d->collision_shape_input_layout);
//		d3d->context->OMSetDepthStencilState(d3d->collision_shape_depth_stencil_state, 0);
//		d3d->context->OMSetBlendState(d3d->collision_shape_blend_state, nullptr, 0xffffffff);
//
//		model_render_data = world->render_data.model_list;
//		while (model_render_data) {
//			for (uint32 i = 0; i < model_render_data->shape_count; i += 1) {
//				uint32 vertex_buffer_stride = 12;
//				uint32 vertex_buffer_offset = 0;
//				d3d->context->IASetVertexBuffers(0, 1, &model_render_data->shape_vertex_buffers[i], &vertex_buffer_stride, &vertex_buffer_offset);
//				uint32 constant_buffer_offset = model_render_data->shape_constant_buffer_offsets[i] / 16;
//				uint32 constant_count = 16;
//				d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &constant_buffer_offset, &constant_count);
//				d3d->context->Draw(model_render_data->shape_vertex_counts[i], 0);
//			}
//			model_render_data = model_render_data->next;
//		}
//	}
//	{ // render terrain
//		if (render_world_desc->render_terrain && world->terrain_index < world->terrain_count) {
//			terrain *terrain = &world->terrains[world->terrain_index];
//			d3d->context->VSSetShader(d3d->terrain_vs, nullptr, 0);
//			d3d->context->PSSetShader(d3d->terrain_ps, nullptr, 0);
//			d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//			d3d->context->IASetInputLayout(d3d->terrain_input_layout);
//			d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
//			d3d->context->OMSetDepthStencilState(d3d->terrain_depth_stencil_state, 0);
//			d3d->context->VSSetShaderResources(0, 1, &terrain->height_texture_view);
//			d3d->context->PSSetShaderResources(1, 1, &terrain->diffuse_texture_view);
//			d3d->context->PSSetShaderResources(2, 1, &d3d->shadow_framebuffer_texture_views[0]);
//			d3d->context->VSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
//			d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
//			d3d->context->PSSetSamplers(1, 1, &d3d->shadow_framebuffer_sampler_state);
//			uint32 constant_count = 16;
//			d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &world->render_data.terrain_constant_buffer_offset, &constant_count);
//			uint32 vertex_buffer_stride = sizeof(terrain_vertex);
//			uint32 vertex_buffer_offset = 0;
//			d3d->context->IASetVertexBuffers(0, 1, &terrain->vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
//			d3d->context->IASetIndexBuffer(terrain->index_buffer, DXGI_FORMAT_R32_UINT, 0);
//			d3d->context->DrawIndexed(terrain->index_count, 0, 0);
//
//			ID3D11ShaderResourceView *null_view = nullptr;
//			d3d->context->PSSetShaderResources(2, 1, &null_view);
//		}
//	}
//	{ // render skybox
//		if (render_world_desc->render_skybox && world->skybox_index < world->skybox_count) {
//			skybox *skybox = &world->skyboxes[world->skybox_index];
//			d3d->context->VSSetShader(d3d->skybox_vs, nullptr, 0);
//			d3d->context->PSSetShader(d3d->skybox_ps, nullptr, 0);
//			d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//			d3d->context->RSSetState(d3d->skybox_rasterizer_state);
//			d3d->context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
//			d3d->context->OMSetDepthStencilState(d3d->skybox_depth_stencil_state, 0);
//			d3d->context->PSSetShaderResources(0, 1, &skybox->cube_texture_view);
//			d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
//			d3d->context->Draw(36, 0);
//		}
//	}
//	if (render_world_desc->render_shadow_proj_box) {
//		d3d->context->VSSetShader(d3d->collision_shape_vs, nullptr, 0);
//		d3d->context->PSSetShader(d3d->collision_shape_ps, nullptr, 0);
//		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		d3d->context->IASetInputLayout(d3d->collision_shape_input_layout);
//		d3d->context->OMSetDepthStencilState(d3d->collision_shape_depth_stencil_state, 0);
//		d3d->context->OMSetBlendState(d3d->collision_shape_blend_state, nullptr, 0xffffffff);
//
//		uint32 vertex_buffer_stride = 12;
//		uint32 vertex_buffer_offset = 0;
//		d3d->context->IASetVertexBuffers(0, 1, &world->box_vertex_buffer, &vertex_buffer_stride, &vertex_buffer_offset);
//		uint32 constant_buffer_offset = world->render_data.shadow_proj_box_constant_buffer_offset / 16;
//		uint32 constant_count = 16;
//		d3d->context->VSSetConstantBuffers1(1, 1, &world->constant_buffer, &constant_buffer_offset, &constant_count);
//		d3d->context->Draw(m_countof(box_vertices), 0);
//	}
//	if (render_world_desc->render_reference_grid) {
//		render_world_desc->render_reference_grid(render_world_desc->editor, world, d3d);
//	}
//	if (render_world_desc->render_terrain && render_world_desc->render_terrain_brush) {
//		render_world_desc->render_terrain_brush(render_world_desc->editor, world, d3d);
//	}
//	{ // prepare swap chain
//		D3D11_VIEWPORT viewport = {};
//		viewport.Width = (float)d3d->swap_chain_desc.Width;
//		viewport.Height = (float)d3d->swap_chain_desc.Height;
//		viewport.MinDepth = 0.0f;
//		viewport.MaxDepth = 1.0f;
//		d3d->context->RSSetViewports(1, &viewport);
//
//		d3d->context->ClearRenderTargetView(d3d->swap_chain_render_target_view, Colors::Black);
//		d3d->context->OMSetRenderTargets(1, &d3d->swap_chain_render_target_view, nullptr);
//	}
//	{ // final framebuffer to screen
//		d3d->context->VSSetShader(d3d->final_framebuffer_to_screen_vs, nullptr, 0);
//		d3d->context->PSSetShader(d3d->final_framebuffer_to_screen_ps, nullptr, 0);
//		d3d->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		d3d->context->RSSetState(nullptr);
//		d3d->context->PSSetShaderResources(0, 1, &d3d->final_framebuffer_texture_view);
//		d3d->context->PSSetSamplers(0, 1, &d3d->clamp_edge_sampler_state);
//		d3d->context->Draw(3, 0);
//	}
//	if (render_world_desc->render_imgui) {
//		render_world_desc->render_imgui(render_world_desc->editor, world, d3d);
//	}
//
//	m_d3d_assert(d3d->swap_chain->Present(0, 0));
//}

#endif
