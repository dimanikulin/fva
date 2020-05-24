#ifndef _CMD_LINE_TASKS_H_
#define _CMD_LINE_TASKS_H_

#include <QString>

enum MM_FILE_TYPE
{
	MM_FILE_TYPE_UNKNOWN,
	MM_FILE_TYPE_IMG,
	MM_FILE_TYPE_VIDEO
};

const QString CMD_FOLDER_STRUCT_CREATE_BY_FILE				= "folderStructCreateByFile";
const QString CMD_RECURSIVE_FOLDER_STRUCT_BY_FILE			= "recursivefolderStructCreateByFile";
const QString CMD_RECURSIVE_FOLDER_STRUCT_BY_DEVICE_NAME	= "recursivefolderStructCreateByDeviceName";
const QString CMD_FILES_RENAME								= "filesRename";
const QString CMD_RECURSIVE_FILES_RENAME					= "recursiveFilesRename";
const QString CMD_RECURSIVE_DEVICE_NAME_CHECK				= "recursiveDeviceNameCheck";
const QString CMD_RECURSIVE_FILES_RENAME_BY_FOLDER			= "recursiveFilesRenameByFolder";
const QString CMD_RECURSIVE_VIDEO_RENAME_BY_SEQUENCE		= "recursiveVideoRenameBySequence";
const QString CMD_FOLDER_NAME_CHANGE						= "folderNameChange";

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
		MM_FILE_TYPE convertFileExt2FileType ( const QString& type );

	protected: // data

		/*!
		 * keeps argument
		 */
		QString		argument;

};

class CmdLineTask_FolderStructCreateByFile : public CmdLineBaseTask
{	
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_FolderStructCreateByFile( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};

class CmdLineTask_FolderStructCreateByFileRecursive : public CmdLineBaseTask
{	
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_FolderStructCreateByFileRecursive( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};

class CmdLineTask_FolderStructCreateByDeviceRecursive : public CmdLineBaseTask
{	
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_FolderStructCreateByDeviceRecursive( const QString& argument );

		/*!
		 * base methods
		 */
		virtual int execute();
};

class CmdLineTask_FolderNameChange : public CmdLineBaseTask
{	
	public :

		/*!
		 * \brief ctors-dtors section
		 * \param argument - command line argument
		 */
		CmdLineTask_FolderNameChange( const QString& argument );

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
