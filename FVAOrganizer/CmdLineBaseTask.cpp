#include "CmdLineBaseTask.h"

#include <qdebug>

CmdLineBaseTask::CmdLineBaseTask( const QString& folder_, bool readOnly, const QString& custom_ )
	:	m_folder		( folder_ ),
		m_custom		( custom_ ),
		m_readOnly		( readOnly ),
		m_baseFolder	( folder_ )
{
	m_dir = QDir ( m_folder );
}

CmdLineBaseTask::~CmdLineBaseTask( )
{

}


FVA_ERROR_CODE CmdLineBaseTask::processFolderRecursivly( const QString& folder )
{
	QDir recurDir(folder);
	// qDebug() << "[BASE_CMD]dir for rec:" << folder;
	Q_FOREACH(QFileInfo info, recurDir.entryInfoList(QDir::NoDotAndDotDot|QDir::System|QDir::Hidden|QDir::AllDirs|QDir::Files,QDir::DirsFirst))
	{		
		if ( info.isDir() )
		{
			// change folder context on each iteration
			m_folder = info.filePath();
			m_dir = QDir( m_folder );
			FVA_ERROR_CODE res = processFolderRecursivly( info.filePath() );
			if ( res != FVA_NO_ERROR )
				return res;
			continue;
		}
	}
	m_folder = folder;
	m_dir = QDir( m_folder );

	return execute();
}