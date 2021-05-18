/*!
* \file FVAOrganizerInputDirPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerInputDirPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFileDialog>

#include "FVAOrganizerWizard.h"

#include "FVAFlowController.h"

FVAOrganizerInputDirPage::FVAOrganizerInputDirPage(void)
{
#ifdef  FVA_LANGUAGE_RUS
	inputDirLabel	= new QLabel(tr("Укажите входную папку с контентом\n (Внимание! Папка должна начинаться с символа '#'):"));
	dirButton = new QPushButton;
	dirButton->setText(tr("Указать папку"));
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
	QString dir = inputDirLineEdit->text();

	FVAFlowController flow;
	DeviceContext deviceContext;
	FVA_EXIT_CODE exitCode = flow.PerformChecksForInputDir(dir, deviceContext,this);
	if (exitCode != FVA_NO_ERROR)
		return false;

	((FVAOrganizerWizard*)wizard())->inputFolder(dir);
	((FVAOrganizerWizard*)wizard())->fullDeviceMap(deviceContext.fullDeviceMap);
	((FVAOrganizerWizard*)wizard())->matchedDeviceMap(deviceContext.deviceMap);
	((FVAOrganizerWizard*)wizard())->matchedDeviceName(deviceContext.matchedDeviceName);
	
	return true;
}
