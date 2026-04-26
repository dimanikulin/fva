/*!
 * \file fva_qt_port_2_stl.cpp
 * \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
 * \author Dima Nikulin.
 * \version 0.29
 * \date  2014-2021
 */
#include "fva_qt_port_2_stl.h"

#include <sys/stat.h>

#include <algorithm>
#include <cctype>

// TODO create UTs
std::string fvaStrToUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string quoteArg(const std::string& arg) {
    std::string escaped;
    escaped.reserve(arg.size() + 2);
    escaped.push_back('"');
    for (char c : arg) {
        if (c == '"') escaped.push_back('\\');
        escaped.push_back(c);
    }
    escaped.push_back('"');
    return escaped;
}

bool isExistingDirectory(const std::string& dirPath) {
    struct stat pathInfo = {};
    return stat(dirPath.c_str(), &pathInfo) == 0 && ((pathInfo.st_mode & S_IFDIR) != 0);
}

std::string toNativePathString(const std::filesystem::path& path) {
    auto nativePath = path;
    nativePath.make_preferred();
    return nativePath.string();
}

// Helper function to convert string to uppercase
std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}

// Helper function to trim whitespace from string
std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) ++start;
    auto end = str.end();
    do {
        --end;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

// Helper function to remove tabs from string
std::string removeTab(const std::string& str) {
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());
    return result;
}

std::string qtToStrftimeFormat(const std::string& qtFormat) {
    std::string result = qtFormat;

    auto replaceAll = [](std::string& value, const std::string& from, const std::string& to) {
        std::size_t pos = 0;
        while ((pos = value.find(from, pos)) != std::string::npos) {
            value.replace(pos, from.size(), to);
            pos += to.size();
        }
    };

    replaceAll(result, "yyyy", "%Y");
    replaceAll(result, "MM", "%m");
    replaceAll(result, "dd", "%d");
    replaceAll(result, "hh", "%H");
    replaceAll(result, "mm", "%M");
    replaceAll(result, "ss", "%S");

    return result;
}

std::string formatFileTime(const std::filesystem::file_time_type& fileTime, const std::string& qtFormat) {
    using namespace std::chrono;

    const auto systemTimePoint = time_point_cast<system_clock::duration>(
        fileTime - std::filesystem::file_time_type::clock::now() + system_clock::now());
    const std::time_t timeValue = system_clock::to_time_t(systemTimePoint);

    std::tm localTime = {};
#ifdef _WIN32
    localtime_s(&localTime, &timeValue);
#else
    localtime_r(&timeValue, &localTime);
#endif

    std::ostringstream ss;
    ss << std::put_time(&localTime, qtToStrftimeFormat(qtFormat).c_str());
    return ss.str();
}

std::string base64Encode(const std::uint8_t* data, std::size_t size) {
    static const char kAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encoded;
    encoded.reserve(((size + 2) / 3) * 4);

    std::size_t i = 0;
    while (i + 2 < size) {
        const std::uint32_t value = (static_cast<std::uint32_t>(data[i]) << 16U) |
                                    (static_cast<std::uint32_t>(data[i + 1]) << 8U) |
                                    static_cast<std::uint32_t>(data[i + 2]);
        encoded.push_back(kAlphabet[(value >> 18U) & 0x3FU]);
        encoded.push_back(kAlphabet[(value >> 12U) & 0x3FU]);
        encoded.push_back(kAlphabet[(value >> 6U) & 0x3FU]);
        encoded.push_back(kAlphabet[value & 0x3FU]);
        i += 3;
    }

    if (i < size) {
        const std::uint8_t byte0 = data[i];
        const std::uint8_t byte1 = (i + 1 < size) ? data[i + 1] : 0;

        encoded.push_back(kAlphabet[(byte0 >> 2U) & 0x3FU]);
        encoded.push_back(kAlphabet[((byte0 & 0x03U) << 4U) | ((byte1 >> 4U) & 0x0FU)]);
        if (i + 1 < size) {
            encoded.push_back(kAlphabet[(byte1 & 0x0FU) << 2U]);
        } else {
            encoded.push_back('=');
        }
        encoded.push_back('=');
    }

    return encoded;
}

