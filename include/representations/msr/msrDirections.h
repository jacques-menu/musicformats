/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrDirections___
#define ___msrDirections___

#include <string>
#include <ostream>


using namespace std;

namespace MusicFormats
{

// directions
//______________________________________________________________________________
enum class msrDirectionKind {
  kDirectionNone,
  kDirectionUp, kDirectionDown
};

string msrDirectionKindAsString (
  msrDirectionKind directionKind);

ostream& operator << (ostream& os, const msrDirectionKind& elt);


}

#endif
