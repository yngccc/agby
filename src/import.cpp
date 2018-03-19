/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"

#define NVTT_SHARED 1
#include "../vendor/include/nvtt/nvtt.h"

#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#include "../vendor/include/tinygltf/tiny_gltf.h"

#include "math.cpp"
#include "gpk.cpp"
#include <vulkan/vulkan.h>

uint32 tinygltf_wrap_to_vk_wrap(int32 wrap) {
	if (wrap == TINYGLTF_TEXTURE_WRAP_REPEAT) {
		return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	}
	else if (wrap == TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT) {
		return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	}
	else if (wrap == TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE) {
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	}
	else {
		m_assert(false);
		return UINT32_MAX;
	}
};

void rgba_to_bgra(uint8 *image_data, uint32 image_width, uint32 image_height) {
	for (uint32 i = 0; i < image_width * image_height; i += 1) {
		uint8 r = image_data[i * 4];
		image_data[i * 4] = image_data[i * 4 + 2];
		image_data[i * 4 + 2] = r;
	}
}

enum compress_image_type {
	compress_image_type_color,
	compress_image_type_grayscale,
	compress_image_type_normal_map
};

void compress_image(compress_image_type image_type, uint8 *data, uint32 width, uint32 height,
										uint8 **compressed_data, uint32 *compressed_data_mipmap_count, uint32 *compressed_data_size) {
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
	input_options.setNormalMap(image_type == compress_image_type_normal_map);
	m_assert(input_options.setMipmapData(data, width, height));
	if (image_type == compress_image_type_color) {
		compression_options.setFormat(Format_BC1);
	}
	else if (image_type == compress_image_type_grayscale) {
		input_options.setGamma(1.0f, 1.0f);
		compression_options.setFormat(Format_BC4);
	}
	else if (image_type == compress_image_type_normal_map) {
		compression_options.setFormat(Format_BC5);
	}
	else {
		m_assert(false);
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

	m_assert(compressor.process(input_options, compression_options, output_options));

	*compressed_data = output_handler.compressed_data;
	*compressed_data_mipmap_count = output_handler.compressed_data_mipmap_count;
	*compressed_data_size = output_handler.compressed_data_size;
}

void skybox_to_gpk(std::string skybox_dir, std::string gpk_file) {
	printf("begin importing skybox: \"%s\"\n", skybox_dir.c_str());

	const char *cubemap_files[6] = {"left.png", "right.png", "up.png", "down.png", "front.png", "back.png"};
	uint8 *cubemap_data[6] = {};
	int32 cubemap_sizes[12] = {};
	for (uint32 i = 0; i < m_countof(cubemap_files); i += 1) {
		std::string png_file = skybox_dir + "\\" + cubemap_files[i];
		int32 channel = 0;
    cubemap_data[i] = stbi_load(png_file.c_str(), &cubemap_sizes[i * 2], &cubemap_sizes[i * 2 + 1], &channel, 4);
    m_assert(cubemap_data[i]);
	}
	for (uint32 i = 0; i < m_countof(cubemap_sizes); i += 1) {
		m_assert(cubemap_sizes[i] == cubemap_sizes[0]);
	}
	uint32 cubemap_image_size = cubemap_sizes[0] * cubemap_sizes[1] * 4;
	uint32 cubemap_offset = round_up((uint32)sizeof(struct gpk_skybox), 16u);
	uint32 file_size = cubemap_offset + cubemap_image_size * 6;
	file_mapping import_file_mapping;
	m_assert(create_file_mapping(gpk_file.c_str(), file_size, &import_file_mapping));
	gpk_skybox *header = (gpk_skybox *)import_file_mapping.ptr;
	*header = {"GPK_SKYBOX_FORMAT"};
	header->cubemap_offset = cubemap_offset;
	header->cubemap_width = cubemap_sizes[0];
	header->cubemap_height = cubemap_sizes[1];
	for (uint32 i = 0; i < 6; i += 1) {
		uint8 *dst = import_file_mapping.ptr + cubemap_offset + i * cubemap_image_size;
		memcpy(dst, cubemap_data[i], cubemap_image_size);
	}
	flush_file_mapping(import_file_mapping);
	close_file_mapping(import_file_mapping);

	printf("done importing skybox: \"%s\"\n", gpk_file.c_str());
}

void glb_to_gpk(std::string glb_file, std::string gpk_file) {
	printf("begin importing glb: \"%s\" \n", glb_file.c_str());

	tinygltf::Model glb_model;
	{
		tinygltf::TinyGLTF glb_loader;
		std::string glb_loader_err;
		bool glb_loader_ret = glb_loader.LoadBinaryFromFile(&glb_model, &glb_loader_err, glb_file);
		if (!glb_loader_err.empty()) {
			printf("tinygltf err: %s\n", glb_loader_err.c_str());
		}
		m_assert(glb_loader_ret);
	}
	gpk_model gpk_model = {"GPK_MODEL_FORMAT"};
	uint32 current_offset = round_up((uint32)sizeof(gpk_model), 16u);
	std::vector<gpk_model_scene> gpk_model_scenes;
	{
		gpk_model.scene_offset = current_offset;
		gpk_model.scene_count = (uint32)glb_model.scenes.size();
		m_assert(gpk_model.scene_count > 0);
		gpk_model_scenes.resize(gpk_model.scene_count);
		for (uint32 i = 0; i < gpk_model.scene_count; i += 1) {
			auto &src = glb_model.scenes[i];
			auto &dst = gpk_model_scenes[i];
			m_assert(src.name.length() < sizeof(dst.name));
			strcpy(dst.name, src.name.c_str());
			dst.node_index_count = (uint32)src.nodes.size();
			m_assert(dst.node_index_count <= m_countof(dst.node_indices));
			for (uint32 i = 0; i < dst.node_index_count; i += 1) {
				dst.node_indices[i] = (uint32)src.nodes[i];
			}
		}
		current_offset = round_up(current_offset + gpk_model.scene_count * (uint32)sizeof(struct gpk_model_scene), 16u);
	}
	std::vector<gpk_model_node> gpk_model_nodes;
	{
		gpk_model.node_offset = current_offset;
		gpk_model.node_count = (uint32)glb_model.nodes.size();
		m_assert(gpk_model.node_count > 0);
		gpk_model_nodes.resize(gpk_model.node_count);
		for (uint32 i = 0; i < gpk_model.node_count; i += 1) {
			auto &src = glb_model.nodes[i];
			auto &dst = gpk_model_nodes[i];
			dst.mesh_index = (src.mesh >= 0 && src.mesh < glb_model.meshes.size()) ? src.mesh : UINT32_MAX;
			dst.skin_index = (src.skin >= 0 && src.skin < glb_model.skins.size()) ? src.skin : UINT32_MAX;
			dst.child_count = (uint32)src.children.size();
			m_assert(dst.child_count <= m_countof(dst.children));
			for (uint32 i = 0; i < dst.child_count; i += 1) {
				m_assert(src.children[i] >= 0 && src.children[i] < glb_model.nodes.size());
				dst.children[i] = src.children[i];
			}
			if (src.matrix.size() != 0) {
				for (uint32 i = 0; i < 16; i += 1) {
					dst.local_transform_mat[i / 4][i % 4] = (float)src.matrix[i];
				}
			}
			else {
				transform transform = transform_identity();
				if (src.rotation.size() != 0) {
					transform.rotate = {(float)src.rotation[0], (float)src.rotation[1], (float)src.rotation[2], (float)src.rotation[3]};
				}
				if (src.scale.size() != 0) {
					transform.scale =  {(float)src.scale[0], (float)src.scale[1], (float)src.scale[2]};
				}
				if (src.translation.size() != 0) {
					transform.translate = {(float)src.translation[0], (float)src.translation[1], (float)src.translation[2]};
				}
				dst.local_transform_mat = transform_to_mat4(transform);
			}
		}
		current_offset = round_up(current_offset + gpk_model.node_count * (uint32)sizeof(struct gpk_model_node), 16u);
	}
	std::vector<gpk_model_mesh> gpk_model_meshes;
	{
		gpk_model.mesh_offset = current_offset;
		gpk_model.mesh_count = (uint32)glb_model.meshes.size();
		m_assert(gpk_model.mesh_count > 0);
		gpk_model_meshes.resize(gpk_model.mesh_count);
		for (uint32 i = 0; i < gpk_model.mesh_count; i += 1) {
			auto &src = glb_model.meshes[i];
			auto &dst = gpk_model_meshes[i];
			m_assert(src.name.length() < sizeof(dst.name));
			strcpy(dst.name, src.name.c_str());
			m_assert(src.primitives.size() == 1);
			auto &primitive = src.primitives[0];
			m_assert(primitive.mode == TINYGLTF_MODE_TRIANGLES);
			m_assert(primitive.indices >= 0);
			m_assert(primitive.attributes.find("POSITION") != primitive.attributes.end());
			m_assert(primitive.attributes.find("NORMAL") != primitive.attributes.end());
			dst.material_index = primitive.material >= 0 ? primitive.material : UINT32_MAX;
		}
		current_offset = round_up(current_offset + gpk_model.mesh_count * (uint32)sizeof(struct gpk_model_mesh), 16u);
	}
	std::vector<gpk_model_skin> gpk_model_skins;
	{
		gpk_model.skin_offset = current_offset;
		gpk_model.skin_count = (uint32)glb_model.skins.size();
		gpk_model_skins.resize(gpk_model.skin_count);
		m_assert(gpk_model.skin_count <= 1);
		for (uint32 i = 0; i < gpk_model.skin_count; i += 1) {
			auto &src = glb_model.skins[i];
			auto &dst = gpk_model_skins[i];
			m_assert(src.name.length() < sizeof(dst.name));
			strcpy(dst.name, src.name.c_str());
			m_assert(src.skeleton >= 0 && src.skeleton < glb_model.nodes.size());
			dst.root_node_index = (uint32)src.skeleton;
			m_assert(src.joints.size() > 0 && src.joints.size() < glb_model.nodes.size());
			m_assert(src.joints.size() < 256);
			dst.joint_count = (uint32)src.joints.size();
		}
		current_offset = round_up(current_offset + gpk_model.skin_count * (uint32)sizeof(struct gpk_model_skin), 16u);
	}
	struct image_remap {
		uint32 index;
		bool is_base_color;
		bool is_metallic_roughness;
		bool is_normal;
	};
	std::vector<image_remap> image_remaps;
	uint32 image_count = 0;
	{
		image_remaps.resize(glb_model.images.size(), {});
		for (auto &m : glb_model.materials) {
			auto base_color_texture = m.values.find("baseColorTexture");
			if (base_color_texture != m.values.end()) {
				int32 texture_index = base_color_texture->second.TextureIndex();
				m_assert(texture_index >= 0 && texture_index < glb_model.textures.size());
				int32 image_index = glb_model.textures[texture_index].source;
				m_assert(image_index >= 0 && image_index < glb_model.images.size());
				image_remaps[image_index].is_base_color = true;
			}

			auto metallic_roughness_texture = m.values.find("metallicRoughnessTexture");
			if (metallic_roughness_texture != m.values.end()) {
				int32 texture_index = metallic_roughness_texture->second.TextureIndex();
				m_assert(texture_index >= 0 && texture_index < glb_model.textures.size());
				int32 image_index = glb_model.textures[texture_index].source;
				m_assert(image_index >= 0 && image_index < glb_model.images.size());
				image_remaps[image_index].is_metallic_roughness = true;
			}

			auto normal_texture = m.additionalValues.find("normalTexture");
			if (normal_texture != m.additionalValues.end()) {
				int32 texture_index = normal_texture->second.TextureIndex();
				m_assert(texture_index >= 0 && texture_index < glb_model.textures.size());
				int32 image_index = glb_model.textures[texture_index].source;
				m_assert(image_index >= 0 && image_index < glb_model.images.size());
				image_remaps[image_index].is_normal = true;
			}
		}
		for (auto &remap : image_remaps) {
			int32 is = (int32)remap.is_base_color + (int32)remap.is_metallic_roughness + (int32)remap.is_normal;
			m_assert(is == 0 || is == 1);
			if (is == 0) {
				remap.index = UINT32_MAX;
			}
			else if (is == 1) {
				remap.index = image_count;
				image_count += remap.is_metallic_roughness ? 2 : 1;
			}
		}
	}
	std::vector<gpk_model_material> gpk_model_materials;
	{
		gpk_model.material_offset = current_offset;
		gpk_model.material_count = (uint32)glb_model.materials.size();
		gpk_model_materials.resize(gpk_model.material_count);
		for (uint32 i = 0; i < gpk_model.material_count; i += 1) {
			gpk_model_materials[i].albedo_image_index = UINT32_MAX;
			gpk_model_materials[i].albedo_factor = {0.8f, 0.8f, 0.8f, 1.0f};
			gpk_model_materials[i].metallic_image_index = UINT32_MAX;
			gpk_model_materials[i].roughness_image_index = UINT32_MAX;
			gpk_model_materials[i].metallic_factor = 0.5f;
			gpk_model_materials[i].roughness_factor = 0.5f;
			gpk_model_materials[i].normal_image_index = UINT32_MAX;
		}
		for (uint32 i = 0; i < gpk_model.material_count; i += 1) {
			auto &src = glb_model.materials[i];
			auto &dst = gpk_model_materials[i];
			m_assert(src.name.length() < sizeof(dst.name));
			strcpy(dst.name, src.name.c_str());
			auto base_color_texture = src.values.find("baseColorTexture");
			auto base_color_factor = src.values.find("baseColorFactor");
			auto metallic_roughness_texture = src.values.find("metallicRoughnessTexture");
			auto metallic_factor = src.values.find("metallicFactor");
			auto roughness_factor = src.values.find("roughnessFactor");
			auto normal_texture = src.additionalValues.find("normalTexture");
			if (base_color_texture != src.values.end()) {
				int32 index = base_color_texture->second.TextureIndex();
				m_assert(index >= 0 && index < glb_model.textures.size());
				auto &texture = glb_model.textures[index];
				m_assert(texture.source >= 0 && texture.source < glb_model.images.size());
				m_assert(image_remaps[texture.source].is_base_color);
				dst.albedo_image_index = image_remaps[texture.source].index;
				if (texture.sampler >= 0 && texture.sampler < glb_model.samplers.size()) {
					dst.albedo_image_wrap_s = tinygltf_wrap_to_vk_wrap(glb_model.samplers[texture.sampler].wrapS);
					dst.albedo_image_wrap_t = tinygltf_wrap_to_vk_wrap(glb_model.samplers[texture.sampler].wrapT);
				}
				else {
					dst.albedo_image_wrap_s = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					dst.albedo_image_wrap_t = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				}
			}
			else if (base_color_factor != src.values.end()) {
				auto &color = base_color_factor->second.number_array;
				m_assert(color.size() == 4);
				dst.albedo_factor = {(float)color[0], (float)color[1], (float)color[2], (float)color[3]};
			}
			if (metallic_roughness_texture != src.values.end()) {
				int32 index = metallic_roughness_texture->second.TextureIndex();
				m_assert(index >= 0 && index < glb_model.textures.size());
				auto &texture = glb_model.textures[index];
				m_assert(texture.source >= 0 && texture.source < glb_model.images.size());
				m_assert(image_remaps[texture.source].is_metallic_roughness);
				dst.metallic_image_index = image_remaps[texture.source].index;
				dst.roughness_image_index = image_remaps[texture.source].index + 1;
				if (texture.sampler >= 0 && texture.sampler < glb_model.samplers.size()) {
					dst.metallic_roughness_image_wrap_s = tinygltf_wrap_to_vk_wrap(glb_model.samplers[texture.sampler].wrapS);
					dst.metallic_roughness_image_wrap_t = tinygltf_wrap_to_vk_wrap(glb_model.samplers[texture.sampler].wrapT);
				}
				else {
					dst.metallic_roughness_image_wrap_s = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					dst.metallic_roughness_image_wrap_t = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				}
			}
			else {
				if (metallic_factor != src.values.end()) {
					auto &factor = metallic_factor->second.number_array;
					m_assert(factor.size() == 1);
					dst.metallic_factor = (float)factor[0];
				}
				if (roughness_factor != src.values.end()) {
					auto &factor = roughness_factor->second.number_array;
					m_assert(factor.size() == 1);
					dst.roughness_factor = (float)factor[0];
				}
			}
			if (normal_texture != src.additionalValues.end()) {
				int32 index = normal_texture->second.TextureIndex();
				m_assert(index >= 0 && index < glb_model.textures.size());
				auto &texture = glb_model.textures[index];
				m_assert(texture.source >= 0 && texture.source < glb_model.images.size());
				m_assert(image_remaps[texture.source].is_normal);
				dst.normal_image_index = image_remaps[texture.source].index;
				if (texture.sampler >= 0 && texture.sampler < glb_model.samplers.size()) {
					dst.normal_image_wrap_s = tinygltf_wrap_to_vk_wrap(glb_model.samplers[texture.sampler].wrapS);
					dst.normal_image_wrap_t = tinygltf_wrap_to_vk_wrap(glb_model.samplers[texture.sampler].wrapT);
				}
				else {
					dst.normal_image_wrap_s = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					dst.normal_image_wrap_t = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				}
			}
		}
		current_offset = round_up(current_offset + gpk_model.material_count * (uint32)sizeof(struct gpk_model_material), 16u);
	}
	std::vector<gpk_model_image> gpk_model_images;
	{
		gpk_model.image_offset = current_offset;
		gpk_model.image_count = image_count;
		gpk_model_images.resize(image_count);
		uint32 image_index = 0;
		for (size_t i = 0; i < glb_model.images.size(); i += 1) {
			auto &image = glb_model.images[i];
			auto &remap = image_remaps[i];
			if (remap.index != UINT32_MAX) {
				m_assert(image.width >= 4 && image.width % 4 == 0);
				m_assert(image.height >= 4 && image.height % 4 == 0);
				m_assert(image.component >= 1 && image.component <= 4);
				if (remap.is_base_color) {
					auto &gpk_image = gpk_model_images[image_index++];
					gpk_image.width = image.width;
					gpk_image.height = image.height;
					gpk_image.mipmap_count = 1;
					gpk_image.layer_count = 1;
					gpk_image.size = image.width * image.height * 4;
					gpk_image.format = VK_FORMAT_R8G8B8A8_SRGB;
					gpk_image.format_block_dimension = 1;
					gpk_image.format_block_size = 4;
				}
				else if (remap.is_metallic_roughness) {
					auto &gpk_metallic_image = gpk_model_images[image_index++];
					gpk_metallic_image.width = image.width;
					gpk_metallic_image.height = image.height;
					gpk_metallic_image.mipmap_count = 1;
					gpk_metallic_image.layer_count = 1;
					gpk_metallic_image.size = image.width * image.height;
					gpk_metallic_image.format = VK_FORMAT_R8_UNORM;
					gpk_metallic_image.format_block_dimension = 1;
					gpk_metallic_image.format_block_size = 1;
					auto &gpk_roughness_image = gpk_model_images[image_index++];
					gpk_roughness_image.width = image.width;
					gpk_roughness_image.height = image.height;
					gpk_roughness_image.mipmap_count = 1;
					gpk_roughness_image.layer_count = 1;
					gpk_roughness_image.size = image.width * image.height;
					gpk_roughness_image.format = VK_FORMAT_R8_UNORM;
					gpk_roughness_image.format_block_dimension = 1;
					gpk_roughness_image.format_block_size = 1;
				}
				else if (remap.is_normal) {
					auto &gpk_image = gpk_model_images[image_index++];
					gpk_image.width = image.width;
					gpk_image.height = image.height;
					gpk_image.mipmap_count = 1;
					gpk_image.layer_count = 1;
					gpk_image.size = image.width * image.height * 4;
					gpk_image.format = VK_FORMAT_R8G8B8A8_UNORM;
					gpk_image.format_block_dimension = 1;
					gpk_image.format_block_size = 4;
				}
				else {
					m_assert(false);
				}
			}
		}
		current_offset = round_up(current_offset + gpk_model.image_count * (uint32)sizeof(struct gpk_model_image), 16u);
	}
	for (uint32 i = 0; i < gpk_model.mesh_count; i += 1) {
		auto &src = glb_model.meshes[i];
		auto &dst = gpk_model_meshes[i];
		auto &primitive = src.primitives[0];

		auto &index_accessor = glb_model.accessors[primitive.indices];
		m_assert(index_accessor.count > 0);
		m_assert(index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT || index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE);
		m_assert(index_accessor.type == TINYGLTF_TYPE_SCALAR);
		
		auto &position_accessor = glb_model.accessors[primitive.attributes["POSITION"]];
		m_assert(position_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		m_assert(position_accessor.type == TINYGLTF_TYPE_VEC3);

		auto &normal_accessor = glb_model.accessors[primitive.attributes["NORMAL"]];
		m_assert(normal_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		m_assert(normal_accessor.type == TINYGLTF_TYPE_VEC3);
		m_assert(normal_accessor.count == position_accessor.count);

		if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
			auto &uv_accessor = glb_model.accessors[primitive.attributes["TEXCOORD_0"]];
			m_assert(uv_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(uv_accessor.type == TINYGLTF_TYPE_VEC2);
			m_assert(uv_accessor.count == position_accessor.count);
		}

		if (primitive.attributes.find("TANGENT") != primitive.attributes.end()) {
			auto &tangent_accessor = glb_model.accessors[primitive.attributes["TANGENT"]];
			m_assert(tangent_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(tangent_accessor.type == TINYGLTF_TYPE_VEC4);
			m_assert(tangent_accessor.count == position_accessor.count);
		}

		if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
			auto weights = primitive.attributes.find("WEIGHTS_0");
			m_assert(weights != primitive.attributes.end());
			auto &joint_accessor = glb_model.accessors[primitive.attributes["JOINTS_0"]];
			m_assert(joint_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
			m_assert(joint_accessor.type == TINYGLTF_TYPE_VEC4);
			m_assert(joint_accessor.count == position_accessor.count);

			auto &weight_accessor = glb_model.accessors[primitive.attributes["WEIGHTS_0"]];
			m_assert(weight_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(weight_accessor.type == TINYGLTF_TYPE_VEC4);
			m_assert(weight_accessor.count == position_accessor.count);
		}
		else {
			m_assert(glb_model.skins.size() == 0);
		}

		dst.indices_offset = current_offset;
		dst.index_count = (uint32)index_accessor.count;
		current_offset = round_up(current_offset + dst.index_count * (uint32)sizeof(uint16), 16u);
		dst.vertices_offset = current_offset;
		dst.vertex_count = (uint32)position_accessor.count;
		current_offset = round_up(current_offset + dst.vertex_count * (uint32)sizeof(struct gpk_model_vertex), 16u);
	}
	for (uint32 i = 0; i < gpk_model.skin_count; i += 1) {
		auto & skin = gpk_model_skins[i];
		skin.joints_offset = current_offset;
		current_offset = round_up(current_offset + skin.joint_count * (uint32)sizeof(struct gpk_model_joint), 16u);
	}
	for (uint32 i = 0; i < gpk_model.image_count; i += 1) {
		auto &image = gpk_model_images[i];
		image.data_offset = current_offset;
		current_offset = round_up(current_offset + image.size, 16u);
	}
	
	file_mapping gpk_file_mapping = {};
	m_assert(create_file_mapping(gpk_file.c_str(), current_offset, &gpk_file_mapping));
	*(struct gpk_model *)gpk_file_mapping.ptr = gpk_model;
	memcpy(gpk_file_mapping.ptr + gpk_model.scene_offset, &gpk_model_scenes[0], gpk_model_scenes.size() * sizeof(struct gpk_model_scene));
	memcpy(gpk_file_mapping.ptr + gpk_model.node_offset, &gpk_model_nodes[0], gpk_model_nodes.size() * sizeof(struct gpk_model_node));
	memcpy(gpk_file_mapping.ptr + gpk_model.mesh_offset, &gpk_model_meshes[0], gpk_model_meshes.size() * sizeof(struct gpk_model_mesh));
	memcpy(gpk_file_mapping.ptr + gpk_model.skin_offset, &gpk_model_skins[0], gpk_model_skins.size() * sizeof(struct gpk_model_skin));
	memcpy(gpk_file_mapping.ptr + gpk_model.material_offset, &gpk_model_materials[0], gpk_model_materials.size() * sizeof(struct gpk_model_material));
	memcpy(gpk_file_mapping.ptr + gpk_model.image_offset, &gpk_model_images[0], gpk_model_images.size() * sizeof(struct gpk_model_image));
	for (uint32 i = 0; i < gpk_model.mesh_count; i += 1) {
		auto &primitive = glb_model.meshes[i].primitives[0];
		auto &mesh = gpk_model_meshes[i];

		auto &index_accessor = glb_model.accessors[primitive.indices];
		auto &index_buffer_view = glb_model.bufferViews[index_accessor.bufferView];
		auto &index_buffer = glb_model.buffers[index_buffer_view.buffer];
		unsigned char *index_data = &index_buffer.data[index_buffer_view.byteOffset + index_accessor.byteOffset];
		m_assert(index_buffer_view.byteStride == 0);
		if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
			memcpy(gpk_file_mapping.ptr + mesh.indices_offset, index_data, mesh.index_count * sizeof(uint16));
		}
		if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
			for (uint32 i = 0; i < mesh.index_count; i += 1) {
				((uint16 *)(gpk_file_mapping.ptr + mesh.indices_offset))[i] = index_data[i];
			}
		}
		
		auto &position_accessor = glb_model.accessors[primitive.attributes["POSITION"]];
		auto &position_buffer_view = glb_model.bufferViews[position_accessor.bufferView];
		auto &position_buffer = glb_model.buffers[position_buffer_view.buffer];
		uint32 position_stride = position_buffer_view.byteStride == 0 ? 12 : (uint32)position_buffer_view.byteStride;
		unsigned char *position_data = &position_buffer.data[position_buffer_view.byteOffset + position_accessor.byteOffset];
		
		auto &normal_accessor = glb_model.accessors[primitive.attributes["NORMAL"]];
		auto &normal_buffer_view = glb_model.bufferViews[normal_accessor.bufferView];
		auto &normal_buffer = glb_model.buffers[normal_buffer_view.buffer];
		uint32 normal_stride = normal_buffer_view.byteStride == 0 ? 12 : (uint32)normal_buffer_view.byteStride;
		unsigned char *normal_data = &normal_buffer.data[normal_buffer_view.byteOffset + normal_accessor.byteOffset];

		uint8 *uv_data = nullptr;
		uint32 uv_stride = 0;
		if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
			auto &uv_accessor = glb_model.accessors[primitive.attributes["TEXCOORD_0"]];
			auto &uv_buffer_view = glb_model.bufferViews[uv_accessor.bufferView];
			auto &uv_buffer = glb_model.buffers[uv_buffer_view.buffer];
			uv_stride = uv_buffer_view.byteStride == 0 ? 8 : (uint32)uv_buffer_view.byteStride;
			uv_data = &uv_buffer.data[uv_buffer_view.byteOffset + uv_accessor.byteOffset];
		}

		uint8 *tangent_data = nullptr;
		uint32 tangent_stride = 0;
		if (primitive.attributes.find("TANGENT") != primitive.attributes.end()) {
			auto &tangent_accessor = glb_model.accessors[primitive.attributes["TANGENT"]];
			auto &tangent_buffer_view = glb_model.bufferViews[tangent_accessor.bufferView];
			auto &tangent_buffer = glb_model.buffers[tangent_buffer_view.buffer];
			tangent_stride = tangent_buffer_view.byteStride == 0 ? 16 : (uint32)tangent_buffer_view.byteStride;
			tangent_data = &tangent_buffer.data[tangent_buffer_view.byteOffset + tangent_accessor.byteOffset];
		}

		uint8 *joint_data = nullptr;
		uint32 joint_stride = 0;
		uint8 *weight_data = nullptr;
		uint32 weight_stride = 0;
		uint32 skin_joint_count = 0;
		if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
			auto &joint_accessor = glb_model.accessors[primitive.attributes["JOINTS_0"]];
			auto &joint_buffer_view = glb_model.bufferViews[joint_accessor.bufferView];
			auto &joint_buffer = glb_model.buffers[joint_buffer_view.buffer];
			joint_stride = joint_buffer_view.byteStride == 0 ? 8 : (uint32)joint_buffer_view.byteStride;
			joint_data = &joint_buffer.data[joint_buffer_view.byteOffset + joint_accessor.byteOffset];

			auto &weight_accessor = glb_model.accessors[primitive.attributes["WEIGHTS_0"]];
			auto &weight_buffer_view = glb_model.bufferViews[weight_accessor.bufferView];
			auto &weight_buffer = glb_model.buffers[weight_buffer_view.buffer];
			weight_stride = weight_buffer_view.byteStride == 0 ? 16 : (uint32)weight_buffer_view.byteStride;
			weight_data = &weight_buffer.data[weight_buffer_view.byteOffset + weight_accessor.byteOffset];

			skin_joint_count = (uint32)glb_model.skins[0].joints.size();
			m_assert(skin_joint_count < 256);
		}
		
		for (uint32 i = 0; i < mesh.vertex_count; i += 1) {
			gpk_model_vertex *vertex = ((gpk_model_vertex *)(gpk_file_mapping.ptr + mesh.vertices_offset)) + i;

			vertex->position = *(vec3 *)(position_data + position_stride * i);

			if (uv_data) {
				vertex->uv = *(vec2 *)(uv_data + uv_stride * i);
			}
			else {
				vertex->uv = {};
			}

			vec3 normal = *(vec3 *)(normal_data + normal_stride * i);
			vertex->normal = {(int16)roundf(normal[0] * 32767.0f), (int16)roundf(normal[1] * 32767.0f), (int16)roundf(normal[2] * 32767.0f)};

			vec3 tangent = {};
			if (tangent_data) {
				tangent = *(vec3 *)(tangent_data + tangent_stride * i);
			}
			else {
				vec3 tangent1 = vec3_cross(normal, vec3{0, 0, 1});
				vec3 tangent2 = vec3_cross(normal, vec3{0, 1, 0});
				tangent = vec3_normalize(vec3_len(tangent1) > vec3_len(tangent2) ? tangent1 : tangent2);
			}
			vertex->tangent = {(int16)roundf(tangent[0] * 32767.0f), (int16)roundf(tangent[1] * 32767.0f), (int16)roundf(tangent[2] * 32767.0f)};

			if (joint_data) {
				u16vec4 js = *(u16vec4 *)(joint_data + joint_stride * i);
				m_assert(js[0] < skin_joint_count && js[1] < skin_joint_count && js[2] < skin_joint_count && js[3] < skin_joint_count);
				vertex->joints = {(uint8)js[0], (uint8)js[1], (uint8)js[2], (uint8)js[3]};

				vec4 ws = *(vec4 *)(weight_data + weight_stride * i);
				m_assert(ws[0] >= 0 && ws[1] >= 0 && ws[2] >= 0 && ws[3] >= 0);
				m_assert(ws[0] <= 1 && ws[1] <= 1 && ws[2] <= 1 && ws[3] <= 1);
				vertex->weights = {(uint16)roundf(ws[0] * 65535.0f), (uint16)roundf(ws[1] * 65535.0f), (uint16)roundf(ws[2] * 65535.0f), (uint16)roundf(ws[3] * 65535.0f)};
			}
			else {
				vertex->joints = {0, 0, 0, 0};
				vertex->weights = {UINT16_MAX, 0, 0, 0};
			}
		}
	}
	for (uint32 i = 0; i < gpk_model.skin_count; i += 1) {
		auto &skin = glb_model.skins[i];
		mat4 *inverse_bind_mats = nullptr;
		{
			auto &accessor = glb_model.accessors[skin.inverseBindMatrices];
			m_assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(accessor.type == TINYGLTF_TYPE_MAT4);
			m_assert(accessor.count == skin.joints.size());
			auto &buffer_view = glb_model.bufferViews[accessor.bufferView];
			m_assert(buffer_view.byteStride == 0 || buffer_view.byteStride == sizeof(mat4));
			auto &buffer = glb_model.buffers[buffer_view.buffer];
			inverse_bind_mats = (mat4 *)(&buffer.data[accessor.byteOffset + buffer_view.byteOffset]);
		}
		auto &gpk_skin = gpk_model_skins[i];
		gpk_model_joint *gpk_joints = (gpk_model_joint *)(gpk_file_mapping.ptr + gpk_skin.joints_offset);
		m_assert(gpk_skin.joint_count == skin.joints.size());
		for (uint32 i = 0; i < gpk_skin.joint_count; i += 1) {
			m_assert(skin.joints[i] >= 0 && skin.joints[i] < glb_model.nodes.size());
			gpk_joints[i].node_index = (uint32)skin.joints[i];
			gpk_joints[i].inverse_bind_mat = inverse_bind_mats[i];
		}
	}
	uint32 image_index = 0;
	for (size_t i = 0; i < glb_model.images.size(); i += 1) {
		auto &image = glb_model.images[i];
		auto &remap = image_remaps[i];
		if (remap.index != UINT32_MAX) {
			if (remap.is_base_color) {
				auto &gpk_image = gpk_model_images[image_index++];
				uint8 *gpk_ptr = gpk_file_mapping.ptr + gpk_image.data_offset;
				m_assert(image.component == 3 || image.component == 4);
				for (uint32 i = 0; i < gpk_image.width * gpk_image.height; i += 1) {
					gpk_ptr[i * 4 + 0] = image.image[i * image.component + 0];
					gpk_ptr[i * 4 + 1] = image.image[i * image.component + 1];
					gpk_ptr[i * 4 + 2] = image.image[i * image.component + 2];
					gpk_ptr[i * 4 + 3] = 255;
				}
			}
			else if (remap.is_metallic_roughness) {
				auto &gpk_metallic_image = gpk_model_images[image_index++];
				auto &gpk_roughness_image = gpk_model_images[image_index++];
				uint8 *gpk_metallic_ptr = gpk_file_mapping.ptr + gpk_metallic_image.data_offset;
				uint8 *gpk_roughness_ptr = gpk_file_mapping.ptr + gpk_roughness_image.data_offset;
				m_assert(image.component == 3 || image.component == 4);
				for (uint32 i = 0; i < gpk_metallic_image.width * gpk_metallic_image.height; i += 1) {
					gpk_metallic_ptr[i] = image.image[i * image.component + 2];
				}
				for (uint32 i = 0; i < gpk_roughness_image.width * gpk_roughness_image.height; i += 1) {
					gpk_roughness_ptr[i] = image.image[i * image.component + 1];
				}
			}
			else if (remap.is_normal) {
				auto &gpk_image = gpk_model_images[image_index++];
				uint8 *gpk_ptr = gpk_file_mapping.ptr + gpk_image.data_offset;
				m_assert(image.component == 3 || image.component == 4);
				for (uint32 i = 0; i < gpk_image.width * gpk_image.height; i += 1) {
					gpk_ptr[i * 4 + 0] = image.image[i * image.component + 0];
					gpk_ptr[i * 4 + 1] = image.image[i * image.component + 1];
					gpk_ptr[i * 4 + 2] = image.image[i * image.component + 2];
					gpk_ptr[i * 4 + 3] = 255;
				}
			}
			else {
				m_assert(false);
			}
		}
	}
	flush_file_mapping(gpk_file_mapping);
	close_file_mapping(gpk_file_mapping);
	
	printf("done importing glb: \"%s\" %d\n", glb_file.c_str(), current_offset);
}

struct import_json_schema {
	bool force_import_all;
	bool force_import_models;
	bool force_import_skyboxes;
	struct model {
		bool import;
		std::string glb_file;
		std::string gpk_file;
	};
	std::vector<model> models;
	struct skybox {
		bool import;
		std::string dir;
		std::string gpk_file;
	};
	std::vector<skybox> skyboxes;
};

void from_json(const nlohmann::json &j, import_json_schema &import) {
	import.force_import_all = j["force_import_all"];
	import.force_import_models = j["force_import_models"];
	import.force_import_skyboxes = j["force_import_skyboxes"];
	for (auto &m : j["models"]) {
		import.models.push_back({m["import"], m["glb_file"], m["gpk_file"]});
	}
	for (auto &s : j["skyboxes"]) {
		import.skyboxes.push_back({s["import"], s["dir"], s["gpk_file"]});
	}
}

void import_json(std::string json_file) {
	printf("begin importing json file: \"%s\" \n", json_file.c_str());

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
	uint32 job_count = 0;

	auto create_import_process = [job_object](std::string cmdl_str) {
		STARTUPINFOA startup_info = {sizeof(startup_info)};
		startup_info.dwFlags = STARTF_USESHOWWINDOW;
		startup_info.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION process_info;
		m_assert(CreateProcessA("import.exe", (LPSTR)cmdl_str.c_str(), nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &startup_info, &process_info));
		m_assert(AssignProcessToJobObject(job_object, process_info.hProcess));
		ResumeThread(process_info.hThread);
		CloseHandle(process_info.hThread);
		CloseHandle(process_info.hProcess);
	};

	std::ifstream json_file_fstream(json_file.c_str());
	nlohmann::json json_content;
	json_file_fstream >> json_content;
	import_json_schema import = json_content;
	for (auto &m : import.models) {
		if (m.import || import.force_import_all || import.force_import_models) {
			std::string cmdl_str = std::string("import.exe -glb-to-gpk ") + m.glb_file + " " + m.gpk_file;
			create_import_process(cmdl_str);
			job_count += 1;
		}
	}
	for (auto &s : import.skyboxes) {
		if (s.import || import.force_import_all || import.force_import_skyboxes) {
			std::string cmdl_str = std::string("import.exe -skybox-to-gpk ") + s.dir + " " + s.gpk_file;
			create_import_process(cmdl_str);
			job_count += 1;
		}
	}

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
	printf("done importing json file: \"%s\" \n", json_file.c_str());
}

int main(int argc, char **argv) {
	set_exe_dir_as_current();
	if (argc < 2) {
		printf("error: missing first argument");
	}
	else {
		const char *mode_str = argv[1];
		if (!strcmp(mode_str, "-glb-to-gpk")) {
			if (argc == 4) {
				glb_to_gpk(argv[2], argv[3]);
			}
			else {
				printf("error: expect -glb-to-gpk glb_file gpk_file");
			}
		}
		else if (!strcmp(mode_str, "-skybox-to-gpk")) {
			if (argc == 4) {
				skybox_to_gpk(argv[2], argv[3]);
			}
			else {
				printf("error: expect -skybox-to-gpk skybox_dir gpk_file");
			}
		}
		else if (!strcmp(mode_str, "-import-json")) {
			if (argc == 3) {
				import_json(argv[2]);
			}
			else {
				printf("error: expect -import-json json_file");
			}
		}
		else {
			printf("import.exe error: unknown first argument");
		}
	}
}

#if 0
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
				meshes[i].instances[meshes[i].instance_count].transform_mat = fbx_mat4_to_mat4(&fbx_node->EvaluateGlobalTransform());
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
		if (!fbx_mesh->GenerateNormals(false, true)) {
			fatal("generate normals failed\n");
		}
		if (!fbx_mesh->GenerateTangentsDataForAllUVSets(false)) {
			fatal("generate tangents failed\n");
		}
		struct mesh *mesh = &meshes[mesh_count++];
		mesh->fbx_mesh = fbx_mesh;
		if (strlen(fbx_node->GetName()) >= sizeof(mesh::name)) {
			fatal("mesh name \"%s\" is too long", fbx_node->GetName());
		}
		strcpy(mesh->name, fbx_node->GetName());
		mesh->instances[0].transform_mat = fbx_mat4_to_mat4(&fbx_node->EvaluateGlobalTransform());
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
	gpk_model_mesh_header *mesh_headers = (struct gpk_model_mesh_header *)calloc(gpk_model_max_mesh_count, sizeof(struct gpk_model_mesh_header));
	gpk_model_material_header *material_headers = (struct gpk_model_material_header *)calloc(gpk_model_max_material_count, sizeof(struct gpk_model_material_header));
	uint32 model_total_size = 0;
	{
		model_total_size += sizeof(struct gpk_model_header);
		model_header.mesh_count = mesh_count;
		for (uint32 i = 0; i < mesh_count; i += 1) {
			round_up(&model_total_size, 16u);
			model_header.mesh_offsets[i] = model_total_size;
			mesh_headers[i].instances_offset = round_up((uint32)sizeof(struct gpk_model_mesh_header), 16u);
			mesh_headers[i].instance_count = meshes[i].instance_count;
			mesh_headers[i].vertices_offset = round_up(mesh_headers[i].instances_offset + mesh_headers[i].instance_count * (uint32)sizeof(gpk_model_mesh_instance), 16u);
			mesh_headers[i].vertex_size = sizeof(struct vertex);
			mesh_headers[i].vertex_count = meshes[i].vertex_count;
			mesh_headers[i].indices_offset = round_up(mesh_headers[i].vertices_offset + mesh_headers[i].vertex_size * mesh_headers[i].vertex_count, 16u);
			mesh_headers[i].index_size = 2;
			mesh_headers[i].index_count = meshes[i].index_count;
			mesh_headers[i].material_index = meshes[i].material_index;
			array_copy(mesh_headers[i].name, meshes[i].name);
			model_total_size += mesh_headers[i].indices_offset + mesh_headers[i].index_size * mesh_headers[i].index_count;
		}
		model_header.material_count = material_count;
		for (uint32 i = 0; i < material_count; i += 1) {
			round_up(&model_total_size, 16u);
			model_header.material_offsets[i] = model_total_size;
			snprintf(material_headers[i].name, sizeof(material_headers[i].name), "%s", materials[i].name);
			uint32 material_size = round_up((uint32)sizeof(struct gpk_model_material_header), 16u);
			if (materials[i].albedo_map_data) {
				material_headers[i].albedo_map_offset = round_up(material_size, 16u);
				material_headers[i].albedo_map_width = materials[i].albedo_map_width;
				material_headers[i].albedo_map_height = materials[i].albedo_map_height;
				material_headers[i].albedo_map_mipmap_count = materials[i].albedo_map_mipmap_count;
				material_headers[i].albedo_map_size = materials[i].albedo_map_size;
				material_size = material_headers[i].albedo_map_offset + material_headers[i].albedo_map_size;
			}
			if (materials[i].normal_map_data) {
				material_headers[i].normal_map_offset = round_up(material_size, 16u);
				material_headers[i].normal_map_width = materials[i].normal_map_width;
				material_headers[i].normal_map_height = materials[i].normal_map_height;
				material_headers[i].normal_map_mipmap_count = materials[i].normal_map_mipmap_count;
				material_headers[i].normal_map_size = materials[i].normal_map_size;
				material_size = material_headers[i].normal_map_offset + material_headers[i].normal_map_size;
			}
			if (materials[i].metallic_map_data) {
				material_headers[i].metallic_map_offset = round_up(material_size, 16u);
				material_headers[i].metallic_map_width = materials[i].metallic_map_width;
				material_headers[i].metallic_map_height = materials[i].metallic_map_height;
				material_headers[i].metallic_map_mipmap_count = materials[i].metallic_map_mipmap_count;
				material_headers[i].metallic_map_size = materials[i].metallic_map_size;
				material_size = material_headers[i].metallic_map_offset + material_headers[i].metallic_map_size;
			}
			if (materials[i].roughness_map_data) {
				material_headers[i].roughness_map_offset = round_up(material_size, 16u);
				material_headers[i].roughness_map_width = materials[i].roughness_map_width;
				material_headers[i].roughness_map_height = materials[i].roughness_map_height;
				material_headers[i].roughness_map_mipmap_count = materials[i].roughness_map_mipmap_count;
				material_headers[i].roughness_map_size = materials[i].roughness_map_size;
				material_size = material_headers[i].roughness_map_offset + material_headers[i].roughness_map_size;
			}
			if (materials[i].height_map_data) {
				material_headers[i].height_map_offset = round_up(material_size, 16u);
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
#endif

#if 0
FbxNode **fbx_nodes = nullptr;
gpk_model_node *gpk_model_nodes = nullptr;
uint32 gpk_model_nodes_count = fbx_scene->GetNodeCount();
{
  fbx_nodes = m_calloc(gpk_model_nodes_count, FbxNode *);
  gpk_model_nodes = m_calloc(gpk_model_nodes_count, struct gpk_model_node);
  for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
    fbx_nodes[i] = fbx_scene->GetNode(i);
  }
  for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
    FbxNode *fbx_node = fbx_nodes[i];
    struct gpk_model_node gpk_model_node = {};
    gpk_model_node.parent = -1;
    gpk_model_node.local_transform = fbx_mat4_to_mat4(&(fbx_node->EvaluateLocalTransform()));
    FbxNode *parent = fbx_node->GetParent();
    for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
      if (fbx_nodes[i] == parent) {
        gpk_model_node.parent = i;
        break;
      }
    }
    gpk_model_nodes[i] = gpk_model_node;
  }
}
gpk_model_animation_header *gpk_model_animation_headers = nullptr;
gpk_model_animation_frame ***gpk_model_animation_frames = nullptr;
uint32 gpk_model_animation_count = fbx_scene->GetSrcObjectCount<FbxAnimStack>();
{
  gpk_model_animation_headers = m_calloc(gpk_model_animation_count, struct gpk_model_animation_header);
  gpk_model_animation_frames = m_calloc(gpk_model_animation_count, struct gpk_model_animation_frame **);
  uint32 frames_count = 24;
  for (uint32 i = 0; i < gpk_model_animation_count; i += 1) {
    FbxAnimStack *fbx_anim_stack = fbx_scene->GetSrcObject<FbxAnimStack>(i);
    FbxTimeSpan fbx_anim_time_span = fbx_anim_stack->GetLocalTimeSpan();
    fbx_scene->SetCurrentAnimationStack(fbx_anim_stack);
    FbxAnimEvaluator *fbx_anim_evaluator = fbx_scene->GetAnimationEvaluator();
    double animation_duration = fbx_anim_time_span.GetDuration().GetSecondDouble();
    double animation_frame_time = animation_duration / (frames_count - 1);
    snprintf(gpk_model_animation_headers[i].name, sizeof(gpk_model_animation_headers[i].name), "%s", fbx_anim_stack->GetName());
    gpk_model_animation_headers[i].duration = animation_duration;
    gpk_model_animation_headers[i].frames_offset = (uint32)round_to_multi(sizeof(struct gpk_model_animation_header), 16);
    gpk_model_animation_headers[i].frame_count = frames_count;
    gpk_model_animation_headers[i].total_size = gpk_model_animation_headers[i].frames_offset + gpk_model_nodes_count * frames_count * sizeof(struct gpk_model_animation_frame);
    gpk_model_animation_frame **gpk_model_animation_frames_1 = m_calloc(gpk_model_nodes_count, struct gpk_model_animation_frame *);
    gpk_model_animation_frames[i] = gpk_model_animation_frames_1;
    for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
      gpk_model_animation_frame *gpk_model_animation_frames_2 = m_calloc(frames_count, struct gpk_model_animation_frame);
      gpk_model_animation_frames_1[i] = gpk_model_animation_frames_2;
      FbxNode *fbx_node = fbx_nodes[i];
      double animation_current_time = 0;
      for (uint32 i = 0; i < frames_count; i += 1) {
        FbxTime fbx_time = {};
        fbx_time.SetSecondDouble(animation_current_time);
        FbxVector4 scaling = fbx_anim_evaluator->GetNodeLocalScaling(fbx_node, fbx_time);
        FbxVector4 rotation = fbx_anim_evaluator->GetNodeLocalRotation(fbx_node, fbx_time);
        FbxVector4 translation = fbx_anim_evaluator->GetNodeLocalTranslation(fbx_node, fbx_time);
        gpk_model_animation_frames_2[i].scaling = {{(float)scaling.mData[0], (float)scaling.mData[1], (float)scaling.mData[2]}};
        gpk_model_animation_frames_2[i].rotation = quat_from_euler_angles(degree_to_radian((float)rotation.mData[0]), degree_to_radian((float)rotation.mData[1]), degree_to_radian((float)rotation.mData[2]));
        gpk_model_animation_frames_2[i].translation = {{(float)translation.mData[0], (float)translation.mData[1], (float)translation.mData[2]}};
        gpk_model_animation_frames_2[i].time = animation_current_time;
        animation_current_time += animation_frame_time;
      }
    }        
  }      
}
gpk_image *gpk_model_images = m_calloc(fbx_scene->GetSrcObjectCount<FbxFileTexture>(), struct gpk_image);
uint32 gpk_model_images_count = 0;
gpk_model_mesh *gpk_model_meshes = m_calloc(gpk_model_nodes_count, struct gpk_model_mesh);
uint32 gpk_model_meshes_count = 0;
for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
  FbxNode *fbx_mesh_node = fbx_nodes[i];
  FbxMesh *fbx_mesh = fbx_mesh_node->GetMesh();
  if (!fbx_mesh) {
    continue;
  }
  bool mesh_previously_encountered = false;
  for (uint32 i = 0; i < gpk_model_meshes_count; i += 1) {
    if (gpk_model_meshes[i].fbx_mesh == fbx_mesh) {
      if (gpk_model_meshes[i].header.instance_count > 1) {
        m_error("multiple mesh instances found, not yet supported");
      }
      gpk_model_meshes[i].instances[gpk_model_meshes[i].header.instance_count++] = gpk_model_mesh_instance{fbx_mat4_to_mat4(&fbx_mesh_node->EvaluateGlobalTransform())};
      mesh_previously_encountered = true;
      break;
    }
  }
  if (mesh_previously_encountered) {
    m_error("multiple fbx mesh instances found, this is not yet supported");
  }
  struct gpk_model_mesh *new_mesh = &gpk_model_meshes[gpk_model_meshes_count++];
  {
    new_mesh->fbx_mesh = fbx_mesh;
    new_mesh->header.instances_offset = (uint32)round_to_multi(sizeof(struct gpk_model_mesh_header), 16);
    new_mesh->header.instance_count = 1;
    new_mesh->header.vertex_size = 12 /*position*/ + 4 /*uv*/ + 4 /*normal*/;
    new_mesh->instances = m_calloc(1, gpk_model_mesh_instance);
    new_mesh->instances[0] = gpk_model_mesh_instance{fbx_mat4_to_mat4(&fbx_mesh_node->EvaluateGlobalTransform())};
  }
  {
    if (fbx_mesh_node->GetMaterialCount() <= 0) {
      m_error("fbx mesh has no material");
    }
    new_mesh->header.materials_offset = (uint32)round_to_multi(new_mesh->header.instances_offset + new_mesh->header.instance_count * sizeof(struct gpk_model_mesh_instance), 16);
    new_mesh->header.material_count = fbx_mesh_node->GetMaterialCount();
    new_mesh->materials = m_calloc(new_mesh->header.material_count, gpk_model_mesh_material);
    for (uint32 i = 0; i < new_mesh->header.material_count; i += 1) {
      FbxSurfaceMaterial *fbx_surface_material = fbx_mesh_node->GetMaterial(i);
      gpk_model_mesh_material *material = &new_mesh->materials[i];
      m_assert(strlen(fbx_surface_material->GetName()) < sizeof(material->name));
      strcpy(material->name, fbx_surface_material->GetName());
      FbxProperty fbx_diffuse_property = fbx_surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
      if (!fbx_diffuse_property.IsValid()) {
        m_error("fbx mesh surface material diffuse property not valid");
      }
      FbxFileTexture* fbx_diffuse_file_texture = fbx_diffuse_property.GetSrcObject<FbxFileTexture>(0);
      if (fbx_diffuse_file_texture) {
        material->diffuse_map_image_index = find_or_add_fbx_file_image(gpk_model_images, &gpk_model_images_count, fbx_diffuse_file_texture, gpk_image_format_b8g8r8a8_srgb);
        material->diffuse_map_image_scales = vec2{(float)fbx_diffuse_file_texture->GetScaleU(), (float)fbx_diffuse_file_texture->GetScaleV()};
      }
      else {
        m_error("fbx mesh surface diffuse material has no file texture");
      }
      FbxProperty fbx_normal_map_property = fbx_surface_material->FindProperty(FbxSurfaceMaterial::sNormalMap);
      if (!fbx_normal_map_property.IsValid()) {
        m_error("fbx mesh surface material normal map property not valid");
      }
      FbxFileTexture* fbx_normal_map_file_texture = fbx_normal_map_property.GetSrcObject<FbxFileTexture>(0);
      if (fbx_normal_map_file_texture) {
        material->normal_map_image_index = find_or_add_fbx_file_image(gpk_model_images, &gpk_model_images_count, fbx_normal_map_file_texture, gpk_image_format_b8g8r8a8, true);
        material->normal_map_image_scales = vec2{(float)fbx_normal_map_file_texture->GetScaleU(), (float)fbx_normal_map_file_texture->GetScaleV()};
      }
      else {
        material->normal_map_image_index = -1;
      }
    }
  }
  fbx_mesh->GenerateNormals();
  FbxSkin *fbx_skin = (FbxSkin*)fbx_mesh->GetDeformer(0, FbxDeformer::EDeformerType::eSkin);
  joint_weights *fbx_control_point_joint_weights = nullptr;
  {
    if (fbx_skin && fbx_skin->GetClusterCount() > 128) {
      m_error("mesh with more than 128(%d) joints detected", fbx_skin->GetClusterCount());
    }
    new_mesh->header.joints_offset = (uint32)round_to_multi(new_mesh->header.materials_offset + new_mesh->header.material_count * sizeof(struct gpk_model_mesh_material), 16);
    if (fbx_skin) {
      new_mesh->header.joint_count = fbx_skin->GetClusterCount();
      new_mesh->header.vertex_size += 12;
      new_mesh->joints = m_calloc(new_mesh->header.joint_count, struct gpk_model_mesh_joint);
      fbx_control_point_joint_weights = m_calloc(fbx_mesh->GetControlPointsCount(), struct joint_weights);
      for (int i = 0; i < fbx_skin->GetClusterCount(); i += 1) {
        uint8 joint_index = i;
        FbxCluster *fbx_cluster = fbx_skin->GetCluster(i);
        FbxNode *fbx_cluster_node = fbx_cluster->GetLink();
        uint32 fbx_cluster_node_index = -1;
        for (uint32 i = 0; i < gpk_model_nodes_count; i += 1) {
          if (fbx_nodes[i] == fbx_cluster_node) {
            fbx_cluster_node_index = i;
            break;
          }
        }
        m_assert(fbx_cluster_node_index != -1);
        FbxAMatrix geometry_mat = FbxAMatrix(fbx_cluster_node->GetGeometricTranslation(FbxNode::eSourcePivot), fbx_mesh_node->GetGeometricRotation(FbxNode::eSourcePivot), fbx_cluster_node->GetGeometricScaling(FbxNode::eSourcePivot));
        FbxAMatrix transform_mat;
        FbxAMatrix transform_link_mat;
        fbx_cluster->GetTransformMatrix(transform_mat);
        fbx_cluster->GetTransformLinkMatrix(transform_link_mat);
        FbxAMatrix global_inv_bind_pose_mat = transform_link_mat.Inverse() * transform_mat * geometry_mat;
        new_mesh->joints[i] = gpk_model_mesh_joint{fbx_cluster_node_index, fbx_mat4_to_mat4(&global_inv_bind_pose_mat)};
        int *control_point_indices = fbx_cluster->GetControlPointIndices();
        double *control_point_weights = fbx_cluster->GetControlPointWeights();
        int control_points_count = fbx_cluster->GetControlPointIndicesCount();
        for (int i = 0; i < control_points_count; i += 1) {
          int index = control_point_indices[i];
          float weight = (float)control_point_weights[i];
          if (weight == 0.0f) {
            m_error("control point joint has 0 weight");
          }
          joint_weights *joint_weights = &fbx_control_point_joint_weights[index];
          bool found = false;
          for (uint32 i = 0; i < 4; i += 1) {
            if (joint_weights->weights[i] == 0.0f) {
              joint_weights->weights[i] = weight;
              joint_weights->joint_indices[i] = joint_index;
              found = true;
              break;
            }
          }
          if (!found) {
            m_error("control point with too many joint weights(> 4)");
          }
        }
      }    
    }
    else {
      if (gpk_model_animation_count != 0) {
        gpk_model_animation_count = 0;
      }
    }
  }
  uint32 polygons_count = fbx_mesh->GetPolygonCount();
  {
    for (uint32 i = 0; i < polygons_count; i += 1) {
      if (fbx_mesh->GetPolygonSize(i) != 3) {
        m_error("fbx model polygon count %d is not a triangle(%d vertices)", i, fbx_mesh->GetPolygonSize(i));
      }
    }
    new_mesh->header.vertices_offset = (uint32)round_to_multi(new_mesh->header.joints_offset + new_mesh->header.joint_count * sizeof(struct gpk_model_mesh_joint), 16);
    new_mesh->header.vertex_count = polygons_count * 3;
    new_mesh->header.total_size = new_mesh->header.vertices_offset + new_mesh->header.vertex_count * new_mesh->header.vertex_size;
    new_mesh->vertices_positions = m_calloc(polygons_count * 3, union vec3);
    new_mesh->vertices_uvs = m_calloc(polygons_count * 3, uv);
    new_mesh->vertices_normals = m_calloc(polygons_count * 3, uint32);
    new_mesh->vertices_joint_weights = fbx_skin ? m_calloc(polygons_count * 3, struct joint_weights) : nullptr;
  }
  FbxVector4 *fbx_control_points = fbx_mesh->GetControlPoints();
  int *polygon_control_point_indices = {};
  FbxArray<FbxVector2> polygon_uvs = {};
  FbxArray<FbxVector4> polygon_normals = {};
  polygon_control_point_indices = fbx_mesh->GetPolygonVertices();
  fbx_mesh->GetPolygonVertexNormals(polygon_normals);
  fbx_mesh->GetPolygonVertexUVs("map1", polygon_uvs);
  for (uint32 i = 0; i < polygons_count * 3; i += 1) {
    FbxVector4 control_point = fbx_control_points[polygon_control_point_indices[i]];
    FbxVector2 uv = polygon_uvs[i];
    FbxVector4 normal = polygon_normals[i];
    new_mesh->vertices_positions[i] = vec3{{(float)control_point.mData[0], (float)control_point.mData[1], (float)control_point.mData[2]}};
    new_mesh->vertices_uvs[i] = {float_to_half((float)uv.mData[0]), float_to_half(1.0f - (float)uv.mData[1])};
    new_mesh->vertices_normals[i] = pack_snorm_3x10_1x2((float)normal.mData[0], (float)normal.mData[1], (float)normal.mData[2], 0.0f);
    if (fbx_skin) {
      joint_weights *joint_weights_src = &fbx_control_point_joint_weights[polygon_control_point_indices[i]];
      joint_weights *joint_weights_dst = &new_mesh->vertices_joint_weights[i];
      *joint_weights_dst = *joint_weights_src;
      for (uint32 i = 0; i < 4; i += 1) {
        joint_weights_dst->half_float_weights[i] = float_to_half(joint_weights_dst->weights[i]);
      }
    }
  }
}
for (uint32 i = 0; i < gpk_model_meshes_count; i += 1) {
  if (gpk_model_meshes[i].header.vertex_size != gpk_model_meshes[0].header.vertex_size) {
    m_error("model mesh has different vertex size");
  }
}
struct gpk_model_header gpk_model_header = {};
{
  gpk_model_header.nodes_offset = (uint32)round_to_multi(sizeof(struct gpk_model_header), 16);
  gpk_model_header.node_count = gpk_model_nodes_count;
  uint32 offset = (uint32)round_to_multi(gpk_model_header.nodes_offset + gpk_model_header.node_count * sizeof(struct gpk_model_node), 16);
  gpk_model_header.animation_count = gpk_model_animation_count;
  m_assert(gpk_model_header.animation_count < m_countof(gpk_model_header.animation_offsets));
  for (uint32 i = 0; i < gpk_model_animation_count; i += 1) {
    gpk_model_header.animation_offsets[i] = offset;
    offset = round_to_multi(offset + gpk_model_animation_headers[i].total_size, 16);
  }
  gpk_model_header.image_count = gpk_model_images_count;
  m_assert(gpk_model_header.image_count < m_countof(gpk_model_header.image_offsets));
  for (uint32 i = 0; i < gpk_model_images_count; i += 1) {
    gpk_model_header.image_offsets[i] = offset;
    offset = round_to_multi(offset + gpk_model_images[i].header.total_size, 16);
  }
  gpk_model_header.mesh_count = gpk_model_meshes_count;
  m_assert(gpk_model_header.mesh_count < m_countof(gpk_model_header.mesh_offsets));
  for (uint32 i = 0; i < gpk_model_meshes_count; i += 1) {
    gpk_model_header.mesh_offsets[i] = offset;
    offset = round_to_multi(offset + gpk_model_meshes[i].header.total_size, 16);
  }
  gpk_model_header.total_size = offset;
}
{
  file_mapping import_file_mapping = {};
  create_file_mapping(cmdl.import_file_name, gpk_model_header.total_size, &import_file_mapping);
  *(struct gpk_model_header *)import_file_mapping.ptr = gpk_model_header;
  memcpy(import_file_mapping.ptr + gpk_model_header.nodes_offset, gpk_model_nodes, gpk_model_header.node_count * sizeof(struct gpk_model_node));
  for (uint32 i = 0; i < gpk_model_header.animation_count; i += 1) {
    uint8 *animation_ptr = import_file_mapping.ptr + gpk_model_header.animation_offsets[i];
    *(struct gpk_model_animation_header *)animation_ptr = gpk_model_animation_headers[i];
    animation_ptr += gpk_model_animation_headers[i].frames_offset;
    uint32 frames_count = gpk_model_animation_headers[i].frame_count;
    gpk_model_animation_frame **frames = gpk_model_animation_frames[i];
    for (uint32 i = 0; i < gpk_model_header.node_count; i += 1) {
      memcpy(animation_ptr, frames[i], frames_count * sizeof(struct gpk_model_animation_frame));
      animation_ptr += frames_count * sizeof(struct gpk_model_animation_frame);
    }
  }
  for (uint32 i = 0; i < gpk_model_header.image_count; i += 1) {
    uint8 *image_ptr = import_file_mapping.ptr + gpk_model_header.image_offsets[i];
    *(struct gpk_image_header *)image_ptr = gpk_model_images[i].header;
    memcpy(image_ptr + gpk_model_images[i].header.image_offset, gpk_model_images[i].image_data, gpk_model_images[i].header.image_size);
  }
  for (uint32 i = 0; i < gpk_model_header.mesh_count; i += 1) {
    struct gpk_model_mesh *mesh = &gpk_model_meshes[i];
    uint8 *mesh_ptr = import_file_mapping.ptr + gpk_model_header.mesh_offsets[i];
    *(struct gpk_model_mesh_header *)mesh_ptr = mesh->header;
    gpk_model_mesh_instance *instances = (gpk_model_mesh_instance *)(mesh_ptr + mesh->header.instances_offset);
    for (uint32 i = 0; i < mesh->header.instance_count; i += 1) {
      instances[i] = mesh->instances[i];
    }
    gpk_model_mesh_material *materials = (gpk_model_mesh_material *)(mesh_ptr + mesh->header.materials_offset);
    for (uint32 i = 0; i < mesh->header.material_count; i += 1) {
      materials[i] = mesh->materials[i];
    }
    gpk_model_mesh_joint *joints = (gpk_model_mesh_joint *)(mesh_ptr + mesh->header.joints_offset);
    for (uint32 i = 0; i < mesh->header.joint_count; i += 1) {
      joints[i] = mesh->joints[i];
    }
    uint8 *vertices = mesh_ptr + mesh->header.vertices_offset;
    for (uint32 i = 0; i < mesh->header.vertex_count; i += 1) {
      *(vec3 *)vertices = mesh->vertices_positions[i];
      vertices += 12;
      ((uint16 *)vertices)[0] = mesh->vertices_uvs[i].u;
      ((uint16 *)vertices)[1] = mesh->vertices_uvs[i].v;
      vertices += 4;
      *(uint32 *)vertices = mesh->vertices_normals[i];
      vertices += 4;
      if (mesh->vertices_joint_weights) {
        memcpy(vertices, mesh->vertices_joint_weights[i].joint_indices, 4);
        vertices += 4;
        memcpy(vertices, mesh->vertices_joint_weights[i].half_float_weights, 8);
        vertices += 8;
      }
    }
  }
  close_file_mapping(import_file_mapping);
  m_done("import model \"%s\" succeeded", cmdl.import_file_name);
}
#endif
