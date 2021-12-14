/*!
* \file FVAOrganizerEventCfgPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerEventCfgPage.h"

#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include <QtCore/QProcess>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvalogger.inl"
#include "FVAFlowController.h"

FVAOrganizerEventCfgPage::FVAOrganizerEventCfgPage(void)
{
        LOG_DEB << "FVAOrganizerEventCfgPage construction" ;
#ifdef  FVA_LANGUAGE_RUS
	fvaInfoButton = new QPushButton;
	fvaInfoButton->setText(tr("Добавить инфо"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	fvaInfoButton = new QPushButton;
	fvaInfoButton->setText(tr("Add info"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
    	
	QVBoxLayout * layout = new QVBoxLayout;

	QGridLayout * oneEventOneDayLayout = new QGridLayout;
	//oneEventOneDayLayout->addWidget(oneEventOneDay, 0, 0);
	oneEventOneDayLayout->addWidget(fvaInfoButton, 0, 1);
	layout->addLayout(oneEventOneDayLayout);

	setLayout(layout);

	connect(fvaInfoButton, SIGNAL(clicked()), this, SLOT(OnFvaInfoButtonPressed()));

        LOG_DEB << "FVAOrganizerEventCfgPage constructed" ;
}
void FVAOrganizerEventCfgPage::OnFvaInfoButtonPressed()
{
	QProcess myProcess(this);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	// TODO will work only for win OS
	//myProcess.start(QCoreApplication::applicationDirPath() + "/FVADescriptionEditor.exe");
	params.append(((FVAOrganizerWizard*)wizard())->inputFolder());
	myProcess.start("explorer");
	myProcess.waitForFinished(-1);
}
void FVAOrganizerEventCfgPage::setVisible(bool visible)
{
        LOG_DEB << "FVAOrganizerEventCfgPage set visible" ;
	return QWizardPage::setVisible(visible);
}

bool FVAOrganizerEventCfgPage::isComplete() const
{
	return true;
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
