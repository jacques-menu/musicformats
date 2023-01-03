/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrLineTypes___
#define ___msrLineTypes___

#include <string>
#include <ostream>


namespace MusicFormats
{

// line types
//______________________________________________________________________________
enum class msrLineTypeKind {
  kLineTypeSolid, kLineTypeDashed,
  kLineTypeDotted, kLineTypeWavy
};

std::string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind);

std::ostream& operator << (std::ostream& os, const msrLineTypeKind& elt);


}

#endif
