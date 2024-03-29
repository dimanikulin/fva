/*!
* \file FVAOrganizerOrientPage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerOrientPage.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include "fvalogger.inl"

FVAOrganizerOrientPage::FVAOrganizerOrientPage()
{
        LOG_DEB << "construction" ;
	// to suggest user to run /jpegr_portable32/jpegr.exe
#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	rotateLabel		= new QLabel(tr("Please check the content orientation before start:"));
	rotateLabel->setAlignment(Qt::AlignLeft);
	rotateButton	= new QPushButton;
	rotateButton->setText(tr("Check"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout * layout = new QVBoxLayout;
	layout->addWidget(rotateLabel);
	layout->addWidget(rotateButton);

	connect( rotateButton, SIGNAL( clicked() ), this, SLOT( OnOrientationButtonClicked() ) );

	setLayout(layout);
        LOG_DEB << "constructed" ;
}

void FVAOrganizerOrientPage::OnOrientationButtonClicked()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(QCoreApplication::applicationDirPath() + "/jpegr/jpegr.exe");
	myProcess.waitForFinished(-1);
}
