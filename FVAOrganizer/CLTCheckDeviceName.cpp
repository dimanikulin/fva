#include "CLTCheckDeviceName.h"
#include "fvacommonexif.h"

FVA_EXIT_CODE CLTCheckDeviceName::execute()
{
	QString deviceName;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		// if it is picture files
		QString suffix = info.suffix().toUpper();
		QString fullname = info.absoluteFilePath();
		if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix))
			continue;

		QString newDeviceName = fvaGetExifMakeAndModelFromFile(info.filePath());

		if (newDeviceName.isEmpty())
		{
			LOG_QWARN << "no device name in picture:" << info.absoluteFilePath();
			continue; // not so crirical
			return FVA_ERROR_EMPTY_DEVICE_NAME;
		}
		else
		{
			if (deviceName.isEmpty())
				LOG_QDEB << "got first device name: " << newDeviceName << "in picture:" << info.absoluteFilePath();
			else if (deviceName != newDeviceName)
			{
				LOG_QCRIT << "got new device name: " << newDeviceName << "in picture:" << info.absoluteFilePath() << "old: " << deviceName;
				return FVA_ERROR_NON_UNIQUE_DEVICE_NAME;
			}
			deviceName = newDeviceName;
		}
	}

	return FVA_NO_ERROR;
};
