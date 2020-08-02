#include "CmdLineMyTasks.h"
#include <QtCore/QXmlStreamReader>
#include "FVADescriptionFile.h"

#include "../FVACommonLib/qexifimageheader.h"

#include <QtCore/qdir>

FVA_ERROR_CODE CLT_Xml_Convert::execute()
{
	std::auto_ptr< QFile > xmlFile ( new QFile ( m_folder/*it is path to file*/ ) );
	
	if ( !xmlFile->open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		LOG_QCRIT << "input file can not be open";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	// Name,Place,People,Device,Description,Scaner
	// Name,Place,People,Device,Description,Scaner,Comment

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

		// Name,Place,People,Device,Description,Scaner,Comment
		if ( !name.isEmpty() )
			qDebug() << file << ",,,," << name << ",," << comment; 
	}

	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Update_File_Description::execute()
{
	QString descPath = m_folder + "/" + FVA_DESCRIPTION_FILE_NAME;
	if ( !m_dir.exists( descPath ) )
	{
		LOG_QCRIT << "description file does not exist";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	FVADescriptionFile descFile;

	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;
	FVA_ERROR_CODE res = descFile.load( descPath, titles, decsItems );
	if ( FVA_NO_ERROR != res )
	{
		LOG_QCRIT << "description file can not be loaded";
		return res;
	}

	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		QString suffix = info.suffix().toUpper();

		if(	FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType ( suffix ) )
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
			if ( -1 == indexColumn )
			{
				LOG_QCRIT << "could not find Description column in description file";
				return FVA_ERROR_INCORRECT_FORMAT;
			}
			description = it.value()[ indexColumn ];

			indexColumn = descFile.getColumnIdByName( titles, "Comment" );
			if ( -1 == indexColumn )
			{
				LOG_QCRIT << "could not find Comment column in description file";
				return FVA_ERROR_INCORRECT_FORMAT;
			}
			comment = it.value()[ indexColumn ];
		}
		//Name,Place,People,Device,Description,Scaner,Comment,oldName
		qDebug() <<  info.baseName().mid( 0, 19 ) + "." + suffix << ",,," << deviceId << "," << description << ",," << comment << "," << oldName + "." + suffix;
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
	QString descFolderPath = m_folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
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
	/*QVariantMap result = QtJson::Json::parse ( jsonData, res ).toMap();

	if ( res && !result["event"].toString().isEmpty() ) 
		LOG_QWARN << "dir:" << m_folder << ",event:" << result["event"].toString();

	if ( !m_readOnly )
	{
		if ( !m_dir.rename( descFolderPath , descFolderPath + "_old" ) )
		{
			LOG_QCRIT << "can not rename description in:" << m_folder;
			return FVA_ERROR_CANT_RENAME_DIR_DESC;
		}
		QString error;
		FVA_ERROR_CODE res = fvaCreateFolderDescription( descFolderPath, result, error );
		if ( FVA_NO_ERROR != res )
		{
			LOG_QCRIT << error;
			return res;
		}

		LOG_QWARN << "updated folder description:" << descFolderPath;
	}*/	
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Rename_File_By_Desc::execute()
{
	QString descPath = m_folder + QDir::separator() + FVA_DESCRIPTION_FILE_NAME;
	if ( !m_dir.exists( descPath ) )
	{
		LOG_QCRIT << "description file does not exist";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	FVADescriptionFile descFile;

	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;
	FVA_ERROR_CODE res = descFile.load( descPath, titles, decsItems );
	if ( FVA_NO_ERROR != res )
	{
		LOG_QCRIT << "description file can not be loaded";
		return res;
	}

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		int indexColumn = descFile.getColumnIdByName( titles, "Name" );
		if ( -1 == indexColumn )
		{
			LOG_QCRIT << "could not find Name column in description file";
			return FVA_ERROR_INCORRECT_FORMAT;
		}
		
		QString newName = it.value()[ indexColumn ];		
		indexColumn = descFile.getColumnIdByName( titles, "oldName" );
		
		if ( -1 == indexColumn )
		{
			LOG_QCRIT << "could not find oldName column in description file";			
			return FVA_ERROR_INCORRECT_FORMAT;
		}
		QString oldName = it.value()[ indexColumn ];

		if (m_dir.exists(m_folder + QDir::separator() + oldName))
		{
			if ( !m_dir.rename( m_folder + QDir::separator() + oldName, m_folder + QDir::separator() + newName ) )
			{
				LOG_QCRIT << "cant move file:" << m_folder << QDir::separator() << oldName << " into " << m_folder << QDir::separator() << newName;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
			{
				LOG_QCRIT << "moved file:" << m_folder << QDir::separator() << oldName << " into " << m_folder << QDir::separator() << newName;
			}
		}
	}

	return FVA_NO_ERROR;
}