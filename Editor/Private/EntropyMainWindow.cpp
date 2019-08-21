#include "stdafx.h"
#include "EntropyMainWindow.h"
#include "EntropyOpenGLWidget.h"
#include "../../GLRenderer/Public/GLRenderer.h"
#include "EntropyDirect3DWidget.h"

EntropyMainWindow::EntropyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// ui.sceneRendererContainer->addWidget(new EntropyOpenGLWidget(this));
	ui.sceneRendererContainer->addWidget(new EntropyDirect3DWidget(this));
}
