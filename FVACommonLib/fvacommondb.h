#ifndef FVACOMMONDB_H
#define FVACOMMONDB_H

#include "fvaerrors.h"
#include "fvaconstants.h"

#include <QtCore/QString>
#include <QtCore/QVariantMap>

/*!
 * \brief it loads FVA dictionaries from file 
 * \param file to load dictionaries from 
 * \param output output parameter to be filled up by json structures
 * \param error - human-readable description of error if any 
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaLoadDictionary( const QString& file, QVariantMap& output, QString& error );

/*!
 * \brief it saves FVA dictionaries to file 
 * \param file to save dictionaries to 
 * \param outputJson input parameter to be saved
 * \param error - human-readable description of error if any 
 * \returns it returns code of error if any or FVA_NO_ERROR if saving was successful
 */
FVA_ERROR_CODE fvaSaveDictionary( const QString& file, QVariantMap& inputJson, QString& error );

/*!
 * \brief it tries to load device id map from dictionary
 * \param deviceIds a map to be filled up
 * \param dictPath path to dictionary to load map from
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaLoadDeviceMapFromDictionary(DEVICE_MAP& deviceMap, const QString& dictPath); 

class fvaItem;
/*!
 * \brief it loads fva items from DB as a tree 
 * \param rootPath root folder of FVA structure
 * \param rootItem root fva item to start loading from
 * \param DBPath path to fva DB
 * \param error error description to be filled up
 * \param number count of items to be proceeded
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_ERROR_CODE fvaLoadItems(const QString& rootPath, fvaItem* rootItem, const QString& DBPath, QString& error, int& number);


#define FILL_COMB_FROM_DICT(dict,combo) \
	vlist = m_dictionaries[dict].toList();\
	combo->clear(); \
	for ( auto i = vlist.begin(); i != vlist.end() ; ++i )\
		combo->addItem ( i->toMap()["name"].toString(), i->toMap()["ID"].toString() );

#endif // FVACOMMONDB_H