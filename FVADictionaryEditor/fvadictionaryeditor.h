#ifndef FVADICTIONARYEDITOR_H
#define FVADICTIONARYEDITOR_H

#include <QtGui/QDialog>
#include "ui_fvadictionaryeditor.h"

class FVADictionaryEditor : public QDialog
{
	Q_OBJECT

public:
	FVADictionaryEditor(const QString&  dictPath, const QString& device ,QWidget *parent = 0);
	~FVADictionaryEditor();

	protected slots:
		void OnAddPersonBtnPressed();
		void OnAddPlaceBtnPressed();
		void OnAddDeviceBtnPressed();

	private:

		Ui::FVADictionaryEditorClass	ui;
		const QString					m_dictPath;
		const QString					m_device;
};

#endif // FVADICTIONARYEDITOR_H
