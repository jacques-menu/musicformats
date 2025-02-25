/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWrappedValue___
#define ___mfWrappedValue___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"


namespace MusicFormats
{

//______________________________________________________________________________
// mfWrappedValue
//______________________________________________________________________________

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
class EXP mfWrappedValue
{
  public:

    // constructor
    // ------------------------------------------------------

                          mfWrappedValue (T bareValue);

    // destructor
    // ------------------------------------------------------

    virtual               ~mfWrappedValue ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBareValue (T bareValue)
                              { fBareValue = bareValue; }

    T                     getBareValue () const
                              { return fBareValue; }

    std::string           getPrintPrefix () const
                              { return fPrintPrefix; }

    std::string           getPrintPostfix () const
                              { return fPrintPostfix; }

  public:

    // conversion  operators T ()
    // ------------------------------------------------------

    explicit operator     T () const;

    bool                  operator == (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherWrappedValue) const;

    // comparison operators, returning bare bool values for use by the STL
    // ------------------------------------------------------

    bool                  operator != (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherWrappedValue) const;

    bool                  operator < (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherWrappedValue) const;

    bool                  operator <= (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherWrappedValue) const;

    bool                  operator >= (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherWrappedValue) const;

    bool                  operator > (
                            const mfWrappedValue <
                              T,
                              printPrefix,
                              printPostfix
                            >&
                              otherWrappedValue) const;

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    T                     fBareValue;

    std::string           fPrintPrefix;
    std::string           fPrintPostfix;
};

// constructor
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
mfWrappedValue <T, printPrefix, printPostfix>::mfWrappedValue (T bareValue)
  : fBareValue (bareValue),
    fPrintPrefix (printPrefix)
{}

// destructor
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
mfWrappedValue <T, printPrefix, printPostfix>::~mfWrappedValue ()
{}

// conversion operator T ()
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
mfWrappedValue <T, printPrefix, printPostfix>::operator T () const
{
  return fBareValue;
}

// comparison operators
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator == (
  const mfWrappedValue& otherWrappedValue) const
{
  return fBareValue == otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator != (
  const mfWrappedValue& otherWrappedValue) const
{
  return fBareValue != otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator < (
  const mfWrappedValue& otherWrappedValue) const
{
  return fBareValue < otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator <= (
  const mfWrappedValue& otherWrappedValue) const
{
  return fBareValue <= otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator >= (
  const mfWrappedValue& otherWrappedValue) const
{
  return fBareValue >= otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
bool mfWrappedValue <T, printPrefix, printPostfix>::operator > (
  const mfWrappedValue& otherWrappedValue) const
{
  return fBareValue < otherWrappedValue.fBareValue;
}

// print
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
std::string mfWrappedValue <T, printPrefix, printPostfix>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix << fBareValue << fPrintPostfix;

  return ss.str ();
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
void mfWrappedValue <T, printPrefix, printPostfix>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix
>
std::ostream& operator << (
  std::ostream&                                        os,
  const mfWrappedValue <T, printPrefix, printPostfix>& elt)
{
  elt.print (os);
  return os;
}


}


#endif // ___mfWrappedValue___
