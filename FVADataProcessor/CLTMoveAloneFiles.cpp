/*!
* \file CLTMoveAloneFiles.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTMoveAloneFiles.h"

CLTMoveAloneFiles::CLTMoveAloneFiles(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsUint("Rename::minFilesInDir", m_minCountSupportedFiles);
	RET_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTMoveAloneFiles::execute(const CLTContext& context)
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
	if ((countSupportedFiles >= m_minCountSupportedFiles) || !countSupportedFiles)
		return FVA_NO_ERROR;

	// get the last dir leaf in input folder
	QString dirName = m_dir.dirName();

	if (dirName.length()==4) // YEAR folder - no need to move these files
	{
		LOG_DEB << "year folder:" << dirName;
		return FVA_NO_ERROR;
	}

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

		LOG_WARN << "to move to:" << folderUp + "/" + info.fileName()
			<< " from " << info.absoluteFilePath();

		// move the files
		if (!context.readOnly)
		{
			if (!m_dir.rename(info.absoluteFilePath(), folderUp + "/" + info.fileName()))
			{
				LOG_CRIT << "cant move file:" << info.absoluteFilePath();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
		}
	}
	if (FVA_NO_ERROR == fvaRemoveDirIfEmpty(m_folder))
	{
		LOG_WARN << "removed empty dir " << m_folder;
	}

	return FVA_NO_ERROR;
}
