#include "fvapeoplelistdlg.h"
#include "fvacommonlib.h"
#include "fvacommondb.h"
#include <QtCore/QProcess>

FVAPeopleListDlg::FVAPeopleListDlg(QVariantMap& dictionaries, const QString& dictPath, QWidget *parent)
	: QDialog		(parent),
	m_dictionaries	(dictionaries),
	m_dictPath		(dictPath)
{
	ui.setupUi(this);

	updateGui();

	connect (ui.btnAddPeople,SIGNAL(clicked()),this,SLOT(OnAddPeopleBtnPressed()) );
}

FVAPeopleListDlg::~FVAPeopleListDlg()
{

}
void FVAPeopleListDlg::updateGui()
{
	ui.listWidget->clear();
	QVariantList people = m_dictionaries["people"].toList();
	for ( auto i = people.begin(); i != people.end() ; ++i )
	{
		QListWidgetItem *itm = new QListWidgetItem;
		itm->setText( i->toMap()["name"].toString() );
		itm->setData( 1, i->toMap()["ID"].toString() );
		ui.listWidget->addItem(itm);
	}
}
void FVAPeopleListDlg::OnAddPeopleBtnPressed()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(m_dictPath);
	myProcess.start("FVADictionaryEditor.exe", params);

	if ( !myProcess.waitForFinished( -1 ) )
	{
		return;
	}
	QString		error;
	FVA_ERROR_CODE res = fvaLoadDictionary( m_dictPath, m_dictionaries, error );
	RET_IF_RES_IS_ERROR

	updateGui();
}