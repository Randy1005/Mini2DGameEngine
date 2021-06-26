#pragma once
#include <stdint.h>
#include <math.h>

namespace Math {
	static const uint32_t allBitsSet = ~0;
	const float NaN = *reinterpret_cast<const float*>(&allBitsSet);

	inline bool isNaN(float i_val) {
		volatile float val = i_val;
		return val != val;
	}

	inline bool areEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff)
	{
		return fabs(double(i_lhs) - i_rhs) < i_maxDiff;
	}


	inline bool isZero(float i_val)
	{
		return areEqual_Eps(i_val, 0.0f, .000000001f);
	}
}
