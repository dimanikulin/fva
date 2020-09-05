#include "CmdLineOldTasks.h"
#include "fvadefaultcfg.h"

#include "fvacommonexif.h"

#include <QtCore/QCoreApplication>

#include <windows.h>
#include <winbase.h>


FVA_EXIT_CODE CLT_Alone_Files_Move_Old::execute()
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
	/*FVA_EXIT_CODE code = fvaGetFolderDescription( m_folder, result, error );
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
FVA_EXIT_CODE CLT_Folder_Merging_Old::execute()
{
	// create folder structure the same as in source folder
	QString subFolder	= m_folder;
	subFolder.remove(m_baseFolder);

	if (!m_dir.exists(m_custom + subFolder + "/"))
	{
		// skip internal folder 
		if (!subFolder.contains("#copy"))
		{
			if (!m_dir.mkpath(m_custom + subFolder + "/"))
			{
				LOG_QCRIT << "could not create dest folder:" << m_custom + subFolder + "/";
				return FVA_ERROR_CANT_CREATE_DIR;
			}
		}
	}
	else
	{
		if (!subFolder.isEmpty())
		{
			if (m_dir.exists(m_custom + subFolder + " #1" + "/" ))
			{
				if (!m_dir.rename(m_custom + subFolder + " #1" + "/", m_custom + subFolder + " #2" + "/"))
					LOG_QWARN << "could not rename source :" << m_custom + subFolder + " #1" + "/" << " into " << m_custom + subFolder + " #2" + "/";
				else
					LOG_QWARN << "renamed source :" << m_custom + subFolder + " #1/" << " into " << m_custom + subFolder + " #2";
			}

			if( !m_dir.rename( m_custom + subFolder, m_custom + subFolder + " #1" + "/" ) )
				LOG_QWARN << "could not rename source :" << m_custom + subFolder << " into " << m_custom + subFolder + " #1/";
			else
				LOG_QWARN << "renamed source :" << m_custom + subFolder << " into " << m_custom + subFolder + " #1/";
			
			// skip internal folder 
			if (!subFolder.contains("#copy"))	
				m_dir.mkpath(m_custom + subFolder + "/");
		}
	}

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{				
		QString original	= m_folder  + "/" + info.fileName();
		QString dest		= m_custom + subFolder + "/" + info.fileName();

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
FVA_EXIT_CODE CLT_Folder_Merging::execute()
{
	QString subFolder = m_folder;
	subFolder.remove(m_baseFolder);

	if (!m_dir.exists(m_custom + subFolder + "/"))
	{
		// skip internal folder 
		if (!subFolder.contains("#copy"))
		{
			if (!m_dir.mkpath(m_custom + subFolder + "/"))
			{
				LOG_QCRIT << "could not create dest folder:" << m_custom + subFolder + "/";
				return FVA_ERROR_CANT_CREATE_DIR;
			}
		}
	}
	else
	{
		if (!subFolder.isEmpty())
		{
			if (m_dir.exists(m_custom + subFolder + " #1" + "/"))
			{
				if (!m_dir.rename(m_custom + subFolder + " #1" + "/", m_custom + subFolder + " #2" + "/"))
					LOG_QWARN << "could not rename source :" << m_custom + subFolder + " #1" + "/" << " into " << m_custom + subFolder + " #2" + "/";
				else
					LOG_QWARN << "renamed source :" << m_custom + subFolder + " #1/" << " into " << m_custom + subFolder + " #2";
			}

			if (!m_dir.rename(m_custom + subFolder, m_custom + subFolder + " #1" + "/"))
				LOG_QWARN << "could not rename source :" << m_custom + subFolder << " into " << m_custom + subFolder + " #1/";
			else
				LOG_QWARN << "renamed source :" << m_custom + subFolder << " into " << m_custom + subFolder + " #1/";

			// skip internal folder 
			if (!subFolder.contains("#copy"))
				m_dir.mkpath(m_custom + subFolder + "/");
		}
	}

	// create folder structure the same as in source folder
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		QString original = m_folder + "/" + info.fileName();
		QString dest = m_custom + subFolder + "/" + info.fileName();

		// skip internal folder 
		if (original.contains("#copy") || dest.contains("#copy"))
			continue;

		if (!m_dir.rename(original, dest))
		{
			if (QDir(original).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries).count() == 0)
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
	}
	return FVA_NO_ERROR;
}