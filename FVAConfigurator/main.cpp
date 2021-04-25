#include "FVAConfigurator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FVAConfigurator w;
	w.show();
	return a.exec();
}
