/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrAccidentals___
#define ___msrAccidentals___

#include <string>
#include <ostream>


using namespace std;

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

string msrAccidentalKindAsString (
  msrAccidentalKind accidentalKind);

ostream& operator << (ostream& os, const msrAccidentalKind& elt);

string msrAccidentalKindAsMusicXMLString (
  msrAccidentalKind accidentalKind);

// editorial accidentals
//______________________________________________________________________________

enum class msrEditorialAccidentalKind {
  kEditorialAccidentalYes, kEditorialAccidentalNo
};

string msrEditorialAccidentalKindAsString (
  msrEditorialAccidentalKind noteEditorialAccidentalKind);

ostream& operator << (ostream& os, const msrEditorialAccidentalKind& elt);

// cautionary accidentals
//______________________________________________________________________________

enum class msrCautionaryAccidentalKind {
  kCautionaryAccidentalYes, kCautionaryAccidentalNo
};

string msrCautionaryAccidentalKindAsString (
  msrCautionaryAccidentalKind noteCautionaryAccidentalKind);

ostream& operator << (ostream& os, const msrCautionaryAccidentalKind& elt);


}


#endif