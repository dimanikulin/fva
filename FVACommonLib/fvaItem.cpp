#include "fvaItem.h"
#include "fvafile.h"
#include "fvafolder.h"

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
				if ( i->toMap()["ID"].toUInt() == fvaFolder->eventId )
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
					if ( ( i->toMap()["ID"].toUInt() == fvaFolder->eventReasonPeopleIds[0])
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
