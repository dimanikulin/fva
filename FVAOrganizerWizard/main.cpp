#include "fvaorganizerwizard.h"
#include <QtWidgets/QApplication>
#include <QtCore/qtextcodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FVAOrganizerWizard w;
	w.show();
	return a.exec();
}
