#include "CLTMerge1EventDir.h"

FVA_EXIT_CODE CLTMerge1EventDir::execute()
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
