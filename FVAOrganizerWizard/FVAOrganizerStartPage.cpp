#include "FVAOrganizerStartPage.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextBrowser>

FVAOrganizerStartPage::FVAOrganizerStartPage()
{
	helloWords = new QTextBrowser;

	helloWords->setText(tr("Добро пожаловать в систему организации медиа-контента!"));

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(helloWords);

	setLayout(layout);
}
