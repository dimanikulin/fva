/*!
* \file CLTContext.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/

#ifndef CLT_CONTEXT_H_
#define CLT_CONTEXT_H_

#include <QtCore/QString>

/*!
 * \brief to keep command line task context (+settings) that is known before task is started
 */
class CLTContext
{
	public: //data
		/*!
		 * \brief human - readable type of command
		 */
		QString cmdType;

		/*!
		 * \brief path to work folder of command to work in
		 */
		QString dir;

		/*!
		 * \brief path to output folder of command to work in
		 */
		QString outputDir;

		/*!
		 * \brief does command work in read-only mode
		 */
		bool readOnly;

		/*!
		 * \brief a parameter to customize a command  - might be string or integer inside
		 */
		QString custom;

		/*!
		 * \brief is it recursive mode now
		 */
		bool recursive;

	public: //methods
		/*!
		 * \breif - default ctor
		 */
		CLTContext() : cmdType(""), dir(""), readOnly(false), custom(""), recursive(true){}

		operator=(const CLTContext& other)
		{
			cmdType = other.cmdType;
			dir = other.dir;
			outputDir = other.outputDir;
			readOnly = other.readOnly;
			custom = other.custom;
			recursive = other.recursive;
		}
};
#endif // CLT_CONTEXT_H_