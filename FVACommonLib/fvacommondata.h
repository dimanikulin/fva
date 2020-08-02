#ifndef FVACOMMONDATA_H
#define FVACOMMONDATA_H

#include <memory>

#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QVector> 
#include <QtCore/QDateTime>

#include "fvaconstants.h"

/*!
 * \brief it describes whole information about device 
 */
class fvaDevice
{
	public:

		/*
		 * device identifier from device dictionary
		 */
		int deviceId;

		/*
		 * owner name from people dictionary
		 */
		QString ownerName;

		/*
		 * device name to register in dictionary(usually <brand name> + <device model>)
		 */
		QString linkedName;

		/*
		 * device name to show in GUI
		 */
		QString guiName;

};

/*!
 * \brief it keeps fva specific information about a folder
 */
class fvaFolder
{
	public:

		fvaFolder();
		virtual ~fvaFolder();

		/*!
		 * event id 
		 */
		unsigned int			eventId;

		/*!
		 * people that are reason of event
		 */
		QVector<unsigned int>	eventReasonPeopleIds;

		/*!
		 * folder tags
		 */
		QString					tags;

		/*!
		 * folder to show together with this folder
		 */
		QString					linkedFolder;
};

/*!
 * \brief it keeps fva specific information about a file
 */
class fvaFile
{
	public:

		fvaFile();
		virtual ~fvaFile();

		/*!
		 * place id 
		 */
		unsigned int			placeId;

		/*!
		 * id of device 
		 */
		unsigned int			deviceId;

		/*!
		 * id of scaner
		 */
		unsigned int			scanerId;

		/*!
		 * people list ids 
		 */
		QVector<unsigned int>	peopleIds;

		/*!
		 * decsription
		 */
		QString					description;

		/*!
		 * file comment
		 */
		QString					comment;

		/*!
		 * name of file
		 */
		QString					name;

};

/*!
 * \brief it keeps whole information about a foto-video-audio element
 */
class fvaItem
{
	public : 

		fvaItem ();
		virtual ~fvaItem ();

		/*!
		 * \brief it returns human-readable string as item name
		 * \param dictionaries - global dictionary set
		 */
		QString getGuiName(const QVariantMap&	dictionaries);

		/*!
		 * \brief it returns human-readable string to show full name
		 * \param dictionaries - global dictionary set
		 */
		QString getGuiFullName(const QVariantMap&	dictionaries);

	public : // data

		/*!
		 * fs type
		 */		
		FVA_FS_TYPE				type;

		/*!
		 * for folder it is event date period, for file it is date taken(dateFrom only)
		 * both are filesystem names
		 */
		QDateTime				dateFrom;
		QDateTime				dateTo;

		/*!
		 * children
		 */
		std::list<fvaItem*>		children;

		/*!
		 * absolute path to item
		 */
		QString					fsFullPath;

		/*!
		 * does this item fit filtration conditions
		 */
		bool					isFiltered;
		
		/*!
		 * folder fva specific info
		 */
		fvaFolder*				fvaFolder;

		/*!
		 * file fva specific info
		 */
		fvaFile*				fvaFile;
};

/*!
 * it keeps data to perform filtration
 */
class fvaFilter
{
	public :
	
		/*!
		 * date to filter from
		 */
		QDateTime				dateFrom;

		/*!
		 * date to filter to
		 */
		QDateTime				dateTo;

		/*!
		 * places to be filtered
		 */
		QVector<unsigned int>	placeIds;

		/*!
		 * people to be entered
		 */
		QVector<unsigned int>	peopleIds;

		/*!
		 * list of id of foto devices
		 */
		QVector<unsigned int>	deviceIds;

		/*!
		 * folder events
		 */
		QVector<unsigned int>	eventIds;

		/*!
		 * people that are reason of event
		 */
		QVector<unsigned int>	eventReasonPeopleIds;

		/*!
		 * folder tags or file desciption/comment
		 */
		QString					text;

		/*
		 * does ID match to filtration
		 */
		bool doesIDMatchToFilter(unsigned int ID, const QVector<unsigned int>& Ids) const;

		/*
		 * does list of ID matche to filtration
		 */
		bool doIDsMatchToFilter(const QVector<unsigned int>& IDs, const QVector<unsigned int>& filterIds) const;
};

/*!
 * \brief it recursivly filters FVA tree
 * \param filter - filter condition set
 * \param fvaitem - fva tree item
 * \param defFilterDataTime - data+time that is defined as default one
 */
void fvaFilterTree( const fvaFilter& filter, fvaItem* fvaitem, const QDateTime& defFilterDataTime );


#endif // FVACOMMONDATA_H