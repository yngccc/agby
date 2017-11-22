#version 450

layout(location = 0) in vec4 position_in;

layout(location = 0) out vec4 color_out;

void main() {
	float depth = position_in.z / position_in.w;
	// depth = depth * 0.5 + 0.5;
	float moment1 = depth;
	float moment2 = depth * depth;
	// Adjusting moments (this is sort of bias per pixel) using partial derivative
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25 * (dx * dx + dy * dy);
	color_out = vec4(moment1, moment2, 0, 0);
}
