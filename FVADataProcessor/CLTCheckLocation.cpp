/*!
* \file CLTCheckLocation.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTCheckLocation.h"
#include "fvacommonexif.h"

FVA_EXIT_CODE CLTCheckLocation::execute(const CLTContext& context)
{
	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix))
			continue;

		bool present = fvaExifGeoDataPresentInFile(info.filePath());
		if (!present)
		{
			if (context.readOnly)
			{
				LOG_CRIT << "found file without exif location:" << info.absoluteFilePath();
				return FVA_ERROR_NO_EXIF_LOCATION;
			}
			else
			{
				m_Issues.push_back(info.absoluteFilePath());					
			}
		}
	}
	return FVA_NO_ERROR;
}
CLTCheckLocation::~CLTCheckLocation()
{
	QString rootSWdir; 

	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", rootSWdir);
	RET_IF_RES_IS_ERROR

	fvaSaveStrListToFile(rootSWdir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv", m_Issues);

	LOG_DEB << "cmd deleted, dir:" << m_folder; 
}