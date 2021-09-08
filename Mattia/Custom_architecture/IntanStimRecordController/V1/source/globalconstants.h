//  ------------------------------------------------------------------------
//
//  This file is part of the Intan Technologies RHS2000 Interface
//  Version 1.01
//  Copyright (C) 2013-2017 Intan Technologies
//
//  ------------------------------------------------------------------------
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

// Trigonometric constants
const double PI = 3.14159265359;
const double TWO_PI = 6.28318530718;
const double DEGREES_TO_RADIANS = 0.0174532925199;
const double RADIANS_TO_DEGREES = 57.2957795132;

// Special Unicode characters, as QString data type
#define QSTRING_MU_SYMBOL  ((QString)((QChar)0x03bc))
#define QSTRING_OMEGA_SYMBOL  ((QString)((QChar)0x03a9))
#define QSTRING_ANGLE_SYMBOL  ((QString)((QChar)0x2220))
#define QSTRING_DEGREE_SYMBOL  ((QString)((QChar)0x00b0))
#define QSTRING_PLUSMINUS_SYMBOL  ((QString)((QChar)0x00b1))

// Saved data file constants
#define DATA_FILE_MAGIC_NUMBER  0xd69127ac
#define DATA_FILE_MAIN_VERSION_NUMBER  1
#define DATA_FILE_SECONDARY_VERSION_NUMBER 0

// Saved settings file constants
#define SETTINGS_FILE_MAGIC_NUMBER  0x14cd21a3
#define SETTINGS_FILE_MAIN_VERSION_NUMBER  1
#define SETTINGS_FILE_SECONDARY_VERSION_NUMBER  0

// RHS2116 chip ID numbers from ROM register 255
#define CHIP_ID_RHS2116  32

// Save File Format Enumeration
enum SaveFormat {
    SaveFormatIntan,
    SaveFormatFilePerSignalType,
    SaveFormatFilePerChannel
};

#endif // GLOBALCONSTANTS_H
