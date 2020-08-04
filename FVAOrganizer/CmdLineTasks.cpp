#include "CmdLineTasks.h"
#include "fvadefaultcfg.h"
#include "fvacommondb.h"
#include "fvacommondata.h"

#include "../FVACommonLib/qexifimageheader.h"

#include <QtCore/QCoreApplication>

#include <iostream>
#include <iomanip>

#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

#include <windows.h>
#include <winbase.h>

FVA_ERROR_CODE CLT_Dir_Struct_Create_By_File::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		// skip meta files
		if ( fvaIsInternalFile( info.fileName() ) )
			continue;

		QString subFolderName		= info.baseName().mid(0,10).replace("-",".");
		QString fullSubFolderpath	= m_folder + "/" + subFolderName;
		fullSubFolderpath			= fullSubFolderpath.replace("-",".");
			
		if ( !m_dir.exists( subFolderName ) )
		{
			if ( !m_readOnly )
				m_dir.mkdir( subFolderName );
			LOG_QDEB << "sub-folder:" << subFolderName << " created";
		}
		if ( !m_readOnly ) 
		{
			// move the file
			if ( !m_dir.rename( info.absoluteFilePath(), fullSubFolderpath + "/" + info.fileName() ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "file renamed:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
		}
	}
	return FVA_NO_ERROR;
}

