#pragma once
#include "Renderer/RendererFactory.hpp"

namespace Entropy {
	class Scene;
	class Renderer;
	class RendererFactory;

	struct EntropyInitConfig {
		RenderAPI renderAPI = Auto;
		HWND hwnd;
	};

	class EntropyCore {
	public:
		static bool Init(EntropyInitConfig config);
		static void Release();

		static EntropyCore* GetInstance() {
			return m_pInstance;
		}

		Renderer* GetRenderer() {
			return m_pRenderer;
		}
		void CreateScene(const char* scene_name);
		void Tick(float deltaTime);

	private:
		EntropyCore();

		static EntropyCore* m_pInstance;
		Renderer* m_pRenderer;
		Scene* m_currentScene;
	};

	EntropyCore* EntropyCore::m_pInstance = nullptr;
};

