/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrAlignments___
#define ___msrAlignments___

#include <string>
#include <ostream>

using namespace std;

namespace MusicFormats
{

// alignements
//______________________________________________________________________________
/*
  The justify entity is used to indicate left, center, or
  right justification. The default value varies for different
  elements. For elements where the justify attribute is present
  but the halign attribute is not, the justify attribute
  indicates horizontal alignment as well as justification.
*/

enum class msrJustifyKind {
  kJustifyNone,
  kJustifyLeft, kJustifyCenter, kJustifyRight
};

string msrJustifyKindAsString (
  msrJustifyKind justifyKind);

ostream& operator << (ostream& os, const msrJustifyKind& elt);

msrJustifyKind msrJustifyKindFromString (
  int           inputLineNumber,
  const string& justifyString);

enum class msrHorizontalAlignmentKind {
  kHorizontalAlignmentNone,
  kHorizontalAlignmentLeft, kHorizontalAlignmentCenter, kHorizontalAlignmentRight
};

string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind);

ostream& operator << (ostream& os, const msrHorizontalAlignmentKind& elt);

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  int           inputLineNumber,
  const string& horizontalAlignString);

enum class msrVerticalAlignmentKind {
  kVerticalAlignmentNone,
  kVerticalAlignmentTop, kVerticalAlignmentMiddle, kVerticalAlignmentBottom
};

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind);

ostream& operator << (ostream& os, const msrVerticalAlignmentKind& elt);

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  int           inputLineNumber,
  const string& verticalAlignmentString);


}

#endif
