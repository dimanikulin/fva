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
class QRadioButton;

// TODO to update
/*!
* \brief FVAOrganizerEventCfgPage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*  
* 1. QPushButton to fill up the fva Information;
*
* This class implements "View" functions from MVC pattern.
* Showing the elements and flow is based on class FvaConfiguration.
*/
class FVAOrganizerEventCfgPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerEventCfgPage();

	protected:

		/*!
		* \brief calls a PerformChecksForInputDir of class FVAFlowController
		* \return it returns nothing
		*/
		virtual bool	validatePage ();

		/*!
		* \brief it validates the input data on the page
		* \return it returns nothing
		*/
		virtual bool	isComplete() const;

		/*!
		* \brief it prepares the page before make it visible
		* \return it returns nothing
		*/
		virtual void	setVisible(bool visible);

	private slots:

		/*!
		* \brief to call descsiption editor
		* \return it returns nothing
		*/
		void OnFvaInfoButtonPressed();

	private:


		/*!
		* \brief to fill up the fva Information
		*/
		QPushButton*	fvaInfoButton;

};

#endif // _FVA_ORG_EVENT_CFG_H_