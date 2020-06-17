#ifndef FVAORGANIZERWIZARD_H
#define FVAORGANIZERWIZARD_H

#include <QtGui/QWizard>
#include "ui_fvaorganizerwizard.h"

class FVAOrganizerWizard : public QWizard
{
	Q_OBJECT

public:
	FVAOrganizerWizard(QWidget *parent = 0);
	~FVAOrganizerWizard();

private:
	Ui::FVAOrganizerWizardClass ui;
};

#endif // FVAORGANIZERWIZARD_H
