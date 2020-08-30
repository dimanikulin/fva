#ifndef _CLT_DB_FVA_FILE_TO_DB_H_
#define _CLT_DB_FVA_FILE_TO_DB_H_

#include "CLTDBBase.h"

class CLTDBFvaFileToDB : public CLTDBBase
{
	public:
		CLTDBFvaFileToDB(const QString& dir_, bool readOnly_ = false, const QString& custom_ = "")
			:CLTDBBase(dir_, readOnly_, custom_)
		{
			LOG_QDEB << "cmd created,dir:" << dir_ << ",RO=" << (readOnly_ ? "yes" : "no") << ",SRO=" << (supportReadOnly() ? "yes" : "no");
		}
		virtual ~CLTDBFvaFileToDB();
		virtual FVA_EXIT_CODE execute();
		static QString Name(){return "CLTDBFvaFileToDB";}
	
	private: // data

	/*!
	 * moved file descriptions
	 */
	QVector<QString>		m_movedFiles;

};
#endif // _CLT_DB_FVA_FILE_TO_DB_H_