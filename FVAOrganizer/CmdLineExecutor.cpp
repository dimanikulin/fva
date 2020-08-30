#include "CmdLineExecutor.h"
#include "CmdLineTasks.h"
#include "CmdLineMyTasks.h"
#include "CmdLineDBTasks.h"
#include "CmdLineOldTasks.h"
#include "CmdLineCSVTasks.h"

#include "CLTRenameFiles.h"
#include "CLTCreateDirStructByDevNames.h"
#include "CLTFixDuplicatedFileNames.h"
#include "CLTPrintFSStructure.h"

#include "CLTAutoChecks3.h"
#include "CLTAutoChecks2.h"

#include "CLTDBCreateFVA.h"

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
	else if (name == CLT_Device_Name_Check::Name())			r.reset(new CLT_Device_Name_Check(dir,readOnly));
	else if (name == CLT_Files_Rename_By_Dir::Name())		r.reset(new CLT_Files_Rename_By_Dir(dir,readOnly));
	else if (name == CLT_Video_Rename_By_Sequence::Name())	r.reset(new CLT_Video_Rename_By_Sequence(dir,readOnly));
	else if (name == CLT_Dir_Name_Change::Name())			r.reset(new CLT_Dir_Name_Change(dir,readOnly));
	else if (name == CLT_Xml_Convert::Name())				r.reset(new CLT_Xml_Convert(dir,readOnly));
	else if (name == CLT_Update_File_Description::Name())	r.reset(new CLT_Update_File_Description(dir,readOnly));
	else if (name == CLTAutoChecks2::Name())				r.reset(new CLTAutoChecks2(dir,readOnly));
	else if (name == CLT_Alone_Files_Move::Name())			r.reset(new CLT_Alone_Files_Move(dir, readOnly));
	else if (name == CLT_Convert_Dir_Desc::Name())			r.reset(new CLT_Convert_Dir_Desc(dir, readOnly));
	else if (name == CLT_Convert_Amr::Name())				r.reset(new CLT_Convert_Amr(dir, readOnly));
	else if (name == CLT_Auto_Checks_1::Name())				r.reset(new CLT_Auto_Checks_1(dir, readOnly));
	else if (name == CLT_Folder_Merging::Name())			r.reset(new CLT_Folder_Merging (dir, readOnly, custom)); 
	else if (name == CLT_Set_File_Atts::Name())				r.reset(new CLT_Set_File_Atts(dir, readOnly));	
	else if (name == CLTPrintFSStructure::Name())			r.reset(new CLTPrintFSStructure(dir, readOnly));
	else if (name == CLT_Rename_File_By_Desc::Name())		r.reset(new CLT_Rename_File_By_Desc(dir, readOnly));
	else if (name == CLT_Fs_To_SQL::Name())					r.reset(new CLT_Fs_To_SQL(dir, readOnly));
	else if (name == CLT_Fva_File_To_SQL::Name())			r.reset(new CLT_Fva_File_To_SQL(dir, readOnly));
	else if (name == CLT_Fva_Folder_To_SQL::Name())			r.reset(new CLT_Fva_Folder_To_SQL(dir, readOnly));
	else if (name == CLTDBCreateFVA::Name())				r.reset(new CLTDBCreateFVA(dir, readOnly, custom));
	else if (name == CLT_1_Event_Folder_Merging::Name())	r.reset(new CLT_1_Event_Folder_Merging(dir, readOnly, custom));
	else if (name == CLT_Fva_Folder_2_CSV::Name() )			r.reset(new CLT_Fva_Folder_2_CSV(dir, readOnly, custom));
	else if (name == CLT_Fva_Files_2_CSV::Name() )			r.reset(new CLT_Fva_Files_2_CSV(dir, readOnly, custom));
	else if (name == CLT_1_Day_Event_Folder_Merging::Name())r.reset(new CLT_1_Day_Event_Folder_Merging(dir, readOnly));
	else if (name == CLT_Get_Fva_Dir_Type::Name())			r.reset(new CLT_Get_Fva_Dir_Type(dir, readOnly));
	else if (name == CLTAutoChecks3::Name())				r.reset(new CLTAutoChecks3(dir, readOnly));
	else if (name == CLTFixDuplicatedFileNames::Name())		r.reset(new CLTFixDuplicatedFileNames(dir, readOnly));

	return r;
}