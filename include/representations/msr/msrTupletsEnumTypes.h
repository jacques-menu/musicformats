/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTupletsEnumTypes___
#define ___msrTupletsEnumTypes___

#include <string>
#include <ostream>

#include "exports.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrTupletInKind {
  kTupletIn_UNKNOWN_,
  kTupletInMeasure,
  kTupletInTuplet
};

EXP std::string msrTupletInKindAsString (
  msrTupletInKind tupletKind);

std::ostream& operator << (std::ostream& os, const msrTupletInKind& elt);

enum class msrTupletTypeKind {
  kTupletTypeNone,
  kTupletTypeStart, kTupletTypeContinue, kTupletTypeStop
};

std::string msrTupletTypeKindAsString (
  msrTupletTypeKind tupletTypeKind);

std::ostream& operator << (std::ostream& os, const msrTupletTypeKind& elt);

enum class msrTupletBracketKind {
  kTupletBracketYes, kTupletBracketNo
};

std::string msrTupletBracketKindAsString (
  msrTupletBracketKind tupletBracketKind);

std::ostream& operator << (std::ostream& os, const msrTupletBracketKind& elt);

enum class msrTupletLineShapeKind {
  kTupletLineShapeStraight, kTupletLineShapeCurved
};

std::string msrTupletLineShapeKindAsString (
  msrTupletLineShapeKind tupletLineShapeKind);

std::ostream& operator << (std::ostream& os, const msrTupletLineShapeKind& elt);

enum class msrTupletShowNumberKind {
  kTupletShowNumberActual, kTupletShowNumberBoth, kTupletShowNumberNone
};

std::string msrTupletShowNumberKindAsString (
  msrTupletShowNumberKind tupletShowNumberKind);

std::ostream& operator << (std::ostream& os, const msrTupletShowNumberKind& elt);

enum class msrTupletShowTypeKind {
  kTupletShowTypeActual, kTupletShowTypeBoth, kTupletShowTypeNone
};

std::string msrTupletShowTypeKindAsString (
  msrTupletShowTypeKind tupletShowTypeKind);

std::ostream& operator << (std::ostream& os, const msrTupletShowTypeKind& elt);


}


#endif // ___msrTupletsEnumTypes___
