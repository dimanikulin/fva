/*!
* \file FVAOrganizerStartPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_START_PAGE_H_
#define _FVA_ORG_START_PAGE_H_

#include <QtWidgets/QWizardPage>

class QPushButton;
class QTextBrowser;

class FVAOrganizerStartPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerStartPage();
	virtual ~FVAOrganizerStartPage(){};


protected slots:
	void OnCfgBtnPressed();

private:

	QTextBrowser*	helloWords;

	QPushButton*	cfgButton;
};

#endif //_FVA_ORG_START_PAGE_H_