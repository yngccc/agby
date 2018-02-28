/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

struct level;
struct editor;
void level_reset(vulkan *vulkan, level *level);
void level_load_json(vulkan *vulkan, const char *level_json_file, level *level);
void editor_initialize(vulkan *vulkan, editor *editor);

struct stbtt_font {
	stbtt_fontinfo stbtt_font_info;
	stbtt_pack_context stbtt_pack_context;
	stbtt_packedchar *stbtt_packed_chars;
	float font_size;
	uint32 packed_bitmap_width;
	uint32 packed_bitmap_height;
};

struct main_menu {
	const char *entries[3];
	uint32 entry_index;
	uint32 entries_text_vertices_count;
	uint32 entries_text_frame_vertices_buffer_offset;
	uint32 entries_text_frame_uniforms_buffer_offset;
	uint32 imgui_frame_vertices_buffer_offset;
	bool show_frame_stats;
};

void main_menu_create(main_menu *main_menu) {
	main_menu->entries[0] = "Play";
	main_menu->entries[1] = "Options";
	main_menu->entries[2] = "Exit";
	main_menu->entry_index = 0;
	main_menu->entries_text_vertices_count = 0;
	for (uint32 i = 0; i < m_countof(main_menu->entries); i += 1) {
		main_menu->entries_text_vertices_count += 6 * (uint32)strlen(main_menu->entries[i]);
	}
}

void imgui_frame_stats_update(bool *show_frame_stats, double last_frame_time) {
	if (ImGui::IsKeyPressed(VK_F6)) {
		*show_frame_stats = !*show_frame_stats;
	}
	if (*show_frame_stats) {
		if (ImGui::Begin("##frame_stats", nullptr, ImVec2{}, 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
			ImGui::Text("frame_time: %.1f ms", last_frame_time * 1000.0);
		}
		ImGui::End();
	}
}

void imgui_update_render_memory(vulkan *vulkan, uint32 *frame_vertices_buffer_offset) {
	round_up(&vulkan->buffers.frame_vertices_buffer_offset, sizeof(ImDrawVert));
	*frame_vertices_buffer_offset = vulkan->buffers.frame_vertices_buffer_offset;
	ImDrawData *imgui_draw_data = ImGui::GetDrawData();
	for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
		ImDrawList *dlist = imgui_draw_data->CmdLists[i];
		uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
		uint32 indices_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
		memcpy(vulkan->buffers.frame_vertices_buffer_ptr + vulkan->buffers.frame_vertices_buffer_offset, dlist->VtxBuffer.Data, vertices_size);
		memcpy(vulkan->buffers.frame_vertices_buffer_ptr + vulkan->buffers.frame_vertices_buffer_offset + vertices_size, dlist->IdxBuffer.Data, indices_size);
		vulkan->buffers.frame_vertices_buffer_offset += round_up(vertices_size + indices_size, (uint32)sizeof(ImDrawVert));
	}
};

