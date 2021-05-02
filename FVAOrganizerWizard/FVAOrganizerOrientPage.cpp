#include "FVAOrganizerOrientPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

FVAOrganizerOrientPage::FVAOrganizerOrientPage()
{
	// to suggest user to run /jpegr_portable32/jpegr.exe
	rotateLabel		= new QLabel(tr("Советуем Вам проверить ориентацию контента перед началом работы:"));
	rotateLabel->setAlignment(Qt::AlignLeft);
	rotateButton	= new QPushButton;
	rotateButton->setText(tr("Проверить"));

	QVBoxLayout * layout = new QVBoxLayout;
	layout->addWidget(rotateLabel);
	layout->addWidget(rotateButton);

	connect( rotateButton, SIGNAL( clicked() ), this, SLOT( OnOrientationButtonClicked() ) );

	setLayout(layout);
}

void FVAOrganizerOrientPage::OnOrientationButtonClicked()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(QCoreApplication::applicationDirPath() + "/jpegr/jpegr.exe");
	myProcess.waitForFinished( -1 );
}
