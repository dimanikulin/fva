/*!
* \file CmdLineBaseTask.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _CMD_LINE_BASE_TASK_H_
#define _CMD_LINE_BASE_TASK_H_

#include <QtCore/QDir>
#include <QtCore/QDebug>

#include "fvacommonlib.h"
#include "fvaconfiguration.h"
#include "fvacltcontext.h"
#include "fvalogger.inl"

/*!
 * \brief keeps base functionality for one command line task (CLT)
 *  Each CLT command is separated in class and files.
 *  Each CLT command logs itself to see later what changes at FS level were applied.
 *  Each CLT command shall be atomic � TBD to explain
 *  Each CLT command shall support read only mode. TBD to explain
 *  Each CLT command shall be repeatable. TBD to explain
 */
class CmdLineBaseTask
{
	public:
		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */                                      
		virtual ~CmdLineBaseTask(){};

		/*!
		 * \brief it performs running of command
		 * \param context - one command parameters (environment)
		 * \return it returns code of error (not FVA_NO_ERROR if error happened, FVA_NO_ERROR - if no error happened) 
		 */
		virtual FVA_EXIT_CODE execute(const CLTContext& context) = 0;

		/*!
		 * \brief it performs procesing folder in recursive mode
		 * \param folder to work with
		 * \param context - one command parameters (environment)
		 * \return it returns error code (not 0 if error happened, 0 - if no error happened) 
		 */		
		FVA_EXIT_CODE processFolderRecursivly(const std::string& folder, const CLTContext& context);

		/*!
		 * \brief it answers if task supports read only mode
		 */
		virtual bool supportReadOnly() {return false; }; 

	protected:
		/*!
		 * \brief ctors-dtors section
		 */
		CmdLineBaseTask(){};
	
	protected: // data
		/*!
		 * it keeps folder name to work with
		 */
		QString		m_folder;

		/*!
		 * it keeps folder object to work with
		 */		
		QDir		m_dir;
};

extern QMap< unsigned int, unsigned int > sizes;

#endif //_CMD_LINE_BASE_TASK_H_
