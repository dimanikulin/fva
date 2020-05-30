#include "fvapeoplelistdlg.h"

FVAPeopleListDlg::FVAPeopleListDlg(const QVariantList& people,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	for ( auto i = people.begin(); i != people.end() ; ++i )
	{
		QListWidgetItem *itm = new QListWidgetItem;
		itm->setText( i->toMap()["name"].toString() );
		itm->setData( 1, i->toMap()["ID"].toString() );
		ui.listWidget->addItem(itm);
	}
}

FVAPeopleListDlg::~FVAPeopleListDlg()
{

}
