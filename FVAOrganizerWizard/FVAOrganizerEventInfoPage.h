/*!
* \file FVAOrganizerEventInfoPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_EVENT_INFO_H_
#define _FVA_ORG_EVENT_INFO_H_

#include <QtWidgets/QWizardPage>

class QPushButton;
class QLabel;
class QTextBrowser;
class QTreeWidget;

/*!
* \brief FVAOrganizerEventInfoPage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. "Explanation words" to user in a QLabel;
* 2. The events(aka dirs) we need to add information about, in form of a QTreeWidget;
* 3. "Open dir" button (QPushButton) to open input dir where processed files are in to recall their content.
* 4. a QTreeWidget to set up Event Type
* 5. a QTreeWidget to set up the people list because this event happened of
* 6. QTextBrowser to output the logging events;
* This class implements "View" functions from MVC pattern.
*/

class FVAOrganizerEventInfoPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerEventInfoPage();

	protected:

		/*!
		* \brief calls a ProcessInputDirForEventInfo of class FVAFlowController
		* \return it returns nothing
		*/
		virtual bool validatePage();

		/*!
		* \brief it prepares the page before make it visible
		* \return it returns nothing
		*/
		virtual void setVisible(bool visible);

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
		
		/*!
		* \brief to output the input dirs structure 
		*/
		QTreeWidget*	inputDirs;			

		/*!
		* \brief to output the structure of people caused the event 
		*/
		QTreeWidget*	people;

		/*!
		* \brief to output the event structure 
		*/
		QTreeWidget*	events;	
			
};

#endif // _FVA_ORG_EVENT_INFO_H_