/*!
* \file FVAOrganizerOrientPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_ORIENT_PAGE_H_
#define _FVA_ORG_ORIENT_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLabel;
class QPushButton;

class FVAOrganizerOrientPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerOrientPage();
	virtual ~FVAOrganizerOrientPage(){};

	private slots:

	void OnOrientationButtonClicked();

private: // data 

	QLabel*			rotateLabel;
	QPushButton*	rotateButton;

};
#endif // _FVA_ORG_ORIENT_PAGE_H_
