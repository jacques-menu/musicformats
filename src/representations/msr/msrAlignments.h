/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrAlignments___
#define ___msrAlignments___

#include <string>
#include <ostream>

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

std::string msrJustifyKindAsString (
  msrJustifyKind justifyKind);

std::ostream& operator << (std::ostream& os, const msrJustifyKind& elt);

msrJustifyKind msrJustifyKindFromString (
  int           inputLineNumber,
  const std::string& justifyString);

enum class msrHorizontalAlignmentKind {
  kHorizontalAlignmentNone,
  kHorizontalAlignmentLeft, kHorizontalAlignmentCenter, kHorizontalAlignmentRight
};

std::string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind);

std::ostream& operator << (std::ostream& os, const msrHorizontalAlignmentKind& elt);

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  int           inputLineNumber,
  const std::string& horizontalAlignString);

enum class msrVerticalAlignmentKind {
  kVerticalAlignmentNone,
  kVerticalAlignmentTop, kVerticalAlignmentMiddle, kVerticalAlignmentBottom
};

std::string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind);

std::ostream& operator << (std::ostream& os, const msrVerticalAlignmentKind& elt);

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  int           inputLineNumber,
  const std::string& verticalAlignmentString);


}

#endif // ___msrAlignments___
