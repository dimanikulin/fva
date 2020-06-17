#include "FVAOrganizerPages.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QProcess>
#include <QCoreApplication>

#include "fvacommonlib.h"

FVAOrganizerStartPage::FVAOrganizerStartPage()
{
	helloWords		= new QTextBrowser;

	helloWords->setText(tr("Добрый пожаловать в систему организации медиа-контента!"));

	QVBoxLayout * layout = new QVBoxLayout;

	layout->addWidget(helloWords);

	setLayout(layout);
}

FVAOrganizerOrientPage::FVAOrganizerOrientPage()
{
	// to suggest user to run /#BIN#/jpegr_portable32/jpegr.exe
	rotateLabel		= new QLabel(tr("Советуем Вам проверить орентацию контента перед началом работы:"));
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
	myProcess.start("#BIN#/jpegr/jpegr.exe");

	myProcess.waitForFinished( -1 );
}

FVAOrganizerInputDirPage::FVAOrganizerInputDirPage(void)
{
    inputDirLabel	= new QLabel(tr("Укажите входную директорию с контентом:"));
    inputDirLabel->setAlignment(Qt::AlignLeft);
    
	inputDirLineEdit = new QLineEdit;
    inputDirLineEdit->setText("");

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
		inputDirLineEdit->setText(path);
}

bool	FVAOrganizerInputDirPage::validatePage ()
{
	QString dir = inputDirLineEdit->text();
	int deviceId = -1;
	if ( dir.isEmpty() )
	{
		// TODO make button be disabled if dir.isEmpty()
		return false;
	}
	QDir _dir(dir); 

	QMap<QString, int> deviceIds;
	FVA_ERROR_CODE res = fvaLoadDeviceMapFromDictionary(deviceIds, QCoreApplication::applicationDirPath() + "\\data.json");
	if ( FVA_NO_ERROR != res )
	{
		// TODO make suggestion
		return false;
	}
	Q_FOREACH(QFileInfo info, _dir.entryInfoList(QDir::System | QDir::Hidden  | QDir::Files, QDir::DirsLast))
	{		
		if (info.isDir())
			continue;
		QString suffix = info.suffix().toUpper();
		FVA_FILE_TYPE type = fvaConvertFileExt2FileType ( suffix );
		if ( FVA_FILE_TYPE_IMG != type )
			continue;

		QString deviceName;
		deviceId = fvaGetDeviceIdForImg(deviceIds, info.filePath(), deviceName);
		if ( -1 == deviceId ) 
			continue;
		else
			break;
	}
	if (-1 == deviceId)
	{
		//TODO make error with suggestion
		return false;
	}
	QStringList cmdList;
	// TODO to run change orintation in auto mode
	cmdList.append("CLT_Video_Rename_By_Sequence");
	cmdList.append("CLT_Convert_Amr");
	cmdList.append("CLT_Device_Name_Check");
	cmdList.append("CLT_Auto_Checks_1");
	cmdList.append("CLT_Files_Rename");
	cmdList.append("CLT_Dir_Struct_Create_By_File");
	cmdList.append("CLT_Auto_Checks_2");
	// cmdList.append("CLT_Folder_Merging");
	// cmdList.append("CLT_Folder_Merging");

	/*		
	TODO#07.folder merging
	TODO#08.SetReadOnly 09 setDescfiles system 10 setarchive
	*/

	for (auto it = cmdList.begin(); it != cmdList.end(); ++it)
	{
		QProcess myProcess(this);
		myProcess.setProcessChannelMode(QProcess::MergedChannels);
		QStringList params;
		params.append(*it);
		params.append(dir);
		params.append("recursive=yes");
		params.append("logvel=4");
		params.append("readonly=no");
		if (*it == "CLT_Dir_Struct_Create_By_File")		
			params.append("custom=" + QString::number(deviceId));

		myProcess.start("FVAOrganizer.exe",params);

		// Continue reading the data until EOF reached
		QByteArray data;

		while(myProcess.waitForReadyRead())
			logOutput->append(myProcess.readAll());

		myProcess.waitForFinished( -1 );

		int exitCode = myProcess.exitCode();

		if (exitCode != FVA_NO_ERROR)
		{
			//TODO show error
			return false;
		}
	}
	// TODO save logs 
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

