#ifndef _CMD_LINE_OLD_TASKS_H_
#define _CMD_LINE_OLD_TASKS_H_

#include "CmdLineTasks.h"

/*!
 * \brief it creates folder structure using device names in input folder
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Struct_Create_By_Device_Name,false)
_CLASS_TASK_DECLARATION(CLT_Files_Rename_By_Dir,false)

/*!
 * \brief it renames folders with replacing "-" to "."
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Name_Change,false)

/*!
 * \brief it prints file system structure with adding info to compare with fs structure on remote PC 
 */
class CLT_Print_FS_Structure : public CmdLineBaseTask
{
public:
	CLT_Print_FS_Structure(const QString& dir_,bool readOnly_=false,const QString& custom_="");
	virtual ~CLT_Print_FS_Structure();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Print_FS_Structure";}
	virtual bool supportReadOnly () {return false;}

private: // data

		QFile m_file;
};

/*!
 * \brief it creates folder structure by days folders, using file names in input folder - old impl
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Struct_Create_By_File_Old,true)

#endif // _CMD_LINE_OLD_TASKS_H_