#pragma once
#include <vector>
#include "GameObject/GameObject.h"
#include "Components/Renderable.h"
#include "Components/Physics.h"
#include "Components/CollidingSystem.h"
#include "Time/Timing.h"
#include "Core/Target.h"
#include "Core/JSONUtilities.h"
#include "Console/ConsolePrint.h"
#include "JobSystem/JobSystem.h"
#include "PerformanceProfiling/PerformanceProfiler.h"



namespace Engine {
	void Run();
	float GetFrameTime_s();
}
