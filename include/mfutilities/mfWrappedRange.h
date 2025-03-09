/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWrappedRange___
#define ___mfWrappedRange___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"


namespace MusicFormats
{

// mfWrappedRange
//______________________________________________________________________________

template <
  typename           T,
  const std::string& printSeparator
>
class EXP mfWrappedRange
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfWrappedRange (
                            T rangeStart,
                            T rangeEnd);

    virtual               ~mfWrappedRange ();

  public:

    // set and get
    // ------------------------------------------------------

    T                     getRangeStart () const
                              { return fRangeStart; }

    T                     getRangeEnd () const
                              { return fRangeStart; }

    std::string           getPprintSeparator () const
                              { return fPprintSeparator; }

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    T                     fRangeStart;
    T                     fRangeEnd;

    std::string           fPprintSeparator;
};

// constructor
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printSeparator
>
mfWrappedRange <T, printSeparator>::mfWrappedRange (
  T rangeStart,
  T rangeEnd)
    : fRangeStart (rangeStart),
      fRangeEnd (rangeEnd),
      fPprintSeparator (printSeparator)
{}

// destructor
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printSeparator
>
mfWrappedRange <T, printSeparator>::~mfWrappedRange ()
{}

// print
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printSeparator
>
std::string mfWrappedRange <T, printSeparator>::asString () const
{
  std::stringstream ss;

  if (fRangeStart == fRangeEnd) {
    ss <<
      fRangeStart;
  }
  else {
    ss <<
      fRangeStart << fPprintSeparator << fRangeEnd;
  }

  return ss.str ();
}

template <
  typename           T,
  const std::string& printSeparator
>
void mfWrappedRange <T, printSeparator>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printSeparator
>
std::ostream& operator << (
  std::ostream&                             os,
  const mfWrappedRange <T, printSeparator>& elt)
{
  elt.print (os);
  return os;
}


}


#endif // ___mfWrappedRange___
