#ifndef _CMD_CSV_UPDATE_FVA_FILES_H_
#define _CMD_CSV_UPDATE_FVA_FILES_H_

#include "CmdLineBaseTask.h"


class CLTCSVUpdateFvaFiles : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTCSVUpdateFvaFiles(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTCSVUpdateFvaFiles()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{ return "CLTCSVUpdateFvaFiles"; }
	virtual bool supportReadOnly()	{ return true; }
};

#endif // _CMD_CSV_UPDATE_FVA_FILES_H_