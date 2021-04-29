#include "CmdLineBaseTask.h"

#include <QtCore/qdebug>

FVA_EXIT_CODE CmdLineBaseTask::processFolderRecursivly(const QString& folder, const CLTContext& context, const FvaConfiguration& cfg)
{
	QDir recurDir(folder);
	// qDebug() << "[BASE_CMD]dir for rec:" << folder;
	Q_FOREACH(QFileInfo info, recurDir.entryInfoList(QDir::NoDotAndDotDot|QDir::System|QDir::Hidden|QDir::AllDirs|QDir::Files,QDir::DirsFirst))
	{
		// just skip internal folder
		if ( info.isDir() && info.fileName()[0] == '#' && info.fileName()[info.fileName().size()-1] == '#' )
		{
			qDebug() << "skipped internal dir - " << info.absoluteFilePath() ;
			continue;
		}

		if ( info.isDir() )
		{
			// change folder context on each iteration
			m_folder = QDir::toNativeSeparators(info.filePath());
			m_dir = QDir( m_folder );
			FVA_EXIT_CODE res = processFolderRecursivly(QDir::toNativeSeparators(info.filePath()), context, cfg);
			RET_RES_IF_RES_IS_ERROR
			continue;
		}
	}
	m_folder = QDir::toNativeSeparators(folder);
	m_dir = QDir( m_folder );

	return execute(context, cfg);
}