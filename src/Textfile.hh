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
#ifndef TEXTFILE_H
#define TEXTFILE_H
/** @file src/Textfile.hh
 * @brief Text file processing
 *
 * Use @ref Textfile to read a representation of a text file, using
 * backspace-based overprinting to represent bold and underline.
 *
 * Subclass @ref TextfileProcessor and pass it to Textfile::process to
 * get the contents back out in a usable form.
 */

#include <string>

/** @brief Base class for text file processors
 *
 * A user of @ref Textfile is expected to subclass TextfileProcessor
 * and implement at least the text() method.
 */
class TextfileProcessor {
public:
  TextfileProcessor(): boldState(false), underlineState(false) {}

  /** @brief Called at the start of a file
   *
   * Called from Textfile::process().
   *
   * The default implementation does nothing.
   */
  void begin();

  /** @brief Enable or disable bold
   * @param state @c true to turn bold on and @c false to turn it off
   *
   * Called from Textfile::process().  This method should only be called if the
   * bold state has changed.
   *
   * The default implementation does nothing.
   */
  virtual void bold(bool state);

  /** @brief Enable or disable underline
   * @param state @c true to turn underline on and @c false to turn it off
   *
   * Called from Textfile::process().  This method should only be called if the
   * underline state has changed.
   *
   * The default implementation does nothing.
   */
  virtual void underline(bool state);

  /** @brief Output text
   * @param s Text string to output
   *
   * Called from Textfile::process().
   */
  virtual void text(const std::wstring &s) = 0;

  /** @brief Output text
   * @param s Text string to output
   *
   * Converts @p s to a wide-character string and calls the virtual
   * method.
   */
  void text(const std::string &s);

  /** @brief Called at the end of a file
   *
   * Called from Textfile::process().
   *
   * The default implementation disables bold and underline, if they
   * are enabled.
   */
  void end();

private:
  bool boldState, underlineState;

  void character(wchar_t c, bool isBold = false, bool isUnderlined = false);

  friend class Textfile;
};

/** @brief Text file representation
 *
 * Reads text files that are encoded in the following way:
 * - they use the current locale's multibyte encoding
 * - they use overprinting with @c BACKSPACE for bold and underline
 *
 * The text file can then be read back by calling process() and
 * implementing TextfileProcessor::text() and perhaps
 * TextfileProcessor::bold() and TextfileProcessor::underline() in a
 * subclass of TextfileProcessor.
 *
 * Text files must be small enough to fit in memory.  On modern
 * general-purpose computers this isn't likely to be a problem.
 */
class Textfile {
public:
  /** @brief Construct an empty Textfile */
  Textfile();

  /** @brief Destroy a Textfile */
  ~Textfile();

  /** @brief Set the tab stop
   * @param n The new tab stop value
   *
   * The default tab stop is 8.
   */
  inline void setTabStop(int n) { tabstop = n; }

  /** @brief Read a file
   * @param path Name of file to read, or "-" for stdin
   * @exception std::runtime_error An IO error occured
   *
   * One Textfile can be used to process multiple inputs by calling
   * this method multiple times.  Each time, the old input will be
   * discarded.  If an error occurs then the stored text is undefined.
   */
  void open(const std::string &path);

  /** @brief Process a text file
   * @param processor Object to supply text and markup to
   *
   * This calls the TextfileProcessor::bold(),
   * TextfileProcessor::underline() and TextfileProcessor::text()
   * methods with the current text.
   *
   * The bold/underline methods are never called with the same state
   * twice in a row; i.e. they will only ever be called if the state
   * actually changes.
   *
   * @bug Currently TextfileProcessor::text() is only ever called with
   * single (wide) characters.
   */
  void process(TextfileProcessor &processor);

private:
  std::wstring contents;
  int tabstop;
};

#endif /* TEXTFILE_H */

