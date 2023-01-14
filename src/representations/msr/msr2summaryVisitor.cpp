/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrTempos.h"

#include "oahOah.h"
#include "msrOah.h"

#include "msr2summaryVisitor.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//________________________________________________________________________
msr2summaryVisitor::msr2summaryVisitor (
  const S_msrOahGroup& msrOpts)
{
  fMsrOahGroup = msrOpts;

  // part groups
  fPartGroupsCounter = 0;

  // parts
  fPartsCounter = 0;

  // staves
  fStavesCounter = 0;
  fOnGoingStaff = false;

  // voices
  fVoicesCounter = 0;

  // stanzas
  fStanzasCounter = 0;

  // notes
  fScoreUnpitchedNotesCounter     = 0;
  fScoreRegularNotesCounter    = 0;
  fScoreRestNotesCounter          = 0;
  fScoreSkipNotesCounter          = 0;
  fScoreDoubleTremoloNotesCounter = 0;
  fScoreGraceNotesGroupCounter    = 0;

  // chords
  fScoreChordNotesCounter         = 0;
  fScoreChordsCounter             = 0;

  // tuplets
  fScoreTupletNotesCounter        = 0;
  fScoreTupletsCounter            = 0;
};

msr2summaryVisitor::~msr2summaryVisitor ()
{}

//________________________________________________________________________
void msr2summaryVisitor::printSummaryFromMsrScore (
  const S_msrScore& score)
{
  if (score) {
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*score);
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrScore& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      std::endl;
  }

  gLogStream <<
    "MSR contents summary of \"" <<
    gGlobalServiceRunData->getInputSourceName () <<
    "\":" <<
    std::endl << std::endl;

  ++gIndenter;
}

void msr2summaryVisitor::visitEnd (S_msrScore& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      std::endl;
  }

  gLogStream <<
    "The score contains:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 3;

  gLogStream <<
    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fPartGroupsCounter,
      "part group",
      "part groups") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fPartsCounter,
      "part",
      "parts") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fStavesCounter,
      "stave",
      "staves") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fVoicesCounter,
      "voice",
      "voices") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fStanzasCounter,
      "stanza",
      "stanzas") <<
    std::endl <<

    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      elt->getScoreNumberOfMeasures (),
      "measure",
      "measures") <<
    std::endl <<

    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreRegularNotesCounter,
      "standalone note",
      "standalone notes") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreRestNotesCounter,
      "rest note",
      "rest notes") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreSkipNotesCounter,
      "skip note",
      "skip notes") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreUnpitchedNotesCounter,
      "unpitched note",
      "unpitched notes") <<
    std::endl <<

    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreChordsCounter,
      "chord",
      "chords") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreChordNotesCounter,
      "chord note",
      "chord notes") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreTupletsCounter,
      "tuplet",
      "tuplets") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreTupletNotesCounter,
      "tuplet note",
      "tuplet notes") <<
    std::endl <<

    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreGraceNotesGroupCounter,
      "grace note group",
      "grace notes groups") <<
    std::endl <<

    std::setw (fieldWidth) <<
    mfSingularOrPlural (
      fScoreDoubleTremoloNotesCounter,
      "double tremolo note",
      "double tremolo notes") <<
    std::endl << std::endl;

  --gIndenter;
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrPartGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup" <<
      std::endl;
  }

  ++fPartGroupsCounter;

  size_t partGroupElementsSize = elt->getPartGroupElementsList ().size ();

  gLogStream <<
    "PartGroup" << ' ' << elt->getPartGroupCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 24;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "partGroupAbsoluteNumber" << ": \"" <<
    elt->getPartGroupAbsoluteNumber  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupName" << ": \"" <<
    elt->getPartGroupName  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupNameDisplayText" << ": \"" <<
    elt->getPartGroupNameDisplayText  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupAccidentalText" << ": \"" <<
    elt->getPartGroupAccidentalText  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupAbbrevation" << ": \"" <<
    elt->getPartGroupAbbreviation  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupSymbolKind" << ": " <<
    msrPartGroupSymbolKindAsString (
      elt->getPartGroupSymbolKind  ()) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupSymbolDefaultX" << ": " <<
    elt->getPartGroupSymbolDefaultX  () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupBarLine" << ": \"" <<
    msrPartGroupBarLineKindAsString (
      elt->getPartGroupBarLineKind ()) <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partGroupInstrumentName" << ": \"" <<
      elt->getPartGroupInstrumentName () << "\"" <<
    std::endl << std::endl;
}

