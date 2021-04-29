#include "CLTGetFvaDirType.h"

FVA_EXIT_CODE CLTGetFvaDirType::execute(const CLTContext& /*context*/, const FvaConfiguration& /*cfg*/)
{
	uint dirNumber = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (!info.isDir())
			// skip not dir
			continue;

		// get the last dir leaf in input folder
		QString dir = m_dir.dirName();
		// skip internal folder 
		if (dir.contains("#copy"))
		{
			LOG_QWARN << "skipped internal folder: " << info.absoluteFilePath();
			continue;
		}

		dirNumber++;
	}
	if (!dirNumber)
		return FVA_NOT_FVA_FOLDER;
	if (1 == dirNumber)
		return FVA_1_EVENT_1_DAY;
	if (dirNumber > 1)
		return FVA_FEW_EVENTS_FEW_DAYS;
	else
		return FVA_NO_ERROR;

	// TODO to impelemnt FVA_FEW_EVENTS_1_DAY
}