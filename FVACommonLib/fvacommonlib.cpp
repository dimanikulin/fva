#include "fvacommonlib.h"

#include <QDir>
#include <QTextStream>
#include <QProcess>
#include <QCoreApplication>
#include <QTGui/QMessageBox>

#include "../lib/json/json.h"
#include "../lib/qexifimageheader.h"
#include "RiffParser.h"

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

FVA_ERROR_CODE fvaSaveDictionary( const QString& file, QVariantMap& inputJson, QString& error )
{
	QDir dir ( file );
	if ( !dir.exists( file ) )
	{
		error = "dictionaries file does not exist" ;
		return FVA_ERROR_CANT_FIND_DICTIONARIES;
	}
	else
	{
		QString newName = file + "_" + QDateTime::currentDateTime().toString( "yyyy-MM-dd-hh-mm-ss").toAscii().data();
		if ( !dir.rename( file, newName ))
		{
			return FVA_ERROR_CANT_OPEN_DICTIONARIES;
		}
	}
	// open it
	QFile _file ( file );
	if ( !_file.open( QIODevice::WriteOnly ) )
	{
		error =  "can not open dictionaries";
		return FVA_ERROR_CANT_OPEN_DICTIONARIES;
	}

	bool res				= false;

	QByteArray data = QtJson::Json::serialize( inputJson, res );
	if ( !res )
	{
		error =  "can not save dictionaries";
		_file.close();
		return FVA_ERROR_CANT_SAVE_DICTIONARIES;
	}
	QString result = QString::fromLocal8Bit(data);

	QTextStream writeStream( &_file );
	writeStream << result;	
	writeStream.flush();
	_file.close();

	return FVA_NO_ERROR;
}

