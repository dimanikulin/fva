/*!
 * \file fvacommoncsv.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "fvacommoncsv.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

#include "FVADescriptionFile.h"
#include "fvacommonlib.h"
#include "fvadevice.h"
#include "fvafile.h"
#include "fvalogger.inl"

// TODO to move these functions to some common place and use in other places as well
// TODO make UT for these functions
// Helper function to convert string to uppercase
std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}

// Helper function to trim whitespace from string
std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) ++start;
    auto end = str.end();
    do {
        --end;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

// Helper function to remove tabs from string
std::string removeTab(const std::string& str) {
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());
    return result;
}

FVA_EXIT_CODE fvaGetIDFromFile(const std::string& fileName, int& ID) {
    std::ifstream file(fileName);
    if (!file.is_open()) return FVA_ERROR_CANT_OPEN_ID_FILE;
    file >> ID;
    file.close();
    return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaSaveIDInFile(const std::string& fileName, int ID) {
    std::ofstream file(fileName, std::ios::out);
    if (!file.is_open()) return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
    file << ID;
    file.flush();
    file.close();
    return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadFvaFileInfoFromCsv(const std::string& rootSWdir, FVA_FILE_INFO_MAP& fvaFileInfo,
                                        const std::string& fvaFileName) {
    FVADescriptionFile fvaFileCsv;

    std::vector<std::string> titles;
    DESCRIPTIONS_MAP decsItems;

    FVA_EXIT_CODE res = fvaFileCsv.load(rootSWdir + "#data#/" + fvaFileName, titles, decsItems);
    RET_RES_IF_RES_IS_ERROR

    // ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,EventId,ReasonPeople,reserved1
    int columnDevId = FVADescriptionFile::getColumnIdByName(titles, "DevId");
    if (-1 == columnDevId) {
        LOG_CRIT << "-1 == columnDevId";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }

    int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnID = FVADescriptionFile::getColumnIdByName(titles, "ID");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnPlaceID = FVADescriptionFile::getColumnIdByName(titles, "PlaceId");
    if (-1 == columnPlaceID) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnEventID = FVADescriptionFile::getColumnIdByName(titles, "EventId");
    if (-1 == columnEventID) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnReasonPeopleID = FVADescriptionFile::getColumnIdByName(titles, "ReasonPeople");
    if (-1 == columnReasonPeopleID) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    for (DESCRIPTIONS_MAP::const_iterator it = decsItems.begin(); it != decsItems.end(); ++it) {
        const std::vector<std::string>& list = it->second;

        const std::string fileName = toUpper(list[columnName]);
        if (fvaFileInfo.find(fileName) != fvaFileInfo.end()) {
            std::ofstream file(rootSWdir + "#data#/fvaNotUniqueFileName.csv", std::ios::app);
            file << toUpper(list[columnID]) << "\n";
            file.close();
            return FVA_ERROR_NON_UNIQUE_FVA_INFO;
        }
        fvaFile newFile;
        newFile.deviceId = static_cast<int>(std::stoul(removeTab(list[columnDevId])));
        newFile.placeId = static_cast<int>(std::stoul(removeTab(list[columnPlaceID])));
        newFile.eventId = static_cast<int>(std::stoul(removeTab(list[columnEventID])));
        newFile.name = fileName;
        std::string eventPeopleIds = trim(removeTab(list[columnReasonPeopleID]));
        newFile.eventPeopleIds = fvaStringToIds(eventPeopleIds);

        // not loaded field yet
        newFile.scanerId = 0;
        newFile.peopleIds;
        newFile.description;
        newFile.comment;

        fvaFileInfo[fileName] = newFile;
    }
    return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaGetDeviceIdFromCsv(const FVA_FILE_INFO_MAP& fvaFileInfo, const std::string& fvaFile, int& deviceID) {
    deviceID = FVA_UNDEFINED_ID;

    const std::string upperFile = toUpper(fvaFile);

    auto it = fvaFileInfo.find(upperFile);
    if (it != fvaFileInfo.end()) {
        deviceID = it->second.deviceId;
        return FVA_NO_ERROR;
    }

    return FVA_ERROR_NO_DEV_ID;
};
FVA_EXIT_CODE fvaLoadSimpleMapFromCsvByItemType(const std::string& rootSWdir, FVA_SIMPLE_MAP& simpleMap,
                                                const std::string& dictName, int typeToFilter) {
    FVADescriptionFile fvaCsv;
    std::vector<std::string> titles;
    DESCRIPTIONS_MAP decsItems;
    FVA_EXIT_CODE res = fvaCsv.load(rootSWdir + "#data#/" + dictName, titles, decsItems);
    RET_RES_IF_RES_IS_ERROR

    // ID,Name
    int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
    if (-1 == columnId) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
    // supress this error if we don't need to filter by type
    if (FVA_UNDEFINED_ID != typeToFilter && -1 == columnType) {
        LOG_CRIT << "-1 == columnType";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    for (DESCRIPTIONS_MAP::const_iterator it = decsItems.begin(); it != decsItems.end(); ++it) {
        const std::vector<std::string>& list = it->second;

        int ID = static_cast<int>(std::stoul(removeTab(list[columnId])));
        const std::string Name = trim(removeTab(list[columnName]));
        if (FVA_UNDEFINED_ID == typeToFilter)
            simpleMap[ID] = Name;
        else {
            int Type = std::stoi(removeTab(list[columnType]));
            if (Type == typeToFilter) simpleMap[ID] = Name;
        }
    }
    return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadDeviceMapFromCsv(const std::string& rootSWdir, DEVICE_MAP& deviceMap) {
    FVADescriptionFile fvaDeviceCsv;
    std::vector<std::string> titles;
    DESCRIPTIONS_MAP decsItems;
    FVA_EXIT_CODE res = fvaDeviceCsv.load(rootSWdir + "#data#/fvaDevices.csv", titles, decsItems);
    RET_RES_IF_RES_IS_ERROR
    // ID,OwnerId,LinkedName,Name,Type
    int columnDevId = FVADescriptionFile::getColumnIdByName(titles, "ID");
    if (-1 == columnDevId) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnOwnerId = FVADescriptionFile::getColumnIdByName(titles, "OwnerId");
    if (-1 == columnOwnerId) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnLinkedName = FVADescriptionFile::getColumnIdByName(titles, "LinkedName");
    if (-1 == columnLinkedName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
    if (-1 == columnType) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    for (DESCRIPTIONS_MAP::const_iterator it = decsItems.begin(); it != decsItems.end(); ++it) {
        const std::vector<std::string>& list = it->second;

        fvaDevice device;
        device.linkedName = trim(removeTab(list[columnLinkedName]));
        device.deviceId = static_cast<int>(std::stoul(removeTab(list[columnDevId])));
        device.guiName = trim(removeTab(list[columnName]));
        device.ownerName = "N/A";
        device.ownerId = static_cast<int>(std::stoul(removeTab(list[columnOwnerId])));
        device.type = static_cast<FVA_DEVICE_TYPE>(std::stoul(removeTab(list[columnType])));
        deviceMap[device.deviceId] = device;
    }
    return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadPeopleMapFromCsv(const std::string& rootSWdir, PEOPLE_MAP& peopleMap) {
    FVADescriptionFile fvaPeopleCsv;
    std::vector<std::string> titles;
    DESCRIPTIONS_MAP decsItems;
    FVA_EXIT_CODE res = fvaPeopleCsv.load(rootSWdir + "#data#/fvaPeople.csv", titles, decsItems);
    RET_RES_IF_RES_IS_ERROR
    // ID,Name,FullName,RelationId,RelPersonID
    int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
    if (-1 == columnId) {
        LOG_CRIT << "-1 == columnId";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
    if (-1 == columnName) {
        LOG_CRIT << "-1 == columnName";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    int columnFullName = FVADescriptionFile::getColumnIdByName(titles, "FullName");
    if (-1 == columnFullName) {
        LOG_CRIT << "-1 == columnFullName";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
    if (-1 == columnType) {
        LOG_CRIT << "-1 == columnType";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    int columnInstitution = FVADescriptionFile::getColumnIdByName(titles, "Institution");
    if (-1 == columnInstitution) {
        LOG_CRIT << "-1 == columnInstitution";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    for (DESCRIPTIONS_MAP::const_iterator it = decsItems.begin(); it != decsItems.end(); ++it) {
        const std::vector<std::string>& list = it->second;

        fvaPerson person;
        person.Id = static_cast<int>(std::stoul(removeTab(list[columnId])));
        person.name = trim(removeTab(list[columnName]));
        person.fullName = trim(removeTab(list[columnFullName]));
        person.type = static_cast<int>(std::stoul(removeTab(list[columnType])));
        person.institution = static_cast<int>(std::stoul(removeTab(list[columnInstitution])));

        peopleMap[person.Id] = person;
    }
    return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadEventMapFromCsv(const std::string& rootSWdir, FVA_EVENT_MAP& eventMap) {
    FVADescriptionFile fvaEventTypesCsv;
    std::vector<std::string> titles;
    DESCRIPTIONS_MAP decsItems;
    FVA_EXIT_CODE res = fvaEventTypesCsv.load(rootSWdir + "#data#/fvaEvents.csv", titles, decsItems);
    RET_RES_IF_RES_IS_ERROR

    // ID,Name,Type,Institution
    int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
    if (-1 == columnId) {
        LOG_CRIT << "-1 == columnId";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
    if (-1 == columnName) {
        LOG_CRIT << "-1 == columnName";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
    if (-1 == columnType) {
        LOG_CRIT << "-1 == columnType";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }
    int columnIns = FVADescriptionFile::getColumnIdByName(titles, "Institution");
    if (-1 == columnIns) {
        LOG_CRIT << "-1 == columnIns";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }

    for (DESCRIPTIONS_MAP::const_iterator it = decsItems.begin(); it != decsItems.end(); ++it) {
        const std::vector<std::string>& list = it->second;

        fvaEvent event;

        event.ID = static_cast<int>(std::stoul(removeTab(list[columnId])));
        event.name = trim(removeTab(list[columnName]));
        event.type = std::stoi(removeTab(list[columnType]));
        event.institution = std::stoi(removeTab(list[columnIns]));

        eventMap[event.ID] = event;
    }
    return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadDictMapFromCsv(const std::string& rootSWdir, BASE_DICT_ITEM_MAP& dictMap,
                                    const std::string& dictName) {
    FVADescriptionFile fvaCsv;
    std::vector<std::string> titles;
    DESCRIPTIONS_MAP decsItems;
    FVA_EXIT_CODE res = fvaCsv.load(rootSWdir + "#data#/" + dictName, titles, decsItems);
    RET_RES_IF_RES_IS_ERROR

    // ID,Name
    int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
    if (-1 == columnId) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
    if (-1 == columnName) return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

    int columnType = FVADescriptionFile::getColumnIdByName(titles, "Type");
    if (-1 == columnType) {
        LOG_CRIT << "-1 == columnType";
        return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;
    }

    for (DESCRIPTIONS_MAP::const_iterator it = decsItems.begin(); it != decsItems.end(); ++it) {
        const std::vector<std::string>& list = it->second;

        fvaBaseDictionaryItem dictionaryItem;

        dictionaryItem.ID = static_cast<int>(std::stoul(removeTab(list[columnId])));
        dictionaryItem.name = trim(removeTab(list[columnName]));
        dictionaryItem.type = std::stoi(removeTab(list[columnType]));

        dictMap[dictionaryItem.ID] = dictionaryItem;
    }
    return FVA_NO_ERROR;
}
