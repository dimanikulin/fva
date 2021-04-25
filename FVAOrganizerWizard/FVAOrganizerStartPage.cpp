#include "FVAOrganizerStartPage.h"
#include "fvadefaultcfg.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

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

	connect(cfgButton, SIGNAL(clicked()), this, SLOT(OnCfgBtnPressed()));
}

void FVAOrganizerStartPage::OnCfgBtnPressed()
{
	QProcess myProcess(this);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	/*QStringList params;
	params.append(FVA_DEFAULT_ROOT_DIR + FVA_DB_NAME);
	QString		deviceName_ = ((FVAOrganizerWizard*)wizard())->matchedDeviceName();
	params.append(deviceName_);*/
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVAConfigurator.exe"/*, params*/);
	myProcess.waitForFinished(-1);
}
