/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPitches___
#define ___msrPitches___

#include <string>
#include <ostream>

#include "exports.h"


namespace MusicFormats
{

// diatonic pitches
//______________________________________________________________________________
enum class msrDiatonicPitchKind {
  kDiatonicPitch_UNKNOWN_,

  // starting at C for LilyPond relative octave calculations
  kDiatonicPitchC,
  kDiatonicPitchD, kDiatonicPitchE, kDiatonicPitchF,
  kDiatonicPitchG, kDiatonicPitchA, kDiatonicPitchB
};

std::string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind);

std::ostream& operator << (std::ostream& os, const msrDiatonicPitchKind& elt);

EXP msrDiatonicPitchKind msrDiatonicPitchKindFromChar (
  char diatonicNoteName);

// semi tones pitches
//______________________________________________________________________________
enum class msrSemiTonesPitchKind {
  kSTP_NoSemiTonesPitch,

  kSTP_C_TripleFlat,
  kSTP_C_DoubleFlat, kSTP_C_Flat,
  kSTP_C_Natural,
  kSTP_C_Sharp, kSTP_C_DoubleSharp,
  kSTP_C_TripleSharp,

  kSTP_D_TripleFlat,
  kSTP_D_DoubleFlat, kSTP_D_Flat,
  kSTP_D_Natural,
  kSTP_D_Sharp, kSTP_D_DoubleSharp,
  kSTP_D_TripleSharp,

  kSTP_E_TripleFlat,
  kSTP_E_DoubleFlat, kSTP_E_Flat,
  kSTP_E_Natural,
  kSTP_E_Sharp, kSTP_E_DoubleSharp,
  kSTP_E_TripleSharp,

  kSTP_F_TripleFlat,
  kSTP_F_DoubleleFlat, kSTP_F_Flat,
  kSTP_F_Natural,
  kSTP_F_Sharp, kSTP_F_DoubleSharp,
  kSTP_F_TripleSharp,

  kSTP_G_TripleFlat,
  kSTP_G_DoubleFlat, kSTP_G_Flat,
  kSTP_G_Natural,
  kSTP_G_Sharp, kSTP_G_DoubleSharp,
  kSTP_G_TripleSharp,

  kSTP_A_TripleFlat,
  kSTP_A_DoubleFlat, kSTP_A_Flat,
  kSTP_A_Natural,
  kSTP_A_Sharp, kSTP_A_DoubleSharp,
  kSTP_A_TripleSharp,

  kSTP_B_TripleFlat,
  kSTP_B_DoubleFlat, kSTP_B_Flat,
  kSTP_B_Natural,
  kSTP_B_Sharp, kSTP_B_DoubleSharp,
  kSTP_B_TripleSharp
};

std::string msrSemiTonesPitchKindAsString (
  msrSemiTonesPitchKind semiTonesPitchKind);

std::ostream& operator << (std::ostream& os, const msrSemiTonesPitchKind& elt);

// quarter tones pitches
//______________________________________________________________________________
enum class msrQuarterTonesPitchKind {
  kQTP_UNKNOWN_,

  kQTP_Rest, kQTP_Skip,

  kQTP_A_TripleFlat,
  kQTP_A_DoubleFlat, kQTP_A_SesquiFlat, kQTP_A_Flat, kQTP_A_SemiFlat,
  kQTP_A_Natural,
  kQTP_A_SemiSharp, kQTP_A_Sharp, kQTP_A_SesquiSharp, kQTP_A_DoubleSharp,
  kQTP_A_TripleSharp,

  kQTP_B_TripleFlat,
  kQTP_B_DoubleFlat, kQTP_B_SesquiFlat, kQTP_B_Flat, kQTP_B_SemiFlat,
  kQTP_B_Natural,
  kQTP_B_SemiSharp, kQTP_B_Sharp, kQTP_B_SesquiSharp, kQTP_B_DoubleSharp,
  kQTP_B_TripleSharp,

