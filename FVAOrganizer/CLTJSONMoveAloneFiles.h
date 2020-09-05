#ifndef _CLT_JSON_MOVE_ALONE_FILES_H_
#define _CLT_JSON_MOVE_ALONE_FILES_H_

#include "CmdLineBaseTask.h"

class CLTJSONMoveAloneFiles : public CmdLineBaseTask
{
public:
	CLTJSONMoveAloneFiles(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTJSONMoveAloneFiles() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTJSONMoveAloneFiles"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_JSON_MOVE_ALONE_FILES_H_