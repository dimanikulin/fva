#include "fvaviewer.h"


FVAViewer::FVAViewer(QWidget *parent, Qt::WFlags flags)
	:QDialog(parent),
	ui(new Ui::FVAViewerClass)
{
	ui->setupUi(this);

	populateTree( QApplication::applicationDirPath(), 0 );
	ui->treeWidget->setMaximumWidth(300);
	ui->treeWidget->setMinimumWidth(50);

	ui->imageLbl->setBackgroundRole(QPalette::Base);
	ui->imageLbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	ui->imageLbl->setScaledContents(true);
	
	ui->contentArea->setWidgetResizable(true);
	ui->contentArea->setBackgroundRole(QPalette::Dark);

	connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem* ,int)),this,SLOT(showItem(QTreeWidgetItem*)));
}

FVAViewer::~FVAViewer()
{
	delete ui;
}

void FVAViewer::showItem( QTreeWidgetItem* item )
{
	QString path = item->data( 1, 1 ).toString();
	if ( path.isEmpty() )
		return;
	QImage image( path );
	if ( image.isNull() )
	{
		qDebug() << "WARNING!, Cannot load file " << path << " to show.";
		return;
	}
	ui->imageLbl->setPixmap(QPixmap::fromImage(image));
}
void FVAViewer::populateTree( const QString& folder, QTreeWidgetItem* item )
{
	// TODO skip #DVD and #PREPARATION# folders
	QDir dir( folder );

	if (dir.exists( folder ))
	{
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
		{
			QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;			
			if ( info.isDir() )
			{
				treeWidgetItem->setText ( 0, info.fileName() );

				if ( item )
					item->addChild( treeWidgetItem );
				else
					ui->treeWidget->addTopLevelItem ( treeWidgetItem );

				populateTree ( info.absoluteFilePath(), treeWidgetItem );
			}
			else if ( item )
			{
				treeWidgetItem->setText ( 0, info.baseName() );
				treeWidgetItem->setData( 1, 1, info.absoluteFilePath() );
				item->addChild( treeWidgetItem );
			}
		}
	}
} 

