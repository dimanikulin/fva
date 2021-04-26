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

	FVA_EXIT_CODE res = cfg.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaParams.csv");
	RET_IF_RES_IS_ERROR

	InitializeCommonTab(cfg);
	InitializeSearchTab(cfg);
	InitializeIntegratorTab(cfg);
	InitializeRenameTab(cfg);
	InitializeFormatTab(cfg);
}

void FVAConfigurator::InitializeCommonTab(const FvaConfiguration& cfg)
{
	QComboBox* cbLanguage = new QComboBox;
	QComboBox* cbLogLevel = new QComboBox;
#ifdef  FVA_LANGUAGE_RUS
	QLabel* fvaRootDirLbl = new QLabel(tr("Путь к корневой папке:"));
	QCheckBox* CheckOrientationCheckBox = new QCheckBox(tr("Проверить ориентацию фото"));
	QLabel* LanguageLbl = new QLabel(tr("Системный язык:"));
	cbLanguage->addItem(tr("Английский"), 0);
	cbLanguage->addItem(tr("Русский"), 0);
	QLabel* LogLbl = new QLabel(tr("Уровень логирования:"));
	cbLogLevel->addItem(tr("Ошибки"), 0);
	cbLogLevel->addItem(tr("Предупрежедения"), 0);
	cbLogLevel->addItem(tr("Отладка"), 0);
#else 
#ifdef  FVA_LANGUAGE_ENG
	QLabel* fvaRootDirLbl = new QLabel(tr("Root folder path:"));
	QCheckBox* CheckOrientationCheckBox = new QCheckBox(tr("Check photo orientation"));
	QLabel* LanguageLbl = new QLabel(tr("System language:"));
	cbLanguage->addItem(tr("English"), 0);
	cbLanguage->addItem(tr("Russian"), 0);
	QLabel* LogLbl = new QLabel(tr("Log level:"));
	cbLogLevel->addItem(tr("Critical"), 0);
	cbLogLevel->addItem(tr("Warnings"), 0);
	cbLogLevel->addItem(tr("Debug"), 0);
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	
	QLineEdit* fvaRootDirEdit = new QLineEdit;
	fvaRootDirEdit->setText(cfg.getParamAsString("Common::RootDir"));
	CheckOrientationCheckBox->setChecked(cfg.getParamAsBoolean("Common::CheckOrientation"));
	// TODO set Common::Language
	// TODO set Common::LogLevel

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(LanguageLbl);
	layout->addWidget(cbLanguage); 
	layout->addWidget(fvaRootDirLbl);
	layout->addWidget(fvaRootDirEdit); 
	layout->addWidget(LogLbl);
	layout->addWidget(cbLogLevel); 
	layout->addWidget(CheckOrientationCheckBox);
	tabCommon->setLayout(layout);
}

void FVAConfigurator::InitializeSearchTab(const FvaConfiguration& cfg)
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

	DateTimeCheckBox->setChecked(cfg.getParamAsBoolean("Search::DateTime"));
	LocationCheckBox->setChecked(cfg.getParamAsBoolean("Search::Location"));
	PeopleCheckBox->setChecked(cfg.getParamAsBoolean("Search::People"));
	DeviceCheckBox->setChecked(cfg.getParamAsBoolean("Search::Device"));
	DescOrCommentCheckBox->setChecked(cfg.getParamAsBoolean("Search::DescOrComment"));
	EventCheckBox->setChecked(cfg.getParamAsBoolean("Search::Event"));
	EventReasonPeopleCheckBox->setChecked(cfg.getParamAsBoolean("Search::EventReasonPeople"));

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

void FVAConfigurator::InitializeIntegratorTab(const FvaConfiguration& cfg)
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

	GooglePhotoCheckBox->setChecked(cfg.getParamAsBoolean("Integration::GooglePhoto"));
	digiKamCheckBox->setChecked(cfg.getParamAsBoolean("Integration::digiKam"));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(GooglePhotoCheckBox);
	layout->addWidget(digiKamCheckBox);

	tabIntegrator->setLayout(layout);
}

void FVAConfigurator::InitializeRenameTab(const FvaConfiguration& cfg)
{
	QSpinBox* minFilesInDirSpin = new QSpinBox();
#ifdef  FVA_LANGUAGE_RUS
	перевести
	QCheckBox* picsByModifTimeCheckBox = new QCheckBox(tr("shall be renamed pictures files using \nthe file modification time if they do not have exif taken time set"));
	QCheckBox* videoByModifTimeCheckBox = new QCheckBox(tr("shall be renamed video files using \nthe file modification time if they do not have taken time set"));
	QLabel* minFilesInDirLbl = new QLabel(tr("Как мало может быть файлов в папке"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	QCheckBox* picsByModifTimeCheckBox = new QCheckBox(tr("shall be renamed pictures files using \nthe file modification time \nif they do not have exif taken time set"));
	QCheckBox* videoByModifTimeCheckBox = new QCheckBox(tr("shall be renamed video files using \nthe file modification time \nif they do not have taken time set"));
	QLabel* minFilesInDirLbl = new QLabel(tr("How few files can be in a folder"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	picsByModifTimeCheckBox->setChecked(cfg.getParamAsBoolean("Rename::picsByModifTime"));
	videoByModifTimeCheckBox->setChecked(cfg.getParamAsBoolean("Rename::videoByModifTime"));
	minFilesInDirSpin->setValue(cfg.getParamAsUint("Rename::minFilesInDir"));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(minFilesInDirLbl);
	layout->addWidget(minFilesInDirSpin);
	layout->addWidget(picsByModifTimeCheckBox);
	layout->addWidget(videoByModifTimeCheckBox);
	tabRename->setLayout(layout);
}

void FVAConfigurator::InitializeFormatTab(const FvaConfiguration& cfg)
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

	fvaDirNameEdit->setText(cfg.getParamAsString("Format::fvaDirName"));
	fvaDirNameYearEdit->setText(cfg.getParamAsString("Format::fvaDirNameYear"));
	fvaFileNameEdit->setText(cfg.getParamAsString("Format::fvaFileName"));
	exifDateTimeEdit->setText(cfg.getParamAsString("Format::exifDateTime"));

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