#ifndef _CLT_RENAME_FILES_BY_DIR_H_
#define _CLT_RENAME_FILES_BY_DIR_H_

#include "CmdLineBaseTask.h"

class CLTRenameFilesByDir : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	virtual ~CLTRenameFilesByDir()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg);
	static QString Name()			{ return "CLTRenameFilesByDir"; }
	virtual bool supportReadOnly()	{ return false; }
};

#endif // _CLT_RENAME_FILES_BY_DIR_H_