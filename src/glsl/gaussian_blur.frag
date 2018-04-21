#version 450

#include "../shader_type.cpp"

layout(location = 0) in vec2 uv_in;

layout(location = 0) out vec4 color_out;

m_declare_2d_texture_array
m_declare_gaussian_blur_push_constant

void main() {
	vec2 ds_dt = vec2(1, 1) / textureSize(textures[pc.index], 0);
	float dx = ds_dt.x * pc.x_dir;
	float dy = ds_dt.y * pc.y_dir;
	vec4 color = vec4(0, 0, 0, 0);
  color += texture(textures[pc.index], vec2(uv_in.x - 3 * dx, uv_in.y - 3 * dy)) * 0.00598;
  color += texture(textures[pc.index], vec2(uv_in.x - 2 * dx, uv_in.y - 2 * dy)) * 0.060626;
  color += texture(textures[pc.index], vec2(uv_in.x - 1 * dx, uv_in.y - 1 * dy)) * 0.241843;
  color += texture(textures[pc.index], vec2(uv_in.x, uv_in.y)) * 0.383103;
  color += texture(textures[pc.index], vec2(uv_in.x + 1 * dx, uv_in.y + 1 * dy)) * 0.241843;
  color += texture(textures[pc.index], vec2(uv_in.x + 2 * dx, uv_in.y + 2 * dy)) * 0.060626;
  color += texture(textures[pc.index], vec2(uv_in.x + 3 * dx, uv_in.y + 3 * dy)) * 0.00598;
  color_out = color;
}

