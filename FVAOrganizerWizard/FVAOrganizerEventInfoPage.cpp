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
	words	= new QLabel(tr("Пожалуйста, выделите папку нижу (событию),помеченной красным\nи выберете для нее тип события и причастных людей"));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Открыть папку"));
	QLabel * eventLbl = new QLabel(tr("Тип события:"));
	QLabel * peopleLbl = new QLabel(tr("Причастные люди:"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	words = new QLabel(tr("Please select a folder bellow (an event),marked by red\nand choose event type and related people."));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Open a folder"));
	QLabel * eventLbl = new QLabel(tr("Event type:"));
	QLabel * peopleLbl = new QLabel(tr("Related people:"));

#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
			
	QVBoxLayout * layout	= new QVBoxLayout;

	layout->addWidget(words);
	layout->addWidget(dirLabel);
	inputDirs = new QTreeWidget;
	layout->addWidget(inputDirs);

	QGridLayout * infoLayout= new QGridLayout;

	events = new QTreeWidget;
	//events->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	//events->setMinimumSize(100,100);
	infoLayout->addWidget(eventLbl,0,0); 	
	infoLayout->addWidget(events,1,0);

	people = new QTreeWidget;
	//people->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	//people->setMinimumSize(100,100);
	infoLayout->addWidget(peopleLbl,0,1);
	infoLayout->addWidget(people,1,1);

        layout->addLayout(infoLayout);

	logOutput		= new QTextBrowser;

	layout->addWidget(inputDirButton);
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
