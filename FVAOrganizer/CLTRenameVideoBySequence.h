#ifndef _CLT_RENAME_VIDEO_BY_SEQUENCE_H_
#define _CLT_RENAME_VIDEO_BY_SEQUENCE_H_

#include "CmdLineBaseTask.h"

class CLTRenameVideoBySequence : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTRenameVideoBySequence(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
		:CmdLineBaseTask(dir_, readOnly_, custom_)
	{
		LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
	}
	virtual ~CLTRenameVideoBySequence()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute();
	static QString Name()			{ return "CLTRenameVideoBySequence"; }
	virtual bool supportReadOnly()	{ return true; }
};

#endif // _CLT_RENAME_VIDEO_BY_SEQUENCE_H_