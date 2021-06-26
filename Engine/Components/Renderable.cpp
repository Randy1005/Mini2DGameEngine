#include "Renderable.h"


std::vector<Renderable*> Renderer::allRenderables;

void Renderer::AddRenderable(WeakPtr<GameObject>& i_gameObject, const char* i_pSpriteFileName) {
	allRenderables.push_back(new Renderable(i_gameObject, CreateSprite(i_pSpriteFileName)));
}


void Renderer::Update(WeakPtr<GameObject>& i_gameObject) {

}

GLib::Sprite* Renderer::CreateSprite(const char* i_pFileName) {
	assert(i_pFileName);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	void* pTextureFile = LoadFile(i_pFileName, sizeTextureFile);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture* pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

	// exit if something didn't work
	// probably need some debug logging in here!!!!
	if (pTextureFile)
		delete[] pTextureFile;

	if (pTexture == nullptr)
		return nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;

	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
	bool result = GLib::GetDimensions(*pTexture, width, height, depth);
	assert(result == true);
	assert((width > 0) && (height > 0));

	// Define the sprite edges
	GLib::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
	GLib::SpriteUVs	UVs = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
	GLib::RGBA							Color = { 255, 255, 255, 255 };

	// Create the sprite
	GLib::Sprite* pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

	// release our reference on the Texture
	GLib::Release(pTexture);

	return pSprite;
}

void* Renderer::LoadFile(const char* i_pFileName, size_t& o_sizeFile) {
	assert(i_pFileName != NULL);

	FILE* pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFileName, "rb");
	if (fopenError != 0)
		return NULL;

	assert(pFile != NULL);

	int FileIOError = fseek(pFile, 0, SEEK_END);
	assert(FileIOError == 0);

	long FileSize = ftell(pFile);
	assert(FileSize >= 0);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	assert(FileIOError == 0);

	uint8_t* pBuffer = new uint8_t[FileSize];
	assert(pBuffer);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	assert(FileRead == FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;
}

void Renderer::DrawAll() {
	// IMPORTANT: Tell GLib that we want to start rendering
	GLib::BeginRendering(DirectX::Colors::Black);

	// Tell GLib that we want to render some sprites
	GLib::Sprites::BeginRendering();


	for (auto& renderable : allRenderables) {

		StrongPtr<GameObject> tmpOwnerPtr = renderable->pGameObject.CreateOwner();
		if (tmpOwnerPtr) {
			GLib::Point2D position = { tmpOwnerPtr->GetPosition().X(), tmpOwnerPtr->GetPosition().Y() };
			GLib::Render(*renderable->pSprite, position, 0.0f, 0.0f);
		}
		else {
			// remove renderable
		}
	}


	// Tell GLib we're done rendering sprites
	GLib::Sprites::EndRendering();

	// IMPORTANT: Tell GLib we're done rendering
	GLib::EndRendering();
}

void Renderer::CleanUp() {
	for (Renderable* renderable : allRenderables) {
		delete renderable;
	}
}
