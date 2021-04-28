#include "CLTMergeFewDaysFewsEventsDir.h"

FVA_EXIT_CODE CLTMergeFewDaysFewsEventsDir::execute()
{
	/*QString subFolder = m_folder;
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
	}*/
	return FVA_NO_ERROR;
}