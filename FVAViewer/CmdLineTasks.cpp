#include "CmdLineTasks.h"
#include "qexifimageheader.h"
#include "RiffParser.h"

#include <QDir>
#include <QDebug>
#include <QDateTime>


CmdLineBaseTask::CmdLineBaseTask( const QString& argument_ )
	: argument ( argument_ )
{

}

CmdLineBaseTask::~CmdLineBaseTask( )
{

}

std::auto_ptr<CmdLineBaseTask> CmdLineBaseTask::createTaskByType( const QString& type, const QString& arg )
{
	std::auto_ptr<CmdLineBaseTask> result( 0 );

	if ( type == "folderStructCreate" )
		result.reset( new CmdLineTask_FolderStructCreate				( arg ) );	
	else if ( type == "filesRename" )
		result.reset( new CmdLineTask_FilesRename						( arg ) );	
	else if ( type == "recursiveFilesRename" )
		result.reset( new CmdLineTask_FilesRenameRecursive				( arg ) );	 
	else if ( type == "recursiveDeviceNameCheck" )
		result.reset( new CmdLineTask_CheckDeviceNameRecursive			( arg ) );
	else if ( type == "recursiveFilesRenameByFolder" )
		result.reset( new CmdLineTask_RenameFileByFolderNameRecursive	( arg ) );
	else if ( type == "recursiveVideoRenameBySequence" )
		result.reset( new CmdLineTask_RenameVideoBySequenceRecursive	( arg ) );

	return result;
}

FILE_TYPE CmdLineBaseTask::convertFileExt2FileType ( const QString& type )
{
	if ( type == "JPG" || type == "JPEG" || type == "PNG" || type == "BMP" || type == "GIF" )
		return FILE_TYPE_IMG;
	
	if ( type == "AVI" ||	type == "MOV" || type == "MPG" )
		return FILE_TYPE_VIDEO;

	return FILE_TYPE_UNKNOWN;
}

CmdLineTask_FolderStructCreate::CmdLineTask_FolderStructCreate( const QString& argument )
	: CmdLineBaseTask( argument )
{
	qDebug() << "[CMD_FOLDER_CREATE]cmd created, argument:" << argument;
}

