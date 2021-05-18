/*!
* \file CLTCheckLocation.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_CHECK_LOCATION_H_
#define _CLT_CHECK_LOCATION_H_

#include "CmdLineBaseTask.h"

class CLTCheckLocation : public CmdLineBaseTask
{
public:

	virtual ~CLTCheckLocation() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	static QString Name(){ return "CLTCheckLocation"; }
	virtual bool supportReadOnly() { return false; }

};
#endif // _CLT_CHECK_LOCATION_H_