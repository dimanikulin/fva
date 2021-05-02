#include "fvaorganizerwizard.h"

#include "fvacommonui.h"

#include "FVAOrganizerDonePage.h"
#include "FVAOrganizerStartPage.h"
#include "FVAOrganizerOrientPage.h"
#include "FVAOrganizerInputDirPage.h"
#include "FVAOrganizerDevicePage.h"
#include "FVAOrganizerOutputDirPage.h"



FVAOrganizerWizard::FVAOrganizerWizard(QWidget *parent)
	: QWizard(parent)
{
	FvaConfiguration cfg;

	FVA_EXIT_CODE res = cfg.load(QCoreApplication::applicationDirPath() + "/fvaParams.csv");
	if (FVA_NO_ERROR != res)
	{
		FVA_MESSAGE_BOX("cfg.load failed with error " + QString::number(res));
		return;
	}
	int index = 0;
	setPage(index++, new FVAOrganizerStartPage);
	setPage(index++, new FVAOrganizerInputDirPage);
	// do we need to show OrientPage?
	bool temp;
	res = cfg.getParamAsBoolean("Common::CheckOrientation", temp);
	RET_IF_RES_IS_ERROR
	if (temp)
		setPage(index++, new FVAOrganizerOrientPage);
	
	// do we need to show device page?
	res = cfg.getParamAsBoolean("Search::Device", temp);
	RET_IF_RES_IS_ERROR
	if (temp)
		setPage(index++, new FVAOrganizerDevicePage);
	setPage(index++, new FVAOrganizerOutputDirPage);
	setPage(index++, new FVAOrganizerDonePage);
	
	setStartId( 0 );

#ifndef Q_OS_MACX
	setWizardStyle(ModernStyle);    
#endif

	setOption(QWizard::IndependentPages);
	setOption(QWizard::DisabledBackButtonOnLastPage);

	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);

#ifdef  FVA_LANGUAGE_RUS
	setWindowTitle(tr("Cистема организации медиа-контента"));
#else 
#ifdef  FVA_LANGUAGE_ENG
	setWindowTitle(tr("FVA Software"));
#endif // FVA_LANGUAGE_ENG
#endif // FVA_LANGUAGE_RUS
	
}

FVAOrganizerWizard::~FVAOrganizerWizard()
{

}
