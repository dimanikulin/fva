/*!
* \file fvaconfiguration.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef _FVA_CONFIGURATION_
#define _FVA_CONFIGURATION_

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "fvaexitcodes.h"
#include "FVADescriptionFile.h"

/*!
 * \brief it keeps functionality to work confuguration like 
 * - loading/saving whole configuration from/to disk
 * - getting/setting one parameter from/to loaded configuration
*/
class FvaConfiguration
{
	public: // methods
		/*!
		* \brief it loads configuration from file
		* \param path - path to CSV file
		* \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
		*/
		FVA_EXIT_CODE load(const QString& path);

		/*!
		* \brief it saves data to file
		* \param path - path to CSV file
		* \returns it returns code of error if any or FVA_NO_ERROR if saving was successful
		*/
		FVA_EXIT_CODE save(const QString& path) ;

		/*!
		* \brief it returns the parameter value as string representation
		* \param paramName - parameter name
		* \param paramVaue - parameter value to be returned
		* \returns it returns code of error if any or FVA_NO_ERROR if getting was successful
		*/
		FVA_EXIT_CODE getParamAsString(const QString& paramName, QString& paramValue) const;

		/*!
		* \brief it returns the parameter value as bool
		* \param paramName - parameter name
		* \param paramVaue - parameter value to be returned
		* \returns it returns code of error if any or FVA_NO_ERROR if getting was successful
		*/
		FVA_EXIT_CODE getParamAsBoolean(const QString& paramName, bool& paramValue) const;

		/*!
		* \brief it returns the parameter value as unsigned integer
		* \param paramName - parameter name
		* \param paramVaue - parameter value to be returned
		* \returns it returns code of error if any or FVA_NO_ERROR if getting was successful
		*/
		FVA_EXIT_CODE getParamAsUint(const QString& paramName, uint& paramValue) const;

		/*!
		* \brief it sets the parameter value as unsigned integer for the futher saving
		* \param paramName - parameter name
		* \param paramValue - parameter value
		* \returns it returns code of error if any or FVA_NO_ERROR if setting was successful
		*/
		FVA_EXIT_CODE setParam(const QString& paramName, uint paramValue);

		/*!
		* \brief it sets the parameter value as boolean for the futher saving
		* \param paramName - parameter name
		* \param paramValue - parameter value
		* \returns it returns code of error if any or FVA_NO_ERROR if setting was successful
		*/
		FVA_EXIT_CODE setParam(const QString& paramName, bool paramValue);

		/*!
		* \brief it sets the parameter value as string for the futher saving
		* \param paramName - parameter name
		* \param paramValue - parameter value
		* \returns it returns code of error if any or FVA_NO_ERROR if setting was successful
		*/
		FVA_EXIT_CODE setParam(const QString& paramName, QString paramValue);

	private: // data
		/*!
		* \brief titles from CSV file that keeps whole the configuration
		*/
		QStringList			m_cfgtitles;

		/*!
		* \brief whole the configuration from CSV file that keeps it
		*/
		DESCRIPTIONS_MAP	m_cfgItems;
};
#endif // _FVA_CONFIGURATION_