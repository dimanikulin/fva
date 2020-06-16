#include "fvadescriptioneditor.h"
#include "FVADescriptionFile.h"
#include "fvapeoplelistdlg.h"

#include <QFileInfo>
#include <QListWidgetItem>
#include <QDir>
#include <QDateTime>
#include <QProcess>

void FVADescriptionEditor::updateDictionaryGUI()
{
	QVariantList vlist;

	FILL_COMB_FROM_DICT("places",ui.cmbBoxPlace);
	FILL_COMB_FROM_DICT("people",ui.cmbBoxWho);
	FILL_COMB_FROM_DICT("devices",ui.cmbBoxDevice);
	FILL_COMB_FROM_DICT("scaners",ui.cmbBoxScaner);
}
FVADescriptionEditor::FVADescriptionEditor(
						const QStringList&					titles, 
							QMap< QString, QStringList >&	decsItems, 
						const QVariantMap&					dictionaries,
						int									indexOfFile,
						const QStringList&					files,
						const QString&						dictPath,
						QWidget *							parent)
	: QMainWindow	(parent),
	m_titles		(titles), 
	m_decsItems		(decsItems), 
	m_dictionaries	(dictionaries),
	m_indexOfFile	(indexOfFile),
	m_files			(files),
	m_dictPath		(dictPath)

{
	ui.setupUi(this);

	updateDictionaryGUI();
	
	connect (ui.btnAdd,SIGNAL(clicked()),this,SLOT(OnAddBtnPressed()) );
	connect (ui.btnDel,SIGNAL(clicked()),this,SLOT(OnRemoveBtnPressed()) );
	connect (ui.btnNext,SIGNAL(clicked()),this,SLOT(OnNextBtnPressed()) );
	connect (ui.btnPrev,SIGNAL(clicked()),this,SLOT(OnPrevBtnPressed()) );
	connect (ui.btnSave,SIGNAL(clicked()),this,SLOT(OnSaveBtnPressed()) );
	connect (ui.btnDict,SIGNAL(clicked()),this,SLOT(OnChangeDictPressed()));

	updateGuiForFile( m_files[m_indexOfFile] );
}

FVADescriptionEditor::~FVADescriptionEditor()
{

}

void FVADescriptionEditor::OnChangeDictPressed()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(m_dictPath);
	myProcess.start("FVADictionaryEditor.exe", params);

	if ( !myProcess.waitForFinished( -1 ) )
	{
	}
	QString		error;
	FVA_ERROR_CODE res = fvaLoadDictionary( m_dictPath, m_dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return ;

	updateDictionaryGUI();
}


void FVADescriptionEditor::updateGuiForFile( const QString& path )
{
	QFileInfo info( path );
	auto it = m_decsItems.find( info.fileName().toUpper() );
	if ( it == m_decsItems.end() )
		return;

	ui.lbName->setText( info.fileName() );

	fvaShowImage( path, ui.lbFoto );

	QStringList list = it.value(); 

	int columnId = FVADescriptionFile::getColumnIdByName(m_titles,"oldName");
	if ( -1 == columnId )
		return;
	ui.editOldName->setText(list[columnId].remove("\t"));

	columnId = FVADescriptionFile::getColumnIdByName(m_titles,"Description");
	if ( -1 == columnId )
		return;
	ui.editDescription->setText(list[columnId].remove("\t"));

	columnId = FVADescriptionFile::getColumnIdByName(m_titles,"Comment");
	if ( -1 == columnId )
		return;
	ui.editComment->setText(list[columnId].remove("\t"));

	QString selID;
	int index;
#define SET_SELECTED_IN_COMBO(col,combo) \
	columnId = FVADescriptionFile::getColumnIdByName(m_titles,col);\
	if ( -1 == columnId )\
		return;\
	selID = list[columnId].remove("\t");\
	index = combo->findData( selID );\
	if ( index != -1 )\
		combo->setCurrentIndex(index);\
	else \
		combo->setCurrentIndex(0);\

	SET_SELECTED_IN_COMBO("place",ui.cmbBoxPlace)
	SET_SELECTED_IN_COMBO("device",ui.cmbBoxDevice)
	SET_SELECTED_IN_COMBO("WhoTook",ui.cmbBoxWho)
	SET_SELECTED_IN_COMBO("scaner",ui.cmbBoxScaner)

	columnId = FVADescriptionFile::getColumnIdByName(m_titles,"people");
	if ( -1 == columnId )
		return;
	QStringList peopleList = list[columnId].split(',');
	QVariantList vlist;
	ui.lstPeople->clear();
	for ( auto iter = peopleList.begin(); iter != peopleList.end(); ++iter )
	{
		vlist = m_dictionaries["people"].toList();
		for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
		{
			if ( i->toMap()["ID"].toString() == *iter )
			{
				QListWidgetItem *itm = new QListWidgetItem;
				itm->setText( i->toMap()["name"].toString() );
				itm->setData( 1, i->toMap()["ID"].toString() );
				ui.lstPeople->addItem(itm);
			}
		}
	}
}

