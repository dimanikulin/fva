/*!
* \file FVAConfigurator.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QMap>

#include "ui_FVAConfigurator.h"
#include "../FVACommonLib/FVAConfiguration.h"

class QDialogButtonBox;
class QTabWidget;
class QCheckBox;
class QSpinBox;
class QComboBox;

typedef QMap<QString, QCheckBox*> CHECKBOXES;
typedef QMap<QString, QLineEdit*> LINEEDITES;

/*!
* \brief FVAConfigurator is child of QDialog and implements the next functions:
*
* 1. Initialization of Tab Widget with Common settings from FvaConfiguration
* 2. Initialization of Tab Widget with Search settings from FvaConfiguration
* 3. Initialization of Tab Widget with Integrator settings from FvaConfiguration
* 4. Initialization of Tab Widget with Rename settings from FvaConfiguration
* 5. Initialization of Tab Widget with Format settings from FvaConfiguration
* 6. Initialization of Tab Widget with Format2 settings from FvaConfiguration
*
*/
class FVAConfigurator : public QDialog
{
	Q_OBJECT

public:

	FVAConfigurator(QWidget *parent = Q_NULLPTR);

	/*!
	* \brief it saves all UI setting to FvaConfiguration
	*/
	void accept();

private:// methods

	/*!
	* \brief it loads common tab UI setting from FvaConfiguration
	* \param cfg - FvaConfiguration to load from
	*/
	void InitializeCommonTab(const FvaConfiguration& cfg);

	/*!
	* \brief it loads search tab UI setting from FvaConfiguration
	* \param cfg - FvaConfiguration to load from
	*/
	void InitializeSearchTab(const FvaConfiguration& cfg);

	/*!
	* \brief it loads integrator tab UI setting from FvaConfiguration
	* \param cfg - FvaConfiguration to load from
	*/
	void InitializeIntegratorTab(const FvaConfiguration& cfg);

	/*!
	* \brief it loads rename tab UI setting from FvaConfiguration
	* \param cfg - FvaConfiguration to load from
	*/
	void InitializeRenameTab(const FvaConfiguration& cfg);

	/*!
	* \brief it loads format tab UI setting from FvaConfiguration
	* \param cfg - FvaConfiguration to load from
	*/
	void InitializeFormatTab(const FvaConfiguration& cfg);

	/*!
	* \brief it loads format2 tab UI setting from FvaConfiguration
	* \param cfg - FvaConfiguration to load from
	*/
	void InitializeFormat2Tab(const FvaConfiguration& cfg);

private: // data

	Ui::FVAConfiguratorClass ui;

	QTabWidget *tabs;
	QDialogButtonBox *buttons;

	QWidget *tabCommon;
	QWidget *tabSearch;
	QWidget *tabIntegrator;
	QWidget *tabRename;
	QWidget *tabFormat;
	QWidget *tabFormat2;

	CHECKBOXES m_checkboxes;
	LINEEDITES m_lineedites;

	QComboBox*	cbLanguage;
	QComboBox*	cbLogLevel;
	QSpinBox*	minFilesInDirSpin;

	FvaConfiguration cfg;
};