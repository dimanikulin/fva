#include "CLTCSVFvaFiles.h"
#include "fvacommoncsv.h"
#include "fvadefaultcfg.h"

FVA_EXIT_CODE CLTCSVFvaFile::execute()
{	
	int ID = FVA_UNDEFINED_ID;
	FVA_EXIT_CODE res = fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR +"#data#/fvaFile.id", ID);
	RET_RES_IF_RES_IS_ERROR

	QList<QString>		records;	
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
		QString csvRecord =  QString::number(++ID) + "," // ID
			+ info.fileName() + ",,," // Name
			+ m_custom	+ ",,,,,,"; // m_custom here is device id
		records.append(csvRecord);
				
	}
	QFile fileNew ( FVA_DEFAULT_ROOT_DIR + "#data#/fvaFileN.csv" );		
	if ( !fileNew.open( QIODevice::WriteOnly | QIODevice::Text ) )	
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;	
	QTextStream writeStream( &fileNew );

	for ( auto it = records.begin(); it != records.end(); ++it )
		if (records.last() == *it) 
			writeStream << *it;
		else
			writeStream << *it << "\n";

	writeStream.flush();
	fileNew.close();	
	return fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR +"#data#/fvaFile.id", ID);
}
