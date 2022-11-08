/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrPitches.h"


namespace MusicFormats
{

//______________________________________________________________________________
string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrDiatonicPitchKind& elt)
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

string msrAlterationKindAsString (
  msrAlterationKind alterationKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrAlterationKind& elt)
{
  os << msrAlterationKindAsString (elt);
  return os;
}

string msrAlterationPreferenceKindAsString (
  msrAlterationPreferenceKind alterationPreferenceKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrAlterationPreferenceKind& elt)
{
  os << msrAlterationPreferenceKindAsString (elt);
  return os;
}

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

