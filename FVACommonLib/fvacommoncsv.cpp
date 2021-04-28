#include "fvacommoncsv.h"
#include "fvadescriptionfile.h"
#include "fvadefaultcfg.h"
#include "data/fvafile.h"
#include "data/fvadevice.h"

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
FVA_EXIT_CODE fvaLoadFvaFileInfoFromCsv(FVA_FILE_INFO_MAP& fvaFileInfo)
{
	FVADescriptionFile fvaFileCsv;

	// firstly - try to get device if from fvaFile.csv as it has high priority 
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;

	FVA_EXIT_CODE res = fvaFileCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,EventId,ReasonPeople,reserved1
	int columnDevId = FVADescriptionFile::getColumnIdByName(titles, "DevId");
	if (-1 == columnDevId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnID = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnPlaceID = FVADescriptionFile::getColumnIdByName(titles, "PlaceId");
	if (-1 == columnPlaceID)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		QString fileName = list[columnName].toUpper();
		if (fvaFileInfo.find(fileName) != fvaFileInfo.end())
		{
			QFile file(FVA_DEFAULT_ROOT_DIR + "#data#/fvaNotUniqueFileName.csv");
			file.open(QIODevice::WriteOnly | QIODevice::Append);
			QTextStream writeStream(&file);
			writeStream << list[columnID].toUpper() << "\n";
			file.close();
			return FVA_ERROR_NON_UNIQUE_FVA_INFO;
		}
		fvaFile newFile;
		newFile.deviceId = list[columnDevId].remove("\t").toUInt();
		newFile.placeId = list[columnPlaceID].remove("\t").toUInt();
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

	// TODO to delete?
	/*QFile file(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFileNoDevId.csv");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	int ID = FVA_UNDEFINED_ID;
	fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.id", ID);
	QTextStream writeStream(&file);
	writeStream << QString::number(++ID) << "," << fvaFile << ",,," << QString::number(deviceID) << ",,,,,,\n";
	fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.id", ID);
	file.close();
	return FVA_NO_ERROR;*/
	return FVA_ERROR_NO_DEV_ID;
};
FVA_EXIT_CODE fvaLoadDeviceMapFromCsv(DEVICE_MAP& deviceMap)
{
	FVADescriptionFile	fvaDeviceCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaDeviceCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaDevices.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR
	// ID,OwnerId,LinkedName,Name,fvaDeviceType
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

	int columnfvaDeviceType = FVADescriptionFile::getColumnIdByName(titles, "fvaDeviceType");
	if (-1 == columnfvaDeviceType)
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
		device.type			= static_cast<FVA_DEVICE_TYPE> (list[columnfvaDeviceType].remove("\t").toUInt());
		deviceMap[device.deviceId] = device;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadPeopleMapFromCsv(PEOPLE_MAP& peopleMap)
{
	FVADescriptionFile	fvaPeopleCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaPeopleCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaPeople.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR
	// ID,Name,FullName,RelationId,RelPersonID
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnFullName = FVADescriptionFile::getColumnIdByName(titles, "FullName");
	if (-1 == columnFullName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnRelationId = FVADescriptionFile::getColumnIdByName(titles, "RelationId");
	if (-1 == columnRelationId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnRelPersonID = FVADescriptionFile::getColumnIdByName(titles, "RelPersonID");
	if (-1 == columnRelPersonID)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		fvaPerson person;
		person.Id				= list[columnId].remove("\t").toUInt();
		person.name				= list[columnName].remove("\t").trimmed();		
		person.fullName			= list[columnFullName].remove("\t").trimmed();
		person.relatedPersionId	= list[columnRelPersonID].remove("\t").toUInt();
		person.relationId		= list[columnRelationId].remove("\t").toUInt();
		peopleMap[person.Id]	= person;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadSimpleMapFromCsv(FVA_SIMPLE_MAP& simpleMap, const QString& dictName)
{
	FVADescriptionFile	fvaCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/" + dictName, titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		int ID = list[columnId].remove("\t").toUInt();
		QString Name = list[columnName].remove("\t").trimmed();
		simpleMap[ID] = Name;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadPeopleRelationMapFromCsv(FVA_PEOPLE_RELATION_MAP& peopleRelationsMap)
{
	FVADescriptionFile	fvaRelationTypesCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaRelationTypesCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaPeopleRelations.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,RelationType
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnRelationType = FVADescriptionFile::getColumnIdByName(titles, "RelationType");
	if (-1 == columnRelationType)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();
		
		fvaPeopleRelation relation;

		relation.Id = list[columnId].remove("\t").toUInt();
		relation.name = list[columnName].remove("\t").trimmed();
		relation.relationType = list[columnRelationType].remove("\t").toInt();

		peopleRelationsMap[relation.Id] = relation;
	}
	return FVA_NO_ERROR;
}