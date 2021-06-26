#pragma once
#include <DirectXColors.h>
#include "GameObject/GameObject.h"

typedef struct Renderable {
	WeakPtr<GameObject> pGameObject;
	GLib::Sprite* pSprite;

	Renderable(WeakPtr<GameObject>& i_gameObject, GLib::Sprite* i_pSprite) :
		pGameObject(i_gameObject),
		pSprite(i_pSprite)
	{ }

	~Renderable() {
		GLib::Release(pSprite);
	}
} Renderable;

class Renderer {
public:
	static void AddRenderable(WeakPtr<GameObject>& i_gameObject, const char* i_pSpriteFileName);

	void Update(WeakPtr<GameObject>& i_gameObject);

	// utility methods for loading sprite file and creating sprite instance
	static GLib::Sprite* CreateSprite(const char* i_pFileName);
	static void* LoadFile(const char* i_pFileName, size_t& o_sizeFile);

	// Draw sprites for all renderables objects
	static void DrawAll();

	// Clean up manually
	static void CleanUp();


	// to store all renderable objects
	static std::vector<Renderable*> allRenderables;

private:


};
