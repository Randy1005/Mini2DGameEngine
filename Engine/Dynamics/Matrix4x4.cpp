#include "Matrix4x4.h"


bool gluInvertMatrix(const float m[16], float invOut[16])
{
    float inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}





Matrix4x4::Matrix4x4(
	float i_00, float i_01, float i_02, float i_03,
	float i_10, float i_11, float i_12, float i_13,
	float i_20, float i_21, float i_22, float i_23,
	float i_30, float i_31, float i_32, float i_33) :
	matRows({ Vector4(i_00, i_01, i_02, i_03),
		Vector4(i_10, i_11, i_12, i_13),
		Vector4(i_20, i_21, i_22, i_23),
		Vector4(i_30, i_31, i_32, i_33) }) 
{

}

Matrix4x4::Matrix4x4(
	const Vector4& row0,
	const Vector4& row1,
	const Vector4& row2,
	const Vector4& row3) :
	matRows({ row0, row1, row2, row3 })
{

}


void Matrix4x4::CreateIdendity() {
    matRows[0] = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
    matRows[1] = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    matRows[2] = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
    matRows[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}


void Matrix4x4::SetEntry(const size_t i_row, const size_t i_col, float value) {
	assert(i_col < DIMENSION && i_col >= 0);
	assert(i_row < DIMENSION && i_row >= 0);
	
	if (i_col == 0)
		matRows[i_row].X(value);
	else if (i_col == 1)
		matRows[i_row].Y(value);
	else if (i_col == 2)
		matRows[i_row].Z(value);
	else
		matRows[i_row].W(value);
}

float Matrix4x4::operator()(const size_t i_row, const size_t i_col) {
	assert(i_col < DIMENSION&& i_col >= 0);
	assert(i_row < DIMENSION&& i_row >= 0);

	return matRows[i_row].At(i_col);

}


Vector4 Matrix4x4::GetColumn(size_t i_col) {
	assert(i_col < DIMENSION && i_col >= 0);
	return Vector4(matRows[0].At(i_col), matRows[1].At(i_col), matRows[2].At(i_col), matRows[3].At(i_col));
}

Vector4 Matrix4x4::GetRow(size_t i_row) {
	assert(i_row < DIMENSION && i_row >= 0);
	return matRows[i_row];
}

Matrix4x4 Matrix4x4::Multiply(Matrix4x4& mat_lhs, Matrix4x4& mat_rhs) {
	Matrix4x4 mat_out;
	mat_out.CreateIdendity();

	for (size_t i = 0; i < DIMENSION * DIMENSION; i++) {
		Vector4 lhsRowVec = mat_lhs.GetRow(i / DIMENSION);
		Vector4 rhsColVec = mat_rhs.GetColumn(i % DIMENSION);

		mat_out.SetEntry(i / DIMENSION, i % DIMENSION, Vector4::dot(lhsRowVec, rhsColVec));
	}

	return mat_out;
}

Vector4 Matrix4x4::Multiply(Matrix4x4& mat_lhs, Vector4& vec_rhs) {

	return Vector4(
		Vector4::dot(mat_lhs.GetRow(0), vec_rhs),
		Vector4::dot(mat_lhs.GetRow(1), vec_rhs),
		Vector4::dot(mat_lhs.GetRow(2), vec_rhs),
		Vector4::dot(mat_lhs.GetRow(3), vec_rhs));
}

void Matrix4x4::Transpose() {
	for (size_t i = 0; i < DIMENSION - 1; i++) {
		for (size_t j = i + 1; j < DIMENSION; j++) {
			float tmp = matRows[i].At(j);
			matRows[i].SetValueAt(j, matRows[j].At(i));
			matRows[j].SetValueAt(i, tmp);
		}
	}
}

Matrix4x4 Matrix4x4::CreateScaling(float x_scale, float y_scale) {
    Matrix4x4 scaleMat;
    scaleMat.CreateIdendity();
    scaleMat.SetEntry(0, 0, x_scale);
    scaleMat.SetEntry(1, 1, y_scale);

    return scaleMat;
}

Matrix4x4 Matrix4x4::CreateRotation(float radian) {
    Matrix4x4 rotationMat;
    rotationMat.CreateIdendity();
    rotationMat.SetEntry(0, 0, cos(radian));
    rotationMat.SetEntry(0, 1, -sin(radian));
    rotationMat.SetEntry(1, 0, sin(radian));
    rotationMat.SetEntry(1, 1, cos(radian));
    
    return rotationMat;
}

Matrix4x4 Matrix4x4::CreateTranslation(float x_trans, float y_trans) {
    Matrix4x4 transMat;
    transMat.CreateIdendity();
    transMat.SetEntry(0, 3, x_trans);
    transMat.SetEntry(1, 3, y_trans);

    return transMat;
}

Matrix4x4 Matrix4x4::CreateTRS(Matrix4x4& transMat, Matrix4x4& rotMat, Matrix4x4& scaleMat) {
    Matrix4x4 tmp = Matrix4x4::Multiply(rotMat, scaleMat);
    Matrix4x4 TRSmat = Matrix4x4::Multiply(transMat, tmp);

    return TRSmat;
}

void Matrix4x4::operator=(Matrix4x4& otherMat) {
	for (size_t i = 0; i < DIMENSION; i++) {
		matRows[i] = otherMat.GetRow(i);
	}
}


bool Matrix4x4::operator==(Matrix4x4& otherMat) {
	for (size_t i = 0; i < DIMENSION; i++) {
		if (matRows[i] != otherMat.GetRow(i)) return false;
	}

	return true;
}

bool Matrix4x4::operator!=(Matrix4x4& otherMat) {
	return !(*this == otherMat);
}

bool Matrix4x4::Invert() {
   
    const float input[16] = {
        matRows[0].X(), matRows[0].Y(), matRows[0].Z(), matRows[0].W(),
        matRows[1].X(), matRows[1].Y(), matRows[1].Z(), matRows[1].W(),
        matRows[2].X(), matRows[2].Y(), matRows[2].Z(), matRows[2].W(),
        matRows[3].X(), matRows[3].Y(), matRows[3].Z(), matRows[3].W()
    };

    float output[16] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };

    if (gluInvertMatrix(input, output)) {
        for (size_t i = 0; i < DIMENSION * DIMENSION; i++) {
            matRows[i / DIMENSION].SetValueAt(i % DIMENSION, output[i]);
        }

        return true;
    }
    else {
        return false;
    }
}


void Matrix4x4::PrintMatrix() {
	DEBUG_PRINT("\n");
	for (auto& row : matRows) {
		DEBUG_PRINT("[%.2f]\t[%.2f]\t[%.2f]\t[%.2f]", row.X(), row.Y(), row.Z(), row.W());
	}
	DEBUG_PRINT("\n");
}






