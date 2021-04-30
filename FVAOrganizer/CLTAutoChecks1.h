#ifndef _CLT_AUTO_CHECKS_1_H_
#define _CLT_AUTO_CHECKS_1_H

#include "CmdLineBaseTask.h"

class CLTAutoChecks1 : public CmdLineBaseTask
{
public:
	virtual ~CLTAutoChecks1() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){return "CLTAutoChecks1";}
	virtual bool supportReadOnly() {return true;}
};
#endif // _CLT_AUTO_CHECKS_2_H