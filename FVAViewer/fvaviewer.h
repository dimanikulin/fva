#ifndef FVAVIEWER_H
#define FVAVIEWER_H

#include <QTGui/QDialog>
#include <QTGui/QMainWindow>
#include <QTCore/QtCore>
#include <QTGui/QTreeWidget>

#include "fvacommonlib.h"
#include "FVADescriptionFile.h"

#include "ui_fvaviewer.h"
#include "ui_filters.h"

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
		FVAViewer( const QString& rootDir, const QString& dictPath, QWidget *parent = 0, Qt::WFlags flags = 0 );
		~FVAViewer();

	private: // methods

		/*!
		 * \brief it recursivly expands GUI tree using fva tree
		 * \param fvaitem - fvaitem to populate from
		 * \param item - GUI tree
		 */
		void populateGUITree( const fvaItem* fvaitem, QTreeWidgetItem* item );

		/*!
		 * \brief it recursivly expands FVA tree using file system tree
		 * \param folder - folder path to be populated
		 * \param item - fva tree item
		 */
		void populateFVATree( const QString& folder, fvaItem* fvaitem );

		/*!
		 * \brief it recursivly filters FVA tree
		 * \param filter - filter condition set
		 * \param fvaitem - fva tree item
		 */
		void filterFVATree( const fvaFilter& filter, fvaItem* fvaitem );

	private: // data

		Ui::FVAViewerClass *		m_ui;

		Ui_Filters*					m_uiFilters;
		/*!
		 * root item
		 */
		std::auto_ptr<fvaItem>		m_rootItem;

		/*!
		 * filter condition
		 */
		fvaFilter					m_filter;

		/*!
		 * file with descriptions for FVA items in a folder
		 */
		FVADescriptionFile			m_descriptionFile;

		/*!
		 * default date-time of filter date-time fields
		 */
		QDateTime					m_defFilterDataTime;

		/*!
		 * global dictionaries
		 */
		QVariantMap					m_dictionaries;

		/*!
		 * dictionaries path
		 */
		QString						m_dictionaryPath;

		/*!
		 * icon set to improve performance 
		 */		
		QIcon						m_videoIcon;
		QIcon						m_audioIcon;
		QIcon						m_photoIcon;
		QIcon						m_folderIcon;

	private slots:

		void showItem( QTreeWidgetItem* item );

		void editFileItem( QTreeWidgetItem* item );
		
		void filterClicked(  );

		void ShowContextMenu(const QPoint & point);
};

#endif // FVAVIEWER_H
