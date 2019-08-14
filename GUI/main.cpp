#include "stdafx.h"
#include "EntropyMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EntropyMainWindow w;
	w.show();
	return a.exec();
}