void imgui_submit_render_commands(vulkan *vulkan, uint32 frame_vertices_buffer_offset) {
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffer;
	vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.imgui_pipeline.pipeline);
	VkViewport viewport = { 0, 0, (float)vulkan->framebuffers.main_framebuffer.width, (float)vulkan->framebuffers.main_framebuffer.height, 1, 0 };
	VkRect2D scissor = { {0, 0}, vulkan->framebuffers.main_framebuffer.width, vulkan->framebuffers.main_framebuffer.height };
	vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
	vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
	VkDeviceSize vertices_offset = 0;
	vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.frame_vertices_buffer.buffer, &vertices_offset);
	vkCmdBindIndexBuffer(cmd_buffer, vulkan->buffers.frame_vertices_buffer.buffer, vertices_offset, VK_INDEX_TYPE_UINT16);
	vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.imgui_pipeline.layout, 0, 1, &vulkan->descriptor_sets.imgui_image_descriptor_set, 0, nullptr);
	vec2 push_consts = { (float)vulkan->framebuffers.main_framebuffer.width, (float)vulkan->framebuffers.main_framebuffer.height };
	vkCmdPushConstants(cmd_buffer, vulkan->pipelines.imgui_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(push_consts), &push_consts);
	ImDrawData *imgui_draw_data = ImGui::GetDrawData();
	for (int32 i = 0; i < imgui_draw_data->CmdListsCount; i += 1) {
		ImDrawList *dlist = imgui_draw_data->CmdLists[i];
		uint32 vertices_size = dlist->VtxBuffer.Size * sizeof(ImDrawVert);
		uint32 elements_size = dlist->IdxBuffer.Size * sizeof(ImDrawIdx);
		uint32 vertex_index = frame_vertices_buffer_offset / sizeof(ImDrawVert);
		uint32 element_index = (frame_vertices_buffer_offset + vertices_size) / sizeof(ImDrawIdx);
		for (int32 i = 0; i < dlist->CmdBuffer.Size; i += 1) {
			ImDrawCmd *dcmd = &dlist->CmdBuffer.Data[i];
			VkRect2D scissor = { {(int32)dcmd->ClipRect.x, (int32)dcmd->ClipRect.y}, {(uint32)(dcmd->ClipRect.z - dcmd->ClipRect.x), (uint32)(dcmd->ClipRect.w - dcmd->ClipRect.y)} };
			vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
			vkCmdDrawIndexed(cmd_buffer, dcmd->ElemCount, 1, element_index, vertex_index, 0);
			element_index += dcmd->ElemCount;
		}
		frame_vertices_buffer_offset += round_up(vertices_size + elements_size, sizeof(ImDrawVert));
	}
};

void swap_chain_submit_render_commands(vulkan *vulkan, uint32 swap_chain_image_index) {
	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffer;
	VkClearValue clear_value = { 0, 0, 0, 1 };
	VkRenderPassBeginInfo render_pass_begin_info = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	render_pass_begin_info.renderPass = vulkan->render_passes.swap_chain_render_pass;
	render_pass_begin_info.framebuffer = vulkan->framebuffers.swap_chain_framebuffers[swap_chain_image_index];
	render_pass_begin_info.renderArea.offset = { 0, 0 };
	render_pass_begin_info.renderArea.extent = { vulkan->swap_chain.image_width, vulkan->swap_chain.image_height };
	render_pass_begin_info.clearValueCount = 1;
	render_pass_begin_info.pClearValues = &clear_value;
	vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.swap_chain_pipeline.pipeline);
	VkViewport viewport = { 0, 0, (float)vulkan->swap_chain.image_width, (float)vulkan->swap_chain.image_height, 0, 1 };
	VkRect2D scissor = { {0, 0}, {vulkan->swap_chain.image_width, vulkan->swap_chain.image_height} };
	vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
	vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
	vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.swap_chain_pipeline.layout, 0, 1, &vulkan->descriptor_sets.swap_chain_image_descriptor_set, 0, nullptr);
	vkCmdDraw(cmd_buffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(cmd_buffer);
};

void main_menu_update(main_menu *main_menu, level *level, editor *editor, vulkan *vulkan, state *state, double last_frame_time) {
	ImGui::NewFrame();
	imgui_frame_stats_update(&main_menu->show_frame_stats, last_frame_time);
	ImGui::Render();
	if (ImGui::IsKeyPressed(VK_UP, false)) {
		if (main_menu->entry_index == 0) {
			main_menu->entry_index = m_countof(main_menu->entries) - 1;
		}
		else {
			main_menu->entry_index = main_menu->entry_index - 1;
		}
	}
	if (ImGui::IsKeyPressed(VK_DOWN, false)) {
		if (main_menu->entry_index == m_countof(main_menu->entries) - 1) {
			main_menu->entry_index = 0;
		}
		else {
			main_menu->entry_index = main_menu->entry_index + 1;
		}
	}
	if (ImGui::IsKeyPressed(VK_RETURN, false)) {
		if (main_menu->entry_index == 0) {
			level_reset(vulkan, level);
			level_load_json(vulkan, "agby_assets\\levels\\level_0.json", level);
			editor_initialize(vulkan, editor);
			*state = state_edit;
		}
		else if (main_menu->entry_index == 2) {
			*state = state_exit;
		}
	}
}

