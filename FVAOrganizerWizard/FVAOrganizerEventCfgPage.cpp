/*!
* \file FVAOrganizerEventCfgPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerEventCfgPage.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

#include <QtCore/QProcess>
#include <QUrl>
#include <QDesktopServices>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvalogger.inl"
#include "FVAFlowController.h"

FVAOrganizerEventCfgPage::FVAOrganizerEventCfgPage(void)
{
        LOG_DEB << "FVAOrganizerEventCfgPage construction" ;
#ifdef  FVA_LANGUAGE_RUS
	words	= new QLabel(tr("Пожалуйста, откройте входную папку\nи объедините однодневные папки в одну,\nесли они относятся к одному событию.\n Так проделайте для каждого события"));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Открыть папку"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	words = new QLabel(tr("Please open an input folder with\n and merge one-day dirs into one dir,\nif they are related to the same event.\n Please do so for each event"));
	inputDirButton = new QPushButton;
	inputDirButton->setText(tr("Open a folder"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
			
	QGridLayout * dirLayout = new QGridLayout;
	dirLayout->addWidget(words,0,0);
	dirLayout->addWidget(inputDirButton,0,1);

	logOutput		= new QTextBrowser;
	
	QVBoxLayout * layout = new QVBoxLayout;

	layout->addLayout(dirLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect( inputDirButton, SIGNAL( clicked() ), this, SLOT( OnDirButtonClicked() ) );
        LOG_DEB << "FVAOrganizerEventCfgPage constructed" ;

}
void FVAOrganizerEventCfgPage::OnFvaInputDirButtonPressed()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(((FVAOrganizerWizard*)wizard())->inputFolder()));

	//QProcess myProcess(this);
	//myProcess.setProcessChannelMode(QProcess::MergedChannels);
	//QStringList params;
	// TODO will work only for win OS
	//myProcess.start(QCoreApplication::applicationDirPath() + "/FVADescriptionEditor.exe");
	//params.append(((FVAOrganizerWizard*)wizard())->inputFolder());
	//myProcess.start("explorer",params);
	//myProcess.waitForFinished(-1);
}
bool	FVAOrganizerEventCfgPage::validatePage()
{
        LOG_DEB << "FVAOrganizerEventCfgPage validate page" ;
	FVAFlowController flow;
	FVA_EXIT_CODE exitCode = flow.ProcessInputDirForEventCfg(((FVAOrganizerWizard*)wizard())->inputFolder(), "TODO"/*, this*/);
	if (exitCode != FVA_NO_ERROR)
		return false;

	return true;
}
