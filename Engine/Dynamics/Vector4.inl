#pragma once
#include <assert.h>
#include "Math/MathUtils.h"
#include "Vector4.h"


inline Vector4::Vector4(float i_x, float i_y, float i_z, float i_w) :
	_x(i_x),
	_y(i_y),
	_z(i_z),
	_w(i_w)
{
	assert(!Math::isNaN(i_x));
	assert(!Math::isNaN(i_y));
	assert(!Math::isNaN(i_z));
	assert(!Math::isNaN(i_w));
}

inline Vector4::Vector4(const Vector2& i_vec2, float i_z, float i_w) :
	_x(i_vec2.X()),
	_y(i_vec2.Y()),
	_z(i_z),
	_w(i_w)
{
	assert(!Math::isNaN(i_z));
	assert(!Math::isNaN(i_w));
}

inline Vector4::Vector4(const Vector4& i_other) :
	_x(i_other._x),
	_y(i_other._y),
	_z(i_other._z),
	_w(i_other._w)
{
	assert(!Math::isNaN(i_other._x));
	assert(!Math::isNaN(i_other._y));
	assert(!Math::isNaN(i_other._z));
	assert(!Math::isNaN(i_other._w));
}

inline Vector4& Vector4::operator=(const Vector4& i_other) {
	assert(!Math::isNaN(i_other._x));
	assert(!Math::isNaN(i_other._y));
	assert(!Math::isNaN(i_other._z));
	assert(!Math::isNaN(i_other._w));

	_x = i_other._x;
	_y = i_other._y;
	_z = i_other._z;
	_w = i_other._w;
	
	return *this;
}

inline float Vector4::X() const {
	return _x;
}

inline float Vector4::Y() const {
	return _y;
}

inline float Vector4::Z() const {
	return _z;
}

inline float Vector4::W() const {
	return _w;
}

inline void Vector4::X(float i_x) {
	assert(!Math::isNaN(i_x));
	_x = i_x;
}

inline void Vector4::Y(float i_y) {
	assert(!Math::isNaN(i_y));
	_y = i_y;
}

inline void Vector4::Z(float i_z) {
	assert(!Math::isNaN(i_z));
	_z = i_z;
}

inline void Vector4::W(float i_w) {
	assert(!Math::isNaN(i_w));
	_w = i_w;
}

inline float Vector4::At(size_t i_index) {
	assert(i_index < DIMENSION && i_index >= 0);
	return i_index == 0 ? _x : i_index == 1 ? _y : i_index == 2 ? _z : _w;
}

inline void Vector4::SetValueAt(size_t i_index, float value) {
	assert(i_index < DIMENSION && i_index >= 0);
	assert(!Math::isNaN(value));
	if (i_index == 0)
		X(value);
	else if (i_index == 1)
		Y(value);
	else if (i_index == 2)
		Z(value);
	else
		W(value);
}


inline Vector4& Vector4::operator+=(const Vector4& i_rhs) {
	assert(!Math::isNaN(i_rhs._x));
	assert(!Math::isNaN(i_rhs._y));
	assert(!Math::isNaN(i_rhs._z));
	assert(!Math::isNaN(i_rhs._w));
	
	_x += i_rhs._x;
	_y += i_rhs._y;
	_z += i_rhs._z;
	_w += i_rhs._w;

	return *this;
}

inline Vector4& Vector4::operator-=(const Vector4& i_rhs) {
	assert(!Math::isNaN(i_rhs._x));
	assert(!Math::isNaN(i_rhs._y));
	assert(!Math::isNaN(i_rhs._z));
	assert(!Math::isNaN(i_rhs._w));

	_x -= i_rhs._x;
	_y -= i_rhs._y;
	_z -= i_rhs._z;
	_w -= i_rhs._w;

	return *this;
}

inline Vector4& Vector4::operator*=(float i_rhs) {
	assert(!Math::isNaN(i_rhs));
	_x *= i_rhs;
	_y *= i_rhs;
	_z *= i_rhs;
	_w *= i_rhs;
	return *this;
}

inline Vector4& Vector4::operator/=(float i_rhs) {
	assert(!Math::isNaN(i_rhs));
	assert(!Math::isZero(i_rhs));
	_x /= i_rhs;
	_y /= i_rhs;
	_z /= i_rhs;
	_w /= i_rhs;
	return *this;
}

inline Vector4 Vector4::operator-() {
	return Vector4(-_x, -_y, -_z, -_w);
}

inline Vector4 operator+(const Vector4& i_lhs, const Vector4& i_rhs) {
	return Vector4(i_lhs.X() + i_rhs.X(), i_lhs.Y() + i_rhs.Y(), i_lhs.Z() + i_rhs.Z(), i_lhs.W() + i_rhs.W());
}

inline Vector4 operator-(const Vector4& i_lhs, const Vector4& i_rhs) {
	return Vector4(i_lhs.X() - i_rhs.X(), i_lhs.Y() - i_rhs.Y(), i_lhs.Z() - i_rhs.Z(), i_lhs.W() - i_rhs.W());
}

inline Vector4 operator*(const Vector4& i_lhs, const Vector4& i_rhs) {
	return Vector4(i_lhs.X() * i_rhs.X(), i_lhs.Y() * i_rhs.Y(), i_lhs.Z() * i_rhs.Z(), i_lhs.W() * i_rhs.W());
}

inline Vector4 operator*(const Vector4& i_lhs, float i_rhs) {
	assert(!Math::isNaN(i_rhs));
	return Vector4(i_lhs.X() * i_rhs, i_lhs.Y() * i_rhs, i_lhs.Z() * i_rhs, i_lhs.W() * i_rhs);
}

inline Vector4 operator*(float i_lhs, const Vector4& i_rhs) {
	assert(!Math::isNaN(i_lhs));
	return Vector4(i_rhs.X() * i_lhs, i_rhs.Y() * i_lhs, i_rhs.Z() * i_lhs, i_rhs.W() * i_lhs);
}

inline Vector4 operator/(const Vector4& i_lhs, float i_rhs) {
	assert(!Math::isNaN(i_rhs));
	assert(!Math::isZero(i_rhs));
	return Vector4(i_lhs.X() / i_rhs, i_lhs.Y() / i_rhs, i_lhs.Z() / i_rhs, i_lhs.W() / i_rhs);
}

inline bool operator==(const Vector4& i_lhs, const Vector4& i_rhs) {
	return Math::areEqual_Eps(i_lhs.X(), i_rhs.X(), .001f) && Math::areEqual_Eps(i_lhs.Y(), i_rhs.Y(), .001f) && Math::areEqual_Eps(i_lhs.Z(), i_rhs.Z(), .001f) && Math::areEqual_Eps(i_lhs.W(), i_rhs.W(), .001f);
}

inline bool operator!=(const Vector4& i_lhs, const Vector4& i_rhs) {
	return !Math::areEqual_Eps(i_lhs.X(), i_rhs.X(), .001f) || !Math::areEqual_Eps(i_lhs.Y(), i_rhs.Y(), .001f) || !Math::areEqual_Eps(i_lhs.Z(), i_rhs.Z(), .001f) || !Math::areEqual_Eps(i_lhs.W(), i_rhs.W(), .001f);
}

