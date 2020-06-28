#ifndef FVACONFIGURATOR_H
#define FVACONFIGURATOR_H

#include <QTGui/QMainWindow>
#include "ui_fvaconfigurator.h"

class FVAConfigurator : public QMainWindow
{
	Q_OBJECT

public:
	FVAConfigurator(QWidget *parent = 0);
	~FVAConfigurator();

private:
	Ui::FVAConfiguratorClass ui;
};

#endif // FVACONFIGURATOR_H
