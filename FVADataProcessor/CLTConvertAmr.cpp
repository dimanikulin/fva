/*!
* \file CLTConvertAmr.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTConvertAmr.h"

FVA_EXIT_CODE CLTConvertAmr::execute(const CLTContext& /*context*/)
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		if (suffix == "AMR")
		{
			LOG_QCRIT << "found not supported file:" << info.absoluteFilePath();
			return FVA_ERROR_NOT_SUPPORTED_FILE;
		}
	}
	return FVA_NO_ERROR;
}
