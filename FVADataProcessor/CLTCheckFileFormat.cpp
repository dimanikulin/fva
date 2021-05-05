#include "CLTCheckFileFormat.h"
FVA_EXIT_CODE CLTCheckFileFormat::execute(const CLTContext& context)
{
	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_UNKNOWN == fvaConvertFileExt2FileType(suffix))
		{
			LOG_QCRIT << "found not correct file format:" << info.absoluteFilePath();
			return FVA_ERROR_INCORRECT_FILE_FORMAT;
		}
	}
	return FVA_NO_ERROR;
}