/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"
#include "math.cpp"

#include <initguid.h>
#include "../vendor/include/uvatlas/uvatlas.h"
#include "../vendor/include/directxmesh/directxmesh.h"

#include "vulkan.cpp"
#include "gpk.cpp"

int main(int argc, char **argv) {
	set_exe_dir_as_current();
	if (argc != 2) {
		printf("incorrect command line arguments");
		return 1;
	}
	char *gpk_file = argv[1];
	file_mapping gpk_file_mapping = {};
	if (!open_file_mapping(gpk_file, &gpk_file_mapping)) {
		printf("cannot open file %s", gpk_file);
		return 1;
	}
	m_scope_exit(close_file_mapping(&gpk_file_mapping));
	gpk_model *gpk_model = (struct gpk_model *)gpk_file_mapping.ptr;
	uint32 total_vertex_count = 0;
	uint32 total_index_count = 0;
	// for (uint32 i = 0; i < gpk_model->node_count; i += 1) {
	// 	gpk_model_mesh *gpk_model_mesh = ((struct gpk_model_mesh *)(gpk_file_mapping.ptr + gpk_model->mesh_offset)) + i;
	// 	total_vertex_count += gpk_model_mesh->vertex_count;
	// 	total_index_count += gpk_model_mesh->index_count;
	// }
	m_assert(total_index_count % 3 == 0);
	DirectX::XMFLOAT3 *positions = (DirectX::XMFLOAT3 *)calloc(total_vertex_count, sizeof(DirectX::XMFLOAT3));
	uint32 *indices = (uint32 *)calloc(total_index_count, sizeof(uint32));
	uint32 *adjaceny_indices = (uint32 *)calloc(total_index_count, sizeof(uint32));
	uint32 accumulate_position_index = 0;
	uint32 accumulate_index_index = 0;
	// for (uint32 i = 0; i < gpk_model->node_count; i += 1) {
	// 	gpk_model_mesh *gpk_model_mesh = ((struct gpk_model_mesh *)(gpk_file_mapping.ptr + gpk_model->mesh_offset)) + i;
	// 	uint8 *gpk_indices = gpk_file_mapping.ptr + gpk_model_mesh->indices_offset;
	// 	uint8 *gpk_vertices = gpk_file_mapping.ptr + gpk_model_mesh->vertices_offset;
	// 	for (uint32 i = 0; i < gpk_model_mesh->index_count; i += 1) {
	// 		uint16 index = *(uint16 *)(gpk_indices + sizeof(uint16) * i);
	// 		indices[accumulate_index_index + i] = accumulate_position_index + index;
	// 	}
	// 	for (uint32 i = 0; i < gpk_model_mesh->vertex_count; i += 1) {
	// 		vec3 vertex = *(vec3 *)(gpk_vertices + sizeof(gpk_model_vertex) * i);
	// 		positions[accumulate_position_index + i] = {vertex.x, vertex.y, vertex.z};
	// 	}
	// 	accumulate_index_index += gpk_model_mesh->index_count;
	// 	accumulate_position_index += gpk_model_mesh->vertex_count;
	// }
	HRESULT generate_adjacency_result = GenerateAdjacencyAndPointReps(indices, total_index_count / 3, positions, total_vertex_count, 0, nullptr, adjaceny_indices);
	m_assert(generate_adjacency_result == S_OK);
	std::vector<DirectX::UVAtlasVertex> mesh_vertex_buffer;
	static_assert(sizeof(DirectX::UVAtlasVertex) == 20, "");
	std::vector<uint8_t> mesh_index_buffer;
	std::vector<uint32> mesh_vertex_remap_buffer;
	std::function<HRESULT(float)> status_callback = [](float percentage) {
		static float progress = 1;
		percentage *= 100;
		if (percentage >= progress) {
			printf("%d%%\n", (int32)(percentage));
			progress = ceilf(percentage);
		}
		return S_OK;
	};
	uint32 lightmap_width = 4096;
	uint32 lightmap_height = 4096;
	HRESULT uv_atlas_create_result = UVAtlasCreate(positions, total_vertex_count, indices, DXGI_FORMAT_R32_UINT, total_index_count / 3, 0, 0, lightmap_width, lightmap_height, 1, adjaceny_indices, nullptr, nullptr, status_callback, 1, 0, mesh_vertex_buffer, mesh_index_buffer, nullptr, &mesh_vertex_remap_buffer, nullptr, nullptr);
	m_assert(uv_atlas_create_result == S_OK);

	vulkan_device vulkan_device = {};
	// initialize_vulkan_device(&vulkan_device);
	VkRenderPass vulkan_render_pass = {};
	VkImage vulkan_framebuffer_attachment_image = {};
	VkImageView vulkan_framebuffer_attachment_image_view = {};
	VkDeviceMemory vulkan_framebuffer_attachment_image_memory = {};
	VkFramebuffer vulkan_framebuffer = {};
	VkCommandPool vulkan_cmd_pool = {};
	VkCommandBuffer vulkan_cmd_buffer = {};
	VkFence vulkan_fence = {};
	VkPipeline vulkan_pipeline = {};
	VkPipelineLayout vulkan_pipeline_layout = {};
	VkBuffer vulkan_vertex_buffer = {};
	VkDeviceMemory vulkan_vertex_buffer_memory = {};
	{
		VkAttachmentDescription attachments[1] = {};
		attachments[0].format = VK_FORMAT_R8G8B8A8_UNORM;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference color_attachment_refs[1] = {{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};
		VkSubpassDescription subpasses[1] = {};
		subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpasses[0].colorAttachmentCount = m_countof(color_attachment_refs);
		subpasses[0].pColorAttachments = color_attachment_refs;

		VkRenderPassCreateInfo create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
		create_info.attachmentCount = m_countof(attachments);
		create_info.pAttachments = attachments;
		create_info.subpassCount = m_countof(subpasses);
		create_info.pSubpasses = subpasses;
		m_vk_assert(vkCreateRenderPass(vulkan_device.device, &create_info, nullptr, &vulkan_render_pass));
	}
	{
		VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = VK_FORMAT_R8G8B8A8_UNORM;
		image_create_info.extent = {lightmap_width, lightmap_height, 1};
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImageViewCreateInfo image_view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = image_create_info.format;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.layerCount = 1;
		// vulkan_allocate_image(&vulkan_device, image_create_info, image_view_create_info, &vulkan_framebuffer_attachment_image_memory, &vulkan_framebuffer_attachment_image, &vulkan_framebuffer_attachment_image_view);

		VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
		framebuffer_create_info.renderPass = vulkan_render_pass;
		framebuffer_create_info.attachmentCount = 1;
		framebuffer_create_info.pAttachments = &vulkan_framebuffer_attachment_image_view;
		framebuffer_create_info.width = lightmap_width;
		framebuffer_create_info.height = lightmap_height;
		framebuffer_create_info.layers = 1;
		m_vk_assert(vkCreateFramebuffer(vulkan_device.device, &framebuffer_create_info, nullptr, &vulkan_framebuffer));
	}
	{
		VkCommandPoolCreateInfo cmd_pool_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
		cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		cmd_pool_info.queueFamilyIndex = vulkan_device.graphic_queue_family;
		m_vk_assert(vkCreateCommandPool(vulkan_device.device, &cmd_pool_info, nullptr, &vulkan_cmd_pool));

		VkCommandBufferAllocateInfo cmd_buffer_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
		cmd_buffer_info.commandPool = vulkan_cmd_pool;
		cmd_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmd_buffer_info.commandBufferCount = 1;
		m_vk_assert(vkAllocateCommandBuffers(vulkan_device.device, &cmd_buffer_info, &vulkan_cmd_buffer));

		VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
		m_vk_assert(vkCreateFence(vulkan_device.device, &fence_create_info, nullptr, &vulkan_fence));
	}
	{
		auto shader_module_from_file = [&vulkan_device](const char *file_path) {
			VkShaderModule shader_module = {};
			file_mapping file_mapping = {};
			m_assert(open_file_mapping(file_path, &file_mapping));
			VkShaderModuleCreateInfo info = {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
			info.codeSize = file_mapping.size;
			info.pCode = (const uint32 *)file_mapping.ptr;
			m_vk_assert(vkCreateShaderModule(vulkan_device.device, &info, nullptr, &shader_module));
			close_file_mapping(&file_mapping);
			return shader_module;
		};
		VkPipelineShaderStageCreateInfo shader_stages[2] = {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}, {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO}};
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = shader_module_from_file("shaders\\lightmap.vert.spv");
		shader_stages[0].pName = "main";
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = shader_module_from_file("shaders\\lightmap.frag.spv");
		shader_stages[1].pName = "main";

		VkVertexInputBindingDescription vertex_input_bindings[1] = {0, 20, VK_VERTEX_INPUT_RATE_VERTEX};
		VkVertexInputAttributeDescription vertex_attributes[2] = {};
		vertex_attributes[0] = {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0};
		vertex_attributes[1] = {1, 0, VK_FORMAT_R32G32_SFLOAT, 12};

		VkPipelineVertexInputStateCreateInfo vertex_input_state = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
		vertex_input_state.vertexBindingDescriptionCount = m_countof(vertex_input_bindings);
		vertex_input_state.pVertexBindingDescriptions = vertex_input_bindings;
		vertex_input_state.vertexAttributeDescriptionCount = m_countof(vertex_attributes);
		vertex_input_state.pVertexAttributeDescriptions = vertex_attributes;

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
		input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo viewport_state = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterization_state = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
		rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_state.cullMode = VK_CULL_MODE_NONE;
		rasterization_state.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo multisample_state = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
		multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
		depth_stencil_state.depthTestEnable = VK_FALSE;
		depth_stencil_state.depthWriteEnable = VK_FALSE;
		depth_stencil_state.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.colorWriteMask = 0xf;
		VkPipelineColorBlendStateCreateInfo color_blend_state = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
		color_blend_state.logicOpEnable = VK_FALSE;
		color_blend_state.attachmentCount = 1;
		color_blend_state.pAttachments = &color_blend_attachment_state;

		VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamic_state = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
		dynamic_state.dynamicStateCount = m_countof(dynamic_states);
		dynamic_state.pDynamicStates = dynamic_states;

		VkPipelineLayoutCreateInfo layout_info = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
		layout_info.setLayoutCount = 0;
		layout_info.pSetLayouts = nullptr;
		layout_info.pushConstantRangeCount = 0;
		layout_info.pPushConstantRanges = nullptr;
		VkPipelineLayout layout = {};
		m_vk_assert(vkCreatePipelineLayout(vulkan_device.device, &layout_info, nullptr, &vulkan_pipeline_layout));

		VkGraphicsPipelineCreateInfo create_info = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
		create_info.stageCount = m_countof(shader_stages);
		create_info.pStages = shader_stages;
		create_info.pVertexInputState = &vertex_input_state;
		create_info.pInputAssemblyState = &input_assembly_state;
		create_info.pTessellationState = nullptr;
		create_info.pViewportState = &viewport_state;
		create_info.pRasterizationState = &rasterization_state;
		create_info.pMultisampleState = &multisample_state;
		create_info.pDepthStencilState = &depth_stencil_state;
		create_info.pColorBlendState = &color_blend_state;
		create_info.pDynamicState = &dynamic_state;
		create_info.layout = vulkan_pipeline_layout;
		create_info.renderPass = vulkan_render_pass;
		create_info.subpass = 0;
		create_info.basePipelineHandle = VK_NULL_HANDLE;
		m_vk_assert(vkCreateGraphicsPipelines(vulkan_device.device, VK_NULL_HANDLE, 1, &create_info, nullptr, &vulkan_pipeline));
	}
	{
		VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_create_info.size = mesh_vertex_buffer.size() * sizeof(DirectX::UVAtlasVertex) + mesh_index_buffer.size();
		buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		// m_assert(vulkan_allocate_buffer(&vulkan_device, buffer_create_info, &vulkan_vertex_buffer_memory, &vulkan_vertex_buffer));
		uint8 *vertex_buffer = nullptr;
		vkMapMemory(vulkan_device.device, vulkan_vertex_buffer_memory, 0, VK_WHOLE_SIZE, 0, (void **)&vertex_buffer);
		memcpy(vertex_buffer, mesh_vertex_buffer.data(), mesh_vertex_buffer.size() * sizeof(DirectX::UVAtlasVertex));
		uint8 *index_buffer = vertex_buffer + mesh_vertex_buffer.size() * sizeof(DirectX::UVAtlasVertex);
		memcpy(index_buffer, mesh_index_buffer.data(), mesh_index_buffer.size());
		vkUnmapMemory(vulkan_device.device, vulkan_vertex_buffer_memory);
	}
	{
		VkDeviceMemory staging_memory = {};
		VkBuffer staging_buffer = {};
		VkBufferCreateInfo buffer_create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
		buffer_create_info.size = lightmap_width * lightmap_height * 4;
		buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		// m_assert(vulkan_allocate_buffer(&vulkan_device, buffer_create_info, &staging_memory, &staging_buffer));

		VkCommandBufferBeginInfo cmd_buffer_begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
		cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		m_vk_assert(vkBeginCommandBuffer(vulkan_cmd_buffer, &cmd_buffer_begin_info));

		VkClearValue clear_values[] = {{0, 0, 0, 0}};
		VkRenderPassBeginInfo render_pass_begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
		render_pass_begin_info.renderPass = vulkan_render_pass;
		render_pass_begin_info.framebuffer = vulkan_framebuffer;
		render_pass_begin_info.renderArea.offset = {0, 0};
		render_pass_begin_info.renderArea.extent = {lightmap_width, lightmap_height};
		render_pass_begin_info.clearValueCount = m_countof(clear_values);
		render_pass_begin_info.pClearValues = clear_values;
		vkCmdBeginRenderPass(vulkan_cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(vulkan_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline);
		VkViewport viewport = {0, 0, (float)lightmap_width, (float)lightmap_height, 0, 1};
		VkRect2D scissor = {{0, 0}, lightmap_width, lightmap_height};
		vkCmdSetViewport(vulkan_cmd_buffer, 0, 1, &viewport);
		vkCmdSetScissor(vulkan_cmd_buffer, 0, 1, &scissor);
		VkDeviceSize vertex_buffer_offset = 0;
		vkCmdBindVertexBuffers(vulkan_cmd_buffer, 0, 1, &vulkan_vertex_buffer, &vertex_buffer_offset);
		vkCmdBindIndexBuffer(vulkan_cmd_buffer, vulkan_vertex_buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(vulkan_cmd_buffer, (uint32)mesh_index_buffer.size() / 4, 1, (uint32)mesh_vertex_buffer.size() * sizeof(DirectX::UVAtlasVertex) / 4, 0, 0);

		vkCmdEndRenderPass(vulkan_cmd_buffer);

		VkImageMemoryBarrier image_memory_barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
		image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		image_memory_barrier.image = vulkan_framebuffer_attachment_image;
		image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_memory_barrier.subresourceRange.levelCount = 1;
		image_memory_barrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(vulkan_cmd_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
		VkBufferImageCopy copy_region = {0, 0, 0, {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1}, {0, 0, 0}, {lightmap_width, lightmap_height, 1}};
		vkCmdCopyImageToBuffer(vulkan_cmd_buffer, vulkan_framebuffer_attachment_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, staging_buffer, 1, &copy_region);

		m_vk_assert(vkEndCommandBuffer(vulkan_cmd_buffer));

		VkSubmitInfo queue_submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
		queue_submit_info.commandBufferCount = 1;
		queue_submit_info.pCommandBuffers = &vulkan_cmd_buffer;
		m_vk_assert(vkQueueSubmit(vulkan_device.graphic_queue, 1, &queue_submit_info, vulkan_fence));

		vkWaitForFences(vulkan_device.device, 1, &vulkan_fence, VK_TRUE, UINT64_MAX);
		uint8 *staging_memory_ptr = nullptr;
		vkMapMemory(vulkan_device.device, staging_memory, 0, VK_WHOLE_SIZE, 0, (void **)&staging_memory_ptr);
		flip_rgba_image(staging_memory_ptr, lightmap_width, lightmap_height);
		rgba_image_to_bmp_file("lightmap.bmp", staging_memory_ptr, lightmap_width, lightmap_height);
		vkUnmapMemory(vulkan_device.device, staging_memory);
		vkDestroyBuffer(vulkan_device.device, staging_buffer, nullptr);
		vkFreeMemory(vulkan_device.device, staging_memory, nullptr);
	}
}
