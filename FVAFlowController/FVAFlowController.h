/*!
* \file FVAFlowController.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef _FVA_FLOW_CONTROLLER_H_
#define _FVA_FLOW_CONTROLLER_H_

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>

#include "fvaexitcodes.h"
#include "fvadevicecontext.h"
#include "fvacltcontext.h"
#include "fvaconfiguration.h"
#include "FVADataProcessor.h"


typedef QList<QString> STR_LIST;

/*!
* \brief DIR_2_ID_MAP maps input dir structure (folder names) to ids
*/
typedef QMap<QString,unsigned int> DIR_2_ID_MAP;

/*!
* \brief DIR_2_IDS_MAP maps input dir structure (folder names) to ids
*/
typedef QMap<QString,QList<unsigned int>> DIR_2_IDS_MAP;

class QObject;

/*!
 * \brief FVAFlowController is a main class to keep a logic of the operation flow and interaction with a user. It implements such functions as:
 *
 * 1. PerformChecksForInputDir - it performs the checks for input folder according to fva configuration;
 * 2. OrganizeInputDir - it performs the organization stuff for the input folder according to fva configuration;
 * 3. MoveInputDirToOutputDirs - it performs the moving input folder content to output folder with checks according to event cfg
 * 4. ProcessInputDirForEvent - it process input folder content as one-event or multi-events according to event cfg and info

 * This class implements "Controller" functions from MVC pattern.
 * Flow control is based on class FvaConfiguration.
 */
class FVAFlowController
{
protected:
friend class FVAFlowControllerTests;
public:
	FVAFlowController();

	/*!
	 * \brief it performs the checks for input folder according to fva configuration
	 * \param dir - directory for the checking
	 * \param deviceContext - devices information got during checks, filled up by this function
	 * \param obj - to attach the child processes to this object
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE PerformChecksForInputDir(const QString& dir, DeviceContext& deviceContext, QObject* obj);

	/*!
	 * \brief it performs the organization stuff for the input folder according to fva configuration
	 * \param dir - directory to perform the actions in
	 * \param deviceId - identifier of device from FVA device dictionary  
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE OrganizeInputDir(const QString& dir, int deviceId);

	/*!
	 * \brief it performs the moving input folder content to output folder with checks according to event cfg
	 * \param inputDir - directory to move content from
	 * \param outputDir - directories to move content into (it is a list of directories to move into)
	 * \param removeInput - flag saying do we need to remove input folder or not
	 * \param obj - to attach the child processes to this object
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE MoveInputDirToOutputDirs(const QString& inputDir, const STR_LIST& outputDirs, bool removeInput, QObject* obj);

	/*!
	 * \brief it updates input folder content according to information got so far (fva info like event info, place info) 
	 * \param inputDir - directory to update content in
	 * \param obj - to attach the child processes to this object
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE UpdateInputDirContent(const QString& inputDir,  QObject* obj);

	/*!
	 * \brief it process input folder content as multi-events folder according to event cfg and info
	 * \param inputDir - directory to perform the actions in
	 * \param eventMap - maps input dir structure (folder names) to event ids got from fvaEvents.csv
	 * \param peopleMap - maps input dir structure (folder names) to people ids (that caused the event) got from fvaPeople.csv
	 * \param obj - to attach the child processes to this object
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE ProcessInputDirForEvents(const QString& inputDir, const DIR_2_ID_MAP& eventMap, const DIR_2_IDS_MAP& peopleMap, QObject* obj);

	/*!
	 * \brief it process input folder content as multi-events folder according to place info
	 * \param placeMap - maps input dir structure (folder names) to place ids got from fvaPlaces.csv
	 * \param obj - to attach the child processes to this object
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE ProcessInputDirForPlaces(const DIR_2_ID_MAP& placeMap, QObject* obj);

	/*!
	 * \brief it returns the list of files (paths) in input folder content that have some problem inside
	 * \param fileListToFillUp - list of files to full up
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE GetProblemFilesList(STR_LIST& fileListToFillUp);
	
protected:
	/*!
	 * \brief it performs the device checks for input folder
	 * \param deviceContext - devices information got during checks, filled up by this function
	 * \param context - one command parameters (environment)
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE performDeviceChecks(DeviceContext& deviceContext, CLTContext& context);

	/*!
	 * \brief it performs the orientation checks for input folder and reorientate the photos for inproper orientation
	 * \param dir - directory for the checking
	 * \param obj - to attach the child processes to this object
	 * \return it returns void
	 */
	void performOrientationChecks(const QString& dir, QObject* obj);

	/*!
	 * \brief it performs the common checks for input folder
	 * \param context - one command parameters (environment)
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)	
	 */
	FVA_EXIT_CODE performCommonChecks(CLTContext& context);

	/*!
	 * \brief it performs the date-time checks for input folder
	 * \param context - one command parameters (environment)
	 * \param obj - to attach the child processes to this object
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE performDTChecks(CLTContext& context, QObject* obj);

	/*!
	 * \brief it runs python command from #script dir
	 * \param scriptName - name of script to run
	 * \param obj - to attach the child processes to this object
	 * \param params - list of params to run the cmd
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE runPythonCMD(const QString& scriptName, QObject* obj, const QStringList& params);

private: //data
	/*!
	 * brief processor to support from the side of the low level actions
	 */
	FVADataProcessor m_dataProcessor;

	/*!
	 * brief globally application configuration
	 */
	FvaConfiguration m_cfg;
};
#endif
