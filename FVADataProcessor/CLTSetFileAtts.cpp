/*!
* \file CLTSetFileAtts.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTSetFileAtts.h"

#include <windows.h>
#include <winbase.h>

FVA_EXIT_CODE CLTSetFileAtts::execute(const CLTContext& /*context*/)
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType(suffix);
		if (FVA_FS_TYPE_UNKNOWN != type)
		{
			if (!SetFileAttributes(info.absoluteFilePath().toStdWString().c_str(), FILE_ATTRIBUTE_READONLY))
				LOG_QCRIT << "can not set attr for fva file:" << info.absoluteFilePath();
		}
		else
		{
			if (fvaIsInternalFile(info.fileName()))
			{
				if (!SetFileAttributes(info.absoluteFilePath().toStdWString().c_str(), /*FILE_ATTRIBUTE_HIDDEN |*/ FILE_ATTRIBUTE_READONLY))
					LOG_QCRIT << "can not set attr for internal file:" << info.absoluteFilePath();
			}
			else
			{
				LOG_QCRIT << "found not supported file:" << info.absoluteFilePath();
				return FVA_ERROR_NOT_SUPPORTED_FILE;
			}
		}
	}

	return FVA_NO_ERROR;
}
