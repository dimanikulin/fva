#ifndef FVADICTIONARYEDITOR_H
#define FVADICTIONARYEDITOR_H

#include <QtGui/QDialog>
#include "ui_fvadictionaryeditor.h"

class FVADictionaryEditor : public QDialog
{
	Q_OBJECT

public:
	FVADictionaryEditor(const QString&  dictPath ,QWidget *parent = 0);
	~FVADictionaryEditor();

protected slots:
	void OnAddPersonBtnPressed();
	void OnAddPlaceBtnPressed();

	private:

		Ui::FVADictionaryEditorClass	ui;
		const QString					m_dictPath;
};

#endif // FVADICTIONARYEDITOR_H
