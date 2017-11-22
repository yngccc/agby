#version 450

layout(location = 0) in vec2 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec4 color_in;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec4 color_out;

out gl_PerVertex {
  vec4 gl_Position;
};

layout(push_constant) uniform push_constant {
  vec2 viewport;
} push_const;

void main() {
  uv_out = uv_in;
  color_out = color_in;
  gl_Position = vec4(-1 + position_in.x / push_const.viewport.x * 2, -1 + position_in.y / push_const.viewport.y * 2, 0, 1);
} 