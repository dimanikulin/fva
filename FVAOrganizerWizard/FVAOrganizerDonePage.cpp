#include "FVAOrganizerDonePage.h"
#include "FVAconstants.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextBrowser>

FVAOrganizerDonePage::FVAOrganizerDonePage(void)
{
	finishWords		= new QTextBrowser;

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(finishWords);

	setLayout(layout);

#ifdef  FVA_LANGUAGE_RUS
	finishWords->setText(tr("Поздравления! Вы в шаге о окончания процесса."));
#else 
#ifdef  FVA_LANGUAGE_ENG
	finishWords->setText(tr("Congratulations, you are ready to finish."));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
}

