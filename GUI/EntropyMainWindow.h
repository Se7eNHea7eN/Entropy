#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EntropyMainWindow.h"

class EntropyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	EntropyMainWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::EntropyMainWindowClass ui;
};
