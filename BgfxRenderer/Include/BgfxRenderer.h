#pragma once
#include <windows.h>		// Header File For Windows
#include "Common/Renderer.hpp"

namespace Entropy {

	class BgfxRenderer :public Renderer {

	public:
		BgfxRenderer(HWND hwnd) ;
		~BgfxRenderer() override;
		void Initialize() override;
		void Resize(int w, int h) override;
		void Draw() override;
	private:
		HWND hwnd;
	};
}
