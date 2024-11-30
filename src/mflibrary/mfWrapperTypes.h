/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfWrapperTypes___
#define ___mfWrapperTypes___

#include <string>
#include <ostream>
#include <sstream>

#include "mfBool.h"

#include "exports.h"


namespace MusicFormats
{

//______________________________________________________________________________
template <typename T, const std::string& printPrefix>
class EXP mfValueWrapper
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfValueWrapper (T value);

    virtual               ~mfValueWrapper ();

  public:

    // set and get
    // ------------------------------------------------------

    T                     getValue () const
                              { return fValue; }

    std::string           getPrintPrefix () const
                              { return fPrintPrefix; }

  public:

    // operators
    // ------------------------------------------------------

    explicit operator     T () const;

    Bool                  operator == (
                            const mfValueWrapper<T, printPrefix>& otherValueWrapper) const;

    Bool                  operator != (
                            const mfValueWrapper<T, printPrefix>& otherValueWrapper) const;

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    T                     fValue;
    std::string           fPrintPrefix;
};

template <typename T, const std::string& printPrefix>
EXP std::ostream& operator << (std::ostream& os, const mfValueWrapper<T, printPrefix>& elt);

template <typename T, const std::string& printPrefix>
mfValueWrapper<T, printPrefix>::mfValueWrapper (T value)
  : fValue (value),
    fPrintPrefix (printPrefix)
{}

template <typename T, const std::string& printPrefix>
mfValueWrapper<T, printPrefix>::~mfValueWrapper ()
{}

template <typename T, const std::string& printPrefix>
mfValueWrapper<T, printPrefix>::operator T () const
{
  return fValue;
}

template <typename T, const std::string& printPrefix>
Bool mfValueWrapper<T, printPrefix>::operator == (
  const mfValueWrapper& otherInputLineNumber) const
{
  return fValue == otherInputLineNumber.fValue;
}

template <typename T, const std::string& printPrefix>
Bool mfValueWrapper<T, printPrefix>::operator != (
  const mfValueWrapper& otherInputLineNumber) const
{
  return fValue != otherInputLineNumber.fValue;
}

template <typename T, const std::string& printPrefix>
std::string mfValueWrapper<T, printPrefix>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix << fValue;

  return ss.str ();
}

template <typename T, const std::string& printPrefix>
void mfValueWrapper<T, printPrefix>::print (std::ostream& os) const
{
  os << asString ();
}

