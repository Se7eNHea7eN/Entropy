#include "stdafx.h"
#include "EntropyDirect3DWidget.h"
#include "../../Direct3DRenderer/Public/Direct3DRenderer.h"

EntropyDirect3DWidget::EntropyDirect3DWidget(QWidget* parent): QWidget{parent} {
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	setMouseTracking(true); // ��������ƶ� ...
}

QPaintEngine* EntropyDirect3DWidget::paintEngine() const {
	return nullptr;
}

void EntropyDirect3DWidget::render() {
	if (_updatePending == false) {
		_updatePending = true;
		QApplication::postEvent(
			this,
			new QEvent{QEvent::UpdateRequest});
	}
}

void EntropyDirect3DWidget::paintEvent(QPaintEvent* paintEvent) {
	if (_isInit == false)
		_init();
	render();
}

void EntropyDirect3DWidget::showEvent(QShowEvent* showEvent) {
	QWidget::showEvent(showEvent);
	if (_isInit == false)
		_init();
}

void EntropyDirect3DWidget::resizeEvent(QResizeEvent* resizeEvent) {
	QWidget::resizeEvent(resizeEvent);
	auto sz = resizeEvent->size();
	if ((sz.width() < 0) || (sz.height() < 0))
		return;
	if (_isInit == false)
		return;

	//
	// put your resize code here...
	//
	QSize size = resizeEvent->size();
	int width = size.width();
	int height = size.height();
	renderer->resize(width, height);

	// because Qt is not sending update request when resizing smaller
	render();
}

bool EntropyDirect3DWidget::event(QEvent* event) {
	switch (event->type()) {
	case QEvent::UpdateRequest:
		_updatePending = false;
		_doRender();
		return true;
	default:
		return QWidget::event(event);
	}
}

void EntropyDirect3DWidget::_doRender() {
	if (isVisible() == false)
		return;
	if (_isInit == false)
		return;

	// you may want to add some code to control rendering frequency
	// and ensure you are not rendering too fast in case of continuous
	// rendering...
	// if you control the rendering frequency, don't forget to make a
	// call to render() if you're not going to do the rendering...

	//
	// do your custom rendering here...
	//
	renderer->draw();
	// next frame if rendering continuously
	if (continuousRender == true)
		render();
}

void EntropyDirect3DWidget::_init() {
	// you can grab the native window handler (HWND for Windows) of
	// this widget:
	auto nativeWindowHandler = winId();
	// do your init code here...

	auto hwnd = (HWND)nativeWindowHandler;

	renderer = new Direct3DRenderer(hwnd);
	renderer->initialize();
	_isInit = true;
}
