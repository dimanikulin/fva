/*!
* \file CLTMerge1DayEventDir.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_MERGE_1_DAY_EVENT_DIR_H_
#define _CLT_MERGE_1_DAY_EVENT_DIR_H_

#include "CmdLineBaseTask.h"

class CLTMerge1DayEventDir : public CmdLineBaseTask
{
public:
	CLTMerge1DayEventDir(const FvaConfiguration& cfg);
	virtual ~CLTMerge1DayEventDir() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTMerge1DayEventDir"; }
	virtual bool supportReadOnly() { return true; }

private: // data

	/*!
	* path to root fva sw folder
	*/
	QString							m_rootSWdir;

};
#endif // _CLT_MERGE_1_DAY_EVENT_DIR_H_