/*!
* \file FVAOrganizerEventInfoPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include <cstddef> // for std::nullptr_t

#include "FVAOrganizerEventInfoPage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QUrl>
#include <QDesktopServices>
#include <QDir>
#include <QBrush>
#include <QFont>

#include <QtCore/QProcess>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvalogger.inl"
#include "fvaconfiguration.h"

FVAOrganizerEventInfoPage::FVAOrganizerEventInfoPage(void)
{
        LOG_DEB << "construction" ;
	
#ifdef  FVA_LANGUAGE_RUS

#else 
#ifdef  FVA_LANGUAGE_ENG
	words = new QLabel(tr("Please select a folder bellow (an event),marked by red\nand choose event type and related people."));
	
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Open a folder"));
	saveButton  = new QPushButton;
	saveButton->setText(tr("Save dir info"));
	addNewEventButton = new QPushButton;
	addNewEventButton->setText(tr("Add event"));
	
	QLabel * eventLbl = new QLabel(tr("Event type:"));
	QLabel * peopleLbl = new QLabel(tr("Related people:"));

#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
			
	QVBoxLayout * layout	= new QVBoxLayout;

	QGridLayout * headerLayout= new QGridLayout;
	headerLayout->addWidget(words,0,0);
	headerLayout->addWidget(inputDirButton,0,1);
	layout->addLayout(headerLayout);

	inputDirsWidget = new QTreeWidget;
	inputDirsWidget->setHeaderHidden(true);
	layout->addWidget(inputDirsWidget);

	QGridLayout * infoLayout= new QGridLayout;

	eventsWidget		= new QTreeWidget;
	eventsWidget->setHeaderHidden(true);
	infoLayout->addWidget(eventLbl,0,0); 	
	infoLayout->addWidget(eventsWidget,1,0);

	peopleWidget		= new QTreeWidget;
	peopleWidget->setHeaderHidden(true);
	infoLayout->addWidget(peopleLbl,0,1);
	infoLayout->addWidget(peopleWidget,1,1);

        layout->addLayout(infoLayout);

	logOutput		= new QTextBrowser;

	QGridLayout * btnLayout= new QGridLayout;
	btnLayout->addWidget(saveButton,0,0);
	btnLayout->addWidget(addNewEventButton,0,1);

	layout->addLayout(btnLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect( inputDirButton, SIGNAL( clicked() ), this, SLOT( OnFvaInputDirButtonPressed() ) );
	connect( saveButton, SIGNAL( clicked() ), this, SLOT( OnSaveButtonPressed() ) );
	connect( addNewEventButton, SIGNAL( clicked() ), this, SLOT( OnAddEventPressed() ) );

        LOG_DEB << "constructed" ;

}
void FVAOrganizerEventInfoPage::OnSaveButtonPressed()
{
	// check if there is a dir selected in inputDir Tree Widget
	QList<QTreeWidgetItem *> itemList = inputDirsWidget->selectedItems();

	if (itemList.length()!=1)
	{
		FVA_MESSAGE_BOX("too many folder selected")
		return;
	}
	QString selectedInputDirPath = itemList.at(0)->data(1, 1).toString();
	if (selectedInputDirPath.isEmpty())
	{
		FVA_MESSAGE_BOX("empty path for folder selected")
		return;
	}

	QList<unsigned int> eventIds, peopleIds;

	for (auto idTop = 0; idTop < peopleWidget->topLevelItemCount();++idTop)
		fvaFindCheckedItem(peopleWidget->topLevelItem(idTop), peopleIds);

	for (auto idTop = 0; idTop < eventsWidget->topLevelItemCount();++idTop)
		fvaFindCheckedItem(eventsWidget->topLevelItem(idTop), eventIds);

	if (eventIds.length() == 0)
	{
		FVA_MESSAGE_BOX("no event is selected")
		return;
	}

	if (eventIds.length() > 1)
	{
		FVA_MESSAGE_BOX("too many events are selected")
		return;
	}
  
	dir2EventMap[selectedInputDirPath] = eventIds.at(0);

	dir2PeopleMap[selectedInputDirPath] = peopleIds;
	
	peopleWidget->selectionModel()->clearSelection();
	eventsWidget->selectionModel()->clearSelection();

	for (auto idTop = 0; idTop < peopleWidget->topLevelItemCount();++idTop)
		fvaClearChecks(peopleWidget->topLevelItem(idTop));

	for (auto idTop = 0; idTop < eventsWidget->topLevelItemCount();++idTop)
		fvaClearChecks(eventsWidget->topLevelItem(idTop));	
}

void FVAOrganizerEventInfoPage::OnFvaInputDirButtonPressed()
{
	QString path = QDir::toNativeSeparators(((FVAOrganizerWizard*)wizard())->inputFolder());
	LOG_DEB << "OnFvaInputDirButtonPressed() input dir=" << path;
	if(!QDesktopServices::openUrl(QUrl::fromLocalFile(((FVAOrganizerWizard*)wizard())->inputFolder())))
		LOG_DEB << "OnFvaInputDirButtonPressed() failed to show input dir=" << path;
	else
		LOG_DEB << "OnFvaInputDirButtonPressed() shows input dir=" << path;	
}

void FVAOrganizerEventInfoPage::updateChecks(QTreeWidgetItem *item, int column)
{
	fvaUpdateChecks(item, column);
}

void FVAOrganizerEventInfoPage::setVisible(bool visible)
{	
	LOG_DEB << "setVisible";

	if (visible)
	{
		LOG_DEB << "setVisible if (visible)";

		FvaConfiguration cfg;
		FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("load.cfg")

		QString rootSWdir;
		exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("get.param")

		QString		inputDir		= ((FVAOrganizerWizard*)wizard())->inputFolder();

		STR_LIST fileListToFillUp; // empty for this case as we wanna have dirs only
		fvaPopulateInputDir(inputDir, nullptr, inputDirsWidget,fileListToFillUp);

		exitCode = fvaBuildPeopleTree(this, peopleWidget, rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("fvaBuildPeopleTree")

		exitCode = fvaBuildEventTree(this, eventsWidget, rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("fvaBuildEventTree")
	}
	LOG_DEB << "setVisible before exit";
	return QWizardPage::setVisible(visible);
}
void FVAOrganizerEventInfoPage::OnAddEventPressed()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVADictionaryEditor.exe");
	myProcess.waitForFinished( -1 );
}

bool FVAOrganizerEventInfoPage::validatePage()
{
        LOG_DEB << "validate page" ;
	FVAFlowController flow;
	QString		inputDir	= ((FVAOrganizerWizard*)wizard())->inputFolder();
	FVA_EXIT_CODE	exitCode	= flow.ProcessInputDirForEvents(inputDir, dir2EventMap, dir2PeopleMap, this);
	if (exitCode != FVA_NO_ERROR)
		return false;

	return true;
}
