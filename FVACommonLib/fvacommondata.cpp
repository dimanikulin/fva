#include "fvacommondata.h"


fvaFolder::fvaFolder()
{
	eventId				= 0;
}

fvaFolder::~fvaFolder()
{

}

fvaFile::fvaFile()
{
	deviceId			= 0;
	scanerId			= 0;
	placeId				= 0;
}

fvaFile::~fvaFile()
{

}

fvaItem::fvaItem ()
{
	isFiltered			= true;
	type				= FVA_FS_TYPE_UNKNOWN;
	fvaFolder			= nullptr;
	fvaFile				= nullptr;
}

fvaItem::~fvaItem ()
{
	for (auto idChild = children.begin(); idChild != children.end(); ++idChild)
	{
		if (*idChild == nullptr)
			continue;
		delete *idChild;
		*idChild = nullptr;
	}

	if ( nullptr != fvaFolder )
		delete fvaFolder;

	if ( nullptr != fvaFile )
		delete fvaFile;
}
QString fvaItem::getGuiName(const QVariantMap&	dictionaries)
{
	if (type == FVA_FS_TYPE_DIR)
	{
		QString desc;
		if ( fvaFolder && fvaFolder->eventId != 0)
		{
			QVariantList vlist = dictionaries["events"].toList();
			for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
			{
				if ( i->toMap()["ID"].toInt() == fvaFolder->eventId )
				{
					desc =	 " - " + i->toMap()["fullName"].toString();
					break;
				}
			}

			if ( fvaFolder && !fvaFolder->eventReasonPeopleIds.isEmpty())
			{
				QVariantList vlist = dictionaries["people"].toList();
				for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
				{
					if ( ( i->toMap()["ID"].toInt() == fvaFolder->eventReasonPeopleIds[0])
					&& fvaFolder->eventReasonPeopleIds[0] )
					{
						desc +=	 "," + i->toMap()["name"].toString();
						break;
					}
				}
			}
		}

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
		for (auto i = vlist.begin(); i != vlist.end(); ++i)
		{
			if (i->toMap()["ID"].toInt() == ident)
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
	if (!fvaFolder && !fvaFile)
		return "";
	if (type != FVA_FS_TYPE_DIR && fvaFile)
	{
		if (!fvaFile->description.isEmpty())
			fullName = fvaFile->description;
	}

	if (type != FVA_FS_TYPE_DIR && fvaFile)
	{
		if (fullName.isEmpty())
			fullName = fvaFile->comment;
		else
			fullName += ", " + fvaFile->comment;
	}
	else if (type == FVA_FS_TYPE_DIR && fvaFolder)
	{
		if (fullName.isEmpty())
			fullName = fvaFolder->tags;
		else
			fullName += ", " + fvaFolder->tags;
	}

	if (type != FVA_FS_TYPE_DIR && fvaFile)
	{
		fillNameByOneId(fvaFile->deviceId, "devices", dictionaries, fullName);
	}
	return fullName;
}

bool fvaFilter::doesIDMatchToFilter(unsigned int ID, const QVector<unsigned int>& Ids) const
{
	for (auto it = Ids.begin(); it != Ids.end(); ++it)
	{
		if (ID == *it)
			return  true;
	}
	return false;
}

bool fvaFilter::doIDsMatchToFilter(const QVector<unsigned int>& IDs, const QVector<unsigned int>& filterIds) const
{
	for (auto it = IDs.begin(); it != IDs.end(); ++it)
	{
		if (doesIDMatchToFilter(*it, filterIds))
			return  true;
	}
	return false;
}

void fvaFilterTree( const fvaFilter& filter, fvaItem* fvaitem, const QDateTime& defFilterDataTime )
{
	for ( auto idChild = fvaitem->children.begin(); idChild != fvaitem->children.end(); ++idChild)
	{
		// reset previous filtration result
		(*idChild)->isFiltered = true;

		// 1. filtration by time
		if (defFilterDataTime		!= filter.dateFrom 
			&& defFilterDataTime	!= filter.dateTo)
		{
			if ( (*idChild)->type == FVA_FS_TYPE_DIR )
			{
				if ((*idChild)->dateFrom != (*idChild)->dateTo)
					(*idChild)->isFiltered = ((*idChild)->dateFrom >= filter.dateFrom)
									&& ((*idChild)->dateTo <= filter.dateTo);
				/*qDebug() << "((*idChild)->dateFrom="		<< (*idChild)->dateFrom
						<< "filter.dateFrom="			<< filter.dateFrom
						<< "((*idChild)->dateTo="		<< (*idChild)->dateTo
						<< "filter.dateTo="				<< filter.dateTo;*/
			}
			else
			{
				(*idChild)->isFiltered = ((*idChild)->dateFrom >= filter.dateFrom) 
									&& ((*idChild)->dateFrom <= filter.dateTo);
			}
		}
		
		// 2. filtration by device id
		if ( (*idChild)->type != FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.deviceIds.empty())
		{
			if ((*idChild)->fvaFile)
				(*idChild)->isFiltered = filter.doesIDMatchToFilter((*idChild)->fvaFile->deviceId,filter.deviceIds);
			else
				(*idChild)->isFiltered = false/*filter.doesIDMatchToFilter(fvaitem->deviceId,filter.deviceIds)*/;
		}
		// 3. filtration by event id
		if ( (*idChild)->type == FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.eventIds.empty())
		{
			if ((*idChild)->fvaFolder)
				(*idChild)->isFiltered = filter.doesIDMatchToFilter((*idChild)->fvaFolder->eventId,filter.eventIds);
			else if ( fvaitem->type == FVA_FS_TYPE_DIR && fvaitem->fvaFolder )
			{
				(*idChild)->isFiltered = filter.doesIDMatchToFilter(fvaitem->fvaFolder->eventId,filter.eventIds);
			}
		}

		// 4. filtration by place ids
		if ( (*idChild)->type != FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.placeIds.empty())
		{
			if ((*idChild)->fvaFile)
				(*idChild)->isFiltered = filter.doesIDMatchToFilter((*idChild)->fvaFile->placeId,filter.placeIds);
			else
				(*idChild)->isFiltered = false/*filter.doesIDMatchToFilter(fvaitem->placeId,filter.placeIds)*/;
		}
		// 5. filtration by people ids
		if ( (*idChild)->type != FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.peopleIds.empty()) 
		{	
			if ((*idChild)->fvaFile)
				(*idChild)->isFiltered = filter.doIDsMatchToFilter((*idChild)->fvaFile->peopleIds,filter.peopleIds);
			else
				(*idChild)->isFiltered = false/*filter.doIDsMatchToFilter(fvaitem->peopleIds,filter.peopleIds)*/;
		}
		// 6. filtration by event, desciption or comment
		if ( (*idChild)->isFiltered && !filter.text.isEmpty() && ((*idChild)->fvaFile || (*idChild)->fvaFolder ) )
		{
			if ( (*idChild)->type == FVA_FS_TYPE_DIR && (*idChild)->fvaFolder )
			{
				(*idChild)->isFiltered = ((*idChild)->fvaFolder->tags == filter.text);
				// if (!(*idChild)->isFiltered)
					// (*idChild)->isFiltered = ((*idChild)->tagsOrComment == filter.text);
			}
		}

		// 7. filtration by event reason people ids
		if ( (*idChild)->type == FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.eventReasonPeopleIds.empty()) 
		{	
			if ( (*idChild)->fvaFolder )
				(*idChild)->isFiltered = filter.doIDsMatchToFilter( (*idChild)->fvaFolder->eventReasonPeopleIds,filter.eventReasonPeopleIds);
			else
				(*idChild)->isFiltered = false/*filter.doIDsMatchToFilter(fvaitem->eventReasonPeopleIds,filter.eventReasonPeopleIds)*/;
		}

		/*if ((*idChild)->isFiltered)
			qDebug() << "filtered name = " << (*idChild)->fsFullPath << " hasDescriptionData=" << (*idChild)->hasDescriptionData;
		*/
		fvaFilterTree( filter, *idChild, defFilterDataTime );

		// TODO make dir filtered if any child filtered and wiseversa
		if ((*idChild)->isFiltered && !fvaitem->isFiltered)
			fvaitem->isFiltered = true;
	}							
}
