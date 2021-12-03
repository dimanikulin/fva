/*!
* \file CLTCheckDateTime.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_CHECK_DATE_TIME_H_
#define _CLT_CHECK_DATE_TIME_H_

#include "../FVACommonLib/CmdLineBaseTask.h"

/*!
* \brief CLTCheckDateTime is child of CmdLineBaseTask and implements the next functions:
*
* 1. it checks for existing EXIF date taken - returns FVA_ERROR_NO_EXIF_DATE_TIME
* 2. TBD
*
*/
class CLTCheckDateTime : public CmdLineBaseTask
{
public:
	CLTCheckDateTime(const FvaConfiguration& cfg);
	virtual ~CLTCheckDateTime() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTCheckDateTime"; }
	virtual bool supportReadOnly() { return false; }

private:
	/*!
	* \brief formatting context to use formatting options from
	*/
	FvaFmtContext					m_fmtctx;
};
#endif // _CLT_CHECK_DATE_TIME_H_