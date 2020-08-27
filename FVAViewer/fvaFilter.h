#ifndef FVAFILTER_H
#define FVAFILTER_H

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
		{
			for (auto it = Ids.begin(); it != Ids.end(); ++it)
			{
				if (ID == *it)
					return  true;
			}
			return false;
		}

		/*
		 * does list of ID matche to filtration
		 */
		bool doIDsMatchToFilter(const QVector<unsigned int>& IDs, const QVector<unsigned int>& filterIds) const;
		{
			for (auto it = IDs.begin(); it != IDs.end(); ++it)
			{
				if (doesIDMatchToFilter(*it, filterIds))
					return  true;
			}
			return false;
		}


};

#endif // FVAFILTER_H

