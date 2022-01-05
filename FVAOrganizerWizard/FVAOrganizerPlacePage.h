/*!
* \file FVAOrganizerPlacePage.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_ORG_PLACE_PAGE_H_
#define _FVA_ORG_PLACE_PAGE_H_

#include <QWizardPage>
#include "FVAFlowController.h"

class QPushButton;
class QLabel;
class QTextBrowser;
class QTreeWidget;
class QTreeWidgetItem;

/*!
* \brief FVAOrganizerPlacePage is a child of <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a> and implements the next UI functions:
*
* 1. "Explanation words" to user in a QLabel;
* 2. The events(aka dirs) we need to add information about, in form of a QTreeWidget;
* 3. "Open dir" button (QPushButton) to open input dir where processed files are in to recall their content.
* 4. "Save" button (QPushButton) to save information user provided
* 5. "Add Place" button (QPushButton) to add new place
* 6. QTreeWidget to set up Location using dictionary
* 7. QTextBrowser to output the logging events;
* 
* This class implements "View" functions from MVC pattern.
*/
class FVAOrganizerPlacePage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerPlacePage();

	protected:

		/*!
		* \brief calls a TODO FIX of class FVAFlowController
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

		/*!
		* \brief to save information user provided
		* \return it returns nothing
		*/
		void OnSaveButtonPressed();

		/*!
		* \brief to call dictionary editor
		* \return it returns nothing
		*/
		void OnAddPlacePressed();

		/*!
		* \brief it updates hierachy of checks around this QTreeWidgetItem
		* \return it returns nothing
		*/
		void updateChecks(QTreeWidgetItem*, int);


	private:
	
		/*!
		* \brief to tell a user what to do here
		*/
		QLabel*			words;

		/*!
		* \brief to open input dir where processed files are in
		*/
		QPushButton*		inputDirButton;

		/*!
		* \brief to save information user provided  
		*/
		QPushButton*		saveButton;

		/*!
		* \brief to add new event
		*/
		QPushButton*		addNewPlaceButton;

		/*!
		* \brief to output the logging events
		*/
		QTextBrowser*		logOutput;
		
		/*!
		* \brief to output the input dirs structure 
		*/
		QTreeWidget*		inputDirsWidget;			

		/*!
		* \brief to output the event structure 
		*/
		QTreeWidget*		placesWidget;	
		
		/*!
		* \brief maps input dir structure (folder names) to place id got from fvaPlaces.csv
		*/
		DIR_2_ID_MAP		dir2PlaceMap;

};


#endif //_FVA_ORG_PLACE_PAGE_H_
