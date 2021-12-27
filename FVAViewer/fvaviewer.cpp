/*!
* \file FVAViewer.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvaviewer.h"
#include "qpicturelabel.h"

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QListWidget>
#include <QProgressDialog>

#include "fvacommonui.h"

#include "fvaItem.h"
#include "fvafolder.h"
#include "fvafile.h"

void FVAViewer::showProgress(const QString& rootDir)
{
	QDir dir ( rootDir );
	if (!dir.exists( rootDir ))
		return;

	int number = 0;
	m_rootItem.reset (new fvaItem);
	QString error;
	// fvaLoadItems ( rootDir, m_rootItem.get(), m_dictionaryPath, error, number /*, &progress*/ );
	QProgressDialog progress("Loading...", "Abort", 0, number, this);
	progress.setWindowModality(Qt::ApplicationModal); 
	int itemNumber = 0;
	populateGUITree( m_rootItem.get(), nullptr, itemNumber, &progress );
	progress.setValue(number);
}

void FVAViewer::prepareFilters()
{
	m_ui->dockWidget->hide();
	m_ui->dockWidget->setFloating(true);
	QTabWidget* tabs = new QTabWidget(m_ui->dockWidget);
	tabs->setFixedSize(600, 500);
	m_ui->dockWidget->setFixedSize(600, 500);
	
	QDialog *tabCommonFilters	= new QDialog ();
	m_uiFiltersCommon			= new Ui_commonFilter ;
	m_uiFiltersCommon->setupUi(tabCommonFilters);
  	tabs->addTab(tabCommonFilters,tr("Общие фильтры"));
	connect(m_uiFiltersCommon->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

	m_defFilterDataTime	= QDateTime::currentDateTime();	
	m_uiFiltersCommon->dateTimeFrom->setDateTime ( m_defFilterDataTime );
	m_uiFiltersCommon->dateTimeTo->setDateTime ( m_defFilterDataTime );

	QDialog *tabEventFilters	= new QDialog ();
	m_uiEventFilter				= new Ui_eventFilter;
	m_uiEventFilter->setupUi(tabEventFilters);
	tabs->addTab(tabEventFilters,tr("События"));
	connect(m_uiEventFilter->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

	QDialog *tabPeopleFilters	= new QDialog ();
	m_uiPeopleFilter			= new Ui_hfilter;
	m_uiPeopleFilter->setupUi(tabPeopleFilters);
  	tabs->addTab(tabPeopleFilters,tr("Люди"));
	connect(m_uiPeopleFilter->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

	QDialog *tabGeoFilters		= new QDialog ();
	m_uiPlaceFilter				= new Ui_placeFilter;
	m_uiPlaceFilter->setupUi(tabGeoFilters);
  	tabs->addTab(tabGeoFilters,tr("Места"));
	connect(m_uiPlaceFilter->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

	QDialog *tabDeviceFilters	= new QDialog ();
	m_uiDeviceFilter			= new Ui_dfilter;
	m_uiDeviceFilter->setupUi(tabDeviceFilters);
  	tabs->addTab(tabDeviceFilters,tr("Устройства"));
	connect(m_uiDeviceFilter->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));
	
	// tabs->addTab(new QWidget(),tr("Свойства"));
	
	fvaBuildPeopleFilterTree(this, m_uiPeopleFilter->treeWidget, false, m_dictionaries);
	fvaBuildPeopleFilterTree(this, m_uiDeviceFilter->treeWidget, true, m_dictionaries);
	fvaBuildPeopleFilterTree(this, m_uiEventFilter->ptreeWidget, false, m_dictionaries);

	fvaBuildFilterTree( this ,
						m_uiPlaceFilter->treeWidget, 
						m_dictionaries["placeTypes"].toList(), 
						m_dictionaries["places"].toList(),
						&m_folderIcon,
						&m_locationIcon);

	fvaBuildFilterTree( this ,
						m_uiEventFilter->treeWidget, 
						m_dictionaries["eventTypes"].toList(), 
						m_dictionaries["events"].toList(),
						&m_folderIcon,
						&m_locationIcon);
}

FVAViewer::FVAViewer(const QString& rootDir, const QString& dictPath, QWidget *parent, Qt::WindowFlags /*flags*/)
	:QDialog 			(parent),
	m_ui				(new Ui::FVAViewerClass),
	m_dictionaryPath	(dictPath)
{
	m_ui->setupUi(this);

	m_videoIcon		= QIcon (QCoreApplication::applicationDirPath() + "/Icons/video.png");
	m_audioIcon		= QIcon (QCoreApplication::applicationDirPath() + "/Icons/audio.png");
	m_photoIcon		= QIcon (QCoreApplication::applicationDirPath() + "/Icons/photo.png");
	m_folderIcon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/folder.png");
	m_locationIcon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/location.png");
	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);

	/*QString		error;
	FVA_EXIT_CODE res = fvaLoadDictionary( dictPath, m_dictionaries, error );
	RET_IF_RES_IS_ERROR*/

	showProgress(rootDir);

	m_ui->treeWidget->setMaximumWidth(300);

	m_ui->imageLbl->setBackgroundRole(QPalette::Base);
	m_ui->contentArea->setBackgroundRole(QPalette::Dark);

	connect(m_ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* ,int)),this,SLOT(showItem(QTreeWidgetItem*)));
	connect(m_ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem* ,int)),this,SLOT(editFileItem(QTreeWidgetItem*)));
	
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
	
	prepareFilters();
}
void FVAViewer::updateChecks(QTreeWidgetItem *item, int column)
{
	fvaUpdateChecks(item, column);
}
FVAViewer::~FVAViewer()
{
	delete	m_ui;
	delete	m_uiFiltersCommon;
	delete	m_uiDeviceFilter;
	delete	m_uiPlaceFilter;
	delete	m_uiPeopleFilter;
	delete	m_uiEventFilter;
}

