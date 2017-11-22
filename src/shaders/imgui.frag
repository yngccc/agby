#version 450

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec4 color_in;

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform sampler2D image;

void main() {
  color_out = color_in * texture(image, uv_in);
}


