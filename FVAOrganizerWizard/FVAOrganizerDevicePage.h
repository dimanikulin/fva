/*!
* \file FVAOrganizerDevicePage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_DEVICE_PAGE_H_
#define _FVA_ORG_DEVICE_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLabel;
class QLineEdit;
class QTextBrowser;
class QPushButton;
class QComboBox;

/*!
* \brief FVAOrganizerDevicePage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. QTextBrowser to output the logging events;
* 2. QLineEdit to show device name;
* 3. QLineEdit to show matched device name;
* 4. QLineEdit to show owner name;
* 5. QComboBox to show a list of all devices for user to select;
* 6. QPushButton to call dictionary editor
*
* This class implements "View" functions from MVC pattern.
* Showing the elements and flow is based on class FvaConfiguration.
*/
class FVAOrganizerDevicePage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerDevicePage();

protected:

	/*!
	* \brief calls a PerformChecksForInputDir of class FVAFlowController
	* \return it returns nothing
	*/
	virtual bool	validatePage();

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

protected slots:

	/*!
	* \brief to call dictionary editor
	* \return it returns nothing
	*/
	void OnChangeDictPressed();

private:

	/*!
	* \brief device id matched
	*/
	int				deviceId;

	/*!
	* \brief to output the logging events
	*/
	QTextBrowser*	logOutput;

	/*!
	* \brief to show device name text
	*/
	QLabel*			deviceLbl;

	/*!
	* \brief to show device name
	*/
	QLineEdit*		deviceName;

	/*!
	* \brief to show matched device name text
	*/
	QLabel*			matchLbl;

	/*!
	* \brief to show matched device name
	*/
	QLineEdit*		matchName;

	/*!
	* \brief to show owner name text
	*/
	QLabel*			ownerLbl;

	/*!
	* \brief to show owner name
	*/
	QLineEdit*		ownerName;

	/*!
	* \brief to show a list of all devices for user to select
	*/
	QComboBox*		cbDevice;

	/*!
	* \brief to call dictionary editor
	*/
	QPushButton*	btnDct;
};


#endif // _FVA_ORG_DEVICE_PAGE_H_
