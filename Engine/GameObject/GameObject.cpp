#include "GameObject/GameObject.h"
std::vector<StrongPtr<GameObject>> GameObject::allGameObjects;

GameObject::GameObject() { }

GameObject::GameObject(const Vector2& i_position, std::string& i_name) :
	Position(i_position.X(), i_position.Y()),
	Name(i_name)
{

}

StrongPtr<GameObject> GameObject::CreateGameObject(const Vector2& i_position, std::string& i_name)
{
	return StrongPtr<GameObject>(new GameObject(i_position, i_name));
}

void GameObject::AddGameObject(StrongPtr<GameObject>& i_newGameObject) {
	allGameObjects.push_back(i_newGameObject);
}


// TODO: copy constructor, move constructor

GameObject::~GameObject() {

}



void GameObject::Update() {

}

// test if function pointers work
void GameObject::OnKeyInput(unsigned int vKeyID, bool bWentDown) {
	if (vKeyID == KeyCode::KEY_A)
		SetPosition(GetPosition() + Vector2(-10.0f, 0.0f));
	if (vKeyID == KeyCode::KEY_D)
		SetPosition(GetPosition() + Vector2(10.0f, 0.0f));
	if (vKeyID == KeyCode::KEY_W)
		SetPosition(GetPosition() + Vector2(0.0f, 10.0f));
	if (vKeyID == KeyCode::KEY_S)
		SetPosition(GetPosition() + Vector2(0.0f, -10.0f));
	if (vKeyID == KeyCode::KEY_SPACE) {
		
	}
}