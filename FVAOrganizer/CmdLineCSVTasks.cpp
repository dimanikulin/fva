#include "CmdLineCSVTasks.h"
#include "fvacommonlib.h"
#include "fvadefaultcfg.h"

FVA_ERROR_CODE CLT_Fva_Folder_2_CSV::execute()
{
	int ID = FVA_UNDEFINED_ID;
	FVA_ERROR_CODE error = fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR +"fvaFolder.id", ID);
	if (error != FVA_NO_ERROR)
		return error;
	QString csvRecord =  QString(ID++) + "," // ID
		+ FVA_TARGET_FOLDER_NAME		+ "," // Name	
		+ m_custom						// DevId
		+ ",,,,,,,,";	//Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder,

	QFile fileNew ( FVA_DEFAULT_ROOT_DIR + "fvaFolderN.csv" );		
	if ( !fileNew.open( QIODevice::WriteOnly | QIODevice::Text ) )	
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;	
	QTextStream writeStream( &fileNew );	
	writeStream << csvRecord;	
	writeStream.flush();	
	fileNew.close();	
	return fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR +"fvaFolder.id", ID);
}

FVA_ERROR_CODE CLT_Fva_Files_2_CSV::execute()
{	
	int ID = FVA_UNDEFINED_ID;
	FVA_ERROR_CODE error = fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR +"fvaFile.id", ID);
	if (error != FVA_NO_ERROR)
		return error;

	QFile fileNew ( FVA_DEFAULT_ROOT_DIR + "fvaFileN.csv" );		
	if ( !fileNew.open( QIODevice::WriteOnly | QIODevice::Text ) )	
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;	
	QTextStream writeStream( &fileNew );

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
		QString csvRecord =  QString(ID++) + "," // ID
			+ info.fileName() + ",,," // Name
			+ m_custom	+ ",,,,,,"; // m_custom here is device id
		writeStream << csvRecord;		
	}
	writeStream.flush();
	fileNew.close();	
	return fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR +"fvaFile.id", ID + 1);
}
