#include "FVAOrganizerPages.h"
#include "fvaorganizerwizard.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include "fvacommonlib.h"
#include "fvadefaultcfg.h"
#include "fvacommoncsv.h"
#include "fvacommonui.h"

FVAOrganizerStartPage::FVAOrganizerStartPage()
{
	helloWords		= new QTextBrowser;

	helloWords->setText(tr("Добро пожаловать в систему организации медиа-контента!"));

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(helloWords);

	setLayout(layout);
}

FVAOrganizerInputDirPage::FVAOrganizerInputDirPage(void)
{
	inputDirLabel	= new QLabel(tr("Укажите входную папку с контентом\n (Внимание! Папка должна начинаться с символа '#'):"));
	inputDirLabel->setAlignment(Qt::AlignLeft);
    
	inputDirLineEdit = new QLineEdit;
	inputDirLineEdit->setText("");
	inputDirLineEdit->setReadOnly(true);

	dirButton		= new QPushButton;
	dirButton->setText(tr("Указать папку"));		

	QGridLayout * dirLayout = new QGridLayout;
	dirLayout->addWidget(inputDirLineEdit,0,0);
	dirLayout->addWidget(dirButton,0,1);

	logOutput		= new QTextBrowser;
	
	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(inputDirLabel);
	layout->addLayout(dirLayout);
	layout->addWidget(logOutput);

	setLayout(layout);

	connect( dirButton, SIGNAL( clicked() ), this, SLOT( OnDirButtonClicked() ) );
}
void FVAOrganizerInputDirPage::OnDirButtonClicked()
{
	QFileDialog dirDialog;

	QString path = dirDialog.getExistingDirectory();

	if (!path.isEmpty())
	{
		inputDirLineEdit->setText(path);
		emit completeChanged();
	}
}
bool FVAOrganizerInputDirPage::isComplete() const
{
	QString dir = inputDirLineEdit->text();

	// make button be disabled if dir.isEmpty()
	if (dir.isEmpty())
	{
		return false;
	}
}
bool	FVAOrganizerInputDirPage::validatePage ()
{
	QString dir = inputDirLineEdit->text();

	((FVAOrganizerWizard*)wizard())->inputFolder(dir);

	QDir _dir(dir); 

	DEVICE_MAP fullDeviceMap;
	FVA_EXIT_CODE res = fvaLoadDeviceMapFromCsv(fullDeviceMap);
	if ( FVA_NO_ERROR != res )
	{
		FVA_MESSAGE_BOX("fvaLoadDeviceMapFromCsv failed with error " + QString::number(res));
		return false;
	}

	PEOPLE_MAP peopleMap;
	res = fvaLoadPeopleMapFromCsv(peopleMap);
	if (FVA_NO_ERROR != res)
	{
		FVA_MESSAGE_BOX("fvaLoadPeopleMapFromCsv failed with error " + QString::number(res));
		return false;
	}

	for (auto it = fullDeviceMap.begin(); it != fullDeviceMap.end(); ++it)
	{
		it.value().ownerName = peopleMap[it.value().ownerId].name;
	}

	DEVICE_MAP deviceMap;
	Q_FOREACH(QFileInfo info, _dir.entryInfoList(QDir::System | QDir::Hidden  | QDir::Files, QDir::DirsLast))
	{		
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FS_TYPE type = fvaConvertFileExt2FileType ( suffix );
		if ( FVA_FS_TYPE_IMG != type )
			continue;

		QString deviceName;		
		deviceMap = fvaGetDeviceMapForImg(fullDeviceMap, info.filePath(), deviceName);
		
		if (deviceName.isEmpty())
			continue;		
		else
		{
			((FVAOrganizerWizard*)wizard())->matchedDeviceName(deviceName);
			break;
		}
	}
	((FVAOrganizerWizard*)wizard())->inputFolder(dir);
	((FVAOrganizerWizard*)wizard())->fullDeviceMap(fullDeviceMap);		
	((FVAOrganizerWizard*)wizard())->matchedDeviceMap(deviceMap);

	// to run change orintation in auto mode
	QProcess myProcess(this);    
	myProcess.setProcessChannelMode(QProcess::MergedChannels);
	myProcess.start(QCoreApplication::applicationDirPath() + "/jpegr/jpegr.exe -auto " + dir);

	myProcess.waitForFinished( -1 );
	return true;
}

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
			if ( ID >= 1)
				deviceId = ID; 
		}
	}

	FVA_EXIT_CODE exitCode = fvaRunCLT("CLT_Video_Rename_By_Sequence", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Video_Rename_By_Sequence")
	exitCode = fvaRunCLT("CLT_Convert_Amr", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Convert_Amr")
	exitCode = fvaRunCLT("CLT_Device_Name_Check", ((FVAOrganizerWizard*)wizard())->inputFolder());
	if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
	{
		exitCode = fvaRunCLT("CLTCreateDirStructByDeviceName", ((FVAOrganizerWizard*)wizard())->inputFolder());
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTCreateDirStructByDeviceName")
	}
	else IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Device_Name_Check")
	exitCode = fvaRunCLT("CLT_Auto_Checks_1", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Auto_Checks_1")
	// in read only mode CLTRenameFiles just checks if renaming is possible 
	exitCode = fvaRunCLT("CLTRenameFiles", ((FVAOrganizerWizard*)wizard())->inputFolder(),false,true);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTRenameFiles RO mode")
	exitCode = fvaRunCLT("CLTRenameFiles", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTRenameFiles")
	exitCode = fvaRunCLT("CLT_Fva_Files_2_CSV", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, QString::number(deviceId));
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Fva_Files_2_CSV")
	exitCode = fvaRunCLT("CLT_Fva_Folder_2_CSV", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, QString::number(deviceId));
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Fva_Folder_2_CSV")
	exitCode = fvaRunCLT("CLT_Dir_Struct_Create_By_File", ((FVAOrganizerWizard*)wizard())->inputFolder(), true, false, QString::number(deviceId));
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Dir_Struct_Create_By_File")
	exitCode = fvaRunCLT("CLT_Alone_Files_Move", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Alone_Files_Move")
	exitCode = fvaRunCLT("CLT_Get_Fva_Dir_Type", ((FVAOrganizerWizard*)wizard())->inputFolder(),false);
	((FVAOrganizerWizard*)wizard())->inputDirType(exitCode);
	exitCode = fvaRunCLT("CLT_Auto_Checks_2", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Auto_Checks_2")

	return true;
}

FVAOrganizerOutputDirPage::FVAOrganizerOutputDirPage(void)
{
	oneEventOneDay				= new QRadioButton("Один день-одно событие");
	severalEventsOneDay			= new QRadioButton("Один день-много событий");;
	severalEventsSeveralDays	= new QRadioButton("Много дней-много событий");;
	oneEventSeveralDays			= new QRadioButton("Много дней-одно событие");;
    	
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
	// TODO to fix as soon as this page is redesigned to be - one-day-event, many-day-one-event, many-event-many-days  
	return true;

	QString dir = outputDirLineEdit->text();

	// make button be disabled if dir.isEmpty()
	if (dir.isEmpty())
	{
		return false;
	}
}
bool	FVAOrganizerOutputDirPage::validatePage ()
{
	QString dir = outputDirLineEdit->text();

	FVA_EXIT_CODE exitCode = fvaRunCLT("CLT_Set_File_Atts", ((FVAOrganizerWizard*)wizard())->inputFolder());
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Set_File_Atts")

	if (oneEventOneDay->isChecked())
	{
		exitCode = fvaRunCLT("CLT_1_Day_Event_Folder_Merging", ((FVAOrganizerWizard*)wizard())->inputFolder());
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_1_Day_Event_Folder_Merging")
	}
	else if (oneEventSeveralDays->isChecked())
	{
		exitCode = fvaRunCLT("CLT_1_Event_Folder_Merging", ((FVAOrganizerWizard*)wizard())->inputFolder(),true,false,dir);
		IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_1_Event_Folder_Merging")
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

	// change FVA_TARGET_FOLDER_NAME tag to actual folder name for sql files
	QString pyScriptPath = "python " 
							+ QCoreApplication::applicationDirPath() 
							+ "/scripts/updateTargetDirName.py " 
							+ FVA_DEFAULT_ROOT_DIR;
	QString mergeDir = outputDirLineEdit->text();
	mergeDir = mergeDir.replace("\\","/");  // replace slaches on backslashes
	mergeDir = mergeDir.remove(FVA_DEFAULT_ROOT_DIR); // remove a prefix as root dir
	pyCmdList.append(pyScriptPath + "#data#/fvaFolderN.csv "	+ "/" + mergeDir);

	// merge 2 csv into one: common one and just generated - for folder CSVs
	QString pyScriptPathMerge = "python " 
							+ QCoreApplication::applicationDirPath() 
							+ "/scripts/merge2csv.py " 
							+ FVA_DEFAULT_ROOT_DIR;
	pyCmdList.append(pyScriptPathMerge + "#data#/fvaFolder.csv " + FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv ");

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
	QFile::remove(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv");
	QFile::remove(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv.bak");

	// last but not least check
	exitCode = fvaRunCLT("CLT_Auto_Checks_3", mergeDir);
	IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLT_Auto_Checks_3")
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

