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

#include "exports.h"

#include "mfBool.h"
#include "mfConstants.h"
#include "msrNotesDurations.h"
#include "msrMoments.h"


namespace MusicFormats
{

//______________________________________________________________________________
template <
  typename T,
  const std::string& printPrefix
>
class EXP mfValue
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfValue (T value);

    virtual               ~mfValue ();

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
                            const mfValue <T, printPrefix>&
                              otherValue) const;

    Bool                  operator != (
                            const mfValue <T, printPrefix>&
                              otherValue) const;

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

// constructor
template <
  typename T,
  const std::string& printPrefix
>
mfValue <T, printPrefix>::mfValue (T value)
  : fValue (value),
    fPrintPrefix (printPrefix)
{}

// destructor
template <
  typename T,
  const std::string& printPrefix
>
mfValue <T, printPrefix>::~mfValue ()
{}

// operator T ()
template <
  typename T,
  const std::string& printPrefix
>
mfValue <T, printPrefix>::operator T () const
{
  return fValue;
}

// comparison operators
template <
  typename T,
  const std::string& printPrefix
>
Bool mfValue <T, printPrefix>::operator == (
  const mfValue& otherInputLineNumber) const
{
  return fValue == otherInputLineNumber.fValue;
}

template <
  typename T,
  const std::string& printPrefix
>
Bool mfValue <T, printPrefix>::operator != (
  const mfValue& otherInputLineNumber) const
{
  return fValue != otherInputLineNumber.fValue;
}

// print
template <
  typename T,
  const std::string& printPrefix
>
std::string mfValue <T, printPrefix>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix << fValue;

  return ss.str ();
}

template <
  typename T,
  const std::string& printPrefix
>
void mfValue <T, printPrefix>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
template <
  typename T,
  const std::string& printPrefix
>
EXP std::ostream& operator << (
  std::ostream&                   os,
  const mfValue <T, printPrefix>& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
/*
A non-type template-parameter shall have one of the following (optionally cv-qualified) types:
  integral or enumeration type,
  pointer to object or pointer to function,
  lvalue reference to object or lvalue reference to function,
  pointer to member,
  std::nullptr_t.

template <std::string * temp> //pointer to object
void f()
{
   cout << *temp << endl;
}

template <std::string & temp> //reference to object
void g()
{
     cout << temp << endl;
     temp += "...appended some string";
}

std::string s; //must not be local as it must have external linkage!

int main() {
        s = "can assign values locally";
        f<&s>();
        g<s>();
        cout << s << endl;
        return 0;
}
*/

//______________________________________________________________________________
template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
class mfValueWithNeutralValue
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfValueWithNeutralValue ();

                          mfValueWithNeutralValue (T value);

    virtual               ~mfValueWithNeutralValue ();

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
                            const mfValueWithNeutralValue <T, printPrefix, neutralValue, neutralValueString>&
                              otherValue) const;

    Bool                  operator != (
                            const mfValueWithNeutralValue <T, printPrefix, neutralValue, neutralValueString>&
                              otherValue) const;

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    T                     fValue;
    std::string           fPrintPrefix;

    T                     fNeutralValue;
    std::string           fNeutralValueString;
};

// constructors
template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::mfValueWithNeutralValue ()
  : fValue (neutralValue),
      fPrintPrefix (printPrefix),
      fNeutralValue (neutralValue),
      fNeutralValueString (neutralValueString)
{}

template <
  typename T, const std::string& printPrefix,
  const T& neutralValue,
  const std::string& neutralValueString
>
mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::mfValueWithNeutralValue (T value)
  : fValue (value),
    fPrintPrefix (printPrefix),
    fNeutralValue (neutralValue),
    fNeutralValueString (neutralValueString)
{}

// destructor
template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::~mfValueWithNeutralValue ()
{}

// operator T ()
template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::operator T () const
{
  return fValue;
}

// comparison operators
template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
Bool mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::operator == (
  const mfValueWithNeutralValue& otherInputLineNumber) const
{
  return fValue == otherInputLineNumber.fValue;
}

