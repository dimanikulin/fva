#ifndef _CLT_MERGE_FEW_DAYS_FEW_EVENTS_DIR_H_
#define _CLT_MERGE_FEW_DAYS_FEW_EVENTS_DIR_H_

#include "CmdLineBaseTask.h"

class CLTMergeFewDaysFewsEventsDir : public CmdLineBaseTask
{
public:
	CLTMergeFewDaysFewsEventsDir(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTMergeFewDaysFewsEventsDir() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTMergeFewDaysFewsEventsDir"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_MERGE_FEW_DAYS_FEW_EVENTS_DIR_H_
