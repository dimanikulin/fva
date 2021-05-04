#ifndef _FVA_DATA_PROCESSOR_H_
#define _FVA_DATA_PROCESSOR_H_

#include "CmdLineBaseExecutor.h"

class FVADataProcessor : public CmdLineBaseExecutor
{
private: // method

	std::auto_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg);

};
#endif //_FVA_DATA_PROCESSOR_H_