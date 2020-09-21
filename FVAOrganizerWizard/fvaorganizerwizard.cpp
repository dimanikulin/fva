#include "fvaorganizerwizard.h"

#include "FVAOrganizerDonePage.h"
#include "FVAOrganizerStartPage.h"
#include "FVAOrganizerOrientPage.h"
#include "FVAOrganizerInputDirPage.h"
#include "FVAOrganizerDevicePage.h"
#include "FVAOrganizerOutputDirPage.h"

FVAOrganizerWizard::FVAOrganizerWizard(QWidget *parent)
	: QWizard(parent)
{
	setPage( 0,	new FVAOrganizerStartPage );
	setPage( 1,	new FVAOrganizerInputDirPage );
	setPage( 2,	new FVAOrganizerOrientPage );
	setPage( 3, new	FVAOrganizerDevicePage );
	setPage( 4,	new FVAOrganizerOutputDirPage );
    setPage( 5,	new FVAOrganizerDonePage );	
	
	setStartId( 0 );

#ifndef Q_OS_MACX
	setWizardStyle(ModernStyle);    
#endif

	setOption(QWizard::IndependentPages);
	setOption(QWizard::DisabledBackButtonOnLastPage);

	QIcon	icon	= QIcon (QCoreApplication::applicationDirPath() + "/Icons/main.png");
	setWindowIcon(icon);
}

FVAOrganizerWizard::~FVAOrganizerWizard()
{

}
