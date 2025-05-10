/*!
* \file CmdLineBaseTask.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CmdLineBaseTask.h"

#include <QtCore/QDebug>

FVA_EXIT_CODE CmdLineBaseTask::processFolderRecursivly(const std::string& folder, const CLTContext& context)
{
	QDir recurDir(folder.c_str());
	// qDebug() << "[BASE_CMD]dir for rec:" << folder;
	Q_FOREACH(QFileInfo info, recurDir.entryInfoList(QDir::NoDotAndDotDot|QDir::System|QDir::Hidden|QDir::AllDirs|QDir::Files,QDir::DirsFirst))
	{
		// just skip internal folder
		if ( info.isDir() && info.fileName()[0] == '#' && info.fileName()[info.fileName().size()-1] == '#' )
		{
			LOG_DEB << "skipped internal dir - " << info.absoluteFilePath() ;
			continue;
		}

		if ( info.isDir() )
		{
			// change folder context on each iteration
			m_folder = QDir::toNativeSeparators(info.filePath());
			m_dir = QDir( m_folder );
			FVA_EXIT_CODE res = processFolderRecursivly(QDir::toNativeSeparators(info.filePath()), context);
			RET_RES_IF_RES_IS_ERROR
			continue;
		}
	}
	m_folder = QDir::toNativeSeparators(folder);
	m_dir = QDir( m_folder );

	return execute(context);
}