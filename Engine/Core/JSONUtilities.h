#pragma once
#include "nlohmann/json.hpp"
#include "JobSystem/JobSystem.h"
#include "Components/Physics.h"
#include "Components/Renderable.h"
#include "Components/CollidingSystem.h"
#include "Console/ConsolePrint.h"

namespace JSONUtilities {
	// utility methods for creating GameObjects from JSON files	
	StrongPtr<GameObject> CreateGameObjectFromJSONObject(nlohmann::json& i_JSONData);

	StrongPtr<GameObject> CreateGameObjectFromJSONFile(const std::string& i_JSONFileName);

	void AddPhysicsItemFromJSONData(WeakPtr<GameObject>& i_gameObject, nlohmann::json i_physicsJSONData);

	void AddRenderableFromJSONData(WeakPtr<GameObject>& i_gameObject, nlohmann::json i_renderJSONData);

	std::vector<uint8_t> LoadFile(const std::string i_pFileName);

}
