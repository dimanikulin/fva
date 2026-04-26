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

#include <filesystem>
#include <string>

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
