/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "msrPitches.h"
#include "msrPitchesNames.h"

#include "msrOah.h"

#include "msrWae.h"


namespace MusicFormats
{

// diatonic pitches
//______________________________________________________________________________
std::string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind)
{
  std::string result;

  switch (diatonicPitchKind) {
    case msrDiatonicPitchKind::kDiatonicPitch_NO_:
      result = "***kDiatonicPitch_NO_***";
      break;

    case msrDiatonicPitchKind::kDiatonicPitchA:
      result = "A";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchB:
      result = "B";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchC:
      result = "C";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchD:
      result = "D";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchE:
      result = "E";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchF:
      result = "F";
      break;
    case msrDiatonicPitchKind::kDiatonicPitchG:
      result = "G";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDiatonicPitchKind& elt)
{
  os << msrDiatonicPitchKindAsString (elt);
  return os;
}

msrDiatonicPitchKind msrDiatonicPitchKindFromChar (
  char diatonicNoteName)
{
  msrDiatonicPitchKind result = msrDiatonicPitchKind::kDiatonicPitch_NO_;

  switch (diatonicNoteName) {
    case 'a':
    case 'A':
      result = msrDiatonicPitchKind::kDiatonicPitchA;
       break;
    case 'b':
    case 'B':
      result = msrDiatonicPitchKind::kDiatonicPitchB;
      break;
    case 'c':
    case 'C':
      result = msrDiatonicPitchKind::kDiatonicPitchC;
      break;
    case 'd':
    case 'D':
      result = msrDiatonicPitchKind::kDiatonicPitchD;
      break;
    case 'e':
    case 'E':
      result = msrDiatonicPitchKind::kDiatonicPitchE;
      break;
    case 'f':
    case 'F':
      result = msrDiatonicPitchKind::kDiatonicPitchF;
      break;
    case 'g':
    case 'G':
      result = msrDiatonicPitchKind::kDiatonicPitchG;
      break;
    default: {}
  } // switch

  return result;
}

// alterations
//______________________________________________________________________________
msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter)
{

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones:

  semi-sharp semi-flat sesqui-sharp sesqui-flat double-sharp double-flat
      +0.5      -0.5        +1.5       -1.5         +2.0        -2.0
*/

  msrAlterationKind result = msrAlterationKind::kAlteration_NO_;

  if      (alter == 0 ) {
    result = msrAlterationKind::kAlterationNatural;
  }

  else if (alter == -1 ) {
    result = msrAlterationKind::kAlterationFlat;
  }

  else if (alter == 1 ) {
    result = msrAlterationKind::kAlterationSharp;
  }

  else if (alter == -0.5 ) {
    result = msrAlterationKind::kAlterationSemiFlat;
  }

  else if (alter == +0.5 ) {
    result = msrAlterationKind::kAlterationSemiSharp;
  }

  else if (alter == -1.5 ) {
    result = msrAlterationKind::kAlterationSesquiFlat;
  }

  else if (alter == +1.5 ) {
    result = msrAlterationKind::kAlterationSesquiSharp;
  }

  else if (alter == -2 ) {
    result = msrAlterationKind::kAlterationDoubleFlat;
  }

  else if (alter == +2 ) {
    result = msrAlterationKind::kAlterationDoubleSharp;
  }

  else if (alter == -3 ) {
    result = msrAlterationKind::kAlterationTripleFlat;
  }

  else if (alter == +3 ) {
    result = msrAlterationKind::kAlterationTripleSharp;
  }

  return result;
}

float msrMusicXMLAlterFromAlterationKind (
  msrAlterationKind alterationKind)
{
  float result = -111.0;

  switch (alterationKind) {
    case msrAlterationKind::kAlteration_NO_:
      break;

    case msrAlterationKind::kAlterationTripleFlat:
      result = -3;
      break;
    case msrAlterationKind::kAlterationDoubleFlat:
      result = -2;
      break;
    case msrAlterationKind::kAlterationSesquiFlat:
      result = -1.5;
      break;
    case msrAlterationKind::kAlterationFlat:
      result = -1;
      break;
    case msrAlterationKind::kAlterationSemiFlat:
      result = -0.5;
      break;
    case msrAlterationKind::kAlterationNatural:
      result = 0;
      break;
    case msrAlterationKind::kAlterationSemiSharp:
      result = +0.5;
      break;
    case msrAlterationKind::kAlterationSharp:
      result = 1;
      break;
    case msrAlterationKind::kAlterationSesquiSharp:
      result = +1.5;
      break;
    case msrAlterationKind::kAlterationDoubleSharp:
      result = +2;
      break;
    case msrAlterationKind::kAlterationTripleSharp:
      result = +3;
      break;
  } // switch

  return result;
}

