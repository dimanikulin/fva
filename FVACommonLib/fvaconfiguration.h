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
	public:

		/*!
		* \brief ctors-dtors section
		*/
		FvaConfiguration() {};
		virtual ~FvaConfiguration(){};

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
		* \param paramName - parameter value
		* \returns it returns the parameter value as string representation
		*/
		QString getParamAsString(const QString& paramName) const;

		/*!
		* \brief it returns the parameter value as bool
		* \param paramName - parameter value
		* \returns it returns the parameter value as bool
		*/
		bool getParamAsBoolean(const QString& paramName) const;

		/*!
		* \brief it returns the parameter value as unsigned integer
		* \param paramName - parameter value
		* \returns it returns the parameter value as unsigned integer
		*/
		uint getParamAsUint(const QString& paramName) const;

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