int CmdLineTask_FolderStructCreate::execute()
{
	QDir folder ( argument );

	if ( !folder.exists( argument ) )
	{
		qCritical() << "[CMD_FOLDER_CREATE]input folder does not exist";
		return 1002;
	}

	Q_FOREACH(QFileInfo info, folder.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		// skip meta file
		if ( "folderDescription"== info.baseName() )
			continue;

		QString subFolderName		= info.baseName().mid(0,10);
		QString fullSubFolderpath	=  argument +  subFolderName;
			
		if ( !folder.exists( subFolderName ) )
		{
			folder.mkdir( subFolderName );
			qDebug() << "[CMD_FOLDER_CREATE]sub-folder:" << subFolderName << " created";

			// copy folder description
			QFile::copy( argument + "folderDescription.json" ,  fullSubFolderpath + "/folderDescription.json" );
		}
		// move the file
		if ( !folder.rename( info.absoluteFilePath(), fullSubFolderpath + "/" + info.fileName() ) )
		{
			qCritical() << "[CMD_FOLDER_CREATE]can not rename file:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
			return 1003;
		}
		else
			qDebug() <<  "[CMD_FOLDER_CREATE]file renamed:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
	}

	return 0;
}
CmdLineTask_FilesRename::CmdLineTask_FilesRename( const QString& argument )
	: CmdLineBaseTask( argument )
{
	qDebug() << "[CMD_FILES_RENAME]cmd created, argument:" << argument ;
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
		qWarning() << "[CMD_FILES_RENAME]file cintains _1:" << fileToCheck.absoluteFilePath() << ", rename time to use:" << prevRenameDateTime.addSecs( 1 ).toString( "yyyy-MM-dd-hh-mm-ss" );
		renameDateTime = prevRenameDateTime.addSecs ( 1 );
	}
	else if ( fileToCheck.baseName().contains("_2") )
	{
		qWarning() << "[CMD_FILES_RENAME]file cintains _2:" << fileToCheck.absoluteFilePath() << ", rename time to use:" << prevRenameDateTime.addSecs( 2 ).toString( "yyyy-MM-dd-hh-mm-ss" );
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
			qWarning() << "[CMD_FILES_RENAME]modification time to use for:" << info.absoluteFilePath() << ",time:" << info.lastModified().toString("yyyy-MM-dd-hh-mm-ss");
			renameDateTime = info.lastModified();
		}
	}				
}
int CmdLineTask_FilesRename::execute()
{
	QDir dir( argument, "" );

	if ( !dir.exists( argument ) )
	{
		qCritical() << "[CMD_FILES_RENAME]input folder does not exist";
		return 1002;
	}

	QFileInfoList files = dir.entryInfoList(QDir::Files);
	QFileInfo info;

	QDateTime renameDateTime, prevRenameDateTime;

	Q_FOREACH( info, files )
	{
		renameDateTime = QDateTime();

		// if it is picture files
		QString suffix = info.suffix().toUpper();
		if(	FILE_TYPE_IMG == convertFileExt2FileType( suffix ) )
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
				//renameDateTime = renameDateTime.addSecs( 58260 );
				*/
				
				QString _newName = renameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
				if ( _newName.isEmpty() )
					fillRenameDateTimeFromLastModifiedIfValid( dir, info, renameDateTime );				
			}
		}
		// if it video file
		else if( FILE_TYPE_VIDEO == convertFileExt2FileType( suffix ) )
		{
			RiffParser riffInfo;
			QString createdDate, error;
			if ( !riffInfo.open( info.absoluteFilePath(), error ) || !riffInfo.findTag( "IDIT", createdDate ) || !riffInfo.convertToDate( createdDate, renameDateTime ) )
			{
				qWarning() << "[CMD_FILES_RENAME]can not get created time from:" << info.absoluteFilePath() << ",error:" << error;
				fillRenameDateTimeFromLastModifiedIfValid( dir, info, renameDateTime );			
			}	
		}
		else if( suffix == "WAV" )		
			fillRenameDateTimeFromLastModifiedIfValid( dir, info, renameDateTime );	
		else
		{
			qWarning() << "[CMD_FILES_RENAME]unsupported file type:" << info.absoluteFilePath() ;
			continue;
		}
		QString newName = renameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
		if ( newName.isEmpty() )
		{
			qWarning() << "[CMD_FILES_RENAME]no time in img file:" << info.absoluteFilePath() << ",prev time to use:" << prevRenameDateTime.addSecs( 1 ).toString("yyyy-MM-dd-hh-mm-ss");
			prevRenameDateTime	= prevRenameDateTime.addSecs( 1 );
			newName				= prevRenameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
		}
		else			
			prevRenameDateTime	= renameDateTime;

		if ( newName.isEmpty() )
		{
			qCritical() << "[CMD_FILES_RENAME]sequence error for file:" << info.absoluteFilePath();
			return 1004;
		}

		QString newPath = dir.path() + "/" + newName + "." + info.suffix();
		if ( info.absoluteFilePath() == newPath )
		{
			qWarning() << "[CMD_FILES_RENAME]File already has the same name:" << info.absoluteFilePath();
			continue;
		}
		
		if ( dir.exists( newPath ) )
		{
			qWarning() << "[CMD_FILES_RENAME]file already exists:" << newPath << ",old path:" << info.absoluteFilePath();
			if ( !dir.exists( dir.path() + "/copy" ) )
			{
				qDebug() << "[CMD_FILES_RENAME]created COPY folder:" << dir.path() + "/copy";
				dir.mkdir( dir.path() + "/copy" );
			}
			QString newCopyPath = dir.path() + "/copy/" + newName + "." + info.suffix();
			if ( !dir.rename( info.absoluteFilePath(), newCopyPath ) )
			{
				qCritical() << "[CMD_FILES_RENAME]can not rename file:" << info.absoluteFilePath() << " to:" << newCopyPath;
				return 1005;
			}
			else
				qDebug() << "[CMD_FILES_RENAME]renamed file:" << info.absoluteFilePath() << " to:" << newCopyPath;
				
			continue;
		}

		if ( !dir.rename( info.absoluteFilePath(), newPath ) )
			qCritical() << "[CMD_FILES_RENAME]can not rename file:" << info.absoluteFilePath() << " to:" << newPath;
		else
			qDebug() << "[CMD_FILES_RENAME]Renamed:" << info.absoluteFilePath() << " to:"<< newPath;
	}
	return 0;
};
CmdLineTask_FilesRenameRecursive::CmdLineTask_FilesRenameRecursive ( const QString& argument )
	: CmdLineBaseTask( argument )
{
	qDebug() << "[CMD_FILES_RENAME_RECURSIVE]cmd created, argument:" << argument ;
}

