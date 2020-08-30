#ifndef _CMD_LINE_DB_TASKS_H_
#define _CMD_LINE_DB_TASKS_H_

#include "CmdLineBaseTask.h"

#include "CLTDBBase.h"

class CLT_Fs_To_SQL : public CLTDBBase
{
public:
	CLT_Fs_To_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
		: CLTDBBase(dir_, readOnly_, custom_), m_skippedFiles(0)
	{
		qWarning()	<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()
						<<"["<< Name().toUpper()
						<<"]cmd created,dir:" <<dir_
						<<",RO=" << (readOnly_?"yes":"no")
						<<",SRO=" << (supportReadOnly()?"yes":"no"); 
	}
	virtual ~CLT_Fs_To_SQL();
	virtual FVA_EXIT_CODE execute();
	static QString Name(){return "CLT_Fs_To_SQL";}
	
private: // data

	/*!
	 * skipped files number
	 */
	int						m_skippedFiles;
};

class CLT_Fva_Folder_To_SQL : public CLTDBBase
{
public:
	CLT_Fva_Folder_To_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
		:CLTDBBase(dir_, readOnly_, custom_)
	{
		qWarning()	<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()
					<<"["<< Name().toUpper()
					<<"]cmd created,dir:" <<dir_
					<<",RO=" << (readOnly_?"yes":"no")
					<<",SRO=" << (supportReadOnly()?"yes":"no"); 
	}
	virtual ~CLT_Fva_Folder_To_SQL();
	virtual FVA_EXIT_CODE execute();
	static QString Name(){return "CLT_Fva_Folder_To_SQL";}

private: // data

	/*!
	 * moved folder descriptions
	 */
	QVector<QString>		m_movedFolders;
};

#endif // _CMD_LINE_DB_TASKS_H_