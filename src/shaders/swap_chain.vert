#version 450

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) out vec2 uv_out;

const vec2 vertices[3] = vec2[](vec2(-1, -1), vec2(-1, 3), vec2(3, -1));

void main() {
  vec2 vertex = vertices[gl_VertexIndex];
  uv_out = vertex * 0.5 + 0.5;
  gl_Position = vec4(vertex, 0, 1);
}