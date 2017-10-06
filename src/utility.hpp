/*************************************************************************
** utility.hpp                                                          **
**                                                                      **
** This file is part of dvisvgm -- a fast DVI to SVG converter          **
** Copyright (C) 2005-2017 Martin Gieseking <martin.gieseking@uos.de>   **
**                                                                      **
** This program is free software; you can redistribute it and/or        **
** modify it under the terms of the GNU General Public License as       **
** published by the Free Software Foundation; either version 3 of       **
** the License, or (at your option) any later version.                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program; if not, see <http://www.gnu.org/licenses/>. **
*************************************************************************/

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace math {

constexpr const double PI      = 3.141592653589793238462643383279502884;
constexpr const double HALF_PI = 1.570796326794896619231321691639751442;
constexpr const double TWO_PI  = 6.283185307179586476925286766559005768;
constexpr const double SQRT2   = 1.414213562373095048801688724209698079;

inline double deg2rad (double deg) {return PI*deg/180.0;}

} // namespace math

namespace util {

template <typename T>
std::string tohex (T val) {
	std::ostringstream oss;
	oss << std::hex << val;
	return oss.str();
}

std::string trim (const std::string &str, const char *ws=" \t\n\r\f");
std::string normalize_space (std::string str, const char *ws=" \t\n\r\f");
std::string tolower (const std::string &str);
std::string replace (std::string str, const std::string &find, const std::string &repl);
std::vector<std::string> split (const std::string &str, const std::string &sep);
int ilog10 (int n);


/** Encodes the bytes in the half-open range [first,last) to Base64 and writes
 *  the result to the range starting at 'dest'.
 *  @param[in] first initial position of the range to be encoded
 *  @param[in] last final position of the range to be encoded
 *  @param[in] dest first position of the destination range */
template <typename InputIterator, typename OutputIterator>
void base64_copy (InputIterator first, InputIterator last, OutputIterator dest) {
	static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	while (first != last) {
		int padding = 0;
		unsigned char c0 = *first++, c1=0, c2=0;
		if (first == last)
			padding = 2;
		else {
			c1 = *first++;
			if (first == last)
				padding = 1;
			else
				c2 = *first++;
		}
		uint32_t n = (c0 << 16) | (c1 << 8) | c2;
		for (int i=0; i <= 3-padding; i++) {
			*dest++ = base64_chars[(n >> 18) & 0x3f];
			n <<= 6;
		}
		while (padding--)
			*dest++ = '=';
	}
}


/** Simple implementation mimicking std::make_unique introduced in C++14.
 *  Constructs an object of class T on the heap and returns a unique_ptr<T> to it.
 *  @param[in] args arguments forwarded to an constructor of T */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique (Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace util

#endif
