#ifndef _FVA_ORG_OUTPUT_PAGE_H_
#define _FVA_ORG_OUTPUT_PAGE_H_

#include <QtWidgets/QWizardPage>
#include <QtWidgets/QRadioButton>

class QLabel;
class QLineEdit;
class QTextBrowser;
class QPushButton;
class QRaddioButton;

class FVAOrganizerOutputDirPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerOutputDirPage();
		virtual ~FVAOrganizerOutputDirPage(){};

	protected:

		virtual bool	validatePage ();
		virtual bool	isComplete() const;
		virtual void	setVisible(bool visible);

	private slots:
		void OnDirButtonClicked();
		void OnFvaInfoButtonPressed();

	private:

		QRadioButton*	oneEventOneDay;
		QPushButton*	fvaInfoButton;

		QRadioButton*	severalEventsOneDay;

		QRadioButton*	severalEventsSeveralDays;

		QRadioButton*	oneEventSeveralDays;
		QLabel*			outputDirLabel;
		QLineEdit*		outputDirLineEdit;
		QPushButton*	dirButton;

};

#endif // _FVA_ORG_OUTPUT_PAGE_H_