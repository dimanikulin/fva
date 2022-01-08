/*!
* \file FVAOrganizerPlacePage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include <cstddef> // for std::nullptr_t

#include "FVAOrganizerPlacePage.h"

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

FVAOrganizerPlacePage::FVAOrganizerPlacePage(void)
{
        LOG_DEB << "construction" ;
	
#ifdef  FVA_LANGUAGE_RUS
	words	= new QLabel(tr("Пожалуйста, выделите папку нижу (событию),помеченной красным\nи выберете для нее место, где оно происходило"));
	
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Открыть папку"));
	saveButton  = new QPushButton;
	saveButton->setText(tr("Сохранить инфо о папке"));
	addNewPlaceButton = new QPushButton;
	addNewPlaceButton->setText(tr("Добавить место"));
	
	QLabel * placeLbl = new QLabel(tr("Место:"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	words = new QLabel(tr("Please select a folder bellow (an event),marked by red\nand choose a place it happened in."));
	
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Open a folder"));
	saveButton  = new QPushButton;
	saveButton->setText(tr("Save dir info"));
	addNewPlaceButton = new QPushButton;
	addNewPlaceButton->setText(tr("Add a place"));
	
	QLabel * placeLbl = new QLabel(tr("Place:"));

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

	placesWidget		= new QTreeWidget;
	placesWidget->setHeaderHidden(true);
	infoLayout->addWidget(placeLbl,0,0); 	
	infoLayout->addWidget(placesWidget,1,0);

        layout->addLayout(infoLayout);

	logOutput		= new QTextBrowser;

	QGridLayout * btnLayout= new QGridLayout;
	btnLayout->addWidget(saveButton,0,0);
	btnLayout->addWidget(addNewPlaceButton,0,1);

	layout->addLayout(btnLayout);

	layout->addWidget(logOutput);

	setLayout(layout);

	connect( inputDirButton, SIGNAL( clicked() ), this, SLOT( OnFvaInputDirButtonPressed() ) );
	connect( saveButton, SIGNAL( clicked() ), this, SLOT( OnSaveButtonPressed() ) );
	connect( addNewPlaceButton, SIGNAL( clicked() ), this, SLOT( OnAddPlacePressed() ) );

        LOG_DEB << "constructed" ;

}

void FVAOrganizerPlacePage::OnAddPlacePressed()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVADictionaryEditor.exe");
	myProcess.waitForFinished( -1 );	
}

void FVAOrganizerPlacePage::OnSaveButtonPressed()
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

	QList<unsigned int> placesIds;

	for (auto idTop = 0; idTop < placesWidget->topLevelItemCount();++idTop)
		fvaFindCheckedItem(placesWidget->topLevelItem(idTop), placesIds);

	if (placesIds.length() == 0)
	{
		FVA_MESSAGE_BOX("no place is selected")
		return;
	}

	if (placesIds.length() > 1)
	{
		FVA_MESSAGE_BOX("too many places are selected")
		return;
	}
  
	dir2PlaceMap[selectedInputDirPath] = placesIds.at(0);
	
	for (auto idTop = 0; idTop < placesWidget->topLevelItemCount();++idTop)
		fvaClearChecks(placesWidget->topLevelItem(idTop));

}

void FVAOrganizerPlacePage::OnFvaInputDirButtonPressed()
{
	QString path = QDir::toNativeSeparators(((FVAOrganizerWizard*)wizard())->inputFolder());
	LOG_DEB << "OnFvaInputDirButtonPressed() input dir=" << path;
	if(!QDesktopServices::openUrl(QUrl::fromLocalFile(((FVAOrganizerWizard*)wizard())->inputFolder())))
		LOG_DEB << "OnFvaInputDirButtonPressed() failed to show input dir=" << path;
	else
		LOG_DEB << "OnFvaInputDirButtonPressed() shows input dir=" << path;	
}

void FVAOrganizerPlacePage::updateChecks(QTreeWidgetItem *item, int column)
{
	fvaUpdateChecks(item, column);
}

void FVAOrganizerPlacePage::setVisible(bool visible)
{	
	LOG_DEB << "setVisible";
	QString		inputDir		= ((FVAOrganizerWizard*)wizard())->inputFolder();

	if (visible)
	{
		LOG_DEB << "setVisible if (visible)";

		FvaConfiguration cfg;
		FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("load.cfg")

		QString rootSWdir;
		exitCode = cfg.getParamAsString("Common::RootDir", rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("get.param")

		FVAFlowController flow;

		STR_LIST fileListToFillUp;
		exitCode = flow.GetProblemFilesList(fileListToFillUp);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("flow.GetProblemFilesList")

		// populate inout file struture with files
		fvaPopulateInputDir(inputDir, nullptr, inputDirsWidget, fileListToFillUp);

		exitCode = fvaBuildPlaceTree(this, placesWidget, rootSWdir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("fvaBuildPlaceTree")
	}
	LOG_DEB << "setVisible before exit";
	return QWizardPage::setVisible(visible);
}
bool FVAOrganizerPlacePage::validatePage()
{
        LOG_DEB << "validate page" ;
	FVAFlowController flow;

	FVA_EXIT_CODE exitCode = flow.ProcessInputDirForPlaces(dir2PlaceMap, this);
	if (exitCode != FVA_NO_ERROR)
		return false;

	return true;
}
