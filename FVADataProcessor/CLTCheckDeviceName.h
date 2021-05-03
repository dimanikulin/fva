#ifndef _CLT_CHECK_DEVICE_NAME_H_
#define _CLT_CHECK_DEVICE_NAME_H_

#include "CmdLineBaseTask.h"

class CLTCheckDeviceName : public CmdLineBaseTask
{
public:
	virtual ~CLTCheckDeviceName() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTCheckDeviceName"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_CHECK_DEVICE_NAME_H_