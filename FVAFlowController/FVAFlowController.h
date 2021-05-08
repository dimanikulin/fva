#ifndef _FVA_FLOW_CONTROLLER_H_
#define _FVA_FLOW_CONTROLLER_H_

#include <QtCore/QString>

#include "fvaexitcodes.h"
#include "fvadevicecontext.h"
#include "fvacltcontext.h"
#include "fvaconfiguration.h"
#include "FVADataProcessor.h"

class QObject;

/*!
 * \brief it is a main class to keep a logic of the operation flow and interaction with a user 
 */
class FVAFlowController
{
public:
	FVAFlowController();
	virtual ~FVAFlowController(){};

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
	* \param outputDir - directory to move content into
	* \return it returns code of error (FVA_NO_ERROR - if no error happened)
	*/
	FVA_EXIT_CODE MoveInputDirToOutputDir(const QString& inputDir, const QString& outputDir);

	/*!
	* \brief it process input folder content as one-event or multi-events according to event cfg
	* \param inputDir - directory to move content from
	* \param outputDir - directory to move content into
	* \return it returns code of error (FVA_NO_ERROR - if no error happened)
	*/
	FVA_EXIT_CODE ProcessInputDirForEventCfg(const QString& inputDir, const QString& outputDir);

private:

	/*!
	 * \brief it performs the device checks for input folder
	 * \param deviceContext - devices information got during checks, filled up by this function
	 * \param context - one command parameters (environment)
	 * \param cfg - system configuration, applicable for whole system
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)
	 */
	FVA_EXIT_CODE performDeviceChecks(DeviceContext& deviceContext, CLTContext& context, const FvaConfiguration& cfg);

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
	 * \param cfg - system configuration, applicable for whole system
	 * \return it returns code of error (FVA_NO_ERROR - if no error happened)	
	 */
	FVA_EXIT_CODE performCommonChecks(CLTContext& context, const FvaConfiguration& cfg);

	/*!
	* \brief it performs the date-time checks for input folder
	* \param context - one command parameters (environment)
	* \param cfg - system configuration, applicable for whole system
	* \return it returns code of error (FVA_NO_ERROR - if no error happened)
	*/
	FVA_EXIT_CODE performDTChecks(CLTContext& context, const FvaConfiguration& cfg);

	/*!
	* \brief it performs the location checks for input folder
	* \param context - one command parameters (environment)
	* \param cfg - system configuration, applicable for whole system
	* \return it returns code of error (FVA_NO_ERROR - if no error happened)
	*/
	FVA_EXIT_CODE performLocationChecks(CLTContext& context, const FvaConfiguration& cfg);

private: //data

	/*!
	 * brief processor to support the frow from low level actions
	 */
	FVADataProcessor m_dataProcessor;

	/*!
	* brief global application configuration
	*/
	FvaConfiguration m_cfg;
};
#endif

