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
#include "FVAOrganizerEventCfgPage.h"
#include "FVAOrganizerEventInfoPage.h"
#include "FVAOrganizerPlacePage.h"

FVAOrganizerWizard::FVAOrganizerWizard(QWidget *parent)
	: QWizard(parent)
{
	FvaConfiguration cfg;
        LOG_DEB << "construction" ;
	FVA_EXIT_CODE exitCode = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.load")
	int index = 0;
	
        setPage(index++, new FVAOrganizerStartPage);
        LOG_DEB << "FVAOrganizerStartPage created";

	setPage(index++, new FVAOrganizerInputDirPage);
	LOG_DEB << "FVAOrganizerInputDirPage created";

        // do we need to show OrientPage?
	bool isSet;
	exitCode = cfg.getParamAsBoolean("Common::CheckOrientation", isSet);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.getParamAsBoolean")
	if (isSet)
	{
		setPage(index++, new FVAOrganizerOrientPage);
		LOG_DEB << "FVAOrganizerOrientPage created";
	}

	// do we need to show device page?
	exitCode = cfg.getParamAsBoolean("Search::Device", isSet);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.getParamAsBoolean")
	if (isSet)
	{
		setPage(index++, new FVAOrganizerDevicePage);
        	LOG_DEB << "FVAOrganizerDevicePage created";
	}

	// do we need to show event page?
	exitCode = cfg.getParamAsBoolean("Search::Event", isSet);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.getParamAsBoolean")
	if (isSet)
	{
		setPage(index++, new FVAOrganizerEventCfgPage);
        	LOG_DEB << "FVAOrganizerEventCfgPage created";

		setPage(index++, new FVAOrganizerEventInfoPage);
        	LOG_DEB << "FVAOrganizerEventInfoPage created";

	}

	// do we need to show place page?
	exitCode = cfg.getParamAsBoolean("Search::Place", isSet);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET("cfg.getParamAsBoolean")
	if (isSet)
	{
		setPage(index++, new FVAOrganizerPlacePage);
        	LOG_DEB << "FVAOrganizerPlacePage created";
	}

	setPage(index++, new FVAOrganizerOutputDirPage);
	LOG_DEB << "FVAOrganizerOutputDirPage created";

        setPage(index++, new FVAOrganizerDonePage);
	
	setStartId( 0 );
	//m_inputDir = "D:/fvaInput/#fromCanon"; // for debugging

#ifndef Q_OS_MACX
	setWizardStyle(ModernStyle);    
#endif

	setOption(QWizard::IndependentPages);
	setOption(QWizard::DisabledBackButtonOnLastPage);

	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);

#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	setWindowTitle(tr("FVA Software"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

        LOG_DEB << "constructed"; 
}