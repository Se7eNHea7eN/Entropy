#pragma once
#include <QWidget>
#include <QResizeEvent>
#include "../../Direct3DRenderer/Public/Direct3DRenderer.h"

class EntropyDirect3DWidget : public QWidget
{
	Q_OBJECT;

public:
	explicit EntropyDirect3DWidget(QWidget* parent);

	virtual ~EntropyDirect3DWidget() = default;

	virtual QPaintEngine* paintEngine() const override;

	void render();

public:
	bool continuousRender = false;

protected:
	virtual void paintEvent(QPaintEvent* paintEvent) override;

	virtual void showEvent(QShowEvent* showEvent) override;

	virtual void resizeEvent(QResizeEvent* resizeEvent) override;

	virtual bool event(QEvent* event) override;

private:
	void _doRender();

	void _init();

private:
	bool _updatePending = false;
	bool _isInit = false;


	Direct3DRenderer* renderer;
};
