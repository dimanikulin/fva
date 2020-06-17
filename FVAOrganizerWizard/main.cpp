#include "fvaorganizerwizard.h"
#include <QtGui/QApplication>
#include <qtextcodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
	FVAOrganizerWizard w;
	w.show();
	return a.exec();
}
