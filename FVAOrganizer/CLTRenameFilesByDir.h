#ifndef _CLT_RENAME_FILES_BY_DIR_H_
#define _CLT_RENAME_FILES_BY_DIR_H_

#include "CmdLineBaseTask.h"

class CLTRenameFilesByDir : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTRenameFilesByDir(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTRenameFilesByDir()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{ return "CLTRenameFilesByDir"; }
	virtual bool supportReadOnly()	{ return false; }
};

#endif // _CLT_RENAME_FILES_BY_DIR_H_