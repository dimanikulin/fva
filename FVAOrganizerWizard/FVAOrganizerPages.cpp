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


FVAOrganizerDevicePage::FVAOrganizerDevicePage(void)
	: deviceId (-1)
{
	QLabel * titleLabel	= new QLabel(tr("Убедитесь, что устройство, которым делались снимки, верно определилось!"));
    titleLabel->setAlignment(Qt::AlignLeft);

	deviceLbl	= new QLabel(tr("Название:"));
    deviceLbl->setAlignment(Qt::AlignLeft);

	deviceName	= new QLineEdit;
	deviceName->setAlignment(Qt::AlignLeft);
	deviceName->setMaxLength(40);

	matchLbl	= new QLabel(tr("Линковочное имя:"));
    matchLbl->setAlignment(Qt::AlignLeft);

	matchName	= new QLineEdit;
    matchName->setAlignment(Qt::AlignLeft);
	matchName->setMaxLength(40);

	ownerLbl	= new QLabel(tr("Владелец:"));
    ownerLbl->setAlignment(Qt::AlignLeft);
	
	ownerName	= new QLineEdit;
	ownerName->setAlignment(Qt::AlignLeft);

	cbDevice	= new QComboBox;

	btnDct		= new QPushButton;
	btnDct->setText(tr("Справочники"));

	QGridLayout * tableLayout = new QGridLayout;

	tableLayout->addWidget(deviceLbl,0,0);
	tableLayout->addWidget(deviceName,0,1);	
	tableLayout->addWidget(btnDct,0,2);

	tableLayout->addWidget(matchLbl,1,0);
	tableLayout->addWidget(matchName,1,1);

	tableLayout->addWidget(ownerLbl,2,0);
	tableLayout->addWidget(ownerName,2,1);
	tableLayout->addWidget(cbDevice,2,2);
	
	logOutput	= new QTextBrowser;
	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(titleLabel);
	layout->addLayout(tableLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect (btnDct,SIGNAL(clicked()),this,SLOT(OnChangeDictPressed()));
}
void FVAOrganizerDevicePage::setVisible(bool visible)
{	
	DEVICE_MAP	deviceMap		= ((FVAOrganizerWizard*)wizard())->matchedDeviceMap();
	QString		deviceName_		= ((FVAOrganizerWizard*)wizard())->matchedDeviceName();

	if (visible)
	{
		matchName->setText(deviceName_);
		matchName->setReadOnly(true);
		cbDevice->setVisible(false);
		if ( deviceMap.size() > 1 )
		{
			cbDevice->setVisible(true);
			cbDevice->clear();
			cbDevice->addItem ( tr("Выбирете владельца"), 0 );
			for ( auto i = deviceMap.begin(); i != deviceMap.end() ; ++i )
				cbDevice->addItem ( i->ownerName, i->deviceId );

			deviceName->setText(tr("НЕОПРЕДЕЛЕННО!"));
			ownerName->setText(tr("НЕОПРЕДЕЛЕН!"));
		}
		else if (deviceMap.size() ==1 )
		{	
			deviceName->setText(deviceMap.begin().value().guiName);
			ownerName->setText(deviceMap.begin().value().ownerName);
			deviceId = deviceMap.begin().value().deviceId;
			emit completeChanged();
		}
		else
		{
			deviceName->setText(tr("НЕОПРЕДЕЛЕННО!"));
			ownerName->setText(tr("НЕОПРЕДЕЛЕН!"));
		}
	}
	return QWizardPage::setVisible(visible);
}
void FVAOrganizerDevicePage::OnChangeDictPressed()
{
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList params;
	params.append(FVA_DEFAULT_ROOT_DIR + FVA_DB_NAME);
	QString		deviceName_		= ((FVAOrganizerWizard*)wizard())->matchedDeviceName();
	params.append(deviceName_);
	myProcess.start(QCoreApplication::applicationDirPath() + "/FVADictionaryEditor.exe", params);
	myProcess.waitForFinished( -1 );
}
bool FVAOrganizerDevicePage::isComplete() const
{
	// make button next ne disabled
	if (deviceId == -1)
	{
		return false;
	}
	return true;
}

bool FVAOrganizerDevicePage::validatePage()
{
	DEVICE_MAP deviceMap = ((FVAOrganizerWizard*)wizard())->matchedDeviceMap();
	if ( deviceMap.size() > 1 )
	{
		int index = cbDevice->currentIndex();
		if ( 1 <= index ) 
		{
			int ID = cbDevice->itemData( index ).toInt();
			if (ID >= 1)
			{
				deviceId = ID;
				((FVAOrganizerWizard*)wizard())->matchedDeviceID(deviceId);
			}
		}
	}

	FVA_EXIT_CODE exitCode = fvaRunCLT("CLTRenameVideoBySequence", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTRenameVideoBySequence")
	exitCode = fvaRunCLT("CLTConvertAmr", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTConvertAmr")
	exitCode = fvaRunCLT("CLTAutoChecks1", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTAutoChecks1")
	// in read only mode CLTRenameFiles just checks if renaming is possible 
	exitCode = fvaRunCLT("CLTRenameFiles", ((FVAOrganizerWizard*)wizard())->inputFolder(),false,true);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTRenameFiles RO mode")
	exitCode = fvaRunCLT("CLTRenameFiles", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTRenameFiles")
	exitCode = fvaRunCLT("CLTCSVFvaFile", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, QString::number(deviceId));
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTCSVFvaFile")
	exitCode = fvaRunCLT("CLTCreateDirStructByFileNames", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, QString::number(deviceId));
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTCreateDirStructByFileNames")
	exitCode = fvaRunCLT("CLTMoveAloneFiles", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTMoveAloneFiles")
	exitCode = fvaRunCLT("CLTGetFvaDirType", ((FVAOrganizerWizard*)wizard())->inputFolder(),false);
	((FVAOrganizerWizard*)wizard())->inputDirType(exitCode);
	exitCode = fvaRunCLT("CLTAutoChecks2", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTAutoChecks2")

	return true;
}

FVAOrganizerOutputDirPage::FVAOrganizerOutputDirPage(void)
{
	oneEventOneDay				= new QRadioButton("Один день-одно событие");
	severalEventsOneDay			= new QRadioButton("Один день-много событий");
	severalEventsSeveralDays	= new QRadioButton("Много дней-много событий");
	oneEventSeveralDays			= new QRadioButton("Много дней-одно событие");
    	
	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(oneEventOneDay);

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

	if (QFileInfo(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv").exists())
	{
		// merge 2 csv into one: common one and just generated - for folder CSVs
		QString pyScriptPathMerge = "python "
			+ QCoreApplication::applicationDirPath()
			+ "/scripts/merge2csv.py "
			+ FVA_DEFAULT_ROOT_DIR;
		pyCmdList.append(pyScriptPathMerge + "#data#/fvaFolder.csv " + FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv ");
	}
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
		// TODO - check fvaFolderN and call CLTAutoChecks3 for all that folders
	}

	// clean up after processing
	QFile::remove(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv");

	return true;
}
FVAOrganizerDonePage::FVAOrganizerDonePage(void)
{
	finishWords		= new QTextBrowser;

	finishWords->setText(tr("Поздравления! Вы в шаге о окончания процесса."));

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(finishWords);

	setLayout(layout);
}

