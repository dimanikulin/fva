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
CLT_Fva_Folder_To_SQL::~CLT_Fva_Folder_To_SQL()
{
	SaveSQL( "12.fvaFolder.sql" );
	
	for ( auto it = m_movedFolders.begin(); it != m_movedFolders.end(); ++it )
	{
		QFileInfo info( *it );
		if ( !m_dir.rename( *it + "/" + FVA_DIR_DESCRIPTION_FILE_NAME, 
						m_folder + "/" + info.fileName() + ".json" ))
		{
			LOG_QCRIT << "can not move fva desc for " << *it;
			continue;
		}
		LOG_QWARN	<< "moved from - "	<< *it + "/" + FVA_DIR_DESCRIPTION_FILE_NAME
					<< " to - "			<< m_folder + "/" + info.fileName() + ".json";
	}

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2 << ", totally moved - " << m_movedFolders.size();	
}

FVA_EXIT_CODE CLT_Fva_Folder_To_SQL::execute()
{
	QVariantMap		dirDesc;
	QString			error;
	/*FVA_EXIT_CODE	code = fvaGetFolderDescription( m_folder, dirDesc, error );
	RET_IF_RES_NO_ERROR // yes, it is NOT an error for this CLT
		*/
	//ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder
	QString insert =  "insert into fvaFolder values ((select max(ID)+1 from fvaFolder),\"" 
		+ m_dir.dirName()						+ "\"," 
		+ fvaDVget( "deviceId", dirDesc)		+ ",\""
		+ fvaDVget( "tags", dirDesc )			+ "\",\""
		+ fvaDVget( "people", dirDesc )			+ "\",\""
		+ fvaDVget( "place", dirDesc )			+ "\",\""
		+ fvaDVget( "event", dirDesc )			+ "\",\""
		+ fvaDVget( "reasonPeople", dirDesc )	+ "\",\""
		+ fvaDVget( "linkedFolder", dirDesc )	+ "\",\""
		+ fvaDVget( "whoTookFotoId", dirDesc )	+ "\",\""
		+ fvaDVget( "scaner", dirDesc)			+ "\");";
	m_SQLs.push_back(insert);	
	QString update;
	QFileInfo info(m_folder);
	update = "update fva set FvaFolderId = (select max(ID) from fvaFolder) where Path || \"/\" ||  Name = \"" + info.absoluteFilePath() + "\";";
	m_SQLs.push_back(update);
	
	if ( dirDesc.size() )
	{
		return FVA_ERROR_INCORRECT_FORMAT;
	}

	// move dir description file for future backup purpose
	m_movedFolders.push_back( m_folder );
	LOG_QWARN << "converted folder description to SQL:" << m_folder;
	return FVA_NO_ERROR;
}