void FVAViewer::ShowContextMenu(const QPoint &)
{
	m_ui->dockWidget->show();
}
void FVAViewer::showItem( QTreeWidgetItem* item )
{
	fvaItem* fvaitem = (fvaItem*) item->data( 1, 1 ).toInt() ;
	if (!fvaitem)
		return;
	setWindowTitle(fvaitem->getGuiName(m_dictionaries) 
		+" [" 
		+ fvaitem->getGuiFullName(m_dictionaries) 
		+ "] (" + fvaitem->fsFullPath + ")");
	QFileInfo info(fvaitem->fsFullPath);
	QString suffix = info.suffix().toUpper();
	if(	FVA_FS_TYPE_VIDEO == fvaConvertFileExt2FileType ( suffix )
		|| FVA_FS_TYPE_AUDIO == fvaConvertFileExt2FileType ( suffix ))
	{
		QProcess myProcess(this);    
		myProcess.setProcessChannelMode(QProcess::MergedChannels);
		QStringList params;
		params.append(fvaitem->fsFullPath);
		QString path = QCoreApplication::applicationDirPath() + "/FVAPlayer.exe";
		myProcess.start(path, params);
		myProcess.waitForFinished( -1 );
	} 
	else if (FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ))
	{
		fvaShowImage( fvaitem->fsFullPath, m_ui->imageLbl, fvaitem->getGuiName(m_dictionaries) );
	}
}

void FVAViewer::editFileItem( QTreeWidgetItem* item )
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(m_dictionaryPath);
	fvaItem* fvaitem = (fvaItem*) item->data( 1, 1 ).toInt() ;
	if (!fvaitem)
		return;
	params.append(fvaitem->fsFullPath);
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVADescriptionEditor.exe", params);
	myProcess.waitForFinished( -1 );
}

