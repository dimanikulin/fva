/*!
* \file CLTAutoChecks3.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef _CLT_AUTO_CHECKS_3_H_
#define _CLT_AUTO_CHECKS_3_H

#include "CmdLineBaseTask.h"
#include "fvafmtcontext.h"

#include "data/fvafile.h"
#include "data/fvadevice.h"

/*!
* \brief CLTAutoChecks3 is child of CmdLineBaseTask and implements the next functions:
*
* 1. it checks for existing device in fva info by fileName-returns FVA_ERROR_NO_DEV_ID and FVA_ERROR_NON_UNIQUE_FVA_INFO
* 2. it checks for existing device in dictionary by device name in pictire-returns FVA_ERROR_UKNOWN_DEVICE, FVA_ERROR_EMPTY_DEVICE, FVA_ERROR_LINKED_WRONG_DEVICE
* 3. it checks for GEO position existing in file-returns FVA_ERROR_NO_GEO
* 4. it checks for existing fva information in CSV file-returns FVA_ERROR_NOT_EXISTING_FVA
* 5. finally it outputs all found issues into rootSWdir + "#logs/issues3.csv".
*
*/
class CLTAutoChecks3 : public CmdLineBaseTask
{
public:
	CLTAutoChecks3(const FvaConfiguration& cfg);
	virtual ~CLTAutoChecks3();
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){ return "CLTAutoChecks3"; }
	virtual bool supportReadOnly() { return true; }

private:

	/*!
	* issues found, divided by type (in string format)
	*/
	QList<QString>					m_Issues;

	/*!
	* global device map
	*/
	DEVICE_MAP						m_deviceMap;

	/*!
	* global fva file information
	*/
	FVA_FILE_INFO_MAP				m_fvaFileInfo;

	/*!
	* copy of global fva file information
	*/
	FVA_FILE_INFO_MAP				m_fvaFileInfoC;

	/*!
	* path to root fva sw folder
	*/
	QString							m_rootSWdir;

	/*!
	* \brief formatting context to use formatting options from
	*/
	FvaFmtContext					m_fmtctx;
};
#endif // _CLT_AUTO_CHECKS_3_H