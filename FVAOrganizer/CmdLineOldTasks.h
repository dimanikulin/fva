#ifndef _CMD_LINE_OLD_TASKS_H_
#define _CMD_LINE_OLD_TASKS_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it renames folders with replacing "-" to "."
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Name_Change,false)

/*!
 * \brief it creates folder structure by days folders, using file names in input folder - old impl
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Struct_Create_By_File_Old,true)

/*!
 * \brief it moves alone (or twins) files to parent folder, with adding their description into parent folder DESCRIPTION_FILE_NAME - old impl
 */
_CLASS_TASK_DECLARATION(CLT_Alone_Files_Move_Old,true)

/*!
 * \brief old impl of CLT_Folder_Merging
 */
_CLASS_TASK_DECLARATION(CLT_Folder_Merging_Old, false)

_CLASS_TASK_DECLARATION(CLT_Folder_Merging, false)

#endif // _CMD_LINE_OLD_TASKS_H_