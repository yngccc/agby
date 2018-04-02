#version 450

#include "../shader_type.cpp"

layout(location = 0) out vec4 color_out;

m_declare_color_vertex_push_constant

void main() {
  color_out = pc.color;
}
