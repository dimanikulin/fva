#include "FVADataProcessor.h"

#include <QtCore/qdebug>
#include <QtCore/qdir>

#include "CLTRenameVideoBySequence.h"
#include "CLTCreateDirStructByDevNames.h"
#include "CLTConvertAmr.h"
#include "CLTCheckDeviceName.h"
#include "CLTAutoChecks1.h"
#include "CLTCheckFileFormat.h"
#include "CLTCheckLocation.h"
#include "CLTCheckDatetime.h"

#include "CLTRenameFiles.h"
#include "CLTFixDuplicatedFileNames.h"
#include "CLTPrintFSStructure.h"

#include "CLTGetFvaDirType.h"
#include "CLTMerge1DayEventDir.h"
#include "CLTMerge1EventDir.h"
#include "CLTSetFileAtts.h"
#include "CLTMoveAloneFiles.h"
#include "CLTCreateDirStructByFileNames.h"

#include "CLTAutoChecks2.h"
#include "CLTAutoChecks3.h"

#include "CLTCSVFvaFiles.h"

std::auto_ptr<CmdLineBaseTask> FVADataProcessor::createTaskByName(const CLTContext& context, const FvaConfiguration& cfg)
{
	std::auto_ptr<CmdLineBaseTask> r(nullptr);

	if (context.cmdType == CLTCreateDirStructByDeviceName::Name())	r.reset(new CLTCreateDirStructByDeviceName());
	else if (context.cmdType == CLTCheckDeviceName::Name())			r.reset(new CLTCheckDeviceName());
	else if (context.cmdType == CLTRenameVideoBySequence::Name())	r.reset(new CLTRenameVideoBySequence(cfg));
	else if (context.cmdType == CLTConvertAmr::Name())				r.reset(new CLTConvertAmr());
	else if (context.cmdType == CLTAutoChecks1::Name())				r.reset(new CLTAutoChecks1(cfg));
	else if (context.cmdType == CLTCheckFileFormat::Name())			r.reset(new CLTCheckFileFormat());
	else if (context.cmdType == CLTCheckLocation::Name())			r.reset(new CLTCheckLocation());
	else if (context.cmdType == CLTCheckDateTime::Name())			r.reset(new CLTCheckDateTime(cfg));
	else if (context.cmdType == CLTCreateDirStructByFileNames::Name())	r.reset(new CLTCreateDirStructByFileNames());
	else if (context.cmdType == CLTRenameFiles::Name())				r.reset(new CLTRenameFiles(cfg));
	else if (context.cmdType == CLTAutoChecks2::Name())				r.reset(new CLTAutoChecks2(cfg));
	else if (context.cmdType == CLTMoveAloneFiles::Name())			r.reset(new CLTMoveAloneFiles(cfg));
	else if (context.cmdType == CLTSetFileAtts::Name())				r.reset(new CLTSetFileAtts());
	else if (context.cmdType == CLTPrintFSStructure::Name())		r.reset(new CLTPrintFSStructure(cfg));
	else if (context.cmdType == CLTMerge1EventDir::Name())			r.reset(new CLTMerge1EventDir());
	else if (context.cmdType == CLTCSVFvaFile::Name())				r.reset(new CLTCSVFvaFile(cfg));
	else if (context.cmdType == CLTMerge1DayEventDir::Name())		r.reset(new CLTMerge1DayEventDir(cfg));
	else if (context.cmdType == CLTGetFvaDirType::Name())			r.reset(new CLTGetFvaDirType());
	else if (context.cmdType == CLTAutoChecks3::Name())				r.reset(new CLTAutoChecks3(cfg));
	else if (context.cmdType == CLTFixDuplicatedFileNames::Name())	r.reset(new CLTFixDuplicatedFileNames(cfg));

	return r;
}