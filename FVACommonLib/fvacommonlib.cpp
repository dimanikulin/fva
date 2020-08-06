#include "fvacommonlib.h"
#include "fvadefaultcfg.h"
#include "fvacommondata.h"
#include "fvadescriptionfile.h"
#include "fvacommonexif.h"

#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include "fvariffparser.h"

#include <windows.h>
#include <winbase.h>

bool fvaIsInternalFile( const QString& fileName )
{
	return (	fileName.toUpper() == FVA_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_BACKGROUND_MUSIC_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DB_NAME  );
}
bool fvaIsFVAFile( const QString& extention )
{
	return FVA_FS_TYPE_UNKNOWN != fvaConvertFileExt2FileType ( extention );
}

FVA_FS_TYPE fvaConvertFileExt2FileType ( const QString& extention )
{	
	if (	extention.toUpper()	== "JPG" 
		|| extention.toUpper()	== "JPEG" 
		|| extention.toUpper()	== "PNG" 
		|| extention.toUpper()	== "BMP" 
		|| extention.toUpper()	== "GIF" )
		return FVA_FS_TYPE_IMG;
	
	if ( extention.toUpper()	== "AVI" 
		|| extention.toUpper()	== "MOV" 
		|| extention.toUpper()	== "MPG" 
		|| extention.toUpper()	== "MP4" 
		|| extention.toUpper()	== "3GP"
		|| extention.toUpper()	== "MKV")
		return FVA_FS_TYPE_VIDEO;

	if ( extention.toUpper()	== "WAV" )
		return FVA_FS_TYPE_AUDIO;

	return FVA_FS_TYPE_UNKNOWN;
}

