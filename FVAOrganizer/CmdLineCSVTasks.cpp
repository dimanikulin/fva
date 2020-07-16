#include "CmdLineCSVTasks.h"
#include "fvacommonlib.h"
#include "fvadefaultcfg.h"

FVA_ERROR_CODE CLT_Fva_Folder_2_CSV::execute()
{
	int ID = FVA_UNDEFINED_ID;
	FVA_ERROR_CODE error = fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR +"fvaFolder.id", ID);
	if (error != FVA_NO_ERROR)
		return error;
	QString csvRecord =  QString(ID + 1) + "," // ID
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
	return fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR +"fvaFolder.id", ID + 1);
}

FVA_ERROR_CODE CLT_Fva_Files_2_CSV::execute()
{	
	return FVA_NO_ERROR;
}

