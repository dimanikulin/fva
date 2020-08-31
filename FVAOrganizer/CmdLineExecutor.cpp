#include "CmdLineExecutor.h"
#include "CmdLineTasks.h"
#include "CmdLineMyTasks.h"
#include "CmdLineOldTasks.h"

#include "CLTRenameFiles.h"
#include "CLTRenameVideoBySequence.h"
#include "CLTCreateDirStructByDevNames.h"
#include "CLTFixDuplicatedFileNames.h"
#include "CLTPrintFSStructure.h"
#include "CLTCSVFvaFiles.h"
#include "CLTGetFvaDirType.h"
#include "CLTMerge1DayEventDir.h"
#include "CLTMerge1EventDir.h"
#include "CLTSetFileAtts.h"
#include "CLTConvertAmr.h"
#include "CLTMoveAloneFiles.h"
#include "CLTCheckDeviceName.h"

#include "CLTAutoChecks1.h"
#include "CLTAutoChecks2.h"
#include "CLTAutoChecks3.h"

#include "CLTDBCreateFVA.h"
#include "CLTDBFvaFileToDB.h"
#include "CLTDBFvaFolderToDB.h"
#include "CLTDBFsToDB.h"

#include <QtCore/qdebug>
#include <QtCore/qdir>

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
	
	if ( name == CLT_Dir_Struct_Create_By_File::Name())		r.reset(new CLT_Dir_Struct_Create_By_File(dir, readOnly, custom));
	else if (name == CLTCreateDirStructByDeviceName::Name())r.reset(new CLTCreateDirStructByDeviceName(dir, readOnly));
	else if (name == CLTRenameFiles::Name())				r.reset(new CLTRenameFiles(dir, readOnly, custom));	
	else if (name == CLTCheckDeviceName::Name())			r.reset(new CLTCheckDeviceName(dir,readOnly));
	else if (name == CLT_Files_Rename_By_Dir::Name())		r.reset(new CLT_Files_Rename_By_Dir(dir,readOnly));
	else if (name == CLTRenameVideoBySequence::Name())		r.reset(new CLTRenameVideoBySequence(dir, readOnly));
	else if (name == CLT_Dir_Name_Change::Name())			r.reset(new CLT_Dir_Name_Change(dir,readOnly));
	else if (name == CLT_Xml_Convert::Name())				r.reset(new CLT_Xml_Convert(dir,readOnly));
	else if (name == CLT_Update_File_Description::Name())	r.reset(new CLT_Update_File_Description(dir,readOnly));
	else if (name == CLTAutoChecks2::Name())				r.reset(new CLTAutoChecks2(dir,readOnly));
	else if (name == CLTMoveAloneFiles::Name())				r.reset(new CLTMoveAloneFiles(dir, readOnly));
	else if (name == CLT_Convert_Dir_Desc::Name())			r.reset(new CLT_Convert_Dir_Desc(dir, readOnly));
	else if (name == CLTConvertAmr::Name())					r.reset(new CLTConvertAmr(dir, readOnly));
	else if (name == CLTAutoChecks1::Name())				r.reset(new CLTAutoChecks1(dir, readOnly));
	else if (name == CLT_Folder_Merging::Name())			r.reset(new CLT_Folder_Merging (dir, readOnly, custom)); 
	else if (name == CLTSetFileAtts::Name())				r.reset(new CLTSetFileAtts(dir, readOnly));	
	else if (name == CLTPrintFSStructure::Name())			r.reset(new CLTPrintFSStructure(dir, readOnly));
	else if (name == CLT_Rename_File_By_Desc::Name())		r.reset(new CLT_Rename_File_By_Desc(dir, readOnly));
	else if (name == CLTDBFsToDB::Name())					r.reset(new CLTDBFsToDB(dir, readOnly));
	else if (name == CLTDBFvaFileToDB::Name())				r.reset(new CLTDBFvaFileToDB(dir, readOnly));
	else if (name == CLTDBFvaFolderToDB::Name())			r.reset(new CLTDBFvaFolderToDB(dir, readOnly));
	else if (name == CLTDBCreateFVA::Name())				r.reset(new CLTDBCreateFVA(dir, readOnly, custom));
	else if (name == CLTMerge1EventDir::Name())				r.reset(new CLTMerge1EventDir(dir, readOnly, custom));
	else if (name == CLTCSVFvaFile::Name())					r.reset(new CLTCSVFvaFile(dir, readOnly, custom));
	else if (name == CLTMerge1DayEventDir::Name())			r.reset(new CLTMerge1DayEventDir(dir, readOnly));
	else if (name == CLTGetFvaDirType::Name())				r.reset(new CLTGetFvaDirType(dir, readOnly));
	else if (name == CLTAutoChecks3::Name())				r.reset(new CLTAutoChecks3(dir, readOnly));
	else if (name == CLTFixDuplicatedFileNames::Name())		r.reset(new CLTFixDuplicatedFileNames(dir, readOnly));

	return r;
}