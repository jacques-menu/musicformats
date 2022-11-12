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

#include "msrPlacements.h"

#include "msrWae.h"


namespace MusicFormats
{

// placement
//______________________________________________________________________________
msrPlacementKind msrPlacementKindFromString (
  int           inputLineNumber,
  const std::string& placementString)
{
  msrPlacementKind result = msrPlacementKind::kPlacement_NO_; // default value

  if      (placementString == "above")
    result = msrPlacementKind::kPlacementAbove;
  else if (placementString == "below")
    result = msrPlacementKind::kPlacementBelow;
  else {
    if (placementString.size ()) {
      std::stringstream s;

      s <<
        "placement \"" << placementString <<
        "\" should be 'above' or 'below'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

std::string msrPlacementKindAsString (
  msrPlacementKind placementKind)
{
  std::string result;

  switch (placementKind) {
    case msrPlacementKind::kPlacement_NO_:
      result = "kPlacement_NO_";
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

