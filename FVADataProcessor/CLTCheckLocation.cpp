#include "CLTCheckLocation.h"
#include "fvacommonexif.h"

FVA_EXIT_CODE CLTCheckLocation::execute(const CLTContext& context)
{
	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();
		if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix))
			continue;

		bool present = fvaExifGeoDataPresentInFile(info.filePath());
		if (!present)
		{
			LOG_QCRIT << "found file without exif location:" << info.absoluteFilePath();
			return FVA_ERROR_NO_EXIF_LOCATION;
		}
	}
	return FVA_NO_ERROR;
}