template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
Bool mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::operator != (
  const mfValueWithNeutralValue& otherInputLineNumber) const
{
  return fValue != otherInputLineNumber.fValue;
}

// print
template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
std::string mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::asString () const
{
  std::stringstream ss;

  ss <<
    fPrintPrefix;

  if (fValue == fNeutralValue) {
    ss << fNeutralValueString;
  }
  else {
    ss << fValue;
  }

  return ss.str ();
}

template <
  typename T,
  const std::string& printPrefix,
  const T&           neutralValue,
  const std::string& neutralValueString
>
void mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
template <
  typename T, const std::string& printPrefix,
  const T& neutralValue,
  const std::string& neutralValueString
>
EXP std::ostream& operator << (
  std::ostream&                                                                      os,
  const mfValueWithNeutralValue <T, printPrefix,  neutralValue, neutralValueString>& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
template <
  typename T,
  const std::string& printSeparator
>
class mfRange
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mfRange (
                            T rangeStart,
                            T rangeEnd);

    virtual               ~mfRange ();

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
template <
  typename T,
  const std::string& printSeparator
>
mfRange <T, printSeparator>::mfRange (
  T rangeStart,
  T rangeEnd)
    : fRangeStart (rangeStart),
      fRangeEnd (rangeEnd),
      fPprintSeparator (printSeparator)
{}

// destructor
template <
  typename T,
  const std::string& printSeparator
>
mfRange <T, printSeparator>::~mfRange ()
{}

// print
template <
  typename T,
  const std::string& printSeparator
>
std::string mfRange <T, printSeparator>::asString () const
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
  typename T,
  const std::string& printSeparator
>
void mfRange <T, printSeparator>::print (std::ostream& os) const
{
  os << asString ();
}

// operator <<
template <
  typename T,
  const std::string& printSeparator
>
EXP std::ostream& operator << (
  std::ostream&                      os,
  const mfRange <T, printSeparator>& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
// input line numbers
typedef mfValueWithNeutralValue
  <int, kInputLineNumberPrefix, K_MF_INPUT_LINE_UNKNOWN_, kInputLineNumberNeutralValueString>
    mfInputLineNumber;

//______________________________________________________________________________
// input locations
typedef mfRange
  <mfInputLineNumber, kInputLocationRangePrefix>
    mfInputLocationRange;

//______________________________________________________________________________
// staff numbers
typedef mfValueWithNeutralValue
  <int, kStaffNumberPrefix, K_STAFF_NUMBER_UNKNOWN_, kInputLineNumberNeutralValueString>
   mfStaffNumber;

//______________________________________________________________________________
// voice numbers
typedef mfValueWithNeutralValue
  <int, kVoiceNumberPrefix, K_VOICE_NUMBER_UNKNOWN_, kVoiceNumberNeutralValueString>
    mfVoiceNumber;

//______________________________________________________________________________
// measure numbers
typedef mfValueWithNeutralValue
  <std::string, kMeasureNumberPrefix, K_MEASURE_NUMBER_UNKNOWN_, kMeasureNumberNeutralValueString>
    mfMeasureNumber;

//______________________________________________________________________________
// whole notes durations
typedef mfValueWithNeutralValue
  <msrWholeNotes, kWholeNotePrefix, K_WHOLE_NOTES_UNKNOWN_, kWholeNoteNeutralValueString>
    mfWohleNotes;

//______________________________________________________________________________
// moments
typedef mfValueWithNeutralValue
  <msrMoment, kMomentPrefix, K_MOMENT_UNKNOWN_, kMomentNeutralValueString>
    mfMoment;

//______________________________________________________________________________
// lyrics
typedef mfValueWithNeutralValue
  <std::string, kStanzaNumberPrefix, K_STANZA_NUMBER_UNKNOWN_, kStanzaNumberNeutralValueString>
    mfStanzaNumber;

//______________________________________________________________________________
void testWrapperTypes ();

}


#endif // ___mfWrapperTypes___
