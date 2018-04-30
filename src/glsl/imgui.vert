#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec2 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec4 color_in;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec4 color_out;

out gl_PerVertex {
  vec4 gl_Position;
};

m_declare_imgui_push_constant

void main() {
  uv_out = uv_in;
  color_out = color_in;

	float px = position_in.x / pc.framebuffer_width;
	float py = position_in.y / pc.framebuffer_height;
	px = pc.swap_chain_framebuffer_region.x + pc.swap_chain_framebuffer_region.z * px;
	py = pc.swap_chain_framebuffer_region.y + pc.swap_chain_framebuffer_region.w * py;
	gl_Position = vec4(px * 2 - 1, py * 2 - 1, 0, 1);
} 