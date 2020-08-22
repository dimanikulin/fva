#include "CmdLineDBTasks.h"
#include "fvadefaultcfg.h"
#include "FVADescriptionFile.h"

void CLT_Base_SQL::SaveSQL( const QString& fileToSaveIn )
{
	QFile fileNew ( fileToSaveIn );	
	if ( !fileNew.open( QIODevice::Append | QIODevice::Text ) )
	{
		LOG_QCRIT << "can not create fva sql for " << m_folder;
		return;
	}
	QTextStream writeStream( &fileNew );
	writeStream.setCodec("UTF-8");
	writeStream << "BEGIN TRANSACTION;" << "\n";
	for ( auto it = m_SQLs.begin(); it != m_SQLs.end(); ++it )
		writeStream << *it << "\n";	
	writeStream << "COMMIT;";
	writeStream.flush();
	fileNew.close();
}

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
CLT_Fva_File_To_SQL::~CLT_Fva_File_To_SQL()
{
	SaveSQL( "13.fvaFile.sql" );

	for ( auto it = m_movedFiles.begin(); it != m_movedFiles.end(); ++it )
	{
		QFileInfo info( *it );
		if ( !m_dir.rename( *it + "/" + FVA_DESCRIPTION_FILE_NAME, 
						m_folder + "/" + info.fileName() + ".csv" ))
		{
			LOG_QCRIT << "can not move file fva desc for " << *it;
			continue;
		}
		LOG_QWARN	<< "moved from - "	<< *it + "/" + FVA_DESCRIPTION_FILE_NAME
					<< " to - "			<< m_folder + "/" + info.fileName() + ".csv";
	}

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2 << ", totally moved - " << m_movedFiles.size();
}
QString FVget( FVADescriptionFile* desc, const QString& fieldName, const QStringList& titles, DESCRIPTIONS_MAP::Iterator it, QStringList& titlesToCheck )
{
	int indexColumn = desc->getColumnIdByName(titles,fieldName);
	if ( -1 == indexColumn )
	{
		// qCritical() << "could not find " << fieldName << " column in description file";
		return "";
	}
	titlesToCheck[ indexColumn ] = "";
	return it.value()[ indexColumn ];
}

FVA_EXIT_CODE CLT_Fva_File_To_SQL::execute()
{
	std::auto_ptr<FVADescriptionFile> desc( new FVADescriptionFile );
	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = desc->load( m_folder + "/" + FVA_DESCRIPTION_FILE_NAME, titles, decsItems );
	RET_RES_IF_RES_IS_ERROR // yes, it is NOT an error for this CLT
	
	QStringList			t2Check = titles; // titlesToCheck 
	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,OldName,WhoTook,OldName1
		QString insert =  "insert into fvaFile values ((select max(ID)+1 from fvaFile),\""
		+ FVget( desc.get(),"Name",titles, it, t2Check ).toUpper()		+ "\",\""
		+ FVget( desc.get(),"Place",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"People",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"Device",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"Description",titles, it, t2Check )	+ "\",\""
		+ FVget( desc.get(),"Scaner",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"Comment",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"oldName",titles, it, t2Check ).toUpper()		+ "\",\""
		+ FVget( desc.get(),"WhoTook",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"oldName1",titles, it, t2Check ).toUpper()	+ "\");";

		m_SQLs.push_back( insert );

		QString update;
		QFileInfo info(m_folder);
		update = "update fva set FvaFileId = (select max(ID) from fvaFile) where Path || \"/\" ||  Name = \"" 
				+ info.absoluteFilePath ().toUpper()
				+ "/" 
				+ FVget( desc.get(),"Name",titles, it, t2Check ).toUpper() 
				+ "\";";
		m_SQLs.push_back(update);
	}

	for ( int id = 0; id < t2Check.size(); ++id )
	{
		if ( t2Check[ id ].length() > 0 )
			return FVA_ERROR_INCORRECT_FORMAT;
	}
	// move dir description file for future backup purpose
	m_movedFiles.push_back( m_folder );
	
	LOG_QWARN << "converted file description to SQL:" << m_dir.dirName();
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE CLT_Create_FVA_SQL::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// just skip internal folder
		if ( ( info.isDir() && info.fileName()[0] == '#' && info.fileName()[info.fileName().size()-1] == '#' )
			|| 
			info.isFile() && !fvaIsFVAFile ( info.suffix().toUpper()))
		{
			qDebug() << "skipped internal fs object - " << info.absoluteFilePath() ;
			continue;
		}
		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,OldName,WhoTook,OldName1
		QString insert =  "insert into fvaFile values ((select max(ID)+1 from fvaFile),\"\",\"\",\"\",\""
		+ m_custom	+ "\",\"\",\"\",\"\",\"\",\"\",\"\");"; // m_custom here is device id

		m_SQLs.push_back( insert );

		QString update;
		update = "update fva set FvaFileId = (select max(ID) from fvaFile) where Path || \"/\" ||  Name = \"" 
				+ FVA_TARGET_FOLDER_NAME
				+ "/" 
				+ info.fileName().toUpper() 
				+ "\";";
		m_SQLs.push_back(update);
	}
	return FVA_NO_ERROR;
}
CLT_Create_FVA_SQL::~CLT_Create_FVA_SQL()	
{
	SaveSQL( "13.fvaFile.sql" );

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2;
}