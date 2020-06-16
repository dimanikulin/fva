#include "fvadictionaryeditor.h"
#include <QtGui/QApplication>
#include "fvacommonlib.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	if ( a.arguments().size() < 2 )
	{
		return FVA_ERROR_NOT_ENOUGH_ARG;
	}

	FVADictionaryEditor w(a.arguments().at(1));
	w.show();
	return a.exec();
}
