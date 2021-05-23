/*!
* \file CLTMoveAloneFiles.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_MOVE_ALONE_FILES_H_
#define _CLT_MOVE_ALONE_FILES_H_

#include "CmdLineBaseTask.h"

/*!
* \brief it moves alone (or twins) files to parent folder
*/
class CLTMoveAloneFiles : public CmdLineBaseTask
{
public:
	CLTMoveAloneFiles(const FvaConfiguration& cfg);
	virtual ~CLTMoveAloneFiles() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTMoveAloneFiles"; }
	virtual bool supportReadOnly() { return true; }

private:
	/*!
	* min count of multimedia files in one folder allowed
	*/
	uint m_minCountSupportedFiles;
};
#endif // _CLT_MOVE_ALONE_FILES_H_