#version 450

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec3 color_in;

layout(location = 0) out vec4 color_out;

layout(set = 1, binding = 0) uniform sampler2D font_atlas;

void main() {
	float alpha = texture(font_atlas, uv_in).r;
	color_out = vec4(color_in, alpha);
}