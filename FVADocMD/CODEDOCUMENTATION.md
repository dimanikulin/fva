# Headline
TBD

# Article description
TBD 

# Tags
TBD

# Content

## C++ Documentation rules
&nbsp;&nbsp;&nbsp; The C++ code shall be documented using [Doxygen](https://en.wikipedia.org/wiki/Doxygen) to increase testability, supportability and maintainability.
The following requirements shall be applied for the C++ code:
-	Each source file shall have copyright header as shown below;
```cpp
/*!
* \file FVABaseDictionaryItem.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2022
*/
```
-	Each class shall have a header to describe the class purpose like here
```cpp
/*!
* \brief FVAOrganizerEventCfgPage is a child of QWizardPage and implements the next UI functions:
*
* 1. "Explanation words" to user in a QLabel;
* 2. "See input dir" button (QPushButton) to open input dir where processed files are in.
* 3. QTextBrowser to output the logging events;
* This class implements "View" functions from MVC pattern.
*/
class FVAOrganizerEventCfgPage : public QWizardPage
{
}
```
-	Each global constant shall be documented with "\brief" like here:
```cpp
/*!
 * \brief id for any object that is not defined
 */
const int FVA_UNDEFINED_ID	= -1;
```
-	Each public function shall have a description of purpose, each parameter description and description of return value as well. Please see for example below:
```cpp
/*!
 * \brief it returns an ID value from file by file path provided
 * \param file - path to file that contains the ID
 * \param int& - variable to fill up by ID loaded from the file
 * \returns it returns code of error if any or FVA_NO_ERROR if loading was successful
 */
FVA_EXIT_CODE fvaGetIDFromFile(const QString& file, int& ID);
```
## Python Documentation rules
The Python code shall be documented by [TBD](https://realpython.com/documenting-python-code/)
The following requirements shall be applied for the python code:
-	TBD

## Folder level rules
README.MD file must be present in each source folder to describe the directory sources.
[This](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet) resource might help

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Doxygen](https://en.wikipedia.org/wiki/Doxygen)| [Doxygen](https://en.wikipedia.org/wiki/Doxygen)  is a [documentation generator](https://en.wikipedia.org/wiki/Documentation_generator) and [static analysis](https://en.wikipedia.org/wiki/Static_analysis) tool for software [source trees](https://en.wikipedia.org/wiki/Source_code#Organization). When used as a documentation generator, Doxygen extracts information from specially-formatted comments within the code.|
| 2 | [MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller)| Model–view–controller (usually known as MVC) is a [software design pattern](https://en.wikipedia.org/wiki/Software_design_pattern) commonly used for developing [user interfaces](https://en.wikipedia.org/wiki/User_interface) that divides the related program logic into three interconnected elements. This is done to separate internal representations of information from the ways information is presented to and accepted from the user|