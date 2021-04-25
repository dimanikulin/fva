#pragma once

#include <QtWidgets/QDialog>
#include "ui_FVAConfigurator.h"

class QDialogButtonBox;
class QTabWidget;

class FVAConfigurator : public QDialog
{
	Q_OBJECT

public:
	FVAConfigurator(QWidget *parent = Q_NULLPTR);

private:// methods
	
	void InitializeCommonTab();
	void InitializeSearchTab();
	void InitializeIntegratorTab();
	void InitializeRenameTab();
	void InitializeFormatTab();

private: // data
	Ui::FVAConfiguratorClass ui;

	QTabWidget *tabs;
	QDialogButtonBox *buttons;
	QWidget *tabCommon;
	QWidget *tabSearch;
	QWidget *tabIntegrator;
	QWidget *tabRename;
	QWidget *tabFormat;
};
