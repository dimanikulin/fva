#ifndef _CMD_LINE_BASE_TASK_H_
#define _CMD_LINE_BASE_TASK_H_

#include <QString>
#include <QDir>

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
		virtual FVA_ERROR_CODE execute() = 0;

		/*!
		 * \brief it performs procesing folder in recursive mode
		 * \param folder to work with
		 * \return it returns error code (not 0 if error happened, 0 - if no error happened) 
		 */		
		FVA_ERROR_CODE processFolderRecursivly( const QString& folder );

		/*!
		 * \brief it answers if task supports read only mode
		 */
		virtual bool supportReadOnly () {return false; }; 

	protected:

		/*!
		 * \brief ctors-dtors section
		 * \param folder - work folder
		 * \param readOnly - read only mode
		 * \param custom - customization flow support
		 */
		CmdLineBaseTask( const QString& folder, bool readOnly = false, const QString& custom = "" );

		/*!
		 * \brief it converts file extention to file type if it is possible
		 */
		FVA_FILE_TYPE convertFileExt2FileType ( const QString& type );

		/*!
		 * \brief it answers if file is for internal purpose
		 */
		bool isInternalFileName( const QString& fileName );

	protected: // data

		/*!
		 * it keeps folder name to work with
		 */
		QString		m_folder;

		/*!
		 * it keeps custom parameter
		 */
		QString		m_custom;

		/*!
		 * it keeps folder name to work with
		 */		
		QDir		m_dir;

		/*!
		 * is it working in read only mode
		 */
		bool		m_readOnly;
};
#endif //_CMD_LINE_BASE_TASK_H_