void main_menu_update_render_memory(main_menu *main_menu, vulkan *vulkan, stbtt_font *font) {
	vkWaitForFences(vulkan->device.device, 1, &vulkan->syncs.queue_submit_fence, VK_TRUE, UINT64_MAX);
	vkResetFences(vulkan->device.device, 1, &vulkan->syncs.queue_submit_fence);

	vulkan->buffers.frame_vertices_buffer_offset = 0;
	vulkan->buffers.frame_uniforms_buffer_offset = 0;
	{
		main_menu->entries_text_frame_vertices_buffer_offset = 0;
		main_menu->entries_text_frame_uniforms_buffer_offset = 0;

		float pen_x = 0;
		float pen_y = 0;
		float bound_x0 = 0;
		float bound_y0 = 0;
		float bound_x1 = 0;
		float bound_y1 = 0;
		for (uint32 i = 0; i < m_countof(main_menu->entries); i += 1) {
			const char *entry = main_menu->entries[i];
			uint32 entry_len = (uint32)strlen(entry);
			u8vec4 color = (i == main_menu->entry_index) ? u8vec4{ 255, 0, 0, 255 } : u8vec4{ 255, 255, 255, 255 };
			for (uint32 i = 0; i < entry_len; i += 1) {
				char character = entry[i];
				stbtt_aligned_quad quad = {};
				stbtt_GetPackedQuad(font->stbtt_packed_chars, font->packed_bitmap_width, font->packed_bitmap_height, character - ' ', &pen_x, &pen_y, &quad, 0);
				bound_x0 = min(bound_x0, quad.x0);
				bound_y0 = min(bound_y0, quad.y0);
				bound_x1 = max(bound_x1, quad.x1);
				bound_y1 = max(bound_y1, quad.y1);
				struct vertex {
					vec4 xyst;
					u8vec4 rgba;
					uint32 mat_index;
				} *vertices = (struct vertex *)(vulkan->buffers.frame_vertices_buffer_ptr + vulkan->buffers.frame_vertices_buffer_offset);
				static_assert(sizeof(vertex) == 24, "");
				vertices[0] = { {quad.x0, quad.y0, quad.s0, quad.t0}, color, 0 };
				vertices[1] = { {quad.x0, quad.y1, quad.s0, quad.t1}, color, 0 };
				vertices[2] = { {quad.x1, quad.y1, quad.s1, quad.t1}, color, 0 };
				vertices[3] = vertices[0];
				vertices[4] = vertices[2];
				vertices[5] = { {quad.x1, quad.y0, quad.s1, quad.t0}, color, 0 };
				vulkan->buffers.frame_vertices_buffer_offset += 6 * sizeof(vertex);
			}
			pen_x = 0;
			pen_y += font->font_size;
		}
		float bound_width = bound_x1 - bound_x0;
		float bound_height = bound_y1 - bound_y0;
		float bound_center_x = bound_x0 + bound_width / 2;
		float bound_center_y = bound_y0 + bound_height / 2;
		float scale = vulkan->framebuffers.main_framebuffer.height / 3 / bound_height;
		float center_dx = (float)vulkan->framebuffers.main_framebuffer.width / 2;
		float center_dy = (float)vulkan->framebuffers.main_framebuffer.height / 2;
		mat4 transform_mat = mat4_vulkan_clip() * mat4_orthographic_projection(0, (float)vulkan->framebuffers.main_framebuffer.width, (float)vulkan->framebuffers.main_framebuffer.height, 0, 0, 1) * mat4_from_translation({ center_dx, center_dy, 0 }) * mat4_from_scaling({ scale, scale, 1 }) * mat4_from_translation({ -bound_center_x, -bound_center_y, 0 });
		*(mat4 *)(vulkan->buffers.frame_uniforms_buffer_ptr + vulkan->buffers.frame_uniforms_buffer_offset) = transform_mat;
		vulkan->buffers.frame_uniforms_buffer_offset += sizeof(mat4);
	}
	imgui_update_render_memory(vulkan, &main_menu->imgui_frame_vertices_buffer_offset);
}

