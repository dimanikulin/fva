#ifndef FVADESCRIPTIONEDITOR_H
#define FVADESCRIPTIONEDITOR_H

#include <QtGui/QMainWindow>
#include "ui_fvadescriptioneditor.h"

class FVADescriptionEditor : public QMainWindow
{
	Q_OBJECT

public:
	FVADescriptionEditor( const QStringList&				titles, 
						const QMap< QString, QStringList >&	decsItems, 
						const QVariantMap&					dictionaries,
						const QString&						file,
						QWidget *							parent = 0);
	~FVADescriptionEditor();

private:
	Ui::FVADescriptionEditorClass ui;

	const QStringList&					m_titles; 
	const QMap< QString, QStringList >&	m_decsItems; 
	const QVariantMap&					m_dictionaries;
	const QString&						m_file;
};

#endif // FVADESCRIPTIONEDITOR_H
