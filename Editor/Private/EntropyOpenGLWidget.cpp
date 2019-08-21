#include "stdafx.h"
#include "EntropyOpenGLWidget.h"
#include "QOpenGLFunctions"
#include "../../GLRenderer/Public/GLRenderer.h"

EntropyOpenGLWidget::EntropyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	this->renderer = new GLRenderer((HWND)winId());
}

void EntropyOpenGLWidget::mousePressEvent(QMouseEvent* event) {
}

void EntropyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
}

void EntropyOpenGLWidget::initializeGL() {
	renderer->initialize();
}

void EntropyOpenGLWidget::resizeGL(int w, int h) {
	renderer->resize(w,h);
}

void EntropyOpenGLWidget::paintGL() {
	renderer->draw();
}
