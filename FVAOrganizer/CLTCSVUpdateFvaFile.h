#ifndef _CLT_CSV_UPDATE_FVA_FILE_H_
#define _CLT_CSV_UPDATE_FVA_FILE_H_

#include "CmdLineBaseTask.h"

class CLTCSVUpdateFvaFile : public CmdLineBaseTask
{
public:
	CLTCSVUpdateFvaFile(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTCSVUpdateFvaFile() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTCSVUpdateFvaFile"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_CSV_UPDATE_FVA_FILE_H_
