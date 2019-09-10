#pragma once
#include "Common/RendererFactory.hpp"

namespace Entropy {
	class Scene;
	class Renderer;
	class EntropyCore {
	public:
		EntropyCore();
		Renderer* CreateRenderer(HWND hwnd);
		RenderAPI renderAPI = Bgfx;
		void CreateScene(const char* scene_name);

		Scene* CurrentScene() {
			return m_currentScene;
		}
		void Tick();

	private:
		Scene* m_currentScene;
	};
}
