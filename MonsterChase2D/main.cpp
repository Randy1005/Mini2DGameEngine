#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include <inttypes.h>
#include "Core/StartupShutdown.h"

// #define Vec4Mat4UnitTestEnabled
#if defined Vec4Mat4UnitTestEnabled
#include "Dynamics/Matrix4x4.h"
#endif

#if defined Vec4Mat4UnitTestEnabled
void Vec4Mat4UnitTest() {
	// Vector4 equal / not equal
	Vector4 eq0(1, 2, 3, 4);
	Vector4 eq1(1, 2, 3, 4);
	assert(eq0 == eq1);

	Vector4 neq0(1, 2, 3, 5);
	assert(eq0 != neq0);

	// Vector4 assign
	Vector4 A(1, 2, 3, 4);
	Vector4 B = A;
	assert(A == B);

	// Vector4 copy
	Vector4 C(3, 4, 5, 6);
	Vector4 C1(C);
	assert(C == C1);

	// Vector4 construct from vector2
	Vector4 C2(Vector2(1, 9), 2, 3);
	assert(C2 == Vector4(1, 9, 2, 3));

	// Vector4 add
	Vector4 D(2, 3, 4, 5);
	Vector4 E(4, 5, 6, 7);
	Vector4 F = D + E;
	assert(F == Vector4(6, 8, 10, 12));

	// Vector4 subtract
	Vector4 G(3, 1, 8, 5);
	Vector4 H(4, 5, 6, 7);
	Vector4 I = G - H;
	assert(I == Vector4(-1, -4, 2, -2));

	// Vector4 multiply (vector * vector)
	Vector4 J = G * H;
	assert(J == Vector4(12, 5, 48, 35));

	// Vector4 multiply (scalar * vector)
	Vector4 K = 3 * A;
	assert(K == Vector4(3, 6, 9, 12));

	// Vector4 multiply (vector * scalar)
	Vector4 L = A * 4;
	assert(L == Vector4(4, 8, 12, 16));

	// Vector4 add assign
	A += B;
	assert(A == Vector4(2, 4, 6, 8));

	// Vector4 sub assign
	A -= B;
	assert(A == Vector4(1, 2, 3, 4));

	// Vector4 mult assign
	A *= 4;
	assert(A == Vector4(4, 8, 12, 16));

	// Vector4 div assign
	A /= 2;
	assert(A == Vector4(2, 4, 6, 8));

	// Vector4 Set Value At index
	A.SetValueAt(3, 1.2f);
	assert(A == Vector4(2, 4, 6, 1.2f));

	A.SetValueAt(1, 1.2f);
	assert(A == Vector4(2, 1.2f, 6, 1.2f));


	// matrix equal / create identity matrix
	Matrix4x4 mat0;
	mat0.CreateIdendity();
	
	Matrix4x4 matIdentity(
		Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	
	assert(mat0 == matIdentity);


	// matrix * matrix multiplication
	Matrix4x4 mat1(
		Vector4(5.0f, 7.0f, 9.0f, 10.0f),
		Vector4(2.0f, 3.0f, 3.0f, 8.0f),
		Vector4(8.0f, 10.0f, 2.0f, 3.0f),
		Vector4(3.0f, 3.0f, 4.0f, 8.0f));

	Matrix4x4 mat2(
		Vector4(3.0f, 10.0f, 12.0f, 18.0f),
		Vector4(12.0f, 1.0f, 4.0f, 9.0f),
		Vector4(9.0f, 10.0f, 12.0f, 2.0f),
		Vector4(3.0f, 12.0f, 4.0f, 10.0f));

	Matrix4x4 mat3 = Matrix4x4::Multiply(mat1, mat2);

	Matrix4x4 mat4(
		Vector4(210.0f, 267.0f, 236.0f, 271.0f),
		Vector4(93.0f, 149.0f, 104.0f, 149.0f),
		Vector4(171.0f, 146.0f, 172.0f, 268.0f),
		Vector4(105.0f, 169.0f, 128.0f, 169.0f));
	assert(mat3 == mat4);


	// matrix not equal
	assert(mat0 != mat1);

	// matrix assignment
	Matrix4x4 mat5 = mat2;
	assert(mat5 == mat2);

	Matrix4x4 mat6(mat2);
	assert(mat6 == mat2);

	// matrix * column vector multiplication
	Matrix4x4 mat7(
		Vector4(3.0f, 10.0f, 1.0f, 3.0f),
		Vector4(4.0f, 2.0f, 9.0f, 7.0f),
		Vector4(6.0f, 4.0f, 11.0f, 23.0f),
		Vector4(6.0f, 5.0f, 12.0f, 0.0f));

	Vector4 colVec(2.0f, 1.0f, 5.0f, 8.0f);

	Vector4 multResult = Matrix4x4::Multiply(mat7, colVec);
	assert(multResult == Vector4(45.0f, 111.0f, 255.0f, 77.0f));

	
	// transpose matrix
	mat7.Transpose();
	Matrix4x4 mat7Transposed(
		Vector4(3.0f, 4.0f, 6.0f, 6.0f),
		Vector4(10.0f, 2.0f, 4.0f, 5.0f),

		Vector4(1.0f, 9.0f, 11.0f, 12.0f),
		Vector4(3.0f, 7.0f, 23.0f, 0.0f));

	assert(mat7 == mat7Transposed);

	// invert matrix
	// mat1 * mat2 = mat3
	// --> mat1 = mat3 * INVERSE(mat2)
	if (mat2.Invert()) {
		DEBUG_PRINT("mat2 is invertible");
		Matrix4x4 mat3MultMat2Inverse = Matrix4x4::Multiply(mat3, mat2);
		// mat1.PrintMatrix();
		// mat3MultMat2Inverse.PrintMatrix();

		assert(mat1 == mat3MultMat2Inverse);
	}
	else {
		DEBUG_PRINT("mat2 is not invertible");
		assert(false);
	}

	// create scaling matrix (scale positive)
	Vector4 origVec(Vector2(2.0f, 1.2f), 0.0f, 1.0f);
	Matrix4x4 scalingMat0 = Matrix4x4::CreateScaling(2.0f, 1.5f);
	Vector4 scaledVec = Matrix4x4::Multiply(scalingMat0, origVec);

	assert(scaledVec == Vector4(4.0f, 1.8f, 0.0f, 1.0f));

	// create scaling matrix (scale negative)
	Matrix4x4 scalingMat1 = Matrix4x4::CreateScaling(-2.2f, -3.0f);
	Vector4 scaledVec1 = Matrix4x4::Multiply(scalingMat1, origVec);

	assert(scaledVec1 == Vector4(-4.4f, -3.6f, 0.0f, 1.0f));

	// create rotation matrix (rotate counter clockwise)
	const float PI = 3.14159f;
	Matrix4x4 rotMat0 = Matrix4x4::CreateRotation(PI / 2.0f);
	Vector4 rotatedVec0 = Matrix4x4::Multiply(rotMat0, origVec);
	assert(rotatedVec0 == Vector4(-1.2f, 2.0f, 0.0f, 1.0f));

	// create rotation matrix (rotate clockwise)
	Matrix4x4 rotMat1 = Matrix4x4::CreateRotation(-PI);
	Vector4 rotatedVec1 = Matrix4x4::Multiply(rotMat1, origVec);
	assert(rotatedVec1 == Vector4(-2.0f, -1.2f, 0.0f, 1.0f));

	// create rotatiom matrix (full 2*PI radian)
	Matrix4x4 rotMat2 = Matrix4x4::CreateRotation(2 * PI);
	Vector4 rotatedVec2 = Matrix4x4::Multiply(rotMat2, origVec);
	assert(rotatedVec2 == origVec);

	// create translation matrix
	Matrix4x4 transMat = Matrix4x4::CreateTranslation(-12.5f, 10.3f);
	Vector4 translatedVec = Matrix4x4::Multiply(transMat, origVec);
	assert(translatedVec == Vector4(-10.5f, 11.5f, 0.0f, 1.0f));

	// create TRS matrix (order: scale -> rotate -> translate)
	Matrix4x4 transMat1 = Matrix4x4::CreateTranslation(1.4f, -9.2f);
	Matrix4x4 rotMat3 = Matrix4x4::CreateRotation(0.5f * PI);
	Matrix4x4 scaleMat2 = Matrix4x4::CreateScaling(0.5f, 0.5f);
	Matrix4x4 TRSmat = Matrix4x4::CreateTRS(transMat1, rotMat3, scaleMat2);
	Vector4 transformedVec = Matrix4x4::Multiply(TRSmat, origVec);

	assert(transformedVec == Vector4(0.8f, -8.2f, 0.0f, 1.0f));	
}
#endif



struct timer
{
	std::string name;
	std::chrono::high_resolution_clock::time_point start;

	timer(const std::string& name) : name(name), start(std::chrono::high_resolution_clock::now()) {}
	~timer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		DEBUG_PRINT("%s : %" PRIu64 " millisecs\n", name, time);
	}
};



int WINAPI wWinMain(_In_ HINSTANCE i_hInstance, _In_opt_ HINSTANCE i_hPrevInstance, _In_ LPWSTR i_lpCmdLine, _In_ int i_nCmdShow) {

#if defined _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif // _DEBUG

#if defined Vec4Mat4UnitTestEnabled
	Vec4Mat4UnitTest();
#endif


	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "MyGameApp", -1, 1200, 800, true);
	

	if (bSuccess) {
		Engine::Run();
	}

	Profiler::PerformanceProfiler::GetInstance().ResetInstance();



}

