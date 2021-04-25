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

private:
	Ui::FVAConfiguratorClass ui;
	QTabWidget *tabs;
	QDialogButtonBox *buttons;

};
