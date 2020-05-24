#ifndef FVAVIEWER_H
#define FVAVIEWER_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

#include "ui_fvaviewer.h"

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
		FVAViewer( QWidget *parent = 0, Qt::WFlags flags = 0 );
		~FVAViewer();

	private: // methods

		/*!
		 * \brief it expands GUI tree using file system tree
		 * \param folder - file folder path to be populated
		 * \param item - GUI tree
		 */
		void populateTree( const QString& folder, QTreeWidgetItem* item );

	private: // data

		Ui::FVAViewerClass *ui;

	private slots:
		void showItem( QTreeWidgetItem* item );
};

#endif // FVAVIEWER_H
