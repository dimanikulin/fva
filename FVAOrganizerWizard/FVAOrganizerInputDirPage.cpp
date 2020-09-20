#include "FVAOrganizerInputDirPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFileDialog>

#include <QtCore/QProcess>

#include "FVAOrganizerWizard.h"
#include "fvacommonui.h"
#include "fvacommoncsv.h"

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
	return true;
}
bool	FVAOrganizerInputDirPage::validatePage ()
{
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
