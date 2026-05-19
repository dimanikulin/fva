#ifndef FVA_QT_PORT_2_STL_H
#define FVA_QT_PORT_2_STL_H

// TODO to create name space for this file and move it to commonlib/utils or something like that
/*!
 * \file fva_qt_port_2_stl.h
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>

// TODO make Unit tests for these functions and class

/*!
 * \brief Helper function to convert string to uppercase
 * \param str The input string
 * \return The uppercase version of the input string
 */
std::string fvaStrToUpper(const std::string& str);

/*!
 * \brief Helper function to quote a command-line argument
 * \param arg The input argument
 * \return The quoted version of the input argument
 */
std::string quoteArg(const std::string& arg);

/*!
 * \brief Helper function to check if a directory exists
 * \param dirPath The path to the directory
 * \return true if the directory exists, false otherwise
 */
bool isExistingDirectory(const std::string& dirPath);

/*!
 * \brief Helper function to convert a filesystem path to a native string format
 * \param path The input filesystem path
 * \return The native string representation of the input path
 */
std::string toNativePathString(const std::filesystem::path& path);

/*!
 * \brief Helper function to convert string to uppercase
 * \param str The input string
 * \return The uppercase version of the input string
 */
std::string toUpper(const std::string& str);

/*!
 * \brief Helper function to trim whitespace from string
 * \param str The input string
 * \return The trimmed version of the input string
 */
std::string trim(const std::string& str);

/*!
 * \brief Helper function to remove tabs from string
 * \param str The input string
 * \return The string with tabs removed
 */
std::string removeTab(const std::string& str);

/*!
 * \brief Helper function to convert Qt date/time format to strftime format
 * \param qtFormat The input Qt date/time format string
 * \return The corresponding strftime format string
 */
std::string qtToStrftimeFormat(const std::string& qtFormat);

/*!
 * \brief Helper function to format file time using a specified Qt format
 * \param fileTime The input file time
 * \param qtFormat The Qt date/time format string
 * \return The formatted date/time string
 */
std::string formatFileTime(const std::filesystem::file_time_type& fileTime, const std::string& qtFormat);

/*!
 * \brief Helper function to encode data in Base64 format
 * \param data The input data to encode
 * \param size The size of the input data
 * \return The Base64 encoded string
 */
std::string base64Encode(const std::uint8_t* data, std::size_t size);

/*!
 * \brief Helper function to get the application directory path
 * \return The directory path where the application is located
 */
std::string getApplicationDirPath();

/*!
 * \brief Helper function to parse a date/time string using a specified Qt format
 * \param value The input date/time string to parse
 * \param qtFormat The Qt date/time format string
 * \param result The output tm structure to hold the parsed date/time
 * \return true if parsing was successful, false otherwise
 */
bool parseDateTime(const std::string& value, const std::string& qtFormat, std::tm& result);

/*!
 * \brief Helper function to add a specified number of days to a tm structure
 * \param value The input tm structure representing the date/time
 * \param days The number of days to add
 * \return true if the operation was successful, false otherwise
 */
bool addDays(std::tm& value, int days);

/*!
 * \brief Helper function to get the last write time of a file as a system clock time point
 * \param filePath The path to the file
 * \return The last write time of the file as a system clock time point, or an empty time point if an error occurs
 */
std::chrono::system_clock::time_point fvaLastWriteTime(const std::filesystem::path& filePath);

/*!
 * \brief Helper function to convert a tm structure to a system clock time point
 * \param value The input tm structure representing the date/time
 * \return The corresponding system clock time point, or an empty time point if the conversion fails
 */
std::chrono::system_clock::time_point fromStdTm(std::tm value);

/*!
 * \brief Helper function to format a system clock time point as a string using a specified Qt format
 * \param value The input system clock time point to format
 * \param qtFormat The Qt date/time format string
 * \return The formatted date/time string, or an empty string if the input time point is empty
 */
std::string formatDateTime(const std::chrono::system_clock::time_point& value, const std::string& qtFormat);

/*!
 * \brief A simple implementation of the SHA-1 hashing algorithm
 */
class Sha1 {
public:
    /*!
     * \brief Constructs a new Sha1 object and initializes its state
     */
    Sha1();

    /*!
     * \brief Updates the hash state with the given data
     * \param data The input data to update the hash with
     * \param len The length of the input data
     */
    void update(const std::uint8_t* data, std::size_t len);

    /*!
     * \brief Finalizes the hash computation and returns the resulting digest
     * \return The SHA-1 digest as an array of 20 bytes
     */
    std::array<std::uint8_t, 20> final();

private:
    static constexpr std::size_t kBlockSize = 64;

    /*!
     * \brief Performs a left rotation on a 32-bit value
     * \param value The value to rotate
     * \param bits The number of bits to rotate
     * \return The rotated value
     */
    static std::uint32_t leftRotate(std::uint32_t value, std::uint32_t bits);

    /*!
     * \brief Resets the hash state to its initial values
     */
    void reset();

    /*!
     * \brief Processes a single 512-bit block of data
     * \param block The input block to process
     */
    void processBlock(const std::uint8_t* block);

    std::array<std::uint32_t, 5> m_state = {};
    std::array<std::uint8_t, kBlockSize> m_buffer = {};
    std::size_t m_bufferSize = 0;
    std::uint64_t m_totalBits = 0;
};

#endif  // FVA_QT_PORT_2_STL_H
