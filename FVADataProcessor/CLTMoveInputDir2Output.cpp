/*!
* \file CLTMoveInputDir2Output.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTMoveInputDir2Output.h"
#include "fvacommoncsv.h"

CLTMoveInputDir2Output::CLTMoveInputDir2Output(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
	LOG_CRIT << "could not get fva root dir";
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTMoveInputDir2Output::execute(const CLTContext& context)
{
	// get the last dir leaf in input folder
	QString dirName = m_dir.dirName();

	QString dstDirPath = context.outputDir + "/" + dirName.mid(0, 4)/*extract year*/;

	if (dirName.length()==4) // YEAR folder
	{
		LOG_DEB << "year folder:" << dirName;
	}
	else
	{
		LOG_DEB << "last leaf folder:" << dirName;
		dstDirPath =+ "/" + dirName;
	}

	FVA_EXIT_CODE res = fvaCreateDirIfNotExists(dstDirPath);
	if (FVA_ERROR_CANT_CREATE_DIR == res)
	{
		LOG_CRIT << "could not create dest folder:" << dstDirPath;
		return FVA_ERROR_CANT_CREATE_DIR;
	}
	else if (FVA_ERROR_DEST_DIR_ALREADY_EXISTS == res)
	{
		// it is first time run
		if (context.custom.isEmpty())
			return FVA_ERROR_DEST_DIR_ALREADY_EXISTS;
		else if (context.custom == "create")
		{
			LOG_CRIT << "destination dir already exists: " << dstDirPath;
			// lets try to create with different name
			dstDirPath += " #1";
			res = fvaCreateDirIfNotExists(dstDirPath);
			if (FVA_ERROR_CANT_CREATE_DIR == res)
			{
				LOG_CRIT << "could not create dest folder:" << dstDirPath;
				return FVA_ERROR_CANT_CREATE_DIR;
			}
			else if (FVA_ERROR_DEST_DIR_ALREADY_EXISTS == res)
			{
				LOG_CRIT << "not immplemented to create dest folder with #2:" << dstDirPath;
				return FVA_ERROR_NOT_IMPLEMENTED;
			}
			else if (FVA_NO_ERROR == res)
			{
				LOG_DEB << "created dest folder:" << dstDirPath;
			}
		}
		else if (context.custom == "merge")
		{
			// do nothing here
		}
	}
	else if (FVA_NO_ERROR == res)
	{
		LOG_DEB << "created dest folder:" << dstDirPath;
	}
	
	
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// skip internal folder 
		if (fvaIsInternalDir(dir) || fvaIsInternalDir(dstDirPath))
		{
			LOG_WARN << "skipped #copy for: " << info.absoluteFilePath() << " , dst: " << dstDirPath;
			continue;
		}

		// check for already existing
		if (m_dir.exists(dstDirPath + "/" + info.fileName()))
		{
			LOG_CRIT << "destination file already exists: " << dstDirPath + "/" + info.fileName();
			return FVA_ERROR_DEST_FILE_ALREADY_EXISTS;
		}

		if (context.readOnly == true) // we MOVE content
		{
			// we move files only
			if (!info.isDir() && !m_dir.rename(info.absoluteFilePath(), dstDirPath + "/" + info.fileName()))
			{
				LOG_CRIT << "could not move:" << info.absoluteFilePath() << " into " << dstDirPath + "/" + info.fileName();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else if (!info.isDir())
				LOG_DEB << "moved:" << info.absoluteFilePath() << " into " << dstDirPath + "/" + info.fileName();

			if (info.isDir() && !fvaRemoveDirIfEmpty(info.absoluteFilePath()))
			{
				LOG_CRIT << "could not remove empty source:" << info.absoluteFilePath();
				return FVA_ERROR_CANT_MOVE_DIR;
			}
			else if (info.isDir())
				LOG_DEB << "removed empty destination:" << info.absoluteFilePath();
		}
		else // we COPY content
		{
			return FVA_ERROR_NOT_IMPLEMENTED;		
		}
	}
	return FVA_NO_ERROR;
}