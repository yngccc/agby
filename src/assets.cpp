/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#define RAPIDJSON_SSE2
#define RAPIDJSON_ASSERT(x) m_assert(x)
#include "../vendor/include/rapidjson/document.h"
#include "../vendor/include/rapidjson/prettywriter.h"
#include "../vendor/include/rapidjson/error/en.h"

// vertex attribute sizes:
//   position                     12   32/32/32
//   uv                           8    32/32
//   normal                       6    16/16/16
//   tangent                      6    16/16/16
//   joint indices                4    8/8/8/8
//   joint weights                8    16/16/16/16
//   max_joints_count_per_vertex  4
//   max_joints_count_per_mesh    256

const uint32 gpk_model_max_mesh_count = 512;
const uint32 gpk_model_max_material_count = 512;

struct gpk_model_mesh_header {
	uint32 instances_offset;
	uint32 instance_count;
	uint32 vertices_offset;
	uint32 vertex_size;
	uint32 vertex_count;
	uint32 indices_offset;
	uint32 index_size;
	uint32 index_count;
	uint32 material_index;
	char name[64];
};

struct gpk_model_mesh_instance {
	mat4 transform;
	aa_bound bound;
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

struct gpk_texture_header {
	uint32 texture_offset;
	uint32 texture_size;
	gpk_texture_format texture_format;
	uint32 texture_pixel_size;
	uint32 texture_width;
	uint32 texture_height;
	uint32 texture_mipmap_count;
};

struct gpk_model_material_header {
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
	char name[64];
};

struct gpk_model_header {
	uint32 mesh_offsets[gpk_model_max_mesh_count];
	uint32 mesh_count;
	uint32 material_offsets[gpk_model_max_material_count];
	uint32 material_count;
	aa_bound bound;
};

struct gpk_skybox_header {
	uint32 cubemap_offset;
	uint32 cubemap_width;
	uint32 cubemap_height;
};

struct gpk_terrain_header {
	uint32 diffuse_map_offset;
	vec2 diffuse_map_scales;
	uint32 vertices_offset;
	uint32 vertex_size;
	uint32 vertex_count;
	mat4 transform;
};

enum gpk_import_type {
	gpk_import_type_fbx_model,
	gpk_import_type_skybox
};

struct gpk_import_cmd_line {
	gpk_import_type import_type;
	char asset_file[256];
	char import_file[256];
};

#ifndef ASSETS_IMPORT_STRUCT_ONLY

void assets_import_issue_jobs() {
	auto create_import_process = [](gpk_import_cmd_line *cmdl, HANDLE job_object) {
		STARTUPINFOA startup_info = {sizeof(startup_info)};
		startup_info.dwFlags = STARTF_USESHOWWINDOW;
		startup_info.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION process_info;
		char cmdl_str[1024];
		snprintf(cmdl_str, sizeof(cmdl_str), "import.exe %d %s %s", cmdl->import_type, cmdl->asset_file, cmdl->import_file);
		m_assert(CreateProcessA("import.exe", cmdl_str, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &startup_info, &process_info));
		m_assert(AssignProcessToJobObject(job_object, process_info.hProcess));
		ResumeThread(process_info.hThread);
		CloseHandle(process_info.hThread);
		CloseHandle(process_info.hProcess);
	};

	HANDLE job_object = CreateJobObject(nullptr, nullptr);
	HANDLE io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
	m_assert(job_object);
	m_assert(io_port);
	m_scope_exit(
		CloseHandle(job_object);
	  CloseHandle(io_port);
	);
	JOBOBJECT_ASSOCIATE_COMPLETION_PORT job_port;
	job_port.CompletionKey = job_object;
	job_port.CompletionPort = io_port;
	m_assert(SetInformationJobObject(job_object, JobObjectAssociateCompletionPortInformation, &job_port, sizeof(job_port)));
	console("Import jobs started\n");
	uint32 job_count = 0;

	const char *import_json_file = "agby_assets\\import.json";
	file_mapping import_json_file_mapping = {};
	m_assert(open_file_mapping(import_json_file, &import_json_file_mapping));
	m_scope_exit(close_file_mapping(import_json_file_mapping));
	rapidjson::Document json;
	rapidjson::ParseResult json_parse_result = json.Parse((const char *)import_json_file_mapping.ptr);
	if (!json_parse_result) {
		fatal("Json parse error:\nFile: %s\nOffset:: %u\nError: %s", import_json_file, (uint32)json_parse_result.Offset(), rapidjson::GetParseError_En(json_parse_result.Code()));
	}

	bool force_import_all = json["force_import_all"].GetBool();
	bool force_import_models = json["force_import_models"].GetBool();
	bool force_import_skyboxes = json["force_import_skyboxes"].GetBool();

	rapidjson::Value::Array models = json["models"].GetArray();
	for (uint32 i = 0; i < models.Size(); i += 1) {
		rapidjson::Value::Object model = models[i].GetObject();
		if (model["import"].GetBool() || force_import_all || force_import_models) {
			const char *asset_file = model["asset_file"].GetString();
			const char *import_file = model["import_file"].GetString();
			gpk_import_cmd_line cmdl = {};
			cmdl.import_type = gpk_import_type_fbx_model;
			snprintf(cmdl.asset_file, sizeof(cmdl.asset_file), "%s", asset_file);
			snprintf(cmdl.import_file, sizeof(cmdl.import_file), "%s", import_file);
			create_import_process(&cmdl, job_object);
			job_count += 1;
		}
	}
	rapidjson::Value::Array skyboxes = json["skyboxes"].GetArray();
	for (uint32 i = 0; i < skyboxes.Size(); i += 1) {
		rapidjson::Value::Object skybox = skyboxes[i].GetObject();
		if (skybox["import"].GetBool() || force_import_all || force_import_skyboxes) {
			const char *asset_file = skybox["asset_file"].GetString();
			const char *import_file = skybox["import_file"].GetString();
			gpk_import_cmd_line cmdl = {};
			cmdl.import_type = gpk_import_type_skybox;
			snprintf(cmdl.asset_file, sizeof(cmdl.asset_file), "%s", asset_file);
			snprintf(cmdl.import_file, sizeof(cmdl.import_file), "%s", import_file);
			create_import_process(&cmdl, job_object);
			job_count += 1;
		}
	}
	// for (uint32 i = 0; i < m_countof(gpk_terrain_infos); i += 1) {
	//   if (check_need_to_import(&gpk_terrain_infos[i].file_name, 1, gpk_terrain_infos[i].import_file_name)) {
	//     jobs[job_id].import_type = gpk_import_type_fbx_terrain;
	//     strcpy(jobs[job_id].import_file_name, gpk_terrain_infos[i].import_file_name);
	//     jobs[job_id].status = gpk_assets_import_job_status_progress;
	//     jobs[job_id].error_message[0] = '\0';
	//     gpk_import_cmd_line cmdl = m_gpk_import_cmd_line_default;
	//     cmdl.import_job_id = job_id;
	//     cmdl.import_type = gpk_import_type_fbx_terrain;
	//     strcpy(cmdl.file_names[0], gpk_terrain_infos[i].file_name);
	//     strcpy(cmdl.import_file_name, gpk_terrain_infos[i].import_file_name);
	//     assets_import_create_process(&cmdl, process_group, memory_arena);
	//     job_id += 1;
	//   }
	// }
	// for (uint32 i = 0; i < m_countof(gpk_skybox_infos); i += 1) {
	//   gpk_skybox_info *gpk_skybox_info = &gpk_skybox_infos[i];
	//   if (check_need_to_import(gpk_skybox_info->file_names, m_countof(gpk_skybox_info->file_names), gpk_skybox_info->import_file_name)) {
	//     jobs[job_id].import_type = gpk_import_type_skybox;
	//     strcpy(jobs[job_id].import_file_name, gpk_skybox_info->import_file_name);
	//     jobs[job_id].status = gpk_assets_import_job_status_progress;
	//     jobs[job_id].error_message[0] = '\0';
	//     gpk_import_cmd_line cmdl = m_gpk_import_cmd_line_default;
	//     cmdl.import_job_id = job_id;
	//     cmdl.import_type = gpk_import_type_skybox;
	//     for (uint32 i = 0; i < m_countof(cmdl.file_names); i += 1) {
	//       strcpy(cmdl.file_names[i], gpk_skybox_info->file_names[i]);
	//     }
	//     strcpy(cmdl.import_file_name, gpk_skybox_infos->import_file_name);
	//     assets_import_create_process(&cmdl, process_group, memory_arena);
	//     job_id += 1;
	//   }
	// }
	if (job_count > 0) {
		DWORD completion_code;
		ULONG_PTR completion_key;
		LPOVERLAPPED overlappped;
		while (GetQueuedCompletionStatus(io_port, &completion_code, &completion_key, &overlappped, INFINITE)) {
			if ((HANDLE)completion_key == job_object && completion_code == JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO) {
				break;
			}
		}
	}
	console("Import jobs all done\n\n");
}

#endif
