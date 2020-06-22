#include "fvaviewer.h"
#include "QPictureLabel.h"
#include <QString>
#include <QtGui/QListWidget>
#include <QtUiTools/QtUiTools>
#include "fvacommonlib.h"


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

	QDir dir ( rootDir );
	if (!dir.exists( rootDir ))
		return;
	
	m_rootItem.reset (new fvaItem); 
	populateFVATree( rootDir, m_rootItem.get() );
	populateGUITree( m_rootItem.get(), nullptr );

	m_ui->treeWidget->setMaximumWidth(200);

	m_ui->imageLbl->setBackgroundRole(QPalette::Base);
	m_ui->contentArea->setBackgroundRole(QPalette::Dark);

	connect(m_ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* ,int)),this,SLOT(showItem(QTreeWidgetItem*)));
	connect(m_ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem* ,int)),this,SLOT(editFileItem(QTreeWidgetItem*)));
	
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));

	m_ui->dockWidget->hide();
	m_ui->dockWidget->setFloating(true);
	QTabWidget* tabs = new QTabWidget(m_ui->dockWidget);
	tabs->setFixedSize(400, 300);
	m_ui->dockWidget->setFixedSize(400, 300);
	m_uiFilters	= new Ui_Filters;
	QDialog *tabFilters = new QDialog ();
	m_uiFilters->setupUi(tabFilters);
  	tabs->addTab(tabFilters,tr("Фильтрация"));  
	tabs->addTab(new QWidget(),tr("Свойства элемента"));

	connect(m_uiFilters->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

	m_defFilterDataTime	= QDateTime::currentDateTime();	
	m_uiFilters->dateTimeFrom->setDateTime ( m_defFilterDataTime );
	m_uiFilters->dateTimeTo->setDateTime ( m_defFilterDataTime );

	QString		error;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, m_dictionaries, error );
	if ( FVA_NO_ERROR != res )
	{
		return;
	}
	QVariantList vlist;
	FILL_COMB_FROM_DICT("places", m_uiFilters->cbPlace);
	FILL_COMB_FROM_DICT("people", m_uiFilters->cbPeople);
	FILL_COMB_FROM_DICT("devices",m_uiFilters->cbDevice);
}

FVAViewer::~FVAViewer()
{
	delete m_ui;
	delete m_uiFilters;
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
	if(	FVA_FILE_TYPE_VIDEO == fvaConvertFileExt2FileType ( suffix )
		|| FVA_FILE_TYPE_AUDIO == fvaConvertFileExt2FileType ( suffix ))
	{
		QProcess myProcess(this);    
		myProcess.setProcessChannelMode(QProcess::MergedChannels);
		QStringList params;
		params.append(fvaitem->fsFullPath);
		QString path = QCoreApplication::applicationDirPath() + "/#BIN#/FVAPlayer.exe";
		myProcess.start(path, params);
		myProcess.waitForFinished( -1 );
	} 
	else if (FVA_FILE_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ))
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

void FVAViewer::filterClicked(  )
{
	m_filter.dateFrom	= m_uiFilters->dateTimeFrom->dateTime();
	m_filter.dateTo		= m_uiFilters->dateTimeTo->dateTime();
	
	m_filter.deviceIds.clear();
	m_filter.peopleIds.clear();
	m_filter.placeIds.clear();
	
	int index = m_uiFilters->cbDevice->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = m_uiFilters->cbDevice->itemData( index ).toInt();
		m_filter.deviceIds.push_back(ID);
	}

	index = m_uiFilters->cbPeople->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = m_uiFilters->cbPeople->itemData( index ).toInt();
		m_filter.peopleIds.push_back(ID);
	}

	index = m_uiFilters->cbPlace->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = m_uiFilters->cbPlace->itemData( index ).toInt();
		m_filter.placeIds.push_back(ID);
	}
	m_filter.eventOrDesc = m_uiFilters->editEvent->text();

	filterFVATree( m_filter, m_rootItem.get() );
	m_ui->treeWidget->clear();
	populateGUITree( m_rootItem.get(), nullptr );
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
			if ( (*idChild)->isFolder )
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
			(*idChild)->isFiltered = ((*idChild)->deviceId == filter.deviceIds[0]);

		// 3. filtration by scaner id
		// TODO	

		// 4. filtration by place ids
		if ((*idChild)->isFiltered && !filter.placeIds.empty() && (*idChild)->hasDescriptionData)
			(*idChild)->isFiltered = ((*idChild)->placeIds == filter.placeIds);

		// 5. filtration by people ids
		if ((*idChild)->isFiltered && !filter.peopleIds.empty() && (*idChild)->hasDescriptionData)
			(*idChild)->isFiltered = ((*idChild)->peopleIds == filter.peopleIds);

		// 7. filtration by event, desciption or comment
		if ((*idChild)->isFiltered && !filter.eventOrDesc.isEmpty() && (*idChild)->hasDescriptionData)
		{
			if ( (*idChild)->isFolder )
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

void FVAViewer::populateFVATree( const QString& folder, fvaItem* fvaitem )
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
			fvaItem* dirItem	= new fvaItem;
			dirItem->isFolder	= true;
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
				dirItem->placeIds.append(	result["place"]			.toUInt());
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
			if ( FVA_NO_ERROR != res )
			{
				// it is not a big problem
			}

			fvaitem->children.append( dirItem );
			populateFVATree( info.absoluteFilePath(), dirItem );
		}
		else
		{
			if(	!fvaIsFVAFile ( info.suffix().toUpper() ) )
				continue;
			fvaItem* fileItem		= new fvaItem;
			fileItem->isFolder		= false;
			fileItem->fsFullPath		= info.absoluteFilePath();
			fileItem->type			= fvaConvertFileExt2FileType(info.suffix().toUpper());
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
						fileItem->placeIds.append(list[columnId].remove("\t").toUInt());

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

void FVAViewer::populateGUITree( const fvaItem* fvaitem, QTreeWidgetItem* item )
{
	for ( auto idChild = fvaitem->children.begin(); idChild != fvaitem->children.end(); ++idChild)
	{
		if ( !(*idChild)->isFiltered )
			continue;
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText		( 0, (*idChild)->getGuiName() );
		treeWidgetItem->setData( 1, 1, (int)(*idChild) );
		if ((*idChild)->isFolder)
			treeWidgetItem->setIcon(0, m_folderIcon);
		else
		{
			switch ((*idChild)->type)
			{
				case FVA_FILE_TYPE_IMG: 
					treeWidgetItem->setIcon(0, m_photoIcon);
				break;
				case FVA_FILE_TYPE_VIDEO: 
					treeWidgetItem->setIcon(0, m_videoIcon);
				break;
				case FVA_FILE_TYPE_AUDIO: 
					treeWidgetItem->setIcon(0, m_audioIcon);
				break;
			}
		}
		if ( item )
			item->addChild( treeWidgetItem );
		else
			m_ui->treeWidget->addTopLevelItem ( treeWidgetItem );

		populateGUITree( *idChild, treeWidgetItem );
	}							
} 
