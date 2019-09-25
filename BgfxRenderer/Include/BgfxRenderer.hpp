#pragma once
#include "Common/Renderer.hpp"
#include <windows.h>		// Header File For Windows
namespace Entropy {
	class BgfxRenderer :public Renderer {
	public:
		BgfxRenderer(HWND hwnd) ;
		~BgfxRenderer() override;
		void Initialize() override;
		void Resize(int w, int h) override;
		void Draw() override;
	protected:
		float* viewMatrixArray = new float[16];
		float* projectionMatrixArray = new float[16];

	private:
		HWND hwnd;
	};
}
