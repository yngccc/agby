#version 450

#include "common.h"

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) out vec2 uv_out;

const vec4 vertices[6] =
vec4[](
	vec4(-1, -1, 0, 0),
	vec4(-1, 1, 0, 1),
	vec4(1, -1, 1, 0),
	vec4(1, -1, 1, 0),
	vec4(-1, 1, 0, 1),
	vec4(1, 1, 1, 1)
);

void main() {
	vec4 vertex = vertices[gl_VertexIndex];
  gl_Position = vec4(vertex.xy, 0, 1);
  uv_out = vertex.zw;
}