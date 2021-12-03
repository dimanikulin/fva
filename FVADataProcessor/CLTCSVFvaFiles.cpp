/*!
* \file CLTCSVFvaFiles.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CLTCSVFvaFiles.h"
#include "../FVACommonLib/fvacommoncsv.h"

CLTCSVFvaFile::CLTCSVFvaFile(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTCSVFvaFile::execute(const CLTContext& context)
{	
	int ID = FVA_UNDEFINED_ID;
	FVA_EXIT_CODE res = fvaGetIDFromFile(m_rootSWdir + "#data#/fvaFile.id", ID);
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
		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,EventId,ReasonPeople,reserved1
		QString csvRecord =  QString::number(++ID) + "," // ID
			+ info.fileName() + ",,," // Name
			+ context.custom + ",,,,,,"; // m_custom here is device id
		records.append(csvRecord);
				
	}
	QFile fileNew(m_rootSWdir + "#data#/fvaFileN.csv");
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
	return fvaSaveIDInFile(m_rootSWdir + "#data#/fvaFile.id", ID);
}
