#ifndef _CLT_JSON_CONVERT_DIR_DESC_H_
#define _CLT_JSON_CONVERT_DIR_DESC_H_

#include "CmdLineBaseTask.h"

class CLTJSONConvertDirDesc : public CmdLineBaseTask
{
public:
	CLTJSONConvertDirDesc(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTJSONConvertDirDesc() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTJSONConvertDirDesc"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_JSON_CONVERT_DIR_DESC_H_