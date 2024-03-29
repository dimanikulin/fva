﻿/*!
* \file FVAConfigurator.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#include "fvaconfigurator.h"
#include "fvaconstants.h" 

#include <QTabWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

FVAConfigurator::FVAConfigurator(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);

	QIcon	icon = QIcon(QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);

	tabs = new QTabWidget();

	tabCommon = new QWidget();
	tabSearch = new QWidget();
	tabIntegrator = new QWidget();
	tabRename = new QWidget();
	tabFormat = new QWidget();
	tabFormat2 = new QWidget();

	buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabs);
	mainLayout->addWidget(buttons);
	setLayout(mainLayout);

#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	tabs->addTab(tabCommon, tr("Common"));
	tabs->addTab(tabSearch, tr("Search by"));
	tabs->addTab(tabIntegrator, tr("Integration"));
	tabs->addTab(tabRename, tr("Rename"));
	tabs->addTab(tabFormat, tr("Format"));
	tabs->addTab(tabFormat2, tr("Format 2"));

	setWindowTitle(tr("Configurator"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	FVA_EXIT_CODE res = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	RET_IF_RES_IS_ERROR

	InitializeCommonTab(cfg);
	InitializeSearchTab(cfg);
	InitializeIntegratorTab(cfg);
	InitializeRenameTab(cfg);
	InitializeFormatTab(cfg);
	InitializeFormat2Tab(cfg);

	bool temp;
	for (CHECKBOXES::iterator it = m_checkboxes.begin(); it != m_checkboxes.end(); ++it)
	{
		res = cfg.getParamAsBoolean(it.key(), temp);
		RET_IF_RES_IS_ERROR
		it.value()->setChecked(temp);
	}
	QString temp2;
	for (LINEEDITES::iterator it = m_lineedites.begin(); it != m_lineedites.end(); ++it)
	{
		res = cfg.getParamAsString(it.key(), temp2);
		RET_IF_RES_IS_ERROR
		it.value()->setText(temp2);
	}
}

void FVAConfigurator::accept()
{
	QDialog::accept();

	for (CHECKBOXES::iterator it = m_checkboxes.begin(); it != m_checkboxes.end(); ++it)
		cfg.setParam(it.key(), it.value()->isChecked());

	for (LINEEDITES::iterator it = m_lineedites.begin(); it != m_lineedites.end(); ++it)
		cfg.setParam(it.key(), it.value()->text());	

	cfg.setParam("Common::Language", static_cast<uint> (cbLanguage->currentIndex()));
	cfg.setParam("Common::LogLevel", static_cast<uint> (cbLogLevel->currentIndex()));
	cfg.setParam("Rename::minFilesInDir", static_cast<uint> (minFilesInDirSpin->value()));

	FVA_EXIT_CODE res = cfg.save(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	RET_IF_RES_IS_ERROR
}

void FVAConfigurator::InitializeCommonTab(const FvaConfiguration& cfg)
{
	cbLanguage = new QComboBox;
	cbLogLevel = new QComboBox;
#ifdef  FVA_LANGUAGE_RUS
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
	m_checkboxes.insert("Common::CheckOrientation", CheckOrientationCheckBox);
	m_lineedites.insert("Common::RootDir", fvaRootDirEdit);

	uint temp;
	FVA_EXIT_CODE res = cfg.getParamAsUint("Common::Language", temp);
	RET_IF_RES_IS_ERROR
	cbLanguage->setCurrentIndex(temp);

	res = cfg.getParamAsUint("Common::LogLevel", temp);
	RET_IF_RES_IS_ERROR
	cbLogLevel->setCurrentIndex(temp);

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
#else 
#ifdef  FVA_LANGUAGE_ENG
	m_checkboxes.insert("Search::DateTime",			new QCheckBox(tr("Date and Time")));
	m_checkboxes.insert("Search::Place",			new QCheckBox(tr("Place")));
	m_checkboxes.insert("Search::People",			new QCheckBox(tr("People")));
	m_checkboxes.insert("Search::Author",			new QCheckBox(tr("Author")));
	m_checkboxes.insert("Search::DescOrComment",		new QCheckBox(tr("Description Or Comment")));
	m_checkboxes.insert("Search::Event",			new QCheckBox(tr("Event")));
	m_checkboxes.insert("Search::EventReasonPeople",	new QCheckBox(tr("Reason event people")));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	QVBoxLayout *layout = new QVBoxLayout;
	for (CHECKBOXES::iterator it = m_checkboxes.begin(); it != m_checkboxes.end(); ++it)
	{
		if (it.key().contains("Search::", Qt::CaseInsensitive))
			layout->addWidget(it.value());
	}
	tabSearch->setLayout(layout);
}

void FVAConfigurator::InitializeIntegratorTab(const FvaConfiguration& cfg)
{
#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	QCheckBox* GooglePhotoCheckBox = new QCheckBox(tr("Integrate with GooglePhoto"));
	QCheckBox* digiKamCheckBox = new QCheckBox(tr("Integrate with digiKam"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	m_checkboxes.insert("Integration::GooglePhoto", GooglePhotoCheckBox);
	m_checkboxes.insert("Integration::digiKam", digiKamCheckBox);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(GooglePhotoCheckBox);
	layout->addWidget(digiKamCheckBox);

	tabIntegrator->setLayout(layout);
}

void FVAConfigurator::InitializeRenameTab(const FvaConfiguration& cfg)
{
	minFilesInDirSpin = new QSpinBox();
#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	QCheckBox* picsByModifTimeCheckBox = new QCheckBox(tr("shall be renamed pictures files using \nthe file modification time \nif they do not have exif taken time set"));
	QCheckBox* videoByModifTimeCheckBox = new QCheckBox(tr("shall be renamed video files using \nthe file modification time \nif they do not have taken time set"));
	QLabel* minFilesInDirLbl = new QLabel(tr("How few files can be in a folder"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS

	uint temp;
	FVA_EXIT_CODE res = cfg.getParamAsUint("Rename::minFilesInDir", temp);
	RET_IF_RES_IS_ERROR
	minFilesInDirSpin->setValue(temp);

	m_checkboxes.insert("Rename::picsByModifTime", picsByModifTimeCheckBox);
	m_checkboxes.insert("Rename::videoByModifTime", videoByModifTimeCheckBox);

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

	m_lineedites.insert("Format::fvaDirName", fvaDirNameEdit);
	m_lineedites.insert("Format::fvaDirNameYear", fvaDirNameYearEdit);
	m_lineedites.insert("Format::fvaFileName", fvaFileNameEdit);
	m_lineedites.insert("Format::exifDateTime", exifDateTimeEdit);

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
/////////////////////////////////
void FVAConfigurator::InitializeFormat2Tab(const FvaConfiguration& cfg)
{
#ifdef  FVA_LANGUAGE_RUS
#else 
#ifdef  FVA_LANGUAGE_ENG
	QLabel* fileName1Lbl = new QLabel(tr("file name format 1 as a date + time:"));
	QLabel* fileName2Lbl = new QLabel(tr("file name format 2 as a date + time:"));
	QLabel* riffDateTime1Lbl = new QLabel(tr("riff format 1 of date + time:"));
	QLabel* riffDateTime2Lbl = new QLabel(tr("riff format 2 of date + time:"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
	QVBoxLayout *layout = new QVBoxLayout;

	QLineEdit* fileName1Edit = new QLineEdit;
	QLineEdit* fileName2Edit = new QLineEdit;
	QLineEdit* riffDateTime1Edit = new QLineEdit;
	QLineEdit* riffDateTime2Edit = new QLineEdit;

	m_lineedites.insert("Format::fileName1", fileName1Edit);
	m_lineedites.insert("Format::fileName2", fileName2Edit);
	m_lineedites.insert("Format::riffDateTime1", riffDateTime1Edit);
	m_lineedites.insert("Format::riffDateTime2", riffDateTime2Edit);

	layout->addWidget(fileName1Lbl);
	layout->addWidget(fileName1Edit);
	layout->addWidget(fileName2Lbl);
	layout->addWidget(fileName2Edit);
	layout->addWidget(riffDateTime1Lbl);
	layout->addWidget(riffDateTime1Edit);
	layout->addWidget(riffDateTime2Lbl);
	layout->addWidget(riffDateTime2Edit);

	tabFormat2->setLayout(layout);
}