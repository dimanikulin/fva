/*!
* \file FVAOrganizerDonePage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_DONE_PAGE_H_
#define _FVA_ORG_DONE_PAGE_H_

#include <QtWidgets/QWizardPage>

class QTextBrowser;

class FVAOrganizerDonePage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerDonePage();
		virtual ~FVAOrganizerDonePage(){};

	private:

		QTextBrowser*	finishWords;

};

#endif // _FVA_ORG_DONE_PAGE_H_
