#include "CLTFixEmptyDateTime.h"
#include "fvacommonexif.h"

CLTFixEmptyDateTime::CLTFixEmptyDateTime(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = m_fmtctx.fillFmtContextFromCfg(cfg);
	RET_IF_RES_IS_ERROR

	res = cfg.getParamAsBoolean("Rename::picsByModifTime", m_fixPicsByModifTime);
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTFixEmptyDateTime::execute(const CLTContext& context)
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
			if (true == m_fixPicsByModifTime)
			{
				LOG_QWARN << " modification time to use for:" << info.absoluteFilePath() << ",time:" << info.lastModified().toString(m_fmtctx.fvaFileName);
			}
			else
				return FVA_ERROR_CANT_FIX_EMPTY_EXIF_DATE_TIME;
		}
	}
	return FVA_NO_ERROR;
}