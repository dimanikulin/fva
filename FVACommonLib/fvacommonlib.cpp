#include "fvacommonlib.h"

#include <QDir>
#include <QTextStream>

#include "../lib/json/json.h"
#include "../lib/qexifimageheader.h"

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
bool fvaIsInternalFile( const QString& fileName )
{
	return (	fileName.toUpper() == FVA_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_BACKGROUND_MUSIC_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_OLD_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_OLD_DESCRIPTION_FILE_NAME.toUpper() ) ;
}
bool fvaIsFVAFile( const QString& extention )
{
	return FVA_FILE_TYPE_UNKNOWN != fvaConvertFileExt2FileType ( extention );
}

FVA_FILE_TYPE fvaConvertFileExt2FileType ( const QString& extention )
{	
	if (	extention.toUpper()	== "JPG" 
		|| extention.toUpper()	== "JPEG" 
		|| extention.toUpper()	== "PNG" 
		|| extention.toUpper()	== "BMP" 
		|| extention.toUpper()	== "GIF" )
		return FVA_FILE_TYPE_IMG;
	
	if ( extention.toUpper()	== "AVI" 
		|| extention.toUpper()	== "MOV" 
		|| extention.toUpper()	== "MPG" 
		|| extention.toUpper()	== "MP4" 
		|| extention.toUpper()	== "3GP" )
		return FVA_FILE_TYPE_VIDEO;

	if ( extention.toUpper()	== "WAV" )
		return FVA_FILE_TYPE_AUDIO;

	return FVA_FILE_TYPE_UNKNOWN;
}
FVA_ERROR_CODE fvaShowImage( const QString& fileName, QLabel* imgLabel )
{
	if ( fileName.isEmpty() || !imgLabel )
	{
		return FVA_ERROR_WRONG_PARAMETERS;
	}

	/*QExifImageHeader img( fileName );
	QImage image_thumb = img.thumbnail();
	if ( !image_thumb.isNull() )
	{
		imgLabel->setPixmap(QPixmap::fromImage( image_thumb ));
		return FVA_NO_ERROR;
	}*/

	QImage image( fileName );
	if ( image.isNull() )
	{
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	QPixmap _qpSource = QPixmap::fromImage(image); 
	QPixmap _qpCurrent = QPixmap::fromImage(image);

    float cw = imgLabel->width(), ch = imgLabel->height();
    float pw = _qpCurrent.width(), ph = _qpCurrent.height();

    if (pw > cw && ph > ch && pw/cw > ph/ch || //both width and high are bigger, ratio at high is bigger or
        pw > cw && ph <= ch || //only the width is bigger or
        pw < cw && ph < ch && cw/pw < ch/ph //both width and height is smaller, ratio at width is smaller
        )
        _qpCurrent = _qpSource.scaledToWidth(cw, Qt::TransformationMode::FastTransformation);
    else if (pw > cw && ph > ch && pw/cw <= ph/ch || //both width and high are bigger, ratio at width is bigger or
        ph > ch && pw <= cw || //only the height is bigger or
        pw < cw && ph < ch && cw/pw > ch/ph //both width and height is smaller, ratio at height is smaller
        )
        _qpCurrent = _qpSource.scaledToHeight(ch, Qt::TransformationMode::FastTransformation);

	imgLabel->setPixmap(_qpCurrent);
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaParseDirName( const QString& dirName, QDateTime& from, QDateTime& to )
{	
	switch( dirName.length() )
	{
		case 4: // one year folder
		{
			from = QDateTime::fromString( dirName, "yyyy");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from/*.addYears(1)*/;
		}
		break;
		case 9: // year period
		{
			if ( dirName[ 4 ] != '-' )
				return FVA_ERROR_WRONG_FOLDER_NAME;

			from	= QDateTime::fromString( dirName.mid( 0, 4 ), "yyyy");
			to	= QDateTime::fromString( dirName.mid( 5, 4 ), "yyyy");

			if ( !from.isValid() || !to.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
		}
		break;
		case 10 : // one-day event
		{
			from = QDateTime::fromString( dirName, "yyyy.MM.dd");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from.addDays(1); 
		}
		break;
		case 13 :
		{
			from = QDateTime::fromString( dirName.mid( 0,10 ), "yyyy.MM.dd");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			if ( dirName [ 10 ]  == ' ' ) // one day and several events
			{
				if ( dirName [ 11 ]  != '#' )
					return FVA_ERROR_WRONG_FOLDER_NAME;
				else
				{
					bool result = false;
					int dEventNumber = dirName.mid( 12, 1 ).toInt( &result );
					if ( !result || !dEventNumber )
						return FVA_ERROR_WRONG_FOLDER_NAME;
				}
				to = from.addDays(1);
			}
			else if ( dirName [ 10 ] == '-' ) // period
			{
				QString sEndDate = dirName.mid( 11,2 );
				bool result = false; 
				int dEndDate = sEndDate.toInt( &result );
				if ( !result || !dEndDate )
					return FVA_ERROR_WRONG_FOLDER_NAME;
				to = from.addDays(dEndDate);
			}
			else
				return FVA_ERROR_WRONG_FOLDER_NAME;
		}
		break;
		default:
			return FVA_ERROR_WRONG_FOLDER_NAME;
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaParseFileName( const QString& fileName, QDateTime& date )
{
	if ( fileName.length() != 19 )
	{
		return FVA_ERROR_WRONG_FILE_NAME;
	}
	date = QDateTime::fromString( fileName, "yyyy-MM-dd-hh-mm-ss" );
	if ( !date.isValid() )
	{
		QString newFileName = QString(fileName).replace( "#","0" );
		date = QDateTime::fromString( newFileName, "yyyy-MM-dd-hh-mm-ss" );
		if ( !date.isValid() )
		{
			return FVA_ERROR_WRONG_FILE_NAME;
		}
	}
	return FVA_NO_ERROR;
}