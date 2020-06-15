#include "fvaviewer.h"
#include "QPictureLabel.h"
#include <QString>
#include "fvacommonlib.h"

FVAViewer::FVAViewer(const QString& rootDir, const QString& dictPath, QWidget *parent, Qt::WFlags flags)
	:QDialog(parent),
	ui(new Ui::FVAViewerClass)
{
	ui->setupUi(this);

	QDir dir ( rootDir );
	if (!dir.exists( rootDir ))
		return;

	rootItem.reset (new fvaItem); 
	populateFVATree( rootDir, rootItem.get() );
	populateGUITree( rootItem.get(), nullptr );

	ui->treeWidget->setMaximumWidth(300);
	ui->treeWidget->setMinimumWidth(50);

	ui->imageLbl->setBackgroundRole(QPalette::Base);
	ui->contentArea->setBackgroundRole(QPalette::Dark);

	connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* ,int)),this,SLOT(showItem(QTreeWidgetItem*)));
	connect(ui->btnFilter,SIGNAL(clicked()),this,SLOT(filterClicked()));

	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
	{
	
	}
	QVariantList vlist;
	FILL_COMB_FROM_DICT("places",ui->cbPlace);
	FILL_COMB_FROM_DICT("people",ui->cbPeople);
	FILL_COMB_FROM_DICT("devices",ui->cbDevice);
}

FVAViewer::~FVAViewer()
{
	delete ui;
}

void FVAViewer::showItem( QTreeWidgetItem* item )
{
	fvaShowImage( item->data( 1, 1 ).toString(), ui->imageLbl );
}
void FVAViewer::filterClicked(  )
{
	filter.dateFrom	= ui->dateTimeFrom->dateTime();
	filter.dateTo	= ui->dateTimeTo->dateTime();

	filter.deviceIds.clear();
	filter.peopleIds.clear();
	filter.placeIds.clear();
	
	int index = ui->cbDevice->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = ui->cbDevice->itemData( index ).toInt();
		filter.deviceIds.push_back(ID);
	}

	index = ui->cbPeople->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = ui->cbPeople->itemData( index ).toInt();
		filter.peopleIds.push_back(ID);
	}

	index = ui->cbPlace->currentIndex();
	if ( 1 <= index ) 
	{
		int ID = ui->cbPlace->itemData( index ).toInt();
		filter.placeIds.push_back(ID);
	}
	filter.eventOrDesc = ui->editEvent->text();

	filterFVATree( filter, rootItem.get() );
	ui->treeWidget->clear();
	populateGUITree( rootItem.get(), nullptr );
}

void FVAViewer::filterFVATree( const fvaFilter& filter, fvaItem* fvaitem )
{
	for ( auto idChild = fvaitem->children.begin(); idChild != fvaitem->children.end(); ++idChild)
	{
		// filtration by time
		if ( (*idChild)->isFolder )
		{
			(*idChild)->isFiltered = ((*idChild)->dateFrom >= filter.dateFrom)
									&& ((*idChild)->dateTo <= filter.dateTo);
		}
		else
		{
			(*idChild)->isFiltered = ((*idChild)->dateFrom >= filter.dateFrom) 
									&& ((*idChild)->dateFrom <= filter.dateTo);
		}
		// filter by device is
		// filter.deviceIds
		filterFVATree( filter, *idChild );
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
			dirItem->fsName		= info.fileName();
			dirItem->isFolder	= true;

			if ( FVA_NO_ERROR != fvaParseDirName(info.fileName(), dirItem->dateFrom, dirItem->dateTo))
			{
				qDebug() << "ERROR!, incorrect folder name " << info.fileName();
				delete dirItem;
				continue;
			}

			fvaitem->children.append( dirItem );
			populateFVATree( info.absoluteFilePath(), dirItem );
		}
		else
		{
			// now only images are supported
			if(	FVA_FILE_TYPE_IMG != fvaConvertFileExt2FileType ( info.suffix().toUpper() ) )
				continue;
			fvaItem* fileItem		= new fvaItem;
			fileItem->fsName			= info.baseName();
			fileItem->isFolder		= false;
			fileItem->fsFullPath		= info.absoluteFilePath();
			if ( FVA_NO_ERROR != fvaParseFileName(info.baseName(), fileItem->dateFrom))
			{
				qDebug() << "ERROR!, incorrect file name " << info.fileName();
				delete fileItem;
				continue;
			}
			fvaitem->children.append( fileItem );
		}
	}
}

void FVAViewer::populateGUITree( const fvaItem* fvaitem, QTreeWidgetItem* item )
{
	if ( !fvaitem->isFiltered )
		return;
	for ( auto idChild = fvaitem->children.begin(); idChild != fvaitem->children.end(); ++idChild)
	{
		if ( !(*idChild)->isFiltered )
			continue;
		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
		treeWidgetItem->setText ( 0, (*idChild)->fsName );
		if (!(*idChild)->isFolder)
		{
			treeWidgetItem->setData( 1, 1, (*idChild)->fsFullPath );
			treeWidgetItem->setToolTip(0, "SOME VERY LONG!!!! HINT!!!!!!!!!!!!!!!!!!!!!!!!!!" );
		}
		if ( item )
			item->addChild( treeWidgetItem );
		else
			ui->treeWidget->addTopLevelItem ( treeWidgetItem );

		populateGUITree( *idChild, treeWidgetItem );
	}							
} 

