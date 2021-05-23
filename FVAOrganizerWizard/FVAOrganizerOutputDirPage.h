/*!
* \file FVAOrganizerOutputDirPage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_OUTPUT_PAGE_H_
#define _FVA_ORG_OUTPUT_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLineEdit;
class QCheckBox;

/*!
* \brief FVAOrganizerOutputDirPage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. QLineEdit for the digiKam directory to put the Multimedia Data in;
* 2. QLineEdit for the GooglePhoto directory to put the Multimedia Data in;
* 3. QCheckBox to remove origin (input) directory to get the Multimedia Data from;
* 
* Showing the elements and flow is based on class FvaConfiguration.
* This class impelements "View" functions from MVC pattern.
*/

class FVAOrganizerOutputDirPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerOutputDirPage();

	protected:

		/*!
		* \brief calls a MoveInputDirToOutputDirs of class FVAFlowController
		* \return it returns nothing
		*/
		virtual bool	validatePage ();

		/*!
		* \brief it validates the input data on the page 
		* \return it returns nothing
		*/
		virtual bool	isComplete() const;

	private slots:

		/*!
		* \brief opens QFileDialog to select DigiKam directory
		* \return it returns nothing
		*/
		void OnDigiKamDirButtonClicked();

		/*!
		* \brief opens QFileDialog to select GooglePhoto directory
		* \return it returns nothing
		*/
		void OnGooglePhotoDirButtonClicked();

	private: //data

		/*!
		* \brief to keep selected digiKam directory
		*/
		QLineEdit* digiKamLineEdit;

		/*!
		* \brief to keep selected GooglePhoto directory
		*/
		QLineEdit* googlePhotoLineEdit;

		/*!
		* \brief to ask users if they want to remove origin (input) directory
		*/
		QCheckBox* removeOriginDirCheckBox;
};

#endif // _FVA_ORG_OUTPUT_PAGE_H_