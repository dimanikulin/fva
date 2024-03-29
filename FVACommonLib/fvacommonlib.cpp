/*!
* \file fvacommonlib.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvacommonlib.h"
#include "fvacommonexif.h"

#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

bool fvaIsInternalFile( const QString& fileName )
{
	return (fileName.toUpper() == FVA_BACKGROUND_MUSIC_FILE_NAME.toUpper() 
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
		//|| extention.toUpper()	== "PNG" 
		//|| extention.toUpper()	== "BMP" 
		//|| extention.toUpper()	== "GIF" 
		)
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

FVA_EXIT_CODE fvaParseDirName(const QString& dirName, QDateTime& from, QDateTime& to, const FvaFmtContext& ctx)
{	
	switch( dirName.length() )
	{
		case 4: // one year folder
		{
			from = QDateTime::fromString(dirName, ctx.fvaDirNameYear);
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from/*.addYears(1)*/;
		}
		break;
		case 9: // year period
		{
			if ( dirName[ 4 ] != '-' )
				return FVA_ERROR_WRONG_FOLDER_NAME;

			from	= QDateTime::fromString( dirName.mid( 0, 4 ), ctx.fvaDirNameYear);
			to = QDateTime::fromString(dirName.mid(5, 4), ctx.fvaDirNameYear);

			if ( !from.isValid() || !to.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
		}
		break;
		case 10 : // one-day event
		{
			from = QDateTime::fromString( dirName, ctx.fvaDirName);
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from.addDays(1); 
		}
		break;
		case 13 :
		{
			from = QDateTime::fromString(dirName.mid( 0,10 ), ctx.fvaDirName);
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
			from = QDateTime::fromString(dirName.mid( 0,10 ), ctx.fvaDirName);
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			if ( dirName [ 10 ] != '-' ) // not a period
				return FVA_ERROR_WRONG_FOLDER_NAME;

			QString sTo = dirName.mid( 0, 4 ) + "." + dirName.mid( 11,5 );
			to = QDateTime::fromString(sTo, ctx.fvaDirName);
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
FVA_EXIT_CODE fvaParseFileName(const QString& fileName, QDateTime& date,const FvaFmtContext& ctx)
{
	if (fileName.contains("IMG_") && fileName.length() == 19)
	{
		// it is also file name to extract name from "IMG_20150504_142546"
		QString newFileName = fileName;
		newFileName.remove("IMG_");
		date = QDateTime::fromString(newFileName, ctx.fileName1);
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
		date = QDateTime::fromString(newFileName, ctx.fileName2);
		if (!date.isValid())
			return FVA_ERROR_WRONG_FILE_NAME;
		else
			return FVA_NO_ERROR;
	}
	else if (fileName.contains("_") && fileName.length() == 15)
	{
		// it is also file name to extract name from "20150504_142546"
		QString newFileName = fileName;
		date = QDateTime::fromString(newFileName, ctx.fileName1);
		if (!date.isValid())
			return FVA_ERROR_WRONG_FILE_NAME;
		else
			return FVA_NO_ERROR;
	}

	date = QDateTime::fromString(fileName, ctx.fvaFileName);
	if ( !date.isValid() )
	{
		QString newFileName = QString(fileName).replace( "##","01" );
		date = QDateTime::fromString( newFileName, ctx.fvaFileName);
		if ( !date.isValid() )
		{
			return FVA_ERROR_WRONG_FILE_NAME;
		}
	}
	return FVA_NO_ERROR;
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
		QString name = it.value().linkedName.toUpper(); 
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


QVector<unsigned int> fvaStringToIds(const QString& strList)
{
	QVector<unsigned int> result;
	QStringList l = strList.split(',');
	for ( auto iter = l.begin(); iter != l.end(); ++iter )
		result.append(iter->toUInt());					

	return result;
}

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
FVA_EXIT_CODE fvaRunCLT(const QString& cmdName, const QString& inputDir, bool isRecursive, bool isReadOnly, const QString& custom)
{
	QProcess myProcess;
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(cmdName);
	params.append(inputDir);
	params.append(isRecursive ? "recursive=yes" : "recursive=no");
	params.append("logvel=4");
	params.append(isReadOnly ? "readonly=yes" : "readonly=no");
	if (!custom.isEmpty())
		params.append("custom=" + custom);

	myProcess.start("FVACLTProcess.exe", params);
	myProcess.waitForFinished(-1);

	return static_cast<FVA_EXIT_CODE> (myProcess.exitCode());
}
FVA_EXIT_CODE fvaCreateDirIfNotExists(const QString& dirPath)
{
	if (!QDir(dirPath).exists())
	{
		QDir dir(dirPath);

		if (!dir.mkdir(dirPath))
			return FVA_ERROR_CANT_CREATE_DIR;
		else
			return FVA_NO_ERROR;
	}
	else
		return FVA_ERROR_DEST_DIR_ALREADY_EXISTS;
}

FVA_EXIT_CODE fvaSaveStrListToFile(const QString& path, const QList<QString>& strList)
{
	QFile fileNew(path);
	fileNew.open(QIODevice::Append | QIODevice::Text);
	QTextStream writeStream(&fileNew);
	writeStream.setCodec("UTF-8");
	for (auto it = strList.begin(); it != strList.end(); ++it)
		writeStream << *it << "\n";
	writeStream.flush();
	fileNew.close();

	return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaLoadStrListFromFile(const QString& path, QList<QString>& strList)
{
	QFile file(path);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	
	QTextStream readStream(&file);
	readStream.setCodec("UTF-8");

	while (true)
	{
    		QString line = readStream.readLine();
		if (line.isNull())
			break;
		else
			strList.append(line);
	}

	file.close();

	return FVA_NO_ERROR;
}