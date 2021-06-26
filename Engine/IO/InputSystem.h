#pragma once
#include "GLib.h"
#include <vector>
#include <unordered_set>


enum KeyCode : unsigned int {
	KEY_SPACE = 0x20,
	KEY_A = 0x41,
	KEY_W = 0x57,
	KEY_D = 0x44,
	KEY_S = 0x53
};



class InputSystem {
public:
	InputSystem();
	~InputSystem();

	static InputSystem* GetInstance();
	void Initialize();
	void OnKeyChange(unsigned int vKeyID, bool bWentDown);
	void RegisterUserFunction(std::function<void(unsigned int, bool)> i_functionPointer);
	void ResetInstance();


private:
	static InputSystem* m_instance;

	// to store a bunch of registered function pointers from user code
	static std::vector<std::function<void(unsigned int, bool)>> m_userRegisteredFunctions;


};


