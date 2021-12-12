/*!
* \file CLTCreateDirStructByFileNames.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTCreateDirStructByFileNames.h"

FVA_EXIT_CODE CLTCreateDirStructByFileNames::execute(const CLTContext& context)
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		// skip meta files
		if ( fvaIsInternalFile( info.fileName() ) )
			continue;

		// create year dir first
		QString yearDirPath = m_folder + "/" + m_dir.dirName().mid(0, 4)/*extract year*/;

		if (!m_dir.exists(yearFolderName))
		{
			if ( !context.readOnly )
				m_dir.mkdir(yearFolderName);
			LOG_QDEB << "year-folder:" << yearFolderName << " created";
		}

		QString subFolderName		= info.baseName().mid(0,10).replace("-",".");
		QString fullSubFolderpath	= m_folder + "/" + m_dir.dirName().mid(0, 4) + "/" + subFolderName;

		// and noe day name	
		if (!m_dir.exists(subFolderName))
		{
			if ( !context.readOnly )
				m_dir.mkdir( subFolderName );
			LOG_QDEB << "sub-folder:" << subFolderName << " created";
		}
		if ( !context.readOnly ) 
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


