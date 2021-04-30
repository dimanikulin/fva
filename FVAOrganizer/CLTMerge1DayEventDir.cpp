#include "CLTMerge1DayEventDir.h"
#include "fvadefaultcfg.h"
#include "fvacommoncsv.h"

FVA_EXIT_CODE CLTMerge1DayEventDir::execute(const CLTContext& context)
{
	// get the last dir leaf in input folder
	QString dir = m_dir.dirName();
	QString dstDirPath = FVA_DEFAULT_ROOT_DIR + dir.mid(0, 4)/*extract year*/ + "/" + m_dir.dirName();
	if (!dstDirPath.contains("#")) // # means internal folder and not subject of merging
	{
		FVA_EXIT_CODE res = fvaCreateDirIfNotExists(dstDirPath);
		if (FVA_ERROR_CANT_CREATE_DIR == res)
		{
			LOG_QCRIT << "could not create dest folder:" << dstDirPath;
			return FVA_ERROR_CANT_CREATE_DIR;
		}
		else if (FVA_ERROR_DEST_DIR_ALREADY_EXISTS == res)
		{
			// it is first time run
			if (context.custom.isEmpty())
				return FVA_ERROR_DEST_DIR_ALREADY_EXISTS;
			else if (context.custom == "create")
			{
				LOG_QCRIT << "destination dir already exists: " << dstDirPath;
				// lets try to create with different name
				dstDirPath += " #1";
				res = fvaCreateDirIfNotExists(dstDirPath);
				if (FVA_ERROR_CANT_CREATE_DIR == res)
				{
					LOG_QCRIT << "could not create dest folder:" << dstDirPath;
					return FVA_ERROR_CANT_CREATE_DIR;
				}
				else if (FVA_ERROR_DEST_DIR_ALREADY_EXISTS == res)
				{
					LOG_QCRIT << "not immplemented to create dest folder with #2:" << dstDirPath;
					return FVA_ERROR_NOT_IMPLEMENTED;
				}
				else if (FVA_NO_ERROR == res)
				{
					LOG_QDEB << "created dest folder:" << dstDirPath;
				}
			}
			else if (context.custom == "merge")
			{
				// do nothing here
			}
		}
		else if (FVA_NO_ERROR == res)
		{
			LOG_QDEB << "created dest folder:" << dstDirPath;
		}
	}
	
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// skip internal folder 
		if (dir.contains("#copy") || dstDirPath.contains("#copy"))
		{
			LOG_QWARN << "skipped #copy for: " << info.absoluteFilePath() << " , dst: " << dstDirPath;
			continue;
		}

		// check for already existing
		if (m_dir.exists(dstDirPath + "/" + info.fileName()))
		{
			LOG_QCRIT << "destination file already exists: " << dstDirPath + "/" + info.fileName();
			return FVA_ERROR_DEST_FILE_ALREADY_EXISTS;
		}

		// we move files only
		if (!info.isDir() && !m_dir.rename(info.absoluteFilePath(), dstDirPath + "/" + info.fileName()))
		{
			LOG_QCRIT << "could not move:" << info.absoluteFilePath() << " into " << dstDirPath + "/" + info.fileName();
			return FVA_ERROR_CANT_RENAME_FILE;
		}
		else if (!info.isDir())
			LOG_QDEB << "moved:" << info.absoluteFilePath() << " into " << dstDirPath + "/" + info.fileName();

		if (info.isDir() && !fvaRemoveDirIfEmpty(info.absoluteFilePath()))
		{
			LOG_QCRIT << "could not remove empty source:" << info.absoluteFilePath();
			return FVA_ERROR_CANT_MOVE_DIR;
		}
		else if (info.isDir())
			LOG_QDEB << "removed empty destination:" << info.absoluteFilePath();
	}
	return FVA_NO_ERROR;
}