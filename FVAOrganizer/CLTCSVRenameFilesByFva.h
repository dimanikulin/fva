#ifndef _CMD_CSV_RENAME_FILE_BY_FVA_H_
#define _CMD_CSV_RENAME_FILE_BY_FVA_H_

#include "CmdLineBaseTask.h"

class CLTCSVRenameFilesByFva : public CmdLineBaseTask
{
public:
	CLTCSVRenameFilesByFva(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTCSVRenameFilesByFva() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name(){ return "CLTCSVRenameFilesByFva"; }
	virtual bool supportReadOnly() { return false; }
};
#endif // _CMD_CSV_RENAME_FILE_BY_FVA_H_
