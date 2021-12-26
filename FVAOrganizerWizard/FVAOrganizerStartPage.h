/*!
* \file FVAOrganizerStartPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_START_PAGE_H_
#define _FVA_ORG_START_PAGE_H_

#include <QWizardPage>

class QPushButton;
class QTextBrowser;

/*!
* \brief FVAOrganizerStartPage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. "Welcome words" to user in a QTextBrowser;
* 2. "Configuration" button (QPushButton) to start Configurator UI before System to use.
* 
* This class implements "View" functions from MVC pattern.
*/
class FVAOrganizerStartPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerStartPage();

protected slots:

	/*!
	* \brief it starts the Configurator UI
	* \return it returns nothing
	*/
	void OnCfgBtnPressed();

private:

	/*!
	* \brief to tell the user the "Welcome words"
	*/
	QTextBrowser*	helloWords;

	/*!
	* \brief to start the Configurator UI
	*/
	QPushButton*	cfgButton;
};

#endif //_FVA_ORG_START_PAGE_H_