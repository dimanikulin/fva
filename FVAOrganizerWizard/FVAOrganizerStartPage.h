#ifndef _FVA_ORG_START_PAGES_H_
#define _FVA_ORG_START_PAGES_H_

#include <QtWidgets/QWizardPage>
#include <QtWidgets/QTextBrowser>

class FVAOrganizerStartPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerStartPage();
	virtual ~FVAOrganizerStartPage(){};

private:

	QTextBrowser*	helloWords;
};

#endif //_FVA_ORG_START_PAGES_H_