#ifndef _FVA_ORG_OUTPUT_PAGE_H_
#define _FVA_ORG_OUTPUT_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLineEdit;
class QCheckBox;

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
		void OnDigiKamDirButtonClicked();
		void OnGooglePhotoDirButtonClicked();

	private: //data

		QLineEdit* digiKamLineEdit;
		QLineEdit* googlePhotoLineEdit;

		QCheckBox* removeOriginDirCheckBox;
};

#endif // _FVA_ORG_OUTPUT_PAGE_H_