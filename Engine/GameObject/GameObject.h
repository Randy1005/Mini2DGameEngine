#pragma once
#include "Dynamics/Vector2.h"
#include "SmartPointers/SmartPointers.h"
#include <vector>
#include "IO/InputSystem.h"
#include "Console/ConsolePrint.h"


class GameObject {

public:
	GameObject();

	GameObject(const Vector2& i_position, std::string& i_name);

	// utility method for strong pointer wrappers
	static StrongPtr<GameObject> CreateGameObject(const Vector2& i_position, std::string& i_name);
	static void AddGameObject(StrongPtr<GameObject>& i_newGameObject);
	~GameObject();

	Vector2 GetPosition() { return Position; }
	Vector2 GetVelocity() { return Velocity; }
	void SetPosition(const Vector2& i_position) { Position.X(i_position.X()); Position.Y(i_position.Y()); }
	void SetVelocity(const Vector2& i_velocity) { Velocity.X(i_velocity.X()); Velocity.Y(i_velocity.Y()); }
	virtual void Update();


	// TEST function to register to inputSystem
	void OnKeyInput(unsigned int vKeyID, bool bWentDown);

	bool inputEnabled = false;


private:
	Vector2 Position;
	Vector2 Velocity;
	std::string Name;
	static std::vector<StrongPtr<GameObject>> allGameObjects;
	
};
