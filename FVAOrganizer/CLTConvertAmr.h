#ifndef _CLT_CONVERT_AMR_H_
#define _CLT_CONVERT_AMR_H_

#include "CmdLineBaseTask.h"

class CLTConvertAmr : public CmdLineBaseTask
{
public:
	virtual ~CLTConvertAmr() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg);
	static QString Name(){ return "CLTConvertAmr"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_CONVERT_AMR_H_