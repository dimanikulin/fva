/*!
* \file FVAOrganizerStartPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerStartPage.h"
#include "fvaconstants.h"
#include "fvalogger.inl"

#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

FVAOrganizerStartPage::FVAOrganizerStartPage()
{
        LOG_DEB << "FVAOrganizerStartPage construction" ;

	helloWords = new QTextBrowser;
	cfgButton = new QPushButton;

#ifdef  FVA_LANGUAGE_RUS
	helloWords->setText(tr("Добро пожаловать в систему организации медиа-контента\n Нажмите кнопку \"Следующий\" для старта или \n \"Настройка\" для конфигурации!"));
	cfgButton->setText(tr("Настройка"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	helloWords->setText(tr("Welcome into the multimedia content system!\nPlease press button \"Next\" to continue or \"Configure\" for configuration"));
	cfgButton->setText(tr("Configure"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(helloWords);
	layout->addWidget(cfgButton);

	setLayout(layout);

	connect(cfgButton, SIGNAL(clicked()), this, SLOT(OnCfgBtnPressed()));

        LOG_DEB << "FVAOrganizerStartPage constructed" ;
}

void FVAOrganizerStartPage::OnCfgBtnPressed()
{
	QProcess myProcess(this);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVAConfigurator.exe");
	myProcess.waitForFinished(-1);
}
