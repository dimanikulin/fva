/*!
* \file FVAOrganizerEventCfgPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerEventCfgPage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>

#include <QUrl>
#include <QDesktopServices>
#include <QDir>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvalogger.inl"

FVAOrganizerEventCfgPage::FVAOrganizerEventCfgPage(void)
{
        LOG_DEB << "construction" ;
#ifdef  FVA_LANGUAGE_RUS
	words	= new QLabel(tr("Пожалуйста, откройте входную папку и объедините однодневные папки в одну,\nесли они относятся к одному событию. Так проделайте для каждого события"));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Открыть папку"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	words = new QLabel(tr("Please open an input folder and merge one-day dirs into one dir,\nif they are related to the same event. Please do so for each event"));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Open a folder"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
			
	QGridLayout * dirLayout = new QGridLayout;
	dirLayout->addWidget(words,0,0);
	dirLayout->addWidget(inputDirButton,1,0);

	logOutput		= new QTextBrowser;
	
	QVBoxLayout * layout = new QVBoxLayout;

	layout->addLayout(dirLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect( inputDirButton, SIGNAL( clicked() ), this, SLOT( OnFvaInputDirButtonPressed() ) );
        LOG_DEB << "constructed" ;

}
void FVAOrganizerEventCfgPage::OnFvaInputDirButtonPressed()
{
	QString path = QDir::toNativeSeparators(((FVAOrganizerWizard*)wizard())->inputFolder());
	LOG_DEB << "OnFvaInputDirButtonPressed() input dir=" << path;
	if(! QDesktopServices::openUrl(QUrl::fromLocalFile(((FVAOrganizerWizard*)wizard())->inputFolder())))
		LOG_DEB << "OnFvaInputDirButtonPressed() failed to show input dir=" << path;
	else
		LOG_DEB << "OnFvaInputDirButtonPressed() shows input dir=" << path;	
}
