#version 450

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 normal_in;

layout(location = 0) out vec3 position_out;
layout(location = 1) out vec2 uv_out;
layout(location = 2) out vec3 normal_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform uniform_buffer {
  mat4 camera_view_proj_mat;
};

layout(set = 0, binding = 1) uniform uniform_buffer_2 {
  mat4 model_mat;
  mat4 normal_mat;
};

void main() {
	vec4 position = model_mat * vec4(position_in, 1);
  position_out = position.xyz;
  gl_Position = camera_view_proj_mat * position;
  uv_out = uv_in;
  normal_out = normalize((normal_mat * vec4(normal_in, 1)).xyz);
}

