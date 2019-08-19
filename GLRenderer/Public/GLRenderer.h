#pragma once
#include "../../Renderer/Public/Renderer.h"

class GLRenderer : public Renderer {

	void initialize() override;
	void resize(int w, int h) override;
	void draw() override;
};
