/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw()), set::precision(), ...

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "msrMoments.h"


namespace MusicFormats
{

// moments
//______________________________________________________________________________
// constants
const Rational  msrMoment::K_NO_POSITION (-987, 1);
const msrMoment msrMoment::K_NO_MOMENT   (K_NO_POSITION, K_NO_POSITION);

msrMoment::msrMoment ()
{
  fWrittenPositionInMeseasure = Rational (-1, 1);
  fSoundingRelativeOffset     = Rational (-1, 1);
}

msrMoment::msrMoment (
  const Rational& writtenPositionInMeseasure,
  const Rational& soundingRelativeOffset)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = soundingRelativeOffset;
}

msrMoment::msrMoment (
  const Rational& writtenPositionInMeseasure)
{
  fWrittenPositionInMeseasure = writtenPositionInMeseasure;
  fSoundingRelativeOffset     = Rational (0, 1);
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

string msrMoment::asString () const
{
  stringstream s;

  s <<
    "[Moment" <<
    " writtenPositionInMeseasure: " <<
    fWrittenPositionInMeseasure <<
    ", soundingRelativeOffset: " <<
    fSoundingRelativeOffset <<
    ']';

  return s.str ();
}

void msrMoment::print (ostream& os) const
{
  os <<
    "[Moment" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "fWrittenPositionInMeseasure" << " : " << fWrittenPositionInMeseasure <<
    endl <<
    setw (fieldWidth) <<
    "fSoundingRelativeOffset" << " : " << fSoundingRelativeOffset <<
    endl;

  --gIndenter;

  os << ']' << endl;
};

ostream& operator << (ostream& os, const msrMoment& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
void testMsrMomentComparisons (ostream& os)
{
  msrMoment m0 (Rational (3, 4));
  msrMoment m1 (Rational (3, 4), Rational (-1, 16));
  msrMoment m2 (Rational (3, 4), Rational (2, 16));

  os <<
    "m1: " << m1 << endl <<
    "m0: " << m0 << endl <<
    "m2: " << m2 << endl <<
    endl <<

    "m0 == m0: " << Bool (m0 == m0) << endl << // JMI v0.9.66
    "m0 == m0: " << mfBooleanAsString (m0 == m0) << endl <<
    "m1 == m1: " << mfBooleanAsString (m1 == m1) << endl <<
    "m2 == m2: " << mfBooleanAsString (m2 == m2) << endl <<
    endl <<

    "m0 != m0: " << mfBooleanAsString (m0 != m0) << endl <<
    "m1 != m1: " << mfBooleanAsString (m1 != m1) << endl <<
    "m2 != m2: " << mfBooleanAsString (m2 != m2) << endl <<
    endl <<

    "m0 < m0: " << mfBooleanAsString (m0 < m0) << endl <<
    "m1 < m1: " << mfBooleanAsString (m1 < m1) << endl <<
    "m2 < m2: " << mfBooleanAsString (m2 < m2) << endl <<
    endl <<

    "m0 <= m0: " << mfBooleanAsString (m0 <= m0) << endl <<
    "m1 <= m1: " << mfBooleanAsString (m1 <= m1) << endl <<
    "m2 <= m2: " << mfBooleanAsString (m2 <= m2) << endl <<
    endl <<

    "m0 >= m0: " << mfBooleanAsString (m0 >= m0) << endl <<
    "m1 >= m1: " << mfBooleanAsString (m1 >= m1) << endl <<
    "m2 >= m2: " << mfBooleanAsString (m2 >= m2) << endl <<
    endl <<

    "m0 > m0: " << mfBooleanAsString (m0 > m0) << endl <<
    "m1 > m1: " << mfBooleanAsString (m1 > m1) << endl <<
    "m2 > m2: " << mfBooleanAsString (m2 > m2) << endl <<
    endl <<
    endl <<

    "m1 == m0: " << mfBooleanAsString (m1 == m0) << endl <<
    "m1 == m1: " << mfBooleanAsString (m1 == m1) << endl <<
    "m1 == m2: " << mfBooleanAsString (m1 == m2) << endl <<
    endl <<

    "m1 != m0: " << mfBooleanAsString (m1 != m0) << endl <<
    "m1 != m1: " << mfBooleanAsString (m1 != m1) << endl <<
    "m1 != m2: " << mfBooleanAsString (m1 != m2) << endl <<
    endl <<

    "m1 < m0: " << mfBooleanAsString (m1 < m0) << endl <<
    "m1 < m1: " << mfBooleanAsString (m1 < m1) << endl <<
    "m1 < m2: " << mfBooleanAsString (m1 < m2) << endl <<
    endl <<

    "m1 <= m0: " << mfBooleanAsString (m1 <= m0) << endl <<
    "m1 <= m1: " << mfBooleanAsString (m1 <= m1) << endl <<
    "m1 <= m2: " << mfBooleanAsString (m1 <= m2) << endl <<
    endl <<

    "m1 >= m0: " << mfBooleanAsString (m1 >= m0) << endl <<
    "m1 >= m1: " << mfBooleanAsString (m1 >= m1) << endl <<
    "m1 >= m2: " << mfBooleanAsString (m1 >= m2) << endl <<
    endl <<

    "m1 > m0: " << mfBooleanAsString (m1 > m0) << endl <<
    "m1 > m1: " << mfBooleanAsString (m1 > m1) << endl <<
    "m1 > m2: " << mfBooleanAsString (m1 > m2) << endl <<
    endl <<

    endl;

/* output: JMI

m1: [Moment
fWrittenPositionInMeseasure : [Rational 3/4]
fSoundingRelativeOffset     : [Rational -1/16]
]

m0: [Moment
fWrittenPositionInMeseasure : [Rational 3/4]
fSoundingRelativeOffset     : [Rational 0/1]
]

m2: [Moment
fWrittenPositionInMeseasure : [Rational 3/4]
fSoundingRelativeOffset     : [Rational 1/8]
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

