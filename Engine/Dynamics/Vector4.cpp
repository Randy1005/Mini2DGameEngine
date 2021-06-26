#include "Vector4.h"

const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);

float Vector4::dot(const Vector4& i_lhs, const Vector4& i_rhs) {
	return i_lhs.X() * i_rhs.X() + i_lhs.Y() * i_rhs.Y() + i_lhs.Z() * i_rhs.Z() + i_lhs.W() * i_rhs.W();
}

Vector4 Vector4::normalized() {
	float length = sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
	Vector4 Normalized(*this / length);

	return Normalized;
}