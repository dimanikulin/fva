#include "fvadescriptioneditor.h"
#include "FVADescriptionFile.h"

#include "../lib/qexifimageheader.h"

#include <QFileInfo>
#include <QListWidgetItem>

FVADescriptionEditor::FVADescriptionEditor(
						const QStringList&					titles, 
						const QMap< QString, QStringList >&	decsItems, 
						const QVariantMap&					dictionaries,
						const QString&						file,
						QWidget *							parent)
	: QMainWindow	(parent),
	m_titles		(titles), 
	m_decsItems		(decsItems), 
	m_dictionaries	(dictionaries),
	m_file			(file)
{
	ui.setupUi(this);


	QExifImageHeader img( file );

	ui.lbFoto->setBackgroundRole(QPalette::Base);
	ui.lbFoto->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	QImage image = img.thumbnail();
	if ( !image.isNull() )
		ui.lbFoto->setPixmap(QPixmap::fromImage( image ));
	else
	{
		image = QImage( file );
		if ( image.isNull() )
		{
			return;
		}

		ui.lbFoto->setScaledContents(true);
		ui.lbFoto->setPixmap(QPixmap::fromImage( image ));
	}
	QFileInfo info(file);
	auto it = m_decsItems.find( info.fileName().toUpper() );
	if ( it == decsItems.end() )
	{
		//
		return;
	}

	QStringList list = it.value(); 
	ui.lbName->setText( info.fileName() );

	int columnId = FVADescriptionFile::getColumnIdByName(titles,"oldName");
	if ( -1 == columnId )
		return;
	ui.editOldName->setText(list[columnId].remove("\t"));

	columnId = FVADescriptionFile::getColumnIdByName(titles,"Description");
	if ( -1 == columnId )
		return;
	ui.editDescription->setText(list[columnId].remove("\t"));

	columnId = FVADescriptionFile::getColumnIdByName(titles,"Comment");
	if ( -1 == columnId )
		return;
	ui.editComment->setText(list[columnId].remove("\t"));

	QVariantList vlist;
#define FILL_COMB_FROM_DICT(dict,combo) \
	vlist = dictionaries[dict].toList();\
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )\
		combo->addItem ( i->toMap()["name"].toString(), i->toMap()["ID"].toString() );
	FILL_COMB_FROM_DICT("places",ui.cmbBoxPlace);
	FILL_COMB_FROM_DICT("people",ui.cmbBoxWho);
	FILL_COMB_FROM_DICT("devices",ui.cmbBoxDevice);
	FILL_COMB_FROM_DICT("scaners",ui.cmbBoxScaner)

	QString selID;
	int index;
#define SET_SELECTED_IN_COMBO(col,combo) \
	columnId = FVADescriptionFile::getColumnIdByName(titles,col);\
	if ( -1 == columnId )\
		return;\
	selID = list[columnId].remove("\t");\
	index = combo->findData( selID );\
	if ( index != -1 )\
		combo->setCurrentIndex(index);
	SET_SELECTED_IN_COMBO("place",ui.cmbBoxPlace)
	SET_SELECTED_IN_COMBO("device",ui.cmbBoxDevice)
	SET_SELECTED_IN_COMBO("WhoTook",ui.cmbBoxWho)
	SET_SELECTED_IN_COMBO("scaner",ui.cmbBoxScaner)

	columnId = FVADescriptionFile::getColumnIdByName(titles,"people");
	if ( -1 == columnId )
		return;
	QStringList peopleList = list[columnId].split(',');
	for ( auto iter = peopleList.begin(); iter != peopleList.end(); ++iter )
	{
		vlist = dictionaries["people"].toList();
		for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
		{
			if ( i->toMap()["ID"].toString() == *iter )
			{
				QListWidgetItem *itm = new QListWidgetItem;
				itm->setText( i->toMap()["name"].toString() );
				itm->setData( 1, *iter );
				ui.lstPeople->addItem(itm);
			}
		}
	}

	ui.dateTimeEdit->setVisible( false );
}

FVADescriptionEditor::~FVADescriptionEditor()
{

}
