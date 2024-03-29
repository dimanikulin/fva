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

#include "CLTRenameVideoBySequence.h"
#include "CLTCreateDirStructByDevNames.h"

#include "CLTCheckDeviceName.h"
#include "CLTAutoChecks1.h"
#include "CLTCheckFileFormat.h"
#include "CLTCheckLocation.h"
#include "CLTCheckDateTime.h"

#include "CLTRenameFiles.h"
#include "CLTFixDuplicatedFileNames.h"
#include "CLTPrintFSStructure.h"

#include "CLTMoveInputDir2Output.h"
#include "CLTSetFileAtts.h"
#include "CLTMoveAloneFiles.h"
#include "CLTCreateDirStructByFileNames.h"

#include "CLTAutoChecks2.h"
#include "CLTAutoChecks3.h"

#include "CLTCSVFvaFiles.h"
#include "CLTCSVGetTagsForFvaFiles.h"

std::unique_ptr<CmdLineBaseTask> FVADataProcessor::createTaskByName(const CLTContext& context, const FvaConfiguration& cfg)
{
	std::unique_ptr<CmdLineBaseTask> r(nullptr);

	if (context.cmdType == CLTCreateDirStructByDeviceName::Name())	r.reset(new CLTCreateDirStructByDeviceName());
	else if (context.cmdType == CLTCheckDeviceName::Name())		r.reset(new CLTCheckDeviceName());
	else if (context.cmdType == CLTRenameVideoBySequence::Name())	r.reset(new CLTRenameVideoBySequence(cfg));
	else if (context.cmdType == CLTAutoChecks1::Name())		r.reset(new CLTAutoChecks1(cfg));
	else if (context.cmdType == CLTCheckFileFormat::Name())		r.reset(new CLTCheckFileFormat());
	else if (context.cmdType == CLTCheckLocation::Name())		r.reset(new CLTCheckLocation());
	else if (context.cmdType == CLTCheckDateTime::Name())		r.reset(new CLTCheckDateTime(cfg));
	else if (context.cmdType == CLTCreateDirStructByFileNames::Name())	r.reset(new CLTCreateDirStructByFileNames());
	else if (context.cmdType == CLTRenameFiles::Name())		r.reset(new CLTRenameFiles(cfg));
	else if (context.cmdType == CLTAutoChecks2::Name())		r.reset(new CLTAutoChecks2(cfg));
	else if (context.cmdType == CLTMoveAloneFiles::Name())		r.reset(new CLTMoveAloneFiles(cfg));
	else if (context.cmdType == CLTSetFileAtts::Name())		r.reset(new CLTSetFileAtts());
	else if (context.cmdType == CLTPrintFSStructure::Name())	r.reset(new CLTPrintFSStructure(cfg));
	else if (context.cmdType == CLTMoveInputDir2Output::Name())	r.reset(new CLTMoveInputDir2Output(cfg));
	else if (context.cmdType == CLTCSVFvaFile::Name())		r.reset(new CLTCSVFvaFile(cfg));
	else if (context.cmdType == CLTAutoChecks3::Name())		r.reset(new CLTAutoChecks3(cfg));
	else if (context.cmdType == CLTFixDuplicatedFileNames::Name())	r.reset(new CLTFixDuplicatedFileNames(cfg));
	else if (context.cmdType == CLTCSVGetTagsForFvaFiles::Name())	r.reset(new CLTCSVGetTagsForFvaFiles(cfg));

	return r;
}