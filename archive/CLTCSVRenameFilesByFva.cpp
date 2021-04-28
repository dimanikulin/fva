#include "CLTCSVRenameFilesByFva.h"

#include "FVADescriptionFile.h"

FVA_EXIT_CODE CLTCSVRenameFilesByFva::execute()
{
	/*QString descPath = m_folder + "/" + FVA_DESCRIPTION_FILE_NAME;
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
	}*/

	return FVA_NO_ERROR;
}