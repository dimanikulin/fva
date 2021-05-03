#ifndef _CLT_RENAME_VIDEO_BY_SEQUENCE_H_
#define _CLT_RENAME_VIDEO_BY_SEQUENCE_H_

#include "CmdLineBaseTask.h"

class CLTRenameVideoBySequence : public CmdLineBaseTask
{
public:
	CLTRenameVideoBySequence(const FvaConfiguration& cfg);
	virtual ~CLTRenameVideoBySequence()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name()			{ return "CLTRenameVideoBySequence"; }
	virtual bool supportReadOnly()	{ return true; }
private:

	/*!
	* \brief shall be renamed video files using the file modification time if they do not have taken time set
	*/
	bool			m_renameVideoByModifTime;

	/*!
	* \brief formatting context to use formatting options from
	*/
	FvaFmtContext	m_fmtctx;
};

#endif // _CLT_RENAME_VIDEO_BY_SEQUENCE_H_