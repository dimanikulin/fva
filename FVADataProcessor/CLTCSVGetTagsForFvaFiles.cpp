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
	LOG_DEB << "cmd created, dir:" << m_folder;

	FVA_EXIT_CODE res = cfg.getParamAsString("Common::RootDir", m_rootSWdir);
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to load Common::RootDir with erorr - " << res;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by certain serach attribute
	res = cfg.getParamAsBoolean("Search::Place", m_SearchByPlace);
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to getParamAsBoolean(Search::Place) with erorr - " << res;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by device/author
	res = cfg.getParamAsBoolean("Search::Author", m_SearchByAuthor);
	if (res !=FVA_NO_ERROR)  LOG_CRIT << "Failed to getParamAsBoolean(Search::Author) with erorr - " << res;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by event
	res = cfg.getParamAsBoolean("Search::Event", m_SearchByEvent);
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to getParamAsBoolean(Search::Event) with erorr - " << res;
	RET_IF_RES_IS_ERROR

	// ask configuration if we need to search by EventReasonPeople
	res = cfg.getParamAsBoolean("Search::EventReasonPeople", m_SearchByEventReasonPeople);
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to getParamAsBoolean(Search::EventReasonPeople) with erorr - " << res;
	RET_IF_RES_IS_ERROR

	res = fvaLoadFvaFileInfoFromCsv(m_rootSWdir, m_fvaFileInfo, "fvaFileN.csv");	
	if (res !=FVA_NO_ERROR) 
		LOG_CRIT << "Failed to fvaLoadFvaFileInfoFromCsv with erorr - " << res;
	else
		LOG_CRIT << "fvaLoadFvaFileInfoFromCsv loaded " << m_fvaFileInfo.size() << "items" ;
	RET_IF_RES_IS_ERROR

	res = fvaLoadSimpleMapFromCsvByItemType(m_rootSWdir, m_fvaTagsTypeMap, "fvaTagTypes.csv");
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to fvaLoadSimpleMapFromCsvByItemType with erorr - " << res;
	RET_IF_RES_IS_ERROR

	res = fvaLoadSimpleMapFromCsvByItemType(m_rootSWdir, m_fvaPlaceTypesMap, "fvaPlaceTypes.csv");
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to fvaLoadSimpleMapFromCsvByItemType fvaPlaceTypes.csv with erorr - " << res;
	RET_IF_RES_IS_ERROR

	res = fvaLoadDictMapFromCsv(m_rootSWdir, m_fvaPlaceMap, "fvaPlaces.csv");
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to fvaLoadDictMapFromCsv fvaPlace.csv with erorr - " << res;
	RET_IF_RES_IS_ERROR

	res = fvaLoadDeviceMapFromCsv(m_rootSWdir, m_fvaDeviceMap);
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to fvaLoadDeviceMapFromCsv with erorr - " << res;
	RET_IF_RES_IS_ERROR

	res = fvaLoadPeopleMapFromCsv(m_rootSWdir, m_fvaPeopleMap);
	if (res !=FVA_NO_ERROR) LOG_CRIT << "Failed to fvaLoadPeopleMapFromCsv with erorr - " << res;
	RET_IF_RES_IS_ERROR
}
FVA_EXIT_CODE CLTCSVGetTagsForFvaFiles::getFvaTagsForFile(const QString& fileName, QString& tags)
{
	const QString TagTypeDelim = "~";

	// lets try to find it first
        fvaFile fvaFileItem;
	auto itFvaFileItem = m_fvaFileInfo.find(fileName.toUpper());
	if (itFvaFileItem != m_fvaFileInfo.end())
		fvaFileItem = *itFvaFileItem;
	else
	{
		LOG_CRIT << "fva item not found in fvaFileN.csv - " << fileName ;	
		return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;	
	}

	//TODO make constant to tag types IDs
	if (m_SearchByPlace 
		&& fvaFileItem.placeId != 0 && fvaFileItem.placeId != FVA_UNDEFINED_ID)
	{
		auto itPlace = m_fvaPlaceMap.find(fvaFileItem.placeId);
		if (itPlace == m_fvaPlaceMap.end())
		{
			LOG_CRIT << "place item not found in fvaPlaces.csv, ID - " << fvaFileItem.placeId ;	
			return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;
		}
		
		auto itPlaceType = m_fvaPlaceTypesMap.find(itPlace.value().type);
		if (itPlaceType == m_fvaPlaceTypesMap.end())
		{
			LOG_CRIT << "place type item not found in fvaPlaceTypes.csv, type - " << itPlace.value().type;	
			return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;
		}
		tags +=	TagTypeDelim + m_fvaTagsTypeMap[1] + "/" + itPlaceType.value() + "/" + itPlace.value().name;  
	}

	if (m_SearchByAuthor
		&& fvaFileItem.deviceId != 0 && fvaFileItem.deviceId != FVA_UNDEFINED_ID)
	{
		auto itDevice = m_fvaDeviceMap.find(fvaFileItem.deviceId);
		if (itDevice == m_fvaDeviceMap.end())
		{
			LOG_CRIT << "device not found in fvaDevices.csv, ID - " << fvaFileItem.deviceId ;	
			return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;
		}

		auto itPerson = m_fvaPeopleMap.find(itDevice.value().ownerId);
		if (itPerson == m_fvaPeopleMap.end())
		{
			LOG_CRIT << "person not found in fvaPeople.csv, ID - " << itDevice.value().ownerId ;	
			return FVA_ERROR_CANT_FIND_FVA_FILE_ITEM;
		}

		tags +=	TagTypeDelim + m_fvaTagsTypeMap[2] + "/" ;
	}

	if (m_SearchByEvent)
	{
		tags +=	TagTypeDelim + m_fvaTagsTypeMap[3] + "/";
	}

	if (m_SearchByEventReasonPeople)
	{
		tags +=	TagTypeDelim + m_fvaTagsTypeMap[4] + "/";
	}

	return FVA_NO_ERROR;
}

FVA_EXIT_CODE CLTCSVGetTagsForFvaFiles::execute(const CLTContext& context)
{			
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// just skip internal folder
		if (	info.isDir() 
			|| 
			info.isFile() && !fvaIsFVAFile ( info.suffix().toUpper()))
		{
			LOG_DEB << "skipped dir or internal fs object - " << info.absoluteFilePath() ;
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
