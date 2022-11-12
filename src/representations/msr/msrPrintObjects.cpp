/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "mfServiceRunData.h"

#include "msrPrintObjects.h"

#include "msrWae.h"


namespace MusicFormats
{

// print object
//______________________________________________________________________________
msrPrintObjectKind msrPrintObjectKindFromString (
  int           inputLineNumber,
  const std::string& printObjectString)
{
  msrPrintObjectKind
    result =
      msrPrintObjectKind::kPrintObjectNone; // default value JMI v0.9.65

//   gLogStream << // JMI TEST JMI v0.9.65
//     "printObjectString = " <<
//     printObjectString <<
//     std::endl;

  if      (printObjectString == "yes")
    result = msrPrintObjectKind::kPrintObjectYes;
  else if (printObjectString == "no")
    result = msrPrintObjectKind::kPrintObjectNo;
  else {
    if (printObjectString.size ()) {
      std::stringstream s;

      s <<
        "printObject \"" << printObjectString <<
        "\" should be 'yes' or 'no'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

std::string msrPrintObjectKindAsString (
  msrPrintObjectKind printObjectKind)
{
  std::string result;

  switch (printObjectKind) {
    case msrPrintObjectKind::kPrintObjectNone:
      result = "***kPrintObjectNone***";
      break;
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

