#include "fvaItem.h"
#include "fvafile.h"
#include "fvafolder.h"

#include <iostream>


fvaItem::fvaItem ()
{
	isFiltered			= true;
	type				= FVA_FS_TYPE_UNKNOWN;
	pFvaFolder			= nullptr;
	pFvaFile			= nullptr;
}

fvaItem::~fvaItem ()
{
	std::cout << "fvaItem destructor" << std::endl;
	for (auto idChild = children.begin(); idChild != children.end(); ++idChild)
	{
		if (*idChild == nullptr)
			continue;
		delete *idChild;
		*idChild = nullptr;
	}

	if ( nullptr != pFvaFolder )
		delete pFvaFolder;

	if ( nullptr != pFvaFile )
		delete pFvaFile;
		
	std::cout << "fvaItem destructor end" << std::endl;
}
QString fvaItem::getGuiName(const QVariantMap&	dictionaries)
{
	if (type == FVA_FS_TYPE_DIR)
	{
		QString desc;
		if ( pFvaFolder && pFvaFolder->eventId != 0)
		{
			QVariantList vlist = dictionaries["events"].toList();
			for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
			{
				if ( i->toMap()["ID"].toUInt() == pFvaFolder->eventId )
				{
					desc =	 " - " + i->toMap()["fullName"].toString();
					break;
				}
			}

			if ( pFvaFolder && !pFvaFolder->eventReasonPeopleIds.isEmpty())
			{
				QVariantList vlist = dictionaries["people"].toList();
				for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
				{
					if ( ( i->toMap()["ID"].toUInt() == pFvaFolder->eventReasonPeopleIds[0])
					&& pFvaFolder->eventReasonPeopleIds[0] )
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
	if (!pFvaFolder && !pFvaFile)
		return "";
	if (type != FVA_FS_TYPE_DIR && pFvaFile)
	{
		if (!pFvaFile->description.isEmpty())
			fullName = pFvaFile->description;
	}

	if (type != FVA_FS_TYPE_DIR && pFvaFile)
	{
		if (fullName.isEmpty())
			fullName = pFvaFile->comment;
		else
			fullName += ", " + pFvaFile->comment;
	}
	else if (type == FVA_FS_TYPE_DIR && pFvaFolder)
	{
		if (fullName.isEmpty())
			fullName = pFvaFolder->tags;
		else
			fullName += ", " + pFvaFolder->tags;
	}

	if (type != FVA_FS_TYPE_DIR && pFvaFile)
	{
		fillNameByOneId(pFvaFile->deviceId, "devices", dictionaries, fullName);
	}
	return fullName;
}
