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

class FVAOrganizerEventCfgPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerEventCfgPage();
		virtual ~FVAOrganizerEventCfgPage(){};

	protected:

		virtual bool	validatePage ();
		virtual bool	isComplete() const;
		virtual void	setVisible(bool visible);

	private slots:
		void OnFvaInfoButtonPressed();

	private:

		QRadioButton*	oneEventOneDay;
		QPushButton*	fvaInfoButton;
		QRadioButton*	severalEventsOneDay;
		QRadioButton*	severalEventsSeveralDays;
		QRadioButton*	oneEventSeveralDays;
};

#endif // _FVA_ORG_EVENT_CFG_H_