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
#ifndef OPTIONAL_H
#define OPTIONAL_H
/** @file src/Optional.hh
 * @brief Optional value template
 */
#include <stdexcept>

/** @brief Optional value template
 * @param T type to wrap
 *
 * Adds an extra "nonexistent" value to type @p T.
 */
template <typename T> class Optional {
public:
  /** @brief Construct with a nonexistent value */
  inline Optional(): present(false), value() {}

  /** @brief Construct with an existing value
   * @param x Initial value
   */
  inline Optional(const T &x): present(true), value(x) {}

  /** @brief Test whether the value exists
   * @return @c true if the value exists and @c false otherwise
   */
  inline bool exists() const {
    return present;
  }

  /** @brief Return the (existing) value
   * @return The value
   * @exception std::logic_error if the value does not exist
   */
  inline operator T &() {
    if(!present)
      throw std::logic_error("retrieving an absent optional value");
    return value;
  }

  /** @brief Assign from an existing value
   * @param x New value */
  inline Optional &operator=(const T &x) {
    present = true;
    value = x;
    return *this;
  }

private:
  bool present;
  T value;
};

#endif /* OPTIONAL_H */
