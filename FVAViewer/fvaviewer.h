#ifndef FVAVIEWER_H
#define FVAVIEWER_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QMainWindow>
#include <QTCore/QtCore>
#include <QtWidgets/QTreeWidget>

#include "fvacommonlib.h"
#include "FVADescriptionFile.h"
#include "fvafilter.h"

#include <memory>

#include "ui_fvaviewer.h"
#include "ui_filters.h"
#include "ui_d-filter.h"
#include "ui_geo-filter.h"
#include "ui_h-filter.h"
#include "ui_e-filter.h"

class QProgressDialog;
class fvaItem;

/*!
 * \brief keeps main dilaog functionality
 */
class FVAViewer : public QDialog 
{
	Q_OBJECT

	public:

		/*!
		 * \brief ctors-dtors section
		 */
		FVAViewer( const QString& rootDir, const QString& dictPath, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
		~FVAViewer();

	private: // methods

		/*!
		 * \brief it recursivly expands GUI tree using fva tree
		 * \param fvaitem - fvaitem to populate from
		 * \param item - GUI tree
		 */
		void populateGUITree( const fvaItem* fvaitem, QTreeWidgetItem* item, int& number, QProgressDialog* progress );

		/*!
		 * \brief it starts showing progress for loading fs tree
		 */
		void showProgress(const QString& rootDir);

		/*!
		 * \brief it initializes gui widgets to show filters
		 */
		void prepareFilters();

		/*!
		* \brief it recursivly filters FVA tree
		* \param filter - filter condition set
		* \param fvaitem - fva tree item
		* \param defFilterDataTime - data+time that is defined as default one
		*/
		void fvaFilterTree(const fvaFilter& filter, fvaItem* fvaitem, const QDateTime& defFilterDataTime);

	private: // data

		Ui::FVAViewerClass *		m_ui;

		/*!
		 * filters
		 */
		Ui_commonFilter*			m_uiFiltersCommon;
		Ui_dfilter*					m_uiDeviceFilter;
		Ui_placeFilter*				m_uiPlaceFilter;
		Ui_hfilter*					m_uiPeopleFilter;
		Ui_eventFilter*				m_uiEventFilter;

		/*!
		 * root item
		 */
		std::unique_ptr<fvaItem>	m_rootItem;

		/*!
		 * filter condition
		 */
		fvaFilter					m_filter;

		/*!
		 * default date-time of filter date-time fields
		  */
		QDateTime					m_defFilterDataTime;

		/*!
		 * global dictionaries
		 */
		QVariantMap					m_dictionaries;

		/*!
		 * path to dictionaries
		 */
		QString						m_dictionaryPath;

		/*!
		 * icon set to improve performance 
		 */		
		QIcon						m_videoIcon;
		QIcon						m_audioIcon;
		QIcon						m_photoIcon;
		QIcon						m_folderIcon;
		QIcon						m_locationIcon;

	private slots:

		void showItem( QTreeWidgetItem* item );

		void editFileItem( QTreeWidgetItem* item );
		
		void filterClicked(  );

		void ShowContextMenu(const QPoint & point);

		void updateChecks(QTreeWidgetItem*, int);
};

#endif // FVAVIEWER_H