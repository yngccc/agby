/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#pragma once

#include "common.cpp"

union vec2 {
	struct { float x, y; };
	struct { float r, g; };
	struct { float s, t; };
	struct { float width, height; };
	struct { float min, max; };
	float e[2];

	bool operator==(const vec2 &v) const { return (x == v.x) && (y == v.y); }
	bool operator!=(const vec2 &v) const { return !(*this == v); }
	vec2 operator+(const vec2 &v) const { return vec2{x + v.x, y + v.y}; }
	vec2 operator-() const { return vec2{-x, -y}; }
	vec2 operator-(const vec2 &v) const { return vec2{x - v.x, y - v.y}; }
	vec2 operator-(float d) const { return vec2{x - d, y - d}; }
	vec2 operator*(float d) const { return vec2{x * d, y * d}; }
	vec2 operator*(const vec2 &v) const { return vec2{x * v.x, y * v.y}; }
	vec2 operator/(float d) const { return vec2{x / d, y / d}; }
	vec2 operator/(const vec2 &v) const { return vec2{x / v.x, y / v.y}; }
	void operator+=(const vec2 &v) { x += v.x; y += v.y; }
	void operator-=(const vec2 &v) { x -= v.x; y -= v.y; }
	void operator*=(const vec2 &v) { x *= v.x; y *= v.y; }
	void operator*=(float f) { x *= f; y *= f; }
	void operator/=(const vec2 &v) { x /= v.x; y /= v.y; }
	void operator/=(float f) { x /= f; y /= f; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
};

union vec3 {
	struct { float x, y, z; };
	struct { float r, g, b; };
	struct { float s, t, p; };
	struct { float u, v, w; };
	struct { float pitch, yaw, roll; };
	float e[3];

	bool operator==(const vec3 &v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const vec3 &v) const { return !(*this == v); }
	vec3 operator+(const vec3 &v) const { return vec3{x + v.x, y + v.y, z + v.z}; }
	vec3 operator-() const { return vec3{-x, -y, -z}; }
	vec3 operator-(const vec3 &v) const { return vec3{x - v.x, y - v.y, z - v.z}; }
	vec3 operator-(float d) const { return vec3{x - d, y - d, z - d}; }
	vec3 operator*(float d) const { return vec3{x * d, y * d, z * d}; }
	vec3 operator*(const vec3 &v) const { return vec3{x * v.x, y * v.y, z * v.z}; }
	vec3 operator/(float d) const { return vec3{x / d, y / d, z / d}; }
	vec3 operator/(const vec3 &v) const { return vec3{x / v.x, y / v.y, z / v.z}; }
	void operator+=(const vec3 &v) { x += v.x; y += v.y; z += v.z; }
	void operator-=(const vec3 &v) { x -= v.x; y -= v.y; z -= v.z; }
	void operator*=(const vec3 &v) { x *= v.x; y *= v.y; z *= v.z; }
	void operator*=(float f) { x *= f; y *= f; z *= f; }
	void operator/=(const vec3 &v) { x /= v.x; y /= v.y; z /= v.z; }
	void operator/=(float f) { x /= f; y /= f; z /= f; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
	vec2 xy() { return vec2{x, y}; };
};

union vec4 {
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
	struct { float s, t, p, q; };
	struct { float x0, y0, x1, y1; };
	float e[4];