template <typename T, const std::string& printPrefix>
EXP std::ostream& operator << (std::ostream& os, const mfValueWrapper<T, printPrefix>& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
template <typename T, const std::string& printSeparator>
class EXP mfRangeWrapper
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfRangeWrapper (
                            T rangeStart,
                            T rangeEnd);

    virtual               ~mfRangeWrapper ();

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

template <typename T, const std::string& printSeparator>
EXP std::ostream& operator << (std::ostream& os, const mfRangeWrapper<T, printSeparator>& elt);


template <typename T, const std::string& printSeparator>
mfRangeWrapper<T, printSeparator>::mfRangeWrapper (
  T rangeStart,
  T rangeEnd)
    : fRangeStart (rangeStart),
      fRangeEnd (rangeEnd),
      fPprintSeparator (printSeparator)
{}

template <typename T, const std::string& printSeparator>
mfRangeWrapper<T, printSeparator>::~mfRangeWrapper ()
{}

template <typename T, const std::string& printSeparator>
std::string mfRangeWrapper<T, printSeparator>::asString () const
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

template <typename T, const std::string& printSeparator>
void mfRangeWrapper<T, printSeparator>::print (std::ostream& os) const
{
  os << asString ();
}

template <typename T, const std::string& printSeparator>
EXP std::ostream& operator << (std::ostream& os, const mfRangeWrapper<T, printSeparator>& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
const std::string kInputLineNumberPrefix ("L_");
typedef mfValueWrapper<int, kInputLineNumberPrefix> mfInputLineNumber;

const std::string kStaffNumberPrefix ("S_");
typedef mfValueWrapper<int, kStaffNumberPrefix> mfStaffNumber;

const std::string kVoiceNumberPrefix ("V_");
typedef mfValueWrapper<int, kVoiceNumberPrefix> mfVoiceNumber;

const std::string kInputLocationRangePrefix ("..");
typedef mfRangeWrapper<mfInputLineNumber, kInputLocationRangePrefix> mfInputLocationRange;



//                           mfInputLocation (
//                             mfInputLineNumber inputStartLineNumber,
//                             mfInputLineNumber inputEndLineNumber);


// class EXP mfInputLocation
// {
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           mfInputLocation (
//                             mfInputLineNumber inputStartLineNumber,
//                             mfInputLineNumber inputEndLineNumber);
//
//     virtual               ~mfInputLocation ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     mfInputLineNumber     getInputStartLineNumber () const
//                               { return fInputStartLineNumber; }
//
//     mfInputLineNumber     getInputEndLineNumber () const
//                               { return fInputEndLineNumber; }
//
//   public:
//
//     // print
//     // ------------------------------------------------------
//
//     std::string           asString () const;
//
//     void                  print (std::ostream& os) const;
//
//   private:
//
//     mfInputLineNumber     fInputStartLineNumber;
//     mfInputLineNumber     fInputEndLineNumber;
// };
//
// EXP std::ostream& operator << (std::ostream& os, const mfInputLocation& elt);
//
// //______________________________________________________________________________
// mfInputLocation::mfInputLocation (
//   mfInputLineNumber inputStartLineNumber,
//   mfInputLineNumber inputEndLineNumber)
//     : fInputStartLineNumber (
//         inputStartLineNumber),
//       fInputEndLineNumber (
//         inputEndLineNumber)
// {}
//
// mfInputLocation::~mfInputLocation ()
// {}
//
// std::string mfInputLocation::asString () const
// {
//   std::stringstream ss;
//
//   mfInputLineNumber
//     startInputLineNumber = fInputStartLineNumber.getValue (),
//     endInputLineNumber = fInputEndLineNumber.getValue ();
//
//   if (startInputLineNumber == endInputLineNumber) {
//     ss << "line " << startInputLineNumber;
//   }
//   else {
//     ss << "lines " << startInputLineNumber << ".." << endInputLineNumber;
//   }
//
//   return ss.str ();
// }
//
// void mfInputLocation::print (std::ostream& os) const
// {
//   os << asString ();
// }
//
// EXP std::ostream& operator << (std::ostream& os, const mfInputLocation& elt)
// {
//   elt.print (os);
//   return os;
// }

//______________________________________________________________________________
// class EXP mfInputLineNumber
// {
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           mfInputLineNumber (int value);
//
//     virtual               ~mfInputLineNumber ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     int                   getValue () const
//                               { return fValue; }
//
//   public:
//
//     // operators
//     // ------------------------------------------------------
//
//     explicit operator     int () const;
//
//     Bool                  operator == (
//                             const mfInputLineNumber& otherInputLineNumber) const;
//
//   private:
//
//     int                   fValue;
// };
//
// EXP std::ostream& operator << (std::ostream& os, const mfInputLineNumber& elt);

// //______________________________________________________________________________
// class EXP mfStaffNumber
// {
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           mfStaffNumber (int value);
//
//     virtual               ~mfStaffNumber ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     int                   getValue () const
//                               { return fValue; }
//
//   public:
//
//     // operators
//     // ------------------------------------------------------
//
//     explicit operator     int () const;
//
//     Bool                  operator == (
//                             const mfStaffNumber& otherStaffNumber) const;
//
//   private:
//
//     int                   fValue;
// };
//
// EXP std::ostream& operator << (std::ostream& os, const mfStaffNumber& elt);
//
// //______________________________________________________________________________
// class EXP mfVoiceNumber
// {
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           mfVoiceNumber (int value);
//
//     virtual               ~mfVoiceNumber ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     int                   getValue () const
//                               { return fValue; }
//
//   public:
//
//     // operators
//     // ------------------------------------------------------
//
//     explicit operator     int () const;
//
//     Bool                  operator == (
//                             const mfVoiceNumber& otherVoiceNumber) const;
//
//   private:
//
//     int                   fValue;
// };
//
// EXP std::ostream& operator << (std::ostream& os, const mfVoiceNumber& elt);
//
// //______________________________________________________________________________
// class EXP mfMeasureNumber
// {
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           mfMeasureNumber (const std::string& value);
//
//     virtual               ~mfMeasureNumber ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     std::string           getValue () const
//                               { return fValue; }
//   public:
//
//     // operators
//     // ------------------------------------------------------
//
//     explicit operator     std::string () const;
//
//     Bool                  operator == (
//                             const mfMeasureNumber& otherMeasureNumber) const;
//
//   private:
//
//     std::string           fValue;
// };
//
// EXP std::ostream& operator << (std::ostream& os, const mfMeasureNumber& elt);





}


#endif // ___mfWrapperTypes___
