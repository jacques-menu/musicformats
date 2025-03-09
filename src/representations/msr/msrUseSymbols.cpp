/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "mfServices.h"

#include "msrUseSymbols.h"

#include "msrWae.h"


namespace MusicFormats
{

// placement
//______________________________________________________________________________
msrUseSymbolsKind msrUseSymbolsKindFromString (
  int                inputLineNumber,
  const std::string& useSymbolsString)
{
  msrUseSymbolsKind
    result =
      msrUseSymbolsKind::kUseSymbols_UNKNOWN_; // default value

  if      (useSymbolsString == "no")
    result = msrUseSymbolsKind::kUseSymbolsNo;
  else if (useSymbolsString == "yes")
    result = msrUseSymbolsKind::kUseSymbolsYes;
  else {
    if (useSymbolsString.size ()) {
      std::stringstream ss;

      ss <<
        "harmony use-symbols \"" << useSymbolsString <<
        "\" should be 'no' or 'yes'";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  return result;
}

std::string msrUseSymbolsKindAsString (
  msrUseSymbolsKind placementKind)
{
  std::string result;

  switch (placementKind) {
    case msrUseSymbolsKind::kUseSymbols_UNKNOWN_:
      result = "kUseSymbols_UNKNOWN_";
      break;
    case msrUseSymbolsKind::kUseSymbolsNo:
      result = "kUseSymbolsNo";
      break;
    case msrUseSymbolsKind::kUseSymbolsYes:
      result = "kUseSymbolsYes";
      break;
    default:
      ;
      // cannot occur since 'enum class' variables are initialized to 0
      // and the C++ compilers check for missing, unspecified switch values
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrUseSymbolsKind& elt)
{
  os << msrUseSymbolsKindAsString (elt);
  return os;
}


}

