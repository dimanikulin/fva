/*!
 * \file FVADataProcessor.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "FVADataProcessor.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "CLTAutoChecks1.h"
#include "CLTAutoChecks2.h"
#include "CLTAutoChecks3.h"
#include "CLTCSVFvaFiles.h"
#include "CLTCSVGetTagsForFvaFiles.h"
#include "CLTCheckDateTime.h"
#include "CLTCheckDeviceName.h"
#include "CLTCheckFileFormat.h"
#include "CLTCheckLocation.h"
#include "CLTCreateDirStructByDevNames.h"
#include "CLTCreateDirStructByFileNames.h"
#include "CLTFixDuplicatedFileNames.h"
#include "CLTMoveAloneFiles.h"
#include "CLTMoveInputDir2Output.h"
#include "CLTPrintFSStructure.h"
#include "CLTRenameFiles.h"
#include "CLTRenameVideoBySequence.h"
#include "CLTSetFileAtts.h"

std::unique_ptr<CmdLineBaseTask> FVADataProcessor::createTaskByName(const CLTContext& context,
                                                                    const FvaConfiguration& cfg) {
    std::unique_ptr<CmdLineBaseTask> r(nullptr);
    const QString cmdType = QString::fromStdString(context.cmdType);

    if (cmdType == QString::fromStdString(CLTCreateDirStructByDeviceName::Name()))
        r.reset(new CLTCreateDirStructByDeviceName());
    else if (cmdType == QString::fromStdString(CLTCheckDeviceName::Name()))
        r.reset(new CLTCheckDeviceName());
    else if (cmdType == CLTRenameVideoBySequence::Name())
        r.reset(new CLTRenameVideoBySequence(cfg));
    else if (cmdType == QString::fromStdString(CLTAutoChecks1::Name()))
        r.reset(new CLTAutoChecks1(cfg));
    else if (cmdType == QString::fromStdString(CLTCheckFileFormat::Name()))
        r.reset(new CLTCheckFileFormat());
    else if (cmdType == CLTCheckLocation::Name())
        r.reset(new CLTCheckLocation());
    else if (cmdType == QString::fromStdString(CLTCheckDateTime::Name()))
        r.reset(new CLTCheckDateTime(cfg));
    else if (cmdType == QString::fromStdString(CLTCreateDirStructByFileNames::Name()))
        r.reset(new CLTCreateDirStructByFileNames());
    else if (cmdType == CLTRenameFiles::Name())
        r.reset(new CLTRenameFiles(cfg));
    else if (cmdType == QString::fromStdString(CLTAutoChecks2::Name()))
        r.reset(new CLTAutoChecks2(cfg));
    else if (cmdType == QString::fromStdString(CLTMoveAloneFiles::Name()))
        r.reset(new CLTMoveAloneFiles(cfg));
    else if (cmdType == QString::fromStdString(CLTSetFileAtts::Name()))
        r.reset(new CLTSetFileAtts());
    else if (cmdType == CLTPrintFSStructure::Name())
        r.reset(new CLTPrintFSStructure(cfg));
    else if (cmdType == QString::fromStdString(CLTMoveInputDir2Output::Name()))
        r.reset(new CLTMoveInputDir2Output(cfg));
    else if (cmdType == QString::fromStdString(CLTCSVFvaFile::Name()))
        r.reset(new CLTCSVFvaFile(cfg));
    else if (cmdType == CLTAutoChecks3::Name())
        r.reset(new CLTAutoChecks3(cfg));
    else if (cmdType == QString::fromStdString(CLTFixDuplicatedFileNames::Name()))
        r.reset(new CLTFixDuplicatedFileNames(cfg));
    else if (cmdType == QString::fromStdString(CLTCSVGetTagsForFvaFiles::Name()))
        r.reset(new CLTCSVGetTagsForFvaFiles(cfg));

    return r;
}