FVA_ERROR_CODE fvaParseDirName( const QString& dirName, QDateTime& from, QDateTime& to )
{	
	switch( dirName.length() )
	{
		case 4: // one year folder
		{
			from = QDateTime::fromString( dirName, FVA_DIR_NAME_YEAR_FMT);
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from/*.addYears(1)*/;
		}
		break;
		case 9: // year period
		{
			if ( dirName[ 4 ] != '-' )
				return FVA_ERROR_WRONG_FOLDER_NAME;

			from	= QDateTime::fromString( dirName.mid( 0, 4 ), FVA_DIR_NAME_YEAR_FMT);
			to	= QDateTime::fromString( dirName.mid( 5, 4 ), FVA_DIR_NAME_YEAR_FMT);

			if ( !from.isValid() || !to.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
		}
		break;
		case 10 : // one-day event
		{
			from = QDateTime::fromString( dirName, FVA_DIR_NAME_FMT);
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from.addDays(1); 
		}
		break;
		case 13 :
		{
			from = QDateTime::fromString( dirName.mid( 0,10 ), FVA_DIR_NAME_FMT );
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
			from = QDateTime::fromString( dirName.mid( 0,10 ), FVA_DIR_NAME_FMT);
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			if ( dirName [ 10 ] != '-' ) // not a period
				return FVA_ERROR_WRONG_FOLDER_NAME;

			QString sTo = dirName.mid( 0, 4 ) + "." + dirName.mid( 11,5 );
			to = QDateTime::fromString( sTo, FVA_DIR_NAME_FMT);
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
	if (fileName.contains("IMG_") && fileName.length() == 19 )
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
	else if (fileName.contains("WP_") && fileName.length() == 24)
	{
		// it is also file name to extract name from "WP_20151220_13_49_40_Pro"
		QString newFileName = fileName;
		newFileName.remove("WP_"); newFileName.remove("_Pro");
		date = QDateTime::fromString( newFileName, "yyyyMMdd_hh_mm_ss" );
		if (!date.isValid())
			return FVA_ERROR_WRONG_FILE_NAME;
		else
			return FVA_NO_ERROR;
	}
	else if (fileName.contains("_") && fileName.length() == 15)
	{
		// it is also file name to extract name from "20150504_142546"
		QString newFileName = fileName;
		date = QDateTime::fromString( newFileName, "yyyyMMdd_hhmmss" );
		if (!date.isValid())
			return FVA_ERROR_WRONG_FILE_NAME;
		else
			return FVA_NO_ERROR;
	}

	date = QDateTime::fromString( fileName, FVA_FILE_NAME_FMT );
	if ( !date.isValid() )
	{
		QString newFileName = QString(fileName).replace( "##","01" );
		date = QDateTime::fromString( newFileName, FVA_FILE_NAME_FMT );
		if ( !date.isValid() )
		{
			return FVA_ERROR_WRONG_FILE_NAME;
		}
	}
	return FVA_NO_ERROR;
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
	if ( !fvaFolder && !fvaFile )
		return "";
	if ( type != FVA_FS_TYPE_DIR && fvaFile )
	{
		if ( !fvaFile->description.isEmpty() )
			fullName = fvaFile->description;
	}

	if ( type != FVA_FS_TYPE_DIR && fvaFile)
	{
		if (fullName.isEmpty())
			fullName = fvaFile->comment;
		else
			fullName += ", " + fvaFile->comment;
	}
	else if (type == FVA_FS_TYPE_DIR && fvaFolder)
	{
		if (fullName.isEmpty())
			fullName = fvaFolder->tags;
		else
			fullName += ", " + fvaFolder->tags;		
	}

	if ( type != FVA_FS_TYPE_DIR && fvaFile)
	{
		fillNameByOneId(fvaFile->deviceId,"devices",dictionaries,fullName);
	}
	return fullName;
}
bool fvaFilter::doesIDMatchToFilter(unsigned int ID, const QVector<unsigned int>& Ids) const
{
	for (auto it = Ids.begin(); it != Ids.end();++it)
	{
		if (ID == *it)
			return  true;
	}
	return false;
}

bool fvaFilter::doIDsMatchToFilter(const QVector<unsigned int>& IDs, const QVector<unsigned int>& filterIds) const
{
	for (auto it = IDs.begin(); it != IDs.end();++it)
	{
		if (doesIDMatchToFilter(*it,filterIds))
			return  true;		
	}
	return false;
}
DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const QString& pathToFile, QString& deviceName)
{
	deviceName = fvaGetExifMakeAndModelFromFile(pathToFile);

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

	QDateTime renameDateTime = fvaGetExifDateTimeOriginalFromFile(pathToFile); 
	QString _newName = renameDateTime.toString( FVA_FILE_NAME_FMT );
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
			myProcess.start(QCoreApplication::applicationDirPath() + "/exiftool(-k).exe", params);
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
					renameDateTime = QDateTime::fromString(time, " " + EXIF_DATE_TIME_FMT);
				}
			}
			
		}
	}
	return renameDateTime;
}

QVector<unsigned int> fvaStringToIds(const QString& strList)
{
	QVector<unsigned int> result;
	QStringList l = strList.split(',');
	for ( auto iter = l.begin(); iter != l.end(); ++iter )
		result.append(iter->toUInt());					

	return result;
}

QString fvaDVget( const QString& fieldName, QVariantMap& result )
{
	QString fieldValue = "";
	if ( result.contains( fieldName ) )
	{
		fieldValue = result[fieldName].toString();
		result.remove(fieldName);
	}
	return fieldValue;
}

