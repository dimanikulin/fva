#ifndef FVAPEOPLELISTDLG_H
#define FVAPEOPLELISTDLG_H

#include <QtWidgets/QDialog>
#include "ui_fvapeoplelistdlg.h"

class FVAPeopleListDlg : public QDialog
{
	Q_OBJECT

public:
	FVAPeopleListDlg( QVariantMap& dictionaries, const QString& dictPath, QWidget *parent = 0);
	~FVAPeopleListDlg();

	Ui::FVAPeopleListDlg& getUI() { return ui; };

protected slots:

	void OnAddPeopleBtnPressed();

private:

	void updateGui();

	Ui::FVAPeopleListDlg ui;

	QVariantMap&	m_dictionaries; 
	QString			m_dictPath;

};

#endif // FVAPEOPLELISTDLG_H
