#include "CLTMoveAloneFiles.h"
#include "fvadefaultcfg.h"

FVA_EXIT_CODE CLTMoveAloneFiles::execute()
{
	unsigned int countSupportedFiles = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
		if (FVA_FS_TYPE_UNKNOWN != type)
			countSupportedFiles++;
	}
	// no need to move these files
	if (countSupportedFiles >= FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR || !countSupportedFiles)
		return FVA_NO_ERROR;

	m_dir.cdUp();
	QString folderUp = m_dir.absolutePath();
	m_dir = QDir(m_folder);

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		// skip meta files
		if (fvaIsInternalFile(info.fileName()))
			continue;

		LOG_QWARN << "to move to:" << folderUp + "/" + info.fileName()
			<< " from " << info.absoluteFilePath();

		// move the files
		if (!m_readOnly)
		{
			if (!m_dir.rename(info.absoluteFilePath(), folderUp + "/" + info.fileName()))
			{
				LOG_QCRIT << "cant move file:" << info.absoluteFilePath();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
		}
	}

	return FVA_NO_ERROR;
}
