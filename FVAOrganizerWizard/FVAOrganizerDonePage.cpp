/*!
* \file FVAOrganizerDonePage.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "FVAOrganizerDonePage.h"
#include "fvaconstants.h"
#include "fvalogger.inl"

#include <QVBoxLayout>
#include <QTextBrowser>

FVAOrganizerDonePage::FVAOrganizerDonePage(void)
{
        LOG_DEB << "construction" ;
	finishWords		= new QTextBrowser;

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(finishWords);

	setLayout(layout);

#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	finishWords->setText(tr("Congratulations, you are ready to finish."));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
        LOG_DEB << "constructed" ;
}

