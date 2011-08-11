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
#ifndef CHARACTERS_H
#define CHARACTERS_H
/** @file src/Characters.hh
 * @brief Selected Unicode code points
 *
 * Unicode is assumed.  Only those code points that are actually used
 * are defined here.
 *
 * Note that elsewhere it is assumed that the current multibyte
 * encoding is ASCII-compatible.
 */

#define LINE_FEED 0x000a
#define CHARACTER_TABULATION 0x0009
#define BACKSPACE 0x0008
#define SPACE 0x0020
#define AMPERSAND 0x0026
#define LESS_THAN_SIGN 0x003C
#define TILDE 0x007e

#endif /* CHARACTERS_H */
