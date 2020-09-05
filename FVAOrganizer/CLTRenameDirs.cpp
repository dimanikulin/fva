#include "CLTRenameDirs.h"

FVA_EXIT_CODE CLTRenameDirs::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
		{
			QString subFolderName = info.baseName().mid(0, 10);
			info.absoluteDir();
			QDir subdir(info.absoluteDir());

			QString oldPath = info.absoluteFilePath();
			QString newPath = info.absoluteFilePath().replace("-", ".");
			if (subdir.rename(oldPath, newPath))
				LOG_QWARN << "renamed folder from " << oldPath << " to " << newPath;
			else
				LOG_QCRIT << "cannot rename folder from " << oldPath << " to " << newPath;
		}
		else
			continue;
	}

	return FVA_NO_ERROR;
}
