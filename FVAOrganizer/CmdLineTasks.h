#ifndef _CMD_LINE_TASKS_H_
#define _CMD_LINE_TASKS_H_

#include <QString>
#include <QDebug>

#include "CmdLineBaseTask.h"

extern QMap< unsigned int , unsigned int > sizes; 

/*!
 * \brief custom paramater value for renaming files with old names
 */
const QString OLD_FILE_NAME_PARAM = "OLD_NAME";

#define _CLASS_TASK_DECLARATION(name,supReadOnly) class name : public CmdLineBaseTask \
{\
public:\
	name(const QString& dir_,bool readOnly_=false,const QString& custom_="")\
	:CmdLineBaseTask( dir_,readOnly_,custom_){qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;}\
	virtual ~name() {qDebug()<<"["<<Name().toUpper()<<"]cmd deleted, dir:"<<m_folder;}\
	virtual FVA_ERROR_CODE execute();\
	static QString Name(){return #name;}\
	virtual bool supportReadOnly () {return supReadOnly;}\
};\

// name, support read only mode
/*!
 * \brief it creates folder structure by days folders, using file names in input folder
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Struct_Create_By_File,true)
/*!
 * \brief it creates folder structure using device names in input folder
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Struct_Create_By_Device_Name,false)
_CLASS_TASK_DECLARATION(CLT_Files_Rename,false)
_CLASS_TASK_DECLARATION(CLT_Device_Name_Check,false)
_CLASS_TASK_DECLARATION(CLT_Files_Rename_By_Dir,false)
_CLASS_TASK_DECLARATION(CLT_Video_Rename_By_Sequence,false)
_CLASS_TASK_DECLARATION(CLT_Auto_Checks_1, false)
_CLASS_TASK_DECLARATION(CLT_Convert_Amr, false)
_CLASS_TASK_DECLARATION(CLT_Folder_Merging, false)
_CLASS_TASK_DECLARATION(CLT_Set_File_Atts, false)

/*!
 * \brief it renames folders with replacing "-" to "."
 */
_CLASS_TASK_DECLARATION(CLT_Dir_Name_Change,false)

/*!
 * \brief it moves alone (or twins) files to parent folder, with adding their description into parent folder DESCRIPTION_FILE_NAME
 */
_CLASS_TASK_DECLARATION(CLT_Alone_Files_Move,true)

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

class CLT_Auto_Checks_2 : public CmdLineBaseTask
{
public:
	CLT_Auto_Checks_2(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	:CmdLineBaseTask( dir_,readOnly_,custom_){qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;}
	virtual ~CLT_Auto_Checks_2();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Auto_Checks_2";}
	virtual bool supportReadOnly () {return false;}

private: // data

	/*!
	 * count of files in each subfolder
	 */
	QMap<QString, unsigned int>		m_fileCount;
};

#define LOG_QWARN qWarning()<<"[WRN]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()<<"["<<Name().toUpper()<<"]"
#define LOG_QCRIT qCritical()<<"[ERR]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()<<"["<<Name().toUpper()<<"]"
#define LOG_QDEB qDebug()<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()<<"["<<Name().toUpper()<<"]"

#endif // _CMD_LINE_TASKS_H_