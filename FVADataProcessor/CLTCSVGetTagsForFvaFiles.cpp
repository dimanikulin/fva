/*!
* \file CLTCSVGetTagsForFvaFiles.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2022
*/
#include "CLTCSVGetTagsForFvaFiles.h"
#include "fvacommoncsv.h"

CLTCSVGetTagsForFvaFiles::CLTCSVGetTagsForFvaFiles(const FvaConfiguration& cfg)
{
	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to load Common::RootDir" ;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by certain serach attribute
	exitCode = cfg.getParamAsBoolean("Search::Place", m_SearchByPlace);
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to getParamAsBoolean(Search::Place)" ;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by device/author
	exitCode = cfg.getParamAsBoolean("Search::Author", m_SearchByAuthor);
	if (res !=FVA_NO_ERROR)  LOG_DEB << "Failed to getParamAsBoolean(Search::Author)" ;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by event
	exitCode = cfg.getParamAsBoolean("Search::Event", m_SearchByEvent);
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to getParamAsBoolean(Search::Event)" ;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by EventReasonPeople
	exitCode = cfg.getParamAsBoolean("Search::EventReasonPeople", m_SearchByEventReasonPeople);
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to getParamAsBoolean(Search::EventReasonPeople)" ;
	RET_IF_RES_IS_ERROR

	res = fvaLoadFvaFileInfoFromCsv(m_rootSWdir, m_fvaFileInfo, "fvaFileN.csv");
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to fvaLoadFvaFileInfoFromCsv" ;
}

FVA_EXIT_CODE CLTCSVGetTagsForFvaFiles::execute(const CLTContext& context)
{	
	/*QList<QString>		records;	
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// just skip internal folder
		if ( ( info.isDir() && info.fileName()[0] == '#' && info.fileName()[info.fileName().size()-1] == '#' )
			|| 
			info.isFile() && !fvaIsFVAFile ( info.suffix().toUpper()))
		{
			LOG_DEB << "skipped internal fs object - " << info.absoluteFilePath() ;
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
	fileNew.close();*/
	return FVA_NO_ERROR;
}
