/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrLineTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind)
{
  string result;

  switch (lineTypeKind) {
    case msrLineTypeKind::kLineTypeSolid:
      result = "kLineTypeSolid";
      break;
    case msrLineTypeKind::kLineTypeDashed:
      result = "kLineTypeDashed";
      break;
    case msrLineTypeKind::kLineTypeDotted:
      result = "kLineTypeDotted";
      break;
    case msrLineTypeKind::kLineTypeWavy:
      result = "kLineTypeWavy";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrLineTypeKind& elt)
{
  os << msrLineTypeKindAsString (elt);
  return os;
}


}
