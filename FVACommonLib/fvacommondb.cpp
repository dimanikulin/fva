#include "fvacommondb.h"
#include "fvacommondata.h"
#include "fvacommonlib.h"
#include "fvadefaultcfg.h"

#include "json.h"

#include <QtSql/QtSql>

FVA_ERROR_CODE fillOneDictFromDB(QVariantMap& outputData, const QString& dictName)
{
    QSqlQuery query;
    if (!query.exec("SELECT * FROM " + dictName)) 
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;

	QSqlRecord		rec = query.record();
	QVariantList	list;
	QVariantMap		map;
    while (query.next()) 
	{
		map["ID"]	= query.value(rec.indexOf("ID"));
		map["name"]	= query.value(rec.indexOf("Name"));
		list.append(map);
    }
    outputData[dictName] = list;
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaLoadDictionary( const QString& file, QVariantMap& outputData, QString& error )
{
	QDir dir ( file );
	if ( !dir.exists( file ) )
	{
		error = "dictionaries file does not exist" ;
		return FVA_ERROR_CANT_FIND_DICTIONARIES;
	}

	QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(file);
    if (!dbase.open()) 
	{
		QSqlError sqlerror = dbase.lastError();
		error =  "can not open dictionaries" + sqlerror.text();
		return FVA_ERROR_CANT_OPEN_DICTIONARIES;
	}
	fillOneDictFromDB(outputData,"fvaRelationTypes");
	fillOneDictFromDB(outputData,"fvaPlaceTypes");
	fillOneDictFromDB(outputData,"fvaEventTypes");

	////////// RELATIONS///////////////////
	QSqlQuery query;
    if (!query.exec("SELECT * FROM fvaPeopleRelations")) 
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;
	QSqlRecord		rec = query.record();
	QVariantList	list;
	QVariantMap		map;
    while (query.next()) 
	{
		map["ID"]			= query.value(rec.indexOf("ID"));
		map["name"]			= query.value(rec.indexOf("Name"));
		map["RelationType"]	= query.value(rec.indexOf("RelationType"));
		list.append(map);
    }
    outputData["fvaRelations"] = list;
	////////// DEVICES///////////////////
    if (!query.exec("SELECT * FROM fvaDevices ")) 
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;
	rec = query.record();
	list.clear();
    while (query.next()) 
	{
		map["ID"]			= query.value(rec.indexOf("ID"));
		map["name"]			= query.value(rec.indexOf("Name"));
		map["OwnerID"]		= query.value(rec.indexOf("OwnerId"));
		map["LinkedName"]	= query.value(rec.indexOf("LinkedName"));
		list.append(map);
    }
    outputData["fvaDevices"] = list;

	/////////////////////PEOPLE///////////////////
    if (!query.exec("select *, (select name from fvaPeople p1 where p.RelPersonID = p1.ID  ) relname from fvaPeople p ")) 
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;
	rec = query.record();
	list.clear();
    while (query.next()) 
	{
		map["ID"]			= query.value(rec.indexOf("ID"));
		map["name"]			= query.value(rec.indexOf("Name"));
		map["RelationId"]	= query.value(rec.indexOf("RelationId"));
		map["fullName"]		= query.value(rec.indexOf("FullName")).toString() 
							+ " [" + query.value(rec.indexOf("relname")).toString() + "]";
		map["RelPersonID"]	= query.value(rec.indexOf("RelPersonID"));
		list.append(map);
    }
    outputData["fvaPeople"] = list;

	/////////////////////PLACES///////////////////
    if (!query.exec("select * from fvaPlaces ")) 
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;
	rec = query.record();
	list.clear();
    while (query.next()) 
	{
		map["ID"]			= query.value(rec.indexOf("ID"));
		map["name"]			= query.value(rec.indexOf("Name"));
		map["type"]			= query.value(rec.indexOf("Type"));
		list.append(map);
	}
	outputData["fvaPlaces"] = list;

	/////////////////////EVENTS///////////////////
    if (!query.exec("select *, et.Name as etName from fvaEventRelations er, fvaEventTypes et where er.Type=et.ID")) 
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;
	rec = query.record();
	list.clear();
    while (query.next()) 
	{
		map["ID"]			= query.value(rec.indexOf("ID"));
		map["name"]			= query.value(rec.indexOf("Name"));
		map["type"]			= query.value(rec.indexOf("Type"));
		map["fullName"]		= query.value(rec.indexOf("Name")).toString()
							+ "(" +
							query.value(rec.indexOf("etName")).toString() 
							+ ")"; 
		list.append(map);
	}
	outputData["fvaEvents"] = list;
	dbase.close();
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE fvaSaveDictionary( const QString& file, QVariantMap& inputJson, QString& error )
{
	QDir dir ( file );
	if ( !dir.exists( file ) )
	{
		error = "dictionaries file does not exist" ;
		return FVA_ERROR_CANT_FIND_DICTIONARIES;
	}
	else
	{
		QString newName = file + "_" + QDateTime::currentDateTime().toString(FVA_FILE_NAME_FMT).toLatin1().data();
		if ( !dir.rename( file, newName ))
		{
			return FVA_ERROR_CANT_OPEN_DICTIONARIES;
		}
	}
	// open it
	QFile _file ( file );
	if ( !_file.open( QIODevice::WriteOnly ) )
	{
		error =  "can not open dictionaries";
		return FVA_ERROR_CANT_OPEN_DICTIONARIES;
	}

	bool res				= false;

	QByteArray data = QtJson::Json::serialize( inputJson, res );
	if ( !res )
	{
		error =  "can not save dictionaries";
		_file.close();
		return FVA_ERROR_CANT_SAVE_DICTIONARIES;
	}
	QString result = QString::fromLocal8Bit(data);

	QTextStream writeStream( &_file );
	writeStream << result;	
	writeStream.flush();
	_file.close();

	return FVA_NO_ERROR;
}

FVA_ERROR_CODE fvaLoadDeviceMapFromDictionary(DEVICE_MAP& deviceMap, const QString& dictPath)
{
	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	RET_RES_IF_RES_IS_ERROR

	QVariantList vlist = dictionaries["fvaDevices"].toList();
	QVariantList people = dictionaries["fvaPeople"].toList();

	for ( auto it = vlist.begin(); vlist.end() != it; ++it)
	{
		fvaDevice device;
		device.linkedName			= it->toMap()["LinkedName"].toString().toUpper().trimmed();
		device.deviceId				= it->toMap()["ID"].toInt();
		device.guiName				= it->toMap()["name"].toString().toUpper().trimmed();
		for (auto itP = people.begin(); people.end() != itP; ++itP )
		{
			if ( it->toMap()["OwnerID"].toInt() == itP->toMap()["ID"].toInt())
			{
				device.ownerName	= itP->toMap()["name"].toString();
				break;
			}
		}
		deviceMap[device.deviceId]	= device;
	}
	
	return FVA_NO_ERROR;
}
#define OPEN_DB \
	QDir dir ( DBPath ); \
	if ( !dir.exists( DBPath ) ) \
	{ \
		error = "DB file does not exist"; \
		return FVA_ERROR_CANT_FIND_FVA_DB; \
	}\
	QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE"); \
    dbase.setDatabaseName( DBPath ); \
    if ( !dbase.open() ) \
	{ \
		error =  "can not open fva DB"; \
		return FVA_ERROR_CANT_OPEN_FVA_DB; \
	}
FVA_ERROR_CODE fvaLoadFiles( fvaItem* rootItem, const QString& DBPath, QString& error )
{
	OPEN_DB
	QSqlQuery query;
    if (!query.exec("SELECT * FROM fvaFile")) 
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	QSqlRecord		rec = query.record();
    while (query.next()) 
	{

	}
	return FVA_NO_ERROR;
}
typedef QMap<int, fvaFolder*> FVA_FOLDER_MAP; 
FVA_ERROR_CODE fvaLoadFolders( FVA_FOLDER_MAP& folderMap, const QString& DBPath, QString& error )
{
	OPEN_DB
	QSqlQuery query;
    if (!query.exec("SELECT * FROM fvaFolder")) 
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	QSqlRecord		rec = query.record();
    while ( query.next() ) 
	{
		fvaFolder * pFolder				= new fvaFolder;
		
		pFolder->eventId				= query.value(rec.indexOf("EventId")).toInt();
		pFolder->linkedFolder			= query.value(rec.indexOf("LinkedFolder")).toString();
		pFolder->tags					= query.value(rec.indexOf("Tags")).toString();
		pFolder->eventReasonPeopleIds	= fvaStringToIds(query.value(rec.indexOf("ReasonPeople")).toString());
		
		folderMap[ query.value(rec.indexOf("ID")).toInt() ] = pFolder; 
	}
	return FVA_NO_ERROR;
}

typedef QMap<int, fvaFile*> FVA_FILE_MAP; 
FVA_ERROR_CODE fvaLoadFiles( FVA_FILE_MAP& fileMap, const QString& DBPath, QString& error )
{
	OPEN_DB
	QSqlQuery query;
    if (!query.exec("SELECT * FROM fvaFile")) 
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	QSqlRecord		rec = query.record();
    while ( query.next() ) 
	{
		fvaFile * pFile		= new fvaFile;
		
		pFile->deviceId		=	query.value(rec.indexOf("DevId")).toInt();
		pFile->placeId		=	query.value(rec.indexOf("PlaceId")).toInt();
		pFile->scanerId		=	 query.value(rec.indexOf("ScanerId")).toInt();
		pFile->peopleIds	=	fvaStringToIds(query.value(rec.indexOf("People")).toString());
		pFile->comment		=	query.value(rec.indexOf("Comment")).toString();
		pFile->description	=	query.value(rec.indexOf("Description")).toString();
		pFile->name			=	query.value(rec.indexOf("Name")).toString();

		fileMap[ query.value(rec.indexOf("ID")).toInt() ] = pFile; 
	}
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE fvaLoadItems( const QString& rootPath, fvaItem* rootItem, const QString& DBPath, QString& error, int& number )
{
	OPEN_DB	
	FVA_FOLDER_MAP fvaFolderMap;
	fvaLoadFolders( fvaFolderMap, DBPath, error );
	FVA_FILE_MAP fvaFileMap;
	fvaLoadFiles( fvaFileMap, DBPath, error );
	QSqlQuery query;
    if (!query.exec("SELECT * FROM fva")) 
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	QSqlRecord		rec = query.record();
	std::map <QString, fvaItem*> fvaFolderItems;
	std::map <QString, fvaItem*> fvaFileItems;
	fvaFolderItems.insert(std::pair<QString, fvaItem*>( rootPath, rootItem ) );
	QString Name, Path;
    while ( query.next() ) 
	{
		fvaItem* pItem = new fvaItem;
		++number;
		// fill up the item
		pItem->type		= static_cast<FVA_FS_TYPE> (query.value(rec.indexOf("Type")).toInt());
				Name	= query.value(rec.indexOf("Name")).toString();
				Path	= query.value(rec.indexOf("Path")).toString();
		pItem->fsFullPath = Path + "/" + Name;
		
		if ( FVA_FS_TYPE_DIR == pItem->type )
		{
			if ( FVA_NO_ERROR != fvaParseDirName( Name, pItem->dateFrom, pItem->dateTo ) )
			{
				qCritical() << "incorrect folder name " << Name;
				delete pItem;
				continue;
			}
			fvaFolderItems.insert( std::pair<QString, fvaItem*>(pItem->fsFullPath, pItem ) );
			int FvaFolderID = query.value(rec.indexOf("FvaFolderId")).toInt();
			if ( FVA_UNDEFINED_ID != FvaFolderID )
			{
				auto it = fvaFolderMap.find( FvaFolderID );
				if ( it == fvaFolderMap.end() )
					qCritical() << "not found fva folder for ID " << FvaFolderID;
				else
				{
					pItem->fvaFolder = it.value();
					fvaFolderMap.erase( it );
				}
			}
		}
		else
		{
			if ( FVA_NO_ERROR != fvaParseFileName( Name.split(".",QString::SkipEmptyParts).at(0), pItem->dateFrom ) )
			{
				qDebug() << "ERROR!, incorrect file name " << Name;
				delete pItem;
				continue;
			}
			auto it = fvaFolderItems.find( Path );
			if (it != fvaFolderItems.end())
				it->second->children.push_front(pItem);
			else
				fvaFileItems.insert(std::pair<QString, fvaItem*>(pItem->fsFullPath, pItem));
			
			int FvaFileID = query.value(rec.indexOf("FvaFileId")).toInt();
			if ( FVA_UNDEFINED_ID != FvaFileID )
			{
				auto it = fvaFileMap.find( FvaFileID );
				if ( it == fvaFileMap.end() )
					qCritical() << "not found fva file for ID " << FvaFileID;
				else
				{
					pItem->fvaFile = it.value();
					fvaFileMap.erase( it );
				}
			}
		}			
		pItem->isFiltered = true;
    }

	QFileInfo fi;
	for ( auto it = fvaFolderItems.begin(); it != fvaFolderItems.end(); ++it )
	{
		fi = it->first;
		auto itF = fvaFolderItems.find( fi.absolutePath() );
		if ( itF != fvaFolderItems.end() )
			itF->second->children.push_front(it->second);
	}
	// checks
	if ( fvaFileItems.size() )
	{
		error = "ERROR!, not empty fvaFiles";
		qDebug() << "ERROR!, not empty fvaFiles";
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	}

	if ( fvaFolderMap.size() )
	{
		error = "ERROR!, not empty fvaFolders";
		qDebug() << "ERROR!, not empty fvaFolders";
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	}

	if ( fvaFileMap.size() )
	{
		for ( auto id = fvaFileMap.begin(); id != fvaFileMap.end(); ++id )
		{
			qDebug() << "name=" << id.value()->name;
		}

		error = " ERROR!, not empty fvaFiles";
		qDebug() << "ERROR!, not empty fvaFiles";
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	}
	return FVA_NO_ERROR;
}
