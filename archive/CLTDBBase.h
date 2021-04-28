#ifndef _CLT_DB_BASE_H_
#define _CLT_DB_BASE_H_

#include "CmdLineBaseTask.h"

class CLTDBBase : public CmdLineBaseTask
{
public:
	CLTDBBase(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{	}
	virtual ~CLTDBBase()
	{	}
	virtual bool supportReadOnly() { return true; }
	static QString Name(){ return "CLTDBBase"; }

protected:

	/*!
	* SQL inserts
	*/
	QList<QString>		m_SQLs;

	/*!
	* common work with output
	*/
	void SaveSQL(const QString& fileToSaveIn);
};
#endif // _CLT_DB_BASE_H_