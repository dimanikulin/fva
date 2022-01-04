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

// TODO to make multilanguage

// TODO make a validate for data entered

FVA_EXIT_CODE fillUpCB(const QString& rootSWdir,const QString& dictName, QComboBox* cb)
{
	FVA_SIMPLE_MAP fvaMap;
	FVA_EXIT_CODE exitCode = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, fvaMap, dictName);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE("FVADictionaryEditor." + dictName)
	
	cb->clear();
	for (auto i = fvaMap.begin(); i != fvaMap.end(); ++i)
		cb->addItem(i->second, i->first);

	return FVA_NO_ERROR;
}

FVADictionaryEditor::FVADictionaryEditor(const QString& device, QWidget *parent)
	: QDialog	(parent),
	 m_device	(device)
{
	LOG_DEB << "construction";
	ui.setupUi(this);

	connect (ui.btnAddPerson,SIGNAL(clicked()),this,SLOT(OnAddPersonBtnPressed()));
	connect (ui.btnAddPlace,SIGNAL(clicked()),this,SLOT(OnAddPlaceBtnPressed()));
	connect (ui.btnAddDevice,SIGNAL(clicked()),this,SLOT(OnAddDeviceBtnPressed()));
	connect (ui.btnAddEvent,SIGNAL(clicked()),this,SLOT(OnAddEventBtnPressed()));

	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.load.cfg")

	QString rootSWdir;
	exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.get.rootdir")

	LOG_DEB << "people group box building"; 
	PEOPLE_MAP peopleMap;
	exitCode = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.fvaLoadPeopleMapFromCsv")

	ui.cbOwner->clear();
	ui.cbOwner->addItem ( tr("Выберете владельца"), 0 );
	for (auto i = peopleMap.begin(); i != peopleMap.end(); ++i)
		ui.cbOwner->addItem(i->name, i->Id);

	LOG_DEB << "device group box building"; 
	ui.editName->setText(m_device);
	ui.editLinkName->setText(m_device);

	LOG_DEB << "event group box building"; 
	fillUpCB(rootSWdir, "fvaEvents.csv", ui.cbEventType);

	fillUpCB(rootSWdir, "fvaInstitutions.csv", ui.cbEventTypeInstit);

	LOG_DEB << "location group box building"; 
	fillUpCB(rootSWdir, "fvaPlaceTypes.csv", ui.cbPlaceType);

	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);
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
	// ID,OwnerId,LinkedName,Name,Type
	writeStream << "\n" 
		<< deviceMap.lastKey() + 1					<< ","
		<< ui.cbOwner->itemData(ui.cbOwner->currentIndex()).toString()	<< ","
		<< ui.editLinkName->text()					<< ","
		<< ui.editName->text()						<< ","
		<< "1"; // hardcoded now to photo-video device type 
	writeStream.flush();
	file.close();

	close();
}

void FVADictionaryEditor::OnAddPlaceBtnPressed()
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.getRootDir")

	FVA_SIMPLE_MAP fvaMap;
	exitCode = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, fvaMap, "fvaPlaces.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.fvaPlaces.csv" )

	QFile file(rootSWdir + "#data#/fvaPlaces.csv");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream writeStream(&file);

	writeStream.setCodec("UTF-8"); 
	// ID,Name,Type,GPSLatitude,GPSLongitude,GPSLatitudeRef,GPSLongitudeRef
	writeStream << "\n" 
		<< fvaMap.lastKey() + 1						<< ",\""
		<< ui.editPlace->text()						<< "\","
		<< ui.cbPlaceType->itemData(ui.cbPlaceType->currentIndex()).toInt()	<< ","
		<< ui.editGPSLatitude->text()					<< ","
		<< ui.editGPSLongitude->text()					<< ","
		<< ui.cbGPSLatitudeRef->currentText().at(0)			<< ","
		<< ui.cbGPSLongitudeRef->currentText().at(0);

	writeStream.flush();
	file.close();

	close();
}
void FVADictionaryEditor::OnAddEventBtnPressed()
{
	QString rootSWdir;
	FVA_EXIT_CODE exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.getRootDir")

	FVA_SIMPLE_MAP fvaMap;
	exitCode = fvaLoadSimpleMapFromCsvByItemType(rootSWdir, fvaMap, "fvaEvents.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("FVADictionaryEditor.fvaEvents.csv" )

	QFile file(rootSWdir + "#data#/fvaEvents.csv");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream writeStream(&file);

	writeStream.setCodec("UTF-8"); 
	// ID,Name,Type,Institution
	writeStream << "\n" 
		<< fvaMap.lastKey() + 1						<< ",\""
		<< ui.editEvent->text()						<< "\","
		<< ui.cbEventType->itemData(ui.cbEventType->currentIndex()).toInt()	<< ","
		<< ui.cbEventTypeInstit->itemData(ui.cbEventTypeInstit->currentIndex()).toInt();
	writeStream.flush();
	file.close();

	close();
}