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
	QFile fileNew(m_rootSWdir + "#data#/FVA_ERROR_NO_EXIF_LOCATION.csv");
	fileNew.open(QIODevice::Append | QIODevice::Text);
	QTextStream writeStream(&fileNew);
	writeStream.setCodec("UTF-8");
	for (auto it = m_Issues.begin(); it != m_Issues.end(); ++it)
		writeStream << *it << "\n";
	writeStream.flush();
	fileNew.close();

	LOG_DEB << "cmd deleted, dir:" << m_folder; 
}