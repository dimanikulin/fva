/*!
* \file CmdLineBaseExecutor.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CmdLineBaseExecutor.h"

#include <QtCore/QDir>

FVA_EXIT_CODE CmdLineBaseExecutor::run(const CLTContext& context, const FvaConfiguration& cfg)
{
	QDir dir(context.dir);

	if (!dir.exists(context.dir))
	{
		LOG_CRIT << "input folder does not exist";
		return FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG;
	}

	std::unique_ptr<CmdLineBaseTask> task(createTaskByName(context, cfg));
	if (!task.get())
	{
		LOG_CRIT << "uknown command:" << context.cmdType;
		return FVA_ERROR_UKNOWN_CMD;
	}
	LOG_DEB << "cmd created,dir:" << context.dir
		<< ",RO=" << (context.readOnly ? "yes" : "no")
		<< ",REC=" << (context.recursive ? "yes" : "no");

	if (context.readOnly && !task->supportReadOnly())
	{
		LOG_CRIT << "command does not support readonly:" << context.cmdType;
		return FVA_ERROR_NOT_SUPPORTED_RO_MODE;
	}

	// if it is recursive command
	if (context.recursive)
	{
		LOG_WARN << "RECURSIVE mode for cmd:" << context.cmdType;
		// TODO to make right conversion from QString to std::string
		return task->processFolderRecursivly(context.dir.toStdString(), context);
	}
	else
		return task->execute(context);
}