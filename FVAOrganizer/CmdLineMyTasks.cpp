#include "CmdLineMyTasks.h"

#include "FVADescriptionFile.h"

#include "FVADescriptionFile.h"

FVA_EXIT_CODE CLT_Convert_Dir_Desc::execute()
{
	QString descFolderPath = m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
	if ( !m_dir.exists( descFolderPath ) )
	{
		LOG_QWARN << "description does not exist in:" << m_folder;
		return FVA_NO_ERROR;
	}
	QFile file ( descFolderPath );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		LOG_QCRIT << "can not find old description in:" << m_folder;
		return FVA_ERROR_CANT_OPEN_DIR_DESC;
	}
	
	QTextStream readStream( &file );

	// readStream.setCodec("IBM866");
	QString jsonData = readStream.readAll();
	jsonData.remove("{");
	jsonData.remove("}");
	jsonData = "{" + jsonData + "}";
	file.close();

	bool res = false;
	/*QVariantMap result = QtJson::Json::parse ( jsonData, res ).toMap();

	if ( res && !result["event"].toString().isEmpty() ) 
		LOG_QWARN << "dir:" << m_folder << ",event:" << result["event"].toString();

	if ( !m_readOnly )
	{
		if ( !m_dir.rename( descFolderPath , descFolderPath + "_old" ) )
		{
			LOG_QCRIT << "can not rename description in:" << m_folder;
			return FVA_ERROR_CANT_RENAME_DIR_DESC;
		}
		QString error;
		FVA_EXIT_CODE res = fvaCreateFolderDescription( descFolderPath, result, error );
		if ( FVA_NO_ERROR != res )
		{
			LOG_QCRIT << error;
			return res;
		}

		LOG_QWARN << "updated folder description:" << descFolderPath;
	}*/	
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE CLT_Rename_File_By_Desc::execute()
{
	QString descPath = m_folder + "/" + FVA_DESCRIPTION_FILE_NAME;
	if ( !m_dir.exists( descPath ) )
	{
		LOG_QCRIT << "description file does not exist";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	FVADescriptionFile descFile;

	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = descFile.load( descPath, titles, decsItems );
	if ( FVA_NO_ERROR != res )
	{
		LOG_QCRIT << "description file can not be loaded";
		return res;
	}

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		int indexColumn = descFile.getColumnIdByName( titles, "Name" );
		if ( -1 == indexColumn )
		{
			LOG_QCRIT << "could not find Name column in description file";
			return FVA_ERROR_INCORRECT_FORMAT;
		}
		
		QString newName = it.value()[ indexColumn ];		
		indexColumn = descFile.getColumnIdByName( titles, "oldName" );
		
		if ( -1 == indexColumn )
		{
			LOG_QCRIT << "could not find oldName column in description file";			
			return FVA_ERROR_INCORRECT_FORMAT;
		}
		QString oldName = it.value()[ indexColumn ];

		if (m_dir.exists(m_folder + "/" + oldName))
		{
			if ( !m_dir.rename( m_folder + "/" + oldName, m_folder + "/" + newName ) )
			{
				LOG_QCRIT << "cant move file:" << m_folder << "/" << oldName << " into " << m_folder << "/" << newName;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
			{
				LOG_QCRIT << "moved file:" << m_folder << "/" << oldName << " into " << m_folder << "/" << newName;
			}
		}
	}

	return FVA_NO_ERROR;
}