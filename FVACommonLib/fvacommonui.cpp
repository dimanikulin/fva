/*!
* \file fvacommonui.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include <QtWidgets/QMessageBox>
#include <QTGui/QPainter>
#include <QTGui/QIcon>
#include <QtCore/QCoreApplication>

#include "fvacommoncsv.h"
#include "fvalogger.inl"
#include "fvacommonui.h"

FVA_EXIT_CODE fvaShowImage( const QString& fileName, QLabel* imgLabel, const QString& text )
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

FVA_EXIT_CODE fvaBuildEventTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, const QString& rootSWdir)
{
	pMainWnd->connect(pTreeWidget, 
			SIGNAL(itemChanged(QTreeWidgetItem*, int)), 
			pMainWnd,
			SLOT(updateChecks(QTreeWidgetItem*, int)));

	QIcon	personIcon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/person.png");
	QIcon	peopleIcon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/people.png");
	QIcon	photoIcon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/photo.png");

	FVA_SIMPLE_MAP eventTypesMap;
	FVA_EXIT_CODE res = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, eventTypesMap, "fvaEventTypes.csv");
	RET_RES_IF_RES_IS_ERROR

	for (auto i = eventTypesMap.begin(); i != eventTypesMap.end(); ++i)
	{
		int ID = i->first;
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText(0, i->second);
		treeWidgetItem->setFlags(treeWidgetItem->flags() | Qt::ItemIsUserCheckable);
		treeWidgetItem->setCheckState(0, Qt::Unchecked);

		FVA_SIMPLE_MAP  eventsMap;
		// load only events for this type
		res = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, eventsMap, "fvaEvents.csv", ID);
		RET_RES_IF_RES_IS_ERROR

		for (auto index = eventsMap.begin(); index != eventsMap.end(); ++index)
		{
			QTreeWidgetItem* childWidgetItem = new QTreeWidgetItem;
			childWidgetItem->setText(0, index->second);
			childWidgetItem->setFlags(childWidgetItem->flags() | Qt::ItemIsUserCheckable);
			childWidgetItem->setCheckState(0, Qt::Unchecked);
		} // for (auto index = eventsMap.begin(); index != eventsMap.end(); ++index) 
	} // for (auto i = eventTypesMap.begin(); i != eventTypesMap.end(); ++i)
	return FVA_NO_ERROR;

}
// #define _SHOW_ICONS_
FVA_EXIT_CODE fvaBuildPeopleFilterTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, bool devices, const QString& rootSWdir)
{
	LOG_DEB << "fvaBuildPeopleFilterTree enter";
	/*pMainWnd->connect(pTreeWidget,
		SIGNAL(itemChanged(QTreeWidgetItem*, int)),
		pMainWnd,
		SLOT(updateChecks(QTreeWidgetItem*, int)));
#ifdef _SHOW_ICONS_
	QIcon	personIcon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/person.png");
	QIcon	peopleIcon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/people.png");
	QIcon	photoIcon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/photo.png");
#endif // _SHOW_ICONS_
	FVA_SIMPLE_MAP RelationsMap;
	FVA_EXIT_CODE res = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, RelationsMap, "fvaPeopleRelationTypes.csv");
	RET_RES_IF_RES_IS_ERROR

	FVA_PEOPLE_RELATION_MAP  peopleRelationsMap;
	res = fvaLoadPeopleRelationMapFromCsv(rootSWdir, peopleRelationsMap);
	RET_RES_IF_RES_IS_ERROR

	DEVICE_MAP deviceMap;
	res = fvaLoadDeviceMapFromCsv(rootSWdir, deviceMap);
	RET_RES_IF_RES_IS_ERROR

	PEOPLE_MAP peopleMap;
	res = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
	RET_RES_IF_RES_IS_ERROR
	LOG_DEB << "for (auto i = RelationsMap.begin(); i != RelationsMap.end(); ++i)";
	for (auto i = RelationsMap.begin(); i != RelationsMap.end(); ++i)
	{
		int ID = i->first;
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText(0, i->second);
		LOG_DEB << "treeWidgetItem->setIcon(0, peopleIcon);"; 
#ifdef _SHOW_ICONS_
		treeWidgetItem->setIcon(0, peopleIcon);
#endif // _SHOW_ICONS_
		treeWidgetItem->setFlags(treeWidgetItem->flags() | Qt::ItemIsUserCheckable);
		treeWidgetItem->setCheckState(0, Qt::Unchecked);
		LOG_DEB << "for (auto index = peopleRelationsMap.begin(); index != peopleRelationsMap.end(); ++index)";
		for (auto index = peopleRelationsMap.begin(); index != peopleRelationsMap.end(); ++index)
		{
			int IDc = index->Id;
			int IDrel = index->relationType;
			if (IDrel != ID)
				continue;

			QTreeWidgetItem* childWidgetItem = new QTreeWidgetItem;
			childWidgetItem->setText(0, index->name);
			LOG_DEB << "childWidgetItem->setIcon(0, peopleIcon);";
#ifdef _SHOW_ICONS_
			childWidgetItem->setIcon(0, peopleIcon);
#endif // #ifdef _SHOW_ICONS_
			childWidgetItem->setFlags(childWidgetItem->flags() | Qt::ItemIsUserCheckable);
			childWidgetItem->setCheckState(0, Qt::Unchecked);
                        LOG_DEB << "auto indexp = peopleMap.begin(); indexp != peopleMap.end(); ++indexp";
			for (auto indexp = peopleMap.begin(); indexp != peopleMap.end(); ++indexp)
			{
				int IDp = indexp->Id;
				if (IDp == 0)
					continue;

				int IDrelp = indexp->relationId;
				if (IDrelp != IDc)
					continue;

				QTreeWidgetItem* personWidgetItem = new QTreeWidgetItem;

				personWidgetItem->setText(0, indexp->fullName);
				if (!devices)
					personWidgetItem->setData(1, 1, IDp);
				LOG_DEB << "personWidgetItem->setIcon(0, personIcon)";
#ifdef _SHOW_ICONS_
				personWidgetItem->setIcon(0, personIcon);
#endif //_SHOW_ICONS_
				personWidgetItem->setFlags(personWidgetItem->flags() | Qt::ItemIsUserCheckable);
				personWidgetItem->setCheckState(0, Qt::Unchecked);

				if (!devices)
				{
					childWidgetItem->addChild(personWidgetItem);
					continue;
				}
				LOG_DEB << "for (auto inddev = deviceMap.begin(); inddev != deviceMap.end(); ++inddev)"; 
				for (auto inddev = deviceMap.begin(); inddev != deviceMap.end(); ++inddev)
				{
					int IDdev = inddev->deviceId;
					if (IDdev == 0)
						continue;

					int IDOwner = inddev->ownerId;
					if (IDOwner != IDp)
						continue;

					QTreeWidgetItem* deviceWidgetItem = new QTreeWidgetItem;
					deviceWidgetItem->setText(0, inddev->guiName);
					deviceWidgetItem->setData(1, 1, IDdev);
					LOG_DEB << "deviceWidgetItem->setIcon(0, photoIcon);";
#ifdef _SHOW_ICONS_
					deviceWidgetItem->setIcon(0, photoIcon);
#endif //_SHOW_ICONS_
					deviceWidgetItem->setFlags(deviceWidgetItem->flags() | Qt::ItemIsUserCheckable);
					deviceWidgetItem->setCheckState(0, Qt::Unchecked);
					personWidgetItem->addChild(deviceWidgetItem);
				}
				if (personWidgetItem->childCount())
					childWidgetItem->addChild(personWidgetItem);
				else
					delete personWidgetItem;
			}// for (auto indexp = people.begin(); indexp != people.end(); ++indexp)
			if (childWidgetItem->childCount())
				treeWidgetItem->addChild(childWidgetItem);
			else
				delete childWidgetItem;
		} // for (auto index = vlist1.begin(); index != vlist1.end(); ++index)
		if (treeWidgetItem->childCount())
			pTreeWidget->addTopLevelItem(treeWidgetItem);
		else
			delete treeWidgetItem;
	}*/
	LOG_DEB << "fvaBuildPeopleFilterTree exit";
	return FVA_NO_ERROR;
}

void fvaBuildPeopleFilterTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, bool devicesNeed, const QVariantMap& dict )
{
	pMainWnd->connect(pTreeWidget, 
			SIGNAL(itemChanged(QTreeWidgetItem*, int)), 
			pMainWnd,
			SLOT(updateChecks(QTreeWidgetItem*, int)));

	QIcon	personIcon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/person.png");
	QIcon	peopleIcon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/people.png");
	QIcon	photoIcon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/photo.png");

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

void fvaFindCheckedItem(QTreeWidgetItem *item, QList<unsigned int>& Ids)
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
