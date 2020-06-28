#include "Core/EntropyCore.hpp"
#include "Common/Scene.hpp"
#include "D3d11Renderer.hpp"
#include "Utils/Debug.hpp"

using namespace Entropy;

EntropyCore::EntropyCore():
	m_currentScene(nullptr),
	m_pRenderer(nullptr)
{
}

bool EntropyCore::Init(EntropyInitConfig config)
{
	if (m_pInstance == nullptr) {
		m_pInstance = new EntropyCore();
		m_pInstance->m_pRenderer = RendererFactory::CreateRenderer(config.renderAPI, config.hwnd);
		m_pInstance->m_pRenderer->SetEngine(m_pInstance);
	}

	return true;
}

void EntropyCore::Release()
{
}

void EntropyCore::CreateScene(const char* scene_name) {
	m_currentScene = new Scene(scene_name);
}

void EntropyCore::Tick(float deltaTime) {
	if(m_currentScene != nullptr) {
		m_currentScene->Tick(deltaTime);
	}
}
