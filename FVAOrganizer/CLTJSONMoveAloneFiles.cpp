#include "CLTJSONMoveAloneFiles.h"

#include <windows.h>
#include <winbase.h>

#include "fvadefaultcfg.h"

FVA_EXIT_CODE CLTJSONMoveAloneFiles::execute()
{
	unsigned int countSupportedFiles = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
		if (FVA_FS_TYPE_UNKNOWN != type)
			countSupportedFiles++;
	}
	// no need to move these files
	if (countSupportedFiles >= FVA_DEFAULT_MIN_COUNT_FILES_IN_DIR || !countSupportedFiles)
		return FVA_NO_ERROR;

	QString descFilePath = m_folder + "/" + FVA_DESCRIPTION_FILE_NAME;
	if (m_dir.exists(descFilePath))
	{
		LOG_QCRIT << "description for files exists, skipping folder:" << m_folder;
		return FVA_NO_ERROR;
	}

	QVariantMap result;
	QString error;
	/*FVA_EXIT_CODE code = fvaGetFolderDescription( m_folder, result, error );
	if ( FVA_NO_ERROR != code )
	{
	LOG_QCRIT << error;
	return code;
	}*/
	// Name,Place,People,Device,Description,Scaner,Comment,oldName
	QString oneFileDesc = ","
		+ result["place"].toString() + ","
		+ result["people"].toString() + ","
		+ result["deviceId"].toString() + ","
		+ result["event"].toString() + ",,"
		+ result["tags"].toString() + ",\n";

	if (result["deviceId"].toString().isEmpty())
	{
		LOG_QCRIT << "deviceId is empty in:" << m_folder;
		return FVA_ERROR_EMPTY_DEV_ID;
	}

	std::auto_ptr <QFile>	pFile(nullptr);
	std::auto_ptr <QTextStream> pTextStream(nullptr);
	m_dir.cdUp();
	QString folderUp = m_dir.absolutePath();
	if (!m_readOnly)
	{
		pFile.reset(new QFile(folderUp + "/" + FVA_DESCRIPTION_FILE_NAME));
		QFile::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append;
		if (!pFile->open(mode))
		{
			LOG_QCRIT << "cant open description file:" << folderUp + "/" + FVA_DESCRIPTION_FILE_NAME;
			return FVA_ERROR_CANT_OPEN_FILE_DESC;
		}
		pTextStream.reset(new QTextStream(pFile.get()));
		if (m_custom != "FIRST")
		{
			*pTextStream.get() << "Name,Place,People,Device,Description,Scaner,Comment,oldName\n";
			m_custom = "FIRST";
		}
	}
	m_dir = QDir(m_folder);

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		// skip meta files
		if (fvaIsInternalFile(info.fileName()))
			continue;

		LOG_QWARN << "descr:" << info.fileName() + oneFileDesc;
		LOG_QWARN << "to move to:" << folderUp + "/" + info.fileName()
			<< " from " << info.absoluteFilePath();

		// move the files
		if (!m_readOnly)
		{
			if (!m_dir.rename(info.absoluteFilePath(), folderUp + "/" + info.fileName()))
			{
				LOG_QCRIT << "cant move file:" << info.absoluteFilePath();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			*pTextStream.get() << info.fileName() << oneFileDesc;
			pTextStream->flush();
		}
	}

	if (!m_readOnly)
	{
		pFile->close();
		QString descFolderPath = m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;

		SetFileAttributes(descFolderPath.toStdWString().c_str(), FILE_ATTRIBUTE_NORMAL);

		if (!m_dir.remove(descFolderPath))
		{
			LOG_QCRIT << "can not remove description for folder in folder:" << m_folder;
			return FVA_ERROR_CANT_REMOVE_FILE_OR_DIR;
		}
		m_dir.cdUp();
		if (!m_dir.rmdir(m_folder))
		{
			LOG_QCRIT << "can not remove folder:" << m_folder;
			return FVA_ERROR_CANT_REMOVE_FILE_OR_DIR;
		}
		LOG_QDEB << "removed folder:" << m_folder;
	}
	return FVA_NO_ERROR;
}