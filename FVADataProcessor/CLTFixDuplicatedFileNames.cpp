/*!
* \file CLTFixDuplicatedFileNames.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTFixDuplicatedFileNames.h" 

//TODO to work with digiKam dir but not fva dir
CLTFixDuplicatedFileNames::CLTFixDuplicatedFileNames(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTFixDuplicatedFileNames::execute(const CLTContext& context)
{        
	// get the last dir leaf in input folder
	QString dir = m_dir.dirName();

	QString dstDirPath;
	if (!context.custom.isEmpty())
	{
		// it is destintation folder fo us
		dstDirPath = context.custom;
	}
	else 
		dstDirPath = m_rootSWdir + dir.mid(0, 4)/*extract year*/ + "/" + m_dir.dirName();

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// skip internal folder 
		if (fvaIsInternalDir(dir) || fvaIsInternalDir(dstDirPath))
		{
			LOG_WARN << "skipped #copy for: " << info.absoluteFilePath() << " , dst: " << dstDirPath;
			continue;
		}

		if (info.isDir())
			continue;

		// check for already existing
		if (!m_dir.exists(dstDirPath + "/" + info.fileName()))
			continue;

		QString cropedFileName = info.fileName().split(".", QString::SkipEmptyParts).at(0);
		QString fileNameExt = info.fileName().split(".", QString::SkipEmptyParts).at(1);

		int lastDigit = cropedFileName[cropedFileName.length()-1].digitValue();
		if (lastDigit == 9)
			// last digit seconds overflow
			lastDigit = -1;
			
		cropedFileName.chop(1);
		QString newFileName = cropedFileName + QString::number(++lastDigit) + "." + fileNameExt;
		QString newFullPath = info.absoluteDir().absolutePath() + "/" + newFileName;

		// check for already existing again
		if (m_dir.exists(newFullPath))
		{
			LOG_CRIT << "destination file already exists again: " << newFullPath;
			return FVA_ERROR_DEST_FILE_ALREADY_EXISTS;
		}
		else
		{
			if (!m_dir.rename(info.absoluteFilePath(), newFullPath))
			{
				LOG_CRIT << "could not move:" << info.absoluteFilePath() << " into " << newFullPath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
			{
				LOG_DEB << "renamed:" << info.absoluteFilePath() << " into " << newFullPath;
				LOG_DEB << "need to fix taken time for:" << newFullPath;
			}
		}
	}
	return FVA_NO_ERROR;
}