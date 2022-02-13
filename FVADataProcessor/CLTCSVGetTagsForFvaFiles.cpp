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
	res = cfg.getParamAsBoolean("Search::Place", m_SearchByPlace);
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to getParamAsBoolean(Search::Place)" ;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by device/author
	res = cfg.getParamAsBoolean("Search::Author", m_SearchByAuthor);
	if (res !=FVA_NO_ERROR)  LOG_DEB << "Failed to getParamAsBoolean(Search::Author)" ;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by event
	res = cfg.getParamAsBoolean("Search::Event", m_SearchByEvent);
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to getParamAsBoolean(Search::Event)" ;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by EventReasonPeople
	res = cfg.getParamAsBoolean("Search::EventReasonPeople", m_SearchByEventReasonPeople);
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to getParamAsBoolean(Search::EventReasonPeople)" ;
	RET_IF_RES_IS_ERROR

	res = fvaLoadFvaFileInfoFromCsv(m_rootSWdir, m_fvaFileInfo, "fvaFileN.csv");
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to fvaLoadFvaFileInfoFromCsv" ;
}

FVA_EXIT_CODE CLTCSVGetTagsForFvaFiles::getFvaTagsForFile(const QString fileName, QString& tags)
{
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE CLTCSVGetTagsForFvaFiles::execute(const CLTContext& context)
{			
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
		QString fvaTags;
		FVA_EXIT_CODE res = getFvaTagsForFile(info.fileName(), fvaTags); 
		if (FVA_NO_ERROR != res)
			return res;
		// full path to tags		
		QString csvRecord =  info.absoluteFilePath() + ",\"" + fvaTags + "\"";
		m_records.append(csvRecord);
				
	}
	return FVA_NO_ERROR;
}
CLTCSVGetTagsForFvaFiles::~CLTCSVGetTagsForFvaFiles()	
{
	fvaSaveStrListToFile(m_rootSWdir + "#data#/fvaFileTags.csv", m_records);

	LOG_DEB << "cmd deleted, dir:" << m_folder; 
}
