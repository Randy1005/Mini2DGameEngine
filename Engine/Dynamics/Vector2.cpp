#include "Vector2.h"

const Vector2 Vector2::Zero(0.0f, 0.0f);

float Vector2::dot(const Vector2& i_lhs, const Vector2& i_rhs) {
	return i_lhs.X() * i_rhs.X() + i_lhs.Y() * i_rhs.Y();
}

float Vector2::length() {
	return sqrt(_x * _x + _y * _y);
}

Vector2 Vector2::normalized() {
	float length = sqrt(_x * _x + _y * _y);
	Vector2 Normalized(*this / length);

	return Normalized;
}