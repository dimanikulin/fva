#ifndef _CMD_LINE_TASKS_H_
#define _CMD_LINE_TASKS_H_

#include "CmdLineBaseTask.h"


/*!
 * \brief it creates folder structure by days folders, using file names in input folder
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Struct_Create_By_File,true)

_CLASS_TASK_DECLARATION(CLT_Files_Rename,false)
_CLASS_TASK_DECLARATION(CLT_Device_Name_Check,false)
_CLASS_TASK_DECLARATION(CLT_Video_Rename_By_Sequence,false)
_CLASS_TASK_DECLARATION(CLT_Auto_Checks_1, false)
_CLASS_TASK_DECLARATION(CLT_Convert_Amr, false)
_CLASS_TASK_DECLARATION(CLT_1_Event_Folder_Merging, false)
_CLASS_TASK_DECLARATION(CLT_Set_File_Atts, false)
_CLASS_TASK_DECLARATION(CLT_1_Day_Event_Folder_Merging, false)

/*!
 * \brief it moves alone (or twins) files to parent folder, with adding their description into parent folder DESCRIPTION_FILE_NAME
 */
_CLASS_TASK_DECLARATION(CLT_Alone_Files_Move,true)


class CLT_Auto_Checks_2 : public CmdLineBaseTask
{
public:
	CLT_Auto_Checks_2(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	:CmdLineBaseTask( dir_,readOnly_,custom_){qWarning()<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_<<",RO=" << (readOnly_?"yes":"no")<<",SRO=" << (supportReadOnly()?"yes":"no"); }
	virtual ~CLT_Auto_Checks_2();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Auto_Checks_2";}
	virtual bool supportReadOnly() {return true;}

private: // data

	/*!
	 * count of files in each subfolder
	 */
	QMap<QString, unsigned int>		m_fileCount;

	/*!
	 * issues found, divided by type (in string format)
	 */	
	QVector<QString>				m_Issues;

	/*!
	* file names to file pathes
	*/
	QMap<QString, QString>			m_uniqueFileNames;

};

#endif // _CMD_LINE_TASKS_H_