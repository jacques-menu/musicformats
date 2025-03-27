/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw()), set::precision(), ...

#include "mfConstants.h"
#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "msrMeasureConstants.h"

#include "mfMoments.h"


namespace MusicFormats
{

//______________________________________________________________________________
mfMoment::mfMoment ()
{
  fWrittenPositionInMeseasure = K_POSITION_IN_MEASURE_UNKNOWN_;
  fSoundingRelativeOffset = K_WHOLE_NOTES_UNKNOWN_;
}

mfMoment::mfMoment (
  const mfPositionInMeasure& writtenPositionInMeasure,
  const mfWholeNotes&        soundingRelativeOffset)
{
  fWrittenPositionInMeseasure = writtenPositionInMeasure;
  fSoundingRelativeOffset = soundingRelativeOffset;
}

mfMoment::mfMoment (
  const mfPositionInMeasure& writtenPositionInMeseasure)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset = K_WHOLE_NOTES_ZERO;
}

mfMoment::~mfMoment ()
{}

Bool mfMoment::operator== (const mfMoment& other) const
{
  Bool result;

  if (fWrittenPositionInMeseasure != other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else {
    result = fSoundingRelativeOffset == other.fSoundingRelativeOffset;
  }

  return result;
}

Bool mfMoment::operator< (const mfMoment& other) const
{
  Bool result;

  if (fWrittenPositionInMeseasure > other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else if (fWrittenPositionInMeseasure == other.fWrittenPositionInMeseasure) {
    result = fSoundingRelativeOffset < other.fSoundingRelativeOffset;
  }
  else {
    result = true;
  }

  return result;
}

Bool mfMoment::operator> (const mfMoment& other) const
{
  Bool result;

  if (fWrittenPositionInMeseasure < other.fWrittenPositionInMeseasure) {
    result = false;
  }
  else if (fWrittenPositionInMeseasure == other.fWrittenPositionInMeseasure) {
    result = fSoundingRelativeOffset > other.fSoundingRelativeOffset;
  }
  else {
    result = true;
  }

  return result;
}

std::string mfMoment::asString () const
{
  std::stringstream ss;

  ss <<
    "[Moment" <<
    " writtenPositionInMeseasure: " <<
    fWrittenPositionInMeseasure <<
    ", soundingRelativeOffset: " <<
    fSoundingRelativeOffset <<
    ']';

  return ss.str ();
}

void mfMoment::print (std::ostream& os) const
{
  os <<
    "[Moment" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fWrittenPositionInMeseasure" << ": " << fWrittenPositionInMeseasure <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSoundingRelativeOffset" << ": " << fSoundingRelativeOffset <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
};

std::ostream& operator << (std::ostream& os, const mfMoment& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
void testMsrMomentComparisons (std::ostream& os)
{
  mfMoment m0 (mfPositionInMeasure (3, 4));
  mfMoment m1 (mfPositionInMeasure (3, 4), mfWholeNotes (-1, 16));
  mfMoment m2 (mfPositionInMeasure (3, 4), mfWholeNotes (2, 16));

  os <<
    "m1: " << m1 << std::endl <<
    "m0: " << m0 << std::endl <<
    "m2: " << m2 << std::endl <<
    std::endl <<

    "m0 == m0: " << Bool (m0 == m0) << std::endl << // JMI 0.9.66
    "m0 == m0: " << mfBooleanAsString (m0 == m0) << std::endl <<
    "m1 == m1: " << mfBooleanAsString (m1 == m1) << std::endl <<
    "m2 == m2: " << mfBooleanAsString (m2 == m2) << std::endl <<
    std::endl <<

    "m0 != m0: " << mfBooleanAsString (m0 != m0) << std::endl <<
    "m1 != m1: " << mfBooleanAsString (m1 != m1) << std::endl <<
    "m2 != m2: " << mfBooleanAsString (m2 != m2) << std::endl <<
    std::endl <<

    "m0 < m0: " << mfBooleanAsString (m0 < m0) << std::endl <<
    "m1 < m1: " << mfBooleanAsString (m1 < m1) << std::endl <<
    "m2 < m2: " << mfBooleanAsString (m2 < m2) << std::endl <<
    std::endl <<

    "m0 <= m0: " << mfBooleanAsString (m0 <= m0) << std::endl <<
    "m1 <= m1: " << mfBooleanAsString (m1 <= m1) << std::endl <<
    "m2 <= m2: " << mfBooleanAsString (m2 <= m2) << std::endl <<
    std::endl <<

    "m0 >= m0: " << mfBooleanAsString (m0 >= m0) << std::endl <<
    "m1 >= m1: " << mfBooleanAsString (m1 >= m1) << std::endl <<
    "m2 >= m2: " << mfBooleanAsString (m2 >= m2) << std::endl <<
    std::endl <<

    "m0 > m0: " << mfBooleanAsString (m0 > m0) << std::endl <<
    "m1 > m1: " << mfBooleanAsString (m1 > m1) << std::endl <<
    "m2 > m2: " << mfBooleanAsString (m2 > m2) << std::endl <<
    std::endl <<
    std::endl <<

    "m1 == m0: " << mfBooleanAsString (m1 == m0) << std::endl <<
    "m1 == m1: " << mfBooleanAsString (m1 == m1) << std::endl <<
    "m1 == m2: " << mfBooleanAsString (m1 == m2) << std::endl <<
    std::endl <<

    "m1 != m0: " << mfBooleanAsString (m1 != m0) << std::endl <<
    "m1 != m1: " << mfBooleanAsString (m1 != m1) << std::endl <<
    "m1 != m2: " << mfBooleanAsString (m1 != m2) << std::endl <<
    std::endl <<

    "m1 < m0: " << mfBooleanAsString (m1 < m0) << std::endl <<
    "m1 < m1: " << mfBooleanAsString (m1 < m1) << std::endl <<
    "m1 < m2: " << mfBooleanAsString (m1 < m2) << std::endl <<
    std::endl <<

    "m1 <= m0: " << mfBooleanAsString (m1 <= m0) << std::endl <<
    "m1 <= m1: " << mfBooleanAsString (m1 <= m1) << std::endl <<
    "m1 <= m2: " << mfBooleanAsString (m1 <= m2) << std::endl <<
    std::endl <<

    "m1 >= m0: " << mfBooleanAsString (m1 >= m0) << std::endl <<
    "m1 >= m1: " << mfBooleanAsString (m1 >= m1) << std::endl <<
    "m1 >= m2: " << mfBooleanAsString (m1 >= m2) << std::endl <<
    std::endl <<

    "m1 > m0: " << mfBooleanAsString (m1 > m0) << std::endl <<
    "m1 > m1: " << mfBooleanAsString (m1 > m1) << std::endl <<
    "m1 > m2: " << mfBooleanAsString (m1 > m2) << std::endl <<
    std::endl <<

    std::endl;

/* output: JMI

m1: [Moment
fWrittenPositionInMeseasure : [mfWholeNotes 3/4]
fSoundingRelativeOffset     : [mfWholeNotes -1/16]
]

m0: [Moment
fWrittenPositionInMeseasure : [mfWholeNotes 3/4]
fSoundingRelativeOffset     : [mfWholeNotes 0/1]
]

m2: [Moment
fWrittenPositionInMeseasure : [mfWholeNotes 3/4]
fSoundingRelativeOffset     : [mfWholeNotes 1/8]
]


m0 == m0: true
m0 == m0: true
m1 == m1: true
m2 == m2: true

m0 != m0: false
m1 != m1: false
m2 != m2: false

m0 < m0: false
m1 < m1: false
m2 < m2: false

m0 <= m0: true
m1 <= m1: true
m2 <= m2: true

m0 >= m0: true
m1 >= m1: true
m2 >= m2: true

m0 > m0: false
m1 > m1: false
m2 > m2: false


m1 == m0: false
m1 == m1: true
m1 == m2: false

m1 != m0: true
m1 != m1: false
m1 != m2: true

m1 < m0: true
m1 < m1: false
m1 < m2: true

m1 <= m0: true
m1 <= m1: true
m1 <= m2: true

m1 >= m0: false
m1 >= m1: true
m1 >= m2: false

m1 > m0: false
m1 > m1: false
m1 > m2: false

*/
}


}