  kQTP_C_TripleFlat,
  kQTP_C_DoubleFlat, kQTP_C_SesquiFlat, kQTP_C_Flat, kQTP_C_SemiFlat,
  kQTP_C_Natural,
  kQTP_C_SemiSharp, kQTP_C_Sharp, kQTP_C_SesquiSharp, kQTP_C_DoubleSharp,
  kQTP_C_TripleSharp,

  kQTP_D_TripleFlat,
  kQTP_D_DoubleFlat, kQTP_D_SesquiFlat, kQTP_D_Flat, kQTP_D_SemiFlat,
  kQTP_D_Natural,
  kQTP_D_SemiSharp, kQTP_D_Sharp, kQTP_D_SesquiSharp, kQTP_D_DoubleSharp,
  kQTP_D_TripleSharp,

  kQTP_E_TripleFlat,
  kQTP_E_DoubleFlat, kQTP_E_SesquiFlat, kQTP_E_Flat, kQTP_E_SemiFlat,
  kQTP_E_Natural,
  kQTP_E_SemiSharp, kQTP_E_Sharp, kQTP_E_SesquiSharp, kQTP_E_DoubleSharp,
  kQTP_E_TripleSharp,

  kQTP_F_TripleFlat,
  kQTP_F_DoubleFlat, kQTP_F_SesquiFlat, kQTP_F_Flat, kQTP_F_SemiFlat,
  kQTP_F_Natural,
  kQTP_F_SemiSharp, kQTP_F_Sharp, kQTP_F_SesquiSharp, kQTP_F_DoubleSharp,
  kQTP_F_TripleSharp,

  kQTP_G_TripleFlat,
  kQTP_G_DoubleFlat, kQTP_G_SesquiFlat, kQTP_G_Flat, kQTP_G_SemiFlat,
  kQTP_G_Natural,
  kQTP_G_SemiSharp, kQTP_G_Sharp, kQTP_G_SesquiSharp, kQTP_G_DoubleSharp,
  kQTP_G_TripleSharp
};

EXP std::string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchKind quarterTonesPitchKind);

std::ostream& operator << (std::ostream& os, const msrQuarterTonesPitchKind& elt);

// alterations
//______________________________________________________________________________
enum class msrAlterationKind {
  kAlteration_UNKNOWN_,

  kAlterationTripleFlat, kAlterationDoubleFlat, kAlterationSesquiFlat,
  kAlterationFlat, kAlterationSemiFlat,
  kAlterationNatural,
  kAlterationSemiSharp, kAlterationSharp, kAlterationSesquiSharp,
  kAlterationDoubleSharp, kAlterationTripleSharp
};

std::string msrAlterationKindAsString (
  msrAlterationKind alterationKind);

std::ostream& operator << (std::ostream& os, const msrAlterationKind& elt);

EXP msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter);

EXP float msrMusicXMLAlterFromAlterationKind (
  msrAlterationKind alterationKind);

// alterations preferences
//______________________________________________________________________________

enum class msrAlterationPreferenceKind {
  kAlterationPreferenceFlat,
  kAlterationPreferenceNatural,
  kAlterationPreferenceSharp
};

EXP std::string msrAlterationPreferenceKindAsString (
  msrAlterationPreferenceKind alterationPreferenceKind);

std::ostream& operator << (std::ostream& os, const msrAlterationPreferenceKind& elt);

//______________________________________________________________________________
msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);

EXP void fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind);

EXP msrQuarterTonesPitchKind quarterTonesPitchKindFromDiatonicPitchAndAlteration (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind);

EXP msrDiatonicPitchKind diatonicPitchKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrAlterationKind alterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrQuarterTonesPitchKind quarterTonesPitchKindFromSemiTonesPitchKind (
  msrSemiTonesPitchKind semiTonesPitchKind);

EXP msrSemiTonesPitchKind semiTonesPitchKindFromQuarterTonesPitchKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind);

EXP msrSemiTonesPitchKind semiTonesPitchKindFromString (
  const std::string& theString);

// enharmonies
//______________________________________________________________________________
EXP msrSemiTonesPitchKind enharmonicSemiTonesPitch (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);


}

#endif // ___msrPitches___
