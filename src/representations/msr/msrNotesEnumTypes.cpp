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

#include "oahWae.h"
#include "msrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrArticulations.h"
#include "msrGlissandos.h"
#include "msrPitchesNames.h"
#include "msrSlides.h"
#include "msrTechnicals.h"

// #include "msrNotes.h"

#include "oahOah.h"

#include "msrOah.h"

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
    case msrNoteKind::kNote_NO_:
      result = "***kNote_NO_***";
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


}

