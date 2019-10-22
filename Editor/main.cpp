#include "stdafx.h"
#include "EntropyMainWindow.h"
#include <QtWidgets/QApplication>
using namespace Entropy;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EntropyMainWindow w;
	w.show();
	return a.exec();
}
