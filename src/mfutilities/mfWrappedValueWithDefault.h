/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWrappedValueWithDefault___
#define ___mfWrappedValueWithDefault___

#include <string>
#include <ostream>

#include "exports.h"

#include "mfBool.h"


namespace MusicFormats
{

//______________________________________________________________________________
// mfWrappedValueWithDefault
//______________________________________________________________________________

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
class EXP mfWrappedValueWithDefault
{
  public:

    // constructors
    // ------------------------------------------------------

                          mfWrappedValueWithDefault (T bareValue);

                          mfWrappedValueWithDefault (const T& bareValue);

                          mfWrappedValueWithDefault ();

    // destructor
    // ------------------------------------------------------

    virtual               ~mfWrappedValueWithDefault ();

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

    // operators
    // ------------------------------------------------------

    // conversion
    // ------------------------------------------------------

    explicit operator     T () const;

    // comparisons, returning bare bool values for use by the STL
    // ------------------------------------------------------

    bool                  operator == (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator != (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator < (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator <= (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator >= (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator > (
                            const mfWrappedValueWithDefault <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

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

    T                     fDefaultValue;
    std::string           fDefaultValueString;
};

// constructors
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueWithDefault (T bareValue)
  : fBareValue (bareValue),
    fPrintPrefix (printPrefix),
    fDefaultValue (neutralValue),
    fDefaultValueString (neutralValueString)
{}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueWithDefault (const T& bareValue)
  : fBareValue (bareValue),
    fPrintPrefix (printPrefix),
    fDefaultValue (neutralValue),
    fDefaultValueString (neutralValueString)
{}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueWithDefault ()
  : fBareValue (neutralValue),
    fPrintPrefix (printPrefix),
    fDefaultValue (neutralValue),
    fDefaultValueString (neutralValueString)
{}

// destructor
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::~mfWrappedValueWithDefault ()
{}

// conversion operator T ()
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator T () const
{
  return fBareValue;
}

// comparison operators
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator == (
  const mfWrappedValueWithDefault& otherWrappedValue) const
{
  return fBareValue == otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator != (
  const mfWrappedValueWithDefault& otherWrappedValue) const
{
  return fBareValue != otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::operator < (
  const mfWrappedValueWithDefault& otherWrappedValue) const
{
  return fBareValue < otherWrappedValue.fBareValue;
}

// print
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
std::string mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix;

  if (fBareValue == fDefaultValue) {
    ss << fDefaultValueString;
  }
  else {
    ss << fBareValue;
  }

  return ss.str ();
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
void mfWrappedValueWithDefault <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
std::ostream& operator << (
  std::ostream& os,
  const mfWrappedValueWithDefault <
    T,
    printPrefix,
    printPostfix,
    neutralValue,
    neutralValueString
  >& elt)
{
  elt.print (os);
  return os;
}


}


#endif // ___mfWrappedValueWithDefault___
