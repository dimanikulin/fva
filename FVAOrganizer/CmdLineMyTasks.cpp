#include "CmdLineMyTasks.h"
#include <QXmlStreamReader>
#include "CSVFile.h"

#include "../lib/json/json.h"
#include "../lib/qexifimageheader.h"

FVA_ERROR_CODE CLT_Xml_Convert::execute()
{
	std::auto_ptr< QFile > xmlFile ( new QFile ( m_folder/*it is path to file*/ ) );
	
	if ( !xmlFile->open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		LOG_QCRIT << "input file can not be open";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	// Name,Place,People,Device,WhoTook,Description,Scaner
	// Name,Place,People,Device,WhoTook,Description,Scaner,Comment

	QXmlStreamReader xmlData( xmlFile.get() );
	while ( !xmlData.atEnd() )
	{
		QXmlStreamReader::TokenType token = xmlData.readNext();
		if ( token == QXmlStreamReader::StartDocument )
			continue;
		if ( xmlData.name() != "foto" )
			continue;
		QXmlStreamAttributes attributes = xmlData.attributes();
		
		QString name;
		if ( attributes.hasAttribute( "name" ) )
			name	= attributes.value ( "name" ).toString();

		QString file;
		if ( attributes.hasAttribute( "file" ) )
			file	= attributes.value ( "file" ).toString();

		QString comment;
		if ( attributes.hasAttribute( "comment" ) )
			comment	= attributes.value ( "comment" ).toString();

		// Name,Place,People,Device,WhoTook,Description,Scaner,Comment
		if ( !name.isEmpty() )
			qDebug() << file << ",,,,," << name << ",," << comment; 
	}

	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Update_File_Description::execute()
{
	QString descPath = m_folder + "/" + DESCRIPTION_FILE_NAME;
	if ( !m_dir.exists( descPath ) )
	{
		LOG_QCRIT << "description file does not exist";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	CSVFile descFile ( descPath );

	if ( !descFile.open() )
	{
		LOG_QCRIT << "description file can not be open";
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
	}
	QStringList						titles; 
	QMap< QString, QStringList >	decsItems;
	int								indexOfFileNameColumn = -1;
	while ( !descFile.atEnd() )
	{
		QString	line = descFile.readLine();
		if ( line.isEmpty() && descFile.atEnd() )
			break;
		QStringList	values;
		if ( !descFile.parseLine( line, values ) )
			continue;
		else
		{
			if ( titles.isEmpty() )
			{
				indexOfFileNameColumn = descFile.getColumnIdByName( values, "name" );
				if ( -1 == indexOfFileNameColumn )
				{
					LOG_QCRIT << "could not find mandatory column in description file";
					return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
				}
				titles = values;
				qDebug() << values;
				continue;
			}
			else
			{
				if ( values.size() != titles.size() )
				{
					LOG_QCRIT << "found (in description file) row with incorrect column length";
					return FVA_ERROR_INCORRECT_FORMAT;
				}
				decsItems [ values[ indexOfFileNameColumn ].toUpper() ] = values;
			}
		}
	}

	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		QString suffix = info.suffix().toUpper();

		if(	FVA_FILE_TYPE_IMG != convertFileExt2FileType ( suffix ) )
			continue;
		
		QString deviceName = QExifImageHeader( info.filePath()).value(QExifImageHeader::Make).toString()
								+ QExifImageHeader( info.filePath()).value(QExifImageHeader::Model).toString();
		
		QString deviceId;

		// to map subset of devices 
				if ( deviceName.isEmpty() )						deviceId = "0";
		else	if ( "CanonCanon PowerShot G2"	== deviceName )	deviceId = "8";
		else	if ( "CanonCanon EOS D60"		== deviceName )	deviceId = "85";

		QString oldName = info.baseName().mid( 21 ); // remove part of new name
		auto it = decsItems.find( oldName.toUpper() + "." + suffix.toUpper() );

		QString description;
		QString comment;

		if ( it != decsItems.end() )
		{
			int indexColumn = descFile.getColumnIdByName( titles, "Description" );
			if ( -1 == indexOfFileNameColumn )
			{
				LOG_QCRIT << "could not find Description column in description file";
				return FVA_ERROR_INCORRECT_FORMAT;
			}
			description = it.value()[ indexColumn ];

			indexColumn = descFile.getColumnIdByName( titles, "Comment" );
			if ( -1 == indexOfFileNameColumn )
			{
				LOG_QCRIT << "could not find Comment column in description file";
				return FVA_ERROR_INCORRECT_FORMAT;
			}
			comment = it.value()[ indexColumn ];
		}
		//Name,Place,People,Device,WhoTook,Description,Scaner,Comment,oldName
		qDebug() <<  info.baseName().mid( 0, 19 ) + "." + suffix << ",,," << deviceId << ",," << description << ",," << comment << "," << oldName + "." + suffix;
		QString newFileName = m_folder + "/" + info.baseName().mid( 0, 19 ) + "." + suffix;
		if ( !m_dir.rename( info.absoluteFilePath(), newFileName ) )
		{
			LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newFileName;
			return FVA_ERROR_CANT_RENAME_FILE;
		}
		else
			LOG_QDEB << "renamed file:" << info.absoluteFilePath() << " to:" << newFileName;
	}

	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Convert_Dir_Desc::execute()
{
	QString descFolderPath = m_folder + "/" + DESCRIPTION_FOLDER_NAME;
	if ( !m_dir.exists( descFolderPath ) )
	{
		LOG_QWARN << "description does not exist in:" << m_folder;
		return FVA_NO_ERROR;
	}
	QFile file ( descFolderPath );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		LOG_QCRIT << "can not find old description in:" << m_folder;
		return FVA_ERROR_CANT_OPEN_DIR_DESC;
	}
	
	QTextStream readStream( &file );

	// readStream.setCodec("IBM866");
	QString jsonData = readStream.readAll();
	jsonData.remove("{");
	jsonData.remove("}");
	jsonData = "{" + jsonData + "}";
	file.close();

	bool res = false;
	QVariantMap result = QtJson::Json::parse ( jsonData, res ).toMap();

	if ( res && !result["event"].toString().isEmpty() ) 
		LOG_QWARN << "dir:" << m_folder << ",event:" << result["event"].toString();

	if ( !m_readOnly )
	{
		if ( !m_dir.rename( descFolderPath , descFolderPath + "_old" ) )
		{
			LOG_QCRIT << "can not rename description in:" << m_folder;
			return FVA_ERROR_CANT_RENAME_DIR_DESC;
		}
		QFile fileNew ( descFolderPath );	
		if ( !fileNew.open( QIODevice::WriteOnly | QIODevice::Text ) )
		{
			LOG_QCRIT << "can not open new description in:" << m_folder;
			return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
		}
		QByteArray data = jsonData.toAscii();
		QTextStream writeStream( &fileNew );
		writeStream << jsonData;	
		writeStream.flush();
		fileNew.close();

		LOG_QWARN << "updated folder description:" << descFolderPath;
	}	
	return FVA_NO_ERROR;
}