#include "fvacommonlib.h"

#include <QDir>
#include <QTextStream>

#include "../lib/json/json.h"

FVA_ERROR_CODE fvaGetFolderDescription( const QString& folder, QVariantMap& outputJson, QString& error )
{
	QDir dir ( folder );
	QString descFolderPath = folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
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
FVA_ERROR_CODE fvaCreateFolderDescription (const QString& path, const QString& content, QString& error)
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
FVA_ERROR_CODE fvaLoadDictionary( const QString& file, QVariantMap& outputJson, QString& error )
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
bool fvaIsInternalFileName( const QString& fileName )
{
	return (	fileName.toUpper() == FVA_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_BACKGROUND_MUSIC_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_OLD_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_OLD_DESCRIPTION_FILE_NAME.toUpper() ) ;
}
FVA_FILE_TYPE fvaConvertFileExt2FileType ( const QString& type )
{	
	if ( type.toUpper() == "JPG" 
		|| type.toUpper() == "JPEG" 
		|| type.toUpper() == "PNG" 
		|| type.toUpper() == "BMP" 
		|| type.toUpper() == "GIF" )
		return FVA_FILE_TYPE_IMG;
	
	if ( type.toUpper() == "AVI" 
		|| type.toUpper() == "MOV" 
		|| type.toUpper() == "MPG" 
		|| type.toUpper() == "MP4" 
		|| type.toUpper() == "3GP" )
		return FVA_FILE_TYPE_VIDEO;

	if ( type.toUpper() == "WAV" )
		return FVA_FILE_TYPE_AUDIO;

	return FVA_FILE_TYPE_UNKNOWN;
}