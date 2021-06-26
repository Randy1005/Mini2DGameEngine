#include "Core/StartupShutdown.h"


namespace Engine {

	void waste_time() {
		PERFORMANCE_MARKER
		for (int i = 0; i < 10000; i++) {

		}
	}

	void waste_time2() {
		PERFORMANCE_MARKER
		for (int i = 0; i < 20000; i++) {

		}
	}	
	
	Vector2 GetRandomPosition() {
		float rndX = static_cast<float>(rand() % 800 - 400);
		float rndY = static_cast<float>(rand() % 200 + 200);
		return Vector2(rndX, rndY);
	}



	void Run() {
		srand(static_cast<unsigned int>(time(nullptr)));

		bool bDone = false;

		Timing::Init();
		InputSystem::GetInstance()->Initialize();

		// initialize job system
		JobSystem::Initialize();

		StrongPtr<GameObject> goMonster = JSONUtilities::CreateGameObjectFromJSONFile("ActorsJSON/Monster.json");

		// register gameObject function pointer to InputSystem
		goMonster->inputEnabled = true;
		if (goMonster->inputEnabled) {
			// bind gameObject function delegates to keyChangeCallBack
			// TEST binding (really crappy, but in the future we might wanna implement an entire scripting system);
			InputSystem::GetInstance()->RegisterUserFunction(std::bind(&GameObject::OnKeyInput, goMonster, std::placeholders::_1, std::placeholders::_2));
		}

		StrongPtr<GameObject> goHuman = JSONUtilities::CreateGameObjectFromJSONFile("ActorsJSON/Human.json");


		// import physicsItem into collision system
		CollidingSystem::ImportPhysicsItems();

		do {
			GLib::Service(bDone);

			float dt = GetFrameTime_s();

			Physics::RunAll(dt);

			CollidingSystem::ResolveCollisions(dt);

			Renderer::DrawAll();

		} while (!bDone);


		// GLib renderables cleanUp
		Renderer::CleanUp();

		// Physics Items cleanUp
		Physics::CleanUp();

		// input system cleanUp
		InputSystem::GetInstance()->ResetInstance();

		// Collideables cleanup
		CollidingSystem::CleanUp();

		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();

		// terminate job system, wake all blocked threads
		JobSystem::Terminate();

		waste_time();
		waste_time2();

		Profiler::PerformanceProfiler::GetInstance().Log();
	}

	float GetFrameTime_s() {
		PERFORMANCE_MARKER

		static Timing::tick_t lastFrameStartTick = 0;
		float frameTime;

		Timing::tick_t currentFrameStartTick = Timing::GetCurrentTickCounter();

		frameTime = (lastFrameStartTick == 0) ? (1.0f / 60.0f) : (Timing::GetTimeDiff_ms(lastFrameStartTick, currentFrameStartTick) / 1000.0f);
		lastFrameStartTick = currentFrameStartTick;

		return IsDebuggerPresent() ? (1.0f / 60.0f) : frameTime;

	}
}
