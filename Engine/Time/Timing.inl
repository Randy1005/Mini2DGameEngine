#pragma once
namespace Engine {
	namespace Timing {
		inline float ConvertSecToMilliseconds(float i_seconds) {
			return i_seconds * 1000.0f;
		}

		inline float ConvertPerSecToPerMilliseconds(float i_seconds) {
			return i_seconds / 1000.0f;
		}
	}
}
