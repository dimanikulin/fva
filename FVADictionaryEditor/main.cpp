#include "fvadictionaryeditor.h"
#include <QtGui/QApplication>
#include "fvacommonlib.h"
#include <qtextcodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	if ( a.arguments().size() < 2 )
	{
		return FVA_ERROR_NOT_ENOUGH_ARG;
	}
	QString deviceMatchName;
	if ( a.arguments().size() >= 3 )
	{
		deviceMatchName = a.arguments().at(2);
	}
	QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
	FVADictionaryEditor w(a.arguments().at(1), deviceMatchName);
	w.show();
	return a.exec();
}
