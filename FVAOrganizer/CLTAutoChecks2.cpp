#include "fvadefaultcfg.h"

#include "CLTAutoChecks2.h"

#include "fvacommonexif.h"

FVA_EXIT_CODE CLTAutoChecks2::execute()
{
	QMap<QString, unsigned int> fileCount;
	unsigned int countSupportedFiles = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// 1.CHECK FOR PROPER FOLDER NAME (NO COPY FOLDER)
		if (info.isDir())
		{
			QDateTime from, to;
			if (FVA_NO_ERROR != fvaParseDirName(info.fileName(), from, to))
			{
				// skip internal folder 
				if (m_dir.dirName()[0] == '#')
					continue;

				LOG_QCRIT << "wrong folder name:" << info.absoluteFilePath();
				m_Issues.push_back("FVA_ERROR_WRONG_FOLDER_NAME," + info.absoluteFilePath() + "," + info.fileName());
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FOLDER_NAME;
			}
			continue;
		}

		// 2.CHECK FOR PROPER FILE NAME AND SUPPORTED TYPE
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
		if (FVA_FS_TYPE_UNKNOWN != type)
		{
			countSupportedFiles++;	// it is our file
			QDateTime date;
			QString baseFileName = info.baseName();
			if (FVA_NO_ERROR != fvaParseFileName(info.baseName(), date))
			{
				LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath();
				m_Issues.push_back("FVA_ERROR_WRONG_FILE_NAME," + info.absoluteFilePath() + "," + info.fileName());
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FILE_NAME;
			}

			QMap<QString, QString>::iterator it = m_uniqueFileNames.find(info.fileName());
			if (m_uniqueFileNames.end() != it)
			{
				LOG_QWARN << "not uniquie file name found: " << info.fileName() << ",cur path:" << info.absoluteFilePath() << ",prev path:" << it.value();
				m_Issues.push_back("FVA_ERROR_NON_UNIQUE_FILE_NAME," + info.absoluteFilePath() + "," + info.fileName());
			}
			else
				m_uniqueFileNames[info.fileName()] = info.absoluteFilePath();

			////////////////////////////////// 5. check for matching taken time and file name//////////////////////////
			if (FVA_FS_TYPE_IMG == type)
			{
				QString error;
				QDateTime dateTime = fvaGetExifDateTimeOriginalFromFile(info.filePath());

				if (!dateTime.isValid())
				{
					LOG_QWARN << "empty image taken time found in:" << info.absoluteFilePath();
					m_Issues.push_back("FVA_ERROR_NULL_TAKEN_TIME," + info.absoluteFilePath() + "," + info.fileName());
				}
				else
				{
					if (dateTime != date)
					{
						LOG_QWARN << "mismatching image taken time found in:" << info.absoluteFilePath();
						m_Issues.push_back("FVA_ERROR_MISMATCH_TAKEN_TIME," + info.absoluteFilePath() + "," + info.fileName());
					}
				}
			}
			if (FVA_FS_TYPE_VIDEO == type)
			{
				QString error;
				QDateTime dateTime = fvaGetExifDateTimeOriginalFromFile(info.filePath());
				if (!dateTime.isValid())
				{
					LOG_QWARN << "empty video taken time found in:" << info.absoluteFilePath();
					// m_Issues.push_back("FVA_ERROR_EMPTY_VIDEO_TIME," + info.absoluteFilePath() + "," + info.fileName());
				}
			}

			//////////////////////////////////// 6. MATCHING FILE NAME AND FOLDER NAME ////////////////////////////////////////////////////
			QDateTime dateStart, dateEnd;
			if (FVA_NO_ERROR != fvaParseDirName(m_dir.dirName(), dateStart, dateEnd))
			{
				// skip internal folder 
				if (m_dir.dirName()[0] == '#')
					continue;

				LOG_QCRIT << "wrong matching folder name:" << info.absoluteFilePath();
				m_Issues.push_back("FVA_ERROR_WRONG_FOLDER_NAME," + info.absoluteFilePath() + "," + info.fileName());
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_WRONG_FOLDER_NAME;
			}
			if (dateStart == dateEnd)
				dateEnd = dateEnd.addYears(1);
			QDateTime fileDateTime = QDateTime::fromString(baseFileName, FVA_FILE_NAME_FMT);
			QString newFileName = baseFileName.replace("##", "01");
			if (!fileDateTime.isValid())
				fileDateTime = QDateTime::fromString(newFileName, FVA_FILE_NAME_FMT);
			if ((fileDateTime < dateStart) || (fileDateTime > dateEnd))
			{
				LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath() << " data period=" << dateStart << ";" << dateEnd;
				m_Issues.push_back("FVA_ERROR_NOT_SUPPORTED_FILE," + info.absoluteFilePath() + "," + info.fileName());
				if (m_readOnly)
					continue;
				else
					return FVA_ERROR_NOT_SUPPORTED_FILE;
			}
			countSupportedFiles++;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else if (fvaIsInternalFile(info.fileName()))
		{
			// nothing to do here
			m_Issues.push_back("FVA_ERROR_INTERNAL_FILE," + info.absoluteFilePath() + "," + info.fileName());
		}
		else
		{
			LOG_QCRIT << "unsupported file found:" << info.absoluteFilePath();
			m_Issues.push_back("FVA_ERROR_NOT_SUPPORTED_FILE," + info.absoluteFilePath() + "," + info.fileName());
			if (!m_readOnly)
				return FVA_ERROR_NOT_SUPPORTED_FILE;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	m_fileCount[m_folder] = countSupportedFiles;
	if (countSupportedFiles < FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR && countSupportedFiles)
	{
		// check for to little supported 
		LOG_QCRIT << "too little supported files found in:" << m_folder;
		m_Issues.push_back("FVA_ERROR_TOO_LITTLE_FILES," + m_folder + ",");
		if (!m_readOnly)
			return FVA_ERROR_TOO_LITTLE_FILES;
	}

	return FVA_NO_ERROR;
}
QMap< unsigned int, unsigned int > sizes;

CLTAutoChecks2::~CLTAutoChecks2()
{
	LOG_QDEB << "cmd deleted, dir:" << m_folder;
	for (auto it = m_fileCount.begin(); it != m_fileCount.end(); ++it)
	{
		sizes[it.value()] = ++sizes[it.value()];
	}
	if (0 == m_Issues.size())
		return; // not to create file for no issues
	QFile fileNew(FVA_DEFAULT_ROOT_DIR + "issues2.csv");
	fileNew.open(QIODevice::Append | QIODevice::Text);
	QTextStream writeStream(&fileNew);
	writeStream.setCodec("UTF-8");
	for (auto it = m_Issues.begin(); it != m_Issues.end(); ++it)
		writeStream << *it << "\n";
	writeStream.flush();
	fileNew.close();
}