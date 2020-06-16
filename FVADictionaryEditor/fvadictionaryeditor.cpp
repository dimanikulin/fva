#include "fvadictionaryeditor.h"
#include "fvacommonlib.h"

FVADictionaryEditor::FVADictionaryEditor(const QString&	dictPath, QWidget *parent)
	: QDialog	(parent),
	m_dictPath	(dictPath)
{
	ui.setupUi(this);

	connect (ui.btnAddPerson,SIGNAL(clicked()),this,SLOT(OnAddPersonBtnPressed()) );
	connect (ui.btnAddPlace,SIGNAL(clicked()),this,SLOT(OnAddPlaceBtnPressed()));
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

void FVADictionaryEditor::OnAddPlaceBtnPressed()
{
	addDictItem(m_dictPath, ui.editPlace->text(),this, "places" );
}
