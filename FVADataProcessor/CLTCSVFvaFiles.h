/*!
* \file CLTCSVFvaFiles.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CMD_CSV_FVA_FILES_H_
#define _CMD_CSV_FVA_FILES_H_

#include "CmdLineBaseTask.h"

/*!
 * \brief CLTCSVFvaFile is child of CmdLineBaseTask and 
 * it creates CSV file with FVA information for files in input folder 
 */
class CLTCSVFvaFile : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTCSVFvaFile(const FvaConfiguration& cfg);
	virtual ~CLTCSVFvaFile()		{ LOG_QDEB << "cmd deleted, dir:" << m_folder; }
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name()			{ return "CLTCSVFvaFile"; }
	virtual bool supportReadOnly()	{ return true; }

private: // data

	/*!
	* path to root fva sw folder
	*/
	QString							m_rootSWdir;
};

#endif // _CMD_CSV_FVA_FILES_H_