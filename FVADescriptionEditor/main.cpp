#include "fvadescriptioneditor.h"
#include "fvadescriptionfile.h"
#include "fvacommonlib.h"

#include <QtGui/QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if ( a.arguments().size() < 3 )
		return FVA_ERROR_NOT_ENOUGH_ARG;
	
	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = loadDictionary( a.arguments().at(1), dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return res;

	QDir dir(a.arguments().at(2));
	if ( !dir.exists( a.arguments().at(2) ) )
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;

	dir.cdUp();

	FVADescriptionFile desc ( dir.absolutePath() + "/" + DESCRIPTION_FILE_NAME );
	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;

	res = desc.load( titles, decsItems );
	if ( FVA_NO_ERROR != res )
		return res;

	FVADescriptionEditor w( titles, decsItems, dictionaries, a.arguments().at(2) );
	w.show();
	return a.exec();
}
