/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"

#define NVTT_SHARED 1
#include "../vendor/include/nvtt/nvtt.h"

#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include "../vendor/include/tinygltf/tiny_gltf.h"

#include "math.cpp"
#include "gpk.cpp"
#include <vulkan/vulkan.h>

#include <stack>

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

bool gltf_model_node_has_cycle(const tinygltf::Model &gltf_model) {
	bool has_cycle = false;
	for (auto &scene : gltf_model.scenes) {
		for (auto &node :scene.nodes) {
			std::vector<int> node_stack;
			node_stack.push_back(node);
			uint64 node_stack_max_size = 1;
			while (!node_stack.empty()) {
				node_stack_max_size = max(node_stack.size(), node_stack_max_size);
				int index = node_stack.back();
				auto &node = gltf_model.nodes[index];
				for (auto &child : node.children) {
					for (auto &node : node_stack) {
						if (child == node) {
							has_cycle = true;
						}
					}
				}
				node_stack.pop_back();
				for (auto &child : node.children) {
					node_stack.push_back(child);
				}
			}
		}
	}
	return has_cycle;
}

void gltf_to_gpk(std::string gltf_file, std::string gpk_file) {
	printf("begin importing gltf: \"%s\" \n", gltf_file.c_str());

	tinygltf::Model gltf_model;
	{
		tinygltf::TinyGLTF gltf_loader;
		std::string gltf_loader_err;
		if (gltf_file.substr(gltf_file.find_last_of(".") + 1) == "gltf") {
			bool gltf_load_success = gltf_loader.LoadASCIIFromFile(&gltf_model, &gltf_loader_err, gltf_file);
			if (!gltf_loader_err.empty()) {
				printf("%s load err:\n%s\n", gltf_file.c_str(), gltf_loader_err.c_str());
			}
			m_assert(gltf_load_success);
		}
		else if (gltf_file.substr(gltf_file.find_last_of(".") + 1) == "glb") {
			bool gltf_load_success = gltf_loader.LoadBinaryFromFile(&gltf_model, &gltf_loader_err, gltf_file);
			if (!gltf_loader_err.empty()) {
				printf("%s load err:\n%s\n", gltf_file.c_str(), gltf_loader_err.c_str());
			}
			m_assert(gltf_load_success);
		}
		else {
			m_assert(false);
		}
	}
	gpk_model gpk_model = {"GPK_MODEL_FORMAT"};
	uint32 current_offset = round_up((uint32)sizeof(gpk_model), 16u);
	std::vector<gpk_model_scene> gpk_model_scenes;
	{
		gpk_model.scene_offset = current_offset;
		gpk_model.scene_count = (uint32)gltf_model.scenes.size();
		m_assert(gpk_model.scene_count > 0);
		gpk_model_scenes.resize(gpk_model.scene_count);
		for (uint32 i = 0; i < gpk_model.scene_count; i += 1) {
			auto &scene = gltf_model.scenes[i];
			auto &gpk_scene = gpk_model_scenes[i];
			m_assert(scene.name.length() < sizeof(gpk_scene.name));
			strcpy(gpk_scene.name, scene.name.c_str());
			gpk_scene.node_index_count = (uint32)scene.nodes.size();
			m_assert(gpk_scene.node_index_count <= m_countof(gpk_scene.node_indices));
			for (uint32 i = 0; i < gpk_scene.node_index_count; i += 1) {
				gpk_scene.node_indices[i] = (uint32)scene.nodes[i];
			}
		}
		current_offset = round_up(current_offset + gpk_model.scene_count * (uint32)sizeof(struct gpk_model_scene), 16u);
	}
	std::vector<gpk_model_node> gpk_model_nodes;
	{
		m_assert(!gltf_model_node_has_cycle(gltf_model));
		gpk_model.node_offset = current_offset;
		gpk_model.node_count = (uint32)gltf_model.nodes.size();
		m_assert(gpk_model.node_count > 0);
		gpk_model_nodes.resize(gpk_model.node_count);
		for (uint32 i = 0; i < gpk_model.node_count; i += 1) {
			auto &node = gltf_model.nodes[i];
			auto &gpk_node = gpk_model_nodes[i];
			gpk_node.mesh_index = (node.mesh >= 0 && node.mesh < gltf_model.meshes.size()) ? node.mesh : UINT32_MAX;
			gpk_node.skin_index = (node.skin >= 0 && node.skin < gltf_model.skins.size()) ? node.skin : UINT32_MAX;
			array_set(gpk_node.children, UINT32_MAX);
			m_assert(node.children.size() <= m_countof(gpk_node.children));
			gpk_node.child_count = (uint32)node.children.size();
			for (uint32 i = 0; i < gpk_node.child_count; i += 1) {
				m_assert(node.children[i] >= 0 && node.children[i] < gltf_model.nodes.size());
				gpk_node.children[i] = node.children[i];
			}
			if (node.matrix.size() > 0) {
				for (uint32 i = 0; i < 16; i += 1) {
					gpk_node.local_transform_mat[i / 4][i % 4] = (float)node.matrix[i];
				}
				gpk_node.local_transform = mat4_get_transform(gpk_node.local_transform_mat);
			}
			else {
				gpk_node.local_transform = transform_identity();
				if (node.scale.size() > 0) {
					gpk_node.local_transform.scale =  {(float)node.scale[0], (float)node.scale[1], (float)node.scale[2]};
				}
				if (node.rotation.size() > 0) {
					gpk_node.local_transform.rotate = {(float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]};
				}
				if (node.translation.size() > 0) {
					gpk_node.local_transform.translate = {(float)node.translation[0], (float)node.translation[1], (float)node.translation[2]};
				}
				gpk_node.local_transform_mat = mat4_from_transform(gpk_node.local_transform);
			}
		}
		current_offset = round_up(current_offset + gpk_model.node_count * (uint32)sizeof(struct gpk_model_node), 16u);
	}
	std::vector<gpk_model_mesh> gpk_model_meshes;
	{
		gpk_model.mesh_offset = current_offset;
		gpk_model.mesh_count = (uint32)gltf_model.meshes.size();
		m_assert(gpk_model.mesh_count > 0);
		gpk_model_meshes.resize(gpk_model.mesh_count);
		for (uint32 i = 0; i < gpk_model.mesh_count; i += 1) {
			auto &mesh = gltf_model.meshes[i];
			auto &gpk_mesh = gpk_model_meshes[i];
			m_assert(mesh.name.length() < sizeof(gpk_mesh.name));
			strcpy(gpk_mesh.name, mesh.name.c_str());
			gpk_mesh.skin_index = UINT32_MAX;
			m_assert(mesh.primitives.size() > 0);
			gpk_mesh.primitive_count = (uint32)mesh.primitives.size();
			for (auto &p : mesh.primitives) {
				m_assert(p.mode == TINYGLTF_MODE_TRIANGLES);
				m_assert(p.indices >= 0);
				m_assert(p.attributes.find("POSITION") != p.attributes.end());
				m_assert(p.attributes.find("NORMAL") != p.attributes.end());
			}
		}
		for (uint32 i = 0; i < gpk_model.node_count; i += 1) {
			auto &node = gltf_model.nodes[i];
			if (node.skin >= 0 && node.skin < gltf_model.skins.size()) {
				m_assert(node.mesh >= 0 && node.mesh < gltf_model.meshes.size());
				m_assert(gpk_model_meshes[node.mesh].skin_index == UINT32_MAX);
				gpk_model_meshes[node.mesh].skin_index = node.skin;
			}
		}
		current_offset = round_up(current_offset + gpk_model.mesh_count * (uint32)sizeof(struct gpk_model_mesh), 16u);
	}
	std::vector<gpk_model_skin> gpk_model_skins;
	{
		gpk_model.skin_offset = current_offset;
		gpk_model.skin_count = (uint32)gltf_model.skins.size();
		gpk_model_skins.resize(gpk_model.skin_count);
		for (uint32 i = 0; i < gpk_model.skin_count; i += 1) {
			auto &skin = gltf_model.skins[i];
			auto &gpk_skin = gpk_model_skins[i];
			m_assert(skin.name.length() < sizeof(gpk_skin.name));
			strcpy(gpk_skin.name, skin.name.c_str());
			m_assert(skin.skeleton >= 0 && skin.skeleton < gltf_model.nodes.size());
			m_assert(skin.joints.size() > 0 && skin.joints.size() < gltf_model.nodes.size());
			m_assert(skin.joints.size() < 256);
			gpk_skin.joint_count = (uint32)skin.joints.size();
		}
		current_offset = round_up(current_offset + gpk_model.skin_count * (uint32)sizeof(struct gpk_model_skin), 16u);
	}
	std::vector<gpk_model_animation> gpk_model_animations;
	{
		gpk_model.animation_offset = current_offset;
		gpk_model.animation_count = (uint32)gltf_model.animations.size();
		gpk_model_animations.resize(gpk_model.animation_count);
		for (uint32 i = 0; i < gpk_model.animation_count; i += 1) {
			auto &animation = gltf_model.animations[i];
			auto &gpk_animation = gpk_model_animations[i];
			m_assert(animation.name.length() < sizeof(gpk_animation.name));
			strcpy(gpk_animation.name, animation.name.c_str());
			gpk_animation.channel_count = (uint32)animation.channels.size();
			gpk_animation.sampler_count = (uint32)animation.samplers.size();
		}
		current_offset = round_up(current_offset + gpk_model.animation_count * (uint32)sizeof(struct gpk_model_animation), 16u);
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
		image_remaps.resize(gltf_model.images.size(), {});
		for (auto &m : gltf_model.materials) {
			auto base_color_texture = m.values.find("baseColorTexture");
			if (base_color_texture != m.values.end()) {
				int32 texture_index = base_color_texture->second.TextureIndex();
				m_assert(texture_index >= 0 && texture_index < gltf_model.textures.size());
				int32 image_index = gltf_model.textures[texture_index].source;
				m_assert(image_index >= 0 && image_index < gltf_model.images.size());
				image_remaps[image_index].is_base_color = true;
			}

			auto metallic_roughness_texture = m.values.find("metallicRoughnessTexture");
			if (metallic_roughness_texture != m.values.end()) {
				int32 texture_index = metallic_roughness_texture->second.TextureIndex();
				m_assert(texture_index >= 0 && texture_index < gltf_model.textures.size());
				int32 image_index = gltf_model.textures[texture_index].source;
				m_assert(image_index >= 0 && image_index < gltf_model.images.size());
				image_remaps[image_index].is_metallic_roughness = true;
			}

			auto normal_texture = m.additionalValues.find("normalTexture");
			if (normal_texture != m.additionalValues.end()) {
				int32 texture_index = normal_texture->second.TextureIndex();
				m_assert(texture_index >= 0 && texture_index < gltf_model.textures.size());
				int32 image_index = gltf_model.textures[texture_index].source;
				m_assert(image_index >= 0 && image_index < gltf_model.images.size());
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
		gpk_model.material_count = (uint32)gltf_model.materials.size();
		gpk_model_materials.resize(gpk_model.material_count);
		for (uint32 i = 0; i < gpk_model.material_count; i += 1) {
			gpk_model_materials[i].diffuse_image_index = UINT32_MAX;
			gpk_model_materials[i].diffuse_factor = {1.0f, 1.0f, 1.0f, 1.0f};
			gpk_model_materials[i].metallic_image_index = UINT32_MAX;
			gpk_model_materials[i].roughness_image_index = UINT32_MAX;
			gpk_model_materials[i].metallic_factor = 1.0f;
			gpk_model_materials[i].roughness_factor = 1.0f;
			gpk_model_materials[i].normal_image_index = UINT32_MAX;
		}
		for (uint32 i = 0; i < gpk_model.material_count; i += 1) {
			auto &material = gltf_model.materials[i];
			auto &gpk_material = gpk_model_materials[i];
			m_assert(material.name.length() < sizeof(gpk_material.name));
			strcpy(gpk_material.name, material.name.c_str());
			auto base_color_texture = material.values.find("baseColorTexture");
			auto base_color_factor = material.values.find("baseColorFactor");
			auto metallic_roughness_texture = material.values.find("metallicRoughnessTexture");
			auto metallic_factor = material.values.find("metallicFactor");
			auto roughness_factor = material.values.find("roughnessFactor");
			auto normal_texture = material.additionalValues.find("normalTexture");
			if (base_color_texture != material.values.end()) {
				int32 index = base_color_texture->second.TextureIndex();
				m_assert(index >= 0 && index < gltf_model.textures.size());
				auto &texture = gltf_model.textures[index];
				m_assert(texture.source >= 0 && texture.source < gltf_model.images.size());
				m_assert(image_remaps[texture.source].is_base_color);
				gpk_material.diffuse_image_index = image_remaps[texture.source].index;
				if (texture.sampler >= 0 && texture.sampler < gltf_model.samplers.size()) {
					gpk_material.diffuse_image_wrap_s = tinygltf_wrap_to_vk_wrap(gltf_model.samplers[texture.sampler].wrapS);
					gpk_material.diffuse_image_wrap_t = tinygltf_wrap_to_vk_wrap(gltf_model.samplers[texture.sampler].wrapT);
				}
				else {
					gpk_material.diffuse_image_wrap_s = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					gpk_material.diffuse_image_wrap_t = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				}
			}
			else if (base_color_factor != material.values.end()) {
				auto &color = base_color_factor->second.number_array;
				m_assert(color.size() == 4);
				gpk_material.diffuse_factor = {(float)color[0], (float)color[1], (float)color[2], (float)color[3]};
			}
			if (metallic_roughness_texture != material.values.end()) {
				int32 index = metallic_roughness_texture->second.TextureIndex();
				m_assert(index >= 0 && index < gltf_model.textures.size());
				auto &texture = gltf_model.textures[index];
				m_assert(texture.source >= 0 && texture.source < gltf_model.images.size());
				m_assert(image_remaps[texture.source].is_metallic_roughness);
				gpk_material.metallic_image_index = image_remaps[texture.source].index;
				gpk_material.roughness_image_index = image_remaps[texture.source].index + 1;
				if (texture.sampler >= 0 && texture.sampler < gltf_model.samplers.size()) {
					gpk_material.metallic_roughness_image_wrap_s = tinygltf_wrap_to_vk_wrap(gltf_model.samplers[texture.sampler].wrapS);
					gpk_material.metallic_roughness_image_wrap_t = tinygltf_wrap_to_vk_wrap(gltf_model.samplers[texture.sampler].wrapT);
				}
				else {
					gpk_material.metallic_roughness_image_wrap_s = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					gpk_material.metallic_roughness_image_wrap_t = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				}
			}
			else {
				if (metallic_factor != material.values.end()) {
					auto &factor = metallic_factor->second.number_array;
					m_assert(factor.size() == 1);
					gpk_material.metallic_factor = (float)factor[0];
				}
				if (roughness_factor != material.values.end()) {
					auto &factor = roughness_factor->second.number_array;
					m_assert(factor.size() == 1);
					gpk_material.roughness_factor = (float)factor[0];
				}
			}
			if (normal_texture != material.additionalValues.end()) {
				int32 index = normal_texture->second.TextureIndex();
				m_assert(index >= 0 && index < gltf_model.textures.size());
				auto &texture = gltf_model.textures[index];
				m_assert(texture.source >= 0 && texture.source < gltf_model.images.size());
				m_assert(image_remaps[texture.source].is_normal);
				gpk_material.normal_image_index = image_remaps[texture.source].index;
				if (texture.sampler >= 0 && texture.sampler < gltf_model.samplers.size()) {
					gpk_material.normal_image_wrap_s = tinygltf_wrap_to_vk_wrap(gltf_model.samplers[texture.sampler].wrapS);
					gpk_material.normal_image_wrap_t = tinygltf_wrap_to_vk_wrap(gltf_model.samplers[texture.sampler].wrapT);
				}
				else {
					gpk_material.normal_image_wrap_s = VK_SAMPLER_ADDRESS_MODE_REPEAT;
					gpk_material.normal_image_wrap_t = VK_SAMPLER_ADDRESS_MODE_REPEAT;
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
		for (size_t i = 0; i < gltf_model.images.size(); i += 1) {
			auto &image = gltf_model.images[i];
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
		auto &mesh = gltf_model.meshes[i];
		auto &gpk_mesh = gpk_model_meshes[i];
		gpk_mesh.primitive_offset = current_offset;
		current_offset = round_up(current_offset + gpk_mesh.primitive_count * (uint32)sizeof(struct gpk_model_mesh_primitive), 16u);

		for (uint32 i = 0; i < gpk_mesh.primitive_count; i += 1) {
			auto &primitive = mesh.primitives[i];

			auto &index_accessor = gltf_model.accessors[primitive.indices];
			m_assert(index_accessor.count > 0);
			m_assert(
				index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE || 
				index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT || 
				index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT);
			m_assert(index_accessor.type == TINYGLTF_TYPE_SCALAR);

			auto &position_accessor = gltf_model.accessors[primitive.attributes["POSITION"]];
			m_assert(position_accessor.count > 0 && position_accessor.count <= UINT16_MAX);
			m_assert(position_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(position_accessor.type == TINYGLTF_TYPE_VEC3);

			auto &normal_accessor = gltf_model.accessors[primitive.attributes["NORMAL"]];
			m_assert(normal_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(normal_accessor.type == TINYGLTF_TYPE_VEC3);
			m_assert(normal_accessor.count == position_accessor.count);

			if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
				auto &uv_accessor = gltf_model.accessors[primitive.attributes["TEXCOORD_0"]];
				m_assert(uv_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
				m_assert(uv_accessor.type == TINYGLTF_TYPE_VEC2);
				m_assert(uv_accessor.count == position_accessor.count);
			}

			if (primitive.attributes.find("TANGENT") != primitive.attributes.end()) {
				auto &tangent_accessor = gltf_model.accessors[primitive.attributes["TANGENT"]];
				m_assert(tangent_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
				m_assert(tangent_accessor.type == TINYGLTF_TYPE_VEC4);
				m_assert(tangent_accessor.count == position_accessor.count);
			}

			if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
				auto weights = primitive.attributes.find("WEIGHTS_0");
				m_assert(weights != primitive.attributes.end());
				auto &joint_accessor = gltf_model.accessors[primitive.attributes["JOINTS_0"]];
				m_assert(joint_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
				m_assert(joint_accessor.type == TINYGLTF_TYPE_VEC4);
				m_assert(joint_accessor.count == position_accessor.count);

				auto &weight_accessor = gltf_model.accessors[primitive.attributes["WEIGHTS_0"]];
				m_assert(weight_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
				m_assert(weight_accessor.type == TINYGLTF_TYPE_VEC4);
				m_assert(weight_accessor.count == position_accessor.count);
			}

			current_offset = round_up(current_offset + (uint32)index_accessor.count * (uint32)sizeof(uint16), 16u);
			current_offset = round_up(current_offset + (uint32)position_accessor.count * (uint32)sizeof(struct gpk_model_vertex), 16u);
		}
	}
	for (uint32 i = 0; i < gpk_model.skin_count; i += 1) {
		auto & skin = gpk_model_skins[i];
		skin.joints_offset = current_offset;
		current_offset = round_up(current_offset + skin.joint_count * (uint32)sizeof(struct gpk_model_joint), 16u);
	}
	for (uint32 i = 0; i < gpk_model.animation_count; i += 1) {
		auto & animation = gltf_model.animations[i];
		auto & gpk_animation = gpk_model_animations[i];
		gpk_animation.channel_offset = current_offset;
		current_offset = round_up(current_offset + gpk_animation.channel_count * (uint32)sizeof(struct gpk_model_animation_channel), 16u);
		gpk_animation.sampler_offset = current_offset;
		current_offset = round_up(current_offset + gpk_animation.sampler_count * (uint32)sizeof(struct gpk_model_animation_sampler), 16u);
		for (auto &sampler : animation.samplers) {
			m_assert(sampler.input >= 0 && sampler.input < gltf_model.accessors.size());
			m_assert(sampler.output >= 0 && sampler.output < gltf_model.accessors.size());
			auto &input_accessor = gltf_model.accessors[sampler.input];
			auto &output_accessor = gltf_model.accessors[sampler.output];
			m_assert(input_accessor.count > 0 && input_accessor.count <= output_accessor.count);
			m_assert(input_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(input_accessor.type == TINYGLTF_TYPE_SCALAR);
			m_assert(output_accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(
				output_accessor.type == TINYGLTF_TYPE_SCALAR || 
				output_accessor.type == TINYGLTF_TYPE_VEC2 || 
				output_accessor.type == TINYGLTF_TYPE_VEC3 || 
				output_accessor.type == TINYGLTF_TYPE_VEC4);
			current_offset = round_up(current_offset + (uint32)input_accessor.count * (uint32)sizeof(struct gpk_model_animation_key_frame), 16u);
		}
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
	memcpy(gpk_file_mapping.ptr + gpk_model.animation_offset, &gpk_model_animations[0], gpk_model_animations.size() * sizeof(struct gpk_model_animation));
	memcpy(gpk_file_mapping.ptr + gpk_model.material_offset, &gpk_model_materials[0], gpk_model_materials.size() * sizeof(struct gpk_model_material));
	memcpy(gpk_file_mapping.ptr + gpk_model.image_offset, &gpk_model_images[0], gpk_model_images.size() * sizeof(struct gpk_model_image));
	for (uint32 i = 0; i < gpk_model.mesh_count; i += 1) {
		auto &mesh = gltf_model.meshes[i];
		auto &gpk_mesh = gpk_model_meshes[i];

		uint32 indices_vertices_offset = round_up(gpk_mesh.primitive_offset + gpk_mesh.primitive_count * (uint32)sizeof(struct gpk_model_mesh_primitive), 16u);
		for (uint32 i = 0; i < gpk_mesh.primitive_count; i += 1) {
			auto &primitive = mesh.primitives[i];
			auto &gpk_primitive = ((gpk_model_mesh_primitive *)(gpk_file_mapping.ptr + gpk_mesh.primitive_offset))[i];

			auto &index_accessor = gltf_model.accessors[primitive.indices];
			auto &index_buffer_view = gltf_model.bufferViews[index_accessor.bufferView];
			auto &index_buffer = gltf_model.buffers[index_buffer_view.buffer];
			uint32 index_stride = (uint32)index_buffer_view.byteStride;
			if (index_stride == 0) {
				if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
					index_stride = 1;
				}
				else if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					index_stride = 2;
				}
				else if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
					index_stride = 4;
				}
			}
			unsigned char *index_data = &index_buffer.data[index_buffer_view.byteOffset + index_accessor.byteOffset];

			auto &position_accessor = gltf_model.accessors[primitive.attributes["POSITION"]];
			auto &position_buffer_view = gltf_model.bufferViews[position_accessor.bufferView];
			auto &position_buffer = gltf_model.buffers[position_buffer_view.buffer];
			uint32 position_stride = position_buffer_view.byteStride == 0 ? 12 : (uint32)position_buffer_view.byteStride;
			unsigned char *position_data = &position_buffer.data[position_buffer_view.byteOffset + position_accessor.byteOffset];

			auto &normal_accessor = gltf_model.accessors[primitive.attributes["NORMAL"]];
			auto &normal_buffer_view = gltf_model.bufferViews[normal_accessor.bufferView];
			auto &normal_buffer = gltf_model.buffers[normal_buffer_view.buffer];
			uint32 normal_stride = normal_buffer_view.byteStride == 0 ? 12 : (uint32)normal_buffer_view.byteStride;
			unsigned char *normal_data = &normal_buffer.data[normal_buffer_view.byteOffset + normal_accessor.byteOffset];

			uint8 *uv_data = nullptr;
			uint32 uv_stride = 0;
			if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
				auto &uv_accessor = gltf_model.accessors[primitive.attributes["TEXCOORD_0"]];
				auto &uv_buffer_view = gltf_model.bufferViews[uv_accessor.bufferView];
				auto &uv_buffer = gltf_model.buffers[uv_buffer_view.buffer];
				uv_stride = uv_buffer_view.byteStride == 0 ? 8 : (uint32)uv_buffer_view.byteStride;
				uv_data = &uv_buffer.data[uv_buffer_view.byteOffset + uv_accessor.byteOffset];
			}

			uint8 *tangent_data = nullptr;
			uint32 tangent_stride = 0;
			if (primitive.attributes.find("TANGENT") != primitive.attributes.end()) {
				auto &tangent_accessor = gltf_model.accessors[primitive.attributes["TANGENT"]];
				auto &tangent_buffer_view = gltf_model.bufferViews[tangent_accessor.bufferView];
				auto &tangent_buffer = gltf_model.buffers[tangent_buffer_view.buffer];
				tangent_stride = tangent_buffer_view.byteStride == 0 ? 16 : (uint32)tangent_buffer_view.byteStride;
				tangent_data = &tangent_buffer.data[tangent_buffer_view.byteOffset + tangent_accessor.byteOffset];
			}

			uint8 *joint_data = nullptr;
			uint32 joint_stride = 0;
			uint8 *weight_data = nullptr;
			uint32 weight_stride = 0;
			uint32 skin_joint_count = 0;
			if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
				m_assert(gpk_mesh.skin_index < (uint32)gltf_model.skins.size());
				skin_joint_count = (uint32)gltf_model.skins[gpk_mesh.skin_index].joints.size();
				m_assert(skin_joint_count < 256);

				auto &joint_accessor = gltf_model.accessors[primitive.attributes["JOINTS_0"]];
				auto &joint_buffer_view = gltf_model.bufferViews[joint_accessor.bufferView];
				auto &joint_buffer = gltf_model.buffers[joint_buffer_view.buffer];
				joint_stride = joint_buffer_view.byteStride == 0 ? 8 : (uint32)joint_buffer_view.byteStride;
				joint_data = &joint_buffer.data[joint_buffer_view.byteOffset + joint_accessor.byteOffset];

				auto &weight_accessor = gltf_model.accessors[primitive.attributes["WEIGHTS_0"]];
				auto &weight_buffer_view = gltf_model.bufferViews[weight_accessor.bufferView];
				auto &weight_buffer = gltf_model.buffers[weight_buffer_view.buffer];
				weight_stride = weight_buffer_view.byteStride == 0 ? 16 : (uint32)weight_buffer_view.byteStride;
				weight_data = &weight_buffer.data[weight_buffer_view.byteOffset + weight_accessor.byteOffset];
			}

			gpk_primitive.material_index = (primitive.material >= 0 && primitive.material < gltf_model.materials.size()) ? (uint32)primitive.material : UINT32_MAX;
			gpk_primitive.indices_offset = indices_vertices_offset;
			gpk_primitive.index_count = (uint32)index_accessor.count;
			indices_vertices_offset = round_up(indices_vertices_offset + gpk_primitive.index_count * (uint32)sizeof(uint16), 16u);
			gpk_primitive.vertices_offset = indices_vertices_offset;
			gpk_primitive.vertex_count = (uint32)position_accessor.count;
			indices_vertices_offset = round_up(indices_vertices_offset + gpk_primitive.vertex_count * (uint32)sizeof(struct gpk_model_vertex), 16u);
			gpk_primitive.has_joints = joint_data ? 1 : 0;

			if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
				for (uint32 i = 0; i < gpk_primitive.index_count; i += 1) {
					((uint16 *)(gpk_file_mapping.ptr + gpk_primitive.indices_offset))[i] = *(uint8 *)(index_data + index_stride * i);
				}
			}
			else if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
				for (uint32 i = 0; i < gpk_primitive.index_count; i += 1) {
					((uint16 *)(gpk_file_mapping.ptr + gpk_primitive.indices_offset))[i] = *(uint16 *)(index_data + index_stride * i);
				}
			}
			else if  (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
				for (uint32 i = 0; i < gpk_primitive.index_count; i += 1) {
					((uint16 *)(gpk_file_mapping.ptr + gpk_primitive.indices_offset))[i] = *(uint32 *)(index_data + index_stride * i);
				}
			}
			else {
				m_assert(false);
			}

			for (uint32 i = 0; i < gpk_primitive.vertex_count; i += 1) {
				gpk_model_vertex *vertex = ((gpk_model_vertex *)(gpk_file_mapping.ptr + gpk_primitive.vertices_offset)) + i;

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
	}
	for (uint32 i = 0; i < gpk_model.skin_count; i += 1) {
		auto &skin = gltf_model.skins[i];
		mat4 *inverse_bind_mats = nullptr;
		{
			auto &accessor = gltf_model.accessors[skin.inverseBindMatrices];
			m_assert(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			m_assert(accessor.type == TINYGLTF_TYPE_MAT4);
			m_assert(accessor.count == skin.joints.size());
			auto &buffer_view = gltf_model.bufferViews[accessor.bufferView];
			m_assert(buffer_view.byteStride == 0 || buffer_view.byteStride == sizeof(mat4));
			auto &buffer = gltf_model.buffers[buffer_view.buffer];
			inverse_bind_mats = (mat4 *)(&buffer.data[accessor.byteOffset + buffer_view.byteOffset]);
		}
		auto &gpk_skin = gpk_model_skins[i];
		gpk_model_joint *gpk_joints = (gpk_model_joint *)(gpk_file_mapping.ptr + gpk_skin.joints_offset);
		m_assert(gpk_skin.joint_count == skin.joints.size());
		for (uint32 i = 0; i < gpk_skin.joint_count; i += 1) {
			m_assert(skin.joints[i] >= 0 && skin.joints[i] < gltf_model.nodes.size());
			gpk_joints[i].node_index = (uint32)skin.joints[i];
			gpk_joints[i].inverse_bind_mat = inverse_bind_mats[i];
		}
	}
	for (uint32 i = 0; i < gpk_model.animation_count; i += 1) {
		auto &animation = gltf_model.animations[i];
		auto &gpk_animation = gpk_model_animations[i];
		gpk_model_animation_channel *gpk_channels = (gpk_model_animation_channel *)(gpk_file_mapping.ptr + gpk_animation.channel_offset);
		for (uint32 i = 0; i < gpk_animation.channel_count; i += 1) {
			auto &channel = animation.channels[i];
			auto &gpk_channel = gpk_channels[i];
			m_assert(channel.target_node >= 0 && channel.target_node < gltf_model.nodes.size());
			gpk_channel.node_index = (uint32)channel.target_node;
			gpk_channel.channel_type = (
				channel.target_path == "translation" ? gpk_model_animation_translate_channel :
				channel.target_path == "rotation" ? gpk_model_animation_rotate_channel :
				channel.target_path == "scale" ? gpk_model_animation_scale_channel :
				channel.target_path == "weights" ? gpk_model_animation_weights_channel : UINT32_MAX);
			m_assert(gpk_channel.channel_type != UINT32_MAX);
			m_assert(channel.sampler >= 0 && channel.sampler < animation.samplers.size());
			gpk_channel.sampler_index = channel.sampler;
		}
		uint32 key_frame_offset = round_up(gpk_animation.sampler_offset + gpk_animation.sampler_count * (uint32)sizeof(struct gpk_model_animation_sampler), 16u);
		gpk_model_animation_sampler *gpk_samplers = (gpk_model_animation_sampler *)(gpk_file_mapping.ptr + gpk_animation.sampler_offset);
		for (uint32 i = 0; i < gpk_animation.sampler_count; i += 1) {
			auto &sampler = animation.samplers[i];
			auto &gpk_sampler = gpk_samplers[i];
			gpk_sampler.interpolation_type = (
				sampler.interpolation == "LINEAR" ? gpk_model_animation_linear_interpolation :
				sampler.interpolation == "STEP" ? gpk_model_animation_step_interpolation :
				sampler.interpolation == "CATMULLROMSPLINE" ? gpk_model_animation_catmullromspline_interpolation :
				sampler.interpolation == "CUBICSPLINE" ? gpk_model_animation_cubicspline_interpolation : UINT32_MAX);
			m_assert(gpk_sampler.interpolation_type != UINT32_MAX);

			auto &input_accessor = gltf_model.accessors[sampler.input];
			auto &input_buffer_view = gltf_model.bufferViews[input_accessor.bufferView];
			auto &input_buffer = gltf_model.buffers[input_buffer_view.buffer];
			uint32 input_data_stride = input_buffer_view.byteStride == 0 ? 4 : (uint32)input_buffer_view.byteStride;
			uint8 *input_data = &input_buffer.data[input_accessor.byteOffset + input_buffer_view.byteOffset];

			auto &output_accessor = gltf_model.accessors[sampler.output];
			auto &output_buffer_view = gltf_model.bufferViews[output_accessor.bufferView];
			auto &output_buffer = gltf_model.buffers[output_buffer_view.buffer];
			uint32 output_data_stride = (uint32)output_buffer_view.byteStride;
			if (output_data_stride == 0) {
				if (output_accessor.type == TINYGLTF_TYPE_SCALAR) {
					output_data_stride = 4;
				}
				else if (output_accessor.type == TINYGLTF_TYPE_VEC2) {
					output_data_stride = 8;
				}
				else if (output_accessor.type == TINYGLTF_TYPE_VEC3) {
					output_data_stride = 12;
				}
				else if (output_accessor.type == TINYGLTF_TYPE_VEC4) {
					output_data_stride = 16;
				}
				else {
					m_assert(false);
				}
			}
			uint8 *output_data = &output_buffer.data[output_accessor.byteOffset + output_buffer_view.byteOffset];

			gpk_sampler.key_frame_count = (uint32)input_accessor.count;
			gpk_sampler.key_frame_offset = key_frame_offset;
			gpk_model_animation_key_frame *gpk_key_frames = (gpk_model_animation_key_frame *)(gpk_file_mapping.ptr + key_frame_offset);
			for (uint32 i = 0; i < gpk_sampler.key_frame_count; i += 1) {
				auto &gpk_key_frame = gpk_key_frames[i];
				gpk_key_frame.time = *(float *)(input_data + input_data_stride * i);
				float *transform_data = (float * )(output_data + output_data_stride * i);
				gpk_key_frame.transform_data = {transform_data[0], 0, 0, 0};
				if (output_accessor.type == TINYGLTF_TYPE_VEC2) {
					gpk_key_frame.transform_data[1] = transform_data[1];
				}
				else if (output_accessor.type == TINYGLTF_TYPE_VEC3) {
					gpk_key_frame.transform_data[1] = transform_data[1];
					gpk_key_frame.transform_data[2] = transform_data[2];
				}
				else if (output_accessor.type == TINYGLTF_TYPE_VEC4) {
					gpk_key_frame.transform_data[1] = transform_data[1];
					gpk_key_frame.transform_data[2] = transform_data[2];
					gpk_key_frame.transform_data[3] = transform_data[3];
				}
			}
			key_frame_offset = round_up(key_frame_offset + gpk_sampler.key_frame_count * (uint32)sizeof(struct gpk_model_animation_key_frame), 16u);
		}
	}
	uint32 image_index = 0;
	for (size_t i = 0; i < gltf_model.images.size(); i += 1) {
		auto &image = gltf_model.images[i];
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
				m_assert(image.component <= 4);
				uint32 metallic_component_index = 2;
				uint32 roughness_component_index = 1;
				if (image.component == 1) {
					metallic_component_index = 0;
					roughness_component_index = 0;
				}
				else if (image.component == 2) {
					metallic_component_index = 0;
					roughness_component_index = 1;
				}
				for (uint32 i = 0; i < gpk_metallic_image.width * gpk_metallic_image.height; i += 1) {
					gpk_metallic_ptr[i] = image.image[i * image.component + metallic_component_index];
				}
				for (uint32 i = 0; i < gpk_roughness_image.width * gpk_roughness_image.height; i += 1) {
					gpk_roughness_ptr[i] = image.image[i * image.component + roughness_component_index];
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
	
	printf("done importing gltf: \"%s\" %d\n", gltf_file.c_str(), current_offset);
}

struct import_json_schema {
	bool force_import_all;
	bool force_import_models;
	bool force_import_skyboxes;
	struct model {
		bool import;
		std::string gltf_file;
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
		import.models.push_back({m["import"], m["gltf_file"], m["gpk_file"]});
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
	import_json_schema import;
	try {
		json_file_fstream >> json_content;
		import = json_content;
	}
	catch (nlohmann::json::exception &e) {
		fatal("import.exe json exception:\n\n%s\n", e.what());
	}
	for (auto &m : import.models) {
		if (m.import || import.force_import_all || import.force_import_models) {
			std::string cmdl_str = std::string("import.exe -gltf-to-gpk ") + m.gltf_file + " " + m.gpk_file;
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
		if (!strcmp(mode_str, "-gltf-to-gpk")) {
			if (argc == 4) {
				gltf_to_gpk(argv[2], argv[3]);
			}
			else {
				printf("error: expect -gltf-to-gpk gltf_file gpk_file");
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
