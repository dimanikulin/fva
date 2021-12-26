/*!
* \file FVAOrganizerOrientPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_ORIENT_PAGE_H_
#define _FVA_ORG_ORIENT_PAGE_H_

#include <QWizardPage>

class QLabel;
class QPushButton;

/*!
* \brief FVAOrganizerOrientPage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. QLabel to suggest a user to check orintation;
* 2. QPushButton to start "/jpegr/jpegr.exe" to check orintations.
* 
* Showing the elements and flow is based on class FvaConfiguration.
* This class implements "View" functions from MVC pattern.
*/

class FVAOrganizerOrientPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerOrientPage();

	private slots:

	/*!
	* \brief starts "/jpegr/jpegr.exe" to check orintations 
	* \return it returns nothing
	*/
	void OnOrientationButtonClicked();

private: // data 

	/*!
	* \brief to suggest a user to check orintation
	*/
	QLabel*		rotateLabel;

	/*!
	* \brief to start "/jpegr/jpegr.exe" to check orintations
	*/
	QPushButton*	rotateButton;
};
#endif // _FVA_ORG_ORIENT_PAGE_H_
