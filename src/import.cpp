/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"
#include "math.cpp"
#define ASSETS_IMPORT_STRUCT_ONLY
#include "assets.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/include/stb/stb_image.h"
#define FBXSDK_SHARED
#include "../vendor/include/fbx/fbxsdk.h"
#define NVTT_SHARED 1
#include "../vendor/include/nvtt/nvtt.h"

typedef vec3 position;
typedef vec2 uv;
typedef i16vec3 normal;
typedef i16vec3 tangent;

struct vertex {
	position position;
	uv uv;
	normal normal;
	tangent tangent;
};
static_assert(sizeof(vertex) == 32, "");

struct mesh {
	FbxMesh *fbx_mesh;
	gpk_model_mesh_instance instances[128];
	uint32 instance_count;
	vertex *vertices;	
	uint32 vertex_count;
	uint16 *indices;
	uint32 index_count;
	uint32 material_index;
	char name[sizeof(gpk_model_mesh_header::name)];
};

struct material {
	uint8 *albedo_map_data;
	int32 albedo_map_width;
	int32 albedo_map_height;
	uint32 albedo_map_mipmap_count;
	uint32 albedo_map_size;
	uint8 *normal_map_data;
	int32 normal_map_width;
	int32 normal_map_height;
	uint32 normal_map_mipmap_count;
	uint32 normal_map_size;
	uint8 *metallic_map_data;
	int32 metallic_map_width;
	int32 metallic_map_height;
	uint32 metallic_map_mipmap_count;
	uint32 metallic_map_size;
	uint8 *roughness_map_data;
	int32 roughness_map_width;
	int32 roughness_map_height;
	uint32 roughness_map_mipmap_count;
	uint32 roughness_map_size;
	uint8 *ao_map_data;
	int32 ao_map_width;
	int32 ao_map_height;
	uint32 ao_map_mipmap_count;
	uint32 ao_map_size;
	uint8 *height_map_data;
	int32 height_map_width;
	int32 height_map_height;
	uint32 height_map_mipmap_count;
	uint32 height_map_size;
	char name[sizeof(gpk_model_material_header::name)];
};

enum image_type {
	image_type_albedo_map,
	image_type_normal_map,
	image_type_metallic_map,
	image_type_roughness_map,
	image_type_ao_map,
	image_type_height_map
};

template <typename T>
bool traverse_fbx_nodes(FbxNode *node, T node_func) {
	if (!node_func(node)) {
		return false;
	}
	for (int i = 0; i < node->GetChildCount(); i += 1) {
		if (!traverse_fbx_nodes(node->GetChild(i), node_func)) {
			return false;
		}
	}
	return true;
}

mat4 fbx_mat4_to_mat4(FbxAMatrix *fbx_mat) {
	FbxVector4 r1 = fbx_mat->GetRow(0);
	FbxVector4 r2 = fbx_mat->GetRow(1);
	FbxVector4 r3 = fbx_mat->GetRow(2);
	FbxVector4 r4 = fbx_mat->GetRow(3);
	mat4 mat;
	mat.c1 = {{(float)r1[0], (float)r1[1], (float)r1[2], (float)r1[3]}};
	mat.c2 = {{(float)r2[0], (float)r2[1], (float)r2[2], (float)r2[3]}};
	mat.c3 = {{(float)r3[0], (float)r3[1], (float)r3[2], (float)r3[3]}};
	mat.c4 = {{(float)r4[0], (float)r4[1], (float)r4[2], (float)r4[3]}};
	return mat;
}

int32 fbx_get_element_array_index(uint32 polygon_vertex_index, int32 control_point_index, FbxLayerElement::EMappingMode mapping, FbxLayerElement::EReferenceMode ref, FbxLayerElementArrayTemplate<int> &indices) {
	if (mapping == FbxLayerElement::eByControlPoint) {
		if (ref == FbxLayerElement::eDirect) {
			return control_point_index;
		}
		else {
			return indices[control_point_index];
		}
	}
	else if (mapping == FbxLayerElement::eByPolygonVertex) {
		if (ref == FbxLayerElement::eDirect) {
			return polygon_vertex_index;
		}
		else {
			return indices[polygon_vertex_index];
		}
	}
	else {
		m_assert(false);
		return 0;
	}
}

const char *fbx_enum_to_string(FbxLayerElement::EMappingMode mode) {
	switch (mode) {
		case FbxLayerElement::eNone: {
			return "eNone";
		} break;
		case FbxLayerElement::eByControlPoint: {
			return "eByControlPoint";
		} break;
		case FbxLayerElement::eByPolygonVertex: {
			return "eByPolygonVertex";
		} break;
		case FbxLayerElement::eByPolygon: {
			return "eByPolygon";
		} break;
		case FbxLayerElement::eByEdge: {
			return "eByEdge";
		} break;
		case FbxLayerElement::eAllSame: {
			return "eAllSame";
		} break;
		default: {
			return "";
		}
	}
}

const char *fbx_enum_to_string(FbxLayerElement::EReferenceMode mode) {
	switch (mode) {
		case FbxLayerElement::eDirect: {
			return "eDirect";
		} break;
		case FbxLayerElement::eIndex: {
			return "eIndex";
		} break;
		case FbxLayerElement::eIndexToDirect: {
			return "eIndexToDirect";
		} break;
		default: {
			return "";
		}
	}
}

aa_bound compute_mesh_bound(FbxMesh *fbx_mesh, mat4 transform) {
	FbxVector4 *control_points = fbx_mesh->GetControlPoints();
	int32 control_point_count = fbx_mesh->GetControlPointsCount();
	vec3 min_control_point = (transform * vec4{(float)control_points[0][0], (float)control_points[0][1], (float)control_points[0][2], 1}).xyz();
	vec3 max_control_point = min_control_point;
	for (int32 i = 0; i < control_point_count; i += 1) {
		vec4 control_point = vec4{(float)control_points[i][0], (float)control_points[i][1], (float)control_points[i][2], 1};
		control_point = transform * control_point;
		min_control_point.x = min(min_control_point.x, control_point.x);
		min_control_point.y = min(min_control_point.y, control_point.y);
		min_control_point.z = min(min_control_point.z, control_point.z);
		max_control_point.x = max(max_control_point.x, control_point.x);
		max_control_point.y = max(max_control_point.y, control_point.y);
		max_control_point.z = max(max_control_point.z, control_point.z);
	}
	return aa_bound{min_control_point, max_control_point};
}

aa_bound compute_meshes_bound(mesh *meshes, uint32 mesh_count) {
	aa_bound bound = {};
	for (uint32 i = 0; i < mesh_count; i += 1) {
		struct mesh *mesh = &meshes[i];
		for (uint32 i = 0; i < mesh->instance_count; i += 1) {
			aa_bound *b = &mesh->instances[i].bound;
			bound.min.x = min(bound.min.x, b->min.x);
			bound.min.y = min(bound.min.y, b->min.y);
			bound.min.z = min(bound.min.z, b->min.z);
			bound.max.x = max(bound.max.x, b->max.x);
			bound.max.y = max(bound.max.y, b->max.y);
			bound.max.z = max(bound.max.z, b->max.z);
		}
	}
	return bound;
}

void rgba_to_bgra(uint8 *image_data, uint32 image_width, uint32 image_height) {
	for (uint32 i = 0; i < image_width * image_height; i += 1) {
		uint8 r = image_data[i * 4];
		image_data[i * 4] = image_data[i * 4 + 2];
		image_data[i * 4 + 2] = r;
	}
}

