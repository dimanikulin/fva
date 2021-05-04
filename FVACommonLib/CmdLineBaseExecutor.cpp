#include "CmdLineBaseExecutor.h"

#include <QtCore/qdebug>
#include <QtCore/qdir>

FVA_EXIT_CODE CmdLineBaseExecutor::run(const CLTContext& context, const FvaConfiguration& cfg)
{
	QDir dir(context.dir);

	if (!dir.exists(context.dir))
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[CMD_LINE_EXECUTOR]input folder does not exist";
		return FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG;
	}

	std::auto_ptr<CmdLineBaseTask> task(createTaskByName(context, cfg));
	if (!task.get())
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[CMD_LINE_EXECUTOR]uknown command:" << context.cmdType;
		return FVA_ERROR_UKNOWN_CMD;
	}
	qDebug() << "[DBG]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[" << context.cmdType.toUpper() << "]"
		<< "cmd created,dir:" << context.dir
		<< ",RO=" << (context.readOnly ? "yes" : "no")
		<< ",SRO=" << (context.readOnly ? "yes" : "no");

	if (context.readOnly && !task->supportReadOnly())
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[CMD_LINE_EXECUTOR]command does not support readonly:" << context.cmdType;
		return FVA_ERROR_NOT_SUPPORTED_RO_MODE;
	}

	// if it is recursive command
	if (context.recursive)
	{
		qWarning() << "[DBG]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[CMD_LINE_EXECUTOR] RECURSIVE mode for cmd:" << context.cmdType;
		return task->processFolderRecursivly(context.dir, context);
	}
	else
		return task->execute(context);
}