Sha1::Sha1() { reset(); }

void Sha1::update(const std::uint8_t* data, std::size_t len) {
    while (len > 0) {
        const std::size_t copyLen = std::min(len, kBlockSize - m_bufferSize);
        std::memcpy(m_buffer.data() + m_bufferSize, data, copyLen);
        m_bufferSize += copyLen;
        data += copyLen;
        len -= copyLen;

        if (m_bufferSize == kBlockSize) {
            processBlock(m_buffer.data());
            m_totalBits += static_cast<std::uint64_t>(kBlockSize) * 8U;
            m_bufferSize = 0;
        }
    }
}

std::array<std::uint8_t, 20> Sha1::final() {
    m_totalBits += static_cast<std::uint64_t>(m_bufferSize) * 8U;

    m_buffer[m_bufferSize++] = 0x80U;

    if (m_bufferSize > 56) {
        while (m_bufferSize < kBlockSize) m_buffer[m_bufferSize++] = 0;
        processBlock(m_buffer.data());
        m_bufferSize = 0;
    }

    while (m_bufferSize < 56) m_buffer[m_bufferSize++] = 0;

    for (int i = 7; i >= 0; --i) {
        m_buffer[m_bufferSize++] = static_cast<std::uint8_t>((m_totalBits >> (i * 8)) & 0xFFU);
    }
    processBlock(m_buffer.data());

    std::array<std::uint8_t, 20> digest = {};
    for (std::size_t i = 0; i < m_state.size(); ++i) {
        digest[i * 4] = static_cast<std::uint8_t>((m_state[i] >> 24U) & 0xFFU);
        digest[i * 4 + 1] = static_cast<std::uint8_t>((m_state[i] >> 16U) & 0xFFU);
        digest[i * 4 + 2] = static_cast<std::uint8_t>((m_state[i] >> 8U) & 0xFFU);
        digest[i * 4 + 3] = static_cast<std::uint8_t>(m_state[i] & 0xFFU);
    }
    return digest;
}

std::uint32_t Sha1::leftRotate(std::uint32_t value, std::uint32_t bits) {
    return (value << bits) | (value >> (32U - bits));
}

void Sha1::reset() {
    m_state = {0x67452301U, 0xEFCDAB89U, 0x98BADCFEU, 0x10325476U, 0xC3D2E1F0U};
    m_buffer.fill(0);
    m_bufferSize = 0;
    m_totalBits = 0;
}

void Sha1::processBlock(const std::uint8_t* block) {
    std::uint32_t w[80] = {};
    for (int i = 0; i < 16; ++i) {
        w[i] = (static_cast<std::uint32_t>(block[i * 4]) << 24U) |
               (static_cast<std::uint32_t>(block[i * 4 + 1]) << 16U) |
               (static_cast<std::uint32_t>(block[i * 4 + 2]) << 8U) | static_cast<std::uint32_t>(block[i * 4 + 3]);
    }
    for (int i = 16; i < 80; ++i) {
        w[i] = leftRotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1U);
    }

    std::uint32_t a = m_state[0];
    std::uint32_t b = m_state[1];
    std::uint32_t c = m_state[2];
    std::uint32_t d = m_state[3];
    std::uint32_t e = m_state[4];

    for (int i = 0; i < 80; ++i) {
        std::uint32_t f = 0;
        std::uint32_t k = 0;
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999U;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1U;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDCU;
        } else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6U;
        }

        const std::uint32_t temp = leftRotate(a, 5U) + f + e + k + w[i];
        e = d;
        d = c;
        c = leftRotate(b, 30U);
        b = a;
        a = temp;
    }

    m_state[0] += a;
    m_state[1] += b;
    m_state[2] += c;
    m_state[3] += d;
    m_state[4] += e;
}
