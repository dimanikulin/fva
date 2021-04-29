#ifndef _CLT_MERGE_1_DAY_EVENT_DIR_H_
#define _CLT_MERGE_1_DAY_EVENT_DIR_H_

#include "CmdLineBaseTask.h"

class CLTMerge1DayEventDir : public CmdLineBaseTask
{
public:
	virtual ~CLTMerge1DayEventDir() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg);
	static QString Name(){ return "CLTMerge1DayEventDir"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_MERGE_1_DAY_EVENT_DIR_H_