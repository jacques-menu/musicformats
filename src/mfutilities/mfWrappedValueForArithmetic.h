/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWrappedValueForArithmetic___
#define ___mfWrappedValueForArithmetic___

#include <string>
#include <ostream>
#include <sstream>

#include "exports.h"

#include "mfBool.h"
// #include "mfConstants.h"


namespace MusicFormats
{

//______________________________________________________________________________
// mfWrappedValueForArithmetic
//______________________________________________________________________________

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
class mfWrappedValueForArithmetic
{
  public:

    // constructors
    // ------------------------------------------------------

                          mfWrappedValueForArithmetic (T bareValue);

                          mfWrappedValueForArithmetic (const T& bareValue);

                          mfWrappedValueForArithmetic ();

    // destructor
    // ------------------------------------------------------

    virtual               ~mfWrappedValueForArithmetic ();

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

    // comparison operators, returning bare bool values for use by the STL
    // ------------------------------------------------------

    bool                  operator == (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherValue) const;

    bool                  operator != (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    bool                  operator < (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    bool                  operator <= (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    bool                  operator >= (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    bool                  operator > (
                            T otherBareValue) const;

    bool                  operator > (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    // arithmetic operators
    // ------------------------------------------------------

    mfWrappedValueForArithmetic
                          operator + (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    mfWrappedValueForArithmetic
                          operator - (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    // increment/decrement operators
    // ------------------------------------------------------

    mfWrappedValueForArithmetic
                          operator += (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    mfWrappedValueForArithmetic
                          operator -= (
                            const mfWrappedValueForArithmetic <
                              T,
                              printPrefix,
                              printPostfix,
                              neutralValue,
                              neutralValueString
                            >&
                              otherWrappedValueForArithmetic) const;

    mfWrappedValueForArithmetic
                          operator ++ (void);

    mfWrappedValueForArithmetic
                          operator -- (void);

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

// constructors
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueForArithmetic (T bareValue)
  : fBareValue (bareValue),
    fPrintPrefix (printPrefix)
{}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueForArithmetic (const T& bareValue)
  : fBareValue (bareValue),
    fPrintPrefix (printPrefix)
{}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::mfWrappedValueForArithmetic ()
  : fBareValue (neutralValue),
    fPrintPrefix (printPrefix)
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
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::~mfWrappedValueForArithmetic ()
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
mfWrappedValueForArithmetic <
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

// operator ==
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator == (
  const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return fBareValue == otherWrappedValue.fBareValue;
}

// operator !=
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator != (
  const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return fBareValue != otherWrappedValue.fBareValue;
}

// operator <
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator < (
  const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return fBareValue < otherWrappedValue.fBareValue;
}

// operator <=
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator <= (
  const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return fBareValue <= otherWrappedValue.fBareValue;
}

// operator >=
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator >= (
  const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return fBareValue >= otherWrappedValue.fBareValue;
}

// operator >
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator > (
  const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return fBareValue > otherWrappedValue.fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
bool mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator > (
  T otherBareValue) const
{
  return fBareValue > otherBareValue;
}


// arithmetic operators
// ------------------------------------------------------

// operator +
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator + (
    const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return mfWrappedValueForArithmetic (fBareValue + otherWrappedValue.fBareValue);
}

// operator -
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator - (
    const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return mfWrappedValueForArithmetic (fBareValue - otherWrappedValue.fBareValue);
}

// increment/decrement operators
// ------------------------------------------------------

// operator +=
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator += (
    const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return mfWrappedValueForArithmetic (fBareValue += otherWrappedValue.fBareValue);
}

// operator -=
template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator -= (
    const mfWrappedValueForArithmetic& otherWrappedValue) const
{
  return mfWrappedValueForArithmetic (fBareValue -= otherWrappedValue.fBareValue);
}

// increment/decrement operators
// ------------------------------------------------------

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator ++ (void)
{
  return ++fBareValue;
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>
mfWrappedValueForArithmetic <
  T,
  printPrefix,
   printPostfix,
  neutralValue,
  neutralValueString
>::operator -- (void)
{
  return --fBareValue;
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
std::string mfWrappedValueForArithmetic <
  T,
  printPrefix,
  printPostfix,
  neutralValue,
  neutralValueString
>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix << fBareValue << fPrintPostfix;

  return ss.str ();
}

template <
  typename           T,
  const std::string& printPrefix,
  const std::string& printPostfix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
void mfWrappedValueForArithmetic <
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
  std::ostream&                                                     os,
  const mfWrappedValueForArithmetic <
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


#endif // ___mfWrappedValueForArithmetic___
