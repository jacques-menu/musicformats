/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLineTypes___
#define ___msrLineTypes___

#include <string>
#include <ostream>


using namespace std;

namespace MusicFormats
{

// line types
//______________________________________________________________________________
enum class msrLineTypeKind {
  kLineTypeSolid, kLineTypeDashed,
  kLineTypeDotted, kLineTypeWavy
};

string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind);

ostream& operator << (ostream& os, const msrLineTypeKind& elt);


}

#endif
