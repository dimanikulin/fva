#ifndef _FVA_ORG_DONE_PAGE_H_
#define _FVA_ORG_DONE_PAGE_H_

#include <QtWidgets/QWizardPage>

class QTextBrowser;

class FVAOrganizerDonePage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerDonePage();
		virtual ~FVAOrganizerDonePage(){};

	private:

		QTextBrowser*	finishWords;

};

#endif // _FVA_ORG_DONE_PAGE_H_