void compress_image(image_type image_type, uint8 *data, uint32 width, uint32 height, uint8 **compressed_data, uint32 *compressed_data_mipmap_count, uint32 *compressed_data_size) {
	using namespace nvtt;

	InputOptions input_options = {};
	CompressionOptions compression_options = {};
	OutputOptions output_options = {};
	Compressor compressor = {};

	input_options.setTextureLayout(TextureType_2D, width, height);
	input_options.setFormat(InputFormat_BGRA_8UB);
	input_options.setAlphaMode(AlphaMode_None);
	input_options.setMipmapFilter(MipmapFilter_Kaiser);
	input_options.setMipmapGeneration(true);
	input_options.setNormalMap(image_type == image_type_normal_map);
	if (!input_options.setMipmapData(data, width, height)) {
		fatal("call to \"input_options.setMipmapData\" failed");
	}

	if (image_type == image_type_albedo_map) {
		compression_options.setFormat(Format_BC1);
	}
	else if (image_type == image_type_normal_map) {
		compression_options.setFormat(Format_BC5);
	}
	else {
		input_options.setGamma(1.0f, 1.0f);
		compression_options.setFormat(Format_BC4);
	}
	compression_options.setQuality(Quality_Normal); // Quality_Production Quality_Highest

	struct output_handler : public OutputHandler {
		uint32 estimate_compressed_data_size;
		uint8 *compressed_data;
		uint32 compressed_data_mipmap_count;
		uint32 compressed_data_size;

		void beginImage(int size, int width, int height, int depth, int face, int miplevel) {
			compressed_data_mipmap_count += 1;
		}
		bool writeData(const void * data, int size) {
			memcpy(compressed_data + compressed_data_size, data, size);
			compressed_data_size += size;
			m_assert(compressed_data_size <= estimate_compressed_data_size);
			return true;
		}
		void endImage() {}
	};
	output_handler output_handler = {};
	output_handler.estimate_compressed_data_size = compressor.estimateSize(input_options, compression_options);
	output_handler.compressed_data = (uint8 *)calloc(output_handler.estimate_compressed_data_size, 1);
	output_handler.compressed_data_mipmap_count = 0;
	output_handler.compressed_data_size = 0;
	output_options.setOutputHandler(&output_handler);
	output_options.setOutputHeader(false);

	if (!compressor.process(input_options, compression_options, output_options)) {
		fatal("compress image error, call to \"compressor.process\" failed");
	}

	*compressed_data = output_handler.compressed_data;
	*compressed_data_mipmap_count = output_handler.compressed_data_mipmap_count;
	*compressed_data_size = output_handler.compressed_data_size;
}

