#include "FVAOrganizerOutputDirPage.h"

#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

#include <QtCore/QProcess>

#include "fvaorganizerwizard.h"
#include "fvacommonui.h"
#include "fvadefaultcfg.h"

FVAOrganizerOutputDirPage::FVAOrganizerOutputDirPage(void)
{
	oneEventOneDay				= new QRadioButton("Один день-одно событие");
	severalEventsOneDay			= new QRadioButton("Один день-много событий");
	severalEventsSeveralDays	= new QRadioButton("Много дней-много событий");
	oneEventSeveralDays			= new QRadioButton("Много дней-одно событие");
    	
	QVBoxLayout * layout = new QVBoxLayout;

	fvaInfoButton = new QPushButton;
	fvaInfoButton->setText(tr("Добавить инфо"));
	QGridLayout * oneEventOneDayLayout = new QGridLayout;
	oneEventOneDayLayout->addWidget(oneEventOneDay, 0, 0);
	oneEventOneDayLayout->addWidget(fvaInfoButton, 0, 1);
	layout->addLayout(oneEventOneDayLayout);

	layout->addWidget(severalEventsOneDay);
	
	layout->addWidget(severalEventsSeveralDays);
	
	layout->addWidget(oneEventSeveralDays);
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
	connect(fvaInfoButton, SIGNAL(clicked()), this, SLOT(OnFvaInfoButtonPressed()));
}
void FVAOrganizerOutputDirPage::OnFvaInfoButtonPressed()
{
	QProcess myProcess(this);
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVADescriptionEditor.exe");
	myProcess.waitForFinished(-1);
}
void FVAOrganizerOutputDirPage::setVisible(bool visible)
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
	if (oneEventSeveralDays->isChecked())
	{
		QString dir = outputDirLineEdit->text();

		// make button be disabled if dir.isEmpty()
		if (dir.isEmpty())
		{
			return false;
		}
	}
	return true;
}
bool	FVAOrganizerOutputDirPage::validatePage ()
{
	FVA_EXIT_CODE exitCode = fvaRunCLT("CLTSetFileAtts", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTSetFileAtts")

	if (oneEventOneDay->isChecked())
	{
		exitCode = fvaRunCLT("CLTMerge1DayEventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());

		QString mode = "merge";
		if (exitCode == FVA_ERROR_DEST_DIR_ALREADY_EXISTS)
		{
			// ask user for what to do 
			QMessageBox msgBox;
			msgBox.setText("The dir for merge is already present in target!");
			msgBox.setInformativeText("By pressing OK all the content will be merged to already existing folder,\n by pressing Cancel new dir will be created to keep the content");
			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Ok);
			
			QString mode = (msgBox.exec() == QMessageBox::Ok) ? "merge" : "create";			
		}

		// and run it again
		exitCode = fvaRunCLT("CLTMerge1DayEventDir", ((FVAOrganizerWizard*)wizard())->inputFolder(), false, false, mode);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1DayEventDir")

		if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode)
		{
			exitCode = fvaRunCLT("CLTFixDuplicatedFileNames", ((FVAOrganizerWizard*)wizard())->inputFolder());
			IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTFixDuplicatedFileNames")
			exitCode = fvaRunCLT("CLTMerge1DayEventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());
		}
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1DayEventDir")
	}
	else if (oneEventSeveralDays->isChecked())
	{
		exitCode = fvaRunCLT("CLTMerge1EventDir", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, outputDirLineEdit->text());

		if (FVA_ERROR_DEST_FILE_ALREADY_EXISTS == exitCode)
		{
			exitCode = fvaRunCLT("CLTFixDuplicatedFileNames", ((FVAOrganizerWizard*)wizard())->inputFolder());
			IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTFixDuplicatedFileNames")
			exitCode = fvaRunCLT("CLTMerge1EventDir", ((FVAOrganizerWizard*)wizard())->inputFolder());
		}
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMerge1EventDir")
	}
	else
	{
		FVA_MESSAGE_BOX("Not implemented yet");
		return false;
	}
	
	QStringList pyCmdList;

	// merge 2 csv into one: common one and just generated - for file CSVs
	QString pyScriptPathMerge2 = "python "
		+ QCoreApplication::applicationDirPath()
		+ "/scripts/merge2csv.py "
		+ FVA_DEFAULT_ROOT_DIR;
	pyCmdList.append(pyScriptPathMerge2 + "#data#/fvaFile.csv " + FVA_DEFAULT_ROOT_DIR + "#data#/fvaFileN.csv ");

	// lets run python cmd list 
	for (auto it = pyCmdList.begin(); it != pyCmdList.end(); ++it)
	{
		QProcess myProcess(this);
		myProcess.setProcessChannelMode(QProcess::MergedChannels);
		myProcess.start(*it);
		myProcess.waitForFinished(-1);

		int exitCode = myProcess.exitCode();
		if (exitCode != 0)
		{
			FVA_MESSAGE_BOX("Fva cmd " + *it + " failed with error " + QString::number(exitCode));
			return false;
		}
	}

	// clean up after processing
	QFile::remove(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFileN.csv");
	
	// last but not least check
	if (oneEventSeveralDays->isChecked())
	{
		exitCode = fvaRunCLT("CLTAutoChecks3", outputDirLineEdit->text());
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTAutoChecks3")
	}
	else
	{
		// TODO - apply call CLTAutoChecks3 for all merged folders
	}

	return true;
}
