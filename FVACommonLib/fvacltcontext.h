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

};
#endif // CLT_CONTEXT_H_