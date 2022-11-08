/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrDirections.h"


namespace MusicFormats
{

// directions
//______________________________________________________________________________
string msrDirectionKindAsString (
  msrDirectionKind directionKind)
{
  string result;

  switch (directionKind) {
    case msrDirectionKind::kDirectionNone:
      result = "***kDirectionNone***";
      break;

    case msrDirectionKind::kDirectionUp:
      result = "kDirectionUp";
      break;
    case msrDirectionKind::kDirectionDown:
      result = "kDirectionDown";
      break;
    } // switch

  return result;
}

ostream& operator << (ostream& os, const msrDirectionKind& elt)
{
  os << msrDirectionKindAsString (elt);
  return os;
}


}

