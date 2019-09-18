#include "Common/EntropyCore.hpp"
#include "Common/Scene.hpp"
#include "Direct3DRenderer.hpp"

using namespace Entropy;

MemoryManager* g_pMemoryManager = new MemoryManager();

EntropyCore::EntropyCore()
{
	int ret;
	if ((ret = g_pMemoryManager->Initialize()) != 0) {
		Log("Memory Manager Initialize failed, will exit now.");
	}
}

Renderer* EntropyCore::CreateRenderer(HWND hwnd) {
	auto renderer = RendererFactory::CreateRenderer(renderAPI, hwnd);
	renderer->SetEngine(this);
	return renderer;
}

void EntropyCore::CreateScene(const char* scene_name) {
	m_currentScene = new Scene(scene_name);
}

void EntropyCore::Tick(float deltaTime) {
	if(m_currentScene != nullptr) {
		m_currentScene->Tick(deltaTime);
	}
}
