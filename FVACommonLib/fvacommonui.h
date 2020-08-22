#ifndef FVACOMMONUI_H
#define FVACOMMONUI_H

#include <QtCore/QString>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>

#include "fvaerrors.h"

/*!
 * \brief it makes showing images in the most common way
 * \param fileName - path to file
 * \param imgLabel to show image at
 * \param text - text to draw over an image
 * \returns it returns code of error if any or FVA_NO_ERROR if showing was successful
 */
FVA_EXIT_CODE fvaShowImage( const QString& fileName, QLabel* imgLabel, const QString& text);

/*!
 * \brief it created filter tree in gui widgets with two levels
 * \param pMainWnd - window keeping tree widget  
 * \param pTreeWidget - widget to build tree in
 * \param rootLevel - root level dictionary
 * \param level - second level dictionary
 */
void fvaBuildFilterTree(QWidget* pMainWnd, 
					QTreeWidget* pTreeWidget, 
					const QVariantList& rootLevel, 
					const QVariantList& level,
					QIcon* rootIcon,
					QIcon* icon);

/*!
 * \brief it created filter tree in gui widgets
  * \param pMainWnd - window keeping tree widget  
 * \param pTreeWidget - widget to build tree in
 * \param devices - need to insert devices
 */
void fvaBuildPeopleFilterTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, bool devices, const QVariantMap& dict);

/*!
 * \brief it finds all checked item ID in gui widget tree
 * \param item - to search in
 * \param Ids - arrau to fill by id of checked items
 * \param devices - need to insert devices
 */
void fvaFindCheckedItem(QTreeWidgetItem *item, QVector<unsigned int>& Ids);

/*!
 * \brief it recursivle update checked 
 * \param item - to update in
 * \param column - column
 */
void fvaUpdateChecks(QTreeWidgetItem *item, int column);

#define FVA_MESSAGE_BOX(message)\
QMessageBox msgBox; \
msgBox.setText(message); \
msgBox.exec();

#endif // FVACOMMONUI_H