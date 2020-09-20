#ifndef _FVA_ORG_INPUT_PAGE_H_
#define _FVA_ORG_INPUT_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLabel;
class QLineEdit;
class QTextBrowser;
class QPushButton;

class FVAOrganizerInputDirPage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerInputDirPage();
	virtual ~FVAOrganizerInputDirPage(){};

protected:

	virtual bool validatePage();
	virtual bool isComplete() const;

	private slots:
	void OnDirButtonClicked();

private:

	QLabel*			inputDirLabel;
	QLineEdit*		inputDirLineEdit;
	QTextBrowser*	logOutput;
	QPushButton*	dirButton;
};
#endif // _FVA_ORG_INPUT_PAGE_H_