/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfRational___
#define ___mfRational___

#include <string>
#include <ostream>

namespace MusicFormats
{

// score notation
//______________________________________________________________________________
enum class msrScoreNotationKind {
  kScoreNotationWestern,
  kScoreNotationJianpu,
  kScoreNotationDiatonicAccordion
};

std::string msrScoreNotationKindAsString (
  msrScoreNotationKind lilypondScoreNotationKind);

std::ostream& operator << (std::ostream& os, const msrScoreNotationKind& elt);


}


#endif
