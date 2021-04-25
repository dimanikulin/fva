#ifndef FVACONFIGURATOR_H
#define FVACONFIGURATOR_H

#include <QTWidgets/QDialog>
#include "ui_fvaconfigurator.h"

class FVAConfigurator : public QDialog
{
	Q_OBJECT

public:
	FVAConfigurator(QWidget *parent = 0);
	~FVAConfigurator();

private:
	Ui::FVAConfiguratorClass ui;
};

#endif // FVACONFIGURATOR_H
