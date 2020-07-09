#include "CmdLineTasks.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QCryptographicHash>

#include "../lib/qexifimageheader.h"

#include <iostream>
#include <iomanip>

#include <QTextStream>
#include <QDateTime>

#include <windows.h>
#include <winbase.h>

FVA_ERROR_CODE CLT_Dir_Name_Change::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
		{
			QString subFolderName		= info.baseName().mid(0,10);
			info.absoluteDir();
			QDir subdir ( info.absoluteDir() );

			QString oldPath = info.absoluteFilePath();
			QString newPath = info.absoluteFilePath().replace("-",".");
			if ( subdir.rename( oldPath, newPath ) )
				LOG_QWARN << "renamed folder from " << oldPath << " to " << newPath;
			else
				LOG_QCRIT << "cannot rename folder from " << oldPath << " to " << newPath;			
		}
		else
			continue;			
	}

	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Dir_Struct_Create_By_File::execute()
{
	// check for existing folder description
	if ( !m_dir.exists( m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME ))
	{
		if ( m_custom.isEmpty() )
		{
			LOG_QCRIT << FVA_DIR_DESCRIPTION_FILE_NAME << "does not exist in" << m_folder ;
			return FVA_ERROR_CANT_FIND_DIR_DESC;
		}
		// create folder description
		if ( !m_readOnly )
		{
			QString error;
			
			// TODO to remove QString jsonData = "{\"deviceId\":\"" + m_custom + "\"}";
			QVariantMap content;
			content["deviceId"] = m_custom; // m_custom must be device id
			FVA_ERROR_CODE res = fvaCreateFolderDescription( m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME, content, error );
			if ( FVA_NO_ERROR != res )
			{
				LOG_QCRIT << error;
				return res;
			}
		}
	}

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

			// TODO clean up
			// copy folder description
			//if ( !m_readOnly )
			//{
				//if ( QFile::copy( m_folder + "\\" + FVA_DIR_DESCRIPTION_FILE_NAME , fullSubFolderpath + "\\"+ FVA_DIR_DESCRIPTION_FILE_NAME ) )
					//LOG_QDEB << FVA_DIR_DESCRIPTION_FILE_NAME << " is copied to " << subFolderName ;
				//else
				//{
					//LOG_QCRIT << FVA_DIR_DESCRIPTION_FILE_NAME << " is NOT copied to " << subFolderName << ", error =" << GetLastError();
					//return FVA_ERROR_CANT_FIND_DIR_DESC;
				//}
			//}
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
	// check for existing folder description
	if ( m_dir.exists(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
	{
		if (!QFile::remove(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
			// it is not critical error
			LOG_QDEB << "file could not be removed:" << m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
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
		qWarning() << "[ \"CLT_FILES_RENAME\" ]file cintains _1:" << fileToCheck.absoluteFilePath() << ", rename time to use:" << prevRenameDateTime.addSecs( 1 ).toString( "yyyy-MM-dd-hh-mm-ss" );
		renameDateTime = prevRenameDateTime.addSecs ( 1 );
	}
	else if ( fileToCheck.baseName().contains("_2") )
	{
		qWarning() << "[ \"CLT_FILES_RENAME\" ]file cintains _2:" << fileToCheck.absoluteFilePath() << ", rename time to use:" << prevRenameDateTime.addSecs( 2 ).toString( "yyyy-MM-dd-hh-mm-ss" );
		renameDateTime = prevRenameDateTime.addSecs ( 2 );	
	}
	return false;
}
void fillRenameDateTimeFromLastModifiedIfValid( const QDir& dir, const QFileInfo& info, QDateTime& renameDateTime )
{
	QString dirDate = dir.dirName().mid(0,10);
	QDateTime validDateStart = QDateTime::fromString( dirDate, "yyyy.MM.dd" );
				
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
				QString _newName = renameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
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
		QString newName = renameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
		if ( newName.isEmpty() )
		{
			LOG_QWARN << "no time in img file:" << info.absoluteFilePath() << ",prev time to use:" << prevRenameDateTime.addSecs( 1 ).toString("yyyy-MM-dd-hh-mm-ss");
			prevRenameDateTime	= prevRenameDateTime.addSecs( 1 );
			newName				= prevRenameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
		}
		else			
			prevRenameDateTime	= renameDateTime;

		if ( newName.isEmpty() )
		{
			LOG_QCRIT << "sequence error for file:" << info.absoluteFilePath();
			return FVA_ERROR_SEQUENCE;
		}

		QString newPath = "";
		if ( OLD_FILE_NAME_PARAM == m_custom )
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
FVA_ERROR_CODE CLT_Files_Rename_By_Dir::execute()
{
	int id = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		QString suffix = info.suffix().toUpper();
		if(	FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) )
		{				
			QString newPath = m_dir.path() + "/" + m_dir.dirName().replace(".","-").mid(0,10) + "-##-##-" + QString::number( id ) + "." + info.suffix();
			if ( !m_dir.rename( info.absoluteFilePath(), newPath ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newPath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
			{
				LOG_QDEB << "renamed file:" << info.absoluteFilePath() << " to:" << newPath;
				id++;
			}
		}	
	}

	return FVA_NO_ERROR;
}
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
	FVA_ERROR_CODE res = fvaLoadDeviceMapFromDictionary(deviceMap, QCoreApplication::applicationDirPath() + "/" + FVA_DB_NAME);
	if ( FVA_NO_ERROR != res )
		return res;

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
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FILE_NAME;
			}
			//////////////////////////////////// 3. check for exsiting device in dictionary by device name in pictire 
			if (FVA_FS_TYPE_IMG == type)
			{
				QString deviceName;
				if ( !fvaGetDeviceMapForImg(deviceMap, info.filePath(),deviceName).size()) 
				{
					if(deviceName.isEmpty())
					{
						LOG_QWARN << "empty device found:" << deviceName.trimmed() << " in file :" << info.absoluteFilePath();
						countSupportedFiles++;	// it is our file
						continue;
					}
					LOG_QWARN << "unknown device found:" << deviceName.trimmed() << " in file :" << info.absoluteFilePath();
					if (m_readOnly)
						continue;
					else
						return FVA_ERROR_UKNOWN_DEVICE;
				}
			}

			////////////////////////////////// 4. check for matching taken time and file name//////////////////////////
			if (FVA_FS_TYPE_IMG == type)
			{
				QString error;
				QDateTime dateTime = QExifImageHeader( info.filePath()).value(QExifImageHeader::DateTimeOriginal).toDateTime();

				if (!dateTime.isValid())
				{
					LOG_QWARN << "empty image taken time found in:" << info.absoluteFilePath();
				}
				else
				{
					if (dateTime != date)
					{
						LOG_QWARN << "mismatching image taken time found in:" << info.absoluteFilePath();
					}
				}
			}
			if (FVA_FS_TYPE_VIDEO == type)
			{
				QString error;
				QDateTime dateTime = QExifImageHeader( info.filePath()).value(QExifImageHeader::DateTimeOriginal).toDateTime();
				if (!dateTime.isValid())
				{
					// LOG_QWARN << "empty video taken time found in:" << info.absoluteFilePath();
				}
			}

			//////////////////////////////////// 5. MATCHING FILE NAME AND FOLDER NAME ////////////////////////////////////////////////////
			QDateTime dateStart, dateEnd;
			if ( FVA_NO_ERROR != fvaParseDirName( m_dir.dirName(), dateStart, dateEnd))
			{
				// skip internal folder 
				if (m_dir.dirName()[0] == '#')
					continue;

				LOG_QCRIT << "wrong matchig folder name:" << info.absoluteFilePath();
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FOLDER_NAME;
			}
			if (dateStart == dateEnd)
				dateEnd = dateEnd.addYears(1);
			QDateTime fileDateTime = QDateTime::fromString( baseFileName , "yyyy-MM-dd-hh-mm-ss" );
			QString newFileName = baseFileName.replace( "##","01" );
			if ( !fileDateTime.isValid() )
				fileDateTime = QDateTime::fromString( newFileName , "yyyy-MM-dd-hh-mm-ss" );
			if ( ( fileDateTime < dateStart ) ||( fileDateTime > dateEnd ) )
			{
				LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath() << " data period=" << dateStart << ";" << dateEnd;
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
		}
		else
		{
			LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath();
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

	QString descFilePath = m_folder + "/" + FVA_DESCRIPTION_FILE_NAME;
	if ( m_dir.exists( descFilePath ) )
	{
		LOG_QCRIT << "description for files exists, skipping folder:" << m_folder;
		return FVA_NO_ERROR;
	}

	QVariantMap result;
	QString error;
	FVA_ERROR_CODE code = fvaGetFolderDescription( m_folder, result, error );
	if ( FVA_NO_ERROR != code )
	{
		LOG_QCRIT << error;
		return code;
	}
	// Name,Place,People,Device,Description,Scaner,Comment,oldName
	QString oneFileDesc =  "," 
			+ result["place"].toString()			+ ","
			+ result["people"].toString()			+ ","
			+ result["deviceId"].toString()			+ ","
			+ result["event"].toString()			+ ",,"
			+ result["tags"].toString()				+ ",\n";

	if ( result["deviceId"].toString().isEmpty() )
	{
		LOG_QCRIT << "deviceId is empty in:" << m_folder;
		return FVA_ERROR_EMPTY_DEV_ID;
	}

	std::auto_ptr <QFile>	pFile (nullptr);
	std::auto_ptr <QTextStream> pTextStream (nullptr); 
	m_dir.cdUp();
	QString folderUp = m_dir.absolutePath();
	if ( !m_readOnly )
	{
		pFile.reset (new QFile( folderUp + "/" + FVA_DESCRIPTION_FILE_NAME ));
		QFile::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append;
		if (!pFile->open(mode))
		{
			LOG_QCRIT << "cant open description file:" << folderUp + "/" + FVA_DESCRIPTION_FILE_NAME;
			return FVA_ERROR_CANT_OPEN_FILE_DESC;
		}
		pTextStream.reset ( new QTextStream(pFile.get()) );
		if ( m_custom != "FIRST" )
		{
			*pTextStream.get() << "Name,Place,People,Device,Description,Scaner,Comment,oldName\n";
			m_custom = "FIRST";
		}
	}
	m_dir = QDir (m_folder);
	
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{		
		if ( info.isDir() )
			continue;

		// skip meta files
		if ( fvaIsInternalFile( info.fileName() ) )
			continue;

		LOG_QWARN << "descr:" << info.fileName() + oneFileDesc;
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
			*pTextStream.get() << info.fileName() << oneFileDesc;
			pTextStream->flush();
		}
	}

	if ( !m_readOnly )
	{
		pFile->close();
		QString descFolderPath = m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;

		SetFileAttributes(descFolderPath.toStdWString().c_str(), FILE_ATTRIBUTE_NORMAL);

		if ( !m_dir.remove( descFolderPath ) )
		{
			LOG_QCRIT << "can not remove description for folder in folder:" << m_folder;
			return FVA_ERROR_CANT_REMOVE_FILE_OR_DIR;
		}
		m_dir.cdUp();
		if ( !m_dir.rmdir( m_folder ) )
		{
			LOG_QCRIT << "can not remove folder:" << m_folder;
			return FVA_ERROR_CANT_REMOVE_FILE_OR_DIR;
		}
		LOG_QDEB << "removed folder:" << m_folder;
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
			// TODO rename to next number
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
			// TODO make conversion
			LOG_QCRIT << "found not supported file:" << info.absoluteFilePath();
			return FVA_ERROR_NOT_SUPPORTED_FILE;
		}
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Folder_Merging::execute()
{
	// create folder structure the same as in source folder
	QString subFolder	= m_folder;
	subFolder.remove(m_baseFolder);

	if (!m_dir.exists(m_custom + subFolder + QDir::separator()))
	{
		// skip internal folder 
		if (!subFolder.contains("#copy"))
		{
			if (!m_dir.mkpath(m_custom + subFolder + QDir::separator()))
			{
				LOG_QCRIT << "could not create dest folder:" << m_custom + subFolder + QDir::separator();
				return FVA_ERROR_CANT_CREATE_DIR;
			}
		}
	}
	else
	{
		// TODO change folder renaming so it renamed new folder but not one to copy to 
		if (!subFolder.isEmpty())
		{
			if (m_dir.exists(m_custom + subFolder + " #1" + QDir::separator() ))
			{
				if( !m_dir.rename( m_custom + subFolder + " #1" + QDir::separator(), m_custom + subFolder + " #2" + QDir::separator() ) )
					LOG_QWARN << "could not rename source :" << m_custom + subFolder + " #1" + QDir::separator() << " into " << m_custom + subFolder + " #2" + QDir::separator();
				else
					LOG_QWARN << "renamed source :" << m_custom + subFolder + " #1/" << " into " << m_custom + subFolder + " #2";
			}

			if( !m_dir.rename( m_custom + subFolder, m_custom + subFolder + " #1" + QDir::separator() ) )
				LOG_QWARN << "could not rename source :" << m_custom + subFolder << " into " << m_custom + subFolder + " #1/";
			else
				LOG_QWARN << "renamed source :" << m_custom + subFolder << " into " << m_custom + subFolder + " #1/";
			
			// skip internal folder 
			if (!subFolder.contains("#copy"))	
				m_dir.mkpath(m_custom + subFolder + QDir::separator());
		}
	}

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{				
		QString original	= m_folder  + QDir::separator() + info.fileName();
		QString dest		= m_custom + subFolder + QDir::separator() + info.fileName();

		// skip internal folder 
		if (original.contains("#copy") || dest.contains("#copy"))
			continue;	
		
		if (FVA_DESCRIPTION_FILE_NAME == info.fileName())
		{
			QFile fileInput(info.absoluteFilePath()); 
			if (!fileInput.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				LOG_QCRIT << "could not open source file desc:" << original;
				return FVA_ERROR_CANT_OPEN_FILE_DESC;
			}
			if (m_dir.exists(dest))
			{
				if (!SetFileAttributes(dest.toStdWString().c_str(), FILE_ATTRIBUTE_NORMAL ))
				{	
					LOG_QCRIT << "can not set attr for dest desc file:" << dest;
					return FVA_ERROR_CANT_OPEN_FILE_DESC;
				}
			}
			QFile fileOutput(dest);
			if (!fileOutput.open(QIODevice::Append | QIODevice::Text))
			{
				LOG_QCRIT << "could not open dest file desc:" << original;
				return FVA_ERROR_CANT_OPEN_FILE_DESC;
			}
			{
				QTextStream in(&fileInput);
				QString line = in.readLine();
				bool isFirst = true;
				while (!line.isNull())
				{
					if (!isFirst)
					{
						fileOutput.write("\n");
						fileOutput.write(line.toStdString().c_str());
					}
					else
						isFirst = false;
					line = in.readLine();
				}
				in.flush();
			}
			fileOutput.close();
			fileInput.close();
			if (!SetFileAttributes(dest.toStdWString().c_str(), /*FILE_ATTRIBUTE_HIDDEN |*/ FILE_ATTRIBUTE_READONLY ))
			{	
				LOG_QCRIT << "can not set attr for dest desc file:" << dest;
				return FVA_ERROR_CANT_OPEN_FILE_DESC;
			}
			if (!QFile::remove(original))
			{
				LOG_QCRIT << "could not remove source file desc:" << original;
				return FVA_ERROR_CANT_MOVE_DIR;
			}
			LOG_QCRIT << "merged desc file:" << original << " into " << dest;
			continue;
		}
		if( !m_dir.rename( original, dest ) )
		{
			// TODO check if there is already file exists with the same check-sum
			
			if(QDir(original).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0)
			{
				// empty folder now - no need in it to keep
				if (!m_dir.rmdir(original))
				{
					LOG_QCRIT << "could not remove empty source:" << original;
					return FVA_ERROR_CANT_MOVE_DIR;
				}
				else
					continue;
			}
			
			LOG_QCRIT << "could not move:" << original << " into " << dest;
			return FVA_ERROR_CANT_MOVE_DIR;
		}
		LOG_QDEB << "merged:" << original << " into " << dest;
		continue;
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
CLT_Print_FS_Structure::CLT_Print_FS_Structure(const QString& dir_,bool readOnly_,const QString& custom_)
	:CmdLineBaseTask( dir_,readOnly_,custom_)
{
	qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;
	m_file.setFileName(QCoreApplication::applicationDirPath() + "\\fsoutput.txt");
	m_file.open( QIODevice::WriteOnly );
}

CLT_Print_FS_Structure::~CLT_Print_FS_Structure()
{
	m_file.close();
}

FVA_ERROR_CODE CLT_Print_FS_Structure::execute()
{
	char		buffer [ 64* 1024 ];
	qint64		size = 0;
	QCryptographicHash hash( QCryptographicHash::Sha1 );
	QString		result;

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		QFile file( info.absoluteFilePath() );

		if ( !file.open( QIODevice::ReadOnly ) ) 
		{
			LOG_QCRIT << "failed to open file:" << info.absoluteFilePath();
			continue;
		}
		while (!file.atEnd())
		{
			size	= file.read(buffer, 64 * 1024);
			if (size)
				hash.addData( buffer, size);
		}
		QFileInfo f(info);
		result =	info.filePath()										+ "," 
					+	hash.result().toBase64()						+ ","
					+	f.lastModified().toString("yyyy-MM-dd-hh-mm-ss")	+ ","
					+	QString::number(info.size())					+ "\n";
		m_file.write(result.toLocal8Bit());
	}
	return FVA_NO_ERROR;
}
