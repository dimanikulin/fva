#ifndef FVAPEOPLELISTDLG_H
#define FVAPEOPLELISTDLG_H

#include <QDialog>
#include "ui_fvapeoplelistdlg.h"

class FVAPeopleListDlg : public QDialog
{
	Q_OBJECT

public:
	FVAPeopleListDlg(const QVariantList& people, QWidget *parent = 0);
	~FVAPeopleListDlg();

	Ui::FVAPeopleListDlg& getUI() { return ui; };
private:
	Ui::FVAPeopleListDlg ui;
};

#endif // FVAPEOPLELISTDLG_H
