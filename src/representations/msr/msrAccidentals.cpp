/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrAccidentals.h"


namespace MusicFormats
{

// accidentals
//______________________________________________________________________________
std::string msrAccidentalKindAsString (
  msrAccidentalKind accidentalKind)
{
  std::string result;

  switch (accidentalKind) {
    case msrAccidentalKind::kAccidentalNone:
      result = "accidentalNone";
      break;

    case msrAccidentalKind::kAccidentalSharp:
      result = "kAccidentalSharp";
      break;
    case msrAccidentalKind::kAccidentalNatural:
      result = "kAccidentalNatural";
      break;
    case msrAccidentalKind::kAccidentalFlat:
      result = "kAccidentalFlat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSharp:
      result = "kAccidentalDoubleSharp";
      break;
    case msrAccidentalKind::kAccidentalSharpSharp:
      result = "kAccidentalSharpSharp";
      break;
    case msrAccidentalKind::kAccidentalFlatFlat:
      result = "kAccidentalFlatFlat";
      break;
    case msrAccidentalKind::kAccidentalNaturalSharp:
      result = "kAccidentalNaturalSharp";
      break;
    case msrAccidentalKind::kAccidentalNaturalFlat:
      result = "kAccidentalNaturalFlat";
      break;
    case msrAccidentalKind::kAccidentalQuarterFlat:
      result = "kAccidentalQuarterFlat";
      break;
    case msrAccidentalKind::kAccidentalQuarterSharp:
      result = "kAccidentalQuarterSharp";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersFlat:
      result = "kAccidentalThreeQuartersFlat";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersSharp:
      result = "kAccidentalThreeQuartersSharp";
      break;

    case msrAccidentalKind::kAccidentalSharpDown:
      result = "kAccidentalSharpDown";
      break;
    case msrAccidentalKind::kAccidentalSharpUp:
      result = "kAccidentalSharpUp";
      break;
    case msrAccidentalKind::kAccidentalNaturalDown:
      result = "kAccidentalNaturalDown";
      break;
    case msrAccidentalKind::kAccidentalNaturalUp:
      result = "kAccidentalNaturalUp";
      break;
    case msrAccidentalKind::kAccidentalFlatDown:
      result = "kAccidentalFlatDown";
      break;
    case msrAccidentalKind::kAccidentalFlatUp:
      result = "kAccidentalFlatUp";
      break;
    case msrAccidentalKind::kAccidentalTripleSharp:
      result = "kAccidentalTripleSharp";
      break;
    case msrAccidentalKind::kAccidentalTripleFlat:
      result = "kAccidentalTripleFlat";
      break;
    case msrAccidentalKind::kAccidentalSlashQuarterSharp:
      result = "kAccidentalSlashQuarterSharp";
      break;
    case msrAccidentalKind::kAccidentalSlashSharp:
      result = "kAccidentalSlashSharp";
      break;
    case msrAccidentalKind::kAccidentalSlashFlat:
      result = "kAccidentalSlashFlat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSlashFlat:
      result = "kAccidentaldoubleSlashFlat";
      break;
    case msrAccidentalKind::kAccidentalSharp_1:
      result = "kAccidentalSharp_1";
      break;
    case msrAccidentalKind::kAccidentalSharp_2:
      result = "kAccidentalSharp_2";
      break;
    case msrAccidentalKind::kAccidentalSharp_3:
      result = "kAccidentalSharp_3";
      break;
    case msrAccidentalKind::kAccidentalSharp_5:
      result = "kAccidentalSharp_5";
      break;
    case msrAccidentalKind::kAccidentalFlat_1:
      result = "kAccidentalFlat_1";
      break;
    case msrAccidentalKind::kAccidentalFlat_2:
      result = "kAccidentalFlat_2";
      break;
    case msrAccidentalKind::kAccidentalFlat_3:
      result = "kAccidentalFlat_3";
      break;
    case msrAccidentalKind::kAccidentalFlat_4:
      result = "kAccidentalFlat_4";
      break;
    case msrAccidentalKind::kAccidentalSori:
      result = "kAccidentalSori";
      break;
    case msrAccidentalKind::kAccidentalKoron:
      result = "kAccidentalKoron";
      break;

    case msrAccidentalKind::kAccidentalOther:
      result = "kAccidentalOther";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrAccidentalKind& elt)
{
  os <<msrAccidentalKindAsString (elt);
  return os;
}

std::string msrAccidentalKindAsMusicXMLString (
  msrAccidentalKind accidentalKind)
{
  std::string result;

  switch (accidentalKind) {
    case msrAccidentalKind::kAccidentalNone:
      break;

    case msrAccidentalKind::kAccidentalSharp:
      result = "sharp";
      break;
    case msrAccidentalKind::kAccidentalNatural:
      result = "natural";
      break;
    case msrAccidentalKind::kAccidentalFlat:
      result = "flat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSharp:
      result = "double-sharp";
      break;
    case msrAccidentalKind::kAccidentalSharpSharp:
      result = "sharp-sharp";
      break;
    case msrAccidentalKind::kAccidentalFlatFlat:
      result = "flat-flat";
      break;
    case msrAccidentalKind::kAccidentalNaturalSharp:
      result = "natural-sharp";
      break;
    case msrAccidentalKind::kAccidentalNaturalFlat:
      result = "natural-flat";
      break;
    case msrAccidentalKind::kAccidentalQuarterFlat:
      result = "quarter-flat";
      break;
    case msrAccidentalKind::kAccidentalQuarterSharp:
      result = "quarter-sharp";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersFlat:
      result = "three-quarters-flat";
      break;
    case msrAccidentalKind::kAccidentalThreeQuartersSharp:
      result = "three-quarters-sharp";
      break;

    case msrAccidentalKind::kAccidentalSharpDown:
      result = "sharp-down";
      break;
    case msrAccidentalKind::kAccidentalSharpUp:
      result = "sharp-up";
      break;
    case msrAccidentalKind::kAccidentalNaturalDown:
      result = "natural-down";
      break;
    case msrAccidentalKind::kAccidentalNaturalUp:
      result = "natural-up";
      break;
    case msrAccidentalKind::kAccidentalFlatDown:
      result = "flat-down";
      break;
    case msrAccidentalKind::kAccidentalFlatUp:
      result = "flat-up";
      break;
    case msrAccidentalKind::kAccidentalTripleSharp:
      result = "triple-sharp";
      break;
    case msrAccidentalKind::kAccidentalTripleFlat:
      result = "triple-flat";
      break;
    case msrAccidentalKind::kAccidentalSlashQuarterSharp:
      result = "slash-quarter-sharp";
      break;
    case msrAccidentalKind::kAccidentalSlashSharp:
      result = "slash-sharp";
      break;
    case msrAccidentalKind::kAccidentalSlashFlat:
      result = "slash-flat";
      break;
    case msrAccidentalKind::kAccidentalDoubleSlashFlat:
      result = "double-slash-flat";
      break;
    case msrAccidentalKind::kAccidentalSharp_1:
      result = "sharp-1";
      break;
    case msrAccidentalKind::kAccidentalSharp_2:
      result = "sharp-2";
      break;
    case msrAccidentalKind::kAccidentalSharp_3:
      result = "sharp-3";
      break;
    case msrAccidentalKind::kAccidentalSharp_5:
      result = "sharp-5";
      break;
    case msrAccidentalKind::kAccidentalFlat_1:
      result = "flat-1";
      break;
    case msrAccidentalKind::kAccidentalFlat_2:
      result = "flat-2";
      break;
    case msrAccidentalKind::kAccidentalFlat_3:
      result = "flat-3";
      break;
    case msrAccidentalKind::kAccidentalFlat_4:
      result = "flat-4";
      break;
    case msrAccidentalKind::kAccidentalSori:
      result = "sori";
      break;
    case msrAccidentalKind::kAccidentalKoron:
      result = "koron";
      break;

    case msrAccidentalKind::kAccidentalOther:
      result = "other";
      break;
  } // switch

  return result;
}

// editorial accidentals
//______________________________________________________________________________

std::string msrEditorialAccidentalKindAsString (
  msrEditorialAccidentalKind noteEditorialAccidentalKind)
{
  std::string result;

  switch (noteEditorialAccidentalKind) {
    case msrEditorialAccidentalKind::kEditorialAccidentalYes:
      result = "kEditorialAccidentalYes";
      break;
    case msrEditorialAccidentalKind::kEditorialAccidentalNo:
      result = "kEditorialAccidentalNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrEditorialAccidentalKind& elt)
{
  os <<msrEditorialAccidentalKindAsString (elt);
  return os;
}

// cautionary accidentals
//______________________________________________________________________________

std::string msrCautionaryAccidentalKindAsString (
  msrCautionaryAccidentalKind noteCautionaryAccidentalKind)
{
  std::string result;

  switch (noteCautionaryAccidentalKind) {
    case msrCautionaryAccidentalKind::kCautionaryAccidentalYes:
      result = "kCautionaryAccidentalYes";
      break;
    case msrCautionaryAccidentalKind::kCautionaryAccidentalNo:
      result = "kCautionaryAccidentalNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrCautionaryAccidentalKind& elt)
{
  os <<msrCautionaryAccidentalKindAsString (elt);
  return os;
}


}
