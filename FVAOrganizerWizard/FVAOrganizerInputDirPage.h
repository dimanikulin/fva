/*!
* \file FVAOrganizerInputDirPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_INPUT_PAGE_H_
#define _FVA_ORG_INPUT_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLabel;
class QLineEdit;
class QTextBrowser;
class QPushButton;

class FVAOrganizerInputDirPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerInputDirPage();
	virtual ~FVAOrganizerInputDirPage(){};

protected:

	virtual bool validatePage();
	virtual bool isComplete() const;

	private slots:
	void OnDirButtonClicked();

private:

	QLabel*			inputDirLabel;
	QLineEdit*		inputDirLineEdit;
	QTextBrowser*	logOutput;
	QPushButton*	dirButton;
};
#endif // _FVA_ORG_INPUT_PAGE_H_