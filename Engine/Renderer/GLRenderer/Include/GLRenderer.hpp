#pragma once
#include "Renderer/Renderer.hpp"
#include <windows.h>		// Header File For Windows

namespace Entropy {
	class GLRenderer : public Renderer {

	public:
		GLRenderer(HWND hwnd);
		~GLRenderer();
		void Initialize() override;
		void Resize(int w, int h) override;
		void Draw() override;
		void AwaitRenderFrame() override;

	private:
		HDC hDC = NULL; // Private GDI Device Context
		HGLRC hRC = NULL; // Permanent Rendering Context
		HWND hWnd = NULL; // Holds Our Window Handle
	};
}