void fbx_model_to_gpk(gpk_import_cmd_line cmdl) {
	printf("Importing model \"%s\" to \"%s\"\n", cmdl.asset_file, cmdl.import_file);
	FbxScene *fbx_scene = nullptr;
	{
		FbxManager *fbx_sdk_manager = FbxManager::Create();
		FbxIOSettings *fbx_io_settings = FbxIOSettings::Create(fbx_sdk_manager, IOSROOT);
		fbx_io_settings->SetBoolProp(IMP_ANIMATION, true);
		fbx_sdk_manager->SetIOSettings(fbx_io_settings);
		FbxImporter *fbx_importer = FbxImporter::Create(fbx_sdk_manager, "");
		if (!fbx_importer->Initialize(cmdl.asset_file, -1, fbx_sdk_manager->GetIOSettings())) {
			fatal("fbx_importer->Initialize failed, file: \"%s\"", cmdl.asset_file);
		}
		fbx_scene = FbxScene::Create(fbx_sdk_manager, "");
		fbx_importer->Import(fbx_scene);
		FbxGeometryConverter(fbx_sdk_manager).Triangulate(fbx_scene, true);
	}
	struct material *materials = (struct material *)calloc(gpk_model_max_material_count, sizeof(struct material));
	uint32 material_count = 0;
	{
		char materials_dir[512];
		snprintf(materials_dir, sizeof(materials_dir), "%s", cmdl.asset_file);
		string dir_str = {materials_dir, (uint32)strlen(materials_dir), sizeof(materials_dir)};
		string_pop_back(&dir_str, '\\', '/');
		string_cat(&dir_str, "materials\\*", sizeof("materials\\*") - 1);

		WIN32_FIND_DATA find_file_data = {};
		HANDLE find_file_handle = FindFirstFileA(materials_dir, &find_file_data);
		if (find_file_handle != INVALID_HANDLE_VALUE) {
			string_pop_back_n(&dir_str, 1);
			do {
				if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(find_file_data.cFileName, ".") && strcmp(find_file_data.cFileName, "..")) {
					{
						if (material_count >= gpk_model_max_material_count) {
							fatal("model has more material than max material count of %d", gpk_model_max_material_count);
						}
						material *material = &materials[material_count++];
						if (strlen(find_file_data.cFileName) >= sizeof(material::name)) {
							fatal("material name \"%s\" too long", find_file_data.cFileName);
						}
						strcpy(material->name, find_file_data.cFileName);

						int32 channel;
						string_cat(&dir_str, find_file_data.cFileName, (uint32)strlen(find_file_data.cFileName));

						string_cat(&dir_str, "\\albedo.png", (uint32)strlen("\\albedo.png"));
						uint8 *albedo_map_data = stbi_load(dir_str.buf, &material->albedo_map_width, &material->albedo_map_height, &channel, 4);
						if (albedo_map_data) {
							rgba_to_bgra(albedo_map_data, material->albedo_map_width, material->albedo_map_height);
							compress_image(image_type_albedo_map, albedo_map_data, material->albedo_map_width, material->albedo_map_height, &material->albedo_map_data, &material->albedo_map_mipmap_count, &material->albedo_map_size);
							stbi_image_free(albedo_map_data);
						}
						string_pop_back_n(&dir_str, (uint32)strlen("\\albedo.png"));

						string_cat(&dir_str, "\\normal.png", (uint32)strlen("\\normal.png"));
						uint8 *normal_map_data = stbi_load(dir_str.buf, &material->normal_map_width, &material->normal_map_height, &channel, 4);
						if (normal_map_data) {
							rgba_to_bgra(normal_map_data, material->normal_map_width, material->normal_map_height);
							compress_image(image_type_normal_map, normal_map_data, material->normal_map_width, material->normal_map_height, &material->normal_map_data, &material->normal_map_mipmap_count, &material->normal_map_size);
							stbi_image_free(normal_map_data);
						}
						string_pop_back_n(&dir_str, (uint32)strlen("\\normal.png"));

						string_cat(&dir_str, "\\metallic.png", (uint32)strlen("\\metallic.png"));
						uint8 *metallic_map_data = stbi_load(dir_str.buf, &material->metallic_map_width, &material->metallic_map_height, &channel, 4);
						if (metallic_map_data) {
							compress_image(image_type_metallic_map, metallic_map_data, material->metallic_map_width, material->metallic_map_height, &material->metallic_map_data, &material->metallic_map_mipmap_count, &material->metallic_map_size);
							stbi_image_free(metallic_map_data);
						}
						string_pop_back_n(&dir_str, (uint32)strlen("\\metallic.png"));

						string_cat(&dir_str, "\\roughness.png", (uint32)strlen("\\roughness.png"));
						uint8 *roughness_map_data = stbi_load(dir_str.buf, &material->roughness_map_width, &material->roughness_map_height, &channel, 4);
						if (roughness_map_data) {
							compress_image(image_type_roughness_map, roughness_map_data, material->roughness_map_width, material->roughness_map_height, &material->roughness_map_data, &material->roughness_map_mipmap_count, &material->roughness_map_size);
							stbi_image_free(roughness_map_data);
						}
						string_pop_back_n(&dir_str, (uint32)strlen("\\roughness.png"));

					// string_cat(&dir_str, "\\ao.png", (uint32)strlen("\\ao.png"));
					// material->ao_map_data = stbi_load(dir_str.buf, &material->ao_map_width, &material->ao_map_height, &channel, 4);
					// if (material->ao_map_data) {
					//   stbi_image_free(material->ao_map_data);
					// }
					// string_pop_back_n(&dir_str, (uint32)strlen("\\ao.png"));

						string_cat(&dir_str, "\\height.png", (uint32)strlen("\\height.png"));
						uint8 *height_map_data = stbi_load(dir_str.buf, &material->height_map_width, &material->height_map_height, &channel, 4);
						if (height_map_data) {
							for (uint32 i = 0; i < material->height_map_width * material->height_map_height * 4u; i += 1) {
								height_map_data[i] = 255 - height_map_data[i];
							}
							compress_image(image_type_height_map, height_map_data, material->height_map_width, material->height_map_height, &material->height_map_data, &material->height_map_mipmap_count, &material->height_map_size);
							stbi_image_free(height_map_data);
						}
						string_pop_back_n(&dir_str, (uint32)strlen("\\height.png"));

						string_pop_back_n(&dir_str, (uint32)strlen(find_file_data.cFileName));
					}
				}
			} while (FindNextFile(find_file_handle, &find_file_data));
			FindClose(find_file_handle);
		}
	}
	struct mesh *meshes = (struct mesh *)calloc(gpk_model_max_mesh_count, sizeof(struct mesh));
	uint32 mesh_count = 0;
	for (int32 i = 0; i < fbx_scene->GetNodeCount(); i += 1) {
		FbxNode *fbx_node = fbx_scene->GetNode(i);
		FbxMesh *fbx_mesh = fbx_node->GetMesh();
		if (!fbx_mesh) {
			continue;
		}
		bool mesh_instance = false;
		for (uint32 i = 0; i < mesh_count; i += 1) {
			if (meshes[i].fbx_mesh == fbx_mesh) {
				meshes[i].instances[meshes[i].instance_count].transform = fbx_mat4_to_mat4(&fbx_node->EvaluateGlobalTransform());
				meshes[i].instances[meshes[i].instance_count].bound = compute_mesh_bound(meshes[i].fbx_mesh, meshes[i].instances[meshes[i].instance_count].transform);
				meshes[i].instance_count += 1;
				mesh_instance = true;
				break;
			}
		}
		if (mesh_instance) {
			continue;
		}
		if (mesh_count >= gpk_model_max_mesh_count) {
			fatal("model has more mesh than max mesh count of %d", gpk_model_max_mesh_count);
		}
		for (int32 i = 0; i < fbx_mesh->GetPolygonCount(); i += 1) {
			if (fbx_mesh->GetPolygonSize(i) != 3) {
				fatal("non-triangle(%d vertices) mesh polygon found", fbx_mesh->GetPolygonSize(i));
			}
		}
		if (!fbx_mesh->GenerateNormals(true, true)) {
			fatal("generate normals failed\n");
		}
		if (!fbx_mesh->GenerateTangentsDataForAllUVSets(true)) {
			fatal("generate tangents failed\n");
		}
		struct mesh *mesh = &meshes[mesh_count++];
		mesh->fbx_mesh = fbx_mesh;
		if (strlen(fbx_node->GetName()) >= sizeof(mesh::name)) {
			fatal("mesh name \"%s\" is too long", fbx_node->GetName());
		}
		strcpy(mesh->name, fbx_node->GetName());
		mesh->instances[0].transform = fbx_mat4_to_mat4(&fbx_node->EvaluateGlobalTransform());
		mesh->instances[0].bound = compute_mesh_bound(mesh->fbx_mesh, mesh->instances[0].transform);
		mesh->instance_count = 1;
		uint32 polygon_vertex_count = fbx_mesh->GetPolygonCount() * 3;
		mesh->vertices = (struct vertex *)calloc(polygon_vertex_count, sizeof(struct vertex));
		mesh->vertex_count = 0;
		mesh->indices = (uint16 *)calloc(polygon_vertex_count, sizeof(uint16));
		mesh->index_count = polygon_vertex_count;
		mesh->material_index = UINT32_MAX;
		if (fbx_node->GetMaterialCount() > 0) {
			FbxSurfaceMaterial *fbx_surface_material = fbx_node->GetMaterial(0);
			const char *material_name = fbx_surface_material->GetName();
			for (uint32 i = 0; i < material_count; i += 1) {
				if (!strcmp(materials[i].name, material_name)) {
					mesh->material_index = i;
					break;
				}
			}
		}

		FbxVector4 *control_points = fbx_mesh->GetControlPoints();
		int32 *polygon_control_point_indices = fbx_mesh->GetPolygonVertices();

		FbxGeometryElementUV *polygon_element_uvs = fbx_mesh->GetElementUV("map1");
		FbxLayerElement::EMappingMode polygon_uvs_mapping = polygon_element_uvs->GetMappingMode();
		FbxLayerElement::EReferenceMode polygon_uvs_ref = polygon_element_uvs->GetReferenceMode();
		FbxLayerElementArrayTemplate<int32> &polygon_uv_indices = polygon_element_uvs->GetIndexArray();
		FbxLayerElementArrayTemplate<FbxVector2> &polygon_uvs = polygon_element_uvs->GetDirectArray();
		
		FbxGeometryElementNormal *polygon_element_normals = fbx_mesh->GetElementNormal();
		FbxLayerElement::EMappingMode polygon_normals_mapping = polygon_element_normals->GetMappingMode();
		FbxLayerElement::EReferenceMode polygon_normals_ref = polygon_element_normals->GetReferenceMode();
		FbxLayerElementArrayTemplate<int> &polygon_normal_indices = polygon_element_normals->GetIndexArray();
		FbxLayerElementArrayTemplate<FbxVector4> &polygon_normals = polygon_element_normals->GetDirectArray();

		FbxGeometryElementTangent *polygon_element_tangents = fbx_mesh->GetElementTangent();
		FbxLayerElement::EMappingMode polygon_tangents_mapping = polygon_element_tangents->GetMappingMode();
		FbxLayerElement::EReferenceMode polygon_tangents_ref = polygon_element_tangents->GetReferenceMode();
		FbxLayerElementArrayTemplate<int> &polygon_tangent_indices = polygon_element_tangents->GetIndexArray();
		FbxLayerElementArrayTemplate<FbxVector4> &polygon_tangents = polygon_element_tangents->GetDirectArray();

		if (polygon_uvs_mapping != FbxLayerElement::eByControlPoint && polygon_uvs_mapping != FbxLayerElement::eByPolygonVertex) {
			fatal("uvs bad mapping(\"%s\")", fbx_enum_to_string(polygon_uvs_mapping));
		}
		if (polygon_normals_mapping != FbxLayerElement::eByControlPoint && polygon_normals_mapping != FbxLayerElement::eByPolygonVertex) {
			fatal("normals bad mapping(\"%s\")", fbx_enum_to_string(polygon_normals_mapping));
		}
		if (polygon_tangents_mapping != FbxLayerElement::eByControlPoint && polygon_tangents_mapping != FbxLayerElement::eByPolygonVertex) {
			fatal("tangents bad mapping(\"%s\")", fbx_enum_to_string(polygon_tangents_mapping));
		}

		struct unique_control_point {
			struct {
				uv uv;
				normal normal;
				tangent tangent;
			} elements[32];
			uint16 element_count;
			uint16 prev_accumulate_element_count;
		};
		unique_control_point *unique_control_points = (struct unique_control_point *)calloc(fbx_mesh->GetControlPointsCount(), sizeof(struct unique_control_point));
		for (uint32 i = 0; i < polygon_vertex_count; i += 1) {
			int32 control_point_index = polygon_control_point_indices[i];
			unique_control_point *ucp = &unique_control_points[control_point_index];
			m_assert(ucp->element_count < m_countof(ucp->elements));
			FbxVector2 &fbx_uv = polygon_uvs[fbx_get_element_array_index(i, control_point_index, polygon_uvs_mapping, polygon_uvs_ref, polygon_uv_indices)];
			FbxVector4 &fbx_normal = polygon_normals[fbx_get_element_array_index(i, control_point_index, polygon_normals_mapping, polygon_normals_ref, polygon_normal_indices)];
			FbxVector4 &fbx_tangent = polygon_tangents[fbx_get_element_array_index(i, control_point_index, polygon_tangents_mapping, polygon_tangents_ref, polygon_tangent_indices)];
			uv uv = {(float)fbx_uv[0], (float)(1.0 - fbx_uv[1])};
			normal normal = {(int16)round(fbx_normal[0] * 32767.0), (int16)round(fbx_normal[1] * 32767.0), (int16)round(fbx_normal[2] * 32767.0)};
			tangent tangent = {(int16)round(fbx_tangent[0] * 32767.0), (int16)round(fbx_tangent[1] * 32767.0), (int16)round(fbx_tangent[2] * 32767.0)};
			uint16 element_index = ucp->element_count;
			for (uint16 i = 0; i < ucp->element_count; i += 1) {
				if (ucp->elements[i].uv == uv && ucp->elements[i].normal == normal && ucp->elements[i].tangent == tangent) {
					element_index = i;
					break;
				}
			}
			if (element_index == ucp->element_count) {
				ucp->elements[ucp->element_count++] = {uv, normal, tangent};
			}
			mesh->indices[i] = element_index;
		}
		uint16 prev_accumulate_element_count = 0;
		for (int32 i = 0; i < fbx_mesh->GetControlPointsCount(); i += 1) {
			position position = {(float)control_points[i][0], (float)control_points[i][1], (float)control_points[i][2]};
			unique_control_point *ucp = &unique_control_points[i];
			m_assert(ucp->element_count > 0);
			ucp->prev_accumulate_element_count = prev_accumulate_element_count;
			for (uint32 i = 0; i < ucp->element_count; i += 1) {
				mesh->vertices[prev_accumulate_element_count + i] = {position, ucp->elements[i].uv, ucp->elements[i].normal, ucp->elements[i].tangent};
			}
			prev_accumulate_element_count += ucp->element_count;
			mesh->vertex_count += ucp->element_count;
			m_assert(mesh->vertex_count < UINT16_MAX);
		}
		for (uint32 i = 0; i < polygon_vertex_count; i += 1) {
			mesh->indices[i] = unique_control_points[polygon_control_point_indices[i]].prev_accumulate_element_count + mesh->indices[i];
		}
	}
	gpk_model_header model_header = {};
	model_header.bound = compute_meshes_bound(meshes, mesh_count);
	gpk_model_mesh_header *mesh_headers = (struct gpk_model_mesh_header *)calloc(gpk_model_max_mesh_count, sizeof(struct gpk_model_mesh_header));
	gpk_model_material_header *material_headers = (struct gpk_model_material_header *)calloc(gpk_model_max_material_count, sizeof(struct gpk_model_material_header));
	uint32 model_total_size = 0;
	{
		model_total_size += sizeof(struct gpk_model_header);
		model_header.mesh_count = mesh_count;
		for (uint32 i = 0; i < mesh_count; i += 1) {
			model_total_size = round_up(model_total_size, 16);
			model_header.mesh_offsets[i] = model_total_size;
			mesh_headers[i].instances_offset = round_up((uint32)sizeof(struct gpk_model_mesh_header), 16);
			mesh_headers[i].instance_count = meshes[i].instance_count;
			mesh_headers[i].vertices_offset = round_up(mesh_headers[i].instances_offset + mesh_headers[i].instance_count * (uint32)sizeof(gpk_model_mesh_instance), 16);
			mesh_headers[i].vertex_size = sizeof(struct vertex);
			mesh_headers[i].vertex_count = meshes[i].vertex_count;
			mesh_headers[i].indices_offset = round_up(mesh_headers[i].vertices_offset + mesh_headers[i].vertex_size * mesh_headers[i].vertex_count, 16);
			mesh_headers[i].index_size = 2;
			mesh_headers[i].index_count = meshes[i].index_count;
			mesh_headers[i].material_index = meshes[i].material_index;
			array_copy(mesh_headers[i].name, meshes[i].name);
			model_total_size += mesh_headers[i].indices_offset + mesh_headers[i].index_size * mesh_headers[i].index_count;
		}
		model_header.material_count = material_count;
		for (uint32 i = 0; i < material_count; i += 1) {
			model_total_size = round_up(model_total_size, 16);
			model_header.material_offsets[i] = model_total_size;
			snprintf(material_headers[i].name, sizeof(material_headers[i].name), "%s", materials[i].name);
			uint32 material_size = round_up((uint32)sizeof(struct gpk_model_material_header), 16);
			if (materials[i].albedo_map_data) {
				material_headers[i].albedo_map_offset = round_up(material_size, 16);
				material_headers[i].albedo_map_width = materials[i].albedo_map_width;
				material_headers[i].albedo_map_height = materials[i].albedo_map_height;
				material_headers[i].albedo_map_mipmap_count = materials[i].albedo_map_mipmap_count;
				material_headers[i].albedo_map_size = materials[i].albedo_map_size;
				material_size = material_headers[i].albedo_map_offset + material_headers[i].albedo_map_size;
			}
			if (materials[i].normal_map_data) {
				material_headers[i].normal_map_offset = round_up(material_size, 16);
				material_headers[i].normal_map_width = materials[i].normal_map_width;
				material_headers[i].normal_map_height = materials[i].normal_map_height;
				material_headers[i].normal_map_mipmap_count = materials[i].normal_map_mipmap_count;
				material_headers[i].normal_map_size = materials[i].normal_map_size;
				material_size = material_headers[i].normal_map_offset + material_headers[i].normal_map_size;
			}
			if (materials[i].metallic_map_data) {
				material_headers[i].metallic_map_offset = round_up(material_size, 16);
				material_headers[i].metallic_map_width = materials[i].metallic_map_width;
				material_headers[i].metallic_map_height = materials[i].metallic_map_height;
				material_headers[i].metallic_map_mipmap_count = materials[i].metallic_map_mipmap_count;
				material_headers[i].metallic_map_size = materials[i].metallic_map_size;
				material_size = material_headers[i].metallic_map_offset + material_headers[i].metallic_map_size;
			}
			if (materials[i].roughness_map_data) {
				material_headers[i].roughness_map_offset = round_up(material_size, 16);
				material_headers[i].roughness_map_width = materials[i].roughness_map_width;
				material_headers[i].roughness_map_height = materials[i].roughness_map_height;
				material_headers[i].roughness_map_mipmap_count = materials[i].roughness_map_mipmap_count;
				material_headers[i].roughness_map_size = materials[i].roughness_map_size;
				material_size = material_headers[i].roughness_map_offset + material_headers[i].roughness_map_size;
			}
			if (materials[i].height_map_data) {
				material_headers[i].height_map_offset = round_up(material_size, 16);
				material_headers[i].height_map_width = materials[i].height_map_width;
				material_headers[i].height_map_height = materials[i].height_map_height;
				material_headers[i].height_map_mipmap_count = materials[i].height_map_mipmap_count;
				material_headers[i].height_map_size = materials[i].height_map_size;
				material_size = material_headers[i].height_map_offset + material_headers[i].height_map_size;
			}
			model_total_size += material_size;
		}
	}
	file_mapping import_file_mapping;
	if (!create_file_mapping(cmdl.import_file, model_total_size, &import_file_mapping)) {
		fatal("cannot create file mapping %s\n", cmdl.import_file);
	}
	*(gpk_model_header *)import_file_mapping.ptr = model_header;
	for (uint32 i = 0; i < model_header.mesh_count; i += 1) {
		struct mesh *mesh = &meshes[i];
		uint8 *header_ptr = import_file_mapping.ptr + model_header.mesh_offsets[i];
		uint8 *instances_ptr = import_file_mapping.ptr + model_header.mesh_offsets[i] + mesh_headers[i].instances_offset;
		uint8 *vertices_ptr = import_file_mapping.ptr + model_header.mesh_offsets[i] + mesh_headers[i].vertices_offset;
		uint8 *indices_ptr = import_file_mapping.ptr + model_header.mesh_offsets[i] + mesh_headers[i].indices_offset;
		*(gpk_model_mesh_header *)header_ptr = mesh_headers[i];
		memcpy(instances_ptr, mesh->instances, sizeof(struct gpk_model_mesh_instance) * mesh->instance_count);
		memcpy(vertices_ptr, mesh->vertices, sizeof(struct vertex) * mesh->vertex_count);
		memcpy(indices_ptr, mesh->indices, sizeof(uint16) * mesh->index_count);
	}
	for (uint32 i = 0; i < model_header.material_count; i += 1) {
		uint8 *header_ptr = import_file_mapping.ptr + model_header.material_offsets[i];
		*(gpk_model_material_header *)header_ptr = material_headers[i];
		if (materials[i].albedo_map_data) {
			uint8 *albedo_map_ptr = import_file_mapping.ptr + model_header.material_offsets[i] + material_headers[i].albedo_map_offset;
			memcpy(albedo_map_ptr, materials[i].albedo_map_data, materials[i].albedo_map_size);
		}
		if (materials[i].normal_map_data) {
			uint8 *normal_map_ptr = import_file_mapping.ptr + model_header.material_offsets[i] + material_headers[i].normal_map_offset;
			memcpy(normal_map_ptr, materials[i].normal_map_data, materials[i].normal_map_size);
		}
		if (materials[i].metallic_map_data) {
			uint8 *metallic_map_ptr = import_file_mapping.ptr + model_header.material_offsets[i] + material_headers[i].metallic_map_offset;
			memcpy(metallic_map_ptr, materials[i].metallic_map_data, materials[i].metallic_map_size);
		}
		if (materials[i].roughness_map_data) {
			uint8 *roughness_map_ptr = import_file_mapping.ptr + model_header.material_offsets[i] + material_headers[i].roughness_map_offset;
			memcpy(roughness_map_ptr, materials[i].roughness_map_data, materials[i].roughness_map_size);
		}
		if (materials[i].height_map_data) {
			uint8 *height_map_ptr = import_file_mapping.ptr + model_header.material_offsets[i] + material_headers[i].height_map_offset;
			memcpy(height_map_ptr, materials[i].height_map_data, materials[i].height_map_size);
		}
	}
	flush_file_mapping(import_file_mapping);
	close_file_mapping(import_file_mapping);
}

