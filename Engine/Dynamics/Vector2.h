#pragma once
#include <iostream>
#include "Math/MathUtils.h"
#include <assert.h>


class Vector2 {

public:
	Vector2() :
		_x(0.0f),
		_y(0.0f)
	{

	};

	// standard constructor
	inline Vector2(float x, float y);

	// copy constructor
	inline Vector2(const Vector2& i_rhs);

	// getter / setter
	float X() const;
	float Y() const;
	void X(float i_x);
	void Y(float i_y);


	// negate
	Vector2 operator-();


	Vector2& operator= (const Vector2& i_rhs);
	Vector2& operator+= (const Vector2& i_rhs);
	Vector2& operator-= (const Vector2& i_rhs);
	Vector2& operator*= (float scalar);
	Vector2& operator/= (float scalar);

	static float dot(const Vector2& i_lhs, const Vector2& i_rhs);

	float length();
	Vector2 normalized();

	static const Vector2 Zero;

private:
	float _x, _y;

};

inline Vector2 operator+(const Vector2& i_lhs, const Vector2& i_rhs);
inline Vector2 operator-(const Vector2& i_lhs, const Vector2& i_rhs);
inline Vector2 operator*(const Vector2& i_lhs, const Vector2& i_rhs);
inline Vector2 operator*(float i_lhs, const Vector2& i_rhs);
inline Vector2 operator*(const Vector2& i_lhs, float i_rhs);
inline Vector2 operator/(const Vector2& i_lhs, float i_rhs);

inline bool operator==(const Vector2& i_lhs, const Vector2& i_rhs);
inline bool operator!=(const Vector2& i_lhs, const Vector2& i_rhs);


#include "Vector2.inl"
