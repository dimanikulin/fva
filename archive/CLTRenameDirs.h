#ifndef _CLT_RENAME_DIRS_H_
#define _CLT_RENAME_DIRS_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it renames folders with replacing "-" to "."
 */
class CLTRenameDirs : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	virtual ~CLTRenameDirs()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name()			{ return "CLTRenameDirs"; }
	virtual bool supportReadOnly()	{ return false; }
};

#endif // _CLT_RENAME_DIRS_H_