#ifndef _FVA_DATA_PROCESSOR_H_
#define _FVA_DATA_PROCESSOR_H_

#include "fvaconfiguration.h"
#include "fvacltcontext.h"

class FVADataProcessor
{
public:

	/*!
	* \brief ctors-dtors section
	*/
	FVADataProcessor(){};
	virtual ~FVADataProcessor(){};

	/*!
	* \brief performs running of command
	* \param context - one command parameters (environment)
	* \param cfg - system configuration, applicable for whole system
	* \return it returns result code (not 0 if error happened, 0 - if no error happened)
	*/
	static FVA_EXIT_CODE run(const CLTContext& context, const FvaConfiguration& cfg);
};
#endif //_FVA_DATA_PROCESSOR_H_