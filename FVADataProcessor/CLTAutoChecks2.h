/*!
* \file CLTAutoChecks2.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef _CLT_AUTO_CHECKS_2_H_
#define _CLT_AUTO_CHECKS_2_H

#include "../FVACommonLib/CmdLineBaseTask.h"
#include "../FVACommonLib/fvafmtcontext.h"

/*!
* \brief CLTAutoChecks2 is child of CmdLineBaseTask and implements the next functions:
*
* 1. it checks for proper folder name - no internal folder is allowed;
* 2. it checks for proper file name;
* 3. it checks for supported file type;
* 4. it checks for matching taken time and file name;
* 5. it checks for matching file and folder names;
* 6. it checks for not uniquie file name;
* 7. it checks for empty taken date-time;
* 8. it checks for too little files in one folder;
* 9. finally it outputs all found issues into rootSWdir + "#logs#/issues2.csv".
*
*/
class CLTAutoChecks2 : public CmdLineBaseTask
{
public:
	CLTAutoChecks2(const FvaConfiguration& cfg);
	virtual ~CLTAutoChecks2();
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return returns command name as a string
	*/
	static QString Name(){return "CLTAutoChecks2";}
	virtual bool supportReadOnly() {return true;}

private: // data

	/*!
	 * count of files in each subfolder
	 */
	QMap<QString, unsigned int>		m_fileCount;

	/*!
	 * issues found, divided by type (in string format)
	 */	
	QList<QString>					m_Issues;

	/*!
	 * file names to file pathes
	 */
	QMap<QString, QString>			m_uniqueFileNames;

	/*!
	 * path to root fva sw folder
	 */
	QString							m_rootSWdir;

	/*!
	 * min count of multimedia files in one folder allowed
	 */
	uint							m_minCountSupportedFiles;

	/*!
	* \brief formatting context to use formatting options from
	*/
	FvaFmtContext					m_fmtctx;
};
#endif // _CLT_AUTO_CHECKS_2_H