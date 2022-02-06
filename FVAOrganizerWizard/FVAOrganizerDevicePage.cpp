/*!
* \file FVAOrganizerDevicePage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerDevicePage.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>

#include <QtCore/QProcess>

#include "fvacommoncsv.h"
#include "fvacommonui.h"
#include "fvalogger.inl"
#include "fvaorganizerwizard.h"
#include "FVAFlowController.h"

FVAOrganizerDevicePage::FVAOrganizerDevicePage(void)
	: deviceId (-1)
{
        LOG_DEB << "construction"; 

        btnDct = new QPushButton;

#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	QLabel * titleLabel = new QLabel(tr("Please make sure, that device, took the photos, is defined properly!"));
	deviceLbl = new QLabel(tr("Device Name:"));
	matchLbl = new QLabel(tr("Linking name:"));
	ownerLbl = new QLabel(tr("Device owner:"));
	btnDct->setText(tr("Dictionary"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	deviceName	= new QLineEdit;
	deviceName->setMaxLength(40);
	matchName	= new QLineEdit;
	matchName->setMaxLength(40);

	ownerName	= new QLineEdit;
	cbDevice	= new QComboBox;

	QGridLayout * tableLayout = new QGridLayout;

	tableLayout->addWidget(deviceLbl,0,0);
	tableLayout->addWidget(deviceName,0,1);	
	tableLayout->addWidget(btnDct,0,2);

	tableLayout->addWidget(matchLbl,1,0);
	tableLayout->addWidget(matchName,1,1);

	tableLayout->addWidget(ownerLbl,2,0);
	tableLayout->addWidget(ownerName,2,1);
	tableLayout->addWidget(cbDevice,2,2);
	
	logOutput	= new QTextBrowser;
	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(titleLabel);
	layout->addLayout(tableLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect (btnDct,SIGNAL(clicked()),this,SLOT(OnChangeDictPressed()));

        LOG_DEB << "constructed"; 

}
void FVAOrganizerDevicePage::setVisible(bool visible)
{	
	LOG_DEB << "setVisible";
	DEVICE_MAP	deviceMap		= ((FVAOrganizerWizard*)wizard())->matchedDeviceMap();
	QString		deviceName_		= ((FVAOrganizerWizard*)wizard())->matchedDeviceName();

	if (visible)
	{
		LOG_DEB << "setVisible if (visible)";
		matchName->setText(deviceName_);
		matchName->setReadOnly(true);
		cbDevice->setVisible(false);
		if ( deviceMap.size() > 1 )
		{
			LOG_DEB << "setVisible deviceMap.size() > 1";
			cbDevice->setVisible(true);
			cbDevice->clear();
#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
			cbDevice->addItem(tr("Select the owner"), 0);
			for (auto i = deviceMap.begin(); i != deviceMap.end(); ++i)
				cbDevice->addItem(i->ownerName, i->deviceId);

			deviceName->setText(tr("UNDEFINED!"));
			ownerName->setText(tr("UNDEFINED!"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
		}
		else if (deviceMap.size() ==1 )
		{	
			LOG_DEB << "setVisible deviceMap.size() == 1";
			deviceName->setText(deviceMap.begin().value().guiName);
			ownerName->setText(deviceMap.begin().value().ownerName);
			deviceId = deviceMap.begin().value().deviceId;
			emit completeChanged();
		}
		else
		{
			LOG_DEB << "setVisible deviceMap.size() != 1 and !<";
#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
			deviceName->setText(tr("UNDEFINED!"));
			ownerName->setText(tr("UNDEFINED!"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
		}
	}
	LOG_DEB << "setVisible before exit";
	return QWizardPage::setVisible(visible);
}
void FVAOrganizerDevicePage::OnChangeDictPressed()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(((FVAOrganizerWizard*)wizard())->matchedDeviceName());
	LOG_DEB << "called FVADictionaryEditor with device=" << ((FVAOrganizerWizard*)wizard())->matchedDeviceName();
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVADictionaryEditor.exe", params);
	myProcess.waitForFinished( -1 );
}
bool FVAOrganizerDevicePage::isComplete() const
{
	LOG_DEB << "isComplete";
	// make button next ne disabled
	if (deviceId == -1)
	{
		return false;
	}
	return true;
}

bool FVAOrganizerDevicePage::validatePage()
{
	LOG_DEB << "validatepage";
	DEVICE_MAP deviceMap = ((FVAOrganizerWizard*)wizard())->matchedDeviceMap();
	if ( deviceMap.size() > 1 )
	{
		int index = cbDevice->currentIndex();
		if ( 1 <= index ) 
		{
			int ID = cbDevice->itemData( index ).toInt();
			if (ID >= 1)
			{
				deviceId = ID;
				((FVAOrganizerWizard*)wizard())->matchedDeviceID(deviceId);
			}
		}
	}

	FVAFlowController flow;
	FVA_EXIT_CODE exitCode = flow.OrganizeInputDir(((FVAOrganizerWizard*)wizard())->inputFolder(), deviceId);
	if (exitCode != FVA_NO_ERROR)
		return false;
	return true;
}
