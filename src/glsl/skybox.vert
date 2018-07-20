#version 450

#include "common.h"

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) out vec3 uv_out;

layout(push_constant) uniform push_constant {
  mat4 view_proj_mat; 
  uint cube_map_index;
} pc;

const vec3 face_vertices[6][6] = vec3[][](
  vec3[](vec3(-1, 1, 1), vec3(-1, -1, 1), vec3(1, 1, 1), vec3(1, 1, 1), vec3(-1, -1, 1), vec3(1, -1, 1)),
  vec3[](vec3(-1, 1, -1), vec3(-1, -1, -1), vec3(1, 1, -1), vec3(1, 1, -1), vec3(-1, -1, -1), vec3(1, -1, -1)),
  vec3[](vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(-1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, -1), vec3(-1, 1, 1)),
  vec3[](vec3(1, -1, -1), vec3(1, 1, -1), vec3(1, -1, 1), vec3(1, -1, 1), vec3(1, 1, -1), vec3(1, 1, 1)),
  vec3[](vec3(-1, 1, 1), vec3(-1, 1, -1), vec3(1, 1, 1), vec3(1, 1, 1), vec3(-1, 1, -1), vec3(1, 1, -1)),
  vec3[](vec3(-1, -1, 1), vec3(-1, -1, -1), vec3(1, -1, 1), vec3(1, -1, 1), vec3(-1, -1, -1), vec3(1, -1, -1))
);

void main() {
  vec3 vertex = face_vertices[gl_VertexIndex / 6][gl_VertexIndex % 6];
  uv_out = vertex;
  gl_Position = (pc.view_proj_mat * vec4(vertex, 1)).xyww;
}