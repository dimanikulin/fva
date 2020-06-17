#ifndef _FVA_ORGANIZER_PAGES_H_
#define _FVA_ORGANIZER_PAGES_H_ 

#include <QtGui/QWizardPage>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>


/*!
 * \brief keeps all organizer pages functionality
 */


class FVAOrganizerStartPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerStartPage();
		virtual ~FVAOrganizerStartPage(){};

	private:

		QTextBrowser*	helloWords;
};

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

class FVAOrganizerInputDirPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerInputDirPage();
		virtual ~FVAOrganizerInputDirPage(){};

	protected:

		virtual bool	validatePage ();

	private slots:
		void OnDirButtonClicked();

	private:

		QLabel*			inputDirLabel;
		QLineEdit*		inputDirLineEdit;
		QTextBrowser*	logOutput;
		QPushButton*	dirButton;
};

class FVAOrganizerDonePage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerDonePage();
		virtual ~FVAOrganizerDonePage(){};

	private:

		QTextBrowser*	finishWords;

};

#endif // _FVA_ORGANIZER_PAGES_H_
