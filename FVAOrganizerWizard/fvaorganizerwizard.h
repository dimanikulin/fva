/*!
* \file FVAOrganizerWizard.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef FVAORGANIZERWIZARD_H
#define FVAORGANIZERWIZARD_H

#include <QtWidgets/QWizard>
#include "ui_fvaorganizerwizard.h"

#include "fvacommonlib.h"

/*!
* \brief FVAOrganizerWizard is child of <a href="https://doc.qt.io/qt-5/qwizard.html">QWizard</a> and implements the flow with a User to interact.
*
* The flow consists of following steps:
* 1. Start Page - class FVAOrganizerStartPage;
* 2. Input Dir Page - class FVAOrganizerInputDirPage;
* 3. Orient Page - class FVAOrganizerOrientPage;
* 4. Device Page - class FVAOrganizerDevicePage;
* 5. Output Dir Page - class FVAOrganizerOutputDirPage;
* 6. Done Page - class FVAOrganizerDonePage.
* 
* Each page/step is a separate class inherited from <a href="https://doc.qt.io/qt-5/qwizardpage.html">QWizardPage</a>.  
* Showing the elements and flow is based on class FvaConfiguration.
* This class implements "View" functions from MVC pattern.
*/
class FVAOrganizerWizard : public QWizard
{
	Q_OBJECT

	public:
		FVAOrganizerWizard(QWidget *parent = 0);

		/*!
		* \brief it sets up input directory to work with
		* \param inputFolder - input directory to work with  
		* \return it returns nothing
		*/
		void inputFolder(const QString& inputFolder) { m_inputDir = inputFolder;}

		/*!
		* \brief it returns an input directory to work with
		* \return it returns an input directory to work with
		*/
		QString inputFolder(){return m_inputDir;}

		/*!
		* \brief it sets up a device name found in input folder as a single device that all photos were taken by 
		* \param deviceName - device name to work with  
		* \return it returns nothing
		*/
		void matchedDeviceName(const QString& deviceName) { m_deviceName = deviceName;}

		/*!
		* \brief it returns a device name found in input folder as a single device that all photos were taken by 
		* \return it returns a device name found in input folder as a single device that all photos were taken by 
		*/
		QString matchedDeviceName(){return m_deviceName;}

        /*!
		* \brief it sets up a full device map loaded from dictionary
		* \param fullDeviceMap - a full device map loaded from dictionary
		* \return it returns nothing
		*/
		void fullDeviceMap(const DEVICE_MAP& fullDeviceMap) {m_fullDeviceMap = fullDeviceMap;}

		/*!
		* \brief it returns a full device map loaded from dictionary
		* \return it returns a full device map loaded from dictionary
		*/
		DEVICE_MAP fullDeviceMap() {return m_fullDeviceMap;}
		
		/*!
		* \brief it sets up a full device map loaded from dictionary
		* \param matchedDeviceMap - a full device map loaded from dictionary  
		* \return it returns nothing
		*/
		void matchedDeviceMap(const DEVICE_MAP& matchedDeviceMap) {m_matchedDeviceMap = matchedDeviceMap;}

		/*!
		* \brief it returns matched device map that matches a device for this directory
		* \return it returns matched device map that matches a device for this directory
		*/
		DEVICE_MAP matchedDeviceMap() {return m_matchedDeviceMap;}

		/*!
		* \brief it sets up input dir type that is returned as exit code
		* \param inputDirType - input dir type that is returned as exit code  
		* \return it returns nothing
		*/
		void inputDirType(FVA_EXIT_CODE	inputDirType) { m_inputDirType = inputDirType; }

		/*!
		* \brief it returns an input dir type
		* \return it returns an input dir type
		*/
		FVA_EXIT_CODE inputDirType() { return m_inputDirType; }

		/*!
		* \brief it sets up an identifier of device that matches
		* \param matchedDeviceID - an identifier of device that matches  
		* \return it returns nothing
		*/
		void matchedDeviceID(int matchedDeviceID) { m_matchedDeviceID = matchedDeviceID; }

		/*!
		* \brief it returns an identifier of device that matches
		* \return it returns an identifier of device that matches
		*/
		int matchedDeviceID() { return m_matchedDeviceID; }

	private:

		/*!
		* \brief user interface QT implementation to work with
		*/
		Ui::FVAOrganizerWizardClass ui;

		/*!
		* \brief input directory to work with
		*/
		QString			m_inputDir;

		/*!
		* \brief a device name found in input folder as a single device that all photos were taken by
		*/
		QString			m_deviceName;

		/*!
		* \brief a full device map loaded from dictionary
		*/
		DEVICE_MAP		m_fullDeviceMap;

		/*!
		* \brief a matched device map that matches a device for this directory
		*/
		DEVICE_MAP		m_matchedDeviceMap;

		/*!
		* \brief a dir type is returned as exit code
		*/
		FVA_EXIT_CODE	m_inputDirType;  

		/*!
		* \brief an identifier of device that matches
		*/
		int				m_matchedDeviceID;
};

#endif // FVAORGANIZERWIZARD_H
