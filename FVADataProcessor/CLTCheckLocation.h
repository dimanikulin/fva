#ifndef _CLT_CHECK_LOCATION_H_
#define _CLT_CHECK_LOCATION_H_

#include "CmdLineBaseTask.h"

class CLTCheckLocation : public CmdLineBaseTask
{
public:

	virtual ~CLTCheckLocation() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTCheckLocation"; }
	virtual bool supportReadOnly() { return false; }

};
#endif // _CLT_CHECK_LOCATION_H_