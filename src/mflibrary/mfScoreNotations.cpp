/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "mfScoreNotations.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string msrScoreNotationKindAsString (
  msrScoreNotationKind lilypondScoreNotationKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrScoreNotationKind& elt)
{
  os << msrScoreNotationKindAsString (elt);
  return os;
}


}

