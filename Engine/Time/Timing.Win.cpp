#include "Timing.h"
#include <Windows.h>
#include <assert.h>
#include <limits.h>

namespace Engine {
	namespace Timing {
		unsigned int lastFrame_ms;

		LARGE_INTEGER PerformanceFrequency = { 0 };

		void Init() {
			BOOL bSuccess = QueryPerformanceFrequency(&PerformanceFrequency);

			assert(bSuccess == TRUE);
		}

		tick_t GetCurrentTickCounter() {
			LARGE_INTEGER CurrentFrameCounter;
			BOOL bSuccess = QueryPerformanceCounter(&CurrentFrameCounter);
			assert(bSuccess == TRUE);

			return CurrentFrameCounter.QuadPart;
		}

		float GetTimeDiff_ms(tick_t i_startTick, tick_t i_endTick) {
			assert(PerformanceFrequency.QuadPart != 0);
			return (1000.0f * static_cast<float>(i_endTick - i_startTick)) / PerformanceFrequency.QuadPart;
		}
	}
}
