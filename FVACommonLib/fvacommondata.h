#ifndef FVACOMMONDATA_H
#define FVACOMMONDATA_H

/*!
 * \brief it enumerates internal valuable file system types
 */
enum FVA_FS_TYPE
{
	FVA_FS_TYPE_UNKNOWN = 0,
	FVA_FS_TYPE_IMG		= 1,
	FVA_FS_TYPE_VIDEO	= 2,
	FVA_FS_TYPE_AUDIO	= 3,

	FVA_FS_TYPE_DIR		= 4,
};
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

typedef QMap<int, fvaDevice> DEVICE_MAP;

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
		 * does this item have any description data
		 */
		bool					hasDescriptionData;

		/*!
		 * event id 
		 */
		unsigned int			eventId;

		/*!
		 * place id 
		 */
		unsigned int			placeId;

		/*!
		 * id of foto device 
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
		 * people that are reason of event
		 */
		QVector<unsigned int>	eventReasonPeopleIds;

		/*!
		 * file decsription
		 */
		QString					description;

		/*!
		 * folder tags or file comment
		 */
		QString					tagsOrComment;

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
		 * folder to show together with this folder
		 */
		QString					linkedFolder;
		
		/*!
		 * structure of description file 
		 */
		QStringList				descTitles; 

		/*!
		 * content of description file 
		 */
		DESCRIPTIONS_MAP		decsItems;
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

#endif // FVACOMMONDATA_H