#pragma once

#include <QtWidgets/QMainWindow>
#include "../GeneratedFiles/ui_EntropyMainWindow.h"
#include "EntropyCore.hpp"

class EntropyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	EntropyMainWindow(QWidget *parent = Q_NULLPTR);

private:
	EntropyCore* engineCore = new EntropyCore();
	Ui::EntropyMainWindowClass ui;
};
