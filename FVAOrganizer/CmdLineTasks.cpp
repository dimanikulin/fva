#include "CmdLineTasks.h"

#include "RiffParser.h"

#include "../lib/qexifimageheader.h"

#ifdef MEDIAINFO_LIBRARY
    #include "../lib/MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
    #define MediaInfoNameSpace MediaInfoLib;
#else //MEDIAINFO_LIBRARY
    #include "../lib/MediaInfo/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
    #define MediaInfoNameSpace MediaInfoDLL;
#endif //MEDIAINFO_LIBRARY

#include <iostream>
#include <iomanip>

using namespace MediaInfoNameSpace;

#ifdef __MINGW32__
    #ifdef _UNICODE
        #define _itot _itow
    #else //_UNICODE
        #define _itot itoa
    #endif //_UNICODE
#endif //__MINGW32

#include <QTextStream>
#include <QDateTime>

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
FVA_ERROR_CODE CLT_Dir_Struct_Create_By_Device_Name::execute()
{
	QString deviceName;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		// if it is picture files
		QString suffix = info.suffix().toUpper();
		QString fullname = info.absoluteFilePath();
		if(	FVA_FILE_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) )
		{
			QString newDeviceName = QExifImageHeader( info.filePath()).value(QExifImageHeader::Make).toString()
									+ QExifImageHeader( info.filePath()).value(QExifImageHeader::Model).toString();
			
			while(newDeviceName.endsWith( '\n' )) 
				newDeviceName.chop( 1 );

			while(newDeviceName.endsWith( '\r' ))
				newDeviceName.chop( 1 );

			while(newDeviceName.endsWith( QChar( '\0' )))
				newDeviceName.chop( 1 );

			newDeviceName = newDeviceName.trimmed();
				
			QString dirName;
			if ( newDeviceName.isEmpty() )
			{
				LOG_QDEB << "no device name in picture:" << info.absoluteFilePath();
				dirName = "EMPTY";
			}
			else
			{
				if ( deviceName.isEmpty() )
					LOG_QDEB << "got first device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath();
				else if ( deviceName != newDeviceName )	
					LOG_QWARN << "got new device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath() << "old: " << deviceName;
						
				deviceName = newDeviceName;
				dirName = deviceName;
			}

			QString fullSubFolderpath	=  m_folder + "/" + dirName;

			if ( !m_dir.exists( dirName ) )
			{
				m_dir.mkdir( dirName );
				LOG_QDEB << "sub-folder:" << dirName << " created";
			}
			// move the file
			if ( !m_dir.rename( info.absoluteFilePath(), fullSubFolderpath + "/" + info.fileName() ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "file renamed:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
		}
		else
			LOG_QDEB << " no device name in:" << info.absoluteFilePath();
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
			// m_custom must be device id
			QString jsonData = 
				"{\"deviceId\":\"" 
				+ m_custom 
				+ "\",\n\"whoTookFotoId\":\"\",\n\"tags\":\"\",\n\"people\":\"\",\n\"place\":\"\",\n\"event\":\"\"}";
			FVA_ERROR_CODE res = fvaCreateFolderDescription( m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME, jsonData, error );
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
		if ( fvaIsInternalFileName( info.fileName() ) )
			continue;

		QString subFolderName		= info.baseName().mid(0,10).replace("-",".");
		QString fullSubFolderpath	= m_folder + "/" + subFolderName;
		fullSubFolderpath			= fullSubFolderpath.replace("-",".");
			
		if ( !m_dir.exists( subFolderName ) )
		{
			if ( !m_readOnly )
				m_dir.mkdir( subFolderName );
			LOG_QDEB << "sub-folder:" << subFolderName << " created";

			// copy folder description
			if ( !m_readOnly )
			{
				if ( QFile::copy( m_folder + "\\" + FVA_DIR_DESCRIPTION_FILE_NAME , fullSubFolderpath + "\\"+ FVA_DIR_DESCRIPTION_FILE_NAME ) )
					LOG_QDEB << FVA_DIR_DESCRIPTION_FILE_NAME << " is copied to " << subFolderName ;
				else
				{
					LOG_QCRIT << FVA_DIR_DESCRIPTION_FILE_NAME << " is NOT copied to " << subFolderName ;
					DWORD err = GetLastError();
					return FVA_ERROR_CANT_FIND_DIR_DESC;
				}
			}
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
		if(	FVA_FILE_TYPE_IMG == fvaConvertFileExt2FileType( suffix ) )
		{
			if ( !checkIfParentFileExist( info, renameDateTime, prevRenameDateTime ) ) 
			{
				renameDateTime = QExifImageHeader( info.filePath()).value(QExifImageHeader::DateTimeOriginal).toDateTime();
				/*
				for this list time was not correct (was several hours before)
				
				2007.02.17 на квартире
				2007.02.18 Аквариум в квартире
				2007.02.24 день защитника отечества МИ
				2007.02.25 Женя ДР
				2007.03.01-04.07 Командировка на Мальту
				2007.04.08 Пасха у Ромки
				2007.04.09 Пасха у Димыных родителей
				2007.04.09 Пасха у Оксаныных родителей
				2007.05.27 Клебан-бык
				2007.05.28 Майские на квартире
				2007.06.03 Дима маленький ДР
				2007.06.29 на квартире
				2007.07.01 Ищенко ДР
				2007.07.05 На квартире

				I need to change it by
				renameDateTime = renameDateTime.addSecs( 58260 );
				*/
				QString _newName = renameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
				if ( _newName.isEmpty() )
					fillRenameDateTimeFromLastModifiedIfValid( m_dir, info, renameDateTime );				
			}
		}
		// if it video file
		else if( FVA_FILE_TYPE_VIDEO == fvaConvertFileExt2FileType( suffix ) )
		{
			RiffParser riffInfo;
			QString createdDate, error;
			if ( !riffInfo.open( info.absoluteFilePath(), error ) || !riffInfo.findTag( "IDIT", createdDate ) || !riffInfo.convertToDate( createdDate, renameDateTime ) )
			{
				MediaInfo MI;	
				MI.Open(  info.absoluteFilePath().toStdWString().c_str() );
				String EncodedDate = MI.Get( Stream_General, 0, __T("Encoded_Date") );
				if ( !EncodedDate.empty() )
				{
					if ( !riffInfo.convertToDate( QString::fromStdWString ( EncodedDate ), renameDateTime ) )
					{
						LOG_QWARN << "can not get created time from:" << info.absoluteFilePath() << ",error:" << error;
						fillRenameDateTimeFromLastModifiedIfValid( m_dir, info, renameDateTime );
					}
				}
			}
		}
		else if( suffix == "WAV" )		
			fillRenameDateTimeFromLastModifiedIfValid( m_dir, info, renameDateTime );	
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
			if ( !m_dir.exists( m_dir.path() + "/copy" ) )
			{
				LOG_QWARN << "created COPY folder:" << m_dir.path() + "/copy";
				m_dir.mkdir( m_dir.path() + "/copy" );
			}
			QString newCopyPath = m_dir.path() + "/copy/" + newName + "." + info.suffix();
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
		if(	FVA_FILE_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) )
		{
			QString newDeviceName = QExifImageHeader( info.filePath()).value(QExifImageHeader::Make).toString()
									+ QExifImageHeader( info.filePath()).value(QExifImageHeader::Model).toString();
			if ( newDeviceName.isEmpty() )
				LOG_QDEB << "no device name in picture:" << info.absoluteFilePath();
			else
			{
				if ( deviceName.isEmpty() )
					LOG_QDEB << "got first device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath();
				else if ( deviceName != newDeviceName )	
					LOG_QWARN << "got new device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath() << "old: " << deviceName;
						
				deviceName = newDeviceName; 
			}
		}
		else
			LOG_QDEB << "no device name in:" << info.absoluteFilePath();
	}

	return FVA_NO_ERROR;
};
FVA_ERROR_CODE CLT_Files_Rename_By_Dir::execute()
{
	QString deviceName;
	int id = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		QString suffix = info.suffix().toUpper();
		if(	FVA_FILE_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) )
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
		if(	FVA_FILE_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) && imageFilePrefix.isEmpty() )
		{				
			if ( !info.baseName().contains("_") )
				continue;
			int index		= info.baseName().indexOf("_");
			imageFilePrefix	= info.baseName().mid(0, index );
			LOG_QDEB << "got new image prefix:" << imageFilePrefix;
		}
		else if( FVA_FILE_TYPE_VIDEO == fvaConvertFileExt2FileType ( suffix ) )
		{	
			if ( !info.baseName().contains("_") )
			{
				LOG_QWARN << "video file does not contain _:" << info.absoluteFilePath();
				continue;
			}
			int index				= info.baseName().indexOf("_");
			QString videoFilePrefix	= info.baseName().mid(0, index );
			if ( imageFilePrefix.isEmpty() )
			{
				LOG_QWARN << "still empty image prefix for path:" << info.absoluteFilePath();
				continue;
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
		else if ( FVA_FILE_TYPE_UNKNOWN == fvaConvertFileExt2FileType ( suffix ) )
		{
			LOG_QWARN << "unsupported file type:" << info.absoluteFilePath() ;
			continue;
		}
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Auto_Checks_2::execute()
{
	QMap<QString, unsigned int> fileCount;
	unsigned int countSupportedFiles = 0; 
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{		
		// 1.CHECK FOR PROPER FOLDER NAME (NO COPY FOLDER)
		if ( info.isDir() )
		{	
			QDateTime from, to;
			if ( FVA_NO_ERROR != fvaParseDirName(info.fileName(), from, to))
				LOG_QCRIT << "wrong folder name:" << info.absoluteFilePath();
			continue;
		}

		// 2.CHECK FOR PROPER FILE NAME AND SUPPORTED TYPE
		QString suffix = info.suffix().toUpper();
		FVA_FILE_TYPE type = fvaConvertFileExt2FileType ( suffix );
		if ( FVA_FILE_TYPE_UNKNOWN != type )
		{
			QDateTime date;
			QString baseFileName = info.baseName();
			if ( FVA_NO_ERROR != fvaParseFileName(info.baseName(), date))
			{
				LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath();
				continue;
			}
			//////////////////////////////////// 2. MATCHING FILE NAME AND FOLDER NAME ////////////////////////////////////////////////////
			QString dirDate = m_dir.dirName().mid(0,10);
			QDateTime dateStart = QDateTime::fromString( dirDate, "yyyy.MM.dd" );		
			if ( dateStart.isValid() )
			{
				QDateTime dateEnd = dateStart.addDays( 1 );
				if ( m_dir.dirName().size() > 11 && ( m_dir.dirName() [ 10 ]  == '-' ) ) // period
				{
					QString sEndDate = m_dir.dirName().mid( 11, m_dir.dirName().size() - 11 );
					bool result = false; 
					int dEndDate = sEndDate.toInt( &result );
					if ( !result || !dEndDate )
					{
						LOG_QCRIT << "wrong folder name:" << info.absoluteFilePath();
						continue;
					}
					dateEnd = dateStart.addDays( dEndDate - dateStart.date().day() + 1 );
				}
				
				QDateTime fileDateTime = QDateTime::fromString( baseFileName , "yyyy-MM-dd-hh-mm-ss" );
				QString newFileName = baseFileName.replace( "#","0" );
				if ( !fileDateTime.isValid() )
					fileDateTime = QDateTime::fromString( newFileName , "yyyy-MM-dd-hh-mm-ss" );
				if ( ( fileDateTime < dateStart ) ||( fileDateTime > dateEnd ) )
				{
					LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath() << " data period=" << dateStart << ";" << dateEnd;
					continue;
				}
				countSupportedFiles++;
			}	
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else if ( fvaIsInternalFileName ( info.fileName() ) ) 
		{
			// nothing to do here
		}
		else
			LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath();
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	m_fileCount[ m_folder ] = countSupportedFiles;
	if ( countSupportedFiles < FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR && countSupportedFiles )
		LOG_QCRIT << "too little supported files found in:" << m_folder;

	QVariantMap result;
	QString error;
	FVA_ERROR_CODE code = fvaGetFolderDescription( m_folder, result, error );
	if ( FVA_NO_ERROR != code )
		LOG_QCRIT << error;
	else
	{
		// TODO check for existing deviceid, place, people, whoTookFotoId, in dictionary
		QString place	=	result["place"].toString();
		QString people	= 	result["people"].toString();
		QString device	= 	result["deviceId"].toString();
		if ( device.isEmpty() )
			LOG_QCRIT << "deviceID is empty in folder desc in:" << m_folder;
		QString whoTook	= 	result["whoTookFotoId"].toString();
		QString event	= 	result["event"].toString();
		QString tags	= 	result["tags"].toString();
	}
/*
#06.FolderDecriptionValid
#09.decsriptionFileValid
# check for exsiting device in dictionary by device name in pictire 
*/
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
		FVA_FILE_TYPE type = fvaConvertFileExt2FileType ( suffix );
		if ( FVA_FILE_TYPE_UNKNOWN != type )
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
	// Name,Place,People,Device,WhoTook,Description,Scaner,Comment,oldName
	QString oneFileDesc =  "," 
			+ result["place"].toString()			+ ","
			+ result["people"].toString()			+ ","
			+ result["deviceId"].toString()			+ ","
			+ result["whoTookFotoId"].toString()	+ ","
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
			*pTextStream.get() << "Name,Place,People,Device,WhoTook,Description,Scaner,Comment,oldName\n";
			m_custom = "FIRST";
		}
	}
	m_dir = QDir (m_folder);
	
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{		
		if ( info.isDir() )
			continue;

		// skip meta files
		if ( fvaIsInternalFileName( info.fileName() ) )
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
		if ( !m_dir.remove( descFolderPath ) )
		{
			LOG_QCRIT << "can not remove description for folder in folder:" << m_folder;
			return FVA_ERROR_CANT_REMOVE_FILE_OR_DIR;
		}
		if ( !m_dir.remove( descFolderPath + "_old" ) )
		{
			LOG_QCRIT << "can not remove old description for folder in folder:" << m_folder;
			// return FVA_ERROR_CANT_REMOVE_FILE_OR_DIR;
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