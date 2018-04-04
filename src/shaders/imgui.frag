#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec4 color_in;

layout(location = 0) out vec4 color_out;

m_declare_imgui_push_constant
m_declare_2d_texture_array

void main() {
	color_out = color_in * texture(textures[pc.texture_index], uv_in);
}


