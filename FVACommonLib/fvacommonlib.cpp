#include "fvacommonlib.h"

#include <QDir>
#include <QTextStream>

#include "../lib/json/json.h"

FVA_ERROR_CODE getFolderDescription( const QString& folder, QVariantMap& outputJson, QString& error )
{
	QDir dir ( folder );
	QString descFolderPath = folder + "/" + DIR_DESCRIPTION_FILE_NAME;
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
	outputJson				= QtJson::Json::parse ( jsonData, res ).toMap();
	if ( !res )
	{
		error =  "incorrect format of folder description";
		file.close();
		return FVA_ERROR_INCORRECT_FORMAT;
	}
	file.close();
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE createFolderDescription (const QString& path, const QString& content, QString& error)
{
	QFile fileNew ( path );	
	if ( !fileNew.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		error = "can not open new description, in " + path;
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
	}
	QTextStream writeStream( &fileNew );
	writeStream << content;	
	writeStream.flush();
	fileNew.close();

	return FVA_NO_ERROR;
}
FVA_ERROR_CODE loadDictionary( const QString& file, QVariantMap& outputJson, QString& error )
{
	QDir dir ( file );
	if ( !dir.exists( file ) )
	{
		error = "dictionaries file does not exist" ;
		return FVA_ERROR_CANT_FIND_DICTIONARIES;
	}
	// load it
	QFile _file ( file );
	if ( !_file.open( QIODevice::ReadOnly ) )
	{
		error =  "can not open dictionaries";
		return FVA_ERROR_CANT_OPEN_DICTIONARIES;
	}

	bool res				= false;
	QString		jsonData	= QString::fromLocal8Bit ( _file.readAll() );
	outputJson				= QtJson::Json::parse ( jsonData, res ).toMap();
	if ( !res )
	{
		error =  "can not load dictionaries";
		_file.close();
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;
	}
	_file.close();
	return FVA_NO_ERROR;
}
