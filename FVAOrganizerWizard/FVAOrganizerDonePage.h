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

/*!
* \brief FVAOrganizerDonePage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. "Last words" to user in a QTextBrowser;
*
* This class impelements "View" functions from MVC pattern.
*/
class FVAOrganizerDonePage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerDonePage();

	private:

		/*!
		* \brief to tell the user the "Last words"
		*/
		QTextBrowser*	finishWords;
};

#endif // _FVA_ORG_DONE_PAGE_H_
