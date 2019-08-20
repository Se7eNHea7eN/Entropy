#pragma once

#include "../Private/direct3drenderer_global.h"
#include "../../Renderer/Public/Renderer.h"

class DIRECT3DRENDERER_EXPORT Direct3DRenderer : Renderer
{
public:
	Direct3DRenderer();
	~Direct3DRenderer();
	void initialize() override;
	void resize(int w, int h) override;
	void draw() override;
};
