/*!
* \file FVADataProcessor.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_DATA_PROCESSOR_H_
#define _FVA_DATA_PROCESSOR_H_

#include "CmdLineBaseExecutor.h"

/*!
* \brief it creates the CLT command and drives its execution
*/
class FVADataProcessor : public CmdLineBaseExecutor
{
private: // method

	std::auto_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg);

};
#endif //_FVA_DATA_PROCESSOR_H_