/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "visitor.h"

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrArticulations.h"
#include "msrGlissandos.h"
#include "msrPitchesNames.h"
#include "msrSlides.h"
#include "msrTechnicals.h"

#include "oahOah.h"
#include "msrOah.h"

#include "oahWae.h"
#include "msrWae.h"

#include "oahEarlyOptions.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

// notes
//______________________________________________________________________________
std::string msrNoteKindAsString (
  msrNoteKind noteKind)
{
  std::string result;

  switch (noteKind) {
    case msrNoteKind::kNote_UNKNOWN:
      result = "kNote_UNKNOWN";
      break;

    // in measures
    case msrNoteKind::kNoteRegularInMeasure:
      result = "kNoteRegularInMeasure";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      result = "kNoteRestInMeasure";
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      result = "kNoteSkipInMeasure";
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = "kNoteUnpitchedInMeasure";
      break;

    // in chords
    case msrNoteKind::kNoteRegularInChord:
      result = "kNoteRegularInChord";
      break;

    // in tuplets
    case msrNoteKind::kNoteRegularInTuplet:
      result = "kNoteRegularInTuplet";
      break;

    case msrNoteKind::kNoteRestInTuplet:
      result = "kNoteRestInTuplet";
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      result = "kNoteUnpitchedInTuplet";
      break;

    // in grace notes groups
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      result = "kNoteRegularInGraceNotesGroup";
      break;
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      result = "kNoteSkipInGraceNotesGroup";
      break;

    // in chords in grace notes groups
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      result = "kNoteInChordInGraceNotesGroup";
      break;

    // in tuplets in grace notes groups
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      result = "kNoteInTupletInGraceNotesGroup";
      break;

    // in double-tremolos
    case msrNoteKind::kNoteInDoubleTremolo:
      result = "kNoteInDoubleTremolo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNoteKind& elt)
{
  os << msrNoteKindAsString (elt);
  return os;
}

// solo notes or rests
//______________________________________________________________________________

std::string msrSoloNoteOrRestInVoiceKindAsString (
  msrSoloNoteOrRestInVoiceKind soloNoteOrRestInVoiceKind)
{
  std::string result;

  switch (soloNoteOrRestInVoiceKind) {
    case msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceYes:
      result = "kSoloNoteOrRestInVoiceYes";
      break;
    case msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceNo:
      result = "kSoloNoteOrRestInVoiceNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSoloNoteOrRestInVoiceKind& elt)
{
  os << msrSoloNoteOrRestInVoiceKindAsString (elt);
  return os;
}

std::string msrSoloNoteOrRestInStaffKindAsString (
  msrSoloNoteOrRestInStaffKind soloNoteOrRestInStaffKind)
{
  std::string result;

  switch (soloNoteOrRestInStaffKind) {
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffYes:
      result = "kSoloNoteOrRestInStaffYes";
      break;
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffPartially:
      result = "kSoloNoteOrRestInStaffPartially";
      break;
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo:
      result = "kSoloNoteOrRestInStaffNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSoloNoteOrRestInStaffKind& elt)
{
  os << msrSoloNoteOrRestInStaffKindAsString (elt);
  return os;
}

// note heads
//______________________________________________________________________________
std::string msrNoteHeadKindAsString (
  msrNoteHeadKind noteHeadKind)
{
  std::string result;

  switch (noteHeadKind) {
    case msrNoteHeadKind::kNoteHeadSlash:
      result = "kNoteHeadSlash";
      break;
    case msrNoteHeadKind::kNoteHeadTriangle:
      result = "kNoteHeadTriangle";
      break;
    case msrNoteHeadKind::kNoteHeadDiamond:
      result = "kNoteHeadDiamond";
      break;
    case msrNoteHeadKind::kNoteHeadSquare:
      result = "kNoteHeadSquare";
      break;
    case msrNoteHeadKind::kNoteHeadCross:
      result = "kNoteHeadCross";
      break;
    case msrNoteHeadKind::kNoteHeadX:
      result = "kNoteHeadX";
      break;
    case msrNoteHeadKind::kNoteHeadCircleX:
      result = "kNoteHeadCircleX";
      break;
    case msrNoteHeadKind::kNoteHeadInvertedTriangle:
      result = "kNoteHeadInvertedTriangle";
      break;
    case msrNoteHeadKind::kNoteHeadArrowDown:
      result = "kNoteHeadArrowDown";
      break;
    case msrNoteHeadKind::kNoteHeadArrowUp:
      result = "kNoteHeadArrowUp";
      break;
    case msrNoteHeadKind::kNoteHeadSlashed:
      result = "kNoteHeadSlashed";
      break;
    case msrNoteHeadKind::kNoteHeadBackSlashed:
      result = "kNoteHeadBackSlashed";
      break;
    case msrNoteHeadKind::kNoteHeadNormal:
      result = "msrNoteHeadKind::kNoteHeadNormal";
      break;
    case msrNoteHeadKind::kNoteHeadCluster:
      result = "kNoteHeadCluster";
      break;
    case msrNoteHeadKind::kNoteHeadCircleDot:
      result = "kNoteHeadCircleDot";
      break;
    case msrNoteHeadKind::kNoteHeadLeftTriangle:
      result = "kNoteHeadLeftTriangle";
      break;
    case msrNoteHeadKind::kNoteHeadRectangle:
      result = "kNoteHeadRectangle";
      break;
    case msrNoteHeadKind::kNoteHeadNone:
      result = "kNoteHeadNone";
      break;
    case msrNoteHeadKind::kNoteHeadDo:
      result = "kNoteHeadDo";
      break;
    case msrNoteHeadKind::kNoteHeadRe:
      result = "kNoteHeadRe";
      break;
    case msrNoteHeadKind::kNoteHeadMi:
      result = "kNoteHeadMi";
      break;
    case msrNoteHeadKind::kNoteHeadFa:
      result = "kNoteHeadFa";
      break;
    case msrNoteHeadKind::kNoteHeadFaUp:
      result = "kNoteHeadFaUp";
      break;
    case msrNoteHeadKind::kNoteHeadSo:
      result = "kNoteHeadSo";
      break;
    case msrNoteHeadKind::kNoteHeadLa:
      result = "kNoteHeadLa";
      break;
    case msrNoteHeadKind::kNoteHeadTi:
      result = "kNoteHeadTi";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNoteHeadKind& elt)
{
  os << msrNoteHeadKindAsString (elt);
  return os;
}

std::string msrNoteHeadFilledKindAsString (
  msrNoteHeadFilledKind msrNoteHeadFilledKind)
{
  std::string result;

  switch (msrNoteHeadFilledKind) {
    case msrNoteHeadFilledKind::kNoteHeadFilledYes:
      result = "kNoteHeadFilledYes";
      break;
    case msrNoteHeadFilledKind::kNoteHeadFilledNo:
      result = "kNoteHeadFilledNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNoteHeadFilledKind& elt)
{
  os << msrNoteHeadFilledKindAsString (elt);
  return os;
}

std::string msrNoteHeadParenthesesKindAsString (
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind)
{
  std::string result;

  switch (msrNoteHeadParenthesesKind) {
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesYes:
      result = "kNoteHeadParenthesesYes";
      break;
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo:
      result = "kNoteHeadParenthesesNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNoteHeadParenthesesKind& elt)
{
  os << msrNoteHeadParenthesesKindAsString (elt);
  return os;
}

std::string msrNoteIsACueNoteKindAsString (
  msrNoteIsACueNoteKind msrNoteIsACueNoteKind)
{
  std::string result;

  switch (msrNoteIsACueNoteKind) {
    case msrNoteIsACueNoteKind::kNoteIsACueNoteYes:
      result = "kNoteIsACueNoteYes";
      break;
    case msrNoteIsACueNoteKind::kNoteIsACueNoteNo:
      result = "kNoteIsACueNoteNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNoteIsACueNoteKind& elt)
{
  os << msrNoteIsACueNoteKindAsString (elt);
  return os;
}


}

