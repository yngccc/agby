#version 450

#include "../shader_type.cpp"

out gl_PerVertex {
  vec4 gl_Position;
};

layout(location = 0) out vec2 uv_out;

m_declare_swap_chain_push_constant

void main() {
	vec4 vertices[6] = vec4[](vec4(pc.x, pc.y, 0, 0),
														vec4(pc.x, pc.y + pc.height, 0, 1),
														vec4(pc.x + pc.width, pc.y, 1, 0),
														vec4(pc.x + pc.width, pc.y, 1, 0),
														vec4(pc.x, pc.y + pc.height, 0, 1),
														vec4(pc.x + pc.width, pc.y + pc.height, 1, 1));
	vec4 vertex = vertices[gl_VertexIndex];
  gl_Position = vec4(vertex.xy, 0, 1);
  uv_out = vertex.zw;
}