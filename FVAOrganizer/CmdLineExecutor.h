#ifndef _CMD_LINE_EXECUTOR_H_
#define _CMD_LINE_EXECUTOR_H_

#include "CmdLineBaseExecutor.h"

/*!
 * \brief performs command line processing
 */
class CmdLineExecutor : public CmdLineBaseExecutor
{
	private: // method

		std::auto_ptr<CmdLineBaseTask> createTaskByName(const CLTContext& context, const FvaConfiguration& cfg);

};
#endif // _CMD_LINE_EXECUTOR_H_