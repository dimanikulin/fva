/*!
* \file fvaorganizerwizard.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "fvaorganizerwizard.h"

#include "fvacommonui.h"
#include "fvalogger.inl"

#include "FVAOrganizerDonePage.h"
#include "FVAOrganizerStartPage.h"
#include "FVAOrganizerOrientPage.h"
#include "FVAOrganizerInputDirPage.h"
#include "FVAOrganizerDevicePage.h"
#include "FVAOrganizerOutputDirPage.h"


FVAOrganizerWizard::FVAOrganizerWizard(QWidget *parent)
	: QWizard(parent)
{
	FvaConfiguration cfg;
        LOG_QDEB << "FVAOrganizerWizard construction" ;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.load")
	int index = 0;
	
        setPage(index++, new FVAOrganizerStartPage);

        LOG_QDEB << "FVAOrganizerStartPage created";
	setPage(index++, new FVAOrganizerInputDirPage);
	LOG_QDEB << "FVAOrganizerInputDirPage created";
        // do we need to show OrientPage?
	bool temp;
	exitCode = cfg.getParamAsBoolean("Common::CheckOrientation", temp);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.getParamAsBoolean")
	if (temp)
		setPage(index++, new FVAOrganizerOrientPage);

	LOG_QDEB << "FVAOrganizerOrientPage created";
	// do we need to show device page?
	exitCode = cfg.getParamAsBoolean("Search::Device", temp);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.getParamAsBoolean")
	if (temp)
		setPage(index++, new FVAOrganizerDevicePage);
        LOG_QDEB << "FVAOrganizerDevicePage created";
	setPage(index++, new FVAOrganizerOutputDirPage);
	LOG_QDEB << "FVAOrganizerOutputDirPage created";
        setPage(index++, new FVAOrganizerDonePage);
	
	setStartId( 0 );

#ifndef Q_OS_MACX
	setWizardStyle(ModernStyle);    
#endif

	setOption(QWizard::IndependentPages);
	setOption(QWizard::DisabledBackButtonOnLastPage);

	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);

#ifdef  FVA_LANGUAGE_RUS
	setWindowTitle(tr("Cистема организации медиа-контента"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	setWindowTitle(tr("FVA Software"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

        LOG_QDEB << "FVAOrganizerWizard constructed"; 
}