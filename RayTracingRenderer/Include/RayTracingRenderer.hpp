#pragma once
#include "Common/Renderer.hpp"
#include <windows.h>		// Header File For Windows

#include "Tile.hpp"
#include <thread>
#include <functional>

namespace Entropy {
	class Hittable;
	class RTCamera;

	class RayTracingRenderer : public Renderer {

	public:
		RayTracingRenderer(HWND hwnd);
		~RayTracingRenderer();
		void Initialize() override;
		void Resize(int w, int h) override;
		void Draw() override;
		void AwaitRenderFrame() override;
		void RenderTask(Tile* tiles, int tileCount);
		void Render();
		void SetOnUpdate(const std::function<void()>& on_update) {
			onUpdate = on_update;
		}
	private:
		HDC hDC = NULL; // Private GDI Device Context
		HGLRC hRC = NULL; // Permanent Rendering Context
		HWND hWnd = NULL; // Holds Our Window Handle
		unsigned int texture;
		int renderWidth = 1280;
		int renderHeight = 720;
		unsigned char* renderBuffer = new unsigned char[renderWidth * renderHeight * 3];
		Tile tiles[16][16];
		std::thread threads[16];
		Hittable* world;
		RTCamera* camera;
		int sampleCount = 10;
		std::function<void()> onUpdate;

	private:
		void CheckTiles();
		
	};
}
