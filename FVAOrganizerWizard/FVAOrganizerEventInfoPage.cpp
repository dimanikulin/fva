/*!
* \file FVAOrganizerEventInfoPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include <cstddef> // for std::nullptr_t

#include "FVAOrganizerEventInfoPage.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

#include <QUrl>
#include <QDesktopServices>
#include <QDir>
#include <QBrush>
#include <QFont>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvalogger.inl"
#include "fvaconfiguration.h"

void populateInputDir(const QString& folder, QTreeWidgetItem* item, QTreeWidget* treeWidget)
{
	QDir dir(folder);
	Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{			
		if ( !info.isDir() )
			continue;
		// just skip internal folder
		if ( info.fileName()[0] == '#' 
			&& info.fileName()[info.fileName().size()-1] == '#' )
			continue;

		QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem;
                treeWidgetItem->setText	( 0, info.fileName() );

		treeWidgetItem->setData( 1, 1, (QString) info.absoluteFilePath() );

		if (info.fileName().length()!=4) // not YEAR folder
			treeWidgetItem->setForeground( 0 , QBrush (Qt::red) );

		if (item)
		{
			item->addChild(treeWidgetItem);
			QIcon	icon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/folder.png");
			item->setIcon(0, icon);
		}
		else
			treeWidget->addTopLevelItem (treeWidgetItem);

		populateInputDir(info.absoluteFilePath(), treeWidgetItem, treeWidget);
	}		
}

FVAOrganizerEventInfoPage::FVAOrganizerEventInfoPage(void)
{
        LOG_DEB << "FVAOrganizerEventInfoPage construction" ;
	
#ifdef  FVA_LANGUAGE_RUS
	words	= new QLabel(tr("Пожалуйста, выделите папку нижу (событию),помеченной красным\nи выберете для нее тип события и причастных людей"));
	
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Открыть папку"));
	saveButton  = new QPushButton;
	saveButton->setText(tr("Сохранить инфо о папке"));
	
	QLabel * eventLbl = new QLabel(tr("Тип события:"));
	QLabel * peopleLbl = new QLabel(tr("Причастные люди:"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	words = new QLabel(tr("Please select a folder bellow (an event),marked by red\nand choose event type and related people."));
	
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Open a folder"));
	saveButton  = new QPushButton;
	saveButton->setText(tr("Save dir info"));
	
	QLabel * eventLbl = new QLabel(tr("Event type:"));
	QLabel * peopleLbl = new QLabel(tr("Related people:"));

#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
			
	QVBoxLayout * layout	= new QVBoxLayout;

	layout->addWidget(words);
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
	btnLayout->addWidget(inputDirButton,0,1);

	layout->addLayout(btnLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect( inputDirButton, SIGNAL( clicked() ), this, SLOT( OnFvaInputDirButtonPressed() ) );
	connect( saveButton, SIGNAL( clicked() ), this, SLOT( OnSaveButtonPressed() ) );

        LOG_DEB << "FVAOrganizerEventInfoPage constructed" ;

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

	if (peopleIds.length() == 0)
	{
		FVA_MESSAGE_BOX("no people are selected")
		return;
	}

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

void FVAOrganizerEventInfoPage::updateChecks(QTreeWidgetItem *item, int column)
{
	fvaUpdateChecks(item, column);
}

void FVAOrganizerEventInfoPage::setVisible(bool visible)
{	
	LOG_DEB << "FVAOrganizerEventInfoPage::setVisible";
	QString		inputDir		= ((FVAOrganizerWizard*)wizard())->inputFolder();

	if (visible)
	{
		LOG_DEB << "FVAOrganizerEventInfoPage::setVisible if (visible)";

		FvaConfiguration cfg;
		FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("load.cfg")

		QString rootSWdir;
		exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("get.param")

		populateInputDir(inputDir, nullptr, inputDirsWidget);

		exitCode = fvaBuildPeopleFilterTree(this, peopleWidget, false, rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("fvaBuildPeopleFilterTree")

		exitCode = fvaBuildEventTree(this, eventsWidget, rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("fvaBuildEventTree")
	}
	LOG_DEB << "FVAOrganizerEventInfoPage::setVisible before exit";
	return QWizardPage::setVisible(visible);
}
bool FVAOrganizerEventInfoPage::validatePage()
{
        LOG_DEB << "FVAOrganizerEventInfoPage validate page" ;
	FVAFlowController flow;

	FVA_EXIT_CODE exitCode = flow.ProcessInputDirForEvent(dir2EventMap, dir2PeopleMap, this);
	if (exitCode != FVA_NO_ERROR)
		return false;

	return true;
}
