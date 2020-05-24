#ifndef _CMD_LINE_EXECUTOR_H_
#define _CMD_LINE_EXECUTOR_H_

#include <qlist> 
#include <qstring> 
#include <qstringlist> 

/*!
 * \brief performs command line processing
 */
class CmdLineExecutor
{
	public:
		/*!
		 * \brief ctors-dtors section
		 */
		CmdLineExecutor( const QStringList& argumets );
		virtual ~CmdLineExecutor( );

		/*!
		 * \brief performs running of command
		 * \return it returns result code (negative if error happened, 0 - if no error happened) 
		 */
		int run();

	private: // data

		/*!
		 * set of cmd line parameters
		 */
		QStringList	argList;
};
#endif // _CMD_LINE_EXECUTOR_H_