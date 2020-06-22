#include "fvaplayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FVAPlayer w;
	w.show();
	return a.exec();
}
