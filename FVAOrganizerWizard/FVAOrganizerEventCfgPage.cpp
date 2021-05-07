#include "FVAOrganizerEventCfgPage.h"

#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include <QtCore/QProcess>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvaconfiguration.h"
#include "FVAFlowController.h"

FVAOrganizerEventCfgPage::FVAOrganizerEventCfgPage(void)
{
#ifdef  FVA_LANGUAGE_RUS
	oneEventOneDay				= new QRadioButton("Один день-одно событие");
	severalEventsOneDay			= new QRadioButton("Один день-много событий");
	severalEventsSeveralDays = new QRadioButton("Много дней-много событий");
	oneEventSeveralDays = new QRadioButton("Много дней-одно событие");
	fvaInfoButton = new QPushButton;
	fvaInfoButton->setText(tr("Добавить инфо"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	oneEventOneDay = new QRadioButton("One day-one event");
	severalEventsOneDay = new QRadioButton("One day-several events");
	severalEventsSeveralDays = new QRadioButton("Several days-several events");
	oneEventSeveralDays = new QRadioButton("Several days-one event");
	fvaInfoButton = new QPushButton;
	fvaInfoButton->setText(tr("Add info"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
    	
	QVBoxLayout * layout = new QVBoxLayout;

	QGridLayout * oneEventOneDayLayout = new QGridLayout;
	oneEventOneDayLayout->addWidget(oneEventOneDay, 0, 0);
	oneEventOneDayLayout->addWidget(fvaInfoButton, 0, 1);
	layout->addLayout(oneEventOneDayLayout);

	layout->addWidget(severalEventsOneDay);
	
	layout->addWidget(severalEventsSeveralDays);
	
	layout->addWidget(oneEventSeveralDays);

	setLayout(layout);

	connect(fvaInfoButton, SIGNAL(clicked()), this, SLOT(OnFvaInfoButtonPressed()));
}
void FVAOrganizerEventCfgPage::OnFvaInfoButtonPressed()
{
	QProcess myProcess(this);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVADescriptionEditor.exe");
	myProcess.waitForFinished(-1);
}
void FVAOrganizerEventCfgPage::setVisible(bool visible)
{
	if (((FVAOrganizerWizard*)wizard())->inputDirType() == FVA_1_EVENT_1_DAY)
		oneEventOneDay->setChecked(true);

	if (((FVAOrganizerWizard*)wizard())->inputDirType() == FVA_FEW_EVENTS_FEW_DAYS)
		severalEventsSeveralDays->setChecked(true);

	if (((FVAOrganizerWizard*)wizard())->inputDirType() == FVA_FEW_EVENTS_1_DAY)
		severalEventsOneDay->setChecked(true);

	if (((FVAOrganizerWizard*)wizard())->inputDirType() == FVA_1_EVENT_FEW_DAYS)
		oneEventSeveralDays->setChecked(true);
	return QWizardPage::setVisible(visible);
}

bool FVAOrganizerEventCfgPage::isComplete() const
{
	return true;
}
bool	FVAOrganizerEventCfgPage::validatePage()
{
	FVAFlowController flow;
	FVA_EXIT_CODE exitCode = flow.MoveInputDirToOutput(((FVAOrganizerWizard*)wizard())->inputFolder(), "TODO"/*, this*/);
	if (exitCode != FVA_NO_ERROR)
		return false;

	return true;
}
