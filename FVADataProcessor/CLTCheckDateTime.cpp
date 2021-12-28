/*!
* \file CLTCheckDateTime.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTCheckDateTime.h"
#include "fvacommonexif.h"

CLTCheckDateTime::CLTCheckDateTime(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = m_fmtctx.fillFmtContextFromCfg(cfg);
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTCheckDateTime::execute(const CLTContext& context)
{
	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix))
			continue;

		QDateTime DateTime = fvaGetExifDateTimeOriginalFromFile(info.filePath(), m_fmtctx.exifDateTime);

		if (!DateTime.isValid() || DateTime.isNull())
		{
			LOG_CRIT << "found empty exif Date-Time:" << info.absoluteFilePath();
			return FVA_ERROR_NO_EXIF_DATE_TIME;
		}
	}
	return FVA_NO_ERROR;
}