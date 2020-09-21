#include "FVAOrganizerPages.h"
#include "fvaorganizerwizard.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

#include "fvacommonlib.h"
#include "fvadefaultcfg.h"
#include "fvacommoncsv.h"
#include "fvacommonui.h"

FVAOrganizerDonePage::FVAOrganizerDonePage(void)
{
	finishWords		= new QTextBrowser;

	finishWords->setText(tr("Поздравления! Вы в шаге о окончания процесса."));

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(finishWords);

	setLayout(layout);
}

