#ifndef _FVA_ORGANIZER_PAGES_H_
#define _FVA_ORGANIZER_PAGES_H_ 

#include <QtGui/QWizardPage>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

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

class FVAOrganizerOutputDirPage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerOutputDirPage();
		virtual ~FVAOrganizerOutputDirPage(){};

	protected:

		virtual bool	validatePage ();

	private slots:
		void OnDirButtonClicked();

	private:

		QLabel*			outputDirLabel;
		QLineEdit*		outputDirLineEdit;
		QPushButton*	dirButton;

		QTextBrowser*	logOutput;

		QCheckBox*		mergeCheckBox;
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

class FVAOrganizerDevicePage : public QWizardPage
{
	Q_OBJECT
	public:

		FVAOrganizerDevicePage();
		virtual ~FVAOrganizerDevicePage(){};

	protected:

		virtual bool	validatePage ();
		virtual void	setVisible( bool visible );

	protected slots:
		void OnChangeDictPressed();

	private:

		int deviceId;

		QTextBrowser*	logOutput;
		QLabel*			deviceLbl;
		QLineEdit*		deviceName;
		QLabel*			matchLbl;
		QLineEdit*		matchName;
		QLabel*			ownerLbl;
		QLineEdit*		ownerName;
		QComboBox*		cbDevice;
		QPushButton*	btnDct;		
};

#endif // _FVA_ORGANIZER_PAGES_H_
