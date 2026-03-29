/*!
 * \file FVABaseDictionaryItem.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2022
 */
#ifndef FVA_BASE_DICTIONARY_ITEM_H
#define FVA_BASE_DICTIONARY_ITEM_H

#include <map>
#include <string>

/*!
 * \brief it keeps base information about a dictionary element
 */
class fvaBaseDictionaryItem {
public:  // data
    /*!
     * item ID
     */
    int ID;

    /*!
     * item type
     */
    int type;

    /*!
     * item name
     */
    std::string name;

    /*!
     * \brief constructor
     */
    fvaBaseDictionaryItem() : ID(0), type(0){};

    /*!
     * \brief constructor with parameters
     * \param ID item ID
     * \param type item type
     * \param name item name
     * \return none
     * \note none
     * \warning none
     */
    fvaBaseDictionaryItem(int ID, int type, std::string name) : ID(ID), type(type), name(name){};
};

typedef std::map<int, fvaBaseDictionaryItem> BASE_DICT_ITEM_MAP;

#endif  // FVA_BASE_DICTIONARY_ITEM_H
