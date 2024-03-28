/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw()), set::precision(), ...

#include "mfIndentedTextOutput.h"

#include "msrLengths.h"


namespace MusicFormats
{

// length units
//______________________________________________________________________________

std::map<std::string, msrLengthUnitKind>
  gGlobalMsrLengthUnitKindsMap;

std::string msrLengthUnitKindAsString (
  msrLengthUnitKind lengthUnitKind)
{
  std::string result;

  // no CamelCase here, these strings are used in the command line options

  switch (lengthUnitKind) {
    case msrLengthUnitKind::kUnitInch:
      result = "kUnitInch";
      break;
    case msrLengthUnitKind::kUnitCentimeter:
      result = "kUnitCentimeter";
      break;
    case msrLengthUnitKind::kUnitMillimeter: // default value
      result = "kUnitMillimeter";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrLengthUnitKind& elt)
{
  os << msrLengthUnitKindAsString (elt);
  return os;
}

void initializeMsrLengthUnitKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalMsrLengthUnitKindsMap ["in"] = msrLengthUnitKind::kUnitInch;
  gGlobalMsrLengthUnitKindsMap ["cm"] = msrLengthUnitKind::kUnitCentimeter;
  gGlobalMsrLengthUnitKindsMap ["mm"] = msrLengthUnitKind::kUnitMillimeter;
}

std::string availableMsrLengthUnitKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t msrLengthUnitKindsMapSize =
    gGlobalMsrLengthUnitKindsMap.size ();

  if (msrLengthUnitKindsMapSize) {
    size_t nextToLast =
      msrLengthUnitKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msrLengthUnitKind>::const_iterator i =
        gGlobalMsrLengthUnitKindsMap.begin ();
      i != gGlobalMsrLengthUnitKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != msrLengthUnitKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

// lengths
//______________________________________________________________________________
S_msrLength msrLength::create (
  msrLengthUnitKind lengthUnitKind,
  float             lengthValue)
{
  msrLength * obj =
    new msrLength (
      lengthUnitKind,
      lengthValue);
  assert (obj != nullptr);
  return obj;
}

msrLength::msrLength (
  msrLengthUnitKind lengthUnitKind,
  float             lengthValue)
{
  fLengthUnitKind = lengthUnitKind;
  fLengthValue    = lengthValue;
}

msrLength::msrLength ()
{
  fLengthUnitKind = msrLengthUnitKind::kUnitMillimeter;
  fLengthValue    = 0.0;
}

msrLength::~msrLength ()
{}

void msrLength::convertToLengthUnit (
  msrLengthUnitKind lengthUnitKind)
{
  if (fLengthUnitKind != lengthUnitKind) {
    switch (lengthUnitKind) {
      case msrLengthUnitKind::kUnitInch:
        switch (fLengthUnitKind) {
          case msrLengthUnitKind::kUnitInch:
            break;
          case msrLengthUnitKind::kUnitCentimeter:
            fLengthValue /= 2.54f;
            break;
          case msrLengthUnitKind::kUnitMillimeter:
            fLengthValue /= 25.4f;
            break;
        } // switch
        break;

      case msrLengthUnitKind::kUnitCentimeter:
        switch (fLengthUnitKind) {
          case msrLengthUnitKind::kUnitInch:
            fLengthValue *= 2.54f;
            break;
          case msrLengthUnitKind::kUnitCentimeter:
            break;
          case msrLengthUnitKind::kUnitMillimeter:
            fLengthValue /= 10;
            break;
        } // switch
        break;

      case msrLengthUnitKind::kUnitMillimeter:
        switch (fLengthUnitKind) {
          case msrLengthUnitKind::kUnitInch:
            fLengthValue *= 25.4f;
            break;
          case msrLengthUnitKind::kUnitCentimeter:
            fLengthValue *= 10;
            break;
          case msrLengthUnitKind::kUnitMillimeter:
            break;
        } // switch
        break;
    } // switch

    fLengthUnitKind = lengthUnitKind;
  }
}

std::string msrLength::asString () const
{
  std::stringstream ss;

  ss <<
    "[Length " <<
    std::setprecision (4) <<
    fLengthValue <<
    ' ' <<
    msrLengthUnitKindAsString (fLengthUnitKind) <<
    ']';

  return ss.str ();
}

void msrLength::print (std::ostream& os) const
{
  os <<
    asString (); // JMI
};

std::ostream& operator << (std::ostream& os, const msrLength& elt)
{
  elt.print (os);
  return os;
}

std::ostream& operator << (std::ostream& os, const S_msrLength& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}

