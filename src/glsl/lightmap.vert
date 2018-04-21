#version 450

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;

out gl_PerVertex {
  vec4 gl_Position;
};

void main() {
	gl_Position = vec4((uv_in.x - 0.5) * 2, (uv_in.y - 0.5) * 2, 0.5, 1);
}