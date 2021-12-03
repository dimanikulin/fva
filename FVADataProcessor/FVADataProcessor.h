/*!
* \file FVADataProcessor.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_DATA_PROCESSOR_H_
#define _FVA_DATA_PROCESSOR_H_

#include <memory>
#include "../FVACommonLib/CmdLineBaseExecutor.h"

/*!
* \brief FVADataProcessor is child of CmdLineBaseExecutor and 
* 1. creates the CLT command (child of CmdLineBaseTask);
* 2. drives CmdLineBaseTask's child execution per external requests.

* This class implements "Controller" functions from MVC pattern and Class Factory functions as well. 
* Flow control is based on class FvaConfiguration.
*/
class FVADataProcessor : public CmdLineBaseExecutor
{
private: // method

	std::unique_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg);

};
#endif //_FVA_DATA_PROCESSOR_H_