#include "IO/InputSystem.h"

InputSystem* InputSystem::m_instance = nullptr;
std::vector<std::function<void(unsigned int, bool)>> InputSystem::m_userRegisteredFunctions;

InputSystem::InputSystem() {

}

InputSystem::~InputSystem() {

}

InputSystem* InputSystem::GetInstance() {
	if (m_instance == nullptr) {
		m_instance = new InputSystem();
	}
	return m_instance;
}

void InputSystem::Initialize() {
	GLib::SetKeyStateChangeCallback(std::bind(&InputSystem::OnKeyChange, this, std::placeholders::_1, std::placeholders::_2));
}


void InputSystem::OnKeyChange(unsigned int vKeyID, bool bWentDown) {
	
	for (auto& functionCall : m_userRegisteredFunctions) {
		functionCall(vKeyID, bWentDown);
	}
}

void InputSystem::RegisterUserFunction(std::function<void(unsigned int, bool)> i_functionPointer) {
	m_userRegisteredFunctions.push_back(i_functionPointer);
}


void InputSystem::ResetInstance() {
	for (auto& function : m_userRegisteredFunctions) {
		function = nullptr;
	}
	m_userRegisteredFunctions.clear();

	delete m_instance;
	m_instance = nullptr;
}






