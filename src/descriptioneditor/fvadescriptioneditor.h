/*!
* \file FVADescriptionEditor.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef FVADESCRIPTIONEDITOR_H
#define FVADESCRIPTIONEDITOR_H

#include <QMainWindow>
#include "ui_fvadescriptioneditor.h"

class FVADescriptionEditor : public QMainWindow
{
	Q_OBJECT

	public:
		FVADescriptionEditor(bool forFolder, QWidget* parent = 0);
		virtual ~FVADescriptionEditor();

protected slots:
	/*void OnNextBtnPressed();
	void OnPrevBtnPressed();
	void OnSaveBtnPressed();
	void OnChangeDictPressed();*/
	void updateChecks(QTreeWidgetItem*, int);

private: // methods
	void updateGuiForItem( const QString& path );

	void saveCurrentDescription();

	void updateDictionaryGUI();

private: // data
	Ui::FVADescriptionEditorClass		ui;

	bool					m_forFolder;
};

#endif // FVADESCRIPTIONEDITOR_H