FVA_ERROR_CODE fvaGetIDFromFile(const QString& fileName, int& ID)
{
	QFile file( fileName );		
	if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )	
		return FVA_ERROR_CANT_OPEN_ID_FILE;	
	QTextStream readStream( &file );
	readStream >> ID;		
	file.close();	
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE fvaSaveIDInFile(const QString& fileName, int ID)
{
	QFile file ( fileName );		
	if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )	
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;	
	QTextStream writeStream( &file );	
	writeStream << ID;	
	writeStream.flush();	
	file.close();	
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaLoadFvaFileInfoFromScv(FVA_FILE_INFO_MAP& fvaFileInfo)
{
	FVADescriptionFile fvaFileCsv;

	// firstly - try to get device if from fvaFile.csv as it has high priority 
	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;

	FVA_ERROR_CODE res = fvaFileCsv.load( FVA_DEFAULT_ROOT_DIR + "fvaFile.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,OldName,WhoTook,OldName1
	int columnDevId = FVADescriptionFile::getColumnIdByName(titles,"DevId");
	if ( -1 == columnDevId )
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles,"Name");
	if ( -1 == columnName )
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnID = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();
		
		QString fileName = list[columnName].toUpper();
		if (fvaFileInfo.find(fileName) != fvaFileInfo.end())
		{
			QFile file(FVA_DEFAULT_ROOT_DIR + "fvaNotUniqueFileName.csv");
			file.open(QIODevice::WriteOnly | QIODevice::Append);
			QTextStream writeStream(&file);
			writeStream << list[columnID].toUpper() << "\n";
			file.close();
			return FVA_ERROR_NON_UNIQUE_FVA_INFO;
		}
		fvaFile newFile;
		newFile.deviceId = list[columnDevId].remove("\t").toUInt();
		fvaFileInfo[fileName.toUpper()] = newFile; 		
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaGetDeviceIdFromFvaInfo(const FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFile, int& deviceID,const QString& dir)
{
	deviceID = FVA_UNDEFINED_ID;

	if (fvaFileInfo.find(fvaFile.toUpper()) != fvaFileInfo.end() )
	{
		deviceID = fvaFileInfo[fvaFile.toUpper()].deviceId; 
		return FVA_NO_ERROR;
	}

	// we did not find it on fvafile info, lets try to find it in folder fva info
	FVADescriptionFile fvaFolderCsv;
	QStringList			titlesD; 
	DESCRIPTIONS_MAP	decsItemsD;
	FVA_ERROR_CODE res = fvaFolderCsv.load(FVA_DEFAULT_ROOT_DIR + "fvaFolder.csv", titlesD, decsItemsD); 
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder,WhoTookFotoId,Scanerid
	int columnDevIdD = FVADescriptionFile::getColumnIdByName(titlesD,"DevId");
	if ( -1 == columnDevIdD )
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnNameD = FVADescriptionFile::getColumnIdByName(titlesD,"Name");
	if ( -1 == columnNameD )
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	QString dirToMatch = dir;
		dirToMatch = dirToMatch.replace("\\","/");  // replace slaches on backslashes
		dirToMatch = dirToMatch.remove(FVA_DEFAULT_ROOT_DIR); // remove a prefix as root dir
		dirToMatch = "/" + dirToMatch;

	for (DESCRIPTIONS_MAP::Iterator it = decsItemsD.begin(); it != decsItemsD.end(); ++it)
	{
		QStringList list = it.value();
		
		if (list[columnNameD].toUpper() == dirToMatch.toUpper())
		{
			if (deviceID == FVA_UNDEFINED_ID)
			{
				deviceID = list[columnDevIdD].remove("\t").toUInt();
				
				QFile file ( FVA_DEFAULT_ROOT_DIR + "fvaFileNoDevId.csv" );		
				file.open( QIODevice::WriteOnly | QIODevice::Append );	
				int ID = FVA_UNDEFINED_ID;
				fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR +"fvaFile.id", ID);
				QTextStream writeStream( &file );
				writeStream << QString::number(++ID) << "," << fvaFile << ",,," <<  QString::number(deviceID) << ",,,,,,\n" ;
				fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR +"fvaFile.id", ID);
				file.close();

				return FVA_NO_ERROR;
			}
			else
				return FVA_ERROR_NON_UNIQUE_FVA_INFO;
		}
	}

	return FVA_ERROR_NO_DEV_ID;
};

bool fvaIsInternalDir(const QString& dir)
{
	if (dir.contains("#"))
		return true;
	return false;
}
bool fvaRemoveDirIfEmpty(const QString& dirPath)
{
	if (QDir(dirPath).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries).count() == 0)
	{
		QDir dir(dirPath);

		// empty folder now - no need in it to keep
		return dir.rmdir(dirPath);
	}
	else
		return false;
}