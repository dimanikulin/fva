#ifndef _FVA_ORG_START_PAGE_H_
#define _FVA_ORG_START_PAGE_H_

#include <QtWidgets/QWizardPage>

class QPushButton;
class QTextBrowser;

class FVAOrganizerStartPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerStartPage();
	virtual ~FVAOrganizerStartPage(){};

private:

	QTextBrowser*	helloWords;

	QPushButton*	cfgButton;
};

#endif //_FVA_ORG_START_PAGE_H_