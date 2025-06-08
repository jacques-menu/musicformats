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

#include "msrPlacements.h"

#include "msrWae.h"


namespace MusicFormats
{

// placement
//______________________________________________________________________________
msrPlacementKind msrPlacementKindFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& placementString)
{
  msrPlacementKind result = msrPlacementKind::kPlacement_UNKNOWN_; // default value

  if      (placementString == "above")
    result = msrPlacementKind::kPlacementAbove;
  else if (placementString == "below")
    result = msrPlacementKind::kPlacementBelow;
  else {
    if (placementString.size ()) {
      std::stringstream ss;

      ss <<
        "placement \"" << placementString <<
        "\" should be 'above' or 'below'";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
  }

  return result;
}

std::string msrPlacementKindAsString (
  msrPlacementKind placementKind)
{
  std::string result;

  switch (placementKind) {
    case msrPlacementKind::kPlacement_UNKNOWN_:
      result = "kPlacement_UNKNOWN_";
      break;
    case msrPlacementKind::kPlacementAbove:
      result = "kPlacementAbove";
      break;
    case msrPlacementKind::kPlacementBelow:
      result = "kPlacementBelow";
      break;
    default:
      ;
      // cannot occur since 'enum class' variables are initialized to 0
      // and the C++ compilers check for missing, unspecified switch values
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPlacementKind& elt)
{
  os << msrPlacementKindAsString (elt);
  return os;
}


}

