#pragma once
#include "Renderer/RendererFactory.hpp"

namespace Entropy {
	class Scene;
	class Renderer;
	class RendererFactory;
	class EntropyCore {
	public:
		EntropyCore();
		Renderer* CreateRenderer(HWND hwnd);
		RenderAPI renderAPI = D3d11;

		bool debugMode = false;
		void CreateScene(const char* scene_name);

		Scene* CurrentScene() {
			return m_currentScene;
		}
		void Tick(float deltaTime);

	private:
		Scene* m_currentScene;
	};
}