bool checkIfParentFileExist( const QFileInfo& fileToCheck, QDateTime& renameDateTime, const QDateTime& prevRenameDateTime )
{
	QString newPath = fileToCheck.absoluteFilePath().remove("_1");
	newPath = newPath.remove("_2");

	QDir dir;
	if ( !dir.exists( newPath ) )
		return false;

	if ( fileToCheck.baseName().contains("_1") )
	{
		qWarning() << "[ \"CLT_FILES_RENAME\" ]file contains _1:" << fileToCheck.absoluteFilePath() << ", rename time to use:" << prevRenameDateTime.addSecs( 1 ).toString( "yyyy-MM-dd-hh-mm-ss" );
		renameDateTime = prevRenameDateTime.addSecs ( 1 );
	}
	else if ( fileToCheck.baseName().contains("_2") )
	{
		qWarning() << "[ \"CLT_FILES_RENAME\" ]file contains _2:" << fileToCheck.absoluteFilePath() << ", rename time to use:" << prevRenameDateTime.addSecs( 2 ).toString( "yyyy-MM-dd-hh-mm-ss" );
		renameDateTime = prevRenameDateTime.addSecs ( 2 );	
	}
	return false;
}
void fillRenameDateTimeFromLastModifiedIfValid( const QDir& dir, const QFileInfo& info, QDateTime& renameDateTime )
{
	QString dirDate = dir.dirName().mid(0,10);
	QDateTime validDateStart = QDateTime::fromString( dirDate, FVA_DIR_NAME_FMT );
				
	if ( validDateStart.isValid() )
	{
		QDateTime validDateEnd = validDateStart.addDays( 1 );
		if ( ( info.lastModified() > validDateStart ) && ( info.lastModified() < validDateEnd ) )
		{
			qWarning() << "[ \"CLT_FILES_RENAME\" ]modification time to use for:" << info.absoluteFilePath() << ",time:" << info.lastModified().toString("yyyy-MM-dd-hh-mm-ss");
			renameDateTime = info.lastModified();
		}
	}				
}
FVA_ERROR_CODE CLT_Files_Rename::execute()
{
	QFileInfoList files = m_dir.entryInfoList(QDir::Files);
	QFileInfo info;

	QDateTime renameDateTime, prevRenameDateTime;

	Q_FOREACH( info, files )
	{
		renameDateTime = QDateTime();

		// if it is picture files
		QString suffix = info.suffix().toUpper();
		if(	FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType( suffix ) )
		{
			if ( !checkIfParentFileExist( info, renameDateTime, prevRenameDateTime ) ) 
			{
				renameDateTime = QExifImageHeader( info.filePath()).value(QExifImageHeader::DateTimeOriginal).toDateTime();
				QString _newName = renameDateTime.toString( FVA_FILE_NAME_FMT );
				if ( _newName.isEmpty() )
					fillRenameDateTimeFromLastModifiedIfValid( m_dir, info, renameDateTime );				
			}
		}
		// if it is video file
		else if( FVA_FS_TYPE_VIDEO == fvaConvertFileExt2FileType( suffix ) )
		{
			QString error;
			renameDateTime = fvaGetVideoTakenTime(info.filePath(), error);
			if (!renameDateTime.isValid())
			{
				if ( FVA_NO_ERROR != fvaParseFileName(info.baseName(), renameDateTime))
				{
					LOG_QWARN << "can not get taken time from:" << info.absoluteFilePath() << ",error:" << error;
					fillRenameDateTimeFromLastModifiedIfValid( m_dir, info, renameDateTime );
				}
			}
		}
		else if( FVA_FS_TYPE_AUDIO == fvaConvertFileExt2FileType( suffix ) )
		{
			fillRenameDateTimeFromLastModifiedIfValid( m_dir, info, renameDateTime );
		}
		else
		{
			LOG_QWARN << "unsupported file type:" << info.absoluteFilePath() ;
			continue;
		}
		QString newName = renameDateTime.toString( FVA_FILE_NAME_FMT );
		if ( newName.isEmpty() )
		{
			LOG_QWARN << "no time in img file:" << info.absoluteFilePath() << ",prev time to use:" << prevRenameDateTime.addSecs( 1 ).toString("yyyy-MM-dd-hh-mm-ss");
			prevRenameDateTime	= prevRenameDateTime.addSecs( 1 );
			newName				= prevRenameDateTime.toString( FVA_FILE_NAME_FMT );
		}
		else			
			prevRenameDateTime	= renameDateTime;

		if ( newName.isEmpty() )
		{
			LOG_QCRIT << "sequence error for file:" << info.absoluteFilePath();
			return FVA_ERROR_SEQUENCE;
		}

		QString newPath = "";
		if ( "OLD_NAME" == m_custom )
			newPath = m_dir.path() + "/" + newName + "__" + info.baseName() + "." + info.suffix();
		else
			newPath = m_dir.path() + "/" + newName + "." + info.suffix();
		
		if ( info.absoluteFilePath() == newPath )
		{
			LOG_QWARN << "File already has the same name:" << info.absoluteFilePath();
			continue;
		}
		
		if ( m_dir.exists( newPath ) )
		{
			LOG_QWARN << "file already exists:" << newPath << ",old path:" << info.absoluteFilePath();
			if ( !m_dir.exists( m_dir.path() + "/#copy" ) )
			{
				LOG_QWARN << "created COPY folder:" << m_dir.path() + "/#copy";
				m_dir.mkdir( m_dir.path() + "/#copy" );
			}
			QString newCopyPath = m_dir.path() + "/#copy/" + newName + "." + info.suffix();
			if ( !m_dir.rename( info.absoluteFilePath(), newCopyPath ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newCopyPath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "renamed file:" << info.absoluteFilePath() << " to:" << newCopyPath;
				
			continue;
		}

		if ( !m_dir.rename( info.absoluteFilePath(), newPath ) )
			LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newPath;
		else
			LOG_QDEB << "Renamed:" << info.absoluteFilePath() << " to:"<< newPath;
	}
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Device_Name_Check::execute()
{
	QString deviceName;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		// if it is picture files
		QString suffix		= info.suffix().toUpper();
		QString fullname	= info.absoluteFilePath();
		if(	FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType ( suffix ) )
			continue;
		
		QString newDeviceName = QExifImageHeader( info.filePath()).value(QExifImageHeader::Make).toString()
								+ QExifImageHeader( info.filePath()).value(QExifImageHeader::Model).toString();
		if ( newDeviceName.isEmpty() )
		{
			LOG_QWARN << "no device name in picture:" << info.absoluteFilePath();
			continue; // not so crirical
			return FVA_ERROR_EMPTY_DEVICE_NAME;
		}
		else
		{
			if ( deviceName.isEmpty() )
				LOG_QDEB << "got first device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath();
			else if ( deviceName != newDeviceName )	
			{
				LOG_QCRIT << "got new device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath() << "old: " << deviceName;
				return FVA_ERROR_NON_UNIQUE_DEVICE_NAME;
			}			
			deviceName = newDeviceName; 
		}		
	}

	return FVA_NO_ERROR;
};

FVA_ERROR_CODE CLT_Video_Rename_By_Sequence::execute()
{
	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		QString suffix = info.suffix().toUpper();
		if(	FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) && imageFilePrefix.isEmpty() )
		{				
			if ( !info.baseName().contains("_") )
				continue;
			int index		= info.baseName().indexOf("_");
			imageFilePrefix	= info.baseName().mid(0, index );
			LOG_QDEB << "got new image prefix:" << imageFilePrefix;
		}
		else if( FVA_FS_TYPE_VIDEO == fvaConvertFileExt2FileType ( suffix ) )
		{	
			QString error;
			QDateTime renameDateTime = fvaGetVideoTakenTime(info.filePath(), error);
			if (renameDateTime.isValid() || info.baseName().at(0) == 'P' ) // P is first latter for panasonic cameras
			{
				continue;
			}

			FVA_ERROR_CODE res = fvaParseFileName(info.baseName(), renameDateTime );
			if (FVA_NO_ERROR == res)
			{
				continue;
			}
			if ( !info.baseName().contains("_") )
			{
				LOG_QCRIT << "video file does not contain _:" << info.absoluteFilePath();
				return FVA_ERROR_SEQUENCE;
			}
			int index				= info.baseName().indexOf("_");
			QString videoFilePrefix	= info.baseName().mid(0, index );
			if ( imageFilePrefix.isEmpty() )
			{
				LOG_QCRIT << "still empty image prefix for path:" << info.absoluteFilePath();
				return FVA_ERROR_SEQUENCE;
			}

			QString newFilePath	= info.absoluteFilePath().replace( videoFilePrefix, imageFilePrefix ) ;

			// sometimes the file names are already in state as we need
			if ( info.absoluteFilePath() == newFilePath )
			{
				LOG_QWARN << "file has already target name" << info.absoluteFilePath() << ", skipping";
				continue;
			}
			if ( !m_dir.rename( info.absoluteFilePath(), newFilePath ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newFilePath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "renamed file:" << info.absoluteFilePath() << " to:" << newFilePath;
		}
		else if ( FVA_FS_TYPE_UNKNOWN == fvaConvertFileExt2FileType ( suffix ) )
		{
			LOG_QWARN << "unsupported file type:" << info.absoluteFilePath() ;
			continue;
		}
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Auto_Checks_2::execute()
{
	DEVICE_MAP deviceMap;
	FVA_ERROR_CODE res = fvaLoadDeviceMapFromDictionary(deviceMap, FVA_DEFAULT_ROOT_DIR + FVA_DB_NAME);
	RET_RES_IF_RES_IS_ERROR

	FVA_FILE_INFO_MAP fvaFileInfo;
	res = fvaLoadFvaFileInfoFromScv( fvaFileInfo );
	RET_RES_IF_RES_IS_ERROR

	QMap<QString, unsigned int> fileCount;
	unsigned int countSupportedFiles = 0; 
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{		
		// 1.CHECK FOR PROPER FOLDER NAME (NO COPY FOLDER)
		if ( info.isDir() )
		{	
			QDateTime from, to;
			if ( FVA_NO_ERROR != fvaParseDirName(info.fileName(), from, to))
			{
				// skip internal folder 
				if (m_dir.dirName()[0] == '#')
					continue;

				LOG_QCRIT << "wrong folder name:" << info.absoluteFilePath();
				m_Issues.push_back("FVA_ERROR_WRONG_FOLDER_NAME," + info.absoluteFilePath() + "," + info.fileName() );
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FOLDER_NAME;
			}
			continue;
		}

		// 2.CHECK FOR PROPER FILE NAME AND SUPPORTED TYPE
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType ( suffix );
		if ( FVA_FS_TYPE_UNKNOWN != type )
		{
			QDateTime date;
			QString baseFileName = info.baseName();
			if ( FVA_NO_ERROR != fvaParseFileName(info.baseName(), date))
			{
				LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath();
				m_Issues.push_back("FVA_ERROR_WRONG_FILE_NAME," + info.absoluteFilePath() + "," + info.fileName() );
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FILE_NAME;
			}
			//////////////////////////////////// 3. check for exsiting device in fva info by fileName 
			int deviceID = FVA_UNDEFINED_ID;
			FVA_ERROR_CODE res = fvaGetDeviceIdFromFvaInfo(fvaFileInfo, info.fileName(), deviceID, info.absoluteDir().absolutePath());
			if (FVA_NO_ERROR != res)
			{
				LOG_QWARN << "no dev id found for file: " << info.absoluteFilePath();
				if (FVA_ERROR_NO_DEV_ID==res)
					m_Issues.push_back("FVA_ERROR_NO_DEV_ID," + info.absoluteFilePath() + "," + info.fileName() );
				if (FVA_ERROR_NON_UNIQUE_FVA_INFO==res)
					m_Issues.push_back("FVA_ERROR_NON_UNIQUE_FVA_INFO," + info.absoluteFilePath() + "," + info.fileName() );
			}
			//////////////////////////////////// 4. check for exsiting device in dictionary by device name in pictire 
			if (FVA_FS_TYPE_IMG == type)
			{
				QString deviceName;
				if (0 == fvaGetDeviceMapForImg(deviceMap, info.filePath(),deviceName).size()) 
				{
					LOG_QWARN << "unknown device found:" << deviceName.trimmed() << " in file :" << info.absoluteFilePath();
					m_Issues.push_back("FVA_ERROR_UKNOWN_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) + "," + deviceMap[deviceID].guiName + " " + deviceMap[deviceID].ownerName);
					countSupportedFiles++;	// it is our file
					if (m_readOnly)
						continue;
					else
						return FVA_ERROR_UKNOWN_DEVICE;
				}

				if (deviceName.isEmpty())
				{
					LOG_QWARN << "empty device found:" << deviceName.trimmed() << " in file :" << info.absoluteFilePath();
					countSupportedFiles++;	// it is our file
					m_Issues.push_back("FVA_ERROR_EMPTY_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) + "," + deviceMap[deviceID].guiName + " " + deviceMap[deviceID].ownerName);
					continue;
				}
				if (deviceMap.size() == 1 && deviceMap[0].deviceId != deviceID)
				{
					LOG_QWARN << "device id linked wrongly,got from image=" << deviceMap[0].deviceId << ", from fvafile=" << deviceID;
					countSupportedFiles++;	// it is our file
					m_Issues.push_back("FVA_ERROR_LINKED_WRONG_DEVICE," + info.absoluteFilePath() + "," + QString::number(deviceID) + "," + deviceMap[deviceID].guiName + " " + deviceMap[deviceID].ownerName);
					continue;
				}
			}

			////////////////////////////////// 5. check for matching taken time and file name//////////////////////////
			if (FVA_FS_TYPE_IMG == type)
			{
				QString error;
				QDateTime dateTime = QExifImageHeader( info.filePath()).value(QExifImageHeader::DateTimeOriginal).toDateTime();

				if (!dateTime.isValid())
				{
					LOG_QWARN << "empty image taken time found in:" << info.absoluteFilePath();
					m_Issues.push_back("FVA_ERROR_NULL_TAKEN_TIME," + info.absoluteFilePath() + "," + info.fileName() );
				}
				else
				{
					if (dateTime != date)
					{
						LOG_QWARN << "mismatching image taken time found in:" << info.absoluteFilePath();
						m_Issues.push_back("FVA_ERROR_MISMATCH_TAKEN_TIME," + info.absoluteFilePath()+ "," + info.fileName() );
					}
				}
			}
			if (FVA_FS_TYPE_VIDEO == type)
			{
				QString error;
				QDateTime dateTime = QExifImageHeader( info.filePath()).value(QExifImageHeader::DateTimeOriginal).toDateTime();
				if (!dateTime.isValid())
				{
					LOG_QWARN << "empty video taken time found in:" << info.absoluteFilePath();
					m_Issues.push_back("FVA_ERROR_EMPTY_VIDEO_TIME," + info.absoluteFilePath()+ "," + info.fileName() );
				}
			}

			//////////////////////////////////// 6. MATCHING FILE NAME AND FOLDER NAME ////////////////////////////////////////////////////
			QDateTime dateStart, dateEnd;
			if ( FVA_NO_ERROR != fvaParseDirName( m_dir.dirName(), dateStart, dateEnd))
			{
				// skip internal folder 
				if (m_dir.dirName()[0] == '#')
					continue;

				LOG_QCRIT << "wrong matchig folder name:" << info.absoluteFilePath();
				m_Issues.push_back("FVA_ERROR_WRONG_FOLDER_NAME," + info.absoluteFilePath()+ "," + info.fileName() );
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FOLDER_NAME;
			}
			if (dateStart == dateEnd)
				dateEnd = dateEnd.addYears(1);
			QDateTime fileDateTime = QDateTime::fromString( baseFileName , FVA_FILE_NAME_FMT );
			QString newFileName = baseFileName.replace( "##","01" );
			if ( !fileDateTime.isValid() )
				fileDateTime = QDateTime::fromString( newFileName , FVA_FILE_NAME_FMT );
			if ( ( fileDateTime < dateStart ) ||( fileDateTime > dateEnd ) )
			{
				LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath() << " data period=" << dateStart << ";" << dateEnd;
				m_Issues.push_back("FVA_ERROR_NOT_SUPPORTED_FILE," + info.absoluteFilePath() + ","  + info.fileName());
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_NOT_SUPPORTED_FILE;
			}
			countSupportedFiles++;				
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else if ( fvaIsInternalFile ( info.fileName() ) ) 
		{
			// nothing to do here
			m_Issues.push_back("FVA_ERROR_INTERNAL_FILE," + info.absoluteFilePath() + ","  + info.fileName());
		}
		else
		{
			LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath();
			m_Issues.push_back("FVA_ERROR_NOT_SUPPORTED_FILE," + info.absoluteFilePath() + ","  + info.fileName() );
			if (!m_readOnly)
				return FVA_ERROR_NOT_SUPPORTED_FILE;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	m_fileCount[ m_folder ] = countSupportedFiles;
	if ( countSupportedFiles < FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR && countSupportedFiles )
	{
		// check for to little supported 
		LOG_QCRIT << "too little supported files found in:" << m_folder;
		m_Issues.push_back("FVA_ERROR_TOO_LITTLE_FILES," + m_folder + "," );
		if (!m_readOnly)
			return FVA_ERROR_TOO_LITTLE_FILES;
	}

	return FVA_NO_ERROR;
}
QMap< unsigned int , unsigned int > sizes;

CLT_Auto_Checks_2::~CLT_Auto_Checks_2()
{	
	LOG_QDEB << "cmd deleted, dir:" << m_folder;
	for ( auto it = m_fileCount.begin(); it != m_fileCount.end(); ++it )
	{
		sizes[it.value()] = ++sizes[it.value()];
	}
	QFile fileNew ( FVA_DEFAULT_ROOT_DIR + "issues.csv" );	
	fileNew.open( QIODevice::Append | QIODevice::Text );
	QTextStream writeStream( &fileNew );
	writeStream.setCodec("UTF-8");
	for ( auto it = m_Issues.begin(); it != m_Issues.end(); ++it )
		writeStream << *it << "\n";	
	writeStream.flush();
	fileNew.close();
}
FVA_ERROR_CODE CLT_Alone_Files_Move::execute()
{
	unsigned int countSupportedFiles = 0; 
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{		
		if ( info.isDir() )
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType ( suffix );
		if ( FVA_FS_TYPE_UNKNOWN != type )
			countSupportedFiles++;
	}
	// no need to move these files
	if ( countSupportedFiles >= FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR || !countSupportedFiles )
		return FVA_NO_ERROR;

	m_dir.cdUp();
	QString folderUp = m_dir.absolutePath();
	m_dir = QDir (m_folder);
	
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{		
		if ( info.isDir() )
			continue;

		// skip meta files
		if ( fvaIsInternalFile( info.fileName() ) )
			continue;

		LOG_QWARN << "to move to:" << folderUp + "/" + info.fileName() 
			<< " from " << info.absoluteFilePath();

		// move the files
		if ( !m_readOnly )
		{
			if ( !m_dir.rename( info.absoluteFilePath(), folderUp + "/" + info.fileName() ) )
			{
				LOG_QCRIT << "cant move file:" << info.absoluteFilePath();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
		}
	}

	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Auto_Checks_1::execute()
{
	m_dir.setSorting(QDir::LocaleAware);
	bool first = false;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType ( suffix );

		//	#01.NotVideoFirst
		if (!first)
		{
			first = true;
			if (FVA_FS_TYPE_VIDEO == type || FVA_FS_TYPE_AUDIO == type)
			{
				QString error;
				QDateTime time = fvaGetVideoTakenTime(info.absoluteFilePath(),error);
				if (!time.isValid())
				{
					LOG_QWARN << "found first video/audio file:" << info.absoluteFilePath();
					return FVA_ERROR_VIDEO_FIRST;
				}
			}
		}

		//#02.NotSTFiles
		QString st = info.fileName().mid(0,2).toUpper();
		if (st == "ST")
		{
			LOG_QCRIT << "found panoram file:" << info.absoluteFilePath();
			return FVA_ERROR_PANORAM_FILE;
		}
	}

	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Convert_Amr::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		QString suffix = info.suffix().toUpper();
		if ( suffix == "AMR" )
		{
			LOG_QCRIT << "found not supported file:" << info.absoluteFilePath();
			return FVA_ERROR_NOT_SUPPORTED_FILE;
		}
	}
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Set_File_Atts::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType ( suffix );
		if (FVA_FS_TYPE_UNKNOWN != type)
		{
			if (!SetFileAttributes(info.absoluteFilePath().toStdWString().c_str(), FILE_ATTRIBUTE_READONLY))
				LOG_QCRIT << "can not set attr for fva file:" << info.absoluteFilePath();
		}
		else
		{
			if ( fvaIsInternalFile( info.fileName() ) )
			{
				if (!SetFileAttributes(info.absoluteFilePath().toStdWString().c_str(), /*FILE_ATTRIBUTE_HIDDEN |*/ FILE_ATTRIBUTE_READONLY))
					LOG_QCRIT << "can not set attr for internal file:" << info.absoluteFilePath();
			}
			else
			{
				LOG_QCRIT << "found not supported file:" << info.absoluteFilePath();
				return FVA_ERROR_NOT_SUPPORTED_FILE;
			}
		}
	}
	
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_1_Event_Folder_Merging::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{				
		QString original	= m_folder + QDir::separator() + info.fileName();
		QString dest		= m_custom + QDir::separator() + info.fileName();

		// skip internal folder 
		if (original.contains("#copy") || dest.contains("#copy"))
			continue;

		if( !m_dir.rename( original, dest ) )
		{						
			LOG_QCRIT << "could not move:" << original << " into " << dest;
			return FVA_ERROR_CANT_MOVE_DIR;
		}

		LOG_QDEB << "moved:" << original << " into " << dest;

		if (!fvaRemoveDirIfEmpty(dest))
		{
			LOG_QCRIT << "could not remove empty dest:" << dest;
			return FVA_ERROR_CANT_MOVE_DIR;
		}
		else
		{
			LOG_QDEB << "removed empty destination:" << dest;
			continue;
		}
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_1_Day_Event_Folder_Merging::execute()
{
	// get the last dir leaf in input folder
	QString dir = m_dir.dirName();
	QString dstDirPath = FVA_DEFAULT_ROOT_DIR + dir.mid(0, 4)/*extract year*/ + "/" + m_dir.dirName();	
	if (!m_dir.exists(dstDirPath))
	{
		if (!dstDirPath.contains("#"))
		{
			// # means internal folder and not subject of merging
			if (!m_dir.mkpath(dstDirPath))
			{
				LOG_QCRIT << "could not create dest folder:" << dstDirPath;
				return FVA_ERROR_CANT_CREATE_DIR;
			}
		}
	}
	else
	{
		LOG_QCRIT << "could not create dest folder as it already exists:" << dstDirPath;
		return FVA_ERROR_CANT_CREATE_DIR;
	}
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// skip internal folder 
		if (dir.contains("#copy") || dstDirPath.contains("#copy"))
		{
			LOG_QWARN << "skipped #copy for: " << info.absoluteFilePath() << " , dst: " << dstDirPath;
			continue;
		}
		// we move files only
		if (!info.isDir() && !m_dir.rename(info.absoluteFilePath(), dstDirPath + "/" + info.fileName()))
		{
			LOG_QCRIT << "could not move:" << info.absoluteFilePath() << " into " << dstDirPath + "/" + info.fileName();
			return FVA_ERROR_CANT_MOVE_DIR;
		}
		else if (!info.isDir())
			LOG_QDEB << "moved:" << info.absoluteFilePath() << " into " << dstDirPath + "/" + info.fileName();

		if (info.isDir() && !fvaRemoveDirIfEmpty(info.absoluteFilePath()))
		{
			LOG_QCRIT << "could not remove empty source:" << info.absoluteFilePath();
			return FVA_ERROR_CANT_MOVE_DIR;
		}
		else if (info.isDir())
			LOG_QDEB << "removed empty destination:" << info.absoluteFilePath();
	}
	return FVA_NO_ERROR;
}