#include "CmdLineTasks.h"
#include "fvadefaultcfg.h"
#include "fvacommoncsv.h"
#include "fvacommonexif.h"

#include <QtCore/QCoreApplication>

#include <iostream>
#include <iomanip>

#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

#include <windows.h>
#include <winbase.h>

FVA_EXIT_CODE CLT_Dir_Struct_Create_By_File::execute()
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
			
		if (!m_dir.exists(subFolderName))
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


FVA_EXIT_CODE CLT_Device_Name_Check::execute()
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
		
		QString newDeviceName = fvaGetExifMakeAndModelFromFile(info.filePath());

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

FVA_EXIT_CODE CLT_Video_Rename_By_Sequence::execute()
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

			FVA_EXIT_CODE res = fvaParseFileName(info.baseName(), renameDateTime );
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
			if (imageFilePrefix.isEmpty())
			{
				if (FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true && info.lastModified().isValid())
				{					
					continue;
				}
				else
				{
					LOG_QCRIT << "still empty image prefix for path:" << info.absoluteFilePath();
					return FVA_ERROR_SEQUENCE;
				}
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


FVA_EXIT_CODE CLT_Alone_Files_Move::execute()
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
FVA_EXIT_CODE CLT_Auto_Checks_1::execute()
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
					if (FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true && info.lastModified().isValid())
					{
						LOG_QWARN << "skipping first video/audio file (FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true):" << info.absoluteFilePath();
						continue;
					}
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
FVA_EXIT_CODE CLT_Convert_Amr::execute()
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

FVA_EXIT_CODE CLT_Set_File_Atts::execute()
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
FVA_EXIT_CODE CLT_1_Event_Folder_Merging::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{				
		QString original	= m_folder + "/" + info.fileName();
		QString dest		= m_custom + "/" + info.fileName();

		// skip internal folder 
		if (original.contains("#copy") || dest.contains("#copy"))
			continue;

		// check for already existing
		if (m_dir.exists(dest))
		{
			LOG_QCRIT << "destination already exists: " << dest;
			return FVA_ERROR_DEST_ALREADY_EXISTS;
		}

		if( !m_dir.rename( original, dest ) )
		{						
			LOG_QCRIT << "could not move:" << original << " into " << dest;
			return FVA_ERROR_CANT_MOVE_DIR;
		}

		LOG_QDEB << "moved:" << original << " into " << dest;

		if (info.isDir() && !fvaRemoveDirIfEmpty(original))
		{
			LOG_QCRIT << "could not remove empty original:" << original;
			continue;
		}
		else
		{
			if (info.isDir())
				LOG_QDEB << "removed empty destination:" << dest;
		}
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE CLT_1_Day_Event_Folder_Merging::execute()
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
			else
			{
				LOG_QDEB << "created dest folder:" << dstDirPath;

			}
		}
	}
	else
		LOG_QWARN << "could not create dest folder as it already exists:" << dstDirPath;

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// skip internal folder 
		if (dir.contains("#copy") || dstDirPath.contains("#copy"))
		{
			LOG_QWARN << "skipped #copy for: " << info.absoluteFilePath() << " , dst: " << dstDirPath;
			continue;
		}

		// check for already existing
		if (m_dir.exists(dstDirPath + "/" + info.fileName()))
		{
			LOG_QCRIT << "destination already exists: " << dstDirPath + "/" + info.fileName();
			return FVA_ERROR_DEST_ALREADY_EXISTS;
		}

		// we move files only
		if (!info.isDir() && !m_dir.rename(info.absoluteFilePath(), dstDirPath + "/" + info.fileName()))
		{
			LOG_QCRIT << "could not move:" << info.absoluteFilePath() << " into " << dstDirPath + "/" + info.fileName();
			return FVA_ERROR_CANT_RENAME_FILE;
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

FVA_EXIT_CODE CLT_Get_Fva_Dir_Type::execute()
{
	uint dirNumber = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (!info.isDir())
			// skip not dir
			continue;

		// get the last dir leaf in input folder
		QString dir = m_dir.dirName();
		// skip internal folder 
		if ( dir.contains("#copy"))
		{
			LOG_QWARN << "skipped internal folder: " << info.absoluteFilePath();
			continue;
		}

		dirNumber++;
	}
	if (!dirNumber)
		return FVA_NOT_FVA_FOLDER;
	if (1 == dirNumber)
		return FVA_1_EVENT_1_DAY;
	if (dirNumber > 1)
		return FVA_FEW_EVENTS_FEW_DAYS;

	// TODO to impelemnt FVA_FEW_EVENTS_1_DAY
}