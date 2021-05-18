/*!
* \file CLTCheckDateTime.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_CHECK_DATE_TIME_H_
#define _CLT_CHECK_DATE_TIME_H_

#include "CmdLineBaseTask.h"

class CLTCheckDateTime : public CmdLineBaseTask
{
public:
	CLTCheckDateTime(const FvaConfiguration& cfg);
	virtual ~CLTCheckDateTime() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTCheckDateTime"; }
	virtual bool supportReadOnly() { return false; }

private:
	/*!
	* \brief formatting context to use formatting options from
	*/
	FvaFmtContext					m_fmtctx;
};
#endif // _CLT_CHECK_DATE_TIME_H_