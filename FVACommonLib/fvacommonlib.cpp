#include "fvacommonlib.h"
#include <QDir>

#include "../lib/json/json.h"

FVA_ERROR_CODE getFolderDescription( const QString& folder, QVariantMap& outputJson, QString& error )
{
	QDir dir ( folder );
	QString descFolderPath = folder + "/" + DESCRIPTION_FOLDER_NAME;
	if ( !dir.exists( descFolderPath ) )
	{
		error = "description for folder does not exist in folder:" + folder;
		return FVA_ERROR_CANT_FIND_DIR_DESC;
	}

	// load it
	QFile file ( descFolderPath );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		error =  "can not open description for folder in folder:" + folder;
		return FVA_ERROR_CANT_OPEN_DIR_DESC;
	}

	bool res				= false;
	QString		jsonData	= QString::fromLocal8Bit ( file.readAll() );
	QVariantMap	result		= QtJson::Json::parse ( jsonData, res ).toMap();
	file.close();
	return FVA_NO_ERROR;
}
