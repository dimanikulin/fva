/*!
* \file FVADictionaryEditor.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef FVADICTIONARYEDITOR_H
#define FVADICTIONARYEDITOR_H

#include <QDialog>
#include "ui_fvadictionaryeditor.h"

#include "fvaconfiguration.h"

/*!
* \brief FVADictionaryEditor is a child of <a href="https://doc.qt.io/qt-5/qdialog.html">QDialog</a> and implements the next UI functions:
*
* 1. adding a new person to a dictionary;
* 2. adding a new place to a dictionary;
* 3. adding a new device to a dictionary.
* 4. adding a new event to a dictionary.
*
*/
class FVADictionaryEditor : public QDialog
{
	Q_OBJECT

public:
	FVADictionaryEditor(const QString& device ,QWidget *parent = 0);
	
private slots:
		/*!
		* \brief to add a new person to a dictionary
		* \return it returns nothing
		*/
		void OnAddPersonBtnPressed();

		/*!
		* \brief to add a new place to a dictionary
		* \return it returns nothing
		*/
		void OnAddPlaceBtnPressed();

		/*!
		* \brief to add a new device to a dictionary
		* \return it returns nothing
		*/
		void OnAddDeviceBtnPressed();

		/*!
		* \brief to add a new event to a dictionary
		* \return it returns nothing
		*/
		void OnAddEventBtnPressed();

	private:
		/*!
		* \brief user interface QT implementation to work with
		*/
		Ui::FVADictionaryEditorClass	ui;

		/*!
		* \brief devive to be added as new one to a ditionary
		* \return it returns nothing
		*/
		const QString					m_device;

		/*!
		* \brief application wide settings
		*/
		FvaConfiguration				cfg;
};

#endif // FVADICTIONARYEDITOR_H
