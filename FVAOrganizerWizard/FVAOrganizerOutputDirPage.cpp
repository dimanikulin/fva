#include "FVAOrganizerOutputDirPage.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvaconfiguration.h"

FVAOrganizerOutputDirPage::FVAOrganizerOutputDirPage(void)
{   
#ifdef  FVA_LANGUAGE_RUS
	QLabel* inputDirLabel = new QLabel(tr("Укажите папку digiKam для контента:"));
	QPushButton* digiKamButton = new QPushButton;
	digiKamButton->setText(tr("Указать папку"));
	QLabel* dirLabelGP = new QLabel(tr("Укажите папку Google Photo для контента:"));
	QPushButton* googlePhotoButton = new QPushButton;
	googlePhotoButton->setText(tr("Указать папку"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	QLabel* dirLabelDK = new QLabel(tr("Please select a digiKam dir to put the content in:"));
	QPushButton* digiKamButton = new QPushButton;
	digiKamButton->setText(tr("Select a folder"));
	QLabel* dirLabelGP = new QLabel(tr("Please select a Google Photo dir to put the content in:"));
	QPushButton* googlePhotoButton = new QPushButton;
	googlePhotoButton->setText(tr("Select a folder"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout * layout = new QVBoxLayout;

	digiKamLineEdit = new QLineEdit;
	digiKamLineEdit->setReadOnly(true);

	googlePhotoLineEdit = new QLineEdit;
	googlePhotoLineEdit->setReadOnly(true);

	QGridLayout * dirLayout = new QGridLayout;
	dirLayout->addWidget(dirLabelDK, 0, 0);
	dirLayout->addWidget(digiKamLineEdit, 1, 0);
	dirLayout->addWidget(digiKamButton, 1, 1);

	dirLayout->addWidget(dirLabelGP, 2, 0);
	dirLayout->addWidget(googlePhotoLineEdit, 3, 0);
	dirLayout->addWidget(googlePhotoButton, 3, 1);

	layout->addLayout(dirLayout);

	setLayout(layout);

	connect(digiKamButton, SIGNAL(clicked()), this, SLOT(OnDigiKamDirButtonClicked()));
	connect(googlePhotoButton, SIGNAL(clicked()), this, SLOT(OnGooglePhotoDirButtonClicked()));

	// TODO to add check to remove input dir 
}

void FVAOrganizerOutputDirPage::setVisible(bool visible)
{
	return QWizardPage::setVisible(visible);
}
void FVAOrganizerOutputDirPage::OnDigiKamDirButtonClicked()
{
	QFileDialog dirDialog;

	QString path = dirDialog.getExistingDirectory();

	if (!path.isEmpty())
	{
		digiKamLineEdit->setText(path);
		// emit completeChanged();
	}
}
void FVAOrganizerOutputDirPage::OnGooglePhotoDirButtonClicked()
{
	QFileDialog dirDialog;

	QString path = dirDialog.getExistingDirectory();

	if (!path.isEmpty())
	{
		googlePhotoLineEdit->setText(path);
		// emit completeChanged();
	}
}
bool FVAOrganizerOutputDirPage::isComplete() const
{
	return true;
}
bool	FVAOrganizerOutputDirPage::validatePage ()
{
	
	return true;
}
