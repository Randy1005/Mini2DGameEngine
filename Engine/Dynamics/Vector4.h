#pragma once
#include <assert.h>
#include "Vector2.h"

const size_t DIMENSION = 4;

class Vector4 {
public:
	Vector4() :
		_x(0.0f),
		_y(0.0f),
		_z(0.0f),
		_w(0.0f)
	{

	};

	inline Vector4(float i_x, float i_y, float i_z, float i_w);
	inline Vector4(const Vector4 & i_other);
	
	// if we were to construct vector4 from vector2
	inline Vector4(const Vector2& i_vec2, float i_z, float i_w);

	// getter / setter
	float X() const;
	float Y() const;
	float Z() const;
	float W() const;
	void X(float i_x);
	void Y(float i_y);
	void Z(float i_z);
	void W(float i_w);
	float At(size_t i_index);
	void SetValueAt(size_t index, float value);

	// negate
	Vector4 operator-();

	Vector4& operator=(const Vector4& i_other);
	Vector4& operator+=(const Vector4& i_other);
	Vector4& operator-=(const Vector4& i_other);
	Vector4& operator*=(const float i_other);
	Vector4& operator/=(const float i_other);


	static float dot(const Vector4& i_lhs, const Vector4& i_rhs);

	static const Vector4 Zero;

	Vector4 normalized();



private:
	float _x, _y, _z, _w;
};

inline Vector4 operator+(const Vector4& i_lhs, const Vector4& i_rhs);
inline Vector4 operator-(const Vector4& i_lhs, const Vector4& i_rhs);
inline Vector4 operator*(const Vector4& i_lhs, const Vector4& i_rhs);
inline Vector4 operator*(float i_lhs, const Vector4& i_rhs);
inline Vector4 operator*(const Vector4& i_lhs, float i_rhs);
inline Vector4 operator/(const Vector4& i_lhs, float i_rhs);

inline bool operator==(const Vector4& i_lhs, const Vector4& i_rhs);
inline bool operator!=(const Vector4& i_lhs, const Vector4& i_rhs);


#include "Vector4.inl"