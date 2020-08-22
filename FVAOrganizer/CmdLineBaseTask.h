#ifndef _CMD_LINE_BASE_TASK_H_
#define _CMD_LINE_BASE_TASK_H_

#include <QtCore/QDir>
#include <QtCore/QDebug>

#include "fvacommonlib.h"

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
		virtual ~CmdLineBaseTask( );

		/*!
		 * \brief it performs running of command
		 * \return it returns code of error (not 0 if error happened, 0 - if no error happened) 
		 */
		virtual FVA_EXIT_CODE execute() = 0;

		/*!
		 * \brief it performs procesing folder in recursive mode
		 * \param folder to work with
		 * \return it returns error code (not 0 if error happened, 0 - if no error happened) 
		 */		
		FVA_EXIT_CODE processFolderRecursivly( const QString& folder );

		/*!
		 * \brief it answers if task supports read only mode
		 */
		virtual bool supportReadOnly() {return false; }; 

	protected:

		/*!
		 * \brief ctors-dtors section
		 * \param folder - work folder
		 * \param readOnly - read only mode
		 * \param custom - customization flow support
		 */
		CmdLineBaseTask( const QString& folder, bool readOnly = false, const QString& custom = "" );
	
	protected: // data

		/*!
		 * base folder it started from
		 */
		QString		m_baseFolder;

		/*!
		 * it keeps folder name to work with
		 */
		QString		m_folder;

		/*!
		 * it keeps custom parameter
		 */
		QString		m_custom;

		/*!
		 * it keeps folder object to work with
		 */		
		QDir		m_dir;

		/*!
		 * is it working in read only mode
		 */
		bool		m_readOnly;
};

extern QMap< unsigned int, unsigned int > sizes;

#define LOG_QWARN qWarning()<<"[WRN]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"["<<Name().toUpper()<<"]"
#define LOG_QCRIT qCritical()<<"[ERR]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"["<<Name().toUpper()<<"]"
#define LOG_QDEB qDebug()<<"[DBG]"<<QDateTime::currentDateTime().toString( "[hh:mm:ss]").toLatin1().data()<<"["<<Name().toUpper()<<"]"

// name, support read only mode
#define _CLASS_TASK_DECLARATION(name,supReadOnly) class name : public CmdLineBaseTask \
{\
public:\
	name(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")\
	:CmdLineBaseTask(dir_, readOnly_, custom_){ qDebug() << "[DBG]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[" << Name().toUpper() << "]cmd created,dir:" << dir_; }\
	virtual ~name() { qDebug() << "[DBG]" << QDateTime::currentDateTime().toString("[hh:mm:ss]").toLatin1().data() << "[" << Name().toUpper() << "]cmd deleted, dir:" << m_folder; }\
	virtual FVA_EXIT_CODE execute(); \
	static QString Name(){ return #name; }\
	virtual bool supportReadOnly() { return supReadOnly; }\
}; \

#endif //_CMD_LINE_BASE_TASK_H_
