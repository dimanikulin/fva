#include "fvacommonlib.h"

#include <QDir>
#include <QTextStream>
#include <QProcess>
#include <QCoreApplication>
#include <QTGui/QMessageBox>
#include <QTGui/QPainter>
#include <QTGui/QIcon>
#include <QtSql/QtSql>

#include "../lib/json/json.h"
#include "../lib/qexifimageheader.h"
#include "RiffParser.h"

#include <windows.h>
#include <winbase.h>

FVA_ERROR_CODE fvaGetFolderDescription( const QString& folder, QVariantMap& outputJson, QString& error )
{
	QDir dir ( folder );
	QString descFolderPath = folder + "/" + FVA_DIR_DESCRIPTION_FILE_NAME;
	if ( !dir.exists( descFolderPath ) )
	{
		error = "description for folder does not exist in folder:" + folder;
		return FVA_ERROR_CANT_FIND_DIR_DESC;
	}

	// load it
	QFile file ( descFolderPath );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		error =  "can not open description for folder in folder:" + folder;
		return FVA_ERROR_CANT_OPEN_DIR_DESC;
	}

	bool res				= false;
	QString		jsonData	= QString::fromLocal8Bit ( file.readAll() );
	outputJson				= QtJson::Json::parse ( jsonData, res ).toMap();
	if ( !res )
	{
		error =  "incorrect format of folder description";
		file.close();
		return FVA_ERROR_INCORRECT_FORMAT;
	}
	file.close();
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaCreateFolderDescription (const QString& path, const QString& content, QString& error)
{
	QFile fileNew ( path );	
	if ( !fileNew.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		error = "can not open new description, in " + path;
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
	}
	QTextStream writeStream( &fileNew );
	writeStream << content;	
	writeStream.flush();
	fileNew.close();

	SetFileAttributes(path.toStdWString().c_str(), /*FILE_ATTRIBUTE_HIDDEN |*/ FILE_ATTRIBUTE_READONLY );

	return FVA_NO_ERROR;
}
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
#ifdef _NEW_DICTIONARY_
	QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(file);
    if (!dbase.open()) 
	{
		error =  "can not open dictionaries";
		return FVA_ERROR_CANT_OPEN_DICTIONARIES;
	}
	fillOneDictFromDB(outputData,"relationTypes");
	fillOneDictFromDB(outputData,"placeTypes");
	fillOneDictFromDB(outputData,"eventTypes");

	////////// RELATIONS///////////////////
	QSqlQuery query;
    if (!query.exec("SELECT * FROM peoplerelations")) 
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
    outputData["relations"] = list;
	////////// DEVICES///////////////////
    if (!query.exec("SELECT * FROM devices ")) 
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
    outputData["devices"] = list;

	/////////////////////PEOPLE///////////////////
    if (!query.exec("select *, (select name from people p1 where p.RelPersonID = p1.ID  ) relname from people p ")) 
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
    outputData["people"] = list;

	/////////////////////PLACES///////////////////
    if (!query.exec("select * from places ")) 
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
	outputData["places"] = list;

	/////////////////////EVENTS///////////////////
    if (!query.exec("select *, et.Name as etName from eventrelations er, eventTypes et where er.Type=et.ID")) 
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
	outputData["events"] = list;
	dbase.close();
#else
	// load it
	QFile _file ( file );
	if ( !_file.open( QIODevice::ReadOnly ) )
	{
		error =  "can not open dictionaries";
		return FVA_ERROR_CANT_OPEN_DICTIONARIES;
	}

	bool res				= false;
	QString		jsonData	= QString::fromLocal8Bit ( _file.readAll() );
	outputJson				= QtJson::Json::parse ( jsonData, res ).toMap();
	if ( !res )
	{
		error =  "can not load dictionaries";
		_file.close();
		return FVA_ERROR_CANT_LOAD_DICTIONARIES;
	}
	_file.close();
#endif
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
		QString newName = file + "_" + QDateTime::currentDateTime().toString( "yyyy-MM-dd-hh-mm-ss").toAscii().data();
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

