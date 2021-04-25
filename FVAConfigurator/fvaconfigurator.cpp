#include "FVAConfigurator.h"
#include "fvadefaultcfg.h"

#include <QTWidgets/QTabWidget>
#include <QTWidgets/QDialogButtonBox>
#include <QTWidgets/QVBoxLayout>


FVAConfigurator::FVAConfigurator(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	tabs = new QTabWidget();
	tabs->setFixedSize(600, 500);

	QWidget *tabCommon = new QWidget();

	QWidget *tabFilters = new QWidget();

	buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabs);
	mainLayout->addWidget(buttons);
	setLayout(mainLayout);

#ifdef  FVA_LANGUAGE_RUS
	tabs->addTab(tabCommon, tr("Общие"));
	tabs->addTab(tabFilters, tr("Фильтры"));
	setWindowTitle(tr("Конфигуратор"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	tabs->addTab(tabCommon, tr("Common"));
	tabs->addTab(tabFilters, tr("Filters"));
	setWindowTitle(tr("Configurator"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

}
