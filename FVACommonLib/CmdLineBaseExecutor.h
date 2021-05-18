/*!
* \file CmdLineBaseExecutor.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef _CMD_LINE_BASE_EXECUTOR_H_
#define _CMD_LINE_BASE_EXECUTOR_H_

#include "CmdLineBaseTask.h"
#include "fvaconfiguration.h"
#include "fvacltcontext.h"

/*!
* \brief performs execution for command line tasks 
*/
class CmdLineBaseExecutor
{
public:

	/*!
	* \brief performs running of command
	* \param context - one command parameters (environment)
	* \param cfg - system configuration, applicable for whole system
	* \return it returns result code (not 0 if error happened, 0 - if no error happened)
	*/
	FVA_EXIT_CODE run(const CLTContext& context, const FvaConfiguration& cfg);

private: // method

	/*!
	* \brief factory to create the tasks
	* \param context - one command parameters (environment)
	* \param cfg - system configuration, applicable for whole system
	* \return pointer to created task or null if type is not supported
	*/
	virtual std::auto_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg) = 0;

};
#endif // _CMD_LINE_BASE_EXECUTOR_H_