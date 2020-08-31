#ifndef _CLT_SET_FILE_ATTRS_H_
#define _CLT_SET_FILE_ATTRS_H_

#include "CmdLineBaseTask.h"

_CLASS_TASK_DECLARATION(CLT_Set_File_Atts, false)

class CLTSetFileAtts : public CmdLineBaseTask
{
public:
	CLTSetFileAtts(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTSetFileAtts() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTSetFileAtts"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_SET_FILE_ATTRS_H_