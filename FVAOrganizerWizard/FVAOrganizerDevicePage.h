#ifndef _FVA_ORG_DEVICE_PAGE_H_
#define _FVA_ORG_DEVICE_PAGE_H_

#include <QtWidgets/QWizardPage>

class QLabel;
class QLineEdit;
class QTextBrowser;
class QPushButton;
class QComboBox;

class FVAOrganizerDevicePage : public QWizardPage
{
	Q_OBJECT
public:

	FVAOrganizerDevicePage();
	virtual ~FVAOrganizerDevicePage(){};

protected:

	virtual bool	validatePage();
	virtual void	setVisible(bool visible);
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


#endif // _FVA_ORG_DEVICE_PAGE_H_
