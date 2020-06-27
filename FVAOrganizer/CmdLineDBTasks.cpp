#include "CmdLineDBTasks.h"

CLT_Fs_To_SQL::~CLT_Fs_To_SQL()
{
	QFile fileNew ( m_folder + QDir::separator() + "11.fva.sql" );	
	if ( !fileNew.open( QIODevice::Append | QIODevice::Text ) )
	{
		LOG_QCRIT << "can not create fva sql for " << m_folder;
		// return FVA_ERROR_CANT_CREATE_FVA_SQL;
	}
	QTextStream writeStream( &fileNew );
	writeStream << "BEGIN TRANSACTION;" << "\n";
	for ( auto it = m_inserts.begin(); it != m_inserts.end(); ++it )
		writeStream << *it << "\n";	
	writeStream << "COMMIT;";
	writeStream.flush();
	fileNew.close();
	qDebug() << "totally inserted - " << m_inserts.size() << ", totally skipped - " << m_skippedFiles;
}
FVA_ERROR_CODE CLT_Fs_To_SQL::execute()
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
			+ info.fileName() + "\"" + SPRT // Name
			+ "\"" + info.absolutePath() + "\"" + SPRT // Path
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
		m_inserts.push_front(insert);
	}
	return FVA_NO_ERROR;
}
CLT_Fva_Folder_To_SQL::~CLT_Fva_Folder_To_SQL()
{
	QFile fileNew ( m_folder + QDir::separator() + "12.fvaFolder.sql" );	
	if ( !fileNew.open( QIODevice::Append | QIODevice::Text ) )
	{
		LOG_QCRIT << "can not create fva folder sql for " << m_folder;
		// return FVA_ERROR_CANT_CREATE_FVA_SQL;
	}
	QTextStream writeStream( &fileNew );
	writeStream.setCodec("UTF-8");
	writeStream << "BEGIN TRANSACTION;" << "\n";
	for ( auto it = m_inserts.begin(); it != m_inserts.end(); ++it )
		writeStream << *it << "\n";	
	writeStream << "COMMIT;";
	writeStream.flush();
	fileNew.close();
	qDebug() << "totally inserted - " << m_inserts.size() / 2 << ", totally moved - " << m_movedFolders;
}
QString FVget( const QString& fieldName, QVariantMap& result )
{
	QString fieldValue = "";
	if ( result.contains( fieldName ) )
	{
		fieldValue = result[fieldName].toString();
		result.remove(fieldName);
	}

	return fieldValue;
}
FVA_ERROR_CODE CLT_Fva_Folder_To_SQL::execute()
{
	QVariantMap		dirDesc;
	QString			error;
	FVA_ERROR_CODE	code = fvaGetFolderDescription( m_folder, dirDesc, error );
	if ( FVA_NO_ERROR != code )
		return FVA_NO_ERROR; // yes, it is NOT an error for this CLT

	m_movedFolders++;	
	//ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder
	QString insert =  "insert into fvaFolder values ((select max(ID)+1 from fvaFolder),\"" 
		+ m_dir.dirName() + "\"," 
		+ FVget("deviceId", dirDesc) + ",\""
		+ FVget("tags", dirDesc) + "\",\""
		+ FVget("people", dirDesc) + "\",\""
		+ FVget("place", dirDesc) + "\",\""
		+ FVget("event", dirDesc) + "\",\""
		+ FVget("reasonPeople", dirDesc) + "\",\""
		+ FVget("linkedFolder", dirDesc) + "\",\""
		+ FVget("whoTookFotoId", dirDesc) + "\",\""
		+ FVget("scaner", dirDesc) + "\");";
	m_inserts.push_back(insert);	
	QString update;
	QFileInfo info(m_folder);
	update = "update fva set FvaFolderId = (select max(ID) from fvaFolder) where Path || \"/\" ||  Name = \"" + info.absoluteFilePath() + "\";";
	m_inserts.push_back(update);
	
	if (dirDesc.size())
	{
		return FVA_ERROR_INCORRECT_FORMAT;
	}

	// move dir description file for future backup purpose
		// QString fullDirDescPath = m_folder + QDir::separator() + FVA_DIR_DESCRIPTION_FILE_NAME; 
		// m_dir.rename(fullDirDescPath, fullDirDescPath +"_"+ m_dir.dirName() + "(" + result["tags"].toString() +")");

		// LOG_QWARN << "converted folder description to SQL:" << m_folder;
	
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Fva_File_To_SQL::execute()
{
	return FVA_NO_ERROR;
}


FVA_ERROR_CODE CLT_Fsaudio_To_SQL::execute()
{
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Fsvideo_To_SQL::execute()
{
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Fsimage_To_SQL::execute()
{
	return FVA_NO_ERROR;
}

/*
FVA_ERROR_CODE CLT_Desc_To_SQL::execute()
{
	std::auto_ptr<FVADescriptionFile> desc( new FVADescriptionFile );
	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;

	FVA_ERROR_CODE res = desc->load( m_folder + QDir::separator() + FVA_DESCRIPTION_FILE_NAME, titles, decsItems );
	
	if ( FVA_NO_ERROR == res )
	{
		QFile fileNew ( m_folder + QDir::separator() + "fvafile.sql" );	
		if ( !fileNew.open( QIODevice::WriteOnly | QIODevice::Text ) )
		{
			error = "can not create new dir fva sql";
			return FVA_ERROR_CANT_CREATE_FVA_SQL;
		}

		QTextStream writeStream( &fileNew );

		for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
		{
			QString insert = "insert into fva values ((select max(ID)+1 from fva)," ;
			int indexColumn = -1;
#define FILL_SQL_INSERT(FIELD,LAST) \
			indexColumn = desc->getColumnIdByName(titles, FIELD);\
			if (-1==indexColumn)\
			{\
				LOG_QCRIT << "could not find" << FIELD << "column in description file";\
				return FVA_ERROR_INCORRECT_FORMAT;\
			}\
			insert += "\"" + it.value()[ indexColumn ] + "\"" + ((LAST==true)?");":",");

			FILL_SQL_INSERT("Name",false)
			insert += "\"2\",";
			FILL_SQL_INSERT("Device",false)
			FILL_SQL_INSERT("Place",false)
			FILL_SQL_INSERT("People",false)
			insert += "\"\",\"\",\"\",\"\",\"\",";
			FILL_SQL_INSERT("Description",false)
			FILL_SQL_INSERT("Scaner",false)
			FILL_SQL_INSERT("Comment",false)
			FILL_SQL_INSERT("oldName",true)

			writeStream << insert << "\n";
		}
		writeStream.flush();
		fileNew.close();

		// rename file description for future backup purpose
		desc.reset(nullptr);
		QString fullFileDescPath = m_folder + QDir::separator() + FVA_DESCRIPTION_FILE_NAME; 
		if (!m_dir.rename(fullFileDescPath, fullFileDescPath +"_"+ m_dir.dirName()))
		{
			LOG_QCRIT << "can not rename file description in:" << m_folder;
			return FVA_ERROR_CANT_RENAME_FILE_DESC;

		}

		LOG_QWARN << "converted file description to SQL:" << m_dir.dirName();
	}
	return FVA_NO_ERROR;
}
*/