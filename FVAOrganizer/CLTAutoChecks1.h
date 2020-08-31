#ifndef _CLT_AUTO_CHECKS_1_H_
#define _CLT_AUTO_CHECKS_1_H

#include "CmdLineBaseTask.h"

class CLTAutoChecks1 : public CmdLineBaseTask
{
public:
	CLTAutoChecks1(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTAutoChecks1() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){return "CLTAutoChecks1";}
	virtual bool supportReadOnly() {return true;}
};
#endif // _CLT_AUTO_CHECKS_2_H