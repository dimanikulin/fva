/*!
* \file CLTAutoChecks1.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTAutoChecks1.h"
#include "fvacommonexif.h"

CLTAutoChecks1::CLTAutoChecks1(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsBoolean("Rename::videoByModifTime", m_renameVideoByModifTime);
	RET_IF_RES_IS_ERROR

	res = m_fmtctx.fillFmtContextFromCfg(cfg);
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTAutoChecks1::execute(const CLTContext& /*context*/)
{
	m_dir.setSorting(QDir::LocaleAware);
	bool first = false;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);

		//	#01.NotVideoFirst
		if (!first)
		{
			first = true;
			if (FVA_FS_TYPE_VIDEO == type || FVA_FS_TYPE_AUDIO == type)
			{
				QString error;
				QDateTime time = fvaGetVideoTakenTime(info.absoluteFilePath(), error, m_fmtctx);
				if (!time.isValid())
				{
					if (m_renameVideoByModifTime == true && info.lastModified().isValid())
					{
						LOG_WARN << "skipping first video/audio file (FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true):" << info.absoluteFilePath();
						continue;
					}
					LOG_CRIT << "found first video/audio file:" << info.absoluteFilePath();
					return FVA_ERROR_VIDEO_FIRST;
				}
			}
		}

		//#02.NotSTFiles
		QString st = info.fileName().mid(0, 2).toUpper();
		if (st == "ST")
		{
			LOG_CRIT << "found panoram file:" << info.absoluteFilePath();
			return FVA_ERROR_PANORAM_FILE;
		}
	}

	return FVA_NO_ERROR;
}