#include "stdafx.h"
#include "entropyopenglwidget.h"

EntropyOpenGLWidget::EntropyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void EntropyOpenGLWidget::mousePressEvent(QMouseEvent* event) {
}

void EntropyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
}

void EntropyOpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();
}

void EntropyOpenGLWidget::resizeGL(int w, int h) {
}

void EntropyOpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(GLclampf(1.0f), GLclampf(1.0f), GLclampf(1.0f), GLclampf(1.0f));
}
