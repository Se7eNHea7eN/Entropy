#include "stdafx.h"
#include "../Public/GLRenderer.h"
#include "../../glew/include/GL/glew.h"
#pragma comment(lib,"opengl32.lib")

GLRenderer::GLRenderer(HWND hwnd) {
	this->hWnd = hwnd;
}

GLRenderer::~GLRenderer(){
}

void GLRenderer::initialize() {
	// initializeOpenGLFunctions();
	// hDC = GetDC(hWnd);
	// hRC = wglCreateContext(hDC);
	// wglMakeCurrent(hDC, hRC);
	glewInit();
}

void GLRenderer::resize(int w, int h) {

}

void GLRenderer::draw() {
	// wglMakeCurrent(hDC, hRC);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(1,0, 1, 1);
	// SwapBuffers(hDC);
}

