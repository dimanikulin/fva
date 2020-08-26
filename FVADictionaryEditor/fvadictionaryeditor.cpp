#include "fvadictionaryeditor.h"
#include "fvacommonlib.h"
#include "fvacommondb.h"

/*!
* \brief it saves FVA dictionaries to file
* \param file to save dictionaries to
* \param outputJson input parameter to be saved
* \param error - human-readable description of error if any
* \returns it returns code of error if any or FVA_NO_ERROR if saving was successful
*/
FVA_EXIT_CODE fvaSaveDictionary(const QString& file, QVariantMap& inputJson, QString& error);

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
	FVA_EXIT_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	RET_IF_RES_IS_ERROR

	ui.cbOwner->clear();
	ui.cbOwner->addItem ( tr("Выбирете владельца"), 0 );
	QVariantList people = dictionaries["people"].toList();
	for ( auto i = people.begin(); i != people.end() ; ++i )
		ui.cbOwner->addItem ( i->toMap()["name"].toString(), i->toMap()["ID"].toString());

	ui.editName->setText(m_device);
	ui.editLinkName->setText(m_device);

	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);

	ui.groupBox->setDisabled(true);
	ui.groupBox_2->setDisabled(true);
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
	FVA_EXIT_CODE res = fvaLoadDictionary( dictPath, dictionaries, error );
	RET_IF_RES_IS_ERROR

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
	RET_IF_RES_IS_ERROR

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
	FVA_EXIT_CODE res = fvaLoadDictionary( m_dictPath, dictionaries, error );
	RET_IF_RES_IS_ERROR

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
	RET_IF_RES_IS_ERROR

	close();
}

void FVADictionaryEditor::OnAddPlaceBtnPressed()
{
	addDictItem(m_dictPath, ui.editPlace->text(),this, "places" );
}
FVA_EXIT_CODE fvaSaveDictionary(const QString& file, QVariantMap& inputJson, QString& error)
{
	/*QDir dir(file);
	if (!dir.exists(file))
	{
		error = "dictionaries file does not exist";
		return FVA_ERROR_CANT_FIND_DICTIONARIES;
	}
	else
	{
		QString newName = file + "_" + QDateTime::currentDateTime().toString(FVA_FILE_NAME_FMT).toLatin1().data();
		if (!dir.rename(file, newName))
		{
			return FVA_ERROR_CANT_OPEN_DICTIONARIES;
		}
	}
	// open it
	QFile _file(file);
	if (!_file.open(QIODevice::WriteOnly))
	{
		error = "can not open dictionaries";
		return FVA_ERROR_CANT_OPEN_DICTIONARIES;
	}

	bool res = false;

	QByteArray data = QtJson::Json::serialize(inputJson, res);
	if (!res)
	{
		error = "can not save dictionaries";
		_file.close();
		return FVA_ERROR_CANT_SAVE_DICTIONARIES;
	}
	QString result = QString::fromLocal8Bit(data);

	QTextStream writeStream(&_file);
	writeStream << result;
	writeStream.flush();
	_file.close();*/

	return FVA_NO_ERROR;
}
