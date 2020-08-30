#include "CmdLineDBTasks.h"
#include "fvadefaultcfg.h"
#include "FVADescriptionFile.h"


CLT_Fs_To_SQL::~CLT_Fs_To_SQL()
{
	SaveSQL(FVA_DEFAULT_ROOT_DIR + "11.fva.sql");
	qDebug() << "totally inserted - " << m_SQLs.size() << ", totally skipped - " << m_skippedFiles;
}
FVA_EXIT_CODE CLT_Fs_To_SQL::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// just skip internal folder
		if ( ( info.isDir() && info.fileName()[0] == '#' && info.fileName()[info.fileName().size()-1] == '#' )
			|| 
			info.isFile() && !fvaIsFVAFile ( info.suffix().toUpper()))
		{
			qDebug() << "skipped internal fs object - " << info.absoluteFilePath() ;
			m_skippedFiles++;
			continue;
		}
		//ID,Name,Path,Type,ReadOnly,Size,DataModified,DataCreated,FvaFileId,FvaFolderId,FsImageId,FsVideoId,FsAudioId
#define SPRT ","
		QString insert =  "insert into fva values ((select max(ID)+1 from fva),\"" // ID
			+ info.fileName().toUpper() + "\"" + SPRT // Name
			+ "\"" + FVA_TARGET_FOLDER_NAME + "\"" + SPRT // Path
			+ QString::number(info.isDir()?FVA_FS_TYPE_DIR:fvaConvertFileExt2FileType(info.suffix().toUpper())) + SPRT // Type
			+ (info.isWritable() ? "0":"1") + SPRT // ReadOnly
			+ QString::number(info.size()) + SPRT // Size
			+ "\"" + info.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz") + "\"" + SPRT // DataModified
			+ "\"" + info.created().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) + "\"" + SPRT // DataCreated
			+ "-1" + SPRT // FvaFileId
			+ "-1" + SPRT // FvaFolderId 
			+ "-1" + SPRT // FsImageId 
			+ "-1" + SPRT // FsVideoId
			+ "-1"		  // FsAudioId
			+ ");";
		m_SQLs.push_front(insert);
	}
	return FVA_NO_ERROR;
}
