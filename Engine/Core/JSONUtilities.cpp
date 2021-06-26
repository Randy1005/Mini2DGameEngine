#include "Core/JSONUtilities.h"


namespace JSONUtilities {
	std::mutex readWriteToSystemMutex;

	StrongPtr<GameObject> CreateGameObjectFromJSONObject(nlohmann::json& i_JSONData) {
		Vector2 initialPosition = Vector2::Zero;
		std::string gameObjectName = "unknown";

		// parse name
		if (i_JSONData.contains("name")) {
			nlohmann::json name = i_JSONData["name"];
			assert(name.is_string());
			gameObjectName = name;
		}

		// parse position
		if (i_JSONData.contains("initial_position")) {
			nlohmann::json position = i_JSONData["initial_position"];
			assert(position.is_array());
			assert(position.size() == 2);
			assert(position[0].is_number() && position[1].is_number());
			initialPosition = Vector2(position[0], position[1]);
		}


		// now we have name and position, we can create our gameObject now
		StrongPtr<GameObject> newGameObject = GameObject::CreateGameObject(initialPosition, gameObjectName);

		// add this game objects to other systems (rendering systems / physics system / etc.)
		if (i_JSONData.contains("components")) {
			assert(i_JSONData["components"].is_object());

			for (nlohmann::json::iterator it = i_JSONData["components"].begin(); it != i_JSONData["components"].end(); ++it) {
				if (it.key() == "movable") {
					nlohmann::json movableComponent = it.value();
					// create observer instance
					WeakPtr<GameObject> newGameObjectObserver(newGameObject);

				
					JobSystem::RunJob([&newGameObjectObserver, movableComponent]() {
						JSONUtilities::AddPhysicsItemFromJSONData(newGameObjectObserver, movableComponent);
					});

				}

				if (it.key() == "renderable") {
					nlohmann::json renderableComponent = it.value();
					// create observer instance
					WeakPtr<GameObject> newGameObjectObserver(newGameObject);
		
					JobSystem::RunJob([&newGameObjectObserver, renderableComponent]() {
						JSONUtilities::AddRenderableFromJSONData(newGameObjectObserver, renderableComponent);
					});

				}
			}
		}

		JobSystem::Wait();

		return newGameObject;
	}

	StrongPtr<GameObject> CreateGameObjectFromJSONFile(const std::string& i_JSONFileName) {

		StrongPtr<GameObject> newGameObject(nullptr);
		std::vector<uint8_t> Contents = JSONUtilities::LoadFile(i_JSONFileName);

		if (!Contents.empty()) {
			nlohmann::json JSONObject = nlohmann::json::parse(Contents);

			// ** don't know why smart pointer assignment causes crash (maybe I implemented it wrong) **
			// ERROR: 
			// newGameObject = JSONUtilities::CreateGameObjectFromJSONObject(JSONObject);
			// return newGameObject;

			return JSONUtilities::CreateGameObjectFromJSONObject(JSONObject);
		}
		else {
			return StrongPtr<GameObject>(nullptr);
		}


		return nullptr;
	}


