#include "CmdLineExecutor.h"

#include <QtCore/qdebug>
#include <QtCore/qdir>

#include "CLTRenameFilesByDir.h"
#include "CLTRenameFiles.h"
#include "CLTRenameDirs.h"
#include "CLTRenameVideoBySequence.h"
#include "CLTCreateDirStructByDevNames.h"
#include "CLTFixDuplicatedFileNames.h"
#include "CLTPrintFSStructure.h"

#include "CLTGetFvaDirType.h"
#include "CLTMerge1DayEventDir.h"
#include "CLTMerge1EventDir.h"
#include "CLTSetFileAtts.h"
#include "CLTConvertAmr.h"
#include "CLTMoveAloneFiles.h"
#include "CLTCheckDeviceName.h"
#include "CLTCreateDirStructByFileNames.h"

#include "CLTAutoChecks1.h"
#include "CLTAutoChecks2.h"
#include "CLTAutoChecks3.h"

#include "CLTCSVFvaFiles.h"

FVA_EXIT_CODE CmdLineExecutor::run(const CLTContext& context, const FvaConfiguration& cfg)
{
	QDir dir(context.dir);

	if (!dir.exists(context.dir))
	{
		qCritical() << "[ERR]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"[CMD_LINE_EXECUTOR]input folder does not exist";
		return FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG;
	}

	std::auto_ptr<CmdLineBaseTask> task(createTaskByName(context, cfg));
	if ( !task.get() )
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[CMD_LINE_EXECUTOR]uknown command:" << context.cmdType;
		return FVA_ERROR_UKNOWN_CMD;
	}
	if (context.readOnly && !task->supportReadOnly())
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[CMD_LINE_EXECUTOR]command does not support readonly:" << context.cmdType;
		return FVA_ERROR_NOT_SUPPORTED_RO_MODE;
	}
	
	// if it is recursive command
	if (context.recursive)
	{
		qWarning() << "[DBG]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[CMD_LINE_EXECUTOR] RECURSIVE mode for cmd:" << context.cmdType;
		return task->processFolderRecursivly(context.dir);
	}
	else
		return task->execute();
}
std::auto_ptr<CmdLineBaseTask> CmdLineExecutor::createTaskByName(const CLTContext& context, const FvaConfiguration& cfg)
{
	std::auto_ptr<CmdLineBaseTask> r(0);
	
	if (context.cmdType == CLTCreateDirStructByFileNames::Name())	r.reset(new CLTCreateDirStructByFileNames(context, cfg));
	else if (context.cmdType == CLTCreateDirStructByDeviceName::Name())r.reset(new CLTCreateDirStructByDeviceName(context, cfg));
	else if (context.cmdType == CLTRenameFiles::Name())				r.reset(new CLTRenameFiles(context, cfg));
	else if (context.cmdType == CLTCheckDeviceName::Name())			r.reset(new CLTCheckDeviceName(context, cfg));
	else if (context.cmdType == CLTRenameFilesByDir::Name())		r.reset(new CLTRenameFilesByDir(context, cfg));
	else if (context.cmdType == CLTRenameVideoBySequence::Name())	r.reset(new CLTRenameVideoBySequence(context, cfg));
	else if (context.cmdType == CLTRenameDirs::Name())				r.reset(new CLTRenameDirs(context, cfg));
	else if (context.cmdType == CLTAutoChecks2::Name())				r.reset(new CLTAutoChecks2(context, cfg));
	else if (context.cmdType == CLTMoveAloneFiles::Name())			r.reset(new CLTMoveAloneFiles(context, cfg));
	else if (context.cmdType == CLTConvertAmr::Name())				r.reset(new CLTConvertAmr(context, cfg));
	else if (context.cmdType == CLTAutoChecks1::Name())				r.reset(new CLTAutoChecks1(context, cfg));
	else if (context.cmdType == CLTSetFileAtts::Name())				r.reset(new CLTSetFileAtts(context, cfg));
	else if (context.cmdType == CLTPrintFSStructure::Name())		r.reset(new CLTPrintFSStructure(context, cfg));
	else if (context.cmdType == CLTMerge1EventDir::Name())			r.reset(new CLTMerge1EventDir(context, cfg));
	else if (context.cmdType == CLTCSVFvaFile::Name())				r.reset(new CLTCSVFvaFile(context, cfg));
	else if (context.cmdType == CLTMerge1DayEventDir::Name())		r.reset(new CLTMerge1DayEventDir(context, cfg));
	else if (context.cmdType == CLTGetFvaDirType::Name())			r.reset(new CLTGetFvaDirType(context, cfg));
	else if (context.cmdType == CLTAutoChecks3::Name())				r.reset(new CLTAutoChecks3(context, cfg));
	else if (context.cmdType == CLTFixDuplicatedFileNames::Name())	r.reset(new CLTFixDuplicatedFileNames(context, cfg));

	return r;
}