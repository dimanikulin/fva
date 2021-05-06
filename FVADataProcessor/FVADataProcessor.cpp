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

	return r;
}