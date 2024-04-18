/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrAccidentals___
#define ___msrAccidentals___

#include <string>
#include <ostream>


namespace MusicFormats
{

// accidentals
//______________________________________________________________________________
enum class msrAccidentalKind {
  kAccidentalNone,

  kAccidentalSharp, kAccidentalNatural,
  kAccidentalFlat, kAccidentalDoubleSharp,
  kAccidentalSharpSharp,
  kAccidentalFlatFlat, kAccidentalNaturalSharp,
  kAccidentalNaturalFlat, kAccidentalQuarterFlat,
  kAccidentalQuarterSharp,kAccidentalThreeQuartersFlat,
  kAccidentalThreeQuartersSharp,

  kAccidentalSharpDown, kAccidentalSharpUp,
  kAccidentalNaturalDown, kAccidentalNaturalUp,
  kAccidentalFlatDown, kAccidentalFlatUp,
  kAccidentalTripleSharp, kAccidentalTripleFlat,
  kAccidentalSlashQuarterSharp, kAccidentalSlashSharp,
  kAccidentalSlashFlat, kAccidentalDoubleSlashFlat,
  kAccidentalSharp_1, kAccidentalSharp_2,
  kAccidentalSharp_3,  kAccidentalSharp_5,
  kAccidentalFlat_1, kAccidentalFlat_2,
  kAccidentalFlat_3, kAccidentalFlat_4,

  kAccidentalSori, kAccidentalKoron, // persian music

  kAccidentalOther
};

std::string msrAccidentalKindAsString (
  msrAccidentalKind accidentalKind);

std::ostream& operator << (std::ostream& os, const msrAccidentalKind& elt);

std::string msrAccidentalKindAsMusicXMLString (
  msrAccidentalKind accidentalKind);

// editorial accidentals
//______________________________________________________________________________

enum class msrEditorialAccidentalKind {
  kEditorialAccidentalYes, kEditorialAccidentalNo
};

std::string msrEditorialAccidentalKindAsString (
  msrEditorialAccidentalKind noteEditorialAccidentalKind);

std::ostream& operator << (std::ostream& os, const msrEditorialAccidentalKind& elt);

// cautionary accidentals
//______________________________________________________________________________

enum class msrCautionaryAccidentalKind {
  kCautionaryAccidentalYes, kCautionaryAccidentalNo
};

std::string msrCautionaryAccidentalKindAsString (
  msrCautionaryAccidentalKind noteCautionaryAccidentalKind);

std::ostream& operator << (std::ostream& os, const msrCautionaryAccidentalKind& elt);


}


#endif // ___msrAccidentals___