void fbx_mesh_to_text(const char *fbx_file) {
	printf("manually invoking fbx_mesh_to_text(\"%s\")", fbx_file);
	FbxScene *fbx_scene = nullptr;
	{
		FbxManager *fbx_sdk_manager = FbxManager::Create();
		FbxImporter *fbx_importer = FbxImporter::Create(fbx_sdk_manager, "");
		if (!fbx_importer->Initialize(fbx_file, -1, fbx_sdk_manager->GetIOSettings())) {
			fatal("fbx_importer->Initialize failed, file: \"%s\"", fbx_file);
		}
		fbx_scene = FbxScene::Create(fbx_sdk_manager, "");
		fbx_importer->Import(fbx_scene);
		FbxGeometryConverter(fbx_sdk_manager).Triangulate(fbx_scene, true);
	}
	FbxMesh *fbx_mesh = fbx_scene->GetSrcObject<FbxMesh>(0);
	if (!fbx_mesh) {
		fatal("error: cannot find a fbx mesh");
	}
	FILE *output_file = fopen("fbx_mesh_to_text.txt", "w");
	if (!output_file) {
		fatal("error: cannot create file fbx_mesh_to_text.txt");
	}
	FbxVector4 *control_points = fbx_mesh->GetControlPoints();
	int32 *polygon_control_point_indices = fbx_mesh->GetPolygonVertices();
	for (int32 i = 0; i < fbx_mesh->GetPolygonCount() * 3; i += 1) {
		FbxVector4 &cp = control_points[polygon_control_point_indices[i]];
		fprintf(output_file, "{%.3ff, %.3ff, %.3ff}, ", cp[0], cp[1], cp[2]);
		if (i % 3 == 2) {
			fprintf(output_file, "\n");
		}
	}
	fclose(output_file);
	printf("\ndone!");
}

