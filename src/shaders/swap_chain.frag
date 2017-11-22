#version 450

layout(location = 0) in vec2 uv_in;

layout(location = 0) out vec4 color_out;

layout(set = 0, binding = 0) uniform sampler2D image;

void main() {
  vec3 color = texture(image, uv_in).rgb;
//	float luminance = sqrt(0.299 * color.r * color.r + 0.587 * color.g * color.g + 0.114 * color.b * color.b);
//  color /= (1 + luminance / 2.0);
  color_out = vec4(color, 1);
}