	bool operator==(const vec4 &v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const vec4 &v) const { return !(*this == v); }
	vec4 operator+(const vec4 &v) const { return vec4{x + v.x, y + v.y, z + v.z, w + v.w}; }
	vec4 operator-() const { return vec4{-x, -y, -z, -w}; }
	vec4 operator-(const vec4 &v) const { return vec4{x - v.x, y - v.y, z - v.z, w - v.w}; }
	vec4 operator-(float d) const { return vec4{x - d, y - d, z - d, w - d}; }
	vec4 operator*(float d) const { return vec4{x * d, y * d, z * d, w * d}; }
	vec4 operator*(const vec4 &v) const { return vec4{x * v.x, y * v.y, z * v.z, w * v.w}; }
	vec4 operator/(float d) const { return vec4{x / d, y / d, z / d, w / d}; }
	vec4 operator/(const vec4 &v) const { return vec4{x / v.x, y / v.y, z / v.z, w / v.w}; }
	void operator+=(const vec4 &v) { x += v.x; y += v.y; z += v.z; w += v.w; }
	void operator-=(const vec4 &v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	void operator*=(const vec4 &v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
	void operator*=(float f) { x *= f; y *= f; z *= f; w *= f; }
	void operator/=(const vec4 &v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; }
	void operator/=(float f) { x /= f; y /= f; z /= f; w /= f; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
	vec3 xyz() const { return vec3{x, y, z}; }
};

union i8vec3 {
	struct { int8 x, y, z; };
	struct { int8 r, g, b; };
	int8 e[3];

	bool operator==(const i8vec3 &v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const i8vec3 &v) const { return !(*this == v); }
	int8& operator[](uint32 i) { return e[i]; }
	const int8& operator[](uint32 i) const { return e[i]; }
};

union u8vec3 {
	struct { uint8 x, y, z; };
	struct { uint8 r, g, b; };
	uint8 e[3];

	bool operator==(const u8vec3 &v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const u8vec3 &v) const { return !(*this == v); }
	uint8& operator[](uint32 i) { return e[i]; }
	const uint8& operator[](uint32 i) const { return e[i]; }
};

union i8vec4 {
	struct { int8 x, y, z, w; };
	struct { int8 r, g, b, a; };
	int8 e[4];

	bool operator==(const i8vec4 &v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const i8vec4 &v) const { return !(*this == v); }
	int8& operator[](uint32 i) { return e[i]; }
	const int8& operator[](uint32 i) const { return e[i]; }
};

union u8vec4 {
	struct { uint8 x, y, z, w; };
	struct { uint8 r, g, b, a; };
	uint8 e[4];

	bool operator==(const u8vec4 &v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const u8vec4 &v) const { return !(*this == v); }
	uint8& operator[](uint32 i) { return e[i]; }
	const uint8& operator[](uint32 i) const { return e[i]; }
};

union i16vec3 {
	struct { int16 x, y, z; };
	int16 e[3];

	bool operator==(const i16vec3 &v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const i16vec3 &v) const { return !(*this == v); }
	int16& operator[](uint32 i) { return e[i]; }
	const int16& operator[](uint32 i) const { return e[i]; }
};

union i16vec4 {
	struct { int16 x, y, z, w; };
	struct { int16 r, g, b, a; };
	int16 e[4];

	bool operator==(const i16vec4 &v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const i16vec4 &v) const { return !(*this == v); }
	int16& operator[](uint32 i) { return e[i]; }
	const int16& operator[](uint32 i) const { return e[i]; }
};

union u16vec4 {
	struct { uint16 x, y, z, w; };
	struct { uint16 r, g, b, a; };
	uint16 e[4];

	bool operator==(const u16vec4 &v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const u16vec4 &v) const { return !(*this == v); }
	uint16& operator[](uint32 i) { return e[i]; }
	const uint16& operator[](uint32 i) const { return e[i]; }
};

union mat3 {
	struct { vec3 c1, c2, c3; };
	vec3 columns[3];
	vec3 e[3];

	bool operator==(const mat3 &m) const { return (c1 == m.c1) && (c2 == m.c2) && (c3 == m.c3); }
	bool operator!=(const mat3 &m) const { return !(*this == m); }
	vec3 operator*(const vec3 &v) const {
		float v0 = c1.x * v.x;
		float v1 = c1.y * v.x;
		float v2 = c1.z * v.x;
		v0 += c2.x * v.y;
		v1 += c2.y * v.y;
		v2 += c2.z * v.y;
		v0 += c3.x * v.z;
		v1 += c3.y * v.z;
		v2 += c3.z * v.z;
		return vec3{v0, v1, v2};
	}
	vec3& operator[](uint32 i) { return e[i]; }
	const vec3& operator[](uint32 i) const { return e[i]; }
	explicit operator float*() { return &c1.x; }
	explicit operator const float*() const { return &c1.x; }
};

union mat4 {
	struct { vec4 c1, c2, c3, c4; };
	vec4 columns[4];
	vec4 e[4];

	bool operator==(const mat4 &m) const { return (c1 == m.c1) && (c2 == m.c2) && (c3 == m.c3) && (c4 == m.c4); }
	bool operator!=(const mat4 &m) const { return !(*this == m); }
	mat4 operator+(const mat4 &m) const { return mat4{c1 + m.c1, c2 + m.c2, c3 + m.c3, c4 + m.c4}; }
	mat4 operator*(float d) const { mat4 result = *this; result.c1 = result.c1 * d; result.c2 = result.c2 * d; result.c3 = result.c3 * d; result.c4 = result.c4 * d; return result; }
	vec4 operator*(const vec4 &v) const {
		float v0 = c1.x * v.x;
		float v1 = c1.y * v.x;
		float v2 = c1.z * v.x;
		float v3 = c1.w * v.x;
		v0 += c2.x * v.y;
		v1 += c2.y * v.y;
		v2 += c2.z * v.y;
		v3 += c2.w * v.y;
		v0 += c3.x * v.z;
		v1 += c3.y * v.z;
		v2 += c3.z * v.z;
		v3 += c3.w * v.z;
		v0 += c4.x * v.w;
		v1 += c4.y * v.w;
		v2 += c4.z * v.w;
		v3 += c4.w * v.w;
		return vec4{v0, v1, v2, v3};
	}
	vec3 operator*(const vec3 &v) const { vec4 v4 = *this * vec4{v.x, v.y, v.z, 1}; return vec3{v4.x, v4.y, v4.z}; }
	mat4 operator*(const mat4 &m) const {
		mat4 result;
		result.c1 = c1 * m.c1.x + c2 * m.c1.y + c3 * m.c1.z + c4 * m.c1.w;
		result.c2 = c1 * m.c2.x + c2 * m.c2.y + c3 * m.c2.z + c4 * m.c2.w;
		result.c3 = c1 * m.c3.x + c2 * m.c3.y + c3 * m.c3.z + c4 * m.c3.w;
		result.c4 = c1 * m.c4.x + c2 * m.c4.y + c3 * m.c4.z + c4 * m.c4.w;
		return result;
	}
	vec4& operator[](uint32 i) { return e[i]; }
	const vec4& operator[](uint32 i) const { return e[i]; }
	explicit operator float*() { return &c1.x; }
	explicit operator const float*() const { return &c1.x; }
};

union quat {
	struct { float x, y, z, w; };
	struct { float q1, q2, q3, q0; };
	float e[4];

	bool operator==(const quat &q) const { return x == q.x && y == q.y && z == q.z && w == q.w; }
	bool operator!=(const quat &q) const { return !(*this == q); }
	quat operator+(const quat &q) const { return quat{x + q.x, y + q.y, z + q.z, w + q.w}; }
	quat operator-() const { return quat{-x, -y, -z, -w}; }
	quat operator*(float d) const { return quat{x * d, y * d, z * d, w * d}; }
	vec3 operator*(const vec3 &v) const { mat3 quat_to_mat3(quat); return quat_to_mat3(*this) * v; }
	quat operator*(const quat &q) const {
		quat result = {
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y + y * q.w + z * q.x - x * q.z,
			w * q.z + z * q.w + x * q.y - y * q.x,
			w * q.w - x * q.x - y * q.y - z * q.z
		};
		return result;
	}
	quat operator/(float d) const { return quat{x / d, y / d, z / d, w / d}; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
};

struct transform {
	vec3 scale;
	quat rotate;
	vec3 translate;

	bool operator==(const transform &t) const { return scale == t.scale && rotate == t.rotate && translate == t.translate; }
	bool operator!=(const transform &t) const { return !(*this == t); }
};

struct camera {
	vec3 position;
	vec3 view;
	vec3 up;
	float fovy;
	float aspect;
	float znear;
	float zfar;
};

struct plane {
	vec3 normal;
	float distance;
};

struct triangle {
	vec3 a;
	vec3 b;
	vec3 c;
};

struct sphere {
	vec3 center;
	float radius;
};

struct cylinder {
	vec3 begin;
	vec3 end;
	float radius;
};

struct capsule {
	vec3 begin;
	vec3 end;
	float radius;
};

struct aa_bound {
	vec3 min;
	vec3 max;
};

struct ray {
	vec3 origin;
	vec3 direction;
	float len;
};

float vec2_len(vec2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

vec2 vec2_normalize(vec2 v) {
	float len = vec2_len(v);
	m_debug_assert(len > 0);
	return v / len;
}

float vec2_dot(vec2 v1, vec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float vec3_len(vec3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 vec3_normalize(vec3 v) {
	float len = vec3_len(v);
	m_debug_assert(len > 0);
	return v / len;
}

float vec3_dot(vec3 v1, vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 vec3_cross(vec3 v1, vec3 v2) {
	return vec3{v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y};
}

vec3 vec3_lerp(vec3 v1, vec3 v2, float t) {
	m_assert(t >= 0 && t <= 1);  return v1 + (v2 - v1) * t;
}

float vec4_len(vec4 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec4 vec4_normalize(vec4 v) {
	float len = vec4_len(v);
	m_debug_assert(len > 0);
	return v / len;
}

void mat3_to_str(const mat3 &m, char *buf, int buf_size) {
	snprintf(buf, buf_size, "| %.6f %.6f %.6f |\n| %.6f %.6f %.6f |\n| %.6f %.6f %.6f |\n", m.c1.x, m.c2.x, m.c3.x, m.c1.y, m.c2.y, m.c3.y, m.c1.z, m.c2.z, m.c3.z);
}

mat3 mat3_transpose(const mat3 &m) {
	mat3 mat;
	for (int i = 0; i < 3; i += 1) {
		for (int j = 0; j < 3; j += 1) {
			mat.columns[i].e[j] = m.columns[j].e[i];
		}
	}
	return mat;
}

mat3 mat3_inverse(const mat3 &m) {
	float one_over_determinant = 1 / (+m.e[0].e[0] * (m.e[1].e[1] * m.e[2].e[2] - m.e[2].e[1] * m.e[1].e[2])
		- m.e[1].e[0] * (m.e[0].e[1] * m.e[2].e[2] - m.e[2].e[1] * m.e[0].e[2])
		+ m.e[2].e[0] * (m.e[0].e[1] * m.e[1].e[2] - m.e[1].e[1] * m.e[0].e[2]));
	mat3 inverse;
	inverse.e[0].e[0] = +(m.e[1].e[1] * m.e[2].e[2] - m.e[2].e[1] * m.e[1].e[2]) * one_over_determinant;
	inverse.e[1].e[0] = -(m.e[1].e[0] * m.e[2].e[2] - m.e[2].e[0] * m.e[1].e[2]) * one_over_determinant;
	inverse.e[2].e[0] = +(m.e[1].e[0] * m.e[2].e[1] - m.e[2].e[0] * m.e[1].e[1]) * one_over_determinant;
	inverse.e[0].e[1] = -(m.e[0].e[1] * m.e[2].e[2] - m.e[2].e[1] * m.e[0].e[2]) * one_over_determinant;
	inverse.e[1].e[1] = +(m.e[0].e[0] * m.e[2].e[2] - m.e[2].e[0] * m.e[0].e[2]) * one_over_determinant;
	inverse.e[2].e[1] = -(m.e[0].e[0] * m.e[2].e[1] - m.e[2].e[0] * m.e[0].e[1]) * one_over_determinant;
	inverse.e[0].e[2] = +(m.e[0].e[1] * m.e[1].e[2] - m.e[1].e[1] * m.e[0].e[2]) * one_over_determinant;
	inverse.e[1].e[2] = -(m.e[0].e[0] * m.e[1].e[2] - m.e[1].e[0] * m.e[0].e[2]) * one_over_determinant;
	inverse.e[2].e[2] = +(m.e[0].e[0] * m.e[1].e[1] - m.e[1].e[0] * m.e[0].e[1]) * one_over_determinant;
	return inverse;
}

vec3 mat3_get_scaling(const mat3 &m) {
	vec3 scaling = {vec3_len(m.columns[0]), vec3_len(m.columns[1]), vec3_len(m.columns[2])};
	return scaling;
}

quat mat3_get_rotation(const mat3 &m) {
	float four_x_squared_minus_1 = m[0][0] - m[1][1] - m[2][2];
	float four_y_squared_minus_1 = m[1][1] - m[0][0] - m[2][2];
	float four_z_squared_minus_1 = m[2][2] - m[0][0] - m[1][1];
	float four_w_squared_minus_1 = m[0][0] + m[1][1] + m[2][2];
	int32 biggest_index = 0;
	float four_biggest_squared_minus_1 = four_w_squared_minus_1;
	if (four_x_squared_minus_1 > four_biggest_squared_minus_1) {
		four_biggest_squared_minus_1 = four_x_squared_minus_1;
		biggest_index = 1;
	}
	if (four_y_squared_minus_1 > four_biggest_squared_minus_1) {
		four_biggest_squared_minus_1 = four_y_squared_minus_1;
		biggest_index = 2;
	}
	if (four_z_squared_minus_1 > four_biggest_squared_minus_1) {
		four_biggest_squared_minus_1 = four_z_squared_minus_1;
		biggest_index = 3;
	}
	float biggest_val = sqrtf(four_biggest_squared_minus_1 + 1) * 0.5f;
	float mult = 0.25f / biggest_val;
	switch (biggest_index) {
	case 0: return quat{(m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult, biggest_val};
	case 1: return quat{biggest_val, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] - m[2][1]) * mult};
	case 2: return quat{(m[0][1] + m[1][0]) * mult, biggest_val, (m[1][2] + m[2][1]) * mult, (m[2][0] - m[0][2]) * mult};
	case 3: return quat{(m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggest_val, (m[0][1] - m[1][0]) * mult};
	default: { m_assert(false); return quat{0, 0, 0, 1}; }
	}
}

mat3 mat3_from_scaling(float scale) {
	mat3 mat = {};
	mat.c1.x = scale; mat.c2.y = scale; mat.c3.z = scale;
	return mat;
}

mat3 mat3_from_scaling(vec3 scale) {
	mat3 mat = {};
	mat.c1.x = scale.x; mat.c2.y = scale.y; mat.c3.z = scale.z;
	return mat;
}

mat3 mat3_from_rotation(vec3 axis, float angle) {
	m_assert(fabsf(vec3_len(axis) - 1) < (FLT_EPSILON * 2));
	const float c = cosf(angle);
	const float c_1 = 1 - c;
	const float s = sinf(angle);
	mat3 mat = {};
	mat.c1.x = c + axis.x * axis.x * c_1;
	mat.c1.y = axis.y * axis.x * c_1 + axis.z * s;
	mat.c1.z = axis.z * axis.x * c_1 - axis.y * s;
	mat.c2.x = axis.x * axis.y * c_1 - axis.z * s;
	mat.c2.y = c + axis.y * axis.y * c_1;
	mat.c2.z = axis.z * axis.y * c_1 + axis.x * s;
	mat.c3.x = axis.x * axis.z * c_1 + axis.y * s;
	mat.c3.y = axis.y * axis.z * c_1 - axis.x * s;
	mat.c3.z = c + axis.z * axis.z * c_1;
	return mat;
}

mat3 mat4_to_mat3(const mat4 &m) {
	mat3 mat = {vec3{m.c1.x, m.c1.y, m.c1.z}, vec3{m.c2.x, m.c2.y, m.c2.z}, vec3{m.c3.x, m.c3.y, m.c3.z}};
	return mat;
}

void mat4_to_str(const mat4 &m, char *buf, int buf_size) {
	snprintf(buf, buf_size, "| %.6f %.6f %.6f %.6f |\n| %.6f %.6f %.6f %.6f |\n| %.6f %.6f %.6f %.6f |\n| %.6f %.6f %.6f %.6f |\n",
		m.c1.x, m.c2.x, m.c3.x, m.c4.x, m.c1.y, m.c2.y, m.c3.y, m.c4.y, m.c1.z, m.c2.z, m.c3.z, m.c4.z, m.c1.w, m.c2.w, m.c3.w, m.c4.w);
}

mat4 mat4_transpose(const mat4 &m) {
	mat4 mat;
	for (int i = 0; i < 4; i += 1) {
		for (int j = 0; j < 4; j += 1) {
			mat.columns[i].e[j] = m.columns[j].e[i];
		}
	}
	return mat;
}

mat4 mat4_inverse(const mat4 &m) {
	float coef00 = m.c3.z * m.c4.w - m.c4.z * m.c3.w;
	float coef02 = m.c2.z * m.c4.w - m.c4.z * m.c2.w;
	float coef03 = m.c2.z * m.c3.w - m.c3.z * m.c2.w;
	float coef04 = m.c3.y * m.c4.w - m.c4.y * m.c3.w;
	float coef06 = m.c2.y * m.c4.w - m.c4.y * m.c2.w;
	float coef07 = m.c2.y * m.c3.w - m.c3.y * m.c2.w;
	float coef08 = m.c3.y * m.c4.z - m.c4.y * m.c3.z;
	float coef10 = m.c2.y * m.c4.z - m.c4.y * m.c2.z;
	float coef11 = m.c2.y * m.c3.z - m.c3.y * m.c2.z;
	float coef12 = m.c3.x * m.c4.w - m.c4.x * m.c3.w;
	float coef14 = m.c2.x * m.c4.w - m.c4.x * m.c2.w;
	float coef15 = m.c2.x * m.c3.w - m.c3.x * m.c2.w;
	float coef16 = m.c3.x * m.c4.z - m.c4.x * m.c3.z;
	float coef18 = m.c2.x * m.c4.z - m.c4.x * m.c2.z;
	float Coef19 = m.c2.x * m.c3.z - m.c3.x * m.c2.z;
	float coef20 = m.c3.x * m.c4.y - m.c4.x * m.c3.y;
	float coef22 = m.c2.x * m.c4.y - m.c4.x * m.c2.y;
	float coef23 = m.c2.x * m.c3.y - m.c3.x * m.c2.y;
	vec4 fac0 = {coef00, coef00, coef02, coef03};
	vec4 fac1 = {coef04, coef04, coef06, coef07};
	vec4 fac2 = {coef08, coef08, coef10, coef11};
	vec4 fac3 = {coef12, coef12, coef14, coef15};
	vec4 fac4 = {coef16, coef16, coef18, Coef19};
	vec4 fac5 = {coef20, coef20, coef22, coef23};
	vec4 vec0 = {m.c2.x, m.c1.x, m.c1.x, m.c1.x};
	vec4 vec1 = {m.c2.y, m.c1.y, m.c1.y, m.c1.y};
	vec4 vec2 = {m.c2.z, m.c1.z, m.c1.z, m.c1.z};
	vec4 vec3 = {m.c2.w, m.c1.w, m.c1.w, m.c1.w};
	vec4 inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
	vec4 inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
	vec4 inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
	vec4 inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;
	vec4 sign_a = {+1, -1, +1, -1};
	vec4 sign_b = {-1, +1, -1, +1};
	mat4 inverse = {inv0 * sign_a, inv1 * sign_b, inv2 * sign_a, inv3 * sign_b};
	vec4 row0 = {inverse.c1.x, inverse.c2.x, inverse.c3.x, inverse.c4.x};
	vec4 dot0 = m.c1 * row0;
	float dot1 = dot0.x + dot0.y + dot0.z + dot0.w;
	float one_over_determinant = 1 / dot1;
	return inverse * one_over_determinant;
}

mat4 mat4_identity() {
	return mat4{vec4{1, 0, 0, 0}, vec4{0, 1, 0, 0}, vec4{0, 0, 1, 0}, vec4{0, 0, 0, 1}};
}

mat4 mat4_from_scaling(float scale) {
	mat4 mat = {};
	mat.c1.x = scale; 
	mat.c2.y = scale; 
	mat.c3.z = scale; 
	mat.c4.w = 1;
	return mat;
}

mat4 mat4_from_scaling(vec3 scale) {
	mat4 mat = {};
	mat.c1.x = scale.x; 
	mat.c2.y = scale.y; 
	mat.c3.z = scale.z; 
	mat.c4.w = 1;
	return mat;
}

mat4 mat4_from_translation(vec3 translate) {
	mat4 mat = mat4_identity();
	mat.c4.x = translate.x; 
	mat.c4.y = translate.y; 
	mat.c4.z = translate.z; 
	return mat;
}

mat4 mat4_from_rotation(vec3 axis, float angle) {
	m_assert(fabsf(vec3_len(axis) - 1) < (FLT_EPSILON * 2));
	const float c = cosf(angle);
	const float c_1 = 1 - c;
	const float s = sinf(angle);
	mat4 mat = {};
	mat.c1.x = c + axis.x * axis.x * c_1;
	mat.c1.y = axis.y * axis.x * c_1 + axis.z * s;
	mat.c1.z = axis.z * axis.x * c_1 - axis.y * s;
	mat.c2.x = axis.x * axis.y * c_1 - axis.z * s;
	mat.c2.y = c + axis.y * axis.y * c_1;
	mat.c2.z = axis.z * axis.y * c_1 + axis.x * s;
	mat.c3.x = axis.x * axis.z * c_1 + axis.y * s;
	mat.c3.y = axis.y * axis.z * c_1 - axis.x * s;
	mat.c3.z = c + axis.z * axis.z * c_1;
	mat.c4.w = 1;
	return mat;
}

vec3 mat4_get_scaling(const mat4 &m) {
	vec3 scaling = {
		vec3_len(vec3{m.columns[0].x, m.columns[0].y, m.columns[0].z}),
		vec3_len(vec3{m.columns[1].x, m.columns[1].y, m.columns[1].z}),
		vec3_len(vec3{m.columns[2].x, m.columns[2].y, m.columns[2].z})
	};
	return scaling;
}

quat mat4_get_rotation(const mat4 &m) {
	vec3 scaling = mat4_get_scaling(m);
	mat3 m3 = mat4_to_mat3(m);
	m3[0] /= scaling.x;
	m3[1] /= scaling.y;
	m3[2] /= scaling.z;
	quat q = mat3_get_rotation(m3);
	float q_len = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	m_debug_assert(q_len > 0);
	return q / q_len;
}

vec3 mat4_get_translation(const mat4 &m) {
	vec3 translation = {m.columns[3].x, m.columns[3].y, m.columns[3].z};
	return translation;
}

mat4 mat4_orthographic_projection(float left, float right, float bottom, float top, float near, float far) {
	mat4 mat = {};
	mat.c1.x = 2 / (right - left);
	mat.c2.y = 2 / (top - bottom);
	mat.c3.z = -2 / (far - near);
	mat.c4.x = -(right + left) / (right - left);
	mat.c4.y = -(top + bottom) / (top - bottom);
	mat.c4.z = -(far + near) / (far - near);
	mat.c4.w = 1;
	return mat;
}

mat4 mat4_perspective_projection(float fovy, float aspect, float znear, float zfar) {
	float f = 1 / tanf(fovy / 2);
	mat4 mat = {};
	mat.c1.x = f / aspect;
	mat.c2.y = f;
	mat.c3.z = (zfar + znear) / (znear - zfar);
	mat.c3.w = -1;
	mat.c4.z = (2 * zfar * znear) / (znear - zfar);
	return mat;
}

mat4 mat4_look_at(vec3 eye, vec3 center, vec3 up) {
	vec3 f = vec3_normalize(center - eye);
	vec3 s = vec3_normalize(vec3_cross(f, up));
	vec3 u = vec3_cross(s, f);
	mat4 mat = {};
	mat.c1.x = s.x;
	mat.c1.y = u.x;
	mat.c1.z = -f.x;
	mat.c2.x = s.y;
	mat.c2.y = u.y;
	mat.c2.z = -f.y;
	mat.c3.x = s.z;
	mat.c3.y = u.z;
	mat.c3.z = -f.z;
	mat.c4.x = -vec3_dot(s, eye);
	mat.c4.y = -vec3_dot(u, eye);
	mat.c4.z = vec3_dot(f, eye);
	mat.c4.w = 1;
	return mat;
}

vec3 mat4_unproject(vec3 window_coord, const mat4 &view_mat, const mat4 &proj_mat, vec4 view_port) {
	mat4 inverse_mat = mat4_inverse(proj_mat * view_mat);
	vec4 tmp = vec4{window_coord.x, window_coord.y, window_coord.z, 1};
	tmp.x = (tmp.x - view_port.x) / view_port.z;
	tmp.y = (tmp.y - view_port.y) / view_port.w;
	tmp = tmp * 2 - 1;
	vec4 obj = inverse_mat * tmp;
	obj = obj / obj.w;
	return vec3{obj.x, obj.y, obj.z};
}

mat4 mat4_vulkan_clip() {
	return mat4{
		vec4{1, 0, 0, 0},
		vec4{0, -1, 0, 0,},
		vec4{0, 0, 0.5f, 0},
		vec4{0, 0, 0.5f, 1}
	};
}

quat quat_identity() {
	return quat{0, 0, 0, 1};
}

quat quat_normalize(quat q) {
	float len = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	m_debug_assert(len > 0);
	return q / len;
}

quat quat_inverse(quat q) {
	return quat{q.x, q.y, q.z, -q.w};
}

mat3 quat_to_mat3(quat q) {
	float qxx = q.x * q.x;
	float qyy = q.y * q.y;
	float qzz = q.z * q.z;
	float qxz = q.x * q.z;
	float qxy = q.x * q.y;
	float qyz = q.y * q.z;
	float qwx = q.w * q.x;
	float qwy = q.w * q.y;
	float qwz = q.w * q.z;
	mat3 m;
	m.c1 = vec3{1 - 2 * (qyy + qzz), 2 * (qxy + qwz), 2 * (qxz - qwy)};
	m.c2 = vec3{2 * (qxy - qwz), 1 - 2 * (qxx + qzz), 2 * (qyz + qwx)};
	m.c3 = vec3{2 * (qxz + qwy), 2 * (qyz - qwx), 1 - 2 * (qxx + qyy)};
	return m;
}

mat4 quat_to_mat4(quat q) {
	float qxx = q.x * q.x;
	float qyy = q.y * q.y;
	float qzz = q.z * q.z;
	float qxz = q.x * q.z;
	float qxy = q.x * q.y;
	float qyz = q.y * q.z;
	float qwx = q.w * q.x;
	float qwy = q.w * q.y;
	float qwz = q.w * q.z;
	mat4 m;
	m.c1 = vec4{1 - 2 * (qyy + qzz), 2 * (qxy + qwz), 2 * (qxz - qwy), 0};
	m.c2 = vec4{2 * (qxy - qwz), 1 - 2 * (qxx + qzz), 2 * (qyz + qwx), 0};
	m.c3 = vec4{2 * (qxz + qwy), 2 * (qyz - qwx), 1 - 2 * (qxx + qyy), 0};
	m.c4 = vec4{0, 0, 0, 1};
	return m;
}

vec3 quat_to_euler_angles(quat q) {
	return vec3{
		atan2f(2 * (q.q0 * q.q1 + q.q2 * q.q3), 1 - 2 * (q.q1 * q.q1 + q.q2 * q.q2)),
		asinf(2 * (q.q0 * q.q2 - q.q3 * q.q1)),
		atan2f(2 * (q.q0 * q.q3 + q.q1 * q.q2), 1 - 2 * (q.q2 * q.q2 + q.q3 * q.q3))
	};
}

quat quat_from_euler_angles(float x, float y, float z) {
	vec3 euler_angles = {x * 0.5f, y * 0.5f, z *0.5f};
	vec3 c = vec3{cosf(euler_angles.x), cosf(euler_angles.y), cosf(euler_angles.z)};
	vec3 s = vec3{sinf(euler_angles.x), sinf(euler_angles.y), sinf(euler_angles.z)};
	quat q;
	q.w = c.x * c.y * c.z + s.x * s.y * s.z;
	q.x = s.x * c.y * c.z - c.x * s.y * s.z;
	q.y = c.x * s.y * c.z + s.x * c.y * s.z;
	q.z = c.x * c.y * s.z - s.x * s.y * c.z;
	return q;
}

quat quat_from_rotation(vec3 axis, float angle) {
	axis = vec3_normalize(axis);
	float sin = sinf(angle / 2);
	float cos = cosf(angle / 2);
	quat q;
	q.x = axis.x * sin;
	q.y = axis.y * sin;
	q.z = axis.z * sin;
	q.w = cos;
	return q;
}

quat quat_from_between(vec3 a, vec3 b) {
	float adb = vec3_dot(a, b);
	if (adb < -0.999999f) {
		vec3 txa = vec3_cross(vec3{1, 0, 0}, a);
		if (vec3_len(txa) < 0.000001) {
			txa = vec3_cross(vec3{0, 1, 0}, a);
		}
		return quat_from_rotation(txa, (float)M_PI);
	}
	else if (adb > 0.999999f) {
		return quat{0, 0, 0, 1};
	}
	else {
		vec3 axb = vec3_cross(a, b);
		quat q;
		q.x = axb.x;
		q.y = axb.y;
		q.z = axb.z;
		q.w = 1 + adb;
		return quat_normalize(q);
	}
}

quat quat_slerp(quat q1, quat q2, float t) {
	m_debug_assert(t >= 0 && t <= 1);
	quat q3 = q2;
	float cos_theta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	if (cos_theta < 0) {
		q3 = -q2;
		cos_theta = -cos_theta;
	}
	if (cos_theta > 0.9995f) {
		return quat{q1.x + t * (q3.x - q1.x), q1.y + t * (q3.y - q1.y), q1.z + t * (q3.z - q1.z), q1.w + t * (q3.w - q1.w)};
	}
	else {
		float angle = acosf(cos_theta);
		return (q1 * sinf((1 - t) * angle) + q3 * sinf(t * angle)) / sinf(angle);
	}
}

transform transform_identity() {
	return transform{vec3{1, 1, 1}, quat{0, 0, 0, 1}, vec3{0, 0, 0}};
}

transform transform_from_translation(const vec3 &t) {
	return transform{vec3{1, 1, 1}, quat{0, 0, 0, 1}, t};
}

mat4 transform_to_mat4(const transform &t) {
	return mat4_from_translation(t.translate) * quat_to_mat4(t.rotate) * mat4_from_scaling(t.scale);
}

mat4 camera_projection_mat4(const camera &camera) {
	return mat4_perspective_projection(camera.fovy, camera.aspect, camera.znear, camera.zfar);
}

mat4 camera_view_mat4(const camera &camera) {
	return mat4_look_at(camera.position, camera.position + camera.view, camera.up);
}

mat4 camera_view_projection_mat4(const camera &camera) {
	return camera_projection_mat4(camera) * camera_view_mat4(camera);
}

mat4 camera_billboard_mat4(const camera &camera) {
	mat3 mat = mat3_transpose(mat4_to_mat3(camera_view_mat4(camera)));
	return mat4{
		vec4{mat.columns[0].x, mat.columns[0].y, mat.columns[0].z, 0},
		vec4{mat.columns[1].x, mat.columns[1].y, mat.columns[1].z, 0},
		vec4{mat.columns[2].x, mat.columns[2].y, mat.columns[2].z, 0},
		vec4{0, 0, 0, 1}
	};
}

mat4 camera_shadow_map_projection_mat4(const camera &camera, vec3 directional_light) {
	// directional_light = vec3_normalize(directional_light);

	// vec3 camera_right = vec3_cross(camera.view, camera.up);

	// float near_plane_height = tanf(camera.fovy / 2) * camera.znear * 2;
	// float near_plane_width = near_plane_height * camera.aspect;
	// vec3 near_plane_center = camera.position + camera.view * camera.znear;
	// float far_plane_height = tanf(camera.fovy / 2) * camera.zfar * 2;
	// float far_plane_width = far_plane_height * camera.aspect;
	// vec3 far_plane_center = camera.position + camera.view * camera.zfar;

	// vec3 view_frustum_center = (near_plane_center + far_plane_center) / 2;
	// vec3 light_position = view_frustum_center - directional_light * camera.zfar;
	// vec3 light_lookat_up = vec3_normalize(vec3_cross(vec3_cross(directional_light, vec3{0, 1, 0}), directional_light));
	// mat4 light_view_mat4 = mat4_lookat(light_position, view_frustum_center, light_lookat_up);

	// vec3 view_frustum_corners[8];
	// view_frustum_corners[0] = near_plane_center + (camera.up * (near_plane_height / 2)) - (camera_right * (near_plane_width / 2));
	// view_frustum_corners[1] = near_plane_center + (camera.up * (near_plane_height / 2)) + (camera_right * (near_plane_width / 2));
	// view_frustum_corners[2] = near_plane_center - (camera.up * (near_plane_height / 2)) - (camera_right * (near_plane_width / 2));
	// view_frustum_corners[3] = near_plane_center - (camera.up * (near_plane_height / 2)) + (camera_right * (near_plane_width / 2));
	// view_frustum_corners[4] = far_plane_center + (camera.up * (far_plane_height / 2)) - (camera_right * (far_plane_width / 2));
	// view_frustum_corners[5] = far_plane_center + (camera.up * (far_plane_height / 2)) + (camera_right * (far_plane_width / 2));
	// view_frustum_corners[6] = far_plane_center - (camera.up * (far_plane_height / 2)) - (camera_right * (far_plane_width / 2));
	// view_frustum_corners[7] = far_plane_center - (camera.up * (far_plane_height / 2)) + (camera_right * (far_plane_width / 2));
	// for (uint32 i = 0; i < m_countof(view_frustum_corners); i += 1) {
	// 	view_frustum_corners[i] = light_view_mat4 * view_frustum_corners[i];
	// }
	// vec3 bound_min = {};
	// vec3 bound_max = {};
	// for (uint32 i = 0; i < m_countof(view_frustum_corners); i += 1) {
	// 	bound_min.x = min(bound_min.x, view_frustum_corners[i].x);
	// 	bound_min.y = min(bound_min.y, view_frustum_corners[i].y);
	// 	bound_min.z = min(bound_min.z, view_frustum_corners[i].z);
	// 	bound_max.x = max(bound_max.x, view_frustum_corners[i].x);
	// 	bound_max.y = max(bound_max.y, view_frustum_corners[i].y);
	// 	bound_max.z = max(bound_max.z, view_frustum_corners[i].z);
	// }

	// return mat4_orthographic_projection(bound_min.x, bound_max.x, bound_min.y, bound_max.y, -bound_max.z, -bound_min.z) * light_view_mat4;

	mat4 light_view_mat4 = mat4_look_at(directional_light * 50, {0, 0, 0}, {0, 1, 0});
	return mat4_orthographic_projection(-50, 50, -50, 50, 0, 100) * light_view_mat4;
}

float aa_bound_volume(const aa_bound &bound) {
	return fabsf(bound.min.x - bound.max.x) * fabsf(bound.min.y - bound.max.y) * fabsf(bound.min.z - bound.max.z);
}

vec3 aa_bound_size(const aa_bound &bound) {
	return vec3{bound.max.x - bound.min.x, bound.max.y - bound.min.y, bound.max.z - bound.min.z};
}

vec3 aa_bound_center(const aa_bound &bound) {
	return (bound.min + bound.max) * 0.5f;
}

vec3 aa_bound_bottom_center(const aa_bound &bound) {
	return vec3{bound.min.x + (bound.max.x - bound.min.x) / 2, bound.min.y, bound.min.z + (bound.max.z - bound.min.z) / 2};
}

aa_bound aa_bound_translate(const aa_bound &bound, const vec3 &v) {
	return aa_bound{bound.min + v, bound.max + v};
}

aa_bound aa_bound_scale(const aa_bound &bound, const vec3 &v) {
	return aa_bound{bound.min * v, bound.max * v};
}

aa_bound aa_bound_rotate(const aa_bound &bound, const quat &quat) {
	mat4 mat = quat_to_mat4(quat);
	aa_bound new_bound = {};
	for (int i = 0; i < 3; i += 1) {
		for (int j = 0; j < 3; j += 1) {
			float e = mat.columns[j].e[i] * bound.min.e[j];
			float f = mat.columns[j].e[i] * bound.max.e[j];
			if (e < f) {
				new_bound.min.e[i] += e;
				new_bound.max.e[i] += f;
			}
			else {
				new_bound.min.e[i] += f;
				new_bound.max.e[i] += e;
			}
		}
	}
	return new_bound;
}

bool aa_bound_intersect(const aa_bound &bound1, const aa_bound &bound2) {
	if (bound1.max.x < bound2.min.x || bound1.min.x > bound2.max.x) {
		return false;
	}
	if (bound1.max.y < bound2.min.y || bound1.min.y > bound2.max.y) {
		return false;
	}
	if (bound1.max.z < bound2.min.z || bound1.min.z > bound2.max.z) {
		return false;
	}
	return true;
}

aa_bound aa_bound_expand(const aa_bound &bound1, const aa_bound &bound2) {
	aa_bound bound = bound1;
	if (bound2.min.x < bound1.min.x || bound2.min.y < bound1.min.y || bound2.min.z < bound1.min.z) {
		bound.min = bound2.min;
	}
	if (bound2.max.x > bound1.max.x || bound2.max.y > bound1.max.y || bound2.max.z > bound1.max.z) {
		bound.max = bound2.max;
	}
	return bound;
}

bool ray_intersect_sphere(const ray &ray, const sphere &sphere, float *intersection = nullptr) {
	vec3 m = ray.origin - sphere.center;
	float b = vec3_dot(m, ray.direction);
	float c = vec3_dot(m, m) - sphere.radius * sphere.radius;
	if (b > 0 && c > 0) {
		return false;
	}
	float discr = b * b - c;
	if (discr < 0) {
		return false;
	}
	if (intersection) {
		float t = -b - sqrtf(discr);
		*intersection = t < 0 ? 0 : t;
	}
	return true;
}

bool ray_intersect_capsule(const ray &ray, const capsule &capsule, float *intersection = nullptr) {
	const vec3 sa = ray.origin;
	const vec3 sb = ray.origin + ray.direction * ray.len;
	const vec3 p = capsule.begin;
	const vec3 q = capsule.end;
	const float r = capsule.radius;

	auto test_spheres = [&] {
		return ray_intersect_sphere(ray, {p, r}, intersection) || ray_intersect_sphere(ray, {q, r}, intersection);
	};

	vec3 d = q - p, m = sa - p, n = sb - sa;
  float md = vec3_dot(m, d);
  float nd = vec3_dot(n, d);
  float dd = vec3_dot(d, d);
  if (md < 0 && md + nd < 0) {
  	return test_spheres(); // Segment outside ’p’ side of cylinder
  }
  if (md > dd && md + nd > dd) {
  	return test_spheres(); // Segment outside ’q’ side of cylinder
  }
  float t = 0;
  float nn = vec3_dot(n, n);
  float mn = vec3_dot(m, n);
  float a = dd * nn - nd * nd;
  float k = vec3_dot(m, m) - r * r;
  float c = dd * k - md * md;
  if (fabsf(a) < FLT_EPSILON) { // Segment runs parallel to cylinder axis
    if (c > 0) {
    	return test_spheres(); // ’a’ and thus the segment lie outside cylinder
    }
    if (md < 0) {
	    t = -mn / nn; // Intersect segment against ’p’ endcap
	  }
    else if (md > dd) {
    	t = (nd - mn) / nn; // Intersect segment against ’q’ endcap
    }
    else {
	    t = 0; // ’a’ lies inside cylinder
	  }
  }
  else {
	  float b = dd * mn - nd * md;
	  float discr = b * b - a * c;
	  if (discr < 0) {
		  return test_spheres(); // No real roots; no intersection
		}
	  t = (-b - sqrtf(discr)) / a;
	  if (t < 0 || t > 1) {
	  	return test_spheres(); // Intersection lies outside segment
	  }
	  if (md + t * nd < 0) { // Intersection outside cylinder on ’p’ side
	    if (nd <= 0) {
	    	return test_spheres(); // Segment pointing away from endcap
	    }
	    t = -md / nd;
	    // Keep intersection if Dot(S(t) - p, S(t) - p) <= r∧2
	    if (k + 2 * t * (mn + t * nn) > 0) {
	    	return test_spheres();
	    }
	  }
	  else if (md + t * nd > dd) { // Intersection outside cylinder on ’q’ side
	    if (nd >= 0) {
	    	return test_spheres(); // Segment pointing away from endcap
	    }
	    t = (dd - md) / nd;
	    // Keep intersection if Dot(S(t) - q, S(t) - q) <= r∧2
	    if (k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) > 0) {
	    	return test_spheres();
	    }
	  }
	}
  // Segment intersects cylinder between the endcaps; t is correct
	if (intersection) {
		*intersection = ray.len * t;
	}
	return true;
}

bool ray_intersect_bound(const ray &ray, const aa_bound &bound, vec2 *intersection = nullptr) {
	float t0 = 0;
	float t1 = ray.len;
	for (uint32 i = 0; i < 3; i += 1) {
		float fnear = (bound.min[i] - ray.origin[i]) / ray.direction[i];
		float ffar = (bound.max[i] - ray.origin[i]) / ray.direction[i];
		if (fnear > ffar) {
			float fnear2 = fnear;
			fnear = ffar;
			ffar = fnear2;
		}
		if (fnear > t0) {
			t0 = fnear;
		}
		if (ffar < t1) {
			t1 = ffar;
		}
		if (t0 > t1) {
			return false;
		}
	}
	if (intersection) {
		intersection->x = t0;
		intersection->y = t1;
	}
	return true;
}

uint32 ray_intersect_bounds_nearest(const ray &ray, const aa_bound *bounds, uint32 bound_count) {
	float nearest = INFINITY;
	uint32 bound_index = UINT32_MAX;
	for (uint32 i = 0; i < bound_count; i += 1) {
		vec2 intersection = {};
		if (ray_intersect_bound(ray, bounds[i], &intersection) && intersection.x < nearest) {
			nearest = intersection.x;
			bound_index = i;
		}
	}
	return bound_index;
}

bool ray_intersect_triangle(const ray &ray, vec3 a, vec3 b, vec3 c, float *distance = nullptr, vec3 *barycentric_coord = nullptr) {
	vec3 ab = b - a;
	vec3 ac = c - a;
	vec3 qp = -ray.direction * ray.len;
	vec3 n = vec3_cross(ab, ac);
	float d = vec3_dot(qp, n);
	if (d <= 0) {
		return false;
	}
	vec3 ap = ray.origin - a;
	float t = vec3_dot(ap, n);
	if (t < 0 || t > d) {
		return false;
	}
	vec3 e = vec3_cross(qp, ap);
	float v = vec3_dot(ac, e);
	if (v < 0 || v > d) {
		return false;
	}
	float w = -vec3_dot(ab, e);
	if (w < 0 || v + w > d) {
		return false;
	}
	float ood = 1 / d;
	if (distance) {
		*distance = t * ood * ray.len;
	}
	if (barycentric_coord) {
		barycentric_coord->v = v * ood;
		barycentric_coord->w = w * ood;
		barycentric_coord->u = 1 - barycentric_coord->v - barycentric_coord->w;
	}
	return true;
}

float degree_to_radian(float degree) {
	return degree * (float)M_PI / 180;
}

float radian_to_degree(float radian) {
	return radian / (float)M_PI * 180;
}

float wrap_angle(float angle) {
	angle = fmodf(angle, (float)M_PI * 2);
	if (angle < 0) {
		angle += (float)M_PI * 2;
	}
	return angle;
}

uint32 mipmap_levels(uint32 width, uint32 height) {
	return (int)floorf(log2f((float)max(width, height))) + 1;
}

float luminance(float r, float g, float b) {
	return sqrtf(0.299f * r * r + 0.587f * g * g + 0.114f * b * b);
}

void barycentric_coords(vec3 a, vec3 b, vec3 c, vec3 *p, uint32 p_count, vec3 *coords) {
	vec3 v0 = b - a;
	vec3 v1 = c - a;
	float d00 = vec3_dot(v0, v0);
	float d01 = vec3_dot(v0, v1);
	float d11 = vec3_dot(v1, v1);
	float denom = d00 * d11 - d01 * d01;

	for (uint32 i = 0; i < p_count; i += 1) {
		vec3 v2 = p[i] - a;
		float d20 = vec3_dot(v2, v0);
		float d21 = vec3_dot(v2, v1);
		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1 - v - w;
		coords[i] = vec3{v, w, u};
	}
}

bool barycentric_coord_is_inside(vec3 coord) {
	return coord.x >= 0 && coord.y >= 0 && (coord.x + coord.y) <= 1;
}

float horizontal_fov(float vertical_fov, float aspect_ratio) {
	return 2 * atanf(tanf(vertical_fov / 2) * aspect_ratio);
}

float vertical_fov(float horizontal_fov, float aspect_ratio) {
	return 2 * atanf(tanf(horizontal_fov / 2) * (1 / aspect_ratio));
}

bool point_inside_rect(int point_x, int point_y, int rect_x, int rect_y, int rect_w, int rect_h) {
	return (point_x > rect_x && point_x < (rect_x + rect_w) && point_y > rect_y && point_y < (rect_y + rect_h));
}

vec4 fit_rect_into_rect(float rect_outer_width, float rect_outer_height, float rect_inner_width, float rect_inner_height) {
	float scale = min(rect_outer_width / rect_inner_width, rect_outer_height / rect_inner_height);
	rect_inner_width *= scale;
	rect_inner_height *= scale;
	float x = (rect_outer_width - rect_inner_width) / 2;
	float y = (rect_outer_height - rect_inner_height) / 2;
	return vec4{x, y, rect_inner_width, rect_inner_height};
}

void flip_rgba_image(void *image, uint32 w, uint32 h) {
	for (uint32 i = 0; i < (h / 2); i += 1) {
		uint32 *row_1 = (uint32 *)image + i * w;
		uint32 *row_2 = (uint32 *)image + (h - 1 - i) * w;
		for (uint32 i = 0; i < w; i += 1) {
			uint32 tmp_pixel = row_1[i];
			row_1[i] = row_2[i];
			row_2[i] = tmp_pixel;
		}
	}
}

void convert_rgba_image_to_bgra(uint8 *image, uint width, uint height) {
	for (uint i = 0; i < height; i += 1) {
		uint8 *row = image + i * width * 4;
		for (uint i = 0; i < width; i += 1) {
			uint8 *pixel = row + i * 4;
			uint8 r = pixel[0];
			pixel[0] = pixel[2];
			pixel[2] = r;
		}
	}
}

#define m_quad_verts_xyz_st(buf, x, y, z, w, h, s0, t0, s1, t1)                       \
  float buf[] = {x, y, z, s0, t0, x + w, y, z, s1, t0, x, y + h, z, s0, t1,           \
                 x, y + h, z, s0, t1, x + w, y, z, s1, t0, x + w, y + h, z, s1, t1};

#define m_quad_verts_xyz_rgba_st(buf, x, y, z, w, h, r, g, b, a, s0, t0, s1, t1)                                          \
  float buf[] = {x, y, z, r, g, b, a, s0, t0, x + w, y, z, r, g, b, a, s1, t0, x, y + h, z, r, g, b, a, s0, t1,           \
                 x, y + h, z, r, g, b, a, s0, t1, x + w, y, z, r, g, b, a, s1, t0, x + w, y + h, z, r, g, b, a, s1, t1};

#define m_cube_verts_xyz(buf, x, y, z, w, h, d)                                           \
  float buf[] = {x, y, z, x, y, z + d, x, y + h, z + d, x + w, y + h, z,                  \
                 x, y, z, x, y + h, z, x + w, y, z + d, x, y, z,                          \
                 x + w, y, z, x + w, y + h, z, x + w, y, z, x, y, z,                      \
                 x, y, z, x, y + h, z + d, x, y + h, z, x + w, y, z + d,                  \
                 x, y, z + d, x, y, z, x, y + h, z + d, x, y, z + d,                      \
                 x + w, y, z + d, x + w, y + h, z + d, x + w, y, z, x + w, y + h, z,      \
                 x + w, y, z, x + w, y + h, z + d, x + w, y, z + d, x + w, y + h, z + d,  \
                 x + w, y + h, z, x, y + h, z, x + w, y + h, z + d, x, y + h, z,          \
                 x, y + h, z + d, x + w, y + h, z + d, x, y + h, z + d, x + w, y, z + d};

#define m_cube_verts_xyz_rgb(buf, x, y, z, w, h, d, r, g, b)                                                                  \
  float buf[] = {x, y, z, r, g, b, x, y, z + d, r, g, b, x, y + h, z + d, r, g, b, x + w, y + h, z, r, g, b,                  \
                 x, y, z, r, g, b, x, y + h, z, r, g, b, x + w, y, z + d, r, g, b, x, y, z, r, g, b,                          \
                 x + w, y, z, r, g, b, x + w, y + h, z, r, g, b, x + w, y, z, r, g, b, x, y, z, r, g, b,                      \
                 x, y, z, r, g, b, x, y + h, z + d, r, g, b, x, y + h, z, r, g, b, x + w, y, z + d, r, g, b,                  \
                 x, y, z + d, r, g, b, x, y, z, r, g, b, x, y + h, z + d, r, g, b, x, y, z + d, r, g, b,                      \
                 x + w, y, z + d, r, g, b, x + w, y + h, z + d, r, g, b, x + w, y, z, r, g, b, x + w, y + h, z, r, g, b,      \
                 x + w, y, z, r, g, b, x + w, y + h, z + d, r, g, b, x + w, y, z + d, r, g, b, x + w, y + h, z + d, r, g, b,  \
                 x + w, y + h, z, r, g, b, x, y + h, z, r, g, b, x + w, y + h, z + d, r, g, b, x, y + h, z, r, g, b,          \
                 x, y + h, z + d, r, g, b, x + w, y + h, z + d, r, g, b, x, y + h, z + d, r, g, b, x + w, y, z + d, r, g, b};

#define m_cube_verts_xyz_line_strip(buf, x, y, z, w, h, d)                            \
  float buf[] = {x, y, z + d, x + w, y, z + d, x + w, y + h, z + d, x, y + h, z + d,  \
                 x, y, z + d, x, y, z, x, y + h, z, x, y + h, z + d,                  \
                 x + w, y + h, z + d, x + w, y + h, z, x, y + h, z, x, y, z,          \
                 x + w, y, z, x + w, y + h, z, x + w, y, z, x + w, y, z + d};

#define m_cube_verts_xyz_rgb_line_strip(buf, x, y, z, w, h, d, r, g, b)                                                   \
  float buf[] = {x, y, z + d, r, g, b, x + w, y, z + d, r, g, b, x + w, y + h, z + d, r, g, b, x, y + h, z + d, r, g, b,  \
                 x, y, z + d, r, g, b, x, y, z, r, g, b, x, y + h, z, r, g, b, x, y + h, z + d, r, g, b,                  \
                 x + w, y + h, z + d, r, g, b, x + w, y + h, z, r, g, b, x, y + h, z, r, g, b, x, y, z, r, g, b,          \
                 x + w, y, z, r, g, b, x + w, y + h, z, r, g, b, x + w, y, z, r, g, b, x + w, y, z + d, r, g, b};

#define m_circle_verts_xyz(buf, x, y, z, radius, slices)   \
  float buf[slices * 9];                                   \
  {                                                        \
    float delta = (M_PI * 2) / slices;                     \
    float angle = delta;                                   \
    float prev_x = x + radius;                             \
    float prev_y = y;                                      \
    float prev_z = z;                                      \
    for (uint i = 0; i < slices; i += 1) {                 \
      float new_x = x + cosf(angle) * radius;              \
      float new_y = y + sinf(angle) * radius;              \
      float new_z = z;                                     \
      buf[i * 9 + 0] = x;                                  \
      buf[i * 9 + 1] = y;                                  \
      buf[i * 9 + 2] = z;                                  \
      buf[i * 9 + 3] = prev_x;                             \
      buf[i * 9 + 4] = prev_y;                             \
      buf[i * 9 + 5] = prev_z;                             \
      buf[i * 9 + 6] = new_x;                              \
      buf[i * 9 + 7] = new_y;                              \
      buf[i * 9 + 8] = new_z;                              \
      prev_x = new_x;                                      \
      prev_y = new_y;                                      \
      angle += delta;                                      \
    }                                                      \
  }

#define m_circle_verts_xyz_rgb(buf, x, y, z, radius, r, g, b, slices)   \
  float buf[slices * 18];                                               \
  {                                                                     \
   float delta = ((float)M_PI * 2) / slices;                            \
   float angle = delta;                                                 \
   float prev_x = x + radius;                                           \
   float prev_y = y;                                                    \
   float prev_z = z;                                                    \
   for (uint i = 0; i < slices; i += 1) {                               \
     float new_x = x + cosf(angle) * radius;                            \
     float new_y = y + sinf(angle) * radius;                            \
     float new_z = z;                                                   \
     buf[i * 18 + 0] = x;                                               \
     buf[i * 18 + 1] = y;                                               \
     buf[i * 18 + 2] = z;                                               \
     buf[i * 18 + 3] = r;                                               \
     buf[i * 18 + 4] = g;                                               \
     buf[i * 18 + 5] = b;                                               \
     buf[i * 18 + 6] = prev_x;                                          \
     buf[i * 18 + 7] = prev_y;                                          \
     buf[i * 18 + 8] = prev_z;                                          \
     buf[i * 18 + 9] = r;                                               \
     buf[i * 18 + 10] = g;                                              \
     buf[i * 18 + 11] = b;                                              \
     buf[i * 18 + 12] = new_x;                                          \
     buf[i * 18 + 13] = new_y;                                          \
     buf[i * 18 + 14] = new_z;                                          \
     buf[i * 18 + 15] = r;                                              \
     buf[i * 18 + 16] = g;                                              \
     buf[i * 18 + 17] = b;                                              \
     prev_x = new_x;                                                    \
     prev_y = new_y;                                                    \
     angle += delta;                                                    \
   }                                                                    \
  }

#define m_hollow_circle_verts_xyz_rgb(buf, x, y, z, radius1, radius2, r, g, b, slices)          \
  float buf[6 * 6 * slices];                                                                    \
  {                                                                                             \
   float delta = ((float)M_PI * 2) / slices;                                                    \
   float angle = delta;                                                                         \
   vec6 prev_verts[2] = {{x + radius1, y, z, r, g, b}, {x + radius2, y, z, r, g, b}};           \
   for (uint i = 0; i < slices; i += 1) {                                                       \
     vec6 new_verts[2] = {{x + cosf(angle) * radius1, y + sinf(angle) * radius1, z, r, g, b},   \
                          {x + cosf(angle) * radius2, y + sinf(angle) * radius2, z, r, g, b}};  \
     ((vec6*)buf)[i * 6 + 0] = prev_verts[0];                                                   \
     ((vec6*)buf)[i * 6 + 1] = prev_verts[1];                                                   \
     ((vec6*)buf)[i * 6 + 2] = new_verts[0];                                                    \
     ((vec6*)buf)[i * 6 + 3] = new_verts[0];                                                    \
     ((vec6*)buf)[i * 6 + 4] = prev_verts[1];                                                   \
     ((vec6*)buf)[i * 6 + 5] = new_verts[1];                                                    \
     prev_verts[0] = new_verts[0];                                                              \
     prev_verts[1] = new_verts[1];                                                              \
     angle += delta;                                                                            \
   }                                                                                            \
  }

#define m_cone_verts_xyz(buf, x, y, z, radius, height, sectors)           \
  float buf[sectors * 18];                                                \
  {                                                                       \
    float delta_angle = (M_PI * 2) / sectors;                             \
    vec3 top = {x, y + height, z};                                        \
    vec3 prev = {x + radius, y, z};                                       \
    int size = 0;                                                         \
    for (uint i = 1; i <= sectors; i += 1) {                              \
      float angle = i * delta_angle;                                      \
      float dx = radius * cosf(angle);                                    \
      float dz = -radius * sinf(angle);                                   \
      vec3 point = {x + dx, y, z + dz};                                   \
      vec3 tmp_buf[] = {vec3{x, y, z}}, point, prev, prev, point, top};   \
      memcpy(buf + size, tmp_buf, sizeof(tmp_buf));                       \
      size += sizeof(tmp_buf) / sizeof(float);                            \
      prev = point;                                                       \
    }                                                                     \
  }

#define m_cone_verts_xyz_rgb(buf, x, y, z, radius, height, r, g, b, sectors)                             \
  float buf[sectors * 36];                                                                               \
  {                                                                                                      \
   vec3 center = {x, y, z};                                                                              \
   vec3 color = {r, g, b};                                                                               \
   float delta_angle = ((float)M_PI * 2) / sectors;                                                      \
   vec3 top = {x, y + height, z};                                                                        \
   vec3 prev = {x + radius, y, z};                                                                       \
   int size = 0;                                                                                         \
   for (uint i = 1; i <= sectors; i += 1) {                                                              \
     float angle = i * delta_angle;                                                                      \
     float dx = radius * cosf(angle);                                                                    \
     float dz = -radius * sinf(angle);                                                                   \
     vec3 point = {x + dx, y, z + dz};                                                                   \
     vec3 tmp_buf[] = {center, color, point, color, prev, color, prev, color, point, color, top, color}; \
     memcpy(buf + size, tmp_buf, sizeof(tmp_buf));                                                       \
     size += sizeof(tmp_buf) / sizeof(float);                                                            \
     prev = point;                                                                                       \
   }                                                                                                     \
  }

