#include "CLTRenameFilesByDir.h"

FVA_EXIT_CODE CLTRenameFilesByDir::execute(const CLTContext& /*context*/, const FvaConfiguration& /*cfg*/)
{
	int id = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType(suffix))
		{
			QString newPath = m_dir.path() + "/" + m_dir.dirName().replace(".", "-").mid(0, 10) + "-##-##-" + QString::number(id) + "." + info.suffix();
			if (!m_dir.rename(info.absoluteFilePath(), newPath))
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newPath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
			{
				LOG_QDEB << "renamed file:" << info.absoluteFilePath() << " to:" << newPath;
				id++;
			}
		}
	}

	return FVA_NO_ERROR;
}
