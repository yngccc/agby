#version 450

layout(location = 0) out vec4 color_out;

void main() {
  float depth = gl_FragCoord.z;
	float moment1 = depth;
	float moment2 = depth * depth;
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25 * (dx * dx + dy * dy);
	color_out = vec4(moment1, moment2, 0, 0);
}
