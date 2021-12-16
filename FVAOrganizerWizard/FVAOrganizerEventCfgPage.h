/*!
* \file FVAOrganizerEventCfgPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_EVENT_CFG_H_
#define _FVA_ORG_EVENT_CFG_H_

#include <QtWidgets/QWizardPage>

class QPushButton;
class QLabel;
class QTextBrowser;


/*!
* \brief FVAOrganizerEventCfgPage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. "Explanation words" to user in a QLabel;
* 2. "See input dir" button (QPushButton) to open input dir where processed files are in.
* 3. QTextBrowser to output the logging events;
* This class implements "View" functions from MVC pattern.
*/

class FVAOrganizerEventCfgPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerEventCfgPage();


	private slots:

		/*!
		* \brief to open input dir where processed files are in
		* \return it returns nothing
		*/
		void OnFvaInputDirButtonPressed();

	private:
	
		/*!
		* \brief to tell a user what to do here
		*/
		QLabel*		words;

		/*!
		* \brief to open input dir where processed files are in
		*/
		QPushButton*	inputDirButton;

		/*!
		* \brief to output the logging events
		*/
		QTextBrowser*	logOutput;

};

#endif // _FVA_ORG_EVENT_CFG_H_