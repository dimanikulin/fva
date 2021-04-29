#ifndef _CMD_LINE_BASE_TASK_H_
#define _CMD_LINE_BASE_TASK_H_

#include <QtCore/QDir>
#include <QtCore/QDebug>

#include "fvacommonlib.h"
#include "fvaconfiguration.h"
#include "CLTContext.h"

/*!
 * \brief keeps base functionality for one command line task
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
		 * \param cfg - system configuration, applicable for whole system
		 * \return it returns code of error (not 0 if error happened, 0 - if no error happened) 
		 */
		virtual FVA_EXIT_CODE execute(const CLTContext& context, const FvaConfiguration& cfg) = 0;

		/*!
		 * \brief it performs procesing folder in recursive mode
		 * \param folder to work with
		 * \param context - one command parameters (environment)
		 * \param cfg - system configuration, applicable for whole system
		 * \return it returns error code (not 0 if error happened, 0 - if no error happened) 
		 */		
		FVA_EXIT_CODE processFolderRecursivly(const QString& folder, const CLTContext& context, const FvaConfiguration& cfg);

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

#define LOG_QWARN qWarning()<<"[WRN]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"["<<Name().toUpper()<<"]"
#define LOG_QCRIT qCritical()<<"[ERR]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"["<<Name().toUpper()<<"]"
#define LOG_QDEB qDebug()<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"["<<Name().toUpper()<<"]"

#endif //_CMD_LINE_BASE_TASK_H_
