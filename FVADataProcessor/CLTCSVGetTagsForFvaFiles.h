/*!
* \file CLTCSVGetTagsFvaFiles.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2022
*/
#ifndef _CMD_CSV_GET_TAGS_FOR_FVA_FILES_H_
#define _CMD_CSV_GET_TAGS_FOR_FVA_FILES_H_

#include "CmdLineBaseTask.h"

#include "fvafile.h"

/*!
 * \brief CLTCSVGetTagsForFvaFiles is child of CmdLineBaseTask and 
 * it creates CSV file that keeps a map <Path to media file> to <Tags for this media file> 
 */
class CLTCSVGetTagsForFvaFiles : public CmdLineBaseTask
{
public:
	/// CmdLineBaseTask interface
	CLTCSVGetTagsForFvaFiles(const FvaConfiguration& cfg);
	virtual ~CLTCSVGetTagsForFvaFiles();
	virtual FVA_EXIT_CODE execute(const CLTContext& context);
	/*!
	* \brief returns command name
	* \return command name as a string
	*/
	static QString Name()			{ return "CLTCSVGetTagsForFvaFiles"; }
	virtual bool supportReadOnly()		{ return false; }

private:

	/*!
	 * \brief if fills up the tags for file with name provided
	 * \param fileName a name of file to get tags for
	 * \return it returns the code result 
	 */
	FVA_EXIT_CODE getFvaTagsForFile(const QString fileName, QString& tags);

private: // data

	/*!
	 * path to root fva sw folder
	 */
	QString					m_rootSWdir;

	/*!
	 * cfg flag to search by place
	 */
	bool 					m_SearchByPlace;

	/*!
	 * cfg flag to search by author
	 */
	bool 					m_SearchByAuthor;

	/*!
	 * cfg flag to search by event
	 */
	bool 					m_SearchByEvent;

	/*!
	 * cfg flag to search by eventReasonPeople
	 */
	bool					m_SearchByEventReasonPeople;

	/*!
	 * global fva file information
	 */
	FVA_FILE_INFO_MAP			m_fvaFileInfo;

	/*!
	 * list if records we write in the output file
	 */
	QList<QString>				m_records;

	/*!
	 * map of tag types 
	 */
	FVA_SIMPLE_MAP 				m_fvaTagsTypeMap;

	/*!
	 * map of place types 
	 */
	FVA_SIMPLE_MAP 				m_fvaPlacesTypesMap;

	/*!
	 * map of places 
	 */
	FVA_SIMPLE_MAP 				m_fvaPlacesMap;

};

#endif // _CMD_CSV_GET_TAGS_FOR_FVA_FILES_H_