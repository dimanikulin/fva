#ifndef _CLT_CONVERT_AMR_H_
#define _CLT_CONVERT_AMR_H_

#include "CmdLineBaseTask.h"

_CLASS_TASK_DECLARATION(CLT_Convert_Amr, false)

class CLTConvertAmr : public CmdLineBaseTask
{
public:
	CLTConvertAmr(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTConvertAmr() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTConvertAmr"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_CONVERT_AMR_H_