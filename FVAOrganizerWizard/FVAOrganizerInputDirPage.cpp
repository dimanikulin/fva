#include "FVAOrganizerInputDirPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFileDialog>

#include <QtCore/QProcess>

#include "FVAOrganizerWizard.h"
#include "FVAConfiguration.h"
#include "fvacommonui.h"
#include "fvacommoncsv.h"
#include "fvaconstants.h"
#include "FVADataProcessor.h"


FVAOrganizerInputDirPage::FVAOrganizerInputDirPage(void)
{
#ifdef  FVA_LANGUAGE_RUS
	inputDirLabel	= new QLabel(tr("Укажите входную папку с контентом\n (Внимание! Папка должна начинаться с символа '#'):"));
	dirButton = new QPushButton;
	dirButton->setText(tr("Указать папку"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	inputDirLabel = new QLabel(tr("Please select a folder with a content\n (Attention! The folder name shall start with a char '#'):"));
	dirButton = new QPushButton;
	dirButton->setText(tr("Select a folder"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
	
	inputDirLineEdit = new QLineEdit;
	inputDirLineEdit->setText("");
	inputDirLineEdit->setReadOnly(true);
		

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
	return true;
}
bool	FVAOrganizerInputDirPage::validatePage ()
{
	FVADataProcessor p;
	QString dir = inputDirLineEdit->text();

	((FVAOrganizerWizard*)wizard())->inputFolder(dir);

	FVA_EXIT_CODE exitCode = fvaRunCLT("CLTCheckDeviceName", ((FVAOrganizerWizard*)wizard())->inputFolder());
	if (FVA_ERROR_NON_UNIQUE_DEVICE_NAME == exitCode)
	{
		exitCode = fvaRunCLT("CLTCreateDirStructByDeviceName", ((FVAOrganizerWizard*)wizard())->inputFolder());
		FVA_MESSAGE_BOX("Found several devices in a folder, please select other dir!");
		return false;
	}
	else IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE("CLTCheckDeviceName")

	QDir _dir(dir); 

	FvaConfiguration cfg;

	FVA_EXIT_CODE res = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	if (FVA_NO_ERROR != res)
	{
		FVA_MESSAGE_BOX("cfg.load failed with error " + QString::number(res));
		return false;
	}

	QString rootSWdir;
	res = cfg.getParamAsString("Common::RootDir", rootSWdir);
	if (FVA_NO_ERROR != res)
		return false;

	DEVICE_MAP fullDeviceMap;
	res = fvaLoadDeviceMapFromCsv(rootSWdir, fullDeviceMap);
	if (FVA_NO_ERROR != res)
	{
		FVA_MESSAGE_BOX("fvaLoadDeviceMapFromCsv failed with error " + QString::number(res));
		return false;
	}

	PEOPLE_MAP peopleMap;
	res = fvaLoadPeopleMapFromCsv(rootSWdir, peopleMap);
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
