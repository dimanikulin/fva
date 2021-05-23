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

/*!
* \brief FVAOrganizerInputDirPage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. QLineEdit for the input directory to get the Multimedia Data from;
* 2. QTextBrowser to output the logging events;
* 3. QPushButton to open QFileDialog to select input directory.
*
* This class impelements "View" functions from MVC pattern.
*/
class FVAOrganizerInputDirPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerInputDirPage();

protected:

	/*!
	* \brief calls a PerformChecksForInputDir of class FVAFlowController
	* \return it returns nothing
	*/
	virtual bool validatePage();

	/*!
	* \brief it validates the input data on the page
	* \return it returns nothing
	*/
	virtual bool isComplete() const;

private slots:


	/*!
	* \brief opens QFileDialog to select input directory
	* \return it returns nothing
	*/
	void OnDirButtonClicked();

private:

	/*!
	* \brief to say where selected input directory
	*/
	QLabel*			inputDirLabel;

	/*!
	* \brief to keep selected input directory
	*/
	QLineEdit*		inputDirLineEdit;

	/*!
	* \brief to output the logging events
	*/
	QTextBrowser*	logOutput;

	/*!
	* \brief to open QFileDialog to select input directory
	*/
	QPushButton*	dirButton;
};
#endif // _FVA_ORG_INPUT_PAGE_H_