#ifndef FVADESCRIPTIONEDITOR_H
#define FVADESCRIPTIONEDITOR_H

#include <QtGui/QMainWindow>
#include "ui_fvadescriptioneditor.h"

class FVADescriptionEditor : public QMainWindow
{
	Q_OBJECT

public:

	FVADescriptionEditor( const QStringList&				titles, 
							QMap< QString, QStringList >&	decsItems, 
						const QVariantMap&					dictionaries,
						int									indexOfFile,
						const QStringList&					files,
						QWidget *							parent = 0);
	~FVADescriptionEditor();

protected slots:
	void OnAddBtnPressed();
	void OnRemoveBtnPressed();
	void OnNextBtnPressed();
	void OnPrevBtnPressed();
	void OnSaveBtnPressed();

private: // methods

	void updateGuiForFile( const QString& path );

	void saveCurrentDescription();

private: // data

	Ui::FVADescriptionEditorClass ui;

	const QStringList&					m_titles; 
		QMap< QString, QStringList >&	m_decsItems; 
	const QVariantMap&					m_dictionaries;
	int									m_indexOfFile;
	const QStringList&					m_files;
};

#endif // FVADESCRIPTIONEDITOR_H