std::string msrAlterationKindAsString (
  msrAlterationKind alterationKind)
{
  std::string result;

  switch (alterationKind) {
    case msrAlterationKind::kAlteration_NO_:
      result = "***noAlteration***";
      break;

    case msrAlterationKind::kAlterationTripleFlat:
      result = "kAlterationTripleFlat";
      break;
    case msrAlterationKind::kAlterationDoubleFlat:
      result = "kAlterationDoubleFlat";
      break;
    case msrAlterationKind::kAlterationSesquiFlat:
      result = "kAlterationSesquiFlat";
      break;
    case msrAlterationKind::kAlterationFlat:
      result = "kAlterationFlat";
      break;
    case msrAlterationKind::kAlterationSemiFlat:
      result = "kAlterationSemiFlat";
      break;
    case msrAlterationKind::kAlterationNatural:
      result = "kAlterationNatural";
      break;
    case msrAlterationKind::kAlterationSemiSharp:
      result = "kAlterationSemiSharp";
      break;
    case msrAlterationKind::kAlterationSharp:
      result = "kAlterationSharp";
      break;
    case msrAlterationKind::kAlterationSesquiSharp:
      result = "kAlterationSesquiSharp";
      break;
    case msrAlterationKind::kAlterationDoubleSharp:
      result = "kAlterationDoubleSharp";
      break;
    case msrAlterationKind::kAlterationTripleSharp:
      result = "kAlterationTripleSharp";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrAlterationKind& elt)
{
  os << msrAlterationKindAsString (elt);
  return os;
}

std::string msrAlterationPreferenceKindAsString (
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  std::string result;

  switch (alterationPreferenceKind) {
    case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
      result = "kAlterationPreferenceFlat";
      break;
    case msrAlterationPreferenceKind::kAlterationPreferenceNatural:
      result = "kAlterationPreferenceNatural";
      break;
    case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
      result = "kAlterationPreferenceSharp";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrAlterationPreferenceKind& elt)
{
  os << msrAlterationPreferenceKindAsString (elt);
  return os;
}

// semi tones pitches
//______________________________________________________________________________
msrSemiTonesPitchKind semiTonesPitchKindFromString (
  const std::string& theString)
{
  msrSemiTonesPitchKind result;

  // fetch the quarternotes pitches kind
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        theString);

  // fetch the semitones pitches kind
  result =
    semiTonesPitchKindFromQuarterTonesPitchKind (
      quarterTonesPitchKind);

  return result;
}

std::string msrSemiTonesPitchKindAsString (
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  std::string result;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = "***kSTP_NoSemiTonesPitch***";
      break;

    case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
      result = "kSTP_C_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
      result = "kSTP_C_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = "kSTP_C_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Natural:
      result = "kSTP_C_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = "kSTP_C_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      result = "kSTP_C_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
      result = "kSTP_C_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
      result = "kSTP_D_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      result = "kSTP_D_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = "kSTP_D_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Natural:
      result = "kSTP_D_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = "kSTP_D_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      result = "kSTP_D_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
      result = "kSTP_D_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
      result = "kSTP_E_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      result = "kSTP_E_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = "kSTP_E_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Natural:
      result = "kSTP_E_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      result = "kSTP_E_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      result = "kSTP_E_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
      result = "kSTP_E_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
      result = "kSTP_F_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
      result = "kSTP_F_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      result = "kSTP_F_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Natural:
      result = "kSTP_F_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = "kSTP_F_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      result = "kSTP_F_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
      result = "kSTP_F_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
      result = "kSTP_G_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      result = "kSTP_G_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = "kSTP_G_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Natural:
      result = "kSTP_G_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = "kSTP_G_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      result = "kSTP_G_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
      result = "kSTP_G_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
      result = "kSTP_A_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      result = "kSTP_A_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = "kSTP_A_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Natural:
      result = "kSTP_A_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = "kSTP_A_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      result = "kSTP_A_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
      result = "kSTP_A_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
      result = "kSTP_B_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      result = "kSTP_B_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = "kSTP_B_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Natural:
      result = "kSTP_B_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = "kSTP_B_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      result = "kSTP_B_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
      result = "kSTP_B_TripleSharp";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSemiTonesPitchKind& elt)
{
  os << msrSemiTonesPitchKindAsString (elt);
  return os;
}

/* JMI
std::string semiTonesPitchKindAsFlatsAndSharps (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrSemiTonesPitchKind              semiTonesPitchKind)
{
  std::string result;

/ * JMI
  msrDiatonicPitchKind
    diatonicPitchKind =
      diatonicPitchKindFromQuarterTonesPitchKind (
        inputLineNumber,
        quarterTonesPitchKind);
        * /

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = "***kSTP_NoSemiTonesPitch***";
      break;

    case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
      result = "C_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
      result = "C_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = "C_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Natural:
      result = "C_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = "C_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      result = "C_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
      result = "C_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
      result = "D_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      result = "D_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = "D_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Natural:
      result = "D_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = "D_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      result = "D_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
      result = "D_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
      result = "E_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      result = "E_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = "E_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Natural:
      result = "E_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      result = "E_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      result = "E_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
      result = "E_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
      result = "F_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
      result = "F_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      result = "F_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Natural:
      result = "F_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = "F_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      result = "F_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
      result = "F_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
      result = "G_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      result = "G_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = "G_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Natural:
      result = "G_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = "G_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      result = "G_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
      result = "G_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
      result = "A_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      result = "A_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = "A_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Natural:
      result = "A_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = "A_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      result = "A_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
      result = "A_TripleSharp";
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
      result = "B_TripleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      result = "B_DoubleFlat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = "B_Flat";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Natural:
      result = "B_Natural";
      break;
    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = "B_Sharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      result = "B_DoubleSharp";
      break;
    case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
      result = "B_TripleSharp";
      break;
  } // switch

  return result;
}
*/

msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrQuarterTonesPitchKind
    result =
      msrQuarterTonesPitchKind::kQTP_NO_;

  /* JMI
  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = msrQuarterTonesPitchKind::kQTP_NO_;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural: // msrSemiTonesPitchKind::kSTP_B_Sharp, msrSemiTonesPitchKind::kSTP_D_DoubleFlat
      result = kC_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp: // msrSemiTonesPitchKind::kSTP_B_DoubleSharp, msrSemiTonesPitchKind::kSTP_D_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kC_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kD_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural: // msrSemiTonesPitchKind::kSTP_C_DoubleSharp, msrSemiTonesPitchKind::kSTP_E_DoubleFlat
      result = kD_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp: // msrSemiTonesPitchKind::kSTP_E_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kD_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kE_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural: // msrSemiTonesPitchKind::kSTP_D_DoubleSharp, msrSemiTonesPitchKind::kSTP_F_Flat
      result = kE_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural: // msrSemiTonesPitchKind::kSTP_E_Sharp, msrSemiTonesPitchKind::kSTP_G_DoubleFlat
      result = kF_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp: // msrSemiTonesPitchKind::kSTP_E_DoubleSharp, msrSemiTonesPitchKind::kSTP_G_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kF_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kG_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural: // msrSemiTonesPitchKind::kSTP_F_DoubleSharp, msrSemiTonesPitchKind::kSTP_A_DoubleFlat
      result = kG_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp: // msrSemiTonesPitchKind::kSTP_A_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = kG_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural: // msrSemiTonesPitchKind::kSTP_G_DoubleSharp, msrSemiTonesPitchKind::kSTP_B_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_A_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp: // msrSemiTonesPitchKind::kSTP_B_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = kB_Flat;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural: // msrSemiTonesPitchKind::kSTP_A_DoubleSharp, msrSemiTonesPitchKind::kSTP_C_Flat
      result = kB_Natural;
      break;
  } // switch
  */


  /* JMI
  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      result = msrQuarterTonesPitchKind::kQTP_NO_;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural: // msrSemiTonesPitchKind::kSTP_B_Sharp, msrSemiTonesPitchKind::kSTP_D_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_C_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp: // msrSemiTonesPitchKind::kSTP_B_DoubleSharp, msrSemiTonesPitchKind::kSTP_D_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural: // msrSemiTonesPitchKind::kSTP_C_DoubleSharp, msrSemiTonesPitchKind::kSTP_E_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_D_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp: // msrSemiTonesPitchKind::kSTP_E_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural: // msrSemiTonesPitchKind::kSTP_D_DoubleSharp, msrSemiTonesPitchKind::kSTP_F_Flat
      result = msrQuarterTonesPitchKind::kQTP_E_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural: // msrSemiTonesPitchKind::kSTP_E_Sharp, msrSemiTonesPitchKind::kSTP_G_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_F_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp: // msrSemiTonesPitchKind::kSTP_E_DoubleSharp, msrSemiTonesPitchKind::kSTP_G_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural: // msrSemiTonesPitchKind::kSTP_F_DoubleSharp, msrSemiTonesPitchKind::kSTP_A_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_G_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp: // msrSemiTonesPitchKind::kSTP_A_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural: // msrSemiTonesPitchKind::kSTP_G_DoubleSharp, msrSemiTonesPitchKind::kSTP_B_DoubleFlat
      result = msrQuarterTonesPitchKind::kQTP_A_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp: // msrSemiTonesPitchKind::kSTP_B_Flat
      switch (alterationPreferenceKind) {
        case msrAlterationPreferenceKind::kAlterationPreferenceFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrAlterationPreferenceKind::kAlterationPreferenceSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
      } // switch
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural: // msrSemiTonesPitchKind::kSTP_A_DoubleSharp, msrSemiTonesPitchKind::kSTP_C_Flat
      result = msrQuarterTonesPitchKind::kQTP_B_Natural;
      break;
  } // switch
  */

  return result;
}

