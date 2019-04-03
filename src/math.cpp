/****************************************************************************************************/
/*			Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.			*/
/****************************************************************************************************/

#ifndef __MATH_CPP__
#define __MATH_CPP__

#include "common.cpp"

union vec2 {
	struct { float x, y; };
	float e[2];

	bool operator==(vec2 v) const { return (x == v.x) && (y == v.y); }
	bool operator!=(vec2 v) const { return !(*this == v); }
	vec2 operator+(vec2 v) const { return vec2{x + v.x, y + v.y}; }
	vec2 operator+(float f) const { return vec2{x + f, y + f}; }
	vec2 operator-() const { return vec2{-x, -y}; }
	vec2 operator-(vec2 v) const { return vec2{x - v.x, y - v.y}; }
	vec2 operator-(float d) const { return vec2{x - d, y - d}; }
	vec2 operator*(float d) const { return vec2{x * d, y * d}; }
	vec2 operator*(vec2 v) const { return vec2{x * v.x, y * v.y}; }
	vec2 operator/(float d) const { return vec2{x / d, y / d}; }
	vec2 operator/(vec2 v) const { return vec2{x / v.x, y / v.y}; }
	void operator+=(vec2 v) { x += v.x; y += v.y; }
	void operator-=(vec2 v) { x -= v.x; y -= v.y; }
	void operator*=(vec2 v) { x *= v.x; y *= v.y; }
	void operator*=(float f) { x *= f; y *= f; }
	void operator/=(vec2 v) { x /= v.x; y /= v.y; }
	void operator/=(float f) { x /= f; y /= f; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
};

union vec3 {
	struct { float x, y, z; };
	float e[3];

