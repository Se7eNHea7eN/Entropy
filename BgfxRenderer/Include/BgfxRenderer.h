#pragma once
#include <windows.h>		// Header File For Windows
#include "Common/Renderer.hpp"

namespace Entropy {

	class BgfxRenderer :public Renderer {

	public:
		BgfxRenderer(HWND hwnd) ;
		~BgfxRenderer() override;
		void initialize() override;
		void resize(int w, int h) override;
		void draw() override;
	private:
		HWND hwnd;
	};
}