//______________________________________________________________________________
void setDiatonicPitchAndAlterationKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind)
{
  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_NO_:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA; // any value would fit
      alterationKind    = msrAlterationKind::kAlteration_NO_;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchB;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchC;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchD;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchE;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchF;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationTripleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationDoubleFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSesquiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSemiFlat;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationNatural;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSemiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationSesquiSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationDoubleSharp;
      break;
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchG;
      alterationKind    = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
    case msrQuarterTonesPitchKind::kQTP_Skip:
      diatonicPitchKind = msrDiatonicPitchKind::kDiatonicPitchA; // any value would fit JMI
      alterationKind    = msrAlterationKind::kAlteration_NO_;

      break;
  } // switch
}

std::string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  std::string result;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_NO_:
      result = "***noQuarterTonesPitch***";
      break;

    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
      result = "kQTP_A_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
      result = "kQTP_A_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
      result = "kQTP_A_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Flat:
      result = "kQTP_A_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
      result = "kQTP_A_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Natural:
      result = "kQTP_A_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
      result = "kQTP_A_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
      result = "kQTP_A_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
      result = "kQTP_A_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
      result = "kQTP_A_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
      result = "kQTP_A_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
      result = "kQTP_B_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
      result = "kQTP_B_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
      result = "kQTP_B_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
      result = "kQTP_B_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
      result = "kQTP_B_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
      result = "kQTP_B_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
      result = "kQTP_B_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
      result = "kQTP_B_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
      result = "kQTP_B_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
      result = "kQTP_B_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
      result = "kQTP_B_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
      result = "kQTP_C_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
      result = "kQTP_C_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
      result = "kQTP_C_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
      result = "kQTP_C_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
      result = "kQTP_C_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
      result = "kQTP_C_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
      result = "kQTP_C_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
      result = "kQTP_C_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
      result = "kQTP_C_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
      result = "kQTP_C_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
      result = "kQTP_C_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
      result = "kQTP_D_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
      result = "kQTP_D_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
      result = "kQTP_D_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
      result = "kQTP_D_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
      result = "kQTP_D_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
      result = "kQTP_D_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
      result = "kQTP_D_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
      result = "kQTP_D_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
      result = "kQTP_D_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
      result = "kQTP_D_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
      result = "kQTP_D_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
      result = "kQTP_E_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
      result = "kQTP_E_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
      result = "kQTP_E_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
      result = "kQTP_E_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
      result = "kQTP_E_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
      result = "kQTP_E_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
      result = "kQTP_E_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
      result = "kQTP_E_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
      result = "kQTP_E_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
      result = "kQTP_E_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
      result = "kQTP_E_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
      result = "kQTP_F_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
      result = "kQTP_F_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
      result = "kQTP_F_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
      result = "kQTP_F_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
      result = "kQTP_F_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
      result = "kQTP_F_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
      result = "kQTP_F_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
      result = "kQTP_F_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
      result = "kQTP_F_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
      result = "kQTP_F_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
      result = "kQTP_F_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
      result = "kQTP_G_TripleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
      result = "kQTP_G_DoubleFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
      result = "kQTP_G_SesquiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
      result = "kQTP_G_Flat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
      result = "kQTP_G_SemiFlat";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
      result = "kQTP_G_Natural";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
      result = "kQTP_G_SemiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
      result = "kQTP_G_Sharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
      result = "kQTP_G_SesquiSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
      result = "kQTP_G_DoubleSharp";
      break;
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      result = "kQTP_G_TripleSharp";
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
      result = "kQTP_Rest";
      break;
    case msrQuarterTonesPitchKind::kQTP_Skip:
      result = "kQTP_Skip";
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrQuarterTonesPitchKind& elt)
{
  os << msrQuarterTonesPitchKindAsString (elt);
  return os;
}

void fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind)
{
  diatonicPitchKind =
    diatonicPitchKindFromQuarterTonesPitchKind (
      inputLineNumber,
      quarterTonesPitchKind);

  alterationKind  =
    alterationKindFromQuarterTonesPitchKind (
      inputLineNumber,
      quarterTonesPitchKind);
}

std::string existingQuarterTonesPitchesLanguageKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    quarterTonesPitchesLanguageKindsMapSize =
      getQuarterTonesPitchesLanguageKindsMap ().size ();

  if (quarterTonesPitchesLanguageKindsMapSize) {
    size_t
      nextToLast =
        quarterTonesPitchesLanguageKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator i =
        getQuarterTonesPitchesLanguageKindsMap ().begin ();
      i != getQuarterTonesPitchesLanguageKindsMap ().end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != quarterTonesPitchesLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// quarter tones pitches
//______________________________________________________________________________
msrQuarterTonesPitchKind quarterTonesPitchKindFromDiatonicPitchAndAlteration (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind)
{
  msrQuarterTonesPitchKind
    result =
      msrQuarterTonesPitchKind::kQTP_NO_;

  switch (diatonicPitchKind) {
    case msrDiatonicPitchKind::kDiatonicPitch_NO_:
      {
        result = msrQuarterTonesPitchKind::kQTP_NO_;
        /* JMI
        std::stringstream s;

        s <<
          "cannot convert msrDiatonicPitchKind::kDiatonicPitch_NO_ to a quarter tones pitch"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
      break;

    case msrDiatonicPitchKind::kDiatonicPitchA:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_A_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_A_TripleSharp;
          break;
        case msrAlterationKind::kAlteration_NO_:
          {
            std::stringstream s;

            s <<
              "'A' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchB:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_B_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_B_TripleSharp;
          break;
        case msrAlterationKind::kAlteration_NO_:
          {
            std::stringstream s;

            s <<
              "'B' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

      break;

    case msrDiatonicPitchKind::kDiatonicPitchC:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_C_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
          break;
        case msrAlterationKind::kAlteration_NO_:
          {
            std::stringstream s;

            s <<
              "'C' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchD:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_D_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_D_TripleSharp;
          break;
        case msrAlterationKind::kAlteration_NO_:
          {
            std::stringstream s;

            s <<
              "'D' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchE:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_E_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_E_TripleSharp;
          break;
        case msrAlterationKind::kAlteration_NO_:
          {
            std::stringstream s;

            s <<
              "'E' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchF:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_F_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
          break;
        case msrAlterationKind::kAlteration_NO_:
          {
            std::stringstream s;

            s <<
              "'F' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrDiatonicPitchKind::kDiatonicPitchG:
      switch (alterationKind) {
        case msrAlterationKind::kAlterationTripleFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleFlat;
          break;
        case msrAlterationKind::kAlterationDoubleFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
          break;
        case msrAlterationKind::kAlterationSesquiFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_SesquiFlat;
          break;
        case msrAlterationKind::kAlterationFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;
        case msrAlterationKind::kAlterationSemiFlat:
          result = msrQuarterTonesPitchKind::kQTP_G_SemiFlat;
          break;
        case msrAlterationKind::kAlterationNatural:
          result = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;
        case msrAlterationKind::kAlterationSemiSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_SemiSharp;
          break;
        case msrAlterationKind::kAlterationSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;
        case msrAlterationKind::kAlterationSesquiSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_SesquiSharp;
          break;
        case msrAlterationKind::kAlterationDoubleSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
          break;
        case msrAlterationKind::kAlterationTripleSharp:
          result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
          break;
        case msrAlterationKind::kAlteration_NO_:
          {
            std::stringstream s;

            s <<
              "'G' alteration has not been set"
              ", line = " << inputLineNumber;

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;
  } // switch

  return result;
}

msrDiatonicPitchKind diatonicPitchKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrDiatonicPitchKind result = msrDiatonicPitchKind::kDiatonicPitch_NO_;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_A_Flat:
    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_A_Natural:
    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchA;
      break;

    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchB;
      break;

    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchC;
      break;

    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchD;
      break;

    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchE;
      break;

    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchF;
      break;

    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      result = msrDiatonicPitchKind::kDiatonicPitchG;
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
      {
        std::stringstream s;

        s <<
          "cannot get the diatonic pitch of a rest"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::kQTP_Skip:
      {
        std::stringstream s;

        s <<
          "cannot get the diatonic pitch of a skip"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::kQTP_NO_:
      {
        result = msrDiatonicPitchKind::kDiatonicPitch_NO_;

        /* JMI
        std::stringstream s;

        s <<
          "cannot get the diatonic pitch of a msrQuarterTonesPitchKind::kQTP_NO_"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
  } // switch

  return result;
}

msrAlterationKind alterationKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrAlterationKind result = msrAlterationKind::kAlteration_NO_;

  switch (quarterTonesPitchKind) {
    case msrQuarterTonesPitchKind::kQTP_A_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_TripleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_TripleFlat:
      result = msrAlterationKind::kAlterationTripleFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleFlat:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleFlat:
      result = msrAlterationKind::kAlterationDoubleFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiFlat:
      result = msrAlterationKind::kAlterationSesquiFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Flat:
    case msrQuarterTonesPitchKind::kQTP_B_Flat:
    case msrQuarterTonesPitchKind::kQTP_C_Flat:
    case msrQuarterTonesPitchKind::kQTP_D_Flat:
    case msrQuarterTonesPitchKind::kQTP_E_Flat:
    case msrQuarterTonesPitchKind::kQTP_F_Flat:
    case msrQuarterTonesPitchKind::kQTP_G_Flat:
      result = msrAlterationKind::kAlterationFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_B_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_C_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_D_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_E_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_F_SemiFlat:
    case msrQuarterTonesPitchKind::kQTP_G_SemiFlat:
      result = msrAlterationKind::kAlterationSemiFlat;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Natural:
    case msrQuarterTonesPitchKind::kQTP_B_Natural:
    case msrQuarterTonesPitchKind::kQTP_C_Natural:
    case msrQuarterTonesPitchKind::kQTP_D_Natural:
    case msrQuarterTonesPitchKind::kQTP_E_Natural:
    case msrQuarterTonesPitchKind::kQTP_F_Natural:
    case msrQuarterTonesPitchKind::kQTP_G_Natural:
      result = msrAlterationKind::kAlterationNatural;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_SemiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_SemiSharp:
      result = msrAlterationKind::kAlterationSemiSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_Sharp:
    case msrQuarterTonesPitchKind::kQTP_B_Sharp:
    case msrQuarterTonesPitchKind::kQTP_C_Sharp:
    case msrQuarterTonesPitchKind::kQTP_D_Sharp:
    case msrQuarterTonesPitchKind::kQTP_E_Sharp:
    case msrQuarterTonesPitchKind::kQTP_F_Sharp:
    case msrQuarterTonesPitchKind::kQTP_G_Sharp:
      result = msrAlterationKind::kAlterationSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_B_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_C_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_D_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_E_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_F_SesquiSharp:
    case msrQuarterTonesPitchKind::kQTP_G_SesquiSharp:
      result = msrAlterationKind::kAlterationSesquiSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_B_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_C_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_D_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_E_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_F_DoubleSharp:
    case msrQuarterTonesPitchKind::kQTP_G_DoubleSharp:
      result = msrAlterationKind::kAlterationDoubleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_A_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_B_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_C_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_D_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_E_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_F_TripleSharp:
    case msrQuarterTonesPitchKind::kQTP_G_TripleSharp:
      result = msrAlterationKind::kAlterationTripleSharp;
      break;

    case msrQuarterTonesPitchKind::kQTP_Rest:
      {
        std::stringstream s;

        s <<
          "cannot get the alteration kind of a rest"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::kQTP_Skip:
      {
        std::stringstream s;

        s <<
          "cannot get the alteration kind of a skip"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

    case msrQuarterTonesPitchKind::kQTP_NO_:
      {
        result = msrAlterationKind::kAlteration_NO_;

        /* JMI
        std::stringstream s;

        s <<
          "cannot get the diatonic pitch of a msrQuarterTonesPitchKind::kQTP_NO_"
          ", line = " << inputLineNumber;

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          */
      }
  } // switch

  return result;
}

msrQuarterTonesPitchKind quarterTonesPitchKindFromSemiTonesPitchKind (
  msrSemiTonesPitchKind semiTonesPitchKind)
{
  msrQuarterTonesPitchKind result = msrQuarterTonesPitchKind::kQTP_NO_;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch:
      break;

    case msrSemiTonesPitchKind::kSTP_C_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_C_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_C_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = msrQuarterTonesPitchKind::kQTP_C_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_C_Natural:
      result = msrQuarterTonesPitchKind::kQTP_C_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_C_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_C_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_C_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_C_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_C_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_D_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_D_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_D_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = msrQuarterTonesPitchKind::kQTP_D_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Natural:
      result = msrQuarterTonesPitchKind::kQTP_D_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_D_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_D_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_D_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_D_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_D_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_E_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_E_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_E_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = msrQuarterTonesPitchKind::kQTP_E_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Natural:
      result = msrQuarterTonesPitchKind::kQTP_E_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_E_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_E_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_E_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_E_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_E_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_F_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_F_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleleFlat:
      result = msrQuarterTonesPitchKind::kQTP_F_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_F_Flat:
      result = msrQuarterTonesPitchKind::kQTP_F_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_F_Natural:
      result = msrQuarterTonesPitchKind::kQTP_F_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_F_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_F_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_F_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_F_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_F_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_G_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_G_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_G_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = msrQuarterTonesPitchKind::kQTP_G_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Natural:
      result = msrQuarterTonesPitchKind::kQTP_G_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_G_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_G_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_G_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_G_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_G_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_A_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_A_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_A_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = msrQuarterTonesPitchKind::kQTP_A_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Natural:
      result = msrQuarterTonesPitchKind::kQTP_A_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_A_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_A_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_A_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_A_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_A_TripleSharp;
      break;

    case msrSemiTonesPitchKind::kSTP_B_TripleFlat:
      result = msrQuarterTonesPitchKind::kQTP_B_TripleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleFlat:
      result = msrQuarterTonesPitchKind::kQTP_B_DoubleFlat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = msrQuarterTonesPitchKind::kQTP_B_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Natural:
      result = msrQuarterTonesPitchKind::kQTP_B_Natural;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = msrQuarterTonesPitchKind::kQTP_B_Sharp;
      break;
    case msrSemiTonesPitchKind::kSTP_B_DoubleSharp:
      result = msrQuarterTonesPitchKind::kQTP_B_DoubleSharp;
      break;
    case msrSemiTonesPitchKind::kSTP_B_TripleSharp:
      result = msrQuarterTonesPitchKind::kQTP_B_TripleSharp;
      break;
  } // switch

  return result;
}

