#version 450

layout(location = 0) in vec2 uv_in;
layout(location = 1) in vec4 color_in;

layout(location = 0) out vec4 color_out;

layout(push_constant) uniform push_constant {
  vec2 viewport;
  uint image_is_grayscale;
} push_const;

layout(set = 0, binding = 0) uniform sampler2D image;

void main() {
  if (push_const.image_is_grayscale > 0) {
    color_out = vec4(texture(image, uv_in).xxx, 1);
  } else {
    color_out = color_in * texture(image, uv_in);
  }
}


