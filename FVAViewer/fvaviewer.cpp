#include "fvaviewer.h"
#include "QPictureLabel.h"
#include <QString>
#include <QtGui/QListWidget>
#include <QtGui/QProgressDialog>
#include <QtUiTools/QtUiTools>
#include "fvacommonlib.h"

void FVAViewer::showProgress(const QString& rootDir)
{
	QDir dir ( rootDir );
	if (!dir.exists( rootDir ))
		return;

	int number = 0;
	evaluateFSTree (rootDir, number);
	QProgressDialog progress("Loading...", "Abort", 0, number * 2, this);
	progress.setWindowModality(Qt::ApplicationModal);
	m_rootItem.reset (new fvaItem); 
	int itemNumber = 0;
	populateFVATree( rootDir, m_rootItem.get(), itemNumber, &progress );
	populateGUITree( m_rootItem.get(), nullptr, itemNumber, &progress );
    progress.setValue(number *2);
}

void FVAViewer::buildFilterTree(QTreeWidget* pTreeWidget, bool devicesNeed )
{
	connect(pTreeWidget, 
			SIGNAL(itemChanged(QTreeWidgetItem*, int)), 
			this,
			SLOT(updateChecks(QTreeWidgetItem*, int)));

	QIcon	personIcon	= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/person.png");
	QIcon	peopleIcon	= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/people.png");

	QVariantList	vlist	= m_dictionaries["relationTypes"].toList();
	QVariantList	vlist1	= m_dictionaries["relations"].toList();
	QVariantList	people	= m_dictionaries["people"].toList();
	QVariantList	devices	= m_dictionaries["devices"].toList();
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
	{
		int ID = i->toMap()["ID"].toInt();
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText		( 0, i->toMap()["name"].toString() );
		treeWidgetItem->setIcon		(0, peopleIcon);
		treeWidgetItem->setFlags		(treeWidgetItem->flags() | Qt::ItemIsUserCheckable);
		treeWidgetItem->setCheckState(0,Qt::Unchecked);
		pTreeWidget->addTopLevelItem ( treeWidgetItem );
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
			treeWidgetItem->addChild		( childWidgetItem );
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
					continue;
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
					deviceWidgetItem->setIcon(0, personIcon);
					deviceWidgetItem->setFlags(deviceWidgetItem->flags() | Qt::ItemIsUserCheckable);
					deviceWidgetItem->setCheckState(0,Qt::Unchecked);
					personWidgetItem->addChild ( deviceWidgetItem );
				}
				if (!devicesNeed)
					childWidgetItem->addChild ( personWidgetItem );
				else
				{
					if (personWidgetItem->childCount())
						childWidgetItem->addChild ( personWidgetItem );
					else
						delete personWidgetItem;
				}
			}
		}
	}
}

void FVAViewer::prepareFilters()
{
	m_ui->dockWidget->hide();
	m_ui->dockWidget->setFloating(true);
	QTabWidget* tabs = new QTabWidget(m_ui->dockWidget);
	tabs->setFixedSize(400, 300);
	m_ui->dockWidget->setFixedSize(400, 300);
	
	m_uiPlaceFilter		= new Ui_placeFilter;
	m_uiPeopleFilter	= new Ui_hfilter;

	QDialog *tabCommonFilters	= new QDialog ();
	m_uiFiltersCommon			= new Ui_commonFilter ;
	m_uiFiltersCommon->setupUi(tabCommonFilters);
  	tabs->addTab(tabCommonFilters,tr("Общие фильтры"));
	connect(m_uiFiltersCommon->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));
	m_defFilterDataTime	= QDateTime::currentDateTime();	
	m_uiFiltersCommon->dateTimeFrom->setDateTime ( m_defFilterDataTime );
	m_uiFiltersCommon->dateTimeTo->setDateTime ( m_defFilterDataTime );

	QDialog *tabDeviceFilters	= new QDialog ();
	m_uiDeviceFilter			= new Ui_dfilter;
	m_uiDeviceFilter->setupUi(tabDeviceFilters);
  	tabs->addTab(tabDeviceFilters,tr("Устройства"));
	connect(m_uiDeviceFilter->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

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

	tabs->addTab(new QWidget(),tr("Свойства"));

	QVariantList vlist = m_dictionaries["places"].toList();
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
	{
		QListWidgetItem* item = new QListWidgetItem(i->toMap()["name"].toString(), m_uiPlaceFilter->lstPlace);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
		item->setCheckState(Qt::Unchecked); // AND initialize check state
		item->setData(1, i->toMap()["ID"].toString());
	}
	
	buildFilterTree(m_uiPeopleFilter->treeWidget, false);
	buildFilterTree(m_uiDeviceFilter->treeWidget, true);
}

