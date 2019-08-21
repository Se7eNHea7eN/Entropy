#include "stdafx.h"
#include "../Public/GLRenderer.h"

GLRenderer::GLRenderer() {
}

GLRenderer::~GLRenderer(){
}

void GLRenderer::initialize() {
	initializeOpenGLFunctions();
}

void GLRenderer::resize(int w, int h) {

}

void GLRenderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0,0, 1, 0);
}

