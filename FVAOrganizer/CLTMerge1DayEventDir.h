#ifndef _CLT_MERGE_1_DAY_EVENT_DIR_H_
#define _CLT_MERGE_1_DAY_EVENT_DIR_H_

#include "CmdLineBaseTask.h"

class CLTMerge1DayEventDir : public CmdLineBaseTask
{
public:
	CLTMerge1DayEventDir(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTMerge1DayEventDir() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTMerge1DayEventDir"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_MERGE_1_DAY_EVENT_DIR_H_