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

template<typename T>
class Optional {
public:
  inline Optional(): present(false), value() {}
  inline Optional(const T &x): present(true), value(x) {}
  inline bool exists() const { return present; }
  inline operator T &() {
    if(!present) throw std::logic_error("retrieving an absent optional value");
    return value;
  }
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

