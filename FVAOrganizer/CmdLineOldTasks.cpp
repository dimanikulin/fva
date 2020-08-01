#include "CmdLineOldTasks.h"
#include "fvadefaultcfg.h"

#include "../FVACommonLib/qexifimageheader.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QCryptographicHash>

#include <windows.h>
#include <winbase.h>

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
		if(	FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) )
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
CLT_Print_FS_Structure::CLT_Print_FS_Structure(const QString& dir_,bool readOnly_,const QString& custom_)
	:CmdLineBaseTask( dir_,readOnly_,custom_)
{
	qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;
	m_file.setFileName(FVA_DEFAULT_ROOT_DIR + "fsoutput.txt");
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
FVA_ERROR_CODE CLT_Dir_Struct_Create_By_File_Old::execute()
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
			
			QVariantMap content;
			content["deviceId"] = m_custom; // m_custom must be device id
			/* FVA_ERROR_CODE res = fvaCreateFolderDescription( m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME, content, error );
			if ( FVA_NO_ERROR != res )
			{
				LOG_QCRIT << error;
				return res;
			}*/
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

			// copy folder description
			if ( !m_readOnly )
			{
				if ( QFile::copy( m_folder + "\\" + FVA_DIR_DESCRIPTION_FILE_NAME , fullSubFolderpath + "\\"+ FVA_DIR_DESCRIPTION_FILE_NAME ) )
					LOG_QDEB << FVA_DIR_DESCRIPTION_FILE_NAME << " is copied to " << subFolderName ;
				else
				{
					LOG_QCRIT << FVA_DIR_DESCRIPTION_FILE_NAME << " is NOT copied to " << subFolderName << ", error =" << GetLastError();
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
	// check for existing folder description
	if ( m_dir.exists(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
	{
		if (!QFile::remove(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
			// it is not critical error
			LOG_QDEB << "file could not be removed:" << m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
	}
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Alone_Files_Move_Old::execute()
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
	/*FVA_ERROR_CODE code = fvaGetFolderDescription( m_folder, result, error );
	if ( FVA_NO_ERROR != code )
	{
		LOG_QCRIT << error;
		return code;
	}*/
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
FVA_ERROR_CODE CLT_Folder_Merging_Old::execute()
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