bool fvaIsInternalFile( const QString& fileName )
{
	return (	fileName.toUpper() == FVA_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DIR_DESCRIPTION_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_BACKGROUND_MUSIC_FILE_NAME.toUpper() 
			||	fileName.toUpper() == FVA_DICTIONARY_NAME  );
}
bool fvaIsFVAFile( const QString& extention )
{
	return FVA_FS_TYPE_UNKNOWN != fvaConvertFileExt2FileType ( extention );
}

FVA_FS_TYPE fvaConvertFileExt2FileType ( const QString& extention )
{	
	if (	extention.toUpper()	== "JPG" 
		|| extention.toUpper()	== "JPEG" 
		|| extention.toUpper()	== "PNG" 
		|| extention.toUpper()	== "BMP" 
		|| extention.toUpper()	== "GIF" )
		return FVA_FS_TYPE_IMG;
	
	if ( extention.toUpper()	== "AVI" 
		|| extention.toUpper()	== "MOV" 
		|| extention.toUpper()	== "MPG" 
		|| extention.toUpper()	== "MP4" 
		|| extention.toUpper()	== "3GP" )
		return FVA_FS_TYPE_VIDEO;

	if ( extention.toUpper()	== "WAV" )
		return FVA_FS_TYPE_AUDIO;

	return FVA_FS_TYPE_UNKNOWN;
}
FVA_ERROR_CODE fvaShowImage( const QString& fileName, QLabel* imgLabel, const QString& text )
{
	if ( fileName.isEmpty() || !imgLabel )
	{
		return FVA_ERROR_WRONG_PARAMETERS;
	}

	/*QExifImageHeader img( fileName );
	QImage image_thumb = img.thumbnail();
	if ( !image_thumb.isNull() )
	{
		imgLabel->setPixmap(QPixmap::fromImage( image_thumb ));
		return FVA_NO_ERROR;
	}*/

	QImage image( fileName );
	if ( image.isNull() )
	{
		return FVA_ERROR_CANT_OPEN_INPUT_FILE;
	}

	if (!text.isEmpty())
	{
		// tell the painter to draw on the QImage
		QPainter* painter = new QPainter(&image); 
		painter->setPen(Qt::white);
		painter->setFont(QFont("Arial", 30));

		//you probably want the to draw the text to the rect of the image
		painter->drawText(image.rect(), Qt::AlignTop, text);
	}

	QPixmap _qpSource = QPixmap::fromImage(image); 
	QPixmap _qpCurrent = QPixmap::fromImage(image);

    float cw = imgLabel->width(), ch = imgLabel->height();
    float pw = _qpCurrent.width(), ph = _qpCurrent.height();

    if (pw > cw && ph > ch && pw/cw > ph/ch || //both width and high are bigger, ratio at high is bigger or
        pw > cw && ph <= ch || //only the width is bigger or
        pw < cw && ph < ch && cw/pw < ch/ph //both width and height is smaller, ratio at width is smaller
        )
        _qpCurrent = _qpSource.scaledToWidth(cw, Qt::TransformationMode::FastTransformation);
    else if (pw > cw && ph > ch && pw/cw <= ph/ch || //both width and high are bigger, ratio at width is bigger or
        ph > ch && pw <= cw || //only the height is bigger or
        pw < cw && ph < ch && cw/pw > ch/ph //both width and height is smaller, ratio at height is smaller
        )
        _qpCurrent = _qpSource.scaledToHeight(ch, Qt::TransformationMode::FastTransformation);

	imgLabel->setPixmap(_qpCurrent);

	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaParseDirName( const QString& dirName, QDateTime& from, QDateTime& to )
{	
	switch( dirName.length() )
	{
		case 4: // one year folder
		{
			from = QDateTime::fromString( dirName, "yyyy");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from/*.addYears(1)*/;
		}
		break;
		case 9: // year period
		{
			if ( dirName[ 4 ] != '-' )
				return FVA_ERROR_WRONG_FOLDER_NAME;

			from	= QDateTime::fromString( dirName.mid( 0, 4 ), "yyyy");
			to	= QDateTime::fromString( dirName.mid( 5, 4 ), "yyyy");

			if ( !from.isValid() || !to.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
		}
		break;
		case 10 : // one-day event
		{
			from = QDateTime::fromString( dirName, "yyyy.MM.dd");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = from.addDays(1); 
		}
		break;
		case 13 :
		{
			from = QDateTime::fromString( dirName.mid( 0,10 ), "yyyy.MM.dd");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			if ( dirName [ 10 ]  == ' ' ) // one day and several events
			{
				if ( dirName [ 11 ]  != '#' )
					return FVA_ERROR_WRONG_FOLDER_NAME;
				else
				{
					bool result = false;
					int dEventNumber = dirName.mid( 12, 1 ).toInt( &result );
					if ( !result || !dEventNumber )
						return FVA_ERROR_WRONG_FOLDER_NAME;
				}
				to = from.addDays(1);
			}
			else if ( dirName [ 10 ] == '-' ) // period
			{
				QString sEndDate = dirName.mid( 11,2 );
				QString sStartDate = dirName.mid( 8,2 );
				bool res, res1 = false; 
				int dEndDate = sEndDate.toInt( &res );
				int dStartDate = sStartDate.toInt( &res1 );
				if ( !res || !res1 || !dEndDate || !dStartDate)
					return FVA_ERROR_WRONG_FOLDER_NAME;
				to = from.addDays(dEndDate-dStartDate);
				to = to.addDays(1);
			}
			else
				return FVA_ERROR_WRONG_FOLDER_NAME;
		}
		break;
		case 16: //months-day period
		{
			from = QDateTime::fromString( dirName.mid( 0,10 ), "yyyy.MM.dd");
			if ( !from.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			if ( dirName [ 10 ] != '-' ) // not a period
				return FVA_ERROR_WRONG_FOLDER_NAME;

			QString sTo = dirName.mid( 0, 4 ) + "." + dirName.mid( 11,5 );
			to = QDateTime::fromString( sTo, "yyyy.MM.dd");
			if ( !to.isValid() )
				return FVA_ERROR_WRONG_FOLDER_NAME;
			to = to.addDays(1);
		}
		break;
		default:
			return FVA_ERROR_WRONG_FOLDER_NAME;
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE fvaParseFileName( const QString& fileName, QDateTime& date )
{
	if ( fileName.length() != 19 )
	{
		return FVA_ERROR_WRONG_FILE_NAME;
	}
	if (fileName.contains("IMG_"))
	{
		// it is also file name to extract name from "IMG_20150504_142546"
		QString newFileName = fileName;
		newFileName.remove("IMG_");
		date = QDateTime::fromString( newFileName, "yyyyMMdd_hhmmss" );
		if (!date.isValid())
			return FVA_ERROR_WRONG_FILE_NAME;
		else
			return FVA_NO_ERROR;
	}

	date = QDateTime::fromString( fileName, "yyyy-MM-dd-hh-mm-ss" );
	if ( !date.isValid() )
	{
		QString newFileName = QString(fileName).replace( "##","01" );
		date = QDateTime::fromString( newFileName, "yyyy-MM-dd-hh-mm-ss" );
		if ( !date.isValid() )
		{
			return FVA_ERROR_WRONG_FILE_NAME;
		}
	}
	return FVA_NO_ERROR;
}

fvaItem::fvaItem ()
{
	isFiltered			= true;
	hasDescriptionData	= false;
	deviceId			= 0;
	scanerId			= 0;
	placeId				= 0;
	eventId				= 0;
	type				= FVA_FS_TYPE_UNKNOWN;
}

fvaItem::~fvaItem ()
{
	for (auto idChild = children.begin(); idChild != children.end(); ++idChild)
	{
		if (idChild == nullptr)
			continue;
		delete *idChild;
		*idChild = nullptr;
	}
}
QString fvaItem::getGuiName(const QVariantMap&	dictionaries)
{
	if (type == FVA_FS_TYPE_DIR)
	{
		QString desc;
		if (eventId != 0)
		{
			QVariantList vlist = dictionaries["events"].toList();
			for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
			{
				if ( i->toMap()["ID"].toInt() == eventId )
				{
					desc =	 " - " + i->toMap()["fullName"].toString();
					break;
				}
			}

			if (!eventReasonPeopleIds.isEmpty())
			{
				QVariantList vlist = dictionaries["people"].toList();
				for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
				{
					if ( ( i->toMap()["ID"].toInt() == eventReasonPeopleIds[0])
					&& eventReasonPeopleIds[0] )
					{
						desc +=	 "," + i->toMap()["name"].toString();
						break;
					}
				}
			}
		}
		else if (hasDescriptionData && !description.isEmpty())
			desc =  " - " + description.trimmed();

		if (dateTo.isValid())
		{
			if (dateFrom == dateTo) // one year
				return dateFrom.toString( "yyyy") + desc;
			else if (dateFrom.addDays(1) == dateTo)
				return dateFrom.toString( "yyyy/MM/dd") + desc;
			else
				return dateFrom.toString( "yyyy/MM/dd") + dateTo.toString( "-yyyy/MM/dd") + desc;
		}
		else 
			return dateFrom.toString( "yyyy/MM/dd") + desc;
	}
	else
	{
		return dateFrom.toString( "yyyy-MM-dd (hh:mm:ss)");
	}
	return "";
}
void fillNameByOneId(int ident, const QString& dict, const QVariantMap&	dictionaries, QString& fullName)
{
	QVariantList vlist; 

	if (ident)
	{
		vlist = dictionaries[dict].toList();
		for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
		{
			if ( i->toMap()["ID"].toInt() == ident )
			{
				if (fullName.isEmpty())
					fullName = i->toMap()["name"].toString();
				else 
					fullName += "\n[" + i->toMap()["name"].toString() + "]"; 
				break;
			}
		}
	}
}
QString fvaItem::getGuiFullName(const QVariantMap&	dictionaries)
{
	QString fullName;
	if (!hasDescriptionData)
		return "";
	if (type != FVA_FS_TYPE_DIR)
	{
		if (!description.isEmpty())
			fullName = description;
	}

	if (!tagsOrComment.isEmpty())
	{
		if (fullName.isEmpty())
			fullName = tagsOrComment;
		else
			fullName += ", " + tagsOrComment;
	}

	fillNameByOneId(deviceId,"devices",dictionaries,fullName);
	return fullName;
}
bool fvaFilter::doesIDMatchToFilter(unsigned int ID, const QVector<unsigned int>& Ids) const
{
	for (auto it = Ids.begin(); it != Ids.end();++it)
	{
		if (ID == *it)
			return  true;
	}
	return false;
}

bool fvaFilter::doIDsMatchToFilter(const QVector<unsigned int>& IDs, const QVector<unsigned int>& filterIds) const
{
	for (auto it = IDs.begin(); it != IDs.end();++it)
	{
		if (doesIDMatchToFilter(*it,filterIds))
			return  true;		
	}
	return false;
}
FVA_ERROR_CODE fvaLoadDeviceMapFromDictionary(DEVICE_MAP& deviceMap, const QString& dictPath)
{
	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return res;

	QVariantList vlist = dictionaries["devices"].toList();
	QVariantList people = dictionaries["people"].toList();

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
	// TODO add scaners here
	
	return FVA_NO_ERROR;
}
DEVICE_MAP fvaGetDeviceMapForImg(const DEVICE_MAP& deviceMap, const QString& pathToFile, QString& deviceName)
{
	deviceName = QExifImageHeader(pathToFile).value(QExifImageHeader::Make).toString()
						+ QExifImageHeader(pathToFile).value(QExifImageHeader::Model).toString();

	DEVICE_MAP result;
	if (deviceName.isEmpty())
		return DEVICE_MAP();
	QString fixedDevName = deviceName.toUpper().trimmed();
	for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it)
	{
		QString name = it.value().linkedName; 
		if (name == fixedDevName)
			result[it.key()] = it.value(); 
	}

	deviceName = deviceName.remove("  ");
	deviceName = deviceName.remove(QChar('\0'));					
	if (!deviceName.isEmpty())
	{
		for (auto it = deviceMap.begin(); it != deviceMap.end(); ++it)
		{
			if (it.value().linkedName == deviceName.toUpper().trimmed())
				result[it.key()] = it.value(); 
		}
	}
	return result;
}

QDateTime fvaGetVideoTakenTime(const QString& pathToFile, QString& error)
{
	QExifImageHeader header(pathToFile);
	QDateTime renameDateTime = header.value(QExifImageHeader::DateTimeOriginal).toDateTime();
	QString _newName = renameDateTime.toString( "yyyy-MM-dd-hh-mm-ss" );
	if (_newName.isEmpty())
	{
		RiffParser riffInfo;
		QString createdDate;
		if ( !riffInfo.open( pathToFile, error ) || !riffInfo.findTag( "IDIT", createdDate ) || !riffInfo.convertToDate( createdDate, renameDateTime ) )
		{
			
			QProcess myProcess;    
			myProcess.setProcessChannelMode(QProcess::MergedChannels);
			QStringList params;
			params.append(pathToFile);
			myProcess.start(QCoreApplication::applicationDirPath() + "/#BIN#/exiftool(-k).exe", params);
			QString output;
			while(myProcess.waitForReadyRead())
			{
				output = myProcess.readAll();
				myProcess.putChar('\n');
			}
			myProcess.waitForFinished( -1 );
			int index = output.indexOf("Date/Time Original");
			if (index != -1)
			{
				index = output.indexOf(":", index);
				if (index != 1)
				{
					QString time = output.mid(index+1,20 );
					renameDateTime = QDateTime::fromString( time, " yyyy:MM:dd hh:mm:ss" );
				}
			}
			
		}
	}
	return renameDateTime;
}
void fvaBuildFilterTree(QWidget* pMainWnd, 
					QTreeWidget* pTreeWidget, 
					const QVariantList& rootLevel, 
					const QVariantList& level,
					QIcon* rootIcon,
					QIcon* icon)
{
	pMainWnd->connect(pTreeWidget, 
			SIGNAL(itemChanged(QTreeWidgetItem*, int)), 
			pMainWnd,
			SLOT(updateChecks(QTreeWidgetItem*, int)));
	for ( auto i = rootLevel.begin(); i != rootLevel.end() ; ++i )
	{
		int ID = i->toMap()["ID"].toInt();
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText		( 0, i->toMap()["name"].toString() );
		if (rootIcon)
			treeWidgetItem->setIcon		( 0, *rootIcon);
		treeWidgetItem->setFlags		( treeWidgetItem->flags() | Qt::ItemIsUserCheckable);
		treeWidgetItem->setCheckState(0,Qt::Unchecked);
		for (auto index = level.begin(); index != level.end(); ++index)
		{
			int IDc = index->toMap()["ID"].toInt();
			int type = index->toMap()["type"].toInt();

			if (type != ID)
				continue;

			QTreeWidgetItem* childWidgetItem = new QTreeWidgetItem;
			childWidgetItem->setText		( 0, index->toMap()["name"].toString() );
			childWidgetItem->setFlags	( childWidgetItem->flags() | Qt::ItemIsUserCheckable);
			childWidgetItem->setCheckState(0,Qt::Unchecked);
			if (icon)
				childWidgetItem->setIcon		( 0, *icon);
			childWidgetItem->setData( 1, 1, IDc );
			treeWidgetItem->addChild ( childWidgetItem );
		}
		if (treeWidgetItem->childCount())
			pTreeWidget->addTopLevelItem ( treeWidgetItem );
		else
			delete treeWidgetItem;
	}
}
void fvaBuildPeopleFilterTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, bool devicesNeed, const QVariantMap& dict )
{
	pMainWnd->connect(pTreeWidget, 
			SIGNAL(itemChanged(QTreeWidgetItem*, int)), 
			pMainWnd,
			SLOT(updateChecks(QTreeWidgetItem*, int)));

	QIcon	personIcon	= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/person.png");
	QIcon	peopleIcon	= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/people.png");
	QIcon	photoIcon	= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/photo.png");

	QVariantList	vlist	= dict["relationTypes"].toList();
	QVariantList	vlist1	= dict["relations"].toList();
	QVariantList	people	= dict["people"].toList();
	QVariantList	devices	= dict["devices"].toList();
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
	{
		int ID = i->toMap()["ID"].toInt();
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText		( 0, i->toMap()["name"].toString() );
		treeWidgetItem->setIcon		(0, peopleIcon);
		treeWidgetItem->setFlags		(treeWidgetItem->flags() | Qt::ItemIsUserCheckable);
		treeWidgetItem->setCheckState(0,Qt::Unchecked);
		for (auto index = vlist1.begin(); index != vlist1.end(); ++index)
		{
			int IDc = index->toMap()["ID"].toInt();
			int IDrel = index->toMap()["RelationType"].toInt();
			if (IDrel != ID)
				continue;

			QTreeWidgetItem* childWidgetItem = new QTreeWidgetItem;
			childWidgetItem->setText		( 0, index->toMap()["name"].toString() );
			childWidgetItem->setIcon		(0, peopleIcon);
			childWidgetItem->setFlags	(childWidgetItem->flags() | Qt::ItemIsUserCheckable);
			childWidgetItem->setCheckState(0,Qt::Unchecked);
		
			for (auto indexp = people.begin(); indexp != people.end(); ++indexp)
			{
				int IDp = indexp->toMap()["ID"].toInt();
				if (IDp == 0)
					continue;

				int IDrelp = indexp->toMap()["RelationId"].toInt();
				if (IDrelp != IDc)
					continue;

				QTreeWidgetItem* personWidgetItem = new QTreeWidgetItem;
				personWidgetItem->setText		( 0, indexp->toMap()["fullName"].toString() );
				if (!devicesNeed)
					personWidgetItem->setData( 1, 1, IDp );
				personWidgetItem->setIcon(0, personIcon);
				personWidgetItem->setFlags(personWidgetItem->flags() | Qt::ItemIsUserCheckable);
				personWidgetItem->setCheckState(0,Qt::Unchecked);
				
				if (!devicesNeed)
				{
					childWidgetItem->addChild ( personWidgetItem );
					continue;
				}
				for (auto inddev = devices.begin(); inddev != devices.end(); ++inddev)
				{
					int IDdev = inddev->toMap()["ID"].toInt();
					if (IDdev == 0)
						continue;

					int IDOwner = inddev->toMap()["OwnerID"].toInt();
					if (IDOwner != IDp)
						continue;

					QTreeWidgetItem* deviceWidgetItem = new QTreeWidgetItem;
					deviceWidgetItem->setText		( 0, inddev->toMap()["name"].toString() );
					deviceWidgetItem->setData( 1, 1, IDdev );
					deviceWidgetItem->setIcon(0, photoIcon);
					deviceWidgetItem->setFlags(deviceWidgetItem->flags() | Qt::ItemIsUserCheckable);
					deviceWidgetItem->setCheckState(0,Qt::Unchecked);
					personWidgetItem->addChild ( deviceWidgetItem );
				}
				if (personWidgetItem->childCount())
					childWidgetItem->addChild ( personWidgetItem );
				else
					delete personWidgetItem;
			}// for (auto indexp = people.begin(); indexp != people.end(); ++indexp)
			if (childWidgetItem->childCount())
				treeWidgetItem->addChild ( childWidgetItem );
			else
				delete childWidgetItem;
		} // for (auto index = vlist1.begin(); index != vlist1.end(); ++index)
		if (treeWidgetItem->childCount())
			pTreeWidget->addTopLevelItem ( treeWidgetItem );
		else
			delete treeWidgetItem;		
	}
}
void fvaFindCheckedItem(QTreeWidgetItem *item, QVector<unsigned int>& Ids)
{
	if (item->checkState(0) == Qt::CheckState::Checked)
	{
		int ID = item->data(1, 1).toInt();
		if (ID)
			Ids.push_back(ID);
	}

	for (auto id = 0 ; id < item->childCount(); ++id)
		fvaFindCheckedItem(item->child(id), Ids);
}
void fvaUpdateChecks(QTreeWidgetItem *item, int column)
{
    bool diff = false;
    if(column != 0 && column!=-1)
        return;
    if(item->childCount()!=0 && item->checkState(0)!=Qt::PartiallyChecked && column!=-1){
        Qt::CheckState checkState = item->checkState(0);
        for (int i = 0; i < item->childCount(); ++i) {
           item->child(i)->setCheckState(0, checkState);
        }
    } else if (item->childCount()==0 || column==-1) {
        if(item->parent()==0)
            return;
        for (int j = 0; j < item->parent()->childCount(); ++j) {
            if(j != item->parent()->indexOfChild(item) && item->checkState(0)!=item->parent()->child(j)->checkState(0)){
                diff = true;
            }
        }
        if(diff)
            item->parent()->setCheckState(0,Qt::PartiallyChecked);
        else
            item->parent()->setCheckState(0,item->checkState(0));
        if(item->parent()!=0)
            fvaUpdateChecks(item->parent(),-1);
    }
}

QVector<unsigned int> fvaStringToIds(const QString& strList)
{
	QVector<unsigned int> result;
	QStringList l = strList.split(',');
	for ( auto iter = l.begin(); iter != l.end(); ++iter )
		result.append(iter->toUInt());					

	return result;
}

FVA_ERROR_CODE fvaLoadItems(FVA_ITEM_MAP& fvaItemsMap, const QString& DBPath, QString& error)
{
	fvaItemsMap.clear();
	QDir dir ( DBPath );
	if ( !dir.exists( DBPath ) )
	{
		error = "dictionaries file does not exist" ;
		return FVA_ERROR_CANT_FIND_FVA_DB;
	}

	QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(DBPath);
    if (!dbase.open()) 
	{
		error =  "can not open fva DB";
		return FVA_ERROR_CANT_OPEN_FVA_DB;
	}
	QSqlQuery query;
    if (!query.exec("SELECT * FROM fva")) 
		return FVA_ERROR_CANT_LOAD_FVA_DB;
	QSqlRecord		rec = query.record();
	QVariantList	list;
	QVariantMap		map;
    while (query.next()) 
	{
		fvaItem item;
		// fill up the item
		item.type = (query.value(rec.indexOf("Type")) == "1") ? FVA_FS_TYPE_DIR : FVA_FS_TYPE_UNKNOWN;
		item.dateFrom;
		item.dateTo;
		
		item.fsFullPath = query.value(rec.indexOf("Name")).toString();
		item.deviceId = query.value(rec.indexOf("device")).toInt();
		item.peopleIds = fvaStringToIds(query.value(rec.indexOf("people")).toString());
		item.placeId = query.value(rec.indexOf("place")).toInt();
		
		item.eventId = query.value(rec.indexOf("d_event")).toInt();
		item.eventReasonPeopleIds = fvaStringToIds(query.value(rec.indexOf("d_reasonPeople")).toString());
		item.linkedFolder = query.value(rec.indexOf("d_linkedFolder")).toString();

		item.fsFullPath;
		item.hasDescriptionData = true;
		item.isFiltered = false;
		item.scanerId = query.value(rec.indexOf("f_Scaner")).toInt();
		item.tagsOrComment = query.value(rec.indexOf( (item.type == FVA_FS_TYPE_DIR) ? "d_tags": "f_Comment" )).toString();

		// add item to a map
		fvaItemsMap[item.fsFullPath] = item;
    }
	return FVA_NO_ERROR;
}