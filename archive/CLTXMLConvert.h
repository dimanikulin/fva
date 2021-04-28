#ifndef _CLT_XML_CONVERT__H_
#define _CLT_XML_CONVERT__H

#include "CmdLineBaseTask.h"

class CLTXmlConvert : public CmdLineBaseTask
{
public:
	CLTXmlConvert(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTXmlConvert() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTXmlConvert"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CLT_XML_CONVERT__H