/*!
* \file FVABaseDictionaryItem.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2022
*/
#ifndef FVA_BASE_DICTIONARY_ITEM_H
#define FVA_BASE_DICTIONARY_ITEM_H

#include <QtCore/QString>

/*!
 * \brief it keeps base information about a dictionary element
 */
class fvaBaseDictionaryItem
{
	public : 

		fvaBaseDictionaryItem ();
		virtual ~fvaBaseDictionaryItem ();

	public : // data

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
		QString name;
};

#endif // FVA_BASE_DICTIONARY_ITEM_H
