#include "binoreconstruction.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	binoReconstruction w;
	w.show();
	return a.exec();
}
