/*!
* \file CLTMerge1EventDir.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_MERGE_1_EVENT_DIR_H_
#define _CLT_MERGE_1_EVENT_DIR_H_

#include "../FVACommonLib/CmdLineBaseTask.h"

/*!
* \brief CLTMerge1EventDir is child of CmdLineBaseTask and it merges one-event-several-days input folder into output foder
*/
class CLTMerge1EventDir : public CmdLineBaseTask
{
public:
	virtual ~CLTMerge1EventDir() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTMerge1EventDir"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_MERGE_1_EVENT_DIR_H_