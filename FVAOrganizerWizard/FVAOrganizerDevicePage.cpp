#include "FVAOrganizerDevicePage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

#include <QtCore/QProcess>

#include "fvacommoncsv.h"
#include "fvacommonui.h"
#include "FVAOrganizerWizard.h"

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
	// TODO WHY DO WE NEED IT???
	params.append(/*FVA_DEFAULT_ROOT_DIR +*/ FVA_DB_NAME);
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

	// in read only mode CLTRenameFiles just checks if renaming is possible 
	FVA_EXIT_CODE exitCode = fvaRunCLT("CLTRenameFiles", ((FVAOrganizerWizard*)wizard())->inputFolder(), false, true);
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
