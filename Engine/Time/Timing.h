#pragma once
#include <stdint.h>

namespace Engine {
	namespace Timing {
		typedef uint64_t tick_t;

		void Init();
		tick_t GetCurrentTickCounter();
		float GetTimeDiff_ms(tick_t i_startTick, tick_t i_endTick = GetCurrentTickCounter());

		inline float ConvertSecToMilliseconds(float i_seconds);
		inline float ConvertPerSecToPerMilliseconds(float i_seconds);

	}
}

#include "Timing.inl"
