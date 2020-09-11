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
#include "CLTMergeFewDaysFewsEventsDir.h"
#include "CLTSetFileAtts.h"
#include "CLTConvertAmr.h"
#include "CLTMoveAloneFiles.h"
#include "CLTCheckDeviceName.h"
#include "CLTCreateDirStructByFileNames.h"
#include "CLTXmlConvert.h"
#include "CLTJSONConvertDirDesc.h"

#include "CLTAutoChecks1.h"
#include "CLTAutoChecks2.h"
#include "CLTAutoChecks3.h"

#include "CLTDBCreateFVA.h"
#include "CLTDBFvaFileToDB.h"
#include "CLTDBFvaFolderToDB.h"
#include "CLTDBFsToDB.h"

#include "CLTCSVFvaFiles.h"
#include "CLTCSVUpdateFvaFile.h"
#include "CLTCSVRenameFilesByFva.h"

CmdLineExecutor::CmdLineExecutor( const QString& cmdName, const QString& folder, bool recursive, bool readOnly, const QString& custom )
	:	m_cmdName	( cmdName ), 
		m_folder	( folder ),
		m_recursive	( recursive ),
		m_readOnly	( readOnly ),
		m_custom	( custom )
{
}

CmdLineExecutor::~CmdLineExecutor( )
{
}

FVA_EXIT_CODE CmdLineExecutor::run( )
{
	QDir dir( m_folder );

	if ( !dir.exists( m_folder ) )
	{
		qCritical() << "[ERR]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"[CMD_LINE_EXECUTOR]input folder does not exist";
		return FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG;
	}

	std::auto_ptr<CmdLineBaseTask> task ( createTaskByName( m_cmdName, m_folder, m_readOnly, m_custom ) );
	if ( !task.get() )
	{
		qCritical() << "[ERR]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"[CMD_LINE_EXECUTOR]uknown command:"<< m_cmdName;
		return FVA_ERROR_UKNOWN_CMD;
	}
	if ( m_readOnly && !task->supportReadOnly() )
	{
		qCritical() << "[ERR]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"[CMD_LINE_EXECUTOR]command does not support readonly:"<< m_cmdName;
		return FVA_ERROR_NOT_SUPPORTED_RO_MODE;
	}
	
	// if it is recursive command
	if ( m_recursive )
	{
		qWarning() <<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"[CMD_LINE_EXECUTOR] RECURSIVE mode for cmd:"<< m_cmdName;
		return task->processFolderRecursivly(m_folder);
	}
	else
		return task->execute();
}
std::auto_ptr<CmdLineBaseTask> CmdLineExecutor::createTaskByName(const QString& name, const QString& dir, bool readOnly, const QString& custom)
{
	std::auto_ptr<CmdLineBaseTask> r(0);
	
	if (name == CLTCreateDirStructByFileNames::Name())		r.reset(new CLTCreateDirStructByFileNames(dir, readOnly, custom));
	else if (name == CLTCreateDirStructByDeviceName::Name())r.reset(new CLTCreateDirStructByDeviceName(dir, readOnly));
	else if (name == CLTRenameFiles::Name())				r.reset(new CLTRenameFiles(dir, readOnly, custom));	
	else if (name == CLTCheckDeviceName::Name())			r.reset(new CLTCheckDeviceName(dir,readOnly));
	else if (name == CLTRenameFilesByDir::Name())			r.reset(new CLTRenameFilesByDir(dir, readOnly));
	else if (name == CLTRenameVideoBySequence::Name())		r.reset(new CLTRenameVideoBySequence(dir, readOnly));
	else if (name == CLTRenameDirs::Name())					r.reset(new CLTRenameDirs(dir, readOnly));
	else if (name == CLTXmlConvert::Name())					r.reset(new CLTXmlConvert(dir,readOnly));
	else if (name == CLTCSVUpdateFvaFile::Name())			r.reset(new CLTCSVUpdateFvaFile(dir, readOnly));
	else if (name == CLTAutoChecks2::Name())				r.reset(new CLTAutoChecks2(dir,readOnly));
	else if (name == CLTMoveAloneFiles::Name())				r.reset(new CLTMoveAloneFiles(dir, readOnly));
	else if (name == CLTJSONConvertDirDesc::Name())			r.reset(new CLTJSONConvertDirDesc(dir, readOnly));
	else if (name == CLTConvertAmr::Name())					r.reset(new CLTConvertAmr(dir, readOnly));
	else if (name == CLTAutoChecks1::Name())				r.reset(new CLTAutoChecks1(dir, readOnly));
	else if (name == CLTMergeFewDaysFewsEventsDir::Name())	r.reset(new CLTMergeFewDaysFewsEventsDir(dir, readOnly, custom));
	else if (name == CLTSetFileAtts::Name())				r.reset(new CLTSetFileAtts(dir, readOnly));	
	else if (name == CLTPrintFSStructure::Name())			r.reset(new CLTPrintFSStructure(dir, readOnly));
	else if (name == CLTCSVRenameFilesByFva::Name())		r.reset(new CLTCSVRenameFilesByFva(dir, readOnly));
	else if (name == CLTDBFsToDB::Name())					r.reset(new CLTDBFsToDB(dir, readOnly));
	else if (name == CLTDBFvaFileToDB::Name())				r.reset(new CLTDBFvaFileToDB(dir, readOnly));
	else if (name == CLTDBFvaFolderToDB::Name())			r.reset(new CLTDBFvaFolderToDB(dir, readOnly));
	else if (name == CLTDBCreateFVA::Name())				r.reset(new CLTDBCreateFVA(dir, readOnly, custom));
	else if (name == CLTMerge1EventDir::Name())				r.reset(new CLTMerge1EventDir(dir, readOnly, custom));
	else if (name == CLTCSVFvaFile::Name())					r.reset(new CLTCSVFvaFile(dir, readOnly, custom));
	else if (name == CLTMerge1DayEventDir::Name())			r.reset(new CLTMerge1DayEventDir(dir, readOnly, custom));
	else if (name == CLTGetFvaDirType::Name())				r.reset(new CLTGetFvaDirType(dir, readOnly));
	else if (name == CLTAutoChecks3::Name())				r.reset(new CLTAutoChecks3(dir, readOnly));
	else if (name == CLTFixDuplicatedFileNames::Name())		r.reset(new CLTFixDuplicatedFileNames(dir, readOnly));

	return r;
}