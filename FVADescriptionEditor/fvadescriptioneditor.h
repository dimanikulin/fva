#ifndef FVADESCRIPTIONEDITOR_H
#define FVADESCRIPTIONEDITOR_H

#include <QtWidgets/QMainWindow>
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
	void OnChangeDictPressed();
	void updateChecks(QTreeWidgetItem*, int);*/

private: // methods

	void updateGuiForItem( const QString& path );

	void saveCurrentDescription();

	void updateDictionaryGUI();

private: // data

	Ui::FVADescriptionEditorClass		ui;

	bool								m_forFolder;
};

#endif // FVADESCRIPTIONEDITOR_H
