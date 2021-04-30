#ifndef _CLT_RENAME_VIDEO_BY_SEQUENCE_H_
#define _CLT_RENAME_VIDEO_BY_SEQUENCE_H_

#include "CmdLineBaseTask.h"

class CLTRenameVideoBySequence : public CmdLineBaseTask
{
public:
	virtual ~CLTRenameVideoBySequence()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name()			{ return "CLTRenameVideoBySequence"; }
	virtual bool supportReadOnly()	{ return true; }
};

#endif // _CLT_RENAME_VIDEO_BY_SEQUENCE_H_