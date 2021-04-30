#include "CLTCreateDirStructByFileNames.h"

FVA_EXIT_CODE CLTCreateDirStructByFileNames::execute(const CLTContext& context)
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir())
			continue;

		// skip meta files
		if ( fvaIsInternalFile( info.fileName() ) )
			continue;

		QString subFolderName		= info.baseName().mid(0,10).replace("-",".");
		QString fullSubFolderpath	= m_folder + "/" + subFolderName;
			
		if (!m_dir.exists(subFolderName))
		{
			if ( !context.readOnly )
				m_dir.mkdir( subFolderName );
			LOG_QDEB << "sub-folder:" << subFolderName << " created";
		}
		if ( !context.readOnly ) 
		{
			// move the file
			if ( !m_dir.rename( info.absoluteFilePath(), fullSubFolderpath + "/" + info.fileName() ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "file renamed:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
		}
	}
	return FVA_NO_ERROR;
}


