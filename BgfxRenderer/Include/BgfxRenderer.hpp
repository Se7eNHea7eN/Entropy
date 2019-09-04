#pragma once
#include "Framework/Common/Renderer.hpp"
#include <windows.h>		// Header File For Windows

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