void FVAViewer::filterClicked(  )
{
	m_filter.dateFrom	= m_uiFiltersCommon->dateTimeFrom->dateTime();
	m_filter.dateTo		= m_uiFiltersCommon->dateTimeTo->dateTime();
	
	m_filter.deviceIds.clear();
	m_filter.peopleIds.clear();
	m_filter.placeIds.clear();
	m_filter.eventIds.clear();
	m_filter.eventReasonPeopleIds.clear();

	for (auto idTop = 0; idTop < m_uiPeopleFilter->treeWidget->topLevelItemCount();++idTop )
		fvaFindCheckedItem(m_uiPeopleFilter->treeWidget->topLevelItem(idTop), m_filter.peopleIds);

	for (auto idTop = 0; idTop < m_uiDeviceFilter->treeWidget->topLevelItemCount();++idTop )
		fvaFindCheckedItem(m_uiDeviceFilter->treeWidget->topLevelItem(idTop), m_filter.deviceIds);

	for (auto idTop = 0; idTop < m_uiPlaceFilter->treeWidget->topLevelItemCount();++idTop )
		fvaFindCheckedItem(m_uiPlaceFilter->treeWidget->topLevelItem(idTop), m_filter.placeIds);

	for (auto idTop = 0; idTop < m_uiEventFilter->treeWidget->topLevelItemCount();++idTop )
		fvaFindCheckedItem(m_uiEventFilter->treeWidget->topLevelItem(idTop), m_filter.eventIds);

	for (auto idTop = 0; idTop < m_uiEventFilter->ptreeWidget->topLevelItemCount();++idTop )
		fvaFindCheckedItem(m_uiEventFilter->ptreeWidget->topLevelItem(idTop), m_filter.eventReasonPeopleIds);

	m_filter.text = m_uiFiltersCommon->editEvent->text();

	fvaFilterTree( m_filter, m_rootItem.get(), m_defFilterDataTime );
	m_ui->treeWidget->clear();
	int number = 0;
	populateGUITree( m_rootItem.get(), nullptr, number, nullptr );
}

