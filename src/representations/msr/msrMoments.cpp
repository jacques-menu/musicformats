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

#include "msrMoments.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrMoment::msrMoment ()
{
  fWrittenPositionInMeseasure = K_WHOLE_NOTES_UNKNOWN_;
  fSoundingRelativeOffset     = K_WHOLE_NOTES_UNKNOWN_;
}

msrMoment::msrMoment (
  const msrWholeNotes& writtenPositionInMeseasure,
  const msrWholeNotes& soundingRelativeOffset)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = soundingRelativeOffset;
}

msrMoment::msrMoment (
  const msrWholeNotes& writtenPositionInMeseasure)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = msrWholeNotes (0, 1);
}

msrMoment::~msrMoment ()
{}

Bool msrMoment::operator== (const msrMoment& other) const
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

Bool msrMoment::operator< (const msrMoment& other) const
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

Bool msrMoment::operator> (const msrMoment& other) const
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

std::string msrMoment::asString () const
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

void msrMoment::print (std::ostream& os) const
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

std::ostream& operator << (std::ostream& os, const msrMoment& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
void testMsrMomentComparisons (std::ostream& os)
{
  msrMoment m0 (msrWholeNotes (3, 4));
  msrMoment m1 (msrWholeNotes (3, 4), msrWholeNotes (-1, 16));
  msrMoment m2 (msrWholeNotes (3, 4), msrWholeNotes (2, 16));

  os <<
    "m1: " << m1 << std::endl <<
    "m0: " << m0 << std::endl <<
    "m2: " << m2 << std::endl <<
    std::endl <<

    "m0 == m0: " << Bool (m0 == m0) << std::endl << // JMI v0.9.66
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
fWrittenPositionInMeseasure : [msrWholeNotes 3/4]
fSoundingRelativeOffset     : [msrWholeNotes -1/16]
]

m0: [Moment
fWrittenPositionInMeseasure : [msrWholeNotes 3/4]
fSoundingRelativeOffset     : [msrWholeNotes 0/1]
]

m2: [Moment
fWrittenPositionInMeseasure : [msrWholeNotes 3/4]
fSoundingRelativeOffset     : [msrWholeNotes 1/8]
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

