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

#define _CLASS_TASK_DEFINITION(name,supReadOnly) class name : public CmdLineBaseTask \
{\
public:\
	name(const QString& dir_,bool readOnly_=false,const QString& custom_="")\
	:CmdLineBaseTask( dir_,readOnly_,custom_){qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;}\
	~name() {qDebug()<<"["<<Name().toUpper()<<"]cmd deleted, dir:"<<m_folder;}\
	virtual FVA_ERROR_CODE execute();\
	static QString Name(){return #name;}\
	virtual bool supportReadOnly () {return supReadOnly;}\
};\

// name, support read only mode, custom parameter
_CLASS_TASK_DEFINITION(CLT_Dir_Struct_Create_By_File,false)
_CLASS_TASK_DEFINITION(CLT_Dir_Struct_Create_By_Device_Name,false)
_CLASS_TASK_DEFINITION(CLT_Files_Rename,false)
_CLASS_TASK_DEFINITION(CLT_Device_Name_Check,false)
_CLASS_TASK_DEFINITION(CLT_Files_Rename_By_Dir,false)
_CLASS_TASK_DEFINITION(CLT_Video_Rename_By_Sequence,false)
_CLASS_TASK_DEFINITION(CLT_Dir_Name_Change,false)
_CLASS_TASK_DEFINITION(CLT_Alone_Files_Move,true)


class CLT_Auto_Checks_2 : public CmdLineBaseTask
{
public:
	CLT_Auto_Checks_2(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	:CmdLineBaseTask( dir_,readOnly_,custom_){qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;}
	~CLT_Auto_Checks_2();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Auto_Checks_2";}
	virtual bool supportReadOnly () {return false;}

private: // data

	/*!
	 * count of files in each subfolder
	 */
	QMap<QString, unsigned int>		m_fileCount;
};

#define LOG_QWARN qWarning()<<"["<<Name().toUpper()<<"]"
#define LOG_QCRIT qCritical()<<"["<<Name().toUpper()<<"]"
#define LOG_QDEB qDebug()<<"["<<Name().toUpper()<<"]"


#endif // _CMD_LINE_TASKS_H_