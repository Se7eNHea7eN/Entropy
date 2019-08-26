#pragma once
#include <QWidget>
#include "Renderer.hpp"

class RenderWidget : public QWidget
{
	Q_OBJECT;

public:
	explicit RenderWidget(QWidget* parent);

	virtual ~RenderWidget() = default;

	virtual QPaintEngine* paintEngine() const override;

	void render();
	Renderer* renderer;

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


};
