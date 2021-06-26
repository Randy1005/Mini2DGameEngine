#pragma once
#include <array>
#include "Vector4.h"
#include "Console/ConsolePrint.h"
#include <math.h>

class Matrix4x4 {
public:
	// default constructor
	Matrix4x4() {}

	// constructs matrix from separate values
	Matrix4x4(float i_00, float i_01, float i_02, float i_03, 
			float i_10, float i_11, float i_12, float i_13,
			float i_20, float i_21, float i_22, float i_23,
			float i_30, float i_31, float i_32, float i_33);

	// construct matrix from Vector4s
	Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);


	void CreateIdendity();
	
	// get / set entries
	void SetEntry(const size_t i_row, const size_t i_col, float value);
	float operator()(const size_t i_row, const size_t i_col);

	Vector4 GetColumn(size_t i_col);
	Vector4 GetRow(size_t i_row);

	// multiplication: 2 matrices
	static Matrix4x4 Multiply(Matrix4x4& mat_lhs, Matrix4x4& mat_rhs);

	// multiplication: matrix * vector (treated as column vector)
	static Vector4 Multiply(Matrix4x4& mat_lhs, Vector4& vec_rhs);

	// transpose
	void Transpose();

	// invert
	bool Invert();

	// create scaling matrix
	static Matrix4x4 CreateScaling(float x_scale, float y_scale);

	// create rotation matrix
	static Matrix4x4 CreateRotation(float radian);

	// create translation matrix
	static Matrix4x4 CreateTranslation(float x_trans, float y_trans);

	// create TRS matrix
	static Matrix4x4 CreateTRS(Matrix4x4& transMat, Matrix4x4& rotMat, Matrix4x4& scaleMat);


	// assign operator
	void operator=(Matrix4x4& otherMat);


	// equal / not equal
	bool operator==(Matrix4x4& otherMat);
	bool operator!=(Matrix4x4& otherMat);


	void PrintMatrix();
private:
	std::array<Vector4, DIMENSION> matRows;
};