int CmdLineTask_FilesRenameRecursive::execute()
{
	QDir dir( argument );

	if ( !dir.exists( argument ) )
	{
		qCritical() << "[CMD_FILES_RENAME_RECURSIVE]input folder does not exist";
		return 1002;
	}

	Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
		{
			std::auto_ptr<CmdLineBaseTask> folderTask = CmdLineBaseTask::createTaskByType( "recursiveFilesRename", info.filePath() );
			folderTask->execute();
			continue;
		}
	}
	std::auto_ptr<CmdLineBaseTask> task = CmdLineBaseTask::createTaskByType( "filesRename", argument );
	task->execute();

	return 0;
}

CmdLineTask_CheckDeviceNameRecursive::CmdLineTask_CheckDeviceNameRecursive( const QString& argument )
	: CmdLineBaseTask( argument )
{
	qDebug() << "[CMD_DEVICE_NAME_CHECK_RECURSIVE]cmd created, argument:" << argument ;
}

int CmdLineTask_CheckDeviceNameRecursive::execute()
{
	QDir dir( argument );

	if ( !dir.exists( argument ) )
	{
		qCritical() << "[CMD_DEVICE_NAME_CHECK_RECURSIVE]input folder does not exist";
		return 1002;
	}

	QString deviceName;
	Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
		{
			std::auto_ptr<CmdLineBaseTask> folderTask = CmdLineBaseTask::createTaskByType( "recursiveDeviceNameCheck", info.filePath() );
			folderTask->execute();
			continue;
		}
		else
		{
			// if it is picture files
			QString suffix = info.suffix().toUpper();
			QString fullname = info.absoluteFilePath();
			if(	FILE_TYPE_IMG == convertFileExt2FileType ( suffix ) )
			{
				QString newDeviceName = QExifImageHeader( info.filePath()).value(QExifImageHeader::Make).toString()
										+ QExifImageHeader( info.filePath()).value(QExifImageHeader::Model).toString();
				if ( newDeviceName.isEmpty() )
					qDebug() << "[CMD_DEVICE_NAME_CHECK_RECURSIVE] no device name in picture:" << info.absoluteFilePath();
				else
				{
					if ( deviceName.isEmpty() )
						qDebug() << "[CMD_DEVICE_NAME_CHECK_RECURSIVE] got first device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath();
					else if ( deviceName != newDeviceName )	
						qWarning() << "[CMD_DEVICE_NAME_CHECK_RECURSIVE] got new device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath() << "old: " << deviceName;
						
					deviceName = newDeviceName; 
				}
			}
			else
				qDebug() << "[CMD_DEVICE_NAME_CHECK_RECURSIVE] no device name in:" << info.absoluteFilePath();
		}
	}

	return 0;
};

CmdLineTask_RenameFileByFolderNameRecursive::CmdLineTask_RenameFileByFolderNameRecursive( const QString& argument )
: CmdLineBaseTask( argument )
{
	qDebug() << "[CMD_RENAME_FILES_BY_FOLDER_NAME_RECURSIVE]cmd created, argument:" << argument ;
}

