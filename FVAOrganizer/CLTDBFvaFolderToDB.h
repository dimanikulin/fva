#ifndef _CLT_DB_FVA_FOLDER_TO_DB_H_
#define _CLT_DB_FVA_FOLDER_TO_DB_H_

#include "CLTDBBase.h"

class CLTDBFvaFolderToDB : public CLTDBBase
{
	public:
		CLTDBFvaFolderToDB(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
			:CLTDBBase(dir_, readOnly_, custom_)
		{
			LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
		}
		virtual ~CLTDBFvaFolderToDB();
		virtual FVA_EXIT_CODE execute();
		static QString Name(){ return "CLTDBFvaFolderToDB"; }

	private: // data

		/*!
		* moved folder descriptions
		*/
		QList<QString>		m_movedFolders;
};
#endif // _CLT_DB_FVA_FOLDER_TO_DB_H_