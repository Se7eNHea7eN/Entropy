#pragma once
#include "Common/Renderer.hpp"
#include <windows.h>		// Header File For Windows
#include <list>
#include <memory>

namespace Entropy {
	struct BgfxGeometry;
	class BgfxRenderer :public Renderer {
	public:
		BgfxRenderer(HWND hwnd) ;
		~BgfxRenderer() override;
		void Initialize() override;
		void Resize(int w, int h) override;
		void Draw() override;
		void AwaitRenderFrame() override;

	protected:
		float* viewMatrixArray = new float[16];
		float* projectionMatrixArray = new float[16];
		std::list<std::shared_ptr<BgfxGeometry>> geometries;
	private:
		HWND hwnd;
	};
}