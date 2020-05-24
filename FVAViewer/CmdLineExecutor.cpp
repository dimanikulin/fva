#include "CmdLineExecutor.h"
#include "CmdLineTasks.h"
#include <qdebug>

CmdLineExecutor::CmdLineExecutor( const QStringList& argumets )
	: argList ( argumets )
{
}

CmdLineExecutor::~CmdLineExecutor( )
{
}

int CmdLineExecutor::run( )
{
	if ( argList.size() < 3 )
	{
		qCritical() << "[CMD_LINE_EXECUTOR]not enough arguments";
		return 1001;
	}
	std::auto_ptr<CmdLineBaseTask> task = CmdLineBaseTask::createTaskByType( argList[1] /*cmdType*/, argList[2] /*cmdArg*/ ); 
	if ( !task.get() )
	{
		qCritical() << "[CMD_LINE_EXECUTOR]uknown command type:"<< argList[1];
		return 1010;
	}

	return task->execute();
}
