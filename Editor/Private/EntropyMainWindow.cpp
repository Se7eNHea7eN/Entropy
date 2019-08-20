#include "stdafx.h"
#include "EntropyMainWindow.h"
#include "EntropyOpenGLWidget.h"
#include "../../GLRenderer/Public/GLRenderer.h"
#include "EntropyDirect3DWidget.h"

EntropyMainWindow::EntropyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// ui.sceneRendererContainer->addWidget(new EntropyOpenGLWidget(new GLRenderer(),this));
	auto d3d = new EntropyDirect3DWidget(this);
	d3d->CreateD3DApp((HWND)d3d->winId());
	ui.sceneRendererContainer->addWidget(d3d);
}
