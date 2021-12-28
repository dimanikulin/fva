/*!
* \file CLTRenameVideoBySequence.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTRenameVideoBySequence.h"
#include "fvacommonexif.h"

CLTRenameVideoBySequence::CLTRenameVideoBySequence(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsBoolean("Rename::videoByModifTime", m_renameVideoByModifTime);
	RET_IF_RES_IS_ERROR

	res = m_fmtctx.fillFmtContextFromCfg(cfg);
	RET_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTRenameVideoBySequence::execute(const CLTContext& /*context*/)
{
	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType(suffix) && imageFilePrefix.isEmpty())
		{
			if (!info.baseName().contains("_"))
				continue;
			int index = info.baseName().indexOf("_");
			imageFilePrefix = info.baseName().mid(0, index);
			LOG_DEB << "got new image prefix:" << imageFilePrefix;
		}
		else if (FVA_FS_TYPE_VIDEO == fvaConvertFileExt2FileType(suffix))
		{
			QString error;
			QDateTime renameDateTime = fvaGetVideoTakenTime(info.filePath(), error, m_fmtctx);
			if (renameDateTime.isValid() || info.baseName().at(0) == 'P') // P is first latter for panasonic cameras
			{
				continue;
			}

			FVA_EXIT_CODE res = fvaParseFileName(info.baseName(), renameDateTime, m_fmtctx);
			if (FVA_NO_ERROR == res)
			{
				continue;
			}
			if (!info.baseName().contains("_"))
			{
				LOG_CRIT << "video file does not contain _:" << info.absoluteFilePath();
				return FVA_ERROR_SEQUENCE;
			}
			int index = info.baseName().indexOf("_");
			QString videoFilePrefix = info.baseName().mid(0, index);
			if (imageFilePrefix.isEmpty())
			{
				if (m_renameVideoByModifTime == true && info.lastModified().isValid())
				{
					continue;
				}
				else
				{
					LOG_CRIT << "still empty image prefix for path:" << info.absoluteFilePath();
					return FVA_ERROR_SEQUENCE;
				}
			}
			QString newFilePath = info.absoluteFilePath().replace(videoFilePrefix, imageFilePrefix);

			// sometimes the file names are already in state as we need
			if (info.absoluteFilePath() == newFilePath)
			{
				LOG_WARN << "file has already target name" << info.absoluteFilePath() << ", skipping";
				continue;
			}
			if (!m_dir.rename(info.absoluteFilePath(), newFilePath))
			{
				LOG_CRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newFilePath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_DEB << "renamed file:" << info.absoluteFilePath() << " to:" << newFilePath;
		}
		else if (FVA_FS_TYPE_UNKNOWN == fvaConvertFileExt2FileType(suffix))
		{
			LOG_WARN << "unsupported file type:" << info.absoluteFilePath();
			continue;
		}
	}
	return FVA_NO_ERROR;
}