	bool operator==(vec3 v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(vec3 v) const { return !(*this == v); }
	vec3 operator+(vec3 v) const { return vec3{x + v.x, y + v.y, z + v.z}; }
	vec3 operator-() const { return vec3{-x, -y, -z}; }
	vec3 operator-(vec3 v) const { return vec3{x - v.x, y - v.y, z - v.z}; }
	vec3 operator-(float d) const { return vec3{x - d, y - d, z - d}; }
	vec3 operator*(float d) const { return vec3{x * d, y * d, z * d}; }
	vec3 operator*(vec3 v) const { return vec3{x * v.x, y * v.y, z * v.z}; }
	vec3 operator/(float d) const { return vec3{x / d, y / d, z / d}; }
	vec3 operator/(vec3 v) const { return vec3{x / v.x, y / v.y, z / v.z}; }
	void operator+=(vec3 v) { x += v.x; y += v.y; z += v.z; }
	void operator-=(vec3 v) { x -= v.x; y -= v.y; z -= v.z; }
	void operator*=(vec3 v) { x *= v.x; y *= v.y; z *= v.z; }
	void operator*=(float f) { x *= f; y *= f; z *= f; }
	void operator/=(vec3 v) { x /= v.x; y /= v.y; z /= v.z; }
	void operator/=(float f) { x /= f; y /= f; z /= f; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
	vec2 xy() { return vec2{x, y}; }
	vec2 xz() { return vec2{x, z}; }
	vec3 zxy() { return vec3{z, x, y}; }
};

union vec4 {
	struct { float x, y, z, w; };
	float e[4];

	bool operator==(vec4 v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(vec4 v) const { return !(*this == v); }
	vec4 operator+(vec4 v) const { return vec4{x + v.x, y + v.y, z + v.z, w + v.w}; }
	vec4 operator-() const { return vec4{-x, -y, -z, -w}; }
	vec4 operator-(vec4 v) const { return vec4{x - v.x, y - v.y, z - v.z, w - v.w}; }
	vec4 operator-(float d) const { return vec4{x - d, y - d, z - d, w - d}; }
	vec4 operator*(float d) const { return vec4{x * d, y * d, z * d, w * d}; }
	vec4 operator*(vec4 v) const { return vec4{x * v.x, y * v.y, z * v.z, w * v.w}; }
	vec4 operator/(float d) const { return vec4{x / d, y / d, z / d, w / d}; }
	vec4 operator/(vec4 v) const { return vec4{x / v.x, y / v.y, z / v.z, w / v.w}; }
	void operator+=(vec4 v) { x += v.x; y += v.y; z += v.z; w += v.w; }
	void operator-=(vec4 v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	void operator*=(vec4 v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
	void operator*=(float f) { x *= f; y *= f; z *= f; w *= f; }
	void operator/=(vec4 v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; }
	void operator/=(float f) { x /= f; y /= f; z /= f; w /= f; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
	vec3 xyz() const { return vec3{x, y, z}; }
};

union i8vec3 {
	struct { int8 x, y, z; };
	int8 e[3];

	bool operator==(i8vec3 v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(i8vec3 v) const { return !(*this == v); }
	int8& operator[](uint32 i) { return e[i]; }
	const int8& operator[](uint32 i) const { return e[i]; }
};

union u8vec3 {
	struct { uint8 x, y, z; };
	uint8 e[3];

	bool operator==(u8vec3 v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(u8vec3 v) const { return !(*this == v); }
	uint8& operator[](uint32 i) { return e[i]; }
	const uint8& operator[](uint32 i) const { return e[i]; }
};

union i8vec4 {
	struct { int8 x, y, z, w; };
	int8 e[4];

	bool operator==(i8vec4 v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(i8vec4 v) const { return !(*this == v); }
	int8& operator[](uint32 i) { return e[i]; }
	const int8& operator[](uint32 i) const { return e[i]; }
};

union u8vec4 {
	struct { uint8 x, y, z, w; };
	uint8 e[4];

	bool operator==(u8vec4 v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(u8vec4 v) const { return !(*this == v); }
	uint8& operator[](uint32 i) { return e[i]; }
	const uint8& operator[](uint32 i) const { return e[i]; }
};

union i16vec3 {
	struct { int16 x, y, z; };
	int16 e[3];

	bool operator==(i16vec3 v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(i16vec3 v) const { return !(*this == v); }
	int16& operator[](uint32 i) { return e[i]; }
	const int16& operator[](uint32 i) const { return e[i]; }
};

union i16vec4 {
	struct { int16 x, y, z, w; };
	int16 e[4];

	bool operator==(i16vec4 v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(i16vec4 v) const { return !(*this == v); }
	int16& operator[](uint32 i) { return e[i]; }
	const int16& operator[](uint32 i) const { return e[i]; }
};

union u16vec4 {
	struct { uint16 x, y, z, w; };
	uint16 e[4];

	bool operator==(u16vec4 v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(u16vec4 v) const { return !(*this == v); }
	uint16& operator[](uint32 i) { return e[i]; }
	const uint16& operator[](uint32 i) const { return e[i]; }
};

union u32vec4 {
	struct { uint32 x, y, z, w; };
	uint32 e[4];

	bool operator==(u32vec4 v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(u32vec4 v) const { return !(*this == v); }
	uint32& operator[](uint32 i) { return e[i]; }
	const uint32& operator[](uint32 i) const { return e[i]; }
};

union mat3 {
	struct { vec3 c1, c2, c3; };
	vec3 columns[3];

	bool operator==(mat3 m) const { return (c1 == m.c1) && (c2 == m.c2) && (c3 == m.c3); }
	bool operator!=(mat3 m) const { return !(*this == m); }
	mat3 operator+(mat3 m) const { return mat3{c1 + m.c1, c2 + m.c2, c3 + m.c3}; }
	mat3 operator*(float f) const { return mat3{c1 * f, c2 * f, c3 * f}; }
	vec3 operator*(vec3 v) const {
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
	mat3 operator*(mat3 m) const {
		mat3 result;
		result.c1.x = c1.x * m.c1.x + c2.x * m.c1.y + c3.x * m.c1.z;
		result.c1.y = c1.y * m.c1.x + c2.y * m.c1.y + c3.y * m.c1.z;
		result.c1.z = c1.z * m.c1.x + c2.z * m.c1.y + c3.z * m.c1.z;
		result.c2.x = c1.x * m.c2.x + c2.x * m.c2.y + c3.x * m.c2.z;
		result.c2.y = c1.y * m.c2.x + c2.y * m.c2.y + c3.y * m.c2.z;
		result.c2.z = c1.z * m.c2.x + c2.z * m.c2.y + c3.z * m.c2.z;
		result.c3.x = c1.x * m.c3.x + c2.x * m.c3.y + c3.x * m.c3.z;
		result.c3.y = c1.y * m.c3.x + c2.y * m.c3.y + c3.y * m.c3.z;
		result.c3.z = c1.z * m.c3.x + c2.z * m.c3.y + c3.z * m.c3.z;
		return result;		
	}
	vec3& operator[](uint32 i) { return columns[i]; }
	const vec3& operator[](uint32 i) const { return columns[i]; }
	explicit operator float*() { return &c1.x; }
	explicit operator const float*() const { return &c1.x; }
};

union mat4 {
	struct { vec4 c1, c2, c3, c4; };
	vec4 columns[4];

	bool operator==(mat4 m) const { return (c1 == m.c1) && (c2 == m.c2) && (c3 == m.c3) && (c4 == m.c4); }
	bool operator!=(mat4 m) const { return !(*this == m); }
	mat4 operator+(mat4 m) const { return mat4{c1 + m.c1, c2 + m.c2, c3 + m.c3, c4 + m.c4}; }
	mat4 operator*(float f) const { return mat4{c1 * f, c2 * f, c3 * f, c4 * f}; }
	vec4 operator*(vec4 v) const {
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
	vec3 operator*(vec3 v) const { vec4 v4 = *this * vec4{v.x, v.y, v.z, 1}; return vec3{v4.x, v4.y, v4.z}; }
	mat4 operator*(mat4 m) const {
		mat4 result;
		result.c1 = c1 * m.c1.x + c2 * m.c1.y + c3 * m.c1.z + c4 * m.c1.w;
		result.c2 = c1 * m.c2.x + c2 * m.c2.y + c3 * m.c2.z + c4 * m.c2.w;
		result.c3 = c1 * m.c3.x + c2 * m.c3.y + c3 * m.c3.z + c4 * m.c3.w;
		result.c4 = c1 * m.c4.x + c2 * m.c4.y + c3 * m.c4.z + c4 * m.c4.w;
		return result;
	}
	vec4& operator[](uint32 i) { return columns[i]; }
	const vec4& operator[](uint32 i) const { return columns[i]; }
	explicit operator float*() { return &c1.x; }
	explicit operator const float*() const { return &c1.x; }
};

union quat {
	struct { float x, y, z, w; };
	float e[4];

	bool operator==(quat q) const { return x == q.x && y == q.y && z == q.z && w == q.w; }
	bool operator!=(quat q) const { return !(*this == q); }
	quat operator+(quat q) const { return quat{x + q.x, y + q.y, z + q.z, w + q.w}; }
	quat operator-() const { return quat{-x, -y, -z, -w}; }
	quat operator*(float d) const { return quat{x * d, y * d, z * d, w * d}; }
	vec3 operator*(vec3 v) const {
		vec3 vec3_cross(vec3 v1, vec3 v2);
		vec3 t = vec3_cross(vec3{x, y, z}, v) * 2;
		return v + t * w + vec3_cross(vec3{x, y, z}, t);
	}
	quat operator*(quat q) const {
		quat result = {
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y + y * q.w + z * q.x - x * q.z,
			w * q.z + z * q.w + x * q.y - y * q.x,
			w * q.w - x * q.x - y * q.y - z * q.z
		};
		return result;
	}
	void operator*=(quat q) { *this = (*this * q); }
	quat operator/(float d) const { return quat{x / d, y / d, z / d, w / d}; }
	float& operator[](uint32 i) { return e[i]; }
	const float& operator[](uint32 i) const { return e[i]; }
};

struct transform {
	vec3 scale;
	quat rotate;
	vec3 translate;

	bool operator==(transform t) const { return scale == t.scale && rotate == t.rotate && translate == t.translate; }
	bool operator!=(transform t) const { return !(*this == t); }
};

struct camera {
	vec3 position;
	vec3 view;
	float aspect;
	float fovy;
	float znear;
	float zfar;
};

struct plane {
	vec3 normal;
	float distance;
};

struct triangle {
	vec3 points[3];
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

struct aabb {
	vec3 min;
	vec3 max;
};

struct ray {
	vec3 origin;
	vec3 dir;
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
	m_debug_assert(t >= 0 && t <= 1);
	return v1 + (v2 - v1) * t;
}

float vec4_len(vec4 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec4 vec4_normalize(vec4 v) {
	float len = vec4_len(v);
	m_debug_assert(len > 0);
	return v / len;
}

mat3 mat3_transpose(mat3 m) {
	mat3 mat;
	for (int i = 0; i < 3; i += 1) {
		for (int j = 0; j < 3; j += 1) {
			mat.columns[i].e[j] = m.columns[j].e[i];
		}
	}
	return mat;
}

mat3 mat3_inverse(mat3 m) {
	float one_over_determinant = 1.0f / (+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));
	mat3 inverse;
	inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * one_over_determinant;
	inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * one_over_determinant;
	inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * one_over_determinant;
	inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * one_over_determinant;
	inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * one_over_determinant;
	inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * one_over_determinant;
	inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * one_over_determinant;
	inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * one_over_determinant;
	inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * one_over_determinant;
	return inverse;
}

vec3 mat3_get_scale(mat3 m) {
	vec3 scaling = {vec3_len(m.columns[0]), vec3_len(m.columns[1]), vec3_len(m.columns[2])};
	return scaling;
}

quat mat3_get_rotate(mat3 m) {
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
	default: { m_debug_assert(false); return quat{0, 0, 0, 1}; }
	}
}

mat3 mat3_from_scale(vec3 scale) {
	mat3 mat = {};
	mat.c1.x = scale.x;
	mat.c2.y = scale.y;
	mat.c3.z = scale.z;
	return mat;
}

mat3 mat3_from_scale(float scale) {
	return mat3_from_scale(vec3{scale, scale, scale});
}

mat3 mat3_from_axis_rotate(vec3 axis, float angle) {
	m_debug_assert(fabsf(vec3_len(axis) - 1.0f) < 0.000001f);
	float c = cosf(angle);
	float c_1 = 1 - c;
	float s = sinf(angle);
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

mat3 mat3_from_rotate(quat q) {
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

mat3 mat3_from_mat4(mat4 m) {
	mat3 mat = {vec3{m.c1.x, m.c1.y, m.c1.z}, vec3{m.c2.x, m.c2.y, m.c2.z}, vec3{m.c3.x, m.c3.y, m.c3.z}};
	return mat;
}

mat4 mat4_identity() {
	return mat4{vec4{1, 0, 0, 0}, vec4{0, 1, 0, 0}, vec4{0, 0, 1, 0}, vec4{0, 0, 0, 1}};
}

mat4 mat4_transpose(mat4 m) {
	mat4 mat;
	for (int i = 0; i < 4; i += 1) {
		for (int j = 0; j < 4; j += 1) {
			mat.columns[i].e[j] = m.columns[j].e[i];
		}
	}
	return mat;
}

mat4 mat4_inverse(mat4 m) {
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

mat4 mat4_from_scale(vec3 scale) {
	mat4 mat = {};
	mat.c1.x = scale.x; 
	mat.c2.y = scale.y; 
	mat.c3.z = scale.z; 
	mat.c4.w = 1;
	return mat;
}

mat4 mat4_from_scale(float scale) {
	return mat4_from_scale(vec3{scale, scale, scale});
}

mat4 mat4_from_translate(vec3 translate) {
	mat4 mat = mat4_identity();
	mat.c4.x = translate.x; 
	mat.c4.y = translate.y; 
	mat.c4.z = translate.z; 
	return mat;
}

mat4 mat4_from_axis_rotate(vec3 axis, float angle) {
	m_debug_assert(fabsf(vec3_len(axis) - 1) < (FLT_EPSILON * 2));
	float c = cosf(angle);
	float c_1 = 1 - c;
	float s = sinf(angle);
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

mat4 mat4_from_rotate(quat q) {
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

mat4 mat4_from_transform(transform t) {
	return mat4_from_translate(t.translate) * mat4_from_rotate(t.rotate) * mat4_from_scale(t.scale);
}

vec3 mat4_get_scale(mat4 m) {
	vec3 scaling = {
		vec3_len(vec3{m.columns[0].x, m.columns[0].y, m.columns[0].z}),
		vec3_len(vec3{m.columns[1].x, m.columns[1].y, m.columns[1].z}),
		vec3_len(vec3{m.columns[2].x, m.columns[2].y, m.columns[2].z})
	};
	return scaling;
}

quat mat4_get_rotate(mat4 m) {
	vec3 scaling = mat4_get_scale(m);
	mat3 m3 = mat3_from_mat4(m);
	m3[0] /= scaling.x;
	m3[1] /= scaling.y;
	m3[2] /= scaling.z;
	quat q = mat3_get_rotate(m3);
	float q_len = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	m_debug_assert(q_len > 0);
	return q / q_len;
}

vec3 mat4_get_translate(mat4 m) {
	vec3 translation = {m.columns[3].x, m.columns[3].y, m.columns[3].z};
	return translation;
}

transform mat4_get_transform(mat4 m) {
	transform t = {};
	t.scale = mat4_get_scale(m);
	t.rotate = mat4_get_rotate(m);
	t.translate = mat4_get_translate(m);
	return t;
}

mat4 mat4_look_at(vec3 eye, vec3 at) {
	vec3 up = { 0, 1, 0 };
	vec3 f = vec3_normalize(at - eye);
	if (f == up || f == -up) {
		f.x -= 0.000001f;
		f = vec3_normalize(f);
	}
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

mat4 mat4_project(float fovy, float aspect, float znear, float zfar) {
	float f = 1 / tanf(fovy / 2);
	mat4 mat = {};
	mat.c1.x = f / aspect;
	mat.c2.y = f;
	mat.c3.z = zfar / (znear - zfar);
	mat.c3.w = -1;
	mat.c4.z = znear * zfar / (znear - zfar);
	return mat;
}

mat4 mat4_project_reverse_z(float fovy, float aspect, float znear) {
	float f = 1 / tanf(fovy / 2);
	mat4 mat = {};
	mat.c1.x = f / aspect;
	mat.c2.y = f;
	mat.c3.w = -1;
	mat.c4.z = znear;
	return mat;
}

mat4 mat4_project_ortho(float left, float right, float bottom, float top, float near, float far) {
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

vec3 mat4_unproject(vec3 window_coord, mat4 view_mat, mat4 proj_mat, vec4 view_port) {
	mat4 inverse_mat = mat4_inverse(proj_mat * view_mat);
	vec4 tmp = vec4{window_coord.x, window_coord.y, window_coord.z, 1};
	tmp.x = (tmp.x - view_port.x) / view_port.z;
	tmp.y = (tmp.y - view_port.y) / view_port.w;
	tmp = tmp * 2 - 1;
	vec4 obj = inverse_mat * tmp;
	obj = obj / obj.w;
	return vec3{obj.x, obj.y, obj.z};
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

float quat_get_pitch(quat q) {
	float y = 2 * (q.y * q.z + q.w * q.x);
	float x = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
	if (y == 0 && x == 0) {
		return 2 * atan2f(q.x, q.w);
	}
	else {
		return atan2f(y, x);
	}
}

float quat_get_yaw(quat q) {
	return asinf(clamp(2 * (q.w * q.y - q.x * q.z), -1.0f, 1.0f));
}

float quat_get_roll(quat q) {
	return atan2f(2 * (q.w * q.z + q.x * q.y), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
}

vec3 quat_get_euler_angles(quat q) {
	return vec3{quat_get_pitch(q), quat_get_yaw(q), quat_get_roll(q)};
}

quat quat_from_euler_angles(vec3 angles) {
	angles *= 0.5f;
	vec3 c = vec3{cosf(angles.x), cosf(angles.y), cosf(angles.z)};
	vec3 s = vec3{sinf(angles.x), sinf(angles.y), sinf(angles.z)};
	quat q;
	q.w = c.x * c.y * c.z + s.x * s.y * s.z;
	q.x = s.x * c.y * c.z - c.x * s.y * s.z;
	q.y = c.x * s.y * c.z + s.x * c.y * s.z;
	q.z = c.x * c.y * s.z - s.x * s.y * c.z;
	return q;
}

quat quat_from_axis_rotate(vec3 axis, float angle) {
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
		return quat_from_axis_rotate(txa, (float)M_PI);
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

mat4 camera_project_mat4(camera camera) {
	return mat4_project(camera.fovy, camera.aspect, camera.znear, camera.zfar);
}

mat4 camera_view_mat4(camera camera) {
	return mat4_look_at(camera.position, camera.position + camera.view);
}

mat4 camera_view_project_mat4(camera camera) {
	return camera_project_mat4(camera) * camera_view_mat4(camera);
}

mat4 camera_billboard_mat4(camera camera) {
	mat3 mat = mat3_transpose(mat3_from_mat4(camera_view_mat4(camera)));
	return mat4{
		vec4{mat.columns[0].x, mat.columns[0].y, mat.columns[0].z, 0},
		vec4{mat.columns[1].x, mat.columns[1].y, mat.columns[1].z, 0},
		vec4{mat.columns[2].x, mat.columns[2].y, mat.columns[2].z, 0},
		vec4{0, 0, 0, 1}
	};
}

mat4 camera_shadow_map_project_mat4(camera camera, vec3 directional_light) {
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

	// return mat4_orthographic_project(bound_min.x, bound_max.x, bound_min.y, bound_max.y, -bound_max.z, -bound_min.z) * light_view_mat4;

	mat4 light_view_mat4 = mat4_look_at(directional_light * 50, {0, 0, 0});
	return mat4_project_ortho(-50, 50, -50, 50, 0, 100) * light_view_mat4;
}

float degree_to_radian(float degree) {
	return degree * (float)M_PI / 180;
}

float radian_to_degree(float radian) {
	return radian * 180 / (float)M_PI;
}

float wrap_angle(float angle) {
	angle = fmodf(angle, (float)M_PI * 2);
	if (angle < 0) {
		angle += (float)M_PI * 2;
	}
	return angle;
}

float hfov_to_vfov(float hfov, float aspect_ratio) {
	float vfov = 2.0f * atanf(tanf(hfov * 0.5f) / aspect_ratio);
	return vfov;
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
	float width = rect_inner_width * scale;
	float height = rect_inner_height * scale;
	float x = (rect_outer_width - width) / 2;
	float y = (rect_outer_height - height) / 2;
	return {x, y, width, height};
}

void uniform_sample_hemisphere(float u1, float u2, vec3 *xyz, float *pdf) {
	float r = sqrtf(1 - u1 * u1);
	float phi = 2 * (float)M_PI * u2;
	*xyz = vec3{cosf(phi) * r, u1, -sinf(phi) * r};
	*pdf = 1.0f / (2 * (float)M_PI);
}

void cosine_weighted_sample_hemisphere(float u1, float u2, vec3 *xyz, float *pdf) {
	float r = sqrtf(u1);
	float theta = 2 * (float)M_PI * u2;
	*xyz = vec3{r * cosf(theta), sqrtf(max(0.0f, 1 - u1)), -r * sinf(theta)};
	*pdf = cosf(theta) / (float)M_PI;
}

float aabb_volume(aabb bound) {
	return fabsf(bound.min.x - bound.max.x) * fabsf(bound.min.y - bound.max.y) * fabsf(bound.min.z - bound.max.z);
}

vec3 aabb_size(aabb bound) {
	return vec3{bound.max.x - bound.min.x, bound.max.y - bound.min.y, bound.max.z - bound.min.z};
}

vec3 aabb_center(aabb bound) {
	return (bound.min + bound.max) * 0.5f;
}

vec3 aabb_bottom_center(aabb bound) {
	return vec3{bound.min.x + (bound.max.x - bound.min.x) / 2, bound.min.y, bound.min.z + (bound.max.z - bound.min.z) / 2};
}

aabb aabb_translate(aabb bound, vec3 v) {
	return aabb{bound.min + v, bound.max + v};
}

aabb aabb_scale(aabb bound, vec3 v) {
	return aabb{bound.min * v, bound.max * v};
}

aabb aabb_rotate(aabb bound, quat quat) {
	mat4 mat = mat4_from_rotate(quat);
	aabb new_bound = {};
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

bool aabb_intersect(aabb bound1, aabb bound2) {
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

aabb aabb_expand(aabb bound1, aabb bound2) {
	aabb bound = bound1;
	if (bound2.min.x < bound1.min.x || bound2.min.y < bound1.min.y || bound2.min.z < bound1.min.z) {
		bound.min = bound2.min;
	}
	if (bound2.max.x > bound1.max.x || bound2.max.y > bound1.max.y || bound2.max.z > bound1.max.z) {
		bound.max = bound2.max;
	}
	return bound;
}

bool ray_hit_plane(ray ray, plane plane, float *hit) {
	float t = (plane.distance - vec3_dot(plane.normal, ray.origin)) / vec3_dot(plane.normal, ray.dir * ray.len);
	if (t < 0 && t > 1) {
		return false;
	}
	else {
		*hit = t * ray.len;
		return true;
	}
}

bool ray_hit_sphere(ray ray, sphere sphere, float *hit) {
	float t0, t1;
	vec3 l = ray.origin - sphere.center; 
	float a = ray.len * ray.len;
	float b = 2 * vec3_dot(ray.dir * ray.len, l);
	float c = vec3_dot(l, l) - sphere.radius * sphere.radius;
	float discr = b * b - 4 * a * c; 
	if (discr < 0) {
		return false;
	}
	else if (discr == 0) {
		t0 = t1 = -0.5f * b / a;
	}
	else {
		float q = (b > 0) ? -0.5f * (b + sqrtf(discr)) : -0.5f * (b - sqrtf(discr)); 
		t0 = q / a; 
		t1 = c / q; 
	} 
	if (t0 > t1) {
		std::swap(t0, t1);
	}
	if (t0 < 0) { 
		t0 = t1;
		if (t0 < 0 || t0 > 1) {
			return false;
		}
	}
	else if (t0 > 1) {
		return false;
	}
	*hit = t0 * ray.len;
	return true; 
}

bool ray_hit_aabb(ray ray, aabb bound, vec3 *hit_a = nullptr, vec3 *hit_b = nullptr) {
	float t0 = 0;
	float t1 = ray.len;
	for (uint32 i = 0; i < 3; i += 1) {
		float fnear = (bound.min[i] - ray.origin[i]) / ray.dir[i];
		float ffar = (bound.max[i] - ray.origin[i]) / ray.dir[i];
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
	if (hit_a) {
		*hit_a = ray.origin + ray.dir * t0;
	}
	if (hit_b) {
		*hit_b = ray.origin + ray.dir * t1;
	}
	return true;
}

bool ray_hit_triangle(ray ray, vec3 a, vec3 b, vec3 c, float *hit = nullptr, vec3 *hit_point = nullptr, vec3 *barycentric_coord = nullptr) {
	vec3 ab = b - a;
	vec3 ac = c - a;
	vec3 qp = -ray.dir * ray.len;
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
	if (hit) {
		*hit = t * ood;
	}
	if (hit_point) {
		*hit_point = ray.origin + ray.dir * ray.len * t * ood;
	}
	if (barycentric_coord) {
		barycentric_coord->y = v * ood;
		barycentric_coord->z = w * ood;
		barycentric_coord->x = 1 - barycentric_coord->y - barycentric_coord->z;
	}
	return true;
}

#include <directxmath.h>
using namespace DirectX;

vec3 vec3_from_xmvector(XMVECTOR v) {
	XMFLOAT3 dst;
	XMStoreFloat3(&dst, v);
	return vec3{ dst.x, dst.y, dst.z };
}

mat4 mat4_from_xmmatrix(XMMATRIX xm) {
	XMFLOAT4 r0;
	XMFLOAT4 r1;
	XMFLOAT4 r2;
	XMFLOAT4 r3;
	XMStoreFloat4(&r0, xm.r[0]);
	XMStoreFloat4(&r1, xm.r[1]);
	XMStoreFloat4(&r2, xm.r[2]);
	XMStoreFloat4(&r3, xm.r[3]);
	mat4 m;
	m.c1 = { r0.x, r1.x, r2.x, r3.x };
	m.c2 = { r0.y, r1.y, r2.y, r3.y };
	m.c3 = { r0.z, r1.z, r2.z, r3.z };
	m.c4 = { r0.w, r1.w, r2.w, r3.w };
	return m;
}

#endif // __MATH_CPP__
