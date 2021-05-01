#ifndef _CMD_LINE_EXECUTOR_H_
#define _CMD_LINE_EXECUTOR_H_

#include <QtCore/qlist> 
#include <QtCore/qstring> 
#include <QtCore/qstringlist> 

#include "CmdLineBaseTask.h"
#include "fvaconfiguration.h"
#include "fvacltcontext.h"

/*!
 * \brief performs command line processing
 */
class CmdLineExecutor
{
	public:
		/*!
		 * \brief ctors-dtors section
		 */
		CmdLineExecutor() {};
		virtual ~CmdLineExecutor() {};

		/*!
		 * \brief performs running of command
		 * \param context - one command parameters (environment)
		 * \param cfg - system configuration, applicable for whole system  
		 * \return it returns result code (not 0 if error happened, 0 - if no error happened) 
		 */
		FVA_EXIT_CODE run(const CLTContext& context, const FvaConfiguration& cfg);

	private: // method

		/*!
		 * \brief factory to create the tasks
		 * \param context - one command parameters (environment)
		 * \param cfg - system configuration, applicable for whole system
		 * \return pointer to created task or null if type is not supported
		 */
		std::auto_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg);

};
#endif // _CMD_LINE_EXECUTOR_H_