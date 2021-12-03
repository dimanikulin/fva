/*!
* \file CLTAutoChecks1.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CLT_AUTO_CHECKS_1_H_
#define _CLT_AUTO_CHECKS_1_H

#include "../FVACommonLib/CmdLineBaseTask.h"
#include "../FVACommonLib/fvafmtcontext.h"

#include <QtCore/QCoreApplication>

/*!
* \brief CLTAutoChecks1 is child of CmdLineBaseTask and implements the next functions:
*
* 1. checks if there is no first video
* 2. checks if there is no panarom files
*
*/
class CLTAutoChecks1 : public CmdLineBaseTask
{
public:
	CLTAutoChecks1(const FvaConfiguration& cfg);
	virtual ~CLTAutoChecks1() { LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){return "CLTAutoChecks1";}
	virtual bool supportReadOnly() {return true;}

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
#endif // _CLT_AUTO_CHECKS_2_H