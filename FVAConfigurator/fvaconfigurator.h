#pragma once

#include <QtWidgets/QDialog>
#include <QtCore/QMap>

#include "ui_FVAConfigurator.h"
#include "FVAConfiguration.h"

class QDialogButtonBox;
class QTabWidget;
class QCheckBox;

typedef QMap<QString,QCheckBox*> CHECKBOXES;

class FVAConfigurator : public QDialog
{
	Q_OBJECT

public:

	FVAConfigurator(QWidget *parent = Q_NULLPTR);

	void accept();

private:// methods
	
	void InitializeCommonTab(const FvaConfiguration& cfg);
	void InitializeSearchTab(const FvaConfiguration& cfg);
	void InitializeIntegratorTab(const FvaConfiguration& cfg);
	void InitializeRenameTab(const FvaConfiguration& cfg);
	void InitializeFormatTab(const FvaConfiguration& cfg);

private: // data
	Ui::FVAConfiguratorClass ui;

	QTabWidget *tabs;
	QDialogButtonBox *buttons;

	QWidget *tabCommon;
	QWidget *tabSearch;
	QWidget *tabIntegrator;
	QWidget *tabRename;
	QWidget *tabFormat;

	CHECKBOXES m_checkboxes;

	FvaConfiguration cfg;
};