#version 450

layout(location = 0) in vec2 uv_in;

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform sampler2D image;

layout(push_constant) uniform push_constant {
	float x_dir;
	float y_dir;
} push_const;

void main() {
	vec2 ds_dt = vec2(1, 1) / textureSize(image, 0);
	float dx = ds_dt.x * push_const.x_dir;
	float dy = ds_dt.y * push_const.y_dir;
	vec4 color = vec4(0, 0, 0, 0);
	// color += texture(image, vec2(uv_in.x - 4 * dx, uv_in.y - 4 * dy)) * 0.0162162162;
	// color += texture(image, vec2(uv_in.x - 3 * dx, uv_in.y - 3 * dy)) * 0.0540540541;
	// color += texture(image, vec2(uv_in.x - 2 * dx, uv_in.y - 2 * dy)) * 0.1216216216;
	// color += texture(image, vec2(uv_in.x - 1 * dx, uv_in.y - 1 * dy)) * 0.1945945946;
	// color += texture(image, vec2(uv_in.x, uv_in.y)) * 0.2270270270;
	// color += texture(image, vec2(uv_in.x + 1 * dx, uv_in.y + 1 * dy)) * 0.1945945946;
	// color += texture(image, vec2(uv_in.x + 2 * dx, uv_in.y + 2 * dy)) * 0.1216216216;
	// color += texture(image, vec2(uv_in.x + 3 * dx, uv_in.y + 3 * dy)) * 0.0540540541;
	// color += texture(image, vec2(uv_in.x + 4 * dx, uv_in.y + 4 * dy)) * 0.0162162162;
  color += texture(image, vec2(uv_in.x - 3 * dx, uv_in.y - 3 * dy)) * 0.00598;
  color += texture(image, vec2(uv_in.x - 2 * dx, uv_in.y - 2 * dy)) * 0.060626;
  color += texture(image, vec2(uv_in.x - 1 * dx, uv_in.y - 1 * dy)) * 0.241843;
  color += texture(image, vec2(uv_in.x, uv_in.y)) * 0.383103;
  color += texture(image, vec2(uv_in.x + 1 * dx, uv_in.y + 1 * dy)) * 0.241843;
  color += texture(image, vec2(uv_in.x + 2 * dx, uv_in.y + 2 * dy)) * 0.060626;
  color += texture(image, vec2(uv_in.x + 3 * dx, uv_in.y + 3 * dy)) * 0.00598;
  color_out = color;
}

