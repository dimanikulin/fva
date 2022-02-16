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
	RET_IF_RES_IS_ERROR

	res = fvaLoadSimpleMapFromCsvByItemType(m_rootSWdir, m_fvaTagsTypeMap, "fvaTagTypes.csv");
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to fvaLoadSimpleMapFromCsvByItemType" ;
	RET_IF_RES_IS_ERROR

	res = fvaLoadSimpleMapFromCsvByItemType(m_rootSWdir, m_fvaPlaceTypesMap, "fvaPlaceTypes.csv");
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to fvaLoadSimpleMapFromCsvByItemType fvaPlaceTypes.csv" ;
	RET_IF_RES_IS_ERROR

	res = fvaLoadDictMapFromCsv(m_rootSWdir, fvaPlaceMap, "fvaPlaces.csv");
	if (res !=FVA_NO_ERROR) LOG_DEB << "Failed to fvaLoadDictMapFromCsv fvaPlace.csv" ;
	RET_IF_RES_IS_ERROR
}

FVA_EXIT_CODE CLTCSVGetTagsForFvaFiles::getFvaTagsForFile(const QString& fileName, QString& tags)
{
	// lets try to find it first
        fvaFile fvaFileItem;
	auto itFvaFileItem = m_fvaFileInfo.find(fileName.toUpper());
	if (itFvaFileItem != m_fvaFileInfo.end())
		fvaFileItem = *itFvaFileItem;
	else
	{
		LOG_CRIT << "fva item not found in fvaFile.csv - " << fileName ;	
		return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;	
	}

	//TODO make constant to tag types IDs

	if (m_SearchByPlace)
	{
		if (fvaFileItem.placeId != 0 && fvaFileItem.placeId != FVA_UNDEFINED_ID)
		{
			auto itPlace = fvaPlaceMap.find(fvaFileItem.placeId);
			if (itPlace == fvaPlaceMap.end())
			{
				LOG_CRIT << "place item not found in fvaPlaces.csv, ID - " << fvaFileItem.placeId ;	
				return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;
			}
		
			auto itPlaceType = fvaPlacesTypesMap(*itPlace.type);
			if (itPlaceType == fvaPlacesTypesMap.end())
			{
				LOG_CRIT << "place type item not found in fvaPlaceTypes.csv, type - " << *itPlace.type ;	
				return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;
			}
			tags +=	m_fvaTagsTypeMap[1] + "/" + itPlaceType.Value() + "/" + itPlace.Value();  
		}
	}

	if (m_SearchByAuthor)
	{
		tags +=	m_fvaTagsTypeMap[2] + "/";
	}

	if (m_SearchByEvent)
	{
		tags +=	m_fvaTagsTypeMap[3] + "/";
	}

	if (m_SearchByEventReasonPeople)
	{
		tags +=	m_fvaTagsTypeMap[4] + "/";
	}

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
