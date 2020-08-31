#include "CmdLineTasks.h"
#include "fvadefaultcfg.h"
#include "fvacommoncsv.h"
#include "fvacommonexif.h"

#include <QtCore/QCoreApplication>

#include <iostream>
#include <iomanip>

#include <QtCore/QTextStream>
#include <QtCore/QDateTime>


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
