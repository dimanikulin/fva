#include "FVAConfigurator.h"
#include "fvadefaultcfg.h"

#include <QTWidgets/QTabWidget>
#include <QTWidgets/QDialogButtonBox>
#include <QTWidgets/QVBoxLayout>
#include <QTWidgets/QCheckBox>
#include <QTWidgets/QLabel>
#include <QTWidgets/QLineEdit>
#include <QTWidgets/QSpinBox>
#include <QTWidgets/QComboBox>

FVAConfigurator::FVAConfigurator(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	tabs = new QTabWidget();

	tabCommon = new QWidget();
	tabSearch = new QWidget();
	tabIntegrator = new QWidget();
	tabRename = new QWidget();
	tabFormat = new QWidget();

	buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabs);
	mainLayout->addWidget(buttons);
	setLayout(mainLayout);

#ifdef  FVA_LANGUAGE_RUS
	tabs->addTab(tabCommon, tr("Общие"));
	tabs->addTab(tabSearch, tr("Поиск по"));
	tabs->addTab(tabIntegrator, tr("Интеграция"));
	tabs->addTab(tabRename, tr("Переимнование"));
	tabs->addTab(tabFormat, tr("Формат"));

	setWindowTitle(tr("Конфигуратор"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	tabs->addTab(tabCommon, tr("Common"));
	tabs->addTab(tabSearch, tr("Search by"));
	tabs->addTab(tabIntegrator, tr("Integration"));
	tabs->addTab(tabRename, tr("Rename"));
	tabs->addTab(tabFormat, tr("Format"));

	setWindowTitle(tr("Configurator"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	InitializeCommonTab();
	InitializeSearchTab();
	InitializeIntegratorTab();
	InitializeRenameTab();
	InitializeFormatTab();
}

void FVAConfigurator::InitializeCommonTab()
{
	QSpinBox* minFilesInDirSpin = new QSpinBox();
	QComboBox* cbLanguage = new QComboBox;
#ifdef  FVA_LANGUAGE_RUS
	QLabel* fvaRootDirLbl = new QLabel(tr("Путь к корневой папке:"));
	QLabel* minFilesInDirLbl = new QLabel(tr("Как мало может быть файлов в папке"));
	QCheckBox* CheckOrientationCheckBox = new QCheckBox(tr("Проверить ориентацию фото"));
	QLabel* LanguageLbl = new QLabel(tr("Системный язык:"));
	cbLanguage->addItem(tr("Английский"), 0);
	cbLanguage->addItem(tr("Русский"), 0);
#else 
#ifdef  FVA_LANGUAGE_ENG
	QLabel* fvaRootDirLbl = new QLabel(tr("Root folder path:"));
	QLabel* minFilesInDirLbl = new QLabel(tr("How few files can be in a folder"));
	QCheckBox* CheckOrientationCheckBox = new QCheckBox(tr("Check photo orientation"));
	QLabel* LanguageLbl = new QLabel(tr("System language:"));
	cbLanguage->addItem(tr("English"), 0);
	cbLanguage->addItem(tr("Russian"), 0);
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout *layout = new QVBoxLayout;
	QLineEdit* fvaRootDirEdit = new QLineEdit;

	layout->addWidget(LanguageLbl);
	layout->addWidget(cbLanguage);
	layout->addWidget(fvaRootDirLbl);
	layout->addWidget(fvaRootDirEdit);
	layout->addWidget(minFilesInDirLbl);
	layout->addWidget(minFilesInDirSpin);
	layout->addWidget(CheckOrientationCheckBox);
	tabCommon->setLayout(layout);
}

void FVAConfigurator::InitializeSearchTab()
{
#ifdef  FVA_LANGUAGE_RUS
	QCheckBox* DateTimeCheckBox			= new QCheckBox(tr("Дата и время"));
	QCheckBox* LocationCheckBox			= new QCheckBox(tr("Место"));
	QCheckBox* PeopleCheckBox			= new QCheckBox(tr("Люди"));
	QCheckBox* DeviceCheckBox			= new QCheckBox(tr("Устройства"));
	QCheckBox* DescOrCommentCheckBox	= new QCheckBox(tr("Описание или коментарий"));
	QCheckBox* EventCheckBox			= new QCheckBox(tr("Событие"));
	QCheckBox* EventReasonPeopleCheckBox= new QCheckBox(tr("Виновники события"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	QCheckBox* DateTimeCheckBox			= new QCheckBox(tr("Date and Time"));
	QCheckBox* LocationCheckBox			= new QCheckBox(tr("Location"));
	QCheckBox* PeopleCheckBox			= new QCheckBox(tr("People"));
	QCheckBox* DeviceCheckBox			= new QCheckBox(tr("Device"));
	QCheckBox* DescOrCommentCheckBox	= new QCheckBox(tr("Description Or Comment"));
	QCheckBox* EventCheckBox			= new QCheckBox(tr("Event"));
	QCheckBox* EventReasonPeopleCheckBox= new QCheckBox(tr("Reason event people"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(DateTimeCheckBox);
	layout->addWidget(LocationCheckBox);
	layout->addWidget(PeopleCheckBox);
	layout->addWidget(DeviceCheckBox);
	layout->addWidget(DescOrCommentCheckBox);
	layout->addWidget(EventCheckBox);
	layout->addWidget(EventReasonPeopleCheckBox);
	tabSearch->setLayout(layout);
}

void FVAConfigurator::InitializeIntegratorTab()
{
#ifdef  FVA_LANGUAGE_RUS
	QCheckBox* GooglePhotoCheckBox = new QCheckBox(tr("Интеграция с Гугл Фото"));
	QCheckBox* digiKamCheckBox = new QCheckBox(tr("Интеграция с digiKam"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	QCheckBox* GooglePhotoCheckBox = new QCheckBox(tr("Integrate with GooglePhoto"));
	QCheckBox* digiKamCheckBox = new QCheckBox(tr("Integrate with digiKam"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(GooglePhotoCheckBox);
	layout->addWidget(digiKamCheckBox);

	tabIntegrator->setLayout(layout);
}

void FVAConfigurator::InitializeRenameTab()
{
#ifdef  FVA_LANGUAGE_RUS
	перевести
	QCheckBox* picsByModifTimeCheckBox = new QCheckBox(tr("shall be renamed pictures files using \nthe file modification time if they do not have exif taken time set"));
	QCheckBox* videoByModifTimeCheckBox = new QCheckBox(tr("shall be renamed video files using \nthe file modification time if they do not have taken time set"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	QCheckBox* picsByModifTimeCheckBox = new QCheckBox(tr("shall be renamed pictures files using \nthe file modification time if they do not have exif taken time set"));
	QCheckBox* videoByModifTimeCheckBox = new QCheckBox(tr("shall be renamed video files using \nthe file modification time if they do not have taken time set"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(picsByModifTimeCheckBox);
	layout->addWidget(videoByModifTimeCheckBox);

	tabRename->setLayout(layout);
}

void FVAConfigurator::InitializeFormatTab()
{
#ifdef  FVA_LANGUAGE_RUS
	перевести
	QLabel* fvaDirNameLbl = new QLabel(tr("format a dir name as a date:"));
	QLabel* fvaDirNameYearLbl = new QLabel(tr("format a dir name as a year:"));
	QLabel* fvaFileNameLbl = new QLabel(tr("format a FVA file name as a date + time:"));
	QLabel* exifDateTimeLbl = new QLabel(tr("format an EXIF date + time:"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	QLabel* fvaDirNameLbl = new QLabel(tr("format a dir name as a date:"));
	QLabel* fvaDirNameYearLbl = new QLabel(tr("format a dir name as a year:"));
	QLabel* fvaFileNameLbl = new QLabel(tr("format a FVA file name as a date + time:"));
	QLabel* exifDateTimeLbl = new QLabel(tr("format an EXIF date + time:"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
	QVBoxLayout *layout = new QVBoxLayout;

	QLineEdit* fvaDirNameEdit = new QLineEdit;
	QLineEdit* fvaDirNameYearEdit = new QLineEdit;
	QLineEdit* fvaFileNameEdit = new QLineEdit;
	QLineEdit* exifDateTimeEdit = new QLineEdit;

	layout->addWidget(fvaDirNameLbl);
	layout->addWidget(fvaDirNameEdit);
	layout->addWidget(fvaDirNameYearLbl);
	layout->addWidget(fvaDirNameYearEdit);
	layout->addWidget(fvaFileNameLbl);
	layout->addWidget(fvaFileNameEdit);
	layout->addWidget(exifDateTimeLbl);
	layout->addWidget(exifDateTimeEdit);

	tabFormat->setLayout(layout);

}