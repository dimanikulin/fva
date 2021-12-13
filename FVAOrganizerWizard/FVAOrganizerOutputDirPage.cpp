/*!
* \file FVAOrganizerOutputDirPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerOutputDirPage.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QCheckBox>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvaconfiguration.h"
#include "FVAFlowController.h"

FVAOrganizerOutputDirPage::FVAOrganizerOutputDirPage(void)
{   
        LOG_DEB << "FVAOrganizerOutputDirPage construction" ;
#ifdef  FVA_LANGUAGE_RUS
	QLabel* inputDirLabel = new QLabel(tr("Укажите папку digiKam для контента:"));
	QPushButton* digiKamButton = new QPushButton;
	digiKamButton->setText(tr("Указать папку"));
	QLabel* dirLabelGP = new QLabel(tr("Укажите папку Google Photo для контента:"));
	QPushButton* googlePhotoButton = new QPushButton;
	googlePhotoButton->setText(tr("Указать папку"));
	QCheckBox* removeOriginDirCheckBox = new QCheckBox(tr("Удалить исходную папку"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	QLabel* dirLabelDK = new QLabel(tr("Please select a digiKam dir to put the content in:"));
	QPushButton* digiKamButton = new QPushButton;
	digiKamButton->setText(tr("Select a folder"));
	QLabel* dirLabelGP = new QLabel(tr("Please select a Google Photo dir to put the content in:"));
	QPushButton* googlePhotoButton = new QPushButton;
	googlePhotoButton->setText(tr("Select a folder"));
	QCheckBox* removeOriginDirCheckBox = new QCheckBox(tr("Remove input dir"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout * layout = new QVBoxLayout;

	digiKamLineEdit = new QLineEdit;
	digiKamLineEdit->setReadOnly(true);

	googlePhotoLineEdit = new QLineEdit;
	googlePhotoLineEdit->setReadOnly(true);

	FvaConfiguration cfg;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.load")

	QGridLayout * dirLayout = new QGridLayout;

	bool integration = false;
	int rowindex = 0;
	exitCode = cfg.getParamAsBoolean("Integration::digiKam", integration);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.getParamAsBoolean")
	if (integration)
	{
		dirLayout->addWidget(dirLabelDK, rowindex++, 0);
		dirLayout->addWidget(digiKamLineEdit, rowindex, 0);
		dirLayout->addWidget(digiKamButton, rowindex, 1);
	}

	exitCode = cfg.getParamAsBoolean("Integration::GooglePhoto", integration);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.digiKam")

	if (integration)
	{
		dirLayout->addWidget(dirLabelGP, ++rowindex, 0);
		dirLayout->addWidget(googlePhotoLineEdit, ++rowindex, 0);
		dirLayout->addWidget(googlePhotoButton, rowindex, 1);
	}

	dirLayout->addWidget(removeOriginDirCheckBox, ++rowindex, 0);

	layout->addLayout(dirLayout);

	setLayout(layout);

	connect(digiKamButton, SIGNAL(clicked()), this, SLOT(OnDigiKamDirButtonClicked()));
	connect(googlePhotoButton, SIGNAL(clicked()), this, SLOT(OnGooglePhotoDirButtonClicked()));

        LOG_DEB << "FVAOrganizerOutputDirPage constructed" ;
}

void FVAOrganizerOutputDirPage::OnDigiKamDirButtonClicked()
{
	QFileDialog dirDialog;

	QString path = dirDialog.getExistingDirectory();

	if (!path.isEmpty())
	{
		digiKamLineEdit->setText(path);
	}
}
void FVAOrganizerOutputDirPage::OnGooglePhotoDirButtonClicked()
{
	QFileDialog dirDialog;

	QString path = dirDialog.getExistingDirectory();

	if (!path.isEmpty())
	{
		googlePhotoLineEdit->setText(path);
	}
}
bool FVAOrganizerOutputDirPage::isComplete() const
{
	// TODO to fix it
	// return !googlePhotoLineEdit->text().isEmpty() || !digiKamLineEdit->text().isEmpty();
	return true;
}
bool	FVAOrganizerOutputDirPage::validatePage ()
{
        LOG_DEB << "FVAOrganizerOutputDirPage validate page" ;
	// create the flow controller to proceed
	FVAFlowController flow;

	// prepare list of directories to pass later to the FVAFlowController
	STR_LIST dirList;

	// if googlePhotoLineEdit is not empty 
	if (!googlePhotoLineEdit->text().isEmpty())
		// add this folder into list
		dirList.append(googlePhotoLineEdit->text());

	// if digiKamLineEdit is not empty 
	if (!digiKamLineEdit->text().isEmpty())
		// add this folder into list
		dirList.append(digiKamLineEdit->text());

	// call the Flow Controller MoveInputDirToOutputDirs method to integrate the input data into Multimedia IR systems
	FVA_EXIT_CODE exitCode = flow.MoveInputDirToOutputDirs(
							((FVAOrganizerWizard*)wizard())->inputFolder(), 
								dirList, 
								removeOriginDirCheckBox->isChecked());
	
	return exitCode == FVA_NO_ERROR;
}
