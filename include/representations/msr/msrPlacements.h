/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPlacements___
#define ___msrPlacements___

#include <string>
#include <ostream>


namespace MusicFormats
{

// placements
//______________________________________________________________________________
enum class msrPlacementKind {
  kPlacement_UNKNOWN_,
  kPlacementAbove, kPlacementBelow
};

std::string msrPlacementKindAsString (
  msrPlacementKind placementKind);

std::ostream& operator << (std::ostream& os, const msrPlacementKind& elt);

msrPlacementKind msrPlacementKindFromString (
  int                inputLineNumber,
  const std::string& placementString);


}

#endif // ___msrPlacements___
