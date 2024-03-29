/*!
* \file CLTCreateDirStructByDevNames.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTCreateDirStructByDevNames.h"
#include "fvacommonexif.h"

FVA_EXIT_CODE CLTCreateDirStructByDeviceName::execute(const CLTContext& /*context*/)
{
	QString deviceName;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		// if it is picture files
		QString suffix = info.suffix().toUpper();
		QString fullname = info.absoluteFilePath();
		if (FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType(suffix))
		{
			QString newDeviceName = fvaGetExifMakeAndModelFromFile(info.filePath());

			while (newDeviceName.endsWith('\n'))
				newDeviceName.chop(1);

			while (newDeviceName.endsWith('\r'))
				newDeviceName.chop(1);

			while (newDeviceName.endsWith(QChar('\0')))
				newDeviceName.chop(1);

			newDeviceName = newDeviceName.trimmed();

			QString dirName;
			if (newDeviceName.isEmpty())
			{
				LOG_DEB << "no device name in picture:" << info.absoluteFilePath();
				dirName = "EMPTY";
			}
			else
			{
				if (deviceName.isEmpty())
					LOG_DEB << "got first device name: " << newDeviceName << "in picture:" << info.absoluteFilePath();
				else if (deviceName != newDeviceName)
					LOG_WARN << "got new device name: " << newDeviceName << "in picture:" << info.absoluteFilePath() << "old: " << deviceName;

				deviceName = newDeviceName;
				dirName = deviceName;
			}

			QString fullSubFolderpath = m_folder + "/" + dirName;

			if (!m_dir.exists(dirName))
			{
				m_dir.mkdir(dirName);
				LOG_DEB << "sub-folder:" << dirName << " created";
			}
			// move the file
			if (!m_dir.rename(info.absoluteFilePath(), fullSubFolderpath + "/" + info.fileName()))
			{
				LOG_CRIT << "can not rename file:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_DEB << "file renamed:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
		}
		else
			LOG_DEB << " no device name in:" << info.absoluteFilePath();
	}
	return FVA_NO_ERROR;
}