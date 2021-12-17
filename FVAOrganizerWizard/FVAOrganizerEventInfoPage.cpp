/*!
* \file FVAOrganizerEventInfoPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerEventInfoPage.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>

#include <QUrl>
#include <QDesktopServices>
#include <QDir>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvalogger.inl"
#include "FVAFlowController.h"

FVAOrganizerEventInfoPage::FVAOrganizerEventInfoPage(void)
{
        LOG_DEB << "FVAOrganizerEventInfoPage construction" ;
	
#ifdef  FVA_LANGUAGE_RUS
	words	= new QLabel(tr("Пожалуйста, заполните информацию по каждой папке(событию),\n  (помеченной красным цветом) \n типом события и причастными людьми"));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Открыть папку"));
	QLabel* dirLabel = new QLabel(tr("Папки (события):"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	words = new QLabel(tr("Please fill the information up for each folder (an event),\n    (marked by red)   \n  by type and related people"));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Open a folder"));
	QLabel* dirLabel = new QLabel(tr("Folders(events):"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
			
	QGridLayout * dirLayout	= new QGridLayout;
	dirLayout->addWidget(words,0,0);
	dirLayout->addWidget(inputDirButton,1,0);

	QVBoxLayout * layout	= new QVBoxLayout;

	layout->addLayout(dirLayout);
	layout->addWidget(dirLabel);
	inputDirs = new QTreeWidget;
	layout->addWidget(inputDirs);

	QGridLayout * infoLayout= new QGridLayout;
	events = new QTreeWidget; 	
	dirLayout->addWidget(events,0,0);
	people = new QTreeWidget;
	dirLayout->addWidget(people,0,1);

	logOutput		= new QTextBrowser;
	layout->addWidget(logOutput);

	setLayout(layout);

	connect( inputDirButton, SIGNAL( clicked() ), this, SLOT( OnFvaInputDirButtonPressed() ) );
        LOG_DEB << "FVAOrganizerEventInfoPage constructed" ;

}
void FVAOrganizerEventInfoPage::OnFvaInputDirButtonPressed()
{
	QString path = QDir::toNativeSeparators(((FVAOrganizerWizard*)wizard())->inputFolder());
	LOG_DEB << "FVAOrganizerEventInfoPage::OnFvaInputDirButtonPressed() input dir=" << path;
	if(!QDesktopServices::openUrl(QUrl::fromLocalFile(((FVAOrganizerWizard*)wizard())->inputFolder())))
		LOG_DEB << "FVAOrganizerEventInfoPage::OnFvaInputDirButtonPressed() failed to show input dir=" << path;
	else
		LOG_DEB << "FVAOrganizerEventInfoPage::OnFvaInputDirButtonPressed() shows input dir=" << path;	
}

bool FVAOrganizerEventInfoPage::validatePage()
{
        LOG_DEB << "FVAOrganizerEventInfoPage validate page" ;
	FVAFlowController flow;
	FVA_EXIT_CODE exitCode = flow.ProcessInputDirForEvent(((FVAOrganizerWizard*)wizard())->inputFolder(), "TODO"/*, this*/);
	if (exitCode != FVA_NO_ERROR)
		return false;

	return true;
}