FVAViewer::FVAViewer(const QString& rootDir, const QString& dictPath, QWidget *parent, Qt::WFlags flags)
	:QDialog 			(parent),
	m_ui				(new Ui::FVAViewerClass),
	m_dictionaryPath	(dictPath)
{
	m_ui->setupUi(this);

	m_videoIcon		= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/video.png");
	m_audioIcon		= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/audio.png");
	m_photoIcon		= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/photo.png");
	m_folderIcon	= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/folder.png");
	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/#BIN#/Icons/main.png");
	setWindowIcon(icon);

	QString		error;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, m_dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return;

	showProgress(rootDir);

	m_ui->treeWidget->setMaximumWidth(200);

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
            updateChecks(item->parent(),-1);
    }
}
FVAViewer::~FVAViewer()
{
	delete	m_ui;
	delete	m_uiFiltersCommon;
	delete	m_uiDeviceFilter;
	delete	m_uiPlaceFilter;
	delete	m_uiPeopleFilter;
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
	setWindowTitle(fvaitem->getGuiName() 
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
		QString path = QCoreApplication::applicationDirPath() + "/#BIN#/FVAPlayer.exe";
		myProcess.start(path, params);
		myProcess.waitForFinished( -1 );
	} 
	else if (FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ))
	{
		fvaShowImage( fvaitem->fsFullPath, m_ui->imageLbl, fvaitem->getGuiName() );
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
	myProcess.start(QCoreApplication::applicationDirPath() + "/#BIN#/FVADescriptionEditor.exe", params);
	myProcess.waitForFinished( -1 );
}

void findCheckedItem(QTreeWidgetItem *item, QVector<unsigned int>& Ids)
{
	if (item->checkState(0) == Qt::CheckState::Checked)
	{
		int ID = item->data(1, 1).toInt();
		if (ID)
			Ids.push_back(ID);
	}

	for (auto id = 0 ; id < item->childCount(); ++id)
		findCheckedItem(item->child(id), Ids);
}

void FVAViewer::filterClicked(  )
{
	m_filter.dateFrom	= m_uiFiltersCommon->dateTimeFrom->dateTime();
	m_filter.dateTo		= m_uiFiltersCommon->dateTimeTo->dateTime();
	
	m_filter.deviceIds.clear();
	m_filter.peopleIds.clear();
	m_filter.placeIds.clear();
	
	for (auto idTop = 0; idTop < m_uiPeopleFilter->treeWidget->topLevelItemCount();++idTop )
		findCheckedItem(m_uiPeopleFilter->treeWidget->topLevelItem(idTop), m_filter.peopleIds);

	for (auto idTop = 0; idTop < m_uiDeviceFilter->treeWidget->topLevelItemCount();++idTop )
		findCheckedItem(m_uiDeviceFilter->treeWidget->topLevelItem(idTop), m_filter.deviceIds);

	for (auto it = 0; it < m_uiPlaceFilter->lstPlace->count();++it)
	{
		if (m_uiPlaceFilter->lstPlace->item( it )->checkState() == Qt::CheckState::Checked )
		{
			int ID = m_uiPlaceFilter->lstPlace->item( it )->data(1).toInt();
			m_filter.placeIds.push_back(ID);
		}
	}

	m_filter.eventOrDesc = m_uiFiltersCommon->editEvent->text();

	filterFVATree( m_filter, m_rootItem.get() );
	m_ui->treeWidget->clear();
	int number = 0;
	populateGUITree( m_rootItem.get(), nullptr, number, nullptr );
}

