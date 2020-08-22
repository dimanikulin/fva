#ifndef _CMD_LINE_EXECUTOR_H_
#define _CMD_LINE_EXECUTOR_H_

#include <QtCore/qlist> 
#include <QtCore/qstring> 
#include <QtCore/qstringlist> 

#include "CmdLineBaseTask.h"

/*!
 * \brief performs command line processing
 */
class CmdLineExecutor
{
	public:
		/*!
		 * \brief ctors-dtors section
		 */
		CmdLineExecutor( const QString& cmdName, const QString& folder, bool recursive = false, bool readOnly = false, const QString& custom = "" );
		virtual ~CmdLineExecutor( );

		/*!
		 * \brief performs running of command
		 * \return it returns result code (not 0 if error happened, 0 - if no error happened) 
		 */
		FVA_EXIT_CODE run();

	private: // method

		/*!
		 * \brief factory to create the tasks
		 * \param cmdName - human-readable type of command
		 * \param folder - folder to work in
		 * \param custom - parameter to customize command internally
		 * \return pointer to created task or null if type is not supported
		 */
		std::auto_ptr<CmdLineBaseTask> createTaskByName( const QString& cmdName, const QString& folder, bool readOnly, const QString& custom = "" );

	private: // data

		QString	m_cmdName; 
		QString	m_folder; 
		QString m_custom;
		bool	m_recursive;
		bool	m_readOnly;

};
#endif // _CMD_LINE_EXECUTOR_H_