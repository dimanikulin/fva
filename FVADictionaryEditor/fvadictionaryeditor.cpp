#include "fvadictionaryeditor.h"
#include "fvacommonlib.h"

FVADictionaryEditor::FVADictionaryEditor(const QString&	dictPath,const QString& device, QWidget *parent)
	: QDialog	(parent),
	m_dictPath	(dictPath),
	m_device	(device)
{
	ui.setupUi(this);

	connect (ui.btnAddPerson,SIGNAL(clicked()),this,SLOT(OnAddPersonBtnPressed()) );
	connect (ui.btnAddPlace,SIGNAL(clicked()),this,SLOT(OnAddPlaceBtnPressed()));
	connect (ui.btnAddDevice,SIGNAL(clicked()),this,SLOT(OnAddDeviceBtnPressed()));

	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return;

	ui.cbOwner->clear();
	ui.cbOwner->addItem ( tr("Выбирете владельца"), 0 );
	QVariantList people = dictionaries["people"].toList();
	for ( auto i = people.begin(); i != people.end() ; ++i )
		ui.cbOwner->addItem ( i->toMap()["name"].toString(), i->toMap()["ID"].toString());

	ui.editName->setText(m_device);
	ui.editLinkName->setText(m_device);

	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);
}

FVADictionaryEditor::~FVADictionaryEditor()
{

}
void addDictItem(const QString& dictPath, const QString& name, QDialog* dlg, const QString& dictName)
{
	if (name.isEmpty())
		return;

	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return;

	QVariantList vlist = dictionaries[dictName].toList();
	int maxID = -1;
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
	{
		if (maxID < i->toMap()["ID"].toInt())
			maxID = i->toMap()["ID"].toInt();
	}
	if ( -1 == maxID )
		return;
	QVariantMap newPerson;
	newPerson["ID"]		= maxID + 1;
	newPerson["name"]	= name;
	vlist.append(newPerson);

	dictionaries[dictName] = vlist;

	res = fvaSaveDictionary( dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return;

	dlg->close();
}
void FVADictionaryEditor::OnAddPersonBtnPressed()
{
	addDictItem(m_dictPath, ui.editPerson->text(),this, "people" );
}

void FVADictionaryEditor::OnAddDeviceBtnPressed()
{
	QString		error;
	QVariantMap	dictionaries;
	FVA_ERROR_CODE res = fvaLoadDictionary( m_dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return;

	QVariantList vlist = dictionaries["devices"].toList();
	int maxID = -1;
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )
	{
		if (maxID < i->toMap()["ID"].toInt())
			maxID = i->toMap()["ID"].toInt();
	}
	if ( -1 == maxID )
		return;

	QVariantMap newDevice;
	newDevice["ID"]		= maxID + 1;
	newDevice["name"]	= ui.editName->text();
	newDevice["LinkedName"]	= ui.editLinkName->text();
	int index = ui.cbOwner->currentIndex();
	if ( 1 <= index ) 
	{
		newDevice["OwnerId"] = ui.cbOwner->itemData( index ).toString();
	}

	vlist.append(newDevice);

	dictionaries["devices"] = vlist;

	res = fvaSaveDictionary( m_dictPath, dictionaries, error );
	if ( FVA_NO_ERROR != res )
		return;

	close();
}

void FVADictionaryEditor::OnAddPlaceBtnPressed()
{
	addDictItem(m_dictPath, ui.editPlace->text(),this, "places" );
}