void msr2summaryVisitor::visitEnd (S_msrPartGroup& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrPart& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart" <<
      std::endl;
  }

  ++fPartsCounter;

  size_t partStavesMapSize = elt->getPartStaveNumbersToStavesMap ().size ();

  gLogStream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    mfSingularOrPlural (
      partStavesMapSize,
      "staff", "staves") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 28;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "partID" << ": \"" <<
    elt->getPartID () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partMsrName" << ": \"" <<
    elt->getPartMsrName () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partName" << ": \"" <<
    elt->getPartName  () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partAbbrevation" << ": \"" <<
    elt->getPartAbbreviation () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partInstrumentName" << ": \"" <<
    elt->getPartInstrumentName () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partInstrumentAbbreviation" << ": \"" <<
    elt->getPartInstrumentAbbreviation () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "partNumberOfMeasures" << ": " <<
    elt->getPartNumberOfMeasures () <<
    std::endl << std::endl;
}

void msr2summaryVisitor::visitEnd (S_msrPart& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrStaff& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff" <<
      std::endl;
  }

  ++fStavesCounter;

  size_t staffAllVoicesVectorSize =
    elt->getStaffAllVoicesVector ().size ();

  gLogStream <<
    "Staff" << ' ' << elt->getStaffName () <<
    " contains " <<
    mfSingularOrPlural (
      staffAllVoicesVectorSize,
      "voice", "voices") <<
    std::endl;

  ++gIndenter;


  const int fieldWidth = 27;

  // print the staff number
  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "staffNumber" << ": " <<
    elt->getStaffNumber () <<
    std::endl <<

    std::setw (fieldWidth) <<
   "staffInstrumentName" << ": \"" <<
    elt->getStaffInstrumentName () << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
   "staffInstrumentAbbreviation" << ": \"" <<
    elt->getStaffInstrumentAbbreviation () << "\"" <<
    std::endl;

  gLogStream << std::endl;

  fOnGoingStaff = true;
}

void msr2summaryVisitor::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaff" <<
      std::endl;
  }

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrVoice& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice" <<
      std::endl;
  }

  ++fVoicesCounter;

  size_t voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();

  gLogStream <<
    "Voice" << ' ' << elt->getVoiceName () <<
    " has " <<
    mfSingularOrPlural (
      voiceStanzasMapSize,
      "stanza", "stanzas") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 34;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "voiceNumber" << ": " <<
    elt->getVoiceNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << ": " <<
    elt->getRegularVoiceStaffSequentialNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "voiceActualNotesCounter" << ": " <<
    elt->getVoiceActualNotesCounter () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "voiceActualHarmoniesCounter" << ": " <<
    elt->getVoiceActualHarmoniesCounter () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "musicHasBeenInsertedInVoice" << ": " <<
    elt->getMusicHasBeenInsertedInVoice () <<
    std::endl;

  gLogStream << std::endl;

  // don't show fVoiceMuteStanza in the summary
}

void msr2summaryVisitor::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrStanza& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStanza" <<
      std::endl;
  }

  ++fStanzasCounter;

  size_t syllablesSize = elt->getSyllables ().size ();

  gLogStream <<
    "Stanza" << ' ' << elt->getStanzaName () <<
    " contains " <<
    mfSingularOrPlural (
      syllablesSize,
      "syllable", "syllables");

  if (! elt->getStanzaTextPresent ()) {
    gLogStream <<
      " (no actual text)";
  }

  gLogStream <<
    std::endl << std::endl;
}