int CmdLineTask_RenameFileByFolderNameRecursive::execute()
{
	QDir dir( argument );

	if ( !dir.exists( argument ) )
	{
		qCritical() << "[CMD_RENAME_FILES_BY_FOLDER_NAME_RECURSIVE]input folder does not exist";
		return 1002;
	}

	QString deviceName;
	int id = 0;
	Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
		{
			std::auto_ptr<CmdLineBaseTask> folderTask = CmdLineBaseTask::createTaskByType( "recursiveFilesRenameByFolder", info.filePath() );
			folderTask->execute();
			continue;
		}
		else
		{
			QString suffix = info.suffix().toUpper();
			if(	FILE_TYPE_IMG == convertFileExt2FileType ( suffix ) )
			{				
				QString newPath = dir.path() + "/" + dir.dirName().replace(".","-").mid(0,10) + "-##-##-" + QString::number( id ) + "." + info.suffix();
				if ( !dir.rename( info.absoluteFilePath(), newPath ) )
				{
					qCritical() << "[CMD_RENAME_FILES_BY_FOLDER_NAME_RECURSIVE]can not rename file:" << info.absoluteFilePath() << " to:" << newPath;
					return 1005;
				}
				else
				{
					qDebug() << "[CMD_RENAME_FILES_BY_FOLDER_NAME_RECURSIVE]renamed file:" << info.absoluteFilePath() << " to:" << newPath;
					id++;
				}
			}
		}	
	}

	return 0;
}

CmdLineTask_RenameVideoBySequenceRecursive::CmdLineTask_RenameVideoBySequenceRecursive( const QString& argument )
: CmdLineBaseTask( argument )
{
	qDebug() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]cmd created, argument:" << argument;
}

int CmdLineTask_RenameVideoBySequenceRecursive::execute()
{
	QDir dir( argument );

	if ( !dir.exists( argument ) )
	{
		qCritical() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]input folder does not exist";
		return 1002;
	}

	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
		{
			std::auto_ptr<CmdLineBaseTask> folderTask = CmdLineBaseTask::createTaskByType( "recursiveVideoRenameBySequence", info.filePath() );
			folderTask->execute();
			continue;
		}

		QString suffix = info.suffix().toUpper();
		if(	FILE_TYPE_IMG == convertFileExt2FileType ( suffix ) && imageFilePrefix.isEmpty() )
		{				
			if ( !info.baseName().contains("_") )
				continue;
			int index		= info.baseName().indexOf("_");
			imageFilePrefix	= info.baseName().mid(0, index );
			qDebug() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]got new image prefix:" << imageFilePrefix;
		}
		else if( FILE_TYPE_VIDEO == convertFileExt2FileType ( suffix ) )
		{	
			if ( !info.baseName().contains("_") )
			{
				qWarning() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]video file does not contain _:" << info.absoluteFilePath();
				continue;
			}
			int index				= info.baseName().indexOf("_");
			QString videoFilePrefix	= info.baseName().mid(0, index );
			if ( imageFilePrefix.isEmpty() )
			{
				qWarning() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]still empty image prefix for path:" << info.absoluteFilePath();
				continue;
			}

			QString newFilePath	= info.absoluteFilePath().replace( videoFilePrefix, imageFilePrefix ) ;

			if ( !dir.rename( info.absoluteFilePath(), newFilePath ) )
			{
				qCritical() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]can not rename file:" << info.absoluteFilePath() << " to:" << newFilePath;
				return 1005;
			}
			else
				qDebug() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]renamed file:" << info.absoluteFilePath() << " to:" << newFilePath;

		}
		else if ( FILE_TYPE_UNKNOWN == convertFileExt2FileType ( suffix ) )
		{
			qWarning() << "[CMD_RENAME_VIDEO_BY_SEQUENCE_RECURSIVE]unsupported file type:" << info.absoluteFilePath() ;
			continue;
		}
	}
	return 0;
}
