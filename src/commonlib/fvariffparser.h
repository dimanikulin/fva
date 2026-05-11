/*!
 * \file FvaRiffParser.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#ifndef _RIFF_PARSER_H_
#define _RIFF_PARSER_H_

#include <chrono>
#include <fstream>
#include <memory>
#include <string>

#include "fvafmtcontext.h"

/*!
 * \brief contains functionality to worj with RIFF files
 * see https://en.wikipedia.org/wiki/Resource_Interchange_File_Format for the details
 */
class RiffParser {
public:  // methods
    /*
     * \brief it opens file and makes premialiry checks for consitensy
     * \param path riff file absolute path
     * \param error human-readable description of error if any
     * \return it returns true if it succesfully opened the file, otherwize it returns false
     */
    bool open(const std::string& path, std::string& error);

    /*!
     * \brief it finds string type tag by its name
     * \param tag - name of tag to be found
     * \param value out param to be filled by value of tag
     * \return it returns true if it succesfully found the tag, otherwize it returns false
     */
    bool findTag(const std::string& tag, std::string& value);

    /*!
     * \brief it converts string type to date-time type
     * \param strDate - human-readable date string
     * \param value - out param to be filled by date-time value
     * \param ctx - to use formatting options from
     * \return it returns true if it succesfully converted the value, otherwize it returns false
     */
    bool convertToDate(const std::string& strDate, std::chrono::system_clock::time_point& value,
                       const FvaFmtContext& ctx) const;

private:  // methods
    /*!
     * \brief it finds string type tag by its name inside of current node
     * \param tag - name of tag to be found
     * \param value out param to be filled by value of tag
     * \return it returns true if it succesfully found the tag, otherwize it returns false
     */
    bool processNode(const std::string& tag, std::string& value);

private:  // data
    /*!
     * file stream object
     */
    std::unique_ptr<std::ifstream> m_file;
};

#endif  // _RIFF_PARSER_H_
