#include "stdafx.h"
#include "EntropyMainWindow.h"
#include "EntropyOpenGLWidget.h"
#include "../../GLRenderer/Public/GLRenderer.h"
EntropyMainWindow::EntropyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.sceneRendererContainer->addWidget(new EntropyOpenGLWidget(new GLRenderer(),this));
}
