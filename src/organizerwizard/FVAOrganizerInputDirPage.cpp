/*!
* \file FVAOrganizerInputDirPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerInputDirPage.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>

#include "fvaorganizerwizard.h"

#include "FVAFlowController.h"
#include "fvalogger.inl"

FVAOrganizerInputDirPage::FVAOrganizerInputDirPage(void)
{
        LOG_DEB << "construction" ;
#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	inputDirLabel = new QLabel(tr("Please select a folder with a content\n (Attention! The folder name shall start with a char '#'):"));
	dirButton = new QPushButton;
	dirButton->setText(tr("Select a folder"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
	
	inputDirLineEdit = new QLineEdit;
	inputDirLineEdit->setText("");
	inputDirLineEdit->setReadOnly(true);
		
	QGridLayout * dirLayout = new QGridLayout;
	dirLayout->addWidget(inputDirLineEdit,0,0);
	dirLayout->addWidget(dirButton,0,1);

	logOutput		= new QTextBrowser;
	
	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(inputDirLabel);
	layout->addLayout(dirLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect( dirButton, SIGNAL( clicked() ), this, SLOT( OnDirButtonClicked() ) );
        LOG_DEB << "constructed" ;
}
void FVAOrganizerInputDirPage::OnDirButtonClicked()
{
	QFileDialog dirDialog;

	QString path = dirDialog.getExistingDirectory();

	if (!path.isEmpty())
	{
		inputDirLineEdit->setText(path);
		emit completeChanged();
	}
}
bool FVAOrganizerInputDirPage::isComplete() const
{
	QString dir = inputDirLineEdit->text();

	// make button be disabled if dir.isEmpty()
	if (dir.isEmpty())
	{
		return false;
	}
	return true;
}
bool	FVAOrganizerInputDirPage::validatePage ()
{
        LOG_DEB << "validate page" ;
	// get the directory the user selected
	QString dir = inputDirLineEdit->text();

	// create the flow controller to proceed 
	FVAFlowController flow;
	DeviceContext deviceContext;

	// call the flow controller PerformChecksForInputDir method to check the multimedia data and to fix it if needs
	FVA_EXIT_CODE exitCode = flow.PerformChecksForInputDir(dir, deviceContext,this);
	if (exitCode != FVA_NO_ERROR)
		return false;

	((FVAOrganizerWizard*)wizard())->inputFolder(dir);
	((FVAOrganizerWizard*)wizard())->fullDeviceMap(deviceContext.fullDeviceMap);
	((FVAOrganizerWizard*)wizard())->matchedDeviceMap(deviceContext.deviceMap);
	((FVAOrganizerWizard*)wizard())->matchedDeviceName(deviceContext.matchedDeviceName);
	
	return true;
}