msrSemiTonesPitchKind semiTonesPitchKindFromQuarterTonesPitchKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind)
{
  msrSemiTonesPitchKind result = msrSemiTonesPitchKind::kSTP_NoSemiTonesPitch;

    switch (quarterTonesPitchKind) {
      case msrQuarterTonesPitchKind::kQTP_NO_:
        break;

      case msrQuarterTonesPitchKind::kQTP_Rest:
      case msrQuarterTonesPitchKind::kQTP_Skip:
        break;

      case msrQuarterTonesPitchKind::kQTP_A_Flat:
        result = msrSemiTonesPitchKind::kSTP_A_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_A_Natural:
        result = msrSemiTonesPitchKind::kSTP_A_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_A_Sharp:
        result = msrSemiTonesPitchKind::kSTP_A_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_B_Flat:
        result = msrSemiTonesPitchKind::kSTP_B_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_B_Natural:
        result = msrSemiTonesPitchKind::kSTP_B_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_B_Sharp:
        result = msrSemiTonesPitchKind::kSTP_B_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_C_Flat:
        result = msrSemiTonesPitchKind::kSTP_C_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_C_Natural:
        result = msrSemiTonesPitchKind::kSTP_C_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_C_Sharp:
        result = msrSemiTonesPitchKind::kSTP_C_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_D_Flat:
        result = msrSemiTonesPitchKind::kSTP_D_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_D_Natural:
        result = msrSemiTonesPitchKind::kSTP_D_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_D_Sharp:
        result = msrSemiTonesPitchKind::kSTP_D_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_E_Flat:
        result = msrSemiTonesPitchKind::kSTP_E_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_E_Natural:
        result = msrSemiTonesPitchKind::kSTP_E_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_E_Sharp:
        result = msrSemiTonesPitchKind::kSTP_E_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_F_Flat:
        result = msrSemiTonesPitchKind::kSTP_F_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_F_Natural:
        result = msrSemiTonesPitchKind::kSTP_F_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_F_Sharp:
        result = msrSemiTonesPitchKind::kSTP_F_Sharp;
        break;

      case msrQuarterTonesPitchKind::kQTP_G_Flat:
        result = msrSemiTonesPitchKind::kSTP_G_Flat;
        break;
      case msrQuarterTonesPitchKind::kQTP_G_Natural:
        result = msrSemiTonesPitchKind::kSTP_G_Natural;
        break;
      case msrQuarterTonesPitchKind::kQTP_G_Sharp:
        result = msrSemiTonesPitchKind::kSTP_G_Sharp;
        break;

      default:
        ;
    } // switch

  return result;
}

