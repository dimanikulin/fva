#ifndef _CLT_MERGE_1_EVENT_DIR_H_
#define _CLT_MERGE_1_EVENT_DIR_H_

#include "CmdLineBaseTask.h"

class CLTMerge1EventDir : public CmdLineBaseTask
{
public:
	CLTMerge1EventDir(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTMerge1EventDir() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTMerge1EventDir"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_MERGE_1_EVENT_DIR_H_