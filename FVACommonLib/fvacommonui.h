/*!
* \file fvacommonui.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef FVACOMMONUI_H
#define FVACOMMONUI_H

#include <QtCore/QString>
#include <QLabel>
#include <QTreeWidget>
#include <QMessageBox>

#include "fvaexitcodes.h"

/*!
 * \brief it makes showing images in the most common way
 * \param fileName - path to file
 * \param imgLabel to show image at
 * \param text - text to draw over an image
 * \returns it returns code of error if any or FVA_NO_ERROR if showing was successful
 */
FVA_EXIT_CODE fvaShowImage( const QString& fileName, QLabel* imgLabel, const QString& text);

/*!
 * \brief it creates people tree in gui widgets
 * \param pMainWnd - window keeping tree widget
 * \param pTreeWidget - widget to build tree in
 * \param rootSWdir - dir where FVA SW is located
 * \returns it returns code of error if any or FVA_NO_ERROR if building was successful
 */
FVA_EXIT_CODE fvaBuildPeopleTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, bool devices, const QString& rootSWdir);

/*!
 * \brief it creates event tree in gui widgets
 * \param pMainWnd - window keeping tree widget
 * \param pTreeWidget - widget to build tree in
 * \param rootSWdir - dir where FVA SW is located
 * \returns it returns code of error if any or FVA_NO_ERROR if building was successful
 */
FVA_EXIT_CODE fvaBuildEventTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, const QString& rootSWdir);

/*!
 * \brief it creates place tree in gui widgets
 * \param pMainWnd - window keeping tree widget
 * \param pTreeWidget - widget to build tree in
 * \param rootSWdir - dir where FVA SW is located
 * \returns it returns code of error if any or FVA_NO_ERROR if building was successful
 */
FVA_EXIT_CODE fvaBuildPlaceTree(QWidget* pMainWnd, QTreeWidget* pTreeWidget, const QString& rootSWdir);

/*!
 * \brief it finds all checked item ID in gui widget tree
 * \param item - to search in
 * \param Ids - arrau to fill by id of checked items
 * \returns it returns nothing
 */
void fvaFindCheckedItem(QTreeWidgetItem *item, QList<unsigned int>& Ids);

/*!
 * \brief it recursivly updates checks
 * \param item - to update in
 * \param column - column
 * \returns it returns nothing
 */
void fvaUpdateChecks(QTreeWidgetItem *item, int column);

/*!
 * \brief it recursivly clear checks
 * \param item - to update in
 * \returns it returns nothing
 */
void fvaClearChecks(QTreeWidgetItem *item);

/*!
 * \brief it shows input dir structure 
 * \param folder - to show a folder in GUI
 * \param item - current tree item we work with
 * \param treeWidget - a widget we draw dir structure on
 * \param problemFileList - a list of files with some problems (if file name in this list and a file name in UI match - we will mark it by red)
 * \returns it returns nothing
 */
void fvaPopulateInputDir(const QString& folder, QTreeWidgetItem* item, QTreeWidget* treeWidget, const STR_LIST& problemFileList);

#define FVA_MESSAGE_BOX(message)\
	QMessageBox msgBox; \
	msgBox.setText(message); \
	msgBox.exec();

#define IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_EXITCODE(cmdName)\
if (exitCode != FVA_NO_ERROR)\
{\
	QMessageBox msgBox; \
	msgBox.setText(QString("Fva cmd ") + cmdName + " failed with error " + QString::number(exitCode)); \
	msgBox.exec(); \
	return exitCode; \
}

#define IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET_FALSE(cmdName)\
if (exitCode != FVA_NO_ERROR)\
{\
	QMessageBox msgBox; \
	msgBox.setText(QString("Fva cmd ") + cmdName + " failed with error " + QString::number(exitCode)); \
	msgBox.exec(); \
	return false; \
}

#define IF_CLT_ERROR_SHOW_MSG_BOX_AND_RET(cmdName)\
if (exitCode != FVA_NO_ERROR)\
{\
	QMessageBox msgBox; \
	msgBox.setText(QString("Fva cmd ") + cmdName + " failed with error " + QString::number(exitCode)); \
	msgBox.exec(); \
	return; \
}
#endif // FVACOMMONUI_H