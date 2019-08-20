#include "stdafx.h"
#include "EntropyOpenGLWidget.h"
#include "QOpenGLFunctions"

EntropyOpenGLWidget::EntropyOpenGLWidget(Renderer* renderer, QWidget *parent) : QOpenGLWidget(parent)
{
	this->renderer = renderer;
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
