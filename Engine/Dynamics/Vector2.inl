#include <assert.h>
#include "Math/MathUtils.h"
#include "Vector2.h"


inline Vector2::Vector2(float i_x, float i_y) :
	_x(i_x),
	_y(i_y)
{
	assert(!Math::isNaN(i_x));
	assert(!Math::isNaN(i_y));
}

inline Vector2::Vector2(const Vector2& i_rhs) :
	_x(i_rhs._x),
	_y(i_rhs._y)
{
	assert(!Math::isNaN(i_rhs._x));
	assert(!Math::isNaN(i_rhs._y));
}

// operator overloading
inline Vector2& Vector2::operator=(const Vector2& i_rhs) {
	assert(!Math::isNaN(i_rhs._x));
	assert(!Math::isNaN(i_rhs._y));

	_x = i_rhs._x;
	_y = i_rhs._y;

	return *this;
}

inline float Vector2::X() const {
	return _x;
}

inline float Vector2::Y() const {
	return _y;
}

inline void Vector2::X(float i_x) {
	assert(!Math::isNaN(i_x));
	_x = i_x;
}

inline void Vector2::Y(float i_y) {
	assert(!Math::isNaN(i_y));
	_y = i_y;
}

inline Vector2& Vector2::operator+=(const Vector2& i_rhs) {
	_x += i_rhs._x;
	_y += i_rhs._y;
	return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& i_rhs) {
	_x -= i_rhs._x;
	_y -= i_rhs._y;
	return *this;
}

inline Vector2& Vector2::operator*=(float i_rhs) {
	assert(!Math::isNaN(i_rhs));
	_x *= i_rhs;
	_y *= i_rhs;
	return *this;
}

inline Vector2& Vector2::operator/=(float i_rhs) {
	assert(!Math::isNaN(i_rhs));
	assert(!Math::isZero(i_rhs));
	_x /= i_rhs;
	_y /= i_rhs;
	return *this;
}

inline Vector2 Vector2::operator-() {
	return Vector2(-_x, -_y);
}


inline Vector2 operator+(const Vector2& i_lhs, const Vector2& i_rhs) {
	return Vector2(i_lhs.X() + i_rhs.X(), i_lhs.Y() + i_rhs.Y());
}

inline Vector2 operator-(const Vector2& i_lhs, const Vector2& i_rhs) {
	return Vector2(i_lhs.X() - i_rhs.X(), i_lhs.Y() - i_rhs.Y());
}

inline Vector2 operator*(const Vector2& i_lhs, const Vector2& i_rhs) {
	return Vector2(i_lhs.X() * i_rhs.X(), i_lhs.Y() * i_rhs.Y());
}

inline Vector2 operator*(const Vector2& i_lhs, float i_rhs) {
	assert(!Math::isNaN(i_rhs));
	return Vector2(i_lhs.X() * i_rhs, i_lhs.Y() * i_rhs);
}

inline Vector2 operator*(float i_lhs, const Vector2& i_rhs) {
	assert(!Math::isNaN(i_lhs));
	return Vector2(i_rhs.X() * i_lhs, i_rhs.Y() * i_lhs);
}

inline Vector2 operator/(const Vector2& i_lhs, float i_rhs) {
	assert(!Math::isZero(i_rhs));
	assert(!Math::isNaN(i_rhs));

	return Vector2(i_lhs.X() / i_rhs, i_lhs.Y() / i_rhs);
}


inline bool operator==(const Vector2& i_lhs, const Vector2& i_rhs) {
	return Math::areEqual_Eps(i_lhs.X(), i_rhs.X(), .00000000001f) && Math::areEqual_Eps(i_lhs.Y(), i_rhs.Y(), .00000000001f);
}

inline bool operator!=(const Vector2& i_lhs, const Vector2& i_rhs) {
	return !Math::areEqual_Eps(i_lhs.X(), i_rhs.X(), .00000000001f) || !Math::areEqual_Eps(i_lhs.Y(), i_rhs.Y(), .00000000001f);
}


