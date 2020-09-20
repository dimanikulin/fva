#ifndef _FVA_ORGANIZER_PAGES_H_
#define _FVA_ORGANIZER_PAGES_H_ 

#include <QtWidgets/QWizardPage>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>

/*!
 * \brief keeps all organizer pages functionality
 */



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

	private:

		QLabel*			outputDirLabel;
		QLineEdit*		outputDirLineEdit;
		QPushButton*	dirButton;

		QRadioButton*	oneEventOneDay;

		QRadioButton*	severalEventsOneDay;

		QRadioButton*	severalEventsSeveralDays;

		QRadioButton*	oneEventSeveralDays;
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
		virtual bool	isComplete() const;

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
