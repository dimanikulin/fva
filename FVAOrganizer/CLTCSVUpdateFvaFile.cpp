#include "CLTCSVUpdateFvaFile.h"

#include "FVADescriptionFile.h"

#include "fvacommonexif.h"

FVA_EXIT_CODE CLTCSVUpdateFvaFile::execute()
{
	QString descPath = m_folder + "/" + FVA_DESCRIPTION_FILE_NAME;
	if (!m_dir.exists(descPath))
	{
		LOG_QCRIT << "description file does not exist";
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	FVADescriptionFile descFile;

	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = descFile.load(descPath, titles, decsItems);
	if (FVA_NO_ERROR != res)
	{
		LOG_QCRIT << "description file can not be loaded";
		return res;
	}

	QString imageFilePrefix;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		QString suffix = info.suffix().toUpper();

		if (FVA_FS_TYPE_IMG != fvaConvertFileExt2FileType(suffix))
			continue;

		QString deviceName = fvaGetExifMakeAndModelFromFile(info.filePath());
		QString deviceId;

		// to map subset of devices 
		if (deviceName.isEmpty())						deviceId = "0";
		else	if ("CanonCanon PowerShot G2" == deviceName)	deviceId = "8";
		else	if ("CanonCanon EOS D60" == deviceName)	deviceId = "85";

		QString oldName = info.baseName().mid(21); // remove part of new name
		/*auto it = decsItems.find(oldName.toUpper() + "." + suffix.toUpper());

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
		*/

	}

	return FVA_NO_ERROR;
}
