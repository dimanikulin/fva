#ifndef _CMD_LINE_DB_TASKS_H_
#define _CMD_LINE_DB_TASKS_H_

#include "CmdLineTasks.h"

class CLT_Base_SQL : public CmdLineBaseTask
{
public:
	CLT_Base_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	:CmdLineBaseTask( dir_,readOnly_,custom_)
	{	}
	virtual ~CLT_Base_SQL()
	{	}
	virtual bool supportReadOnly() {return true;}
	static QString Name(){return "CLT_Base_SQL";}

protected:

	/*!
	 * SQL inserts
	 */
	QVector<QString>		m_SQLs;

	/*!
	 * common work with output
	 */
	void SaveSQL( const QString& fileToSaveIn );
};

class CLT_Fs_To_SQL : public CLT_Base_SQL
{
public:
	CLT_Fs_To_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	: CLT_Base_SQL( dir_,readOnly_,custom_), m_skippedFiles(0)
	{
		qWarning()	<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()
						<<"["<< Name().toUpper()
						<<"]cmd created,dir:" <<dir_
						<<",RO=" << (readOnly_?"yes":"no")
						<<",SRO=" << (supportReadOnly()?"yes":"no"); 
	}
	virtual ~CLT_Fs_To_SQL();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Fs_To_SQL";}
	
private: // data

	/*!
	 * skipped files number
	 */
	int						m_skippedFiles;
};

class CLT_Fva_Folder_To_SQL : public CLT_Base_SQL
{
public:
	CLT_Fva_Folder_To_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
	:CLT_Base_SQL( dir_,readOnly_,custom_)
	{
		qWarning()	<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()
					<<"["<< Name().toUpper()
					<<"]cmd created,dir:" <<dir_
					<<",RO=" << (readOnly_?"yes":"no")
					<<",SRO=" << (supportReadOnly()?"yes":"no"); 
	}
	virtual ~CLT_Fva_Folder_To_SQL();
	virtual FVA_ERROR_CODE execute();
	static QString Name(){return "CLT_Fva_Folder_To_SQL";}

private: // data

	/*!
	 * moved folder descriptions
	 */
	QVector<QString>		m_movedFolders;
};

class CLT_Fva_File_To_SQL : public CLT_Base_SQL
{
	public:
		CLT_Fva_File_To_SQL(const QString& dir_,bool readOnly_=false,const QString& custom_="")
		:CLT_Base_SQL( dir_,readOnly_,custom_)
		{
			qWarning()	<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toAscii().data()
						<<"["<< Name().toUpper()
						<<"]cmd created,dir:" <<dir_
						<<",RO=" << (readOnly_?"yes":"no")
						<<",SRO=" << (supportReadOnly()?"yes":"no"); 
		}
		virtual ~CLT_Fva_File_To_SQL();
		virtual FVA_ERROR_CODE execute();
		static QString Name(){return "CLT_Fva_File_To_SQL";}
	
	private: // data

	/*!
	 * moved file descriptions
	 */
	QVector<QString>		m_movedFiles;

};


#endif // _CMD_LINE_DB_TASKS_H_