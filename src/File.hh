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
#ifndef FILE_H
#define FILE_H
/** @file src/File.hh
 * @brief Exception-safe stdio wrapper
 */

#include <stdexcept>
#include <string>
#include <cstdio>

/** @brief Exception-safe stdio wrapper
 *
 * A simple wrapper for the stdio @c FILE type with the following
 * features:
 * - exceptions are raised for errors
 * - files are automatically closed in the destructor
 *
 * This allows stdio to be used without constantly checking returns
 * values, catching and re-throwing exceptions in order to perform
 * cleanup, etc.
 *
 * The C99 stream orientation rules still apply.
 *
 * Stdio is preferred due to the terrible syntax, badly designed
 * semantics, and broken implementations, of iostreams.
 *
 * @bug It is not possible to open a file that is actually called
 * "-".
 */
class File {
public:
  /** @brief Construct a (closed) file */
  File(): fp(NULL) {}

  /** @brief Construct and open a file
   * @param path Name of file to open
   * @param mode Mode as per @c fopen
   * @exception std::runtime_error An error occurred opening the file
   *
   * @p path can be "-" to open @c stdin or @c stdout (depending on @p
   * mode).  In this case the file will not be closed on destruction.
   */
  File(const std::string &path, const std::string &mode): fp(NULL) {
    open(path, mode);
  }

  /** @brief Destroy a file
   *
   * If the file is open it is closed, provided it is not @c stdin or
   * @c stdout.
   */
  ~File();

  /** @brief Open the file
   * @param path Name of file to open
   * @param mode Mode as per @c fopen
   * @exception std::runtime_error An error occurred opening the file
   * @exception std::logic_error The file is already open
   *
   * @p path can be "-" to open @c stdin or @c stdout (depending on @p
   * mode).  In this case the file will not be closed on destruction.
   */
  void open(const std::string &path, const std::string &mode);

  /** @brief Close the file.
   * @exception std::runtime_error An error occurred closing the file
   * @exception std::logic_error The file is not open
   *
   * This file is closed even if it is @c stdin or @c stdout.
   */
  void close();

  /** @brief Read one byte
   * @return The byte read, or @c EOF at end of file
   * @exception std::logic_error The file is not open
   * @exception std::runtime_error A read error occurred
   */
  int get();

  /** @brief Read one wide character
   * @return The character read, or @c WEOF at end of file
   * @exception std::logic_error The file is not open
   * @exception std::runtime_error A read error occurred
   */
  wint_t getw();

  /** @brief Write a formatted string
   * @param fmt Format string, as per standard @c printf
   * @param ... Arguments to @c fmt
   * @return Number of bytes written
   * @exception std::logic_error The file is not open
   * @exception std::runtime_error A write error occurred
   */
  int printf(const char *fmt, ...);

  /** @brief Write a byte
   * @param c Byte to write
   * @exception std::logic_error The file is not open
   * @exception std::runtime_error A write error occurred
   */
  void put(int c);

  /** @brief Write a wide character
   * @param wc Character to write
   * @exception std::logic_error The file is not open
   * @exception std::runtime_error A write error occurred
   */
  void putw(int wc);

  /** @brief Flush output */
  void flush();

private:
  FILE *fp;
  std::string path;
};

#endif /* FILE_H */
