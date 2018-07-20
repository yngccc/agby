{ // lines
	round_up(&vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index], 16u);
	editor->render_data.lines_frame_vertex_buffer_offset = vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index];
	struct line_point {
		vec3 position;
		u8vec4 color;
	};
	static_assert(sizeof(struct line_point) == 16, "");
	line_point *line_points = (struct line_point *)(vulkan->buffers.frame_vertex_buffer_ptrs[vulkan->frame_index] + editor->render_data.lines_frame_vertex_buffer_offset);
	if (editor->show_reference_grid) {
		u8vec4 color = u8vec4{200, 200, 200, 255};
		line_point horizontal_points[20];
		float horizontal_z = -4.5f;
		for (uint32 i = 0; i < m_countof(horizontal_points) / 2; i += 1) {
			horizontal_points[i * 2] = {{-4.5f, 0, horizontal_z}, color};
			horizontal_points[i * 2 + 1] = {{4.5f, 0, horizontal_z}, color};
			horizontal_z += 1;
		}
		line_point vertical_points[20];
		float vertical_x = -4.5f;
		for (uint32 i = 0; i < m_countof(vertical_points) / 2; i += 1) {
			vertical_points[i * 2] = {{vertical_x, 0, -4.5f}, color};
			vertical_points[i * 2 + 1] = {{vertical_x, 0, 4.5f}, color};
			vertical_x += 1;
		}
		memcpy(line_points, horizontal_points, sizeof(horizontal_points));
		memcpy(line_points + m_countof(horizontal_points), vertical_points, sizeof(vertical_points));
		line_points += m_countof(horizontal_points) + m_countof(vertical_points);
		editor->render_data.lines_vertex_count += m_countof(horizontal_points) + m_countof(vertical_points);
		vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] += sizeof(horizontal_points) + sizeof(vertical_points);
	}
}

uint32 uniform_buffer_offsets[4] = {level->render_data.frame_uniform_buffer_offset, 0, 0, 0};
if (editor->show_reference_grid) {
	vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.lines_pipeline);
	vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &vulkan->buffers.frame_vertex_buffers[vulkan->frame_index].buffer, &vertices_offset);
	vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines.lines_pipeline.layout, 0, 1, &vulkan->descriptors.frame_uniform_buffer_offsets[vulkan->frame_index], m_countof(uniform_buffer_offsets), uniform_buffer_offsets);
	vkCmdDraw(cmd_buffer, editor->render_data.lines_vertex_count, 1, editor->render_data.lines_frame_vertex_buffer_offset / 16, 0);
}

stbtt_fontinfo stbtt_font_info;
stbtt_pack_context stbtt_pack_context;
stbtt_packedchar stbtt_packed_chars[95];
float font_size;
uint32 font_atlas_width;
uint32 font_atlas_height;
uint32 font_atlas_descriptor_index;

file_mapping font_file_mapping = {};
m_assert(open_file_mapping("assets\\fonts\\Roboto-Medium.ttf", &font_file_mapping));
m_scope_exit(close_file_mapping(&font_file_mapping));

m_assert(stbtt_InitFont(&level->persistant_data.stbtt_font_info, font_file_mapping.ptr, 0));
level->persistant_data.font_size = 128;
level->persistant_data.font_atlas_width = 2048;
level->persistant_data.font_atlas_height = 1024;
uint8 *font_atlas = (uint8 *)allocate_virtual_memory(level->persistant_data.font_atlas_width * level->persistant_data.font_atlas_height);
m_scope_exit(free_virtual_memory(font_atlas));
m_assert(stbtt_PackBegin(&level->persistant_data.stbtt_pack_context, font_atlas, level->persistant_data.font_atlas_width, level->persistant_data.font_atlas_height, 0, 1, nullptr));
stbtt_PackSetOversampling(&level->persistant_data.stbtt_pack_context, 2, 2);
m_assert(stbtt_PackFontRange(&level->persistant_data.stbtt_pack_context, font_file_mapping.ptr, 0, level->persistant_data.font_size, 32, 95, level->persistant_data.stbtt_packed_chars));
stbtt_PackEnd(&level->persistant_data.stbtt_pack_context);

VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
image_info.imageType = VK_IMAGE_TYPE_2D;
image_info.format = VK_FORMAT_R8_UNORM;
image_info.extent = {level->persistant_data.font_atlas_width, level->persistant_data.font_atlas_height, 1};
image_info.mipLevels = 1;
image_info.arrayLayers = 1;
image_info.samples = VK_SAMPLE_COUNT_1_BIT;
image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
VkImageViewCreateInfo image_view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
image_view_info.format = image_info.format;
image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
image_view_info.subresourceRange.levelCount = 1;
image_view_info.subresourceRange.layerCount = 1;
uint32 image_index = append_vulkan_image_region(vulkan, image_info, image_view_info, font_atlas, level->persistant_data.font_atlas_width * level->persistant_data.font_atlas_height, 1, 1);
level->persistant_data.font_atlas_descriptor_index = append_vulkan_combined_2d_image_samplers(vulkan, image_index, vulkan->samplers.mipmap_samplers[0]);

{ // texts
	struct vertex {
		vec4 position_uv;
		u8vec4 color;
		uint32 transform_mat_index;
	};
	static_assert(sizeof(struct vertex) == 24, "");
	round_up(&vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index], (uint32)sizeof(struct vertex));
	round_up(&vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index], 64u);
	level->render_data.text_frame_vertex_buffer_offset = vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index];
	level->render_data.text_frame_uniform_buffer_offset = vulkan->buffers.frame_uniform_buffer_offsets[vulkan->frame_index];
	level->render_data.text_frame_vertex_count = 0;
	level->render_data.text_frame_uniform_count = 0;
	auto append_text_render_data = [level, vulkan, font, camera](const char *text, aa_bound bound) {
		vec4 text_quad = {};
		float xpos = 0;
		float ypos = 0;
		uint32 text_len = (uint32)strlen(text);
		for (uint32 i = 0; i < text_len; i += 1) {
			stbtt_aligned_quad quad = {};
			stbtt_GetPackedQuad(font->stbtt_packed_chars, font->packed_bitmap_width, font->packed_bitmap_height, text[i] - ' ', &xpos, &ypos, &quad, 0);
			quad.y0 = -quad.y0;
			quad.y1 = -quad.y1;
			text_quad.x0 = min(text_quad.x0, quad.x0);
			text_quad.y0 = max(text_quad.y0, quad.y0);
			text_quad.x1 = max(text_quad.x1, quad.x1);
			text_quad.y1 = min(text_quad.y1, quad.y1);
			vertex *vertices = (vertex *)(vulkan->buffers.frame_vertex_buffer_ptrs[vulkan->frame_index] + vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] + sizeof(struct vertex) * 6 * i);
			vertices[0] = {{quad.x0, quad.y0, quad.s0, quad.t0}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
			vertices[1] = {{quad.x0, quad.y1, quad.s0, quad.t1}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
			vertices[2] = {{quad.x1, quad.y1, quad.s1, quad.t1}, {0, 255, 0, 0}, level->render_data.text_frame_uniform_count};
			vertices[3] = vertices[0];
			vertices[4] = vertices[2];
			vertices[5] = {{quad.x1, quad.y0, quad.s1, quad.t0}, {0, 255, 0, 255}, level->render_data.text_frame_uniform_count};
		}
		vulkan->buffers.frame_vertex_buffer_offsets[vulkan->frame_index] += sizeof(struct vertex) * 6 * text_len;

		vec2 text_quad_center = {};
		text_quad_center.x = text_quad.x0 + (text_quad.x1 - text_quad.x0) / 2;
		text_quad_center.y = text_quad.y0 + (text_quad.y1 - text_quad.y0) / 2;

		float text_quad_scale = 1 / font->font_size;
		float text_quad_height = text_quad.y0 - text_quad.y1;
		vec3 text_quad_translate = {0, text_quad_height / 2 * text_quad_scale, 0};
		text_quad_translate = text_quad_translate + aa_bound_center(bound);
		text_quad_translate.y += (bound.max.y - bound.min.y) / 2;

		mat4 transform_mat = mat4_vulkan_clip() * camera_view_projection_mat4(camera) * mat4_from_translation(text_quad_translate) * camera_billboard_mat4(camera) * mat4_from_scaling(text_quad_scale) * mat4_from_translation({(-text_quad_center).x, (-text_quad_center).y, 0});
	};
}
