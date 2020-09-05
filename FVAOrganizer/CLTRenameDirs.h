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
	CLTRenameDirs(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTRenameDirs()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{ return "CLTRenameDirs"; }
	virtual bool supportReadOnly()	{ return false; }
};

#endif // _CLT_RENAME_DIRS_H_