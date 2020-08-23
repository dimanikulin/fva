#include "CLTRenameFiles.h"
#include "fvadefaultcfg.h"
#include "fvacommonexif.h"

bool CLTRenameFiles::checkIfParentFileExist(const QFileInfo& fileToCheck, QDateTime& renameDateTime, const QDateTime& prevRenameDateTime)
{
	QString newPath = fileToCheck.absoluteFilePath().remove("_1");
	newPath = newPath.remove("_2");

	QDir dir;
	if (!dir.exists(newPath))
		return false;

	if (fileToCheck.baseName().contains("_1"))
	{
		LOG_QDEB << "file contains _1 : " << fileToCheck.absoluteFilePath() << ", rename time to use : " << prevRenameDateTime.addSecs(1).toString(FVA_FILE_NAME_FMT);
		renameDateTime = prevRenameDateTime.addSecs(1);
	}
	else if (fileToCheck.baseName().contains("_2"))
	{
		LOG_QDEB << "file contains _2:" << fileToCheck.absoluteFilePath() << ", rename time to use:" << prevRenameDateTime.addSecs(2).toString(FVA_FILE_NAME_FMT);
		renameDateTime = prevRenameDateTime.addSecs(2);
	}
	return false;
}
void CLTRenameFiles::fillRenameDateTimeFromLastModifiedIfValid(const QDir& dir, const QFileInfo& info, QDateTime& renameDateTime)
{
	QString dirDate = dir.dirName().mid(0, 10);
	QDateTime validDateStart = QDateTime::fromString(dirDate, FVA_DIR_NAME_FMT);

	if (validDateStart.isValid())
	{
		QDateTime validDateEnd = validDateStart.addDays(1);
		if ((info.lastModified() > validDateStart) && (info.lastModified() < validDateEnd))
		{
			LOG_QWARN << " modification time to use for:" << info.absoluteFilePath() << ",time:" << info.lastModified().toString(FVA_FILE_NAME_FMT);
			renameDateTime = info.lastModified();
		}
	}
}

FVA_EXIT_CODE CLTRenameFiles::execute()
{
	QFileInfoList files = m_dir.entryInfoList(QDir::Files);
	QFileInfo info;

	QDateTime renameDateTime, prevRenameDateTime;

	Q_FOREACH(info, files)
	{
		renameDateTime = QDateTime();

		// if it is picture files
		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType(suffix))
		{
			if (!checkIfParentFileExist(info, renameDateTime, prevRenameDateTime))
			{
				renameDateTime = fvaGetExifDateTimeOriginalFromFile(info.filePath());
				QString _newName = renameDateTime.toString(FVA_FILE_NAME_FMT);
				if (_newName.isEmpty())
					fillRenameDateTimeFromLastModifiedIfValid(m_dir, info, renameDateTime);
				if (!renameDateTime.isValid() && (true == FVA_RENAME_PICS_BY_MODIF_TIME_IF_EMPTY_EXIF))
				{
					if (info.lastModified().isValid())
					{
						LOG_QWARN << "modification time to use (true == FVA_RENAME_FILES_BY_MODIF_TIME_FOR_EMPTY_EXIF) for:" << info.absoluteFilePath() << ", time : " << info.lastModified().toString(FVA_FILE_NAME_FMT);
						renameDateTime = info.lastModified();
					}
				}
			}
		}
		// if it is video file
		else if (FVA_FS_TYPE_VIDEO == fvaConvertFileExt2FileType(suffix))
		{
			QString error;
			renameDateTime = fvaGetVideoTakenTime(info.filePath(), error);
			if (!renameDateTime.isValid())
			{
				if (FVA_NO_ERROR != fvaParseFileName(info.baseName(), renameDateTime))
				{
					LOG_QWARN << "can not get taken time from:" << info.absoluteFilePath() << ",error:" << error;
					fillRenameDateTimeFromLastModifiedIfValid(m_dir, info, renameDateTime);
					if (!renameDateTime.isValid() && FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true && info.lastModified().isValid())
					{
						LOG_QWARN << "last modif time to use (FVA_RENAME_VIDEO_BY_MODIF_TIME_IF_EMPTY_EXIF == true): for" << info.absoluteFilePath();
						renameDateTime = info.lastModified();
					}
				}
			}
		}
		else if (FVA_FS_TYPE_AUDIO == fvaConvertFileExt2FileType(suffix))
		{
			fillRenameDateTimeFromLastModifiedIfValid(m_dir, info, renameDateTime);
		}
		else
		{
			LOG_QWARN << "unsupported file type:" << info.absoluteFilePath();
			continue;
		}
		QString newName = renameDateTime.toString(FVA_FILE_NAME_FMT);
		if (newName.isEmpty())
		{
			LOG_QWARN << "no time in img file:" << info.absoluteFilePath() << ",prev time to use:" << prevRenameDateTime.addSecs(1).toString("yyyy-MM-dd-hh-mm-ss");
			prevRenameDateTime = prevRenameDateTime.addSecs(1);
			newName = prevRenameDateTime.toString(FVA_FILE_NAME_FMT);
		}
		else
			prevRenameDateTime = renameDateTime;

		if (newName.isEmpty())
		{
			LOG_QCRIT << "sequence error for file:" << info.absoluteFilePath();
			return FVA_ERROR_SEQUENCE;
		}

		QString newPath = "";
		if ("OLD_NAME" == m_custom)
			newPath = m_dir.path() + "/" + newName + "__" + info.baseName() + "." + info.suffix();
		else
			newPath = m_dir.path() + "/" + newName + "." + info.suffix();

		if (info.absoluteFilePath() == newPath)
		{
			LOG_QWARN << "File already has the same name:" << info.absoluteFilePath();
			continue;
		}

		if (m_dir.exists(newPath))
		{
			LOG_QWARN << "file already exists:" << newPath << ",old path:" << info.absoluteFilePath();
			if (!m_dir.exists(m_dir.path() + "/#copy"))
			{
				LOG_QWARN << "created COPY folder:" << m_dir.path() + "/#copy";
				m_dir.mkdir(m_dir.path() + "/#copy");
			}
			QString newCopyPath = m_dir.path() + "/#copy/" + newName + "." + info.suffix();
			if (!m_dir.rename(info.absoluteFilePath(), newCopyPath))
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newCopyPath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "renamed file:" << info.absoluteFilePath() << " to:" << newCopyPath;

			continue;
		}

		if (!m_dir.rename(info.absoluteFilePath(), newPath))
			LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newPath;
		else
			LOG_QDEB << "Renamed:" << info.absoluteFilePath() << " to:" << newPath;
	}
	return FVA_NO_ERROR;
}
