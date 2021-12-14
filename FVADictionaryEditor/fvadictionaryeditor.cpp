/*!
* \file FVADictionaryEditor.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvadictionaryeditor.h"
#include "fvacommonlib.h"
#include "fvacommoncsv.h"
#include "fvalogger.inl"
#include "fvacommonui.h"

#include "QtCore/QFile"
#include "QtCore/QTextStream"

FVADictionaryEditor::FVADictionaryEditor(const QString& device, QWidget *parent)
	: QDialog	(parent),
	 m_device	(device)
{
	LOG_DEB << "FVADictionaryEditor construction";
	ui.setupUi(this);

	connect (ui.btnAddPerson,SIGNAL(clicked()),this,SLOT(OnAddPersonBtnPressed()));
	connect (ui.btnAddPlace,SIGNAL(clicked()),this,SLOT(OnAddPlaceBtnPressed()));
	connect (ui.btnAddDevice,SIGNAL(clicked()),this,SLOT(OnAddDeviceBtnPressed()));

	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.load.cfg")

	QString rootSWdir;
	exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.get.rootdir")

	PEOPLE_MAP peopleMap;
	exitCode = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.fvaLoadPeopleMapFromCsv")

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
	LOG_DEB << "FVADictionaryEditor constructed";
}

void FVADictionaryEditor::OnAddPersonBtnPressed()
{
	//addDictItem(m_dictPath, ui.editPerson->text(),this, "people" );
}

void FVADictionaryEditor::OnAddDeviceBtnPressed()
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.getRootDir")

	DEVICE_MAP deviceMap;
	exitCode = fvaLoadDeviceMapFromCsv(rootSWdir, deviceMap);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.fvaLoadDeviceMapFromCsv")
	
	QFile file(rootSWdir + "#data#/fvadevices.csv");
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
	// TODO to fill up the fvaPlaces.csv you need to find the location by its name and do it at least on 2 map services 
	// like Google Map and Yandex. (https://www.google.com/maps and 
}
