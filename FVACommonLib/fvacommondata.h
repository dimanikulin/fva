#ifndef FVACOMMONDATA_H
#define FVACOMMONDATA_H

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
		 */
		QString getGuiName();

		/*!
		 * \brief it returns human-readable string to show full name
		 * \param dictionaries - global dictionary set
		 */
		QString getGuiFullName(const QVariantMap&	dictionaries);

		/*!
		 * is it folder or file
		 */
		bool					isFolder;

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
		 * set of place id 
		 */
		QVector<unsigned int>	placeIds;

		/*!
		 * people list ids 
		 */
		QVector<unsigned int>	peopleIds;

		/*!
		 * id of foto device 
		 */
		unsigned int			deviceId;

		/*!
		 * id of scaner
		 */
		unsigned int			scanerId;

		/*!
		 * folder event or file decsription
		 */
		QString					eventOrDesc;

		/*!
		 * folder tags or file comment
		 */
		QString					tagsOrComment;

		/*!
		 * children
		 */
		QVector<fvaItem*>		children;

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
		 * folder event or file decsription
		 */
		QString					eventOrDesc;

};

#endif // FVACOMMONDATA_H