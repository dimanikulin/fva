#ifndef _CMD_LINE_TASKS_H_
#define _CMD_LINE_TASKS_H_

#include <QString>

enum FILE_TYPE
{
	FILE_TYPE_UNKNOWN,
	FILE_TYPE_IMG,
	FILE_TYPE_VIDEO
};

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
		CmdLineBaseTask( const QString& argument );
		virtual ~CmdLineBaseTask( );

		/*!
		 * \brief it performs running of command
		 * \return it returns result code (not 0 if error happened, 0 - if no error happened) 
		 */
		virtual int execute() = 0;

		/*!
		 * \brief factory to create the tasks
		 * \param type - human-readable type of command
		 * \param arg - command line argument
		 * \return pointer to created task or null if type is not supported
		 */
		static std::auto_ptr<CmdLineBaseTask> createTaskByType( const QString& type, const QString& arg );

	protected:

		/*!
		 * \brief it converts file extention to file type if it is possible
		 */
		FILE_TYPE convertFileExt2FileType ( const QString& type );

	protected: // data

		/*!
		 * keeps argument
		 */
		QString		argument;

};

class CmdLineTask_FolderStructCreate : public CmdLineBaseTask
{	
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_FolderStructCreate( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};

class CmdLineTask_FilesRename : public CmdLineBaseTask
{
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_FilesRename( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};

class CmdLineTask_FilesRenameRecursive : public CmdLineBaseTask
{
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_FilesRenameRecursive( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};

class CmdLineTask_CheckDeviceNameRecursive : public CmdLineBaseTask
{
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_CheckDeviceNameRecursive( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};

class CmdLineTask_RenameFileByFolderNameRecursive : public CmdLineBaseTask
{
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_RenameFileByFolderNameRecursive( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};


class CmdLineTask_RenameVideoBySequenceRecursive : public CmdLineBaseTask
{
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_RenameVideoBySequenceRecursive( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};


#endif // _CMD_LINE_TASKS_H_
