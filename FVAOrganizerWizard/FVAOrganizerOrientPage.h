#ifndef _FVA_ORG_ORIENT_PAGE_H_
#define _FVA_ORG_ORIENT_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLabel;
class QPushButton;

class FVAOrganizerOrientPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerOrientPage();
	virtual ~FVAOrganizerOrientPage(){};

	private slots:

	void OnOrientationButtonClicked();

private: // data 

	QLabel*			rotateLabel;
	QPushButton*	rotateButton;

};
#endif // _FVA_ORG_ORIENT_PAGE_H_
