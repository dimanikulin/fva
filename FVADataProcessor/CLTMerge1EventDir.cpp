/*!
* \file CLTMerge1EventDir.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTMerge1EventDir.h"

FVA_EXIT_CODE CLTMerge1EventDir::execute(const CLTContext& context)
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{				
		QString original	= m_folder + "/" + info.fileName();
		QString dest		= context.custom + "/" + info.fileName();

		// skip internal folder 
		if (fvaIsInternalDir(original) || fvaIsInternalDir(dest))
			continue;

		// check for already existing
		if (m_dir.exists(dest))
		{
			LOG_QCRIT << "destination file already exists: " << dest;
			return FVA_ERROR_DEST_FILE_ALREADY_EXISTS;
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
