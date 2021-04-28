#include "fvadictionaryeditor.h"
#include "fvacommonlib.h"
#include "fvacommoncsv.h"
#include "fvadefaultcfg.h"

#include "QtCore/QFile"
#include "QtCore/QTextStream"

FVADictionaryEditor::FVADictionaryEditor(const QString&	dictPath,const QString& device, QWidget *parent)
	: QDialog	(parent),
	 m_dictPath	(dictPath),
	 m_device	(device)
{
	ui.setupUi(this);

	connect (ui.btnAddPerson,SIGNAL(clicked()),this,SLOT(OnAddPersonBtnPressed()));
	connect (ui.btnAddPlace,SIGNAL(clicked()),this,SLOT(OnAddPlaceBtnPressed()));
	connect (ui.btnAddDevice,SIGNAL(clicked()),this,SLOT(OnAddDeviceBtnPressed()));

	PEOPLE_MAP peopleMap;
	FVA_EXIT_CODE res = fvaLoadPeopleMapFromCsv(peopleMap);
	RET_IF_RES_IS_ERROR

	ui.cbOwner->clear();
	ui.cbOwner->addItem ( tr("Выбирете владельца"), 0 );
	for (auto i = peopleMap.begin(); i != peopleMap.end(); ++i)
		ui.cbOwner->addItem(i->name, i->Id);

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

void FVADictionaryEditor::OnAddPersonBtnPressed()
{
	//addDictItem(m_dictPath, ui.editPerson->text(),this, "people" );
}

void FVADictionaryEditor::OnAddDeviceBtnPressed()
{
	DEVICE_MAP deviceMap;
	FVA_EXIT_CODE res = fvaLoadDeviceMapFromCsv(deviceMap);
	RET_IF_RES_IS_ERROR
	
	QFile file(FVA_DEFAULT_ROOT_DIR + "#data#/fvadevices.csv");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream writeStream(&file);
	writeStream.setCodec("UTF-8"); 
	// ID,OwnerId,LinkedName,Name,fvaDeviceType
	writeStream << "\n" 
				<< deviceMap.lastKey() + 1										<< ","
				<< ui.cbOwner->itemData(ui.cbOwner->currentIndex()).toString()	<< ","
				<< ui.editLinkName->text()										<< ","
				<< ui.editName->text()											<< ","
				<< "1"; // hardcoded now to photo-video device type 
	writeStream.flush();
	file.close();

	close();
}

void FVADictionaryEditor::OnAddPlaceBtnPressed()
{
	//addDictItem(m_dictPath, ui.editPlace->text(),this, "places" );
}