void FVAViewer::populateGUITree( const fvaItem* fvaitem, QTreeWidgetItem* item,int& number, QProgressDialog* progress )
{
	for ( auto idChild = fvaitem->children.begin(); idChild != fvaitem->children.end(); ++idChild)
	{ 
		if (progress) 
		{
			progress->setValue(number++);
			// progress->setLabelText((*idChild)->getGuiName()); 
		}
		if ( !(*idChild)->isFiltered )
			continue;
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText		( 0, (*idChild)->getGuiName(m_dictionaries) );

		// TODO fix error here - cast from 'fvaItem*' to 'int' loses precision [-fpermissive]
		// treeWidgetItem->setData( 1, 1, (int) *idChild );

		if ((*idChild)->type == FVA_FS_TYPE_DIR)
			treeWidgetItem->setIcon(0, m_folderIcon);
		else
		{
			switch ((*idChild)->type)
			{
				case FVA_FS_TYPE_IMG: 
					treeWidgetItem->setIcon(0, m_photoIcon);
				break;
				case FVA_FS_TYPE_VIDEO: 
					treeWidgetItem->setIcon(0, m_videoIcon);
				break;
				case FVA_FS_TYPE_AUDIO: 
					treeWidgetItem->setIcon(0, m_audioIcon);
				break;
				
			}
		}
		if ( item )
			item->addChild( treeWidgetItem );
		else
			m_ui->treeWidget->addTopLevelItem ( treeWidgetItem );

		populateGUITree( *idChild, treeWidgetItem, number, progress );
	}							
}
/*
void FVAViewer::populateFVATree( const QString& folder, fvaItem* fvaitem, int& number, QProgressDialog* progress )
{	
	if (progress.wasCanceled())
		return;
	
	QDir dir( folder );
	Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{			
		if ( info.isDir() )
		{
			// just skip internal folder
			if ( info.fileName()[0] == '#' 
				&& info.fileName()[info.fileName().size()-1] == '#' )
			{
				continue;
			}
			fvaItem* dirItem	= new fvaItem;
			if (progress)
			{
				progress->setValue(number++);
				//progress->setLabelText(info.absoluteFilePath()); 
			}
			dirItem->type = FVA_FS_TYPE_DIR;
			dirItem->fsFullPath	= info.absoluteFilePath();
			if ( FVA_NO_ERROR != fvaParseDirName(info.fileName(), dirItem->dateFrom, dirItem->dateTo))
			{
				qCritical() << "incorrect folder name " << info.fileName();
				delete dirItem;
				continue;
			}
			QVariantMap result;
			QString error;

			FVA_EXIT_CODE code = fvaGetFolderDescription( info.absoluteFilePath(), result, error );
			if ( FVA_NO_ERROR != code )
			{
				qWarning() << error;
				
				// delete dirItem;
				// continue;
			}
			else
			{
				dirItem->placeId				=	result["place"]			.toUInt();
				dirItem->peopleIds				=	fvaStringToIds(result["people"].toString());
				dirItem->deviceId				=	result["deviceId"]		.toUInt();
				dirItem->eventId				=	result["event"]			.toUInt();
				dirItem->eventReasonPeopleIds	=	fvaStringToIds(result["reasonPeople"].toString());
				dirItem->tagsOrComment			=	result["tags"]			.toString();
				dirItem->linkedFolder			=	result["linkedFolder"]	.toString();
				dirItem->hasDescriptionData		=	true;

				if (!dirItem->deviceId)
					qCritical() << "0 device if for folder " << info.fileName();
				if (!dirItem->placeId)
					qCritical() << "0 placeId if for folder " << info.fileName();

				qDebug() << "FINFO" << info.fileName()
					<<"place=" <<  result["place"].toString()
					<<"people=" <<  result["people"].toString()
					<< "event= " << result["event"].toString()
					// << "resPeople=" << result["reasonPeople"].toString()
					<< "tags= " << result["tags"].toString()
					<< "linDir= " << result["linkedFolder"].toString();
					
			}
			QString descFilePath = info.absoluteFilePath() + "/" + FVA_DESCRIPTION_FILE_NAME;
			FVA_EXIT_CODE res = m_descriptionFile.load( descFilePath,dirItem->descTitles, dirItem->decsItems );
			if ( FVA_ERROR_CANT_OPEN_FILE_DESC == res || FVA_NO_ERROR == res)
			{
				// it is not an error
			}
			else
				qCritical() << "descFile failed to be loaded:" << descFilePath;

			fvaitem->children.append( dirItem );
			populateFVATree( info.absoluteFilePath(), dirItem, number, progress );
		}
		else
		{
			if(	!fvaIsFVAFile ( info.suffix().toUpper() ) )
				continue;
			fvaItem* fileItem		= new fvaItem;
			fileItem->fsFullPath	= info.absoluteFilePath();
			fileItem->type			= fvaConvertFileExt2FileType(info.suffix().toUpper());
			if (progress)
			{
				progress->setValue(number++);
				//progress->setLabelText(info.absoluteFilePath()); 
			}
			if ( FVA_NO_ERROR != fvaParseFileName(info.baseName(), fileItem->dateFrom))
			{
				qDebug() << "ERROR!, incorrect file name " << info.fileName();
				delete fileItem;
				continue;
			}
			if (!fvaitem->decsItems.isEmpty() && !fvaitem->descTitles.isEmpty())
			{
				auto it = fvaitem->decsItems.find( info.fileName().toUpper() );
				if ( it != fvaitem->decsItems.end() )
				{
					auto list = it.value();
					fileItem->hasDescriptionData	= true;
					int columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"Comment");
					if ( -1 != columnId )
						fileItem->tagsOrComment	= list[columnId].remove("\t");
					
					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"Scaner");
					if ( -1 != columnId )
						fileItem->scanerId	= list[columnId].remove("\t").toUInt();
					
					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"Description");
					if ( -1 != columnId )
						fileItem->description	= list[columnId].remove("\t");

					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"Device");
					if ( -1 != columnId )
						fileItem->deviceId	= list[columnId].remove("\t").toUInt();

					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"Place");
					if ( -1 != columnId )
						fileItem->placeId = list[columnId].remove("\t").toUInt();

					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"People");
					if ( -1 != columnId )
						fileItem->peopleIds = fvaStringToIds(list[columnId]);							
				}
			}
			fvaitem->children.append( fileItem );
		}
	}
}

*/
void FVAViewer::fvaFilterTree(const fvaFilter& filter, fvaItem* fvaitem, const QDateTime& defFilterDataTime)
{
	for (auto idChild = fvaitem->children.begin(); idChild != fvaitem->children.end(); ++idChild)
	{
		// reset previous filtration result
		(*idChild)->isFiltered = true;

		// 1. filtration by time
		if (defFilterDataTime != filter.dateFrom
			&& defFilterDataTime != filter.dateTo)
		{
			if ((*idChild)->type == FVA_FS_TYPE_DIR)
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
		if ((*idChild)->type != FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.deviceIds.empty())
		{
			if ((*idChild)->pFvaFile)
				(*idChild)->isFiltered = filter.doesIDMatchToFilter((*idChild)->pFvaFile->deviceId, filter.deviceIds);
			else
				(*idChild)->isFiltered = false/*filter.doesIDMatchToFilter(fvaitem->deviceId,filter.deviceIds)*/;
		}
		// 3. filtration by event id
		if ((*idChild)->type == FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.eventIds.empty())
		{
			if ((*idChild)->pFvaFolder)
				(*idChild)->isFiltered = filter.doesIDMatchToFilter((*idChild)->pFvaFolder->eventId, filter.eventIds);
			else if (fvaitem->type == FVA_FS_TYPE_DIR && fvaitem->pFvaFolder)
			{
				(*idChild)->isFiltered = filter.doesIDMatchToFilter(fvaitem->pFvaFolder->eventId, filter.eventIds);
			}
		}

		// 4. filtration by place ids
		if ((*idChild)->type != FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.placeIds.empty())
		{
			if ((*idChild)->pFvaFile)
				(*idChild)->isFiltered = filter.doesIDMatchToFilter((*idChild)->pFvaFile->placeId, filter.placeIds);
			else
				(*idChild)->isFiltered = false/*filter.doesIDMatchToFilter(fvaitem->placeId,filter.placeIds)*/;
		}
		// 5. filtration by people ids
		if ((*idChild)->type != FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.peopleIds.empty())
		{
			if ((*idChild)->pFvaFile)
				(*idChild)->isFiltered = filter.doIDsMatchToFilter((*idChild)->pFvaFile->peopleIds, filter.peopleIds);
			else
				(*idChild)->isFiltered = false/*filter.doIDsMatchToFilter(fvaitem->peopleIds,filter.peopleIds)*/;
		}
		// 6. filtration by event, desciption or comment
		if ((*idChild)->isFiltered && !filter.text.isEmpty() && ((*idChild)->pFvaFile || (*idChild)->pFvaFolder))
		{
			if ((*idChild)->type == FVA_FS_TYPE_DIR && (*idChild)->pFvaFolder)
			{
				(*idChild)->isFiltered = ((*idChild)->pFvaFolder->tags == filter.text);
				// if (!(*idChild)->isFiltered)
				// (*idChild)->isFiltered = ((*idChild)->tagsOrComment == filter.text);
			}
		}

		// 7. filtration by event reason people ids
		if ((*idChild)->type == FVA_FS_TYPE_DIR && (*idChild)->isFiltered && !filter.eventReasonPeopleIds.empty())
		{
			if ((*idChild)->pFvaFolder)
				(*idChild)->isFiltered = filter.doIDsMatchToFilter((*idChild)->pFvaFolder->eventReasonPeopleIds, filter.eventReasonPeopleIds);
			else
				(*idChild)->isFiltered = false/*filter.doIDsMatchToFilter(fvaitem->eventReasonPeopleIds,filter.eventReasonPeopleIds)*/;
		}

		/*if ((*idChild)->isFiltered)
		qDebug() << "filtered name = " << (*idChild)->fsFullPath << " hasDescriptionData=" << (*idChild)->hasDescriptionData;
		*/
		fvaFilterTree(filter, *idChild, defFilterDataTime);

		// TODO make dir filtered if any child filtered and wiseversa
		if ((*idChild)->isFiltered && !fvaitem->isFiltered)
			fvaitem->isFiltered = true;
	}
}
