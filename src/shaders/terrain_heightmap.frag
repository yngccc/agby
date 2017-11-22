#version 450

layout(location = 0) in vec3 position_in;

layout(location = 0) out vec4 color_out;

void main() {
	color_out.r = position_in.y;
}
