#version 450

layout(location = 0) in vec2 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 color_in;
layout(location = 3) in uint transform_mat_index;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec3 color_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform uniform_buffer {
  mat4 transform_mats[256];
};

void main() {
  uv_out = uv_in;
  color_out = color_in;
  gl_Position = transform_mats[transform_mat_index] * vec4(position_in, 0, 1);
}