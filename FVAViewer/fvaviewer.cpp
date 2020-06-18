#include "fvaviewer.h"
#include "QPictureLabel.h"
#include <QString>
#include "fvacommonlib.h"

#include <Phonon/VideoPlayer>

FVAViewer::FVAViewer(const QString& rootDir, const QString& dictPath, QWidget *parent, Qt::WFlags flags)
	:QDialog			(parent),
	m_ui				(new Ui::FVAViewerClass),
	m_dictionaryPath	(dictPath)
{
	m_ui->setupUi(this);

	QDir dir ( rootDir );
	if (!dir.exists( rootDir ))
		return;

	QString		error;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, m_dictionaries, error );
	if ( FVA_NO_ERROR != res )
	{
		return;
	}
	QVariantList vlist;
	FILL_COMB_FROM_DICT("places", m_ui->cbPlace);
	FILL_COMB_FROM_DICT("people", m_ui->cbPeople);
	FILL_COMB_FROM_DICT("devices",m_ui->cbDevice);

	m_rootItem.reset (new fvaItem); 
	populateFVATree( rootDir, m_rootItem.get() );
	populateGUITree( m_rootItem.get(), nullptr );

	m_ui->treeWidget->setMaximumWidth(300);
	m_ui->treeWidget->setMinimumWidth(50);

	m_ui->imageLbl->setBackgroundRole(QPalette::Base);
	m_ui->contentArea->setBackgroundRole(QPalette::Dark);

	connect(m_ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* ,int)),this,SLOT(showItem(QTreeWidgetItem*)));
	connect(m_ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem* ,int)),this,SLOT(editFileItem(QTreeWidgetItem*)));
	connect(m_ui->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

	m_defFilterDataTime	= QDateTime::currentDateTime();	
	m_ui->dateTimeFrom->setDateTime ( m_defFilterDataTime );
	m_ui->dateTimeTo->setDateTime ( m_defFilterDataTime );
}

FVAViewer::~FVAViewer()
{
	delete m_ui;
}

void FVAViewer::showItem( QTreeWidgetItem* item )
{
	QFileInfo info(item->data( 1, 1 ).toString());
	QString suffix = info.suffix().toUpper();
	if(	FVA_FILE_TYPE_VIDEO == fvaConvertFileExt2FileType ( suffix ))
	{
		QStringList libs = QCoreApplication::libraryPaths();
		Phonon::VideoPlayer *player = new Phonon::VideoPlayer(Phonon::VideoCategory, m_ui->contentArea->widget());
		connect(player, SIGNAL(finished()), player, SLOT(deleteLater()));
		QUrl url(item->data( 1, 1 ).toString());
		player->play(url);

		/*QMediaPlayer* media = new QMediaPlayer(this);
		QVideoWidget* video = new QVideoWidget(this);
		media->setVideoOutput(video);
		QUrl url(item->data( 1, 1 ).toString());
		media->setMedia(url);
		media->play();
		m_ui->contentArea->setWidget(video);*/
	} 
	else if (FVA_FILE_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ))
	{
		fvaShowImage( item->data( 1, 1 ).toString(), m_ui->imageLbl );
	}
}

void FVAViewer::editFileItem( QTreeWidgetItem* item )
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(m_dictionaryPath);
	params.append(item->data( 1, 1 ).toString());
	myProcess.start("#BIN#/FVADescriptionEditor.exe", params);

	if ( !myProcess.waitForFinished( -1 ) )
	{
	}
}

void FVAViewer::filterClicked(  )
{
	m_filter.dateFrom	= m_ui->dateTimeFrom->dateTime();
	m_filter.dateTo		= m_ui->dateTimeTo->dateTime();

	m_filter.deviceIds.clear();
	m_filter.peopleIds.clear();
	m_filter.placeIds.clear();
	
	int index = m_ui->cbDevice->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = m_ui->cbDevice->itemData( index ).toInt();
		m_filter.deviceIds.push_back(ID);
	}

	index = m_ui->cbPeople->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = m_ui->cbPeople->itemData( index ).toInt();
		m_filter.peopleIds.push_back(ID);
	}

	index = m_ui->cbPlace->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = m_ui->cbPlace->itemData( index ).toInt();
		m_filter.placeIds.push_back(ID);
	}
	m_filter.eventOrDesc = m_ui->editEvent->text();

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
		treeWidgetItem->setToolTip	( 0, (*idChild)->getGuiFullName(m_dictionaries) );
		treeWidgetItem->setData( 1, 1, (*idChild)->fsFullPath );

		if ( item )
			item->addChild( treeWidgetItem );
		else
			m_ui->treeWidget->addTopLevelItem ( treeWidgetItem );

		populateGUITree( *idChild, treeWidgetItem );
	}							
} 

