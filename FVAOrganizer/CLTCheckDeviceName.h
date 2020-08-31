#ifndef _CLT_CHECK_DEVICE_NAME_H_
#define _CLT_CHECK_DEVICE_NAME_H_

#include "CmdLineBaseTask.h"

class CLTCheckDeviceName : public CmdLineBaseTask
{
public:
	CLTCheckDeviceName(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTCheckDeviceName() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTCheckDeviceName"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_CHECK_DEVICE_NAME_H_