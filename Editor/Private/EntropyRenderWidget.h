#ifndef ENTROPYRENDERWIDGET_H
#define ENTROPYRENDERWIDGET_H

#include <QWidget>
#include "../../Renderer/Public/Renderer.h"
#include "../../GLRenderer/Public/GLRenderer.h"

class EntropyRenderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EntropyRenderWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
	// Renderer* renderer = new GLRenderer();
};

#endif // ENTROPYRENDERWIDGET_H
