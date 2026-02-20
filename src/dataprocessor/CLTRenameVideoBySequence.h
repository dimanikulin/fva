/*!
* \file CLTRenameVideoBySequence.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_RENAME_VIDEO_BY_SEQUENCE_H_
#define _CLT_RENAME_VIDEO_BY_SEQUENCE_H_

#include "CmdLineBaseTask.h"

/*!
* \brief CLTRenameVideoBySequence is child of CmdLineBaseTask and it renames video files by their sequence formatted in file name 
* In read only mode this cmd just checks if renaming is even possible
*/
class CLTRenameVideoBySequence : public CmdLineBaseTask
{
public:
	CLTRenameVideoBySequence(const FvaConfiguration& cfg);
	virtual ~CLTRenameVideoBySequence()	{ LOG_DEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name()			{ return "CLTRenameVideoBySequence"; }
	virtual bool supportReadOnly()	{ return true; }

	/*!
	* \brief it answers if task shall rename video files using the file modification time
	* for testing purposes
	* \return returns true if they do not have taken time set
	*/
	bool renameVideoByModifTime() const	{ return m_renameVideoByModifTime; }
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