void skybox_to_gpk(gpk_import_cmd_line cmdl) {
	console("Importing skybox \"%s\" to \"%s\"\n", cmdl.asset_file, cmdl.import_file);
	const char *cubemap_files[6] = {"left.png", "right.png", "up.png", "down.png", "front.png", "back.png"};
	uint8 *cubemap_data[6] = {};
	int32 cubemap_sizes[12] = {};
	for (uint32 i = 0; i < m_countof(cubemap_files); i += 1) {
		char asset_file[512];
		snprintf(asset_file, sizeof(asset_file), "%s\\%s", cmdl.asset_file, cubemap_files[i]);
		int32 channel;
    cubemap_data[i] = stbi_load(asset_file, &cubemap_sizes[i * 2], &cubemap_sizes[i * 2 + 1], &channel, 4);
    if (!cubemap_data[i]) {
    	fatal("import skybox failed to load cubemap image \"%s\"", cubemap_files[i]);
    }
	}
	for (uint32 i = 0; i < m_countof(cubemap_sizes); i += 1) {
		if (cubemap_sizes[i] != cubemap_sizes[0]) {
			fatal("import skybox invalid cubemap image size");
		}
	}
	uint32 cubemap_image_size = cubemap_sizes[0] * cubemap_sizes[1] * 4;
	uint32 cubemap_offset = round_up((uint32)sizeof(struct gpk_skybox_header), 16);
	uint32 file_size = cubemap_offset + cubemap_image_size * 6;
	file_mapping import_file_mapping;
	if (!create_file_mapping(cmdl.import_file, file_size, &import_file_mapping)) {
		fatal("cannot create file mapping %s\n", cmdl.import_file);
	}
	gpk_skybox_header *header = (gpk_skybox_header *)import_file_mapping.ptr;
	header->cubemap_offset = cubemap_offset;
	header->cubemap_width = cubemap_sizes[0];
	header->cubemap_height = cubemap_sizes[1];
	for (uint32 i = 0; i < 6; i += 1) {
		uint8 *dst = import_file_mapping.ptr + cubemap_offset + i * cubemap_image_size;
		memcpy(dst, cubemap_data[i], cubemap_image_size);
	}
	flush_file_mapping(import_file_mapping);
	close_file_mapping(import_file_mapping);
}

