#pragma once
#include "Renderer.hpp"
#include <windows.h>		// Header File For Windows

namespace Entropy {
	class GLRenderer : public Renderer {

	public:
		GLRenderer(HWND hwnd);
		~GLRenderer();
		void initialize() override;
		void resize(int w, int h) override;
		void draw() override;
	private:
		HDC hDC = NULL; // Private GDI Device Context
		HGLRC hRC = NULL; // Permanent Rendering Context
		HWND hWnd = NULL; // Holds Our Window Handle
	};
}
