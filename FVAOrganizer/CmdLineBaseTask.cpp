#include "CmdLineBaseTask.h"

#include <qdebug>

CmdLineBaseTask::CmdLineBaseTask( const QString& folder_, bool readOnly, const QString& custom_ )
	:	m_folder	( folder_ ),
		m_custom	( custom_ ),
		m_readOnly	( readOnly )
{
	m_dir = QDir ( m_folder );
}

CmdLineBaseTask::~CmdLineBaseTask( )
{

}

bool CmdLineBaseTask::isInternalFileName( const QString& fileName )
{
	return (	fileName.toUpper() == DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == BACKGROUND_MUSIC_FILE_NAME.toUpper() 
			||	fileName.toUpper() == OLD_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == OLD_DESCRIPTION_FILE_NAME.toUpper() ) ;
}
FVA_FILE_TYPE CmdLineBaseTask::convertFileExt2FileType ( const QString& type )
{
	if ( type == "JPG" || type == "JPEG" || type == "PNG" || type == "BMP" || type == "GIF" )
		return FVA_FILE_TYPE_IMG;
	
	if ( type == "AVI" ||	type == "MOV" || type == "MPG" || type == "MP4" || type == "3GP" )
		return FVA_FILE_TYPE_VIDEO;

	if ( type == "WAV" )
		return FVA_FILE_TYPE_AUDIO;

	return FVA_FILE_TYPE_UNKNOWN;
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