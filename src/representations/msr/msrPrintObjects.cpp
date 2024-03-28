/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "mfServices.h"

#include "msrPrintObjects.h"

#include "msrWae.h"


namespace MusicFormats
{

// print object
//______________________________________________________________________________
msrPrintObjectKind msrPrintObjectKindFromString (
  int                inputLineNumber,
  const std::string& printObjectString)
{
  msrPrintObjectKind
    result =
      msrPrintObjectKind::kPrintObjectYes; // default value

  if      (printObjectString == "yes")
    result = msrPrintObjectKind::kPrintObjectYes;
  else if (printObjectString == "no")
    result = msrPrintObjectKind::kPrintObjectNo;
  else {
    if (printObjectString.size ()) {
      std::stringstream ss;

      ss <<
        "printObject \"" << printObjectString <<
        "\" should be 'yes' or 'no'";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  return result;
}

std::string msrPrintObjectKindAsString (
  msrPrintObjectKind printObjectKind)
{
  std::string result;

  switch (printObjectKind) {
    case msrPrintObjectKind::kPrintObjectYes:
      result = "kPrintObjectYes";
      break;
    case msrPrintObjectKind::kPrintObjectNo:
      result = "kPrintObjectNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPrintObjectKind& elt)
{
  os << msrPrintObjectKindAsString (elt);
  return os;
}


}

