#include "CLTMerge1DayEventDir.h"
#include "fvadefaultcfg.h"
#include "fvacommoncsv.h"

FVA_EXIT_CODE CLTMerge1DayEventDir::execute()
{
	// get the last dir leaf in input folder
	QString dir = m_dir.dirName();
	QString dstDirPath = FVA_DEFAULT_ROOT_DIR + dir.mid(0, 4)/*extract year*/ + "/" + m_dir.dirName();
	if (!m_dir.exists(dstDirPath))
	{
		if (!dstDirPath.contains("#"))
		{
			// # means internal folder and not subject of merging
			if (!m_dir.mkpath(dstDirPath))
			{
				LOG_QCRIT << "could not create dest folder:" << dstDirPath;
				return FVA_ERROR_CANT_CREATE_DIR;
			}
			else
			{
				LOG_QDEB << "created dest folder:" << dstDirPath;

				int ID = FVA_UNDEFINED_ID;
				FVA_EXIT_CODE res = fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolder.id", ID);
				RET_RES_IF_RES_IS_ERROR

					QString dir = dstDirPath.replace("\\", "/");  // replace slaches on backslashes
				dir = dir.remove(FVA_DEFAULT_ROOT_DIR); // remove a prefix as root dir

				QFile fileNew(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv");
				if (!fileNew.open(QIODevice::WriteOnly | QIODevice::Text))
					return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
				QTextStream writeStream(&fileNew);
				//ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder,WhoTookFotoId,Scanerid
				writeStream << "\n" << QString::number(++ID) << ",/" << dir << ",0,,,,,,,,";
				writeStream.flush();
				fileNew.close();

				res = fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolder.id", ID);
				RET_RES_IF_RES_IS_ERROR

					LOG_QDEB << "created fvaFolderN.csv for folder:" << dir;
			}
		}
	}
	else
		LOG_QWARN << "could not create dest folder as it already exists:" << dstDirPath;

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