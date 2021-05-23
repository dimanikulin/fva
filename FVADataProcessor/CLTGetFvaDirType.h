/*!
* \file CLTGetFvaDirType.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_GET_FVA_DIR_TYPE_H_
#define _CLT_GET_FVA_DIR_TYPE_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief it returns the input foder type - FVA_1_EVENT_1_DAY, FVA_1_EVENT_FEW_DAYS, FVA_FEW_EVENTS_1_DAY or FVA_1_EVENT_FEW_DAYS
 */
class CLTGetFvaDirType : public CmdLineBaseTask
{
public:
	virtual ~CLTGetFvaDirType() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTGetFvaDirType"; }
	virtual bool supportReadOnly() { return true; }
};
#endif // _CLT_GET_FVA_DIR_TYPE_H_