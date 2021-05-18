/*!
* \file FVADictionaryEditor.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef FVADICTIONARYEDITOR_H
#define FVADICTIONARYEDITOR_H

#include <QtWidgets/QDialog>
#include "ui_fvadictionaryeditor.h"

#include "fvaconfiguration.h"

class FVADictionaryEditor : public QDialog
{
	Q_OBJECT

public:
	FVADictionaryEditor(const QString&  dictPath, const QString& device ,QWidget *parent = 0);
	virtual ~FVADictionaryEditor();

	private slots:
		void OnAddPersonBtnPressed();
		void OnAddPlaceBtnPressed();
		void OnAddDeviceBtnPressed();

	private:

		Ui::FVADictionaryEditorClass	ui;
		const QString					m_dictPath;
		const QString					m_device;

		FvaConfiguration				cfg;
};

#endif // FVADICTIONARYEDITOR_H
