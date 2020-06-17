#ifndef FVADESCRIPTIONEDITOR_H
#define FVADESCRIPTIONEDITOR_H

#include <QtGui/QMainWindow>
#include "ui_fvadescriptioneditor.h"

class FVADescriptionEditor : public QMainWindow
{
	Q_OBJECT

	public:

		FVADescriptionEditor( bool							forFolder,
						const QStringList&				titles, 
						QMap< QString, QStringList >&	decsItems, 
						const QVariantMap&				dictionaries,
						int								indexOfFile,
						const QStringList&				files,
						const QString&					dictPath,
						const QString&					folderPath,
						QWidget *						parent = 0);


		~FVADescriptionEditor();

protected slots:
	void OnAddBtnPressed();
	void OnRemoveBtnPressed();
	void OnNextBtnPressed();
	void OnPrevBtnPressed();
	void OnSaveBtnPressed();
	void OnChangeDictPressed();

private: // methods

	void updateGuiForItem( const QString& path );

	void saveCurrentDescription();

	void updateDictionaryGUI();

	void init();

private: // data

	Ui::FVADescriptionEditorClass ui;

	QVariantMap							m_dictionaries;
	QString								m_dictPath;
	bool								m_forFolder;
	QStringList							m_titles; 
	QMap< QString, QStringList >		m_decsItems; 
	int									m_indexOfFile;
	QStringList							m_files;
	QString								m_folderPath;

};

#endif // FVADESCRIPTIONEDITOR_H