int main(int argc, char **argv) {
	set_exe_dir_as_current();
	if (argc == 4) {
		gpk_import_cmd_line cmdl = {};
		cmdl.import_type = (gpk_import_type)atoi(argv[1]);
		snprintf(cmdl.asset_file, sizeof(cmdl.asset_file), "%s", argv[2]);
		snprintf(cmdl.import_file, sizeof(cmdl.import_file), "%s", argv[3]);
		if (cmdl.import_type == gpk_import_type_fbx_model) {
			fbx_model_to_gpk(cmdl);
		}
		else if (cmdl.import_type == gpk_import_type_skybox) {
			skybox_to_gpk(cmdl);
		}
	}
	else if (argc == 3) {
		if (!strcmp(argv[1], "-manual_0")) {
			fbx_mesh_to_text(argv[2]);
		}
	}
	else {
		fatal("import.exe: invalid command line arguments");
	}
}

// FbxNode **fbx_nodes = nullptr;
// gpk_model_node *gpk_model_nodes = nullptr;
// uint32 gpk_model_nodes_count = fbx_scene->GetNodeCount();
// {
//   fbx_nodes = m_calloc(gpk_model_nodes_count, FbxNode *);
//   gpk_model_nodes = m_calloc(gpk_model_nodes_count, struct gpk_model_node);
//   for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
//     fbx_nodes[i] = fbx_scene->GetNode(i);
//   }
//   for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
//     FbxNode *fbx_node = fbx_nodes[i];
//     struct gpk_model_node gpk_model_node = {};
//     gpk_model_node.parent = -1;
//     gpk_model_node.local_transform = fbx_mat4_to_mat4(&(fbx_node->EvaluateLocalTransform()));
//     FbxNode *parent = fbx_node->GetParent();
//     for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
//       if (fbx_nodes[i] == parent) {
//         gpk_model_node.parent = i;
//         break;
//       }
//     }
//     gpk_model_nodes[i] = gpk_model_node;
//   }
// }
// gpk_model_animation_header *gpk_model_animation_headers = nullptr;
// gpk_model_animation_frame ***gpk_model_animation_frames = nullptr;
// uint32 gpk_model_animation_count = fbx_scene->GetSrcObjectCount<FbxAnimStack>();
// {
//   gpk_model_animation_headers = m_calloc(gpk_model_animation_count, struct gpk_model_animation_header);
//   gpk_model_animation_frames = m_calloc(gpk_model_animation_count, struct gpk_model_animation_frame **);
//   uint32 frames_count = 24;
//   for (uint32 i = 0; i < gpk_model_animation_count; i += 1) {
//     FbxAnimStack *fbx_anim_stack = fbx_scene->GetSrcObject<FbxAnimStack>(i);
//     FbxTimeSpan fbx_anim_time_span = fbx_anim_stack->GetLocalTimeSpan();
//     fbx_scene->SetCurrentAnimationStack(fbx_anim_stack);
//     FbxAnimEvaluator *fbx_anim_evaluator = fbx_scene->GetAnimationEvaluator();
//     double animation_duration = fbx_anim_time_span.GetDuration().GetSecondDouble();
//     double animation_frame_time = animation_duration / (frames_count - 1);
//     snprintf(gpk_model_animation_headers[i].name, sizeof(gpk_model_animation_headers[i].name), "%s", fbx_anim_stack->GetName());
//     gpk_model_animation_headers[i].duration = animation_duration;
//     gpk_model_animation_headers[i].frames_offset = (uint32)round_to_multi(sizeof(struct gpk_model_animation_header), 16);
//     gpk_model_animation_headers[i].frame_count = frames_count;
//     gpk_model_animation_headers[i].total_size = gpk_model_animation_headers[i].frames_offset + gpk_model_nodes_count * frames_count * sizeof(struct gpk_model_animation_frame);
//     gpk_model_animation_frame **gpk_model_animation_frames_1 = m_calloc(gpk_model_nodes_count, struct gpk_model_animation_frame *);
//     gpk_model_animation_frames[i] = gpk_model_animation_frames_1;
//     for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
//       gpk_model_animation_frame *gpk_model_animation_frames_2 = m_calloc(frames_count, struct gpk_model_animation_frame);
//       gpk_model_animation_frames_1[i] = gpk_model_animation_frames_2;
//       FbxNode *fbx_node = fbx_nodes[i];
//       double animation_current_time = 0;
//       for (uint32 i = 0; i < frames_count; i += 1) {
//         FbxTime fbx_time = {};
//         fbx_time.SetSecondDouble(animation_current_time);
//         FbxVector4 scaling = fbx_anim_evaluator->GetNodeLocalScaling(fbx_node, fbx_time);
//         FbxVector4 rotation = fbx_anim_evaluator->GetNodeLocalRotation(fbx_node, fbx_time);
//         FbxVector4 translation = fbx_anim_evaluator->GetNodeLocalTranslation(fbx_node, fbx_time);
//         gpk_model_animation_frames_2[i].scaling = {{(float)scaling.mData[0], (float)scaling.mData[1], (float)scaling.mData[2]}};
//         gpk_model_animation_frames_2[i].rotation = quat_from_euler_angles(degree_to_radian((float)rotation.mData[0]), degree_to_radian((float)rotation.mData[1]), degree_to_radian((float)rotation.mData[2]));
//         gpk_model_animation_frames_2[i].translation = {{(float)translation.mData[0], (float)translation.mData[1], (float)translation.mData[2]}};
//         gpk_model_animation_frames_2[i].time = animation_current_time;
//         animation_current_time += animation_frame_time;
//       }
//     }        
//   }      
// }
// gpk_image *gpk_model_images = m_calloc(fbx_scene->GetSrcObjectCount<FbxFileTexture>(), struct gpk_image);
// uint32 gpk_model_images_count = 0;
// gpk_model_mesh *gpk_model_meshes = m_calloc(gpk_model_nodes_count, struct gpk_model_mesh);
// uint32 gpk_model_meshes_count = 0;
// for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
//   FbxNode *fbx_mesh_node = fbx_nodes[i];
//   FbxMesh *fbx_mesh = fbx_mesh_node->GetMesh();
//   if (!fbx_mesh) {
//     continue;
//   }
//   bool mesh_previously_encountered = false;
//   for (uint32 i = 0; i < gpk_model_meshes_count; i += 1) {
//     if (gpk_model_meshes[i].fbx_mesh == fbx_mesh) {
//       if (gpk_model_meshes[i].header.instance_count > 1) {
//         m_error("multiple mesh instances found, not yet supported");
//       }
//       gpk_model_meshes[i].instances[gpk_model_meshes[i].header.instance_count++] = gpk_model_mesh_instance{fbx_mat4_to_mat4(&fbx_mesh_node->EvaluateGlobalTransform())};
//       mesh_previously_encountered = true;
//       break;
//     }
//   }
//   if (mesh_previously_encountered) {
//     m_error("multiple fbx mesh instances found, this is not yet supported");
//   }
//   struct gpk_model_mesh *new_mesh = &gpk_model_meshes[gpk_model_meshes_count++];
//   {
//     new_mesh->fbx_mesh = fbx_mesh;
//     new_mesh->header.instances_offset = (uint32)round_to_multi(sizeof(struct gpk_model_mesh_header), 16);
//     new_mesh->header.instance_count = 1;
//     new_mesh->header.vertex_size = 12 /*position*/ + 4 /*uv*/ + 4 /*normal*/;
//     new_mesh->instances = m_calloc(1, gpk_model_mesh_instance);
//     new_mesh->instances[0] = gpk_model_mesh_instance{fbx_mat4_to_mat4(&fbx_mesh_node->EvaluateGlobalTransform())};
//   }
//   {
//     if (fbx_mesh_node->GetMaterialCount() <= 0) {
//       m_error("fbx mesh has no material");
//     }
//     new_mesh->header.materials_offset = (uint32)round_to_multi(new_mesh->header.instances_offset + new_mesh->header.instance_count * sizeof(struct gpk_model_mesh_instance), 16);
//     new_mesh->header.material_count = fbx_mesh_node->GetMaterialCount();
//     new_mesh->materials = m_calloc(new_mesh->header.material_count, gpk_model_mesh_material);
//     for (uint32 i = 0; i < new_mesh->header.material_count; i += 1) {
//       FbxSurfaceMaterial *fbx_surface_material = fbx_mesh_node->GetMaterial(i);
//       gpk_model_mesh_material *material = &new_mesh->materials[i];
//       m_assert(strlen(fbx_surface_material->GetName()) < sizeof(material->name));
//       strcpy(material->name, fbx_surface_material->GetName());
//       FbxProperty fbx_diffuse_property = fbx_surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
//       if (!fbx_diffuse_property.IsValid()) {
//         m_error("fbx mesh surface material diffuse property not valid");
//       }
//       FbxFileTexture* fbx_diffuse_file_texture = fbx_diffuse_property.GetSrcObject<FbxFileTexture>(0);
//       if (fbx_diffuse_file_texture) {
//         material->diffuse_map_image_index = find_or_add_fbx_file_image(gpk_model_images, &gpk_model_images_count, fbx_diffuse_file_texture, gpk_image_format_b8g8r8a8_srgb);
//         material->diffuse_map_image_scales = vec2{(float)fbx_diffuse_file_texture->GetScaleU(), (float)fbx_diffuse_file_texture->GetScaleV()};
//       }
//       else {
//         m_error("fbx mesh surface diffuse material has no file texture");
//       }
//       FbxProperty fbx_normal_map_property = fbx_surface_material->FindProperty(FbxSurfaceMaterial::sNormalMap);
//       if (!fbx_normal_map_property.IsValid()) {
//         m_error("fbx mesh surface material normal map property not valid");
//       }
//       FbxFileTexture* fbx_normal_map_file_texture = fbx_normal_map_property.GetSrcObject<FbxFileTexture>(0);
//       if (fbx_normal_map_file_texture) {
//         material->normal_map_image_index = find_or_add_fbx_file_image(gpk_model_images, &gpk_model_images_count, fbx_normal_map_file_texture, gpk_image_format_b8g8r8a8, true);
//         material->normal_map_image_scales = vec2{(float)fbx_normal_map_file_texture->GetScaleU(), (float)fbx_normal_map_file_texture->GetScaleV()};
//       }
//       else {
//         material->normal_map_image_index = -1;
//       }
//     }
//   }
//   fbx_mesh->GenerateNormals();
//   FbxSkin *fbx_skin = (FbxSkin*)fbx_mesh->GetDeformer(0, FbxDeformer::EDeformerType::eSkin);
//   joint_weights *fbx_control_point_joint_weights = nullptr;
//   {
//     if (fbx_skin && fbx_skin->GetClusterCount() > 128) {
//       m_error("mesh with more than 128(%d) joints detected", fbx_skin->GetClusterCount());
//     }
//     new_mesh->header.joints_offset = (uint32)round_to_multi(new_mesh->header.materials_offset + new_mesh->header.material_count * sizeof(struct gpk_model_mesh_material), 16);
//     if (fbx_skin) {
//       new_mesh->header.joint_count = fbx_skin->GetClusterCount();
//       new_mesh->header.vertex_size += 12;
//       new_mesh->joints = m_calloc(new_mesh->header.joint_count, struct gpk_model_mesh_joint);
//       fbx_control_point_joint_weights = m_calloc(fbx_mesh->GetControlPointsCount(), struct joint_weights);
//       for (int i = 0; i < fbx_skin->GetClusterCount(); i += 1) {
//         uint8 joint_index = i;
//         FbxCluster *fbx_cluster = fbx_skin->GetCluster(i);
//         FbxNode *fbx_cluster_node = fbx_cluster->GetLink();
//         uint32 fbx_cluster_node_index = -1;
//         for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
//           if (fbx_nodes[i] == fbx_cluster_node) {
//             fbx_cluster_node_index = i;
//             break;
//           }
//         }
//         m_assert(fbx_cluster_node_index != -1);
//         FbxAMatrix geometry_mat = FbxAMatrix(fbx_cluster_node->GetGeometricTranslation(FbxNode::eSourcePivot), fbx_mesh_node->GetGeometricRotation(FbxNode::eSourcePivot), fbx_cluster_node->GetGeometricScaling(FbxNode::eSourcePivot));
//         FbxAMatrix transform_mat;
//         FbxAMatrix transform_link_mat;
//         fbx_cluster->GetTransformMatrix(transform_mat);
//         fbx_cluster->GetTransformLinkMatrix(transform_link_mat);
//         FbxAMatrix global_inv_bind_pose_mat = transform_link_mat.Inverse() * transform_mat * geometry_mat;
//         new_mesh->joints[i] = gpk_model_mesh_joint{fbx_cluster_node_index, fbx_mat4_to_mat4(&global_inv_bind_pose_mat)};
//         int *control_point_indices = fbx_cluster->GetControlPointIndices();
//         double *control_point_weights = fbx_cluster->GetControlPointWeights();
//         int control_points_count = fbx_cluster->GetControlPointIndicesCount();
//         for (int i = 0; i < control_points_count; i += 1) {
//           int index = control_point_indices[i];
//           float weight = (float)control_point_weights[i];
//           if (weight == 0.0f) {
//             m_error("control point joint has 0 weight");
//           }
//           joint_weights *joint_weights = &fbx_control_point_joint_weights[index];
//           bool found = false;
//           for (uint32 i = 0; i < 4; i += 1) {
//             if (joint_weights->weights[i] == 0.0f) {
//               joint_weights->weights[i] = weight;
//               joint_weights->joint_indices[i] = joint_index;
//               found = true;
//               break;
//             }
//           }
//           if (!found) {
//             m_error("control point with too many joint weights(> 4)");
//           }
//         }
//       }    
//     }
//     else {
//       if (gpk_model_animation_count != 0) {
//         gpk_model_animation_count = 0;
//       }
//     }
//   }
//   uint32 polygons_count = fbx_mesh->GetPolygonCount();
//   {
//     for (uint32 i = 0; i < polygons_count; i += 1) {
//       if (fbx_mesh->GetPolygonSize(i) != 3) {
//         m_error("fbx model polygon count %d is not a triangle(%d vertices)", i, fbx_mesh->GetPolygonSize(i));
//       }
//     }
//     new_mesh->header.vertices_offset = (uint32)round_to_multi(new_mesh->header.joints_offset + new_mesh->header.joint_count * sizeof(struct gpk_model_mesh_joint), 16);
//     new_mesh->header.vertex_count = polygons_count * 3;
//     new_mesh->header.total_size = new_mesh->header.vertices_offset + new_mesh->header.vertex_count * new_mesh->header.vertex_size;
//     new_mesh->vertices_positions = m_calloc(polygons_count * 3, union vec3);
//     new_mesh->vertices_uvs = m_calloc(polygons_count * 3, uv);
//     new_mesh->vertices_normals = m_calloc(polygons_count * 3, uint32);
//     new_mesh->vertices_joint_weights = fbx_skin ? m_calloc(polygons_count * 3, struct joint_weights) : nullptr;
//   }
//   FbxVector4 *fbx_control_points = fbx_mesh->GetControlPoints();
//   int *polygon_control_point_indices = {};
//   FbxArray<FbxVector2> polygon_uvs = {};
//   FbxArray<FbxVector4> polygon_normals = {};
//   polygon_control_point_indices = fbx_mesh->GetPolygonVertices();
//   fbx_mesh->GetPolygonVertexNormals(polygon_normals);
//   fbx_mesh->GetPolygonVertexUVs("map1", polygon_uvs);
//   for (uint32 i = 0; i < polygons_count * 3; i += 1) {
//     FbxVector4 control_point = fbx_control_points[polygon_control_point_indices[i]];
//     FbxVector2 uv = polygon_uvs[i];
//     FbxVector4 normal = polygon_normals[i];
//     new_mesh->vertices_positions[i] = vec3{{(float)control_point.mData[0], (float)control_point.mData[1], (float)control_point.mData[2]}};
//     new_mesh->vertices_uvs[i] = {float_to_half((float)uv.mData[0]), float_to_half(1.0f - (float)uv.mData[1])};
//     new_mesh->vertices_normals[i] = pack_snorm_3x10_1x2((float)normal.mData[0], (float)normal.mData[1], (float)normal.mData[2], 0.0f);
//     if (fbx_skin) {
//       joint_weights *joint_weights_src = &fbx_control_point_joint_weights[polygon_control_point_indices[i]];
//       joint_weights *joint_weights_dst = &new_mesh->vertices_joint_weights[i];
//       *joint_weights_dst = *joint_weights_src;
//       for (uint32 i = 0; i < 4; i += 1) {
//         joint_weights_dst->half_float_weights[i] = float_to_half(joint_weights_dst->weights[i]);
//       }
//     }
//   }
// }
// for (uint32 i = 0; i < gpk_model_meshes_count; i += 1) {
//   if (gpk_model_meshes[i].header.vertex_size != gpk_model_meshes[0].header.vertex_size) {
//     m_error("model mesh has different vertex size");
//   }
// }
// struct gpk_model_header gpk_model_header = {};
// {
//   gpk_model_header.nodes_offset = (uint32)round_to_multi(sizeof(struct gpk_model_header), 16);
//   gpk_model_header.node_count = gpk_model_nodes_count;
//   uint32 offset = (uint32)round_to_multi(gpk_model_header.nodes_offset + gpk_model_header.node_count * sizeof(struct gpk_model_node), 16);
//   gpk_model_header.animation_count = gpk_model_animation_count;
//   m_assert(gpk_model_header.animation_count < m_countof(gpk_model_header.animation_offsets));
//   for (uint32 i = 0; i < gpk_model_animation_count; i += 1) {
//     gpk_model_header.animation_offsets[i] = offset;
//     offset = round_to_multi(offset + gpk_model_animation_headers[i].total_size, 16);
//   }
//   gpk_model_header.image_count = gpk_model_images_count;
//   m_assert(gpk_model_header.image_count < m_countof(gpk_model_header.image_offsets));
//   for (uint32 i = 0; i < gpk_model_images_count; i += 1) {
//     gpk_model_header.image_offsets[i] = offset;
//     offset = round_to_multi(offset + gpk_model_images[i].header.total_size, 16);
//   }
//   gpk_model_header.mesh_count = gpk_model_meshes_count;
//   m_assert(gpk_model_header.mesh_count < m_countof(gpk_model_header.mesh_offsets));
//   for (uint32 i = 0; i < gpk_model_meshes_count; i += 1) {
//     gpk_model_header.mesh_offsets[i] = offset;
//     offset = round_to_multi(offset + gpk_model_meshes[i].header.total_size, 16);
//   }
//   gpk_model_header.total_size = offset;
// }
// {
//   file_mapping import_file_mapping = {};
//   create_file_mapping(cmdl.import_file_name, gpk_model_header.total_size, &import_file_mapping);
//   *(struct gpk_model_header *)import_file_mapping.ptr = gpk_model_header;
//   memcpy(import_file_mapping.ptr + gpk_model_header.nodes_offset, gpk_model_nodes, gpk_model_header.node_count * sizeof(struct gpk_model_node));
//   for (uint32 i = 0; i < gpk_model_header.animation_count; i += 1) {
//     uint8 *animation_ptr = import_file_mapping.ptr + gpk_model_header.animation_offsets[i];
//     *(struct gpk_model_animation_header *)animation_ptr = gpk_model_animation_headers[i];
//     animation_ptr += gpk_model_animation_headers[i].frames_offset;
//     uint32 frames_count = gpk_model_animation_headers[i].frame_count;
//     gpk_model_animation_frame **frames = gpk_model_animation_frames[i];
//     for (uint32 i = 0; i < gpk_model_header.node_count; i += 1) {
//       memcpy(animation_ptr, frames[i], frames_count * sizeof(struct gpk_model_animation_frame));
//       animation_ptr += frames_count * sizeof(struct gpk_model_animation_frame);
//     }
//   }
//   for (uint32 i = 0; i < gpk_model_header.image_count; i += 1) {
//     uint8 *image_ptr = import_file_mapping.ptr + gpk_model_header.image_offsets[i];
//     *(struct gpk_image_header *)image_ptr = gpk_model_images[i].header;
//     memcpy(image_ptr + gpk_model_images[i].header.image_offset, gpk_model_images[i].image_data, gpk_model_images[i].header.image_size);
//   }
//   for (uint32 i = 0; i < gpk_model_header.mesh_count; i += 1) {
//     struct gpk_model_mesh *mesh = &gpk_model_meshes[i];
//     uint8 *mesh_ptr = import_file_mapping.ptr + gpk_model_header.mesh_offsets[i];
//     *(struct gpk_model_mesh_header *)mesh_ptr = mesh->header;
//     gpk_model_mesh_instance *instances = (gpk_model_mesh_instance *)(mesh_ptr + mesh->header.instances_offset);
//     for (uint32 i = 0; i < mesh->header.instance_count; i += 1) {
//       instances[i] = mesh->instances[i];
//     }
//     gpk_model_mesh_material *materials = (gpk_model_mesh_material *)(mesh_ptr + mesh->header.materials_offset);
//     for (uint32 i = 0; i < mesh->header.material_count; i += 1) {
//       materials[i] = mesh->materials[i];
//     }
//     gpk_model_mesh_joint *joints = (gpk_model_mesh_joint *)(mesh_ptr + mesh->header.joints_offset);
//     for (uint32 i = 0; i < mesh->header.joint_count; i += 1) {
//       joints[i] = mesh->joints[i];
//     }
//     uint8 *vertices = mesh_ptr + mesh->header.vertices_offset;
//     for (uint32 i = 0; i < mesh->header.vertex_count; i += 1) {
//       *(vec3 *)vertices = mesh->vertices_positions[i];
//       vertices += 12;
//       ((uint16 *)vertices)[0] = mesh->vertices_uvs[i].u;
//       ((uint16 *)vertices)[1] = mesh->vertices_uvs[i].v;
//       vertices += 4;
//       *(uint32 *)vertices = mesh->vertices_normals[i];
//       vertices += 4;
//       if (mesh->vertices_joint_weights) {
//         memcpy(vertices, mesh->vertices_joint_weights[i].joint_indices, 4);
//         vertices += 4;
//         memcpy(vertices, mesh->vertices_joint_weights[i].half_float_weights, 8);
//         vertices += 8;
//       }
//     }
//   }
//   close_file_mapping(import_file_mapping);
//   m_done("import model \"%s\" succeeded", cmdl.import_file_name);
// }

