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


