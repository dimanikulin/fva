#include "fvaorganizerwizard.h"
#include "FVAOrganizerPages.h"

FVAOrganizerWizard::FVAOrganizerWizard(QWidget *parent)
	: QWizard(parent)
{
	setPage( 0, new FVAOrganizerStartPage );
    setPage( 1,	new FVAOrganizerOrientPage );
	setPage( 2,	new FVAOrganizerInputDirPage );
    setPage( 3,	new FVAOrganizerDonePage );	
	
	setStartId( 0 );

#ifndef Q_OS_MACX
	setWizardStyle(ModernStyle);    
#endif

	setOption(QWizard::IndependentPages);
	setOption(QWizard::DisabledBackButtonOnLastPage);
}

FVAOrganizerWizard::~FVAOrganizerWizard()
{

}