void FVADescriptionEditor::OnAddBtnPressed()
{
	FVAPeopleListDlg dlg( m_dictionaries["people"].toList());
	int res = dlg.exec( );
	if ( 1 == res )	
	{
		Ui::FVAPeopleListDlg ui_ = dlg.getUI();
		QList<QListWidgetItem*> selList = ui_.listWidget->selectedItems();
		if ( selList.size() )
			ui.lstPeople->addItem( selList.at(0)->clone() );
	}
}
void FVADescriptionEditor::OnRemoveBtnPressed()
{
	QList<QListWidgetItem*> selList = ui.lstPeople->selectedItems();
	if ( selList.size() )
		delete selList.at(0);
}
void FVADescriptionEditor::OnNextBtnPressed()
{
	saveCurrentDescription();
	if ( m_files.size() < m_indexOfFile + 1 )
		return;
	m_indexOfFile++;
	updateGuiForFile(m_files[m_indexOfFile]);
}
void FVADescriptionEditor::OnPrevBtnPressed()
{
	saveCurrentDescription();
	if ( 0 > m_indexOfFile - 1 )
		return;
	m_indexOfFile--;
	updateGuiForFile(m_files[m_indexOfFile]);
}
void FVADescriptionEditor::saveCurrentDescription()
{
	QFileInfo info( m_files[m_indexOfFile] );
	auto it = m_decsItems.find( info.fileName().toUpper() );
	if ( it == m_decsItems.end() )
		return;

	QStringList& list = it.value();
	list[FVADescriptionFile::getColumnIdByName(m_titles,"oldName")]		= ui.editOldName->text().remove("\t");
	list[FVADescriptionFile::getColumnIdByName(m_titles,"Description")] = ui.editDescription->text().remove("\t");
	list[FVADescriptionFile::getColumnIdByName(m_titles,"Comment")]		= ui.editComment->text().remove("\t");
	int index = -1;
#define GET_SELECTED_FROM_COMBO(col,combo) \
	index = combo->currentIndex(); \
	if ( 1 <= index ) \
	{\
		QString ID = combo->itemData( index ).toString();\
		list[FVADescriptionFile::getColumnIdByName(m_titles,col)] = ID;\
	}
	GET_SELECTED_FROM_COMBO("place",ui.cmbBoxPlace)
	GET_SELECTED_FROM_COMBO("device",ui.cmbBoxDevice)
	GET_SELECTED_FROM_COMBO("WhoTook",ui.cmbBoxWho)
	GET_SELECTED_FROM_COMBO("scaner",ui.cmbBoxScaner)

	QString peopleIDs;
	for ( auto iP = 0; iP < ui.lstPeople->count(); ++iP )
	{
		if ( peopleIDs.isEmpty() )
			peopleIDs = ui.lstPeople->item(iP)->data(1).toString();
		else
			peopleIDs += "," + ui.lstPeople->item(iP)->data(1).toString();
	}

	list[FVADescriptionFile::getColumnIdByName(m_titles,"people")] = peopleIDs;

	QDir dir(m_files[m_indexOfFile]);
	dir.cdUp();
	if ( dir.exists( dir.absolutePath() + "/" + FVA_DESCRIPTION_FILE_NAME ) )
	{
		QString newName = dir.absolutePath()
			+ "/" + FVA_DESCRIPTION_FILE_NAME 
			+ "_" + QDateTime::currentDateTime().toString( "yyyy-MM-dd-hh-mm-ss").toAscii().data();
		QString oldName = dir.absolutePath()+ "/" + FVA_DESCRIPTION_FILE_NAME;
		if ( !dir.rename( oldName, newName ))
		{
			return;
		}
	}
	
	FVADescriptionFile desc;
	FVA_ERROR_CODE res = desc.save( dir.absolutePath() + "/" + FVA_DESCRIPTION_FILE_NAME, 
									m_titles, 
									m_decsItems );
	//if ( FVA_NO_ERROR != res )
		// return res;
}
void FVADescriptionEditor::OnSaveBtnPressed()
{
	saveCurrentDescription();
}