void FVAViewer::filterFVATree( const fvaFilter& filter, fvaItem* fvaitem )
{
	for ( auto idChild = fvaitem->children.begin(); idChild != fvaitem->children.end(); ++idChild)
	{
		// reset previous filtration result
		(*idChild)->isFiltered = true;

		// 1. filtration by time
		if (m_defFilterDataTime		!= filter.dateFrom 
			&& m_defFilterDataTime	!= filter.dateTo)
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
		if ((*idChild)->isFiltered && !filter.deviceIds.empty() && (*idChild)->hasDescriptionData)
			(*idChild)->isFiltered = filter.isIDMatchesToFilter((*idChild)->deviceId,filter.deviceIds);

		// 3. filtration by scaner id
		// TODO	

		// 4. filtration by place ids
		if ((*idChild)->isFiltered && !filter.placeIds.empty() && (*idChild)->hasDescriptionData)
			(*idChild)->isFiltered = filter.isIDMatchesToFilter((*idChild)->placeId,filter.placeIds);

		// 5. filtration by people ids
		if ((*idChild)->isFiltered && !filter.peopleIds.empty() && (*idChild)->hasDescriptionData)
		{
			bool res = false;
			for (auto it = (*idChild)->peopleIds.begin(); it != (*idChild)->peopleIds.end();++it)
			{
				if (filter.isIDMatchesToFilter(*it,filter.peopleIds))
				{
					res = true;	
					break;
				}
			}
			(*idChild)->isFiltered = res;
		}
		// 6. filtration by event, desciption or comment
		if ((*idChild)->isFiltered && !filter.eventOrDesc.isEmpty() && (*idChild)->hasDescriptionData)
		{
			if ( (*idChild)->type == FVA_FS_TYPE_DIR )
			{
				(*idChild)->isFiltered = ((*idChild)->eventOrDesc == filter.eventOrDesc);
				if (!(*idChild)->isFiltered)
					(*idChild)->isFiltered = ((*idChild)->tagsOrComment == filter.eventOrDesc);
			}
		}
		/*if ((*idChild)->isFiltered)
			qDebug() << "filtered name = " << (*idChild)->fsName << " hasDescriptionData=" << (*idChild)->hasDescriptionData;
		*/
		filterFVATree( filter, *idChild );

		// TODO make dir filtered if any child filtered and wiseversa
	}							
}
void FVAViewer::evaluateFSTree (const QString& folder, int& number)
{
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
			evaluateFSTree( info.absoluteFilePath(), number );
			number++;
		}
		else
		{
			if(	!fvaIsFVAFile ( info.suffix().toUpper() ) )
				continue;
			number++;
		}
	}
}

void FVAViewer::populateFVATree( const QString& folder, fvaItem* fvaitem, int& number, QProgressDialog* progress )
{	
	/*if (progress.wasCanceled())
		return;
	*/
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

			FVA_ERROR_CODE code = fvaGetFolderDescription( info.absoluteFilePath(), result, error );
			if ( FVA_NO_ERROR != code )
			{
				qWarning() << error;
				
				// delete dirItem;
				// continue;
			}
			else
			{
				dirItem->placeId			=	result["place"]			.toUInt();
				dirItem->peopleIds.append(	result["people"]		.toUInt()); // TODO bug with many people
				dirItem->deviceId		=	result["deviceId"]		.toUInt();
				dirItem->eventOrDesc		=	result["event"]			.toString();
				dirItem->tagsOrComment	=	result["tags"]			.toString();
				dirItem->linkedFolder	=	result["linkedFolder"]	.toString();
				dirItem->hasDescriptionData	= true;

				if (!dirItem->deviceId)
					qCritical() << "0 device if for folder " << info.fileName();
			}

			QString descFilePath = info.absoluteFilePath() + "/" + FVA_DESCRIPTION_FILE_NAME;
			FVA_ERROR_CODE res = m_descriptionFile.load( descFilePath,dirItem->descTitles, dirItem->decsItems );
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
			fileItem->type			= FVA_FS_TYPE_DIR;
			fileItem->fsFullPath		= info.absoluteFilePath();
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
						fileItem->eventOrDesc	= list[columnId].remove("\t");

					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"Device");
					if ( -1 != columnId )
						fileItem->deviceId	= list[columnId].remove("\t").toUInt();

					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"Place");
					if ( -1 != columnId )
						fileItem->placeId = list[columnId].remove("\t").toUInt();

					columnId = FVADescriptionFile::getColumnIdByName(fvaitem->descTitles,"People");
					if ( -1 != columnId )
					{
						QStringList people = list[columnId].split(',');
						for ( auto iter = people.begin(); iter != people.end(); ++iter )
							fileItem->peopleIds.append(iter->toUInt());							
					}
				}
			}			
			fvaitem->children.append( fileItem );
		}
	}
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
		treeWidgetItem->setText		( 0, (*idChild)->getGuiName() );
		treeWidgetItem->setData( 1, 1, (int)(*idChild) );
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
