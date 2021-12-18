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
#include "FVAFlowController.h"
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

		QFont font("" , 9 , QFont::Bold );	
		treeWidgetItem->setForeground( 0 , QBrush (Qt::red) );
		treeWidgetItem->setFont( 0,  font );

		// treeWidgetItem->setIcon(0, m_folderIcon);
		if (item)
			item->addChild(treeWidgetItem);
		else
			treeWidget->addTopLevelItem (treeWidgetItem);

		populateInputDir(info.absoluteFilePath(), item, treeWidget);
	}		
}
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
	inputDirsWidget = new QTreeWidget;
	layout->addWidget(inputDirsWidget);

	QGridLayout * infoLayout= new QGridLayout;

	eventsWidget		= new QTreeWidget;
	//events->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	//events->setMinimumSize(100,100);
	infoLayout->addWidget(eventLbl,0,0); 	
	infoLayout->addWidget(eventsWidget,1,0);

	peopleWidget		= new QTreeWidget;
	//people->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	//people->setMinimumSize(100,100);
	infoLayout->addWidget(peopleLbl,0,1);
	infoLayout->addWidget(peopleWidget,1,1);

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

		fvaBuildPeopleFilterTree(this, peopleWidget, false, rootSWdir);

		fvaBuildEventTree(this, eventWidget, rootSWdir);
	}
	LOG_DEB << "FVAOrganizerDevicePage::setVisible before exit";
	return QWizardPage::setVisible(visible);
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
