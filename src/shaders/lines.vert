#version 450

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 color_in;

layout(location = 0) out vec3 color_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform common_uniform {
  mat4 camera_view_proj_mat;
};

void main() {
	color_out = color_in;
  gl_Position = camera_view_proj_mat * vec4(position_in, 1);
}