void msr2summaryVisitor::visitEnd (S_msrStanza& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStanza" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrSyllable& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSyllable" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrSyllable& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSyllable" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrClef& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrClef" <<
      std::endl;
  }

  if (! fOnGoingStaff) {
    gLogStream <<
      elt->asString () <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrClef& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrClef" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrKey& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrKey" <<
      std::endl;
  }

  if (! fOnGoingStaff) {
    gLogStream <<
      elt->asString () <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrKey& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrKey" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrTimeSignature& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTimeSignature" <<
      std::endl;
  }

  if (! fOnGoingStaff) {
    gLogStream <<
      elt->asString () <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrTimeSignature& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTimeSignature" <<
    std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTempo" <<
       std::endl;
  }

  gLogStream <<
    "Tempo " <<
    elt->getTempoBeatUnit () << ' ' << elt->getTempoPerMinute () <<
    std::endl;
}

void msr2summaryVisitor::visitEnd (S_msrTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTempo" <<
    std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrSegment& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegment" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrSegment& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSegment" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrArticulation& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArticulation" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrArticulation& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrArticulation" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrDynamic& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDynamic" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrDynamic& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrDynamic" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrCrescDecresc& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCrescDecresc" << std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrCrescDecresc& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCrescDecresc" << std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrWedge& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWedge" << std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrWedge& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWedge" << std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrGraceNotesGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGraceNotesGroup" << std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrGraceNotesGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGraceNotesGroup" << std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrNote& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting " <<
      msrNoteKindAsString (
        elt->getNoteKind ()) <<
      " msrNote" <<
      std::endl;
  }

  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNote_UNKNOWN:
      break;
    case msrNoteKind::kNoteRestInMeasure:
      ++fScoreRestNotesCounter;
      break;
    case msrNoteKind::kNoteSkipInMeasure:
      ++fScoreSkipNotesCounter;
      break;
    case msrNoteKind::kNoteUnpitchedInMeasure:
      ++fScoreUnpitchedNotesCounter;
      break;
    case msrNoteKind::kNoteRegularInMeasure:
      ++fScoreRegularNotesCounter;
      break;
    case msrNoteKind::kNoteInDoubleTremolo:
      ++fScoreRegularNotesCounter;
      break;
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ++fScoreGraceNotesGroupCounter;
      break;
    case msrNoteKind::kNoteRegularInChord:
      ++fScoreChordNotesCounter;
      break;
    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      ++fScoreTupletNotesCounter;
      break;
  } // switch
}

void msr2summaryVisitor::visitEnd (S_msrNote& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting " <<
      msrNoteKindAsString (
        elt->getNoteKind ()) <<
      " msrNote" <<
      std::endl;
  }

  switch (elt->getNoteKind ()) { // JMI
    case msrNoteKind::kNote_UNKNOWN:
      break;
    case msrNoteKind::kNoteRestInMeasure:
      break;
    case msrNoteKind::kNoteSkipInMeasure:
      break;
    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;
    case msrNoteKind::kNoteRegularInMeasure:
      break;
    case msrNoteKind::kNoteInDoubleTremolo:
      break;
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;
    case msrNoteKind::kNoteRegularInChord:
      break;
    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrStem& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStem" << std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrStem& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStem" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrBeam& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBeam" << std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrBeam& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBeam" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrChord& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChord" <<
      std::endl;
  }

  ++fScoreChordsCounter;
}

void msr2summaryVisitor::visitEnd (S_msrChord& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChord" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrTuplet& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTuplet" <<
      std::endl;
  }

  ++fScoreTupletsCounter;
}

void msr2summaryVisitor::visitEnd (S_msrTuplet& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTuplet" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrSlur& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlur" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrSlur& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlur" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrBarLine& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarLine" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrBarLine& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarLine" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarNumberCheck" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarNumberCheck" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrLineBreak& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLineBreak" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrLineBreak& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLineBreak" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrRepeat& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeat" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrRepeat& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeat" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrScaling& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScaling" <<
      std::endl;
  }

  gLogStream <<
    elt;
}

void msr2summaryVisitor::visitEnd (S_msrScaling& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScaling" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrPageLayout& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageLayout" <<
      std::endl;
  }

  gLogStream << elt << std::endl;
}

void msr2summaryVisitor::visitEnd (S_msrPageLayout& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageLayout" <<
      std::endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrMidiTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMidiTempo" <<
      std::endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrMidiTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMidiTempo" <<
      std::endl;
  }
}


} // namespace