bool fvaIsInternalFile( const QString& fileName )
{
	return (	fileName.toUpper() == FVA_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_BACKGROUND_MUSIC_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DICTIONARY_NAME  );
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
				QString sStartDate = dirName.mid( 8,2 );
				bool res, res1 = false; 
				int dEndDate = sEndDate.toInt( &res );
				int dStartDate = sStartDate.toInt( &res1 );
				if ( !res || !res1 || !dEndDate || !dStartDate)
					return FVA_ERROR_WRONG_FOLDER_NAME;
				to = from.addDays(dEndDate-dStartDate);
				to = to.addDays(1);
			}
			else
				return FVA_ERROR_WRONG_FOLDER_NAME;
		}
		break;
		case 16: //months-day period
		{
			from = QDateTime::fromString( dirName.mid( 0,10 ), "yyyy.MM.dd");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			if ( dirName [ 10 ] != '-' ) // not a period
				return FVA_ERROR_WRONG_FOLDER_NAME;

			QString sTo = dirName.mid( 0, 4 ) + "." + dirName.mid( 11,5 );
			to = QDateTime::fromString( sTo, "yyyy.MM.dd");
			if ( !to.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = to.addDays(1);
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
	if (fileName.contains("IMG_"))
	{
		// it is also file name to extract name from "IMG_20150504_142546"
		QString newFileName = fileName;
		newFileName.remove("IMG_");
		date = QDateTime::fromString( newFileName, "yyyyMMdd_hhmmss" );
		if (!date.isValid())
			return FVA_ERROR_WRONG_FILE_NAME;
		else
			return FVA_NO_ERROR;
	}

	date = QDateTime::fromString( fileName, "yyyy-MM-dd-hh-mm-ss" );
	if ( !date.isValid() )
	{
		QString newFileName = QString(fileName).replace( "##","01" );
		date = QDateTime::fromString( newFileName, "yyyy-MM-dd-hh-mm-ss" );
		if ( !date.isValid() )
		{
			return FVA_ERROR_WRONG_FILE_NAME;
		}
	}
	return FVA_NO_ERROR;
}

fvaItem::fvaItem ()
{
	isFolder			= false;
	isFiltered			= true;
	hasDescriptionData	= false;
	deviceId			= 0;
	scanerId			= 0;
}

fvaItem::~fvaItem ()
{
	for (auto idChild = children.begin(); idChild != children.end(); ++idChild)
	{
		if (idChild == nullptr)
			continue;
		delete *idChild;
		*idChild = nullptr;
	}
}
QString fvaItem::getGuiName()
{
	if (isFolder)
	{
		QString desc; // = QString("(%1)").arg(children.size());
		if (hasDescriptionData && !eventOrDesc.isEmpty())
			desc = " (" + eventOrDesc.trimmed() + ")";
		
		if (dateTo.isValid())
		{
			if (dateFrom == dateTo) // one year
				return dateFrom.toString( "yyyy") + desc;
			else if (dateFrom.addDays(1) == dateTo)
				return dateFrom.toString( "yyyy/MM/dd") + desc;
			else
				return dateFrom.toString( "yyyy/MM/dd") + dateTo.toString( "-yyyy/MM/dd") + desc;
		}
		else 
			return dateFrom.toString( "yyyy/MM/dd") + desc;
	}
	else
	{
		return dateFrom.toString( "yyyy-MM-dd (hh:mm:ss)");
	}
	return "";
}
void fillNameByOneId(int ident, const QString& dict, const QVariantMap&	dictionaries, QString& fullName)
{
	QVariantList vlist; 

	if (ident)
	{
		vlist = dictionaries[dict].toList();
		for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
		{
			if ( i->toMap()["ID"].toInt() == ident )
			{
				if (fullName.isEmpty())
					fullName = i->toMap()["name"].toString();
				else 
					fullName += "\n[" + i->toMap()["name"].toString() + "]"; 
				break;
			}
		}
	}
}
QString fvaItem::getGuiFullName(const QVariantMap&	dictionaries)
{
	QString fullName;
	if (!hasDescriptionData)
		return "";
	if (!isFolder)
	{
		if (!eventOrDesc.isEmpty())
			fullName = eventOrDesc;
	}

	if (!tagsOrComment.isEmpty())
	{
		if (fullName.isEmpty())
			fullName = tagsOrComment;
		else
			fullName += ", " + tagsOrComment;
	}

	fillNameByOneId(deviceId,"devices",dictionaries,fullName);
	fillNameByOneId(scanerId,"scaners",dictionaries,fullName);

	//this->peopleIds;

	//this->placeIds;

	return fullName;
}
FVA_ERROR_CODE fvaLoadDeviceMapFromDictionary(DEVICE_MAP& deviceMap, const QString& dictPath)
{
	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return res;

	QVariantList vlist = dictionaries["devices"].toList();
	QVariantList people = dictionaries["people"].toList();

	for ( auto it = vlist.begin(); vlist.end() != it; ++it)
	{
		fvaDevice device;
		device.linkedName			= it->toMap()["LinkedName"].toString().toUpper().trimmed();
		device.deviceId				= it->toMap()["ID"].toInt();
		device.guiName				= it->toMap()["name"].toString().toUpper().trimmed();
		for (auto itP = people.begin(); people.end() != itP; ++itP )
		{
			if ( it->toMap()["OwnerId"].toInt() == itP->toMap()["ID"].toInt())
			{
				device.ownerName	= itP->toMap()["name"].toString();
				break;
			}
		}
		deviceMap[device.deviceId]	= device;
	}
	// TODO add scaners here
	
	return FVA_NO_ERROR;
}
DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const QString& pathToFile, QString& deviceName)
{
	deviceName = QExifImageHeader(pathToFile).value(QExifImageHeader::Make).toString()
						+ QExifImageHeader(pathToFile).value(QExifImageHeader::Model).toString();

	DEVICE_MAP result;
	if (deviceName.isEmpty())
		return DEVICE_MAP();
	QString fixedDevName = deviceName.toUpper().trimmed();
	for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it)
	{
		QString name = it.value().linkedName; 
		if (name == fixedDevName)
			result[it.key()] = it.value(); 
	}

	deviceName = deviceName.remove("  ");
	deviceName = deviceName.remove(QChar('\0'));					
	if (!deviceName.isEmpty())
	{
		for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it)
		{
			if (it.value().linkedName == deviceName.toUpper().trimmed())
				result[it.key()] = it.value(); 
		}
	}
	return result;
}

QDateTime fvaGetVideoTakenTime(const QString& pathToFile, QString& error)
{
	QExifImageHeader header(pathToFile);
	QDateTime renameDateTime = header.value(QExifImageHeader::DateTimeOriginal).toDateTime();
	QString _newName = renameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
	if (_newName.isEmpty())
	{
		RiffParser riffInfo;
		QString createdDate;
		if ( !riffInfo.open( pathToFile, error ) || !riffInfo.findTag( "IDIT", createdDate ) || !riffInfo.convertToDate( createdDate, renameDateTime ) )
		{
			
			QProcess myProcess;    
			myProcess.setProcessChannelMode(QProcess::MergedChannels);
			QStringList params;
			params.append(pathToFile);
			myProcess.start(QCoreApplication::applicationDirPath() + "/#BIN#/exiftool(-k).exe", params);
			QString output;
			while(myProcess.waitForReadyRead())
			{
				output = myProcess.readAll();
				myProcess.putChar('\n');
			}
			myProcess.waitForFinished( -1 );
			int index = output.indexOf("Date/Time Original");
			if (index != -1)
			{
				index = output.indexOf(":", index);
				if (index != 1)
				{
					QString time = output.mid(index+1,20 );
					renameDateTime = QDateTime::fromString( time, " yyyy:MM:dd hh:mm:ss" );
				}
			}
			
		}
	}
	return renameDateTime;
}