std::string existingMsrQuarterTonesPitchesLanguageKinds (
  size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    quarterTonesPitchesLanguageKindsMapSize =
      getQuarterTonesPitchesLanguageKindsMap ().size ();

  if (quarterTonesPitchesLanguageKindsMapSize) {
    size_t nextToLast =
      quarterTonesPitchesLanguageKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msrQuarterTonesPitchesLanguageKind>::const_iterator i =
        getQuarterTonesPitchesLanguageKindsMap ().begin ();
      i != getQuarterTonesPitchesLanguageKindsMap ().end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
        break;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != quarterTonesPitchesLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// enharmonies
//______________________________________________________________________________
msrSemiTonesPitchKind enharmonicSemiTonesPitch (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  msrSemiTonesPitchKind result = semiTonesPitchKind;

  switch (semiTonesPitchKind) {
    case msrSemiTonesPitchKind::kSTP_C_Flat:
      result = msrSemiTonesPitchKind::kSTP_B_Natural;
      break;

    case msrSemiTonesPitchKind::kSTP_C_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_C_Sharp:
      result = msrSemiTonesPitchKind::kSTP_D_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_D_Flat:
      result = msrSemiTonesPitchKind::kSTP_C_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_D_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_D_Sharp:
      result = msrSemiTonesPitchKind::kSTP_E_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_E_Flat:
      result = msrSemiTonesPitchKind::kSTP_D_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_E_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_F_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_F_Sharp:
      result = msrSemiTonesPitchKind::kSTP_G_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_G_Flat:
      result = msrSemiTonesPitchKind::kSTP_F_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_G_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_G_Sharp:
      result = msrSemiTonesPitchKind::kSTP_A_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_A_Flat:
      result = msrSemiTonesPitchKind::kSTP_G_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_A_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_A_Sharp:
      result = msrSemiTonesPitchKind::kSTP_B_Flat;
      break;
    case msrSemiTonesPitchKind::kSTP_B_Flat:
      result = msrSemiTonesPitchKind::kSTP_A_Sharp;
      break;

    case msrSemiTonesPitchKind::kSTP_B_Natural:
      break;

    case msrSemiTonesPitchKind::kSTP_B_Sharp:
      result = msrSemiTonesPitchKind::kSTP_C_Natural;
      break;

    default:
      ;
  } // switch

  return result;
}


}

