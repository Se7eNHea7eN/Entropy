#pragma once
#include "../../Renderer/Public/Renderer.h"
#include "glrenderer_global.h"
#include "QOpenGLFunctions"

class GLRENDERER_EXPORT GLRenderer : public Renderer,protected QOpenGLFunctions {

public:
	GLRenderer();
	~GLRenderer();
	void initialize() override;
	void resize(int w, int h) override;
	void draw() override;
};
