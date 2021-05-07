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
	QVBoxLayout * layout = new QVBoxLayout;

	outputDirLineEdit = new QLineEdit;
	outputDirLineEdit->setText("");
	outputDirLineEdit->setReadOnly(true);
	dirButton = new QPushButton;
	dirButton->setText(tr("Указать папку"));
	QGridLayout * dirLayout = new QGridLayout;
	dirLayout->addWidget(outputDirLineEdit, 0, 0);
	dirLayout->addWidget(dirButton, 0, 1);	
	layout->addLayout(dirLayout);

	setLayout(layout);

	connect(dirButton, SIGNAL(clicked()), this, SLOT(OnDirButtonClicked()));
}

void FVAOrganizerOutputDirPage::setVisible(bool visible)
{
	return QWizardPage::setVisible(visible);
}
void FVAOrganizerOutputDirPage::OnDirButtonClicked()
{
	QFileDialog dirDialog;

	QString path = dirDialog.getExistingDirectory();

	if (!path.isEmpty())
	{
		outputDirLineEdit->setText(path);
		emit completeChanged();
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
