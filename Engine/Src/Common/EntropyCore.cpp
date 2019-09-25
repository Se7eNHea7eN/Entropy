#include "Common/EntropyCore.hpp"
#include "Common/Scene.hpp"
#include "Direct3DRenderer.hpp"
#include "Utils/Debug.hpp"

using namespace Entropy;

EntropyCore::EntropyCore()
{
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
