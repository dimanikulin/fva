/*!
 * \file CmdLineBaseExecutor.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "CmdLineBaseExecutor.h"
#include "fva_qt_port_2_stl.h"

FVA_EXIT_CODE CmdLineBaseExecutor::run(const CLTContext& context, const FvaConfiguration& cfg) {
    const std::string& dirPath = context.dir;

    if (!isExistingDirectory(dirPath)) {
        LOG_CRIT << "input folder does not exist:" << dirPath.c_str();
        return FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG;
    }

    std::unique_ptr<CmdLineBaseTask> task(createTaskByName(context, cfg));
    if (!task) {
        LOG_CRIT << "uknown command:" << context.cmdType.c_str();
        return FVA_ERROR_UKNOWN_CMD;
    }
    LOG_DEB << "cmd created,dir:" << dirPath.c_str() << ",RO=" << (context.readOnly ? "yes" : "no")
            << ",REC=" << (context.recursive ? "yes" : "no");

    if (context.readOnly && !task->supportReadOnly()) {
        LOG_CRIT << "command does not support readonly:" << context.cmdType.c_str();
        return FVA_ERROR_NOT_SUPPORTED_RO_MODE;
    }

    // if it is recursive command
    if (context.recursive) {
        LOG_WARN << "RECURSIVE mode for cmd:" << context.cmdType.c_str();
        return task->processFolderRecursivly(context.dir, context);
    } else
        return task->execute(context);
}