void main_menu_submit_render_commands(main_menu *main_menu, vulkan *vulkan) {
	uint32 swap_chain_image_index = 0;
	m_vk_assert(vkAcquireNextImageKHR(vulkan->device.device, vulkan->swap_chain.swap_chain, UINT64_MAX, vulkan->syncs.swap_chain_image_acquire_semaphore, VK_NULL_HANDLE, &swap_chain_image_index));

	VkCommandBuffer cmd_buffer = vulkan->cmd_buffers.graphic_cmd_buffer;
	VkCommandBufferBeginInfo cmd_buffer_begin_info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	cmd_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	m_vk_assert(vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info));
	{
		VkClearValue clear_values[2] = { {0, 0, 0, 1}, {0, 0} };
		VkRenderPassBeginInfo render_pass_begin_info = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
		render_pass_begin_info.renderPass = vulkan->render_passes.main_render_pass;
		render_pass_begin_info.framebuffer = vulkan->framebuffers.main_framebuffer.framebuffer;
		render_pass_begin_info.renderArea.offset = { 0, 0 };
		render_pass_begin_info.renderArea.extent = { vulkan->framebuffers.main_framebuffer.width, vulkan->framebuffers.main_framebuffer.height };
		render_pass_begin_info.clearValueCount = m_countof(clear_values);
		render_pass_begin_info.pClearValues = clear_values;
		vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.text_pipeline.pipeline);
		VkViewport viewport = { 0, 0, (float)vulkan->framebuffers.main_framebuffer.width, (float)vulkan->framebuffers.main_framebuffer.height, 1, 0 };
		VkRect2D scissor = { {0, 0}, vulkan->framebuffers.main_framebuffer.width, vulkan->framebuffers.main_framebuffer.height };
		vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
		vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);
		VkDeviceSize vertices_offset = 0;
		vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.frame_vertices_buffer.buffer, &vertices_offset);
		VkDescriptorSet descriptor_sets[2] = { vulkan->descriptor_sets.text_uniform_descriptor_set, vulkan->descriptor_sets.text_image_descriptor_set };
		vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.text_pipeline.layout, 0, m_countof(descriptor_sets), descriptor_sets, 1, &main_menu->entries_text_frame_uniforms_buffer_offset);
		vkCmdDraw(cmd_buffer, main_menu->entries_text_vertices_count, 1, main_menu->entries_text_frame_vertices_buffer_offset / 24, 0);

		imgui_submit_render_commands(vulkan, main_menu->imgui_frame_vertices_buffer_offset);

		vkCmdEndRenderPass(cmd_buffer);

		swap_chain_submit_render_commands(vulkan, swap_chain_image_index);
	}
	m_vk_assert(vkEndCommandBuffer(cmd_buffer));

	VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo queue_submit_info = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	queue_submit_info.waitSemaphoreCount = 1;
	queue_submit_info.pWaitSemaphores = &vulkan->syncs.swap_chain_image_acquire_semaphore;
	queue_submit_info.pWaitDstStageMask = &wait_dst_stage_mask;
	queue_submit_info.commandBufferCount = 1;
	queue_submit_info.pCommandBuffers = &vulkan->cmd_buffers.graphic_cmd_buffer;
	queue_submit_info.signalSemaphoreCount = 1;
	queue_submit_info.pSignalSemaphores = &vulkan->syncs.queue_submit_semaphore;
	m_vk_assert(vkQueueSubmit(vulkan->device.queue, 1, &queue_submit_info, vulkan->syncs.queue_submit_fence));

	VkPresentInfoKHR device_queue_present_info = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	device_queue_present_info.waitSemaphoreCount = 1;
	device_queue_present_info.pWaitSemaphores = &vulkan->syncs.queue_submit_semaphore;
	device_queue_present_info.swapchainCount = 1;
	device_queue_present_info.pSwapchains = &vulkan->swap_chain.swap_chain;
	device_queue_present_info.pImageIndices = &swap_chain_image_index;
	m_vk_assert(vkQueuePresentKHR(vulkan->device.queue, &device_queue_present_info));
}
