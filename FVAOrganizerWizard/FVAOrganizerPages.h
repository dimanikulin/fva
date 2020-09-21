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
