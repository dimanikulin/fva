#ifndef _CMD_LINE_DB_TASKS_H_
#define _CMD_LINE_DB_TASKS_H_

#include "CmdLineTasks.h"

_CLASS_TASK_DECLARATION(CLT_Fva_File_To_SQL,false)
_CLASS_TASK_DECLARATION(CLT_Fsaudio_To_SQL,false)
_CLASS_TASK_DECLARATION(CLT_Fsvideo_To_SQL,false)
_CLASS_TASK_DECLARATION(CLT_Fsimage_To_SQL,false)

class CLT_Fs_To_SQL : public CmdLineBaseTask
{
public:
	CLT_Fs_To_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	:CmdLineBaseTask( dir_,readOnly_,custom_), m_skippedFiles(0)
	{
		qWarning()<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_<<",RO=" << (readOnly_?"yes":"no")<<",SRO=" << (supportReadOnly()?"yes":"no"); 
	}
	virtual ~CLT_Fs_To_SQL();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Fs_To_SQL";}
	virtual bool supportReadOnly() {return true;}

private: // data

	/*!
	 * SQL inserts
	 */
	QVector<QString>		m_inserts;

	/*!
	 * skipped files number
	 */
	int						m_skippedFiles;
};

class CLT_Fva_Folder_To_SQL : public CmdLineBaseTask
{
public:
	CLT_Fva_Folder_To_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	:CmdLineBaseTask( dir_,readOnly_,custom_), m_movedFolders(0)
	{
		qWarning()<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_<<",RO=" << (readOnly_?"yes":"no")<<",SRO=" << (supportReadOnly()?"yes":"no"); 
	}
	virtual ~CLT_Fva_Folder_To_SQL();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Fva_Folder_To_SQL";}
	virtual bool supportReadOnly() {return true;}

private: // data

	/*!
	 * SQL inserts
	 */
	QVector<QString>		m_inserts;

	/*!
	 * moved folder number
	 */
	int						m_movedFolders;
};

#endif // _CMD_LINE_DB_TASKS_H_