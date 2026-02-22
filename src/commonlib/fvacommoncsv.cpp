/*!
* \file fvacommoncsv.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvacommoncsv.h"
#include "FVADescriptionFile.h"
#include "fvafile.h"
#include "fvadevice.h"
#include "fvalogger.inl"
#include "fvacommonlib.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>

FVA_EXIT_CODE fvaGetIDFromFile(const QString& fileName, int& ID)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return FVA_ERROR_CANT_OPEN_ID_FILE;
	QTextStream readStream(&file);
	readStream >> ID;
	file.close();
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaSaveIDInFile(const QString& fileName, int ID)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
	QTextStream writeStream(&file);
	writeStream << ID;
	writeStream.flush();
	file.close();
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadFvaFileInfoFromCsv(const QString& rootSWdir, FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFileName)
{
	FVADescriptionFile fvaFileCsv;

	QStringList		titles;
	DESCRIPTIONS_MAP	decsItems;

	FVA_EXIT_CODE res = fvaFileCsv.load(rootSWdir + "#data#/" + fvaFileName, titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,EventId,ReasonPeople,reserved1
	int columnDevId = FVADescriptionFile::getColumnIdByName(titles, "DevId");
	if (-1 == columnDevId)
	{
		LOG_CRIT << "-1 == columnDevId";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnID = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnPlaceID = FVADescriptionFile::getColumnIdByName(titles, "PlaceId");
	if (-1 == columnPlaceID)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnEventID = FVADescriptionFile::getColumnIdByName(titles, "EventId");
	if (-1 == columnEventID)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnReasonPeopleID = FVADescriptionFile::getColumnIdByName(titles, "ReasonPeople");
	if (-1 == columnReasonPeopleID)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		QString fileName = list[columnName].toUpper();
		if (fvaFileInfo.find(fileName) != fvaFileInfo.end())
		{
			QFile file(rootSWdir + "#data#/fvaNotUniqueFileName.csv");
			file.open(QIODevice::WriteOnly | QIODevice::Append);
			QTextStream writeStream(&file);
			writeStream << list[columnID].toUpper() << "\n";
			file.close();
			return FVA_ERROR_NON_UNIQUE_FVA_INFO;
		}
		fvaFile newFile;
		newFile.deviceId	= list[columnDevId].remove("\t").toUInt();
		newFile.placeId		= list[columnPlaceID].remove("\t").toUInt();
		newFile.eventId		= list[columnEventID].remove("\t").toUInt();
		QString eventPeopleIds	= list[columnReasonPeopleID].remove("\t").trimmed(); 
		newFile.eventPeopleIds	= fvaStringToIds(eventPeopleIds); 

		// not loaded field yet
		newFile.scanerId = 0;
		newFile.peopleIds; 
		newFile.description;
		newFile.comment;

		fvaFileInfo[fileName.toUpper()] = newFile;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaGetDeviceIdFromCsv(const FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFile, int& deviceID)
{
	deviceID = FVA_UNDEFINED_ID;

	if (fvaFileInfo.find(fvaFile.toUpper()) != fvaFileInfo.end())
	{
		deviceID = fvaFileInfo[fvaFile.toUpper()].deviceId;
		return FVA_NO_ERROR;
	}

	return FVA_ERROR_NO_DEV_ID;
};
FVA_EXIT_CODE fvaLoadSimpleMapFromCsvByItemType(const QString& rootSWdir, FVA_SIMPLE_MAP& simpleMap, const QString& dictName, int typeToFilter)
{
	FVADescriptionFile	fvaCsv;
	QStringList		titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaCsv.load(rootSWdir + "#data#/" + dictName, titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
	// supress this error if we don't need to filter by type 
	if (FVA_UNDEFINED_ID != typeToFilter && -1 == columnType)
	{
		LOG_CRIT << "-1 == columnType";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		int ID = list[columnId].remove("\t").toUInt();
		QString Name = list[columnName].remove("\t").trimmed();
		if ( FVA_UNDEFINED_ID==typeToFilter )
			simpleMap[ID] = Name;
		else
		{
			int Type = list[columnType].remove("\t").toInt();
			if (Type==typeToFilter)
				simpleMap[ID] = Name;				 
		} 		
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadDeviceMapFromCsv(const QString& rootSWdir, DEVICE_MAP& deviceMap)
{
	FVADescriptionFile	fvaDeviceCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaDeviceCsv.load(rootSWdir + "#data#/fvaDevices.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR
	// ID,OwnerId,LinkedName,Name,Type
	int columnDevId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnDevId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnOwnerId = FVADescriptionFile::getColumnIdByName(titles, "OwnerId");
	if (-1 == columnOwnerId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnLinkedName = FVADescriptionFile::getColumnIdByName(titles, "LinkedName");
	if (-1 == columnLinkedName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
	if (-1 == columnType)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		fvaDevice device;
		device.linkedName	= list[columnLinkedName].remove("\t").trimmed();
		device.deviceId		= list[columnDevId].remove("\t").toUInt();
		device.guiName		= list[columnName].remove("\t").trimmed();
		device.ownerName	= "N/A";
		device.ownerId		= list[columnOwnerId].remove("\t").toUInt();
		device.type		= static_cast<FVA_DEVICE_TYPE> (list[columnType].remove("\t").toUInt());
		deviceMap[device.deviceId] = device;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadPeopleMapFromCsv(const QString& rootSWdir, PEOPLE_MAP& peopleMap)
{
	FVADescriptionFile	fvaPeopleCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaPeopleCsv.load(rootSWdir + "#data#/fvaPeople.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR
	// ID,Name,FullName,RelationId,RelPersonID
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
	{
		LOG_CRIT << "-1 == columnId";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
	{
		LOG_CRIT << "-1 == columnName";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	int columnFullName = FVADescriptionFile::getColumnIdByName(titles, "FullName");
	if (-1 == columnFullName)
	{
		LOG_CRIT << "-1 == columnFullName";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
	if (-1 == columnType)	
	{
		LOG_CRIT << "-1 == columnType";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	int columnInstitution = FVADescriptionFile::getColumnIdByName(titles, "Institution");
	if (-1 == columnInstitution)
	{
		LOG_CRIT << "-1 == columnInstitution";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		fvaPerson person;
		person.Id		= list[columnId].remove("\t").toUInt();
		person.name		= list[columnName].remove("\t").trimmed();		
		person.fullName		= list[columnFullName].remove("\t").trimmed();
		person.type		= list[columnType].remove("\t").toUInt();
		person.institution	= list[columnInstitution].remove("\t").toUInt();;

		peopleMap[person.Id]		= person;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadEventMapFromCsv(const QString& rootSWdir, FVA_EVENT_MAP& eventMap)
{
	FVADescriptionFile	fvaEventTypesCsv;
	QStringList		titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaEventTypesCsv.load(rootSWdir + "#data#/fvaEvents.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,Type,Institution
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
	{
		LOG_CRIT << "-1 == columnId";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
	{
		LOG_CRIT << "-1 == columnName";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
	if (-1 == columnType)
	{
		LOG_CRIT << "-1 == columnType";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}
	int columnIns = FVADescriptionFile::getColumnIdByName(titles, "Institution");
	if (-1 == columnIns)
	{
		LOG_CRIT << "-1 == columnIns";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();
		
		fvaEvent event;

		event.ID = list[columnId].remove("\t").toUInt();
		event.name = list[columnName].remove("\t").trimmed();
		event.type = list[columnType].remove("\t").toInt();
		event.institution = list[columnIns].remove("\t").toInt();

		eventMap[event.ID] = event;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadDictMapFromCsv(const QString& rootSWdir, BASE_DICT_ITEM_MAP& dictMap, const QString& dictName)
{
	FVADescriptionFile	fvaCsv;
	QStringList		titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaCsv.load(rootSWdir + "#data#/" + dictName, titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
	if (-1 == columnType)
	{
		LOG_CRIT << "-1 == columnType";
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
	}

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		fvaBaseDictionaryItem dictionaryItem;

		dictionaryItem.ID = list[columnId].remove("\t").toUInt();
		dictionaryItem.name = list[columnName].remove("\t").trimmed();
		dictionaryItem.type = list[columnType].remove("\t").toInt();

		dictMap[dictionaryItem.ID] = dictionaryItem;
	}
	return FVA_NO_ERROR;
}
