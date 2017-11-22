#version 450

layout(location = 0) in vec3 uv_in;

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform samplerCube cubemap;

void main() {
  color_out = texture(cubemap, uv_in);
}
