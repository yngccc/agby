#version 450

#include "../shader_type.cpp"

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) out vec3 uv_out;

m_declare_uniform_buffer

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
  gl_Position = (m_level_view_proj_mat * vec4(vertex, 1)).xyww;
}