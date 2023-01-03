/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "mfScoreNotations.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrScoreNotationKindAsString (
  msrScoreNotationKind lilypondScoreNotationKind)
{
  std::string result;

  switch (lilypondScoreNotationKind) {
    case msrScoreNotationKind::kScoreNotationWestern:
      result = "kScoreNotationWestern";
    case msrScoreNotationKind::kScoreNotationJianpu:
      result = "kScoreNotationWestern";
    case msrScoreNotationKind::kScoreNotationDiatonicAccordion:
      result = "kScoreNotationDiatonicAccordion";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrScoreNotationKind& elt)
{
  os << msrScoreNotationKindAsString (elt);
  return os;
}


}

