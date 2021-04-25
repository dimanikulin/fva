#include "FVAOrganizerStartPage.h"
#include "fvadefaultcfg.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>


FVAOrganizerStartPage::FVAOrganizerStartPage()
{
	helloWords = new QTextBrowser;
	cfgButton = new QPushButton;

#ifdef  FVA_LANGUAGE_RUS
	helloWords->setText(tr("Добро пожаловать в систему организации медиа-контента\n Нажмите кнопку \"Следующий\" для старта или \n \"Настройка\" для конфигурации!"));
	cfgButton->setText(tr("Настройка"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	helloWords->setText(tr("Welcome into the multimedia content system!\nPlease press button \"Next\" to continue or \"Configure\" for configuration"));
	cfgButton->setText(tr("Configure"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(helloWords);
	layout->addWidget(cfgButton);

	setLayout(layout);
}
