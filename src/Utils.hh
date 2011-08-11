// 
// Copyright (C) 2011 Richard Kettlewell
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
#ifndef UTILS_H
#define UTILS_H
/** @file src/Utils.hh
 * @brief Utility functions
 */

#include <string>

/** @brief Convert a multibyte string to a wide string
 * @param s Multibyte string to convert
 * @return Wide-character string equivalent to @p s
 * @exception std::runtime_error An error occurred converting @p s
 */
std::wstring narrowToWide(const std::string &s);

/** @brief Convert a wide string to a UTF-8 string
 * @param s Wide string to convert
 * @return UTF-8 string equivalent to @p s
 *
 * It is assumed that the characters in @p s are valid Unicode code
 * points - if they are not then the UTF-8 will be gibberish.
 */
std::string wideToUtf8(const std::wstring &s);

#endif /* UTILS_H */