	void AddPhysicsItemFromJSONData(WeakPtr<GameObject>& i_gameObject, nlohmann::json i_physicsJSONData) {
		float gameObjectkDrag = 0.5f;
		float gameObjectMass = 5.0f;
		bool gameObjectEnableGravity = false;
		Vector2 gameObjectForce = Vector2::Zero;
		Vector2 gameObjectVelocity = Vector2::Zero;
		bool collision_enabled = false;
		AABB boundingbox(Vector2::Zero, Vector2::Zero);
		bool input_enabled = false;

		// parse mass
		if (i_physicsJSONData.contains("mass")) {
			nlohmann::json mass = i_physicsJSONData["mass"];
			assert(mass.is_number_float());
			gameObjectMass = mass;
		}

		// parse drag coefficient
		if (i_physicsJSONData.contains("kDrag")) {
			nlohmann::json kDrag = i_physicsJSONData["kDrag"];
			assert(kDrag.is_number_float());
			gameObjectkDrag = kDrag;
		}


		// parse gravity enabled
		if (i_physicsJSONData.contains("enable_gravity")) {
			nlohmann::json enableGravity = i_physicsJSONData["enable_gravity"];
			assert(enableGravity.is_boolean());
			gameObjectEnableGravity = enableGravity;
		}
		
		// parse bounding box
		if (i_physicsJSONData.contains("enable_collision")) {
			nlohmann::json enableCollision = i_physicsJSONData["enable_collision"];
			assert(enableCollision.is_boolean());
			collision_enabled = enableCollision;
		}

		// parse bounding box
		if (i_physicsJSONData.contains("bounding_box")) {
			nlohmann::json bbox = i_physicsJSONData["bounding_box"];
			assert(bbox.is_array());
			assert(bbox.size() == 4);
			assert(bbox[0].is_number() && bbox[1].is_number() && bbox[2].is_number() && bbox[3].is_number());

			boundingbox.Center = Vector2(bbox[0], bbox[1]);
			boundingbox.Extents = Vector2(bbox[2], bbox[3]);
		}

		// parse initial force
		if (i_physicsJSONData.contains("initial_force")) {
			nlohmann::json force = i_physicsJSONData["initial_force"];
			assert(force.is_array());
			gameObjectForce = Vector2(force[0], force[1]);
		}

		// parse initial velocity
		if (i_physicsJSONData.contains("initial_velocity")) {
			nlohmann::json velocity = i_physicsJSONData["initial_velocity"];
			assert(velocity.is_array());
			gameObjectVelocity = Vector2(velocity[0], velocity[1]);
		}

		// parse input enabled
		if (i_physicsJSONData.contains("enable_input")) {
			nlohmann::json enableInput = i_physicsJSONData["ebable_input"];
			assert(enableInput.is_boolean());
			input_enabled = enableInput;
		}

		// add gameObject to phyics system (shared data, need to guard against simultaneous access)
		{
			// automatically unlocks when this goes out of scope
			std::unique_lock<std::mutex> lock{ JSONUtilities::readWriteToSystemMutex };
			Physics::AddPhysicsItem(i_gameObject, gameObjectEnableGravity, collision_enabled, boundingbox, gameObjectVelocity, gameObjectForce, gameObjectMass, gameObjectkDrag);
		}
	}

	void AddRenderableFromJSONData(WeakPtr<GameObject>& i_gameObject, nlohmann::json i_renderJSONData) {
		std::string gameObjectSpriteFileName;

		// parse sprite texture file name
		if (i_renderJSONData.contains("sprite_texture")) {
			nlohmann::json spriteFileName = i_renderJSONData["sprite_texture"];
			assert(spriteFileName.is_string());
			gameObjectSpriteFileName = spriteFileName;
		}

		{
			// automatically unlocks when this goes out of scope
			std::unique_lock<std::mutex> lock{ JSONUtilities::readWriteToSystemMutex };
			Renderer::AddRenderable(i_gameObject, gameObjectSpriteFileName.c_str());
		}
	}






	std::vector<uint8_t> LoadFile(const std::string i_pFileName) {
		assert(!i_pFileName.empty());

		std::vector<uint8_t> contents;

		FILE* pFile = NULL;

		errno_t fopenError = fopen_s(&pFile, i_pFileName.c_str(), "rb");

		if (fopenError == 0) {

			assert(pFile != NULL);

			int FileIOError = fseek(pFile, 0, SEEK_END);
			assert(FileIOError == 0);

			long FileSize = ftell(pFile);
			assert(FileSize >= 0);

			FileIOError = fseek(pFile, 0, SEEK_SET);
			assert(FileIOError == 0);

			contents.reserve(FileSize);
			contents.resize(FileSize);

			size_t FileRead = fread(&contents[0], 1, FileSize, pFile);
			assert(FileRead == FileSize);

			fclose(pFile);
		}

		return contents;
	}
}