#include "FVAOrganizerDonePage.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextBrowser>

FVAOrganizerDonePage::FVAOrganizerDonePage(void)
{
	finishWords		= new QTextBrowser;

	finishWords->setText(tr("Поздравления! Вы в шаге о окончания процесса."));

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(finishWords);

	setLayout(layout);
}

