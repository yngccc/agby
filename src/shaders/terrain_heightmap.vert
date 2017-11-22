#version 450

layout(location = 0) in vec3 position_in;

layout(location = 0) out vec3 position_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(push_constant) uniform push_constant {
  mat4 model_mat;
  mat4 proj_mat;
} push_const;

void main() {
	vec4 position = push_const.model_mat * vec4(position_in, 1);
	position_out = position.xyz;
  gl_Position = push_const.proj_mat * position;
}
