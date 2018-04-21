#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec2 uv_in;

layout(location = 0) out vec4 color_out;

m_declare_2d_texture_array
m_declare_swap_chain_push_constant

void main() {
  vec3 color = texture(textures[pc.texture_index], uv_in).rgb;
//	float luminance = sqrt(0.299 * color.r * color.r + 0.587 * color.g * color.g + 0.114 * color.b * color.b);
//  color /= (1 + luminance / 2.0);
  color_out = vec4(color, 1);
}