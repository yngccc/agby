#version 450

layout(location = 0) in vec3 position_in;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform common_uniform {
  mat4 camera_view_proj_mat;
};

layout(push_constant) uniform push_constants {
  mat4 transform_mat;
  vec4 color;
} push_consts;

void main() {
  gl_Position = camera_view_proj_mat * push_consts.transform_mat * vec4(position_in, 1);
}