#include "CLTCheckDateTime.h"
#include "fvacommonexif.h"

CLTCheckDateTime::CLTCheckDateTime(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = m_fmtctx.fillFmtContextFromCfg(cfg);
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTCheckDateTime::execute(const CLTContext& context)
{
	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix))
			continue;

		QDateTime DateTime = fvaGetExifDateTimeOriginalFromFile(info.filePath(), m_fmtctx.exifDateTime);

		if (!DateTime.isValid() || DateTime.isNull())
		{
			LOG_QCRIT << "found not correct exif Date-Time:" << info.absoluteFilePath();
			return FVA_ERROR_INCORRECT_EXIF_DATE_TIME;
		}
	}
	return FVA_NO_ERROR;
}