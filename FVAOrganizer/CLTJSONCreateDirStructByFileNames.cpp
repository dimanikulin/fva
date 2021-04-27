#include "CLTJSONCreateDirStructByFileNames.h"

FVA_EXIT_CODE CLTJSONCreateDirStructByFileNames::execute()
{
	/*// check for existing folder description
	if (!m_dir.exists(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
	{
		if (m_custom.isEmpty())
		{
			LOG_QCRIT << FVA_DIR_DESCRIPTION_FILE_NAME << "does not exist in" << m_folder;
			return FVA_ERROR_CANT_FIND_DIR_DESC;
		}
		// create folder description
		if (!m_readOnly)
		{
			QString error;

			QVariantMap content;
			content["deviceId"] = m_custom; // m_custom must be device id
			/* FVA_EXIT_CODE res = fvaCreateFolderDescription( m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME, content, error );
			if ( FVA_NO_ERROR != res )
			{
			LOG_QCRIT << error;
			return res;
			}
		}
	}*/

	/*Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		// skip meta files
		if (fvaIsInternalFile(info.fileName()))
			continue;

		QString subFolderName = info.baseName().mid(0, 10).replace("-", ".");
		QString fullSubFolderpath = m_folder + "/" + subFolderName;
		fullSubFolderpath = fullSubFolderpath.replace("-", ".");

		if (!m_dir.exists(subFolderName))
		{
			if (!m_readOnly)
				m_dir.mkdir(subFolderName);
			LOG_QDEB << "sub-folder:" << subFolderName << " created";

			// copy folder description
			if (!m_readOnly)
			{
				if (QFile::copy(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME, fullSubFolderpath + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
					LOG_QDEB << FVA_DIR_DESCRIPTION_FILE_NAME << " is copied to " << subFolderName;
				else
				{
					LOG_QCRIT << FVA_DIR_DESCRIPTION_FILE_NAME << " is NOT copied to " << subFolderName;
					return FVA_ERROR_CANT_FIND_DIR_DESC;
				}
			}
		}
		if (!m_readOnly)
		{
			// move the file
			if (!m_dir.rename(info.absoluteFilePath(), fullSubFolderpath + "/" + info.fileName()))
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "file renamed:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
		}
	}
	// check for existing folder description
	if (m_dir.exists(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
	{
		if (!QFile::remove(m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME))
			// it is not critical error
			LOG_QDEB << "file could not be removed:" << m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
	}*/
	return FVA_NO_ERROR;
}
