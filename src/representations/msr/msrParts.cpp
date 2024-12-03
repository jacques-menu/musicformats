/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // std::setw, std::setprecision, ...
#include <algorithm>    // for_each

#include "visitor.h"

#include "mfPreprocessorSettings.h"
#include "mfConstants.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrBarLines.h"
#include "msrBreaks.h"
#include "msrParts.h"
#include "msrRehearsalMarks.h"
#include "msrTempos.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// global variable
int msrPart::sPartsCounter = 0;

S_msrPart msrPart::create (
  int                   inputLineNumber,
  const std::string&    partID,
  const S_msrPartGroup& partUpLinkToPartGroup)
{
  msrPart* obj =
    new msrPart (
      inputLineNumber,
      partID,
      partUpLinkToPartGroup);
  assert (obj != nullptr);
  return obj;
}

msrPart::msrPart (
  int                   inputLineNumber,
  const std::string&    partID,
  const S_msrPartGroup& partUpLinkToPartGroup)
    : msrPartGroupElement (inputLineNumber)
{
  // replace spaces in partID to set fPartID
  for_each (
    partID.begin (),
    partID.end (),
    mfStringSpaceReplacer (fPartID, '_'));

/* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partUpLinkToPartGroup != nullptr,
    "partUpLinkToPartGroup is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  // set part number
  fPartAbsoluteNumber = ++sPartsCounter;

  // set part's part group upLink
  fPartUpLinkToPartGroup = partUpLinkToPartGroup;

  // do other initializations
  initializePart ();
}

void msrPart::initializePart ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Creating part " << asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the MSR part renaming map?
  std::map <std::string, std::string>::const_iterator
    it =
      gMsrOahGroup->getMsrPartsRenamingMap ().find (fPartID);

  if (it != gMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    std::string newMsrPartName = (*it).second;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceParts ()) {
      std::stringstream ss;

      ss <<
        "Renaming MSR part " <<
        fetchPartCombinedName () <<
        " to \"" <<
        newMsrPartName <<
        "\"";

      msrWarning (
        gServiceRunData->getInputSourceName (),
        fInputStartLineNumber,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPartMsrName = newMsrPartName;
  }

  else {
    // coin the names from the argument
    fPartMsrName =
      "Part_" + fPartID;
    fPartAlphabeticName =
      "Part_" + mfStringNumbersToEnglishWords (fPartID);
  }

  // time signature is crucially needed for measure positions determination:
  // we cannot stay without any,
  // and there may be none in the MusicXML data, so:

  // set a default 4/4 time
  fPartCurrentTimeSignature =
    msrTimeSignature::createFourQuartersTime (
      0); // inputLineNumber

  // initialize part's number of regular voices
  fPartRegularVoicesCounter = 0;

  fPartMinimumVoiceNumber = INT_MAX;
  fPartMaximumVoiceNumber = 0;

  // initialize part's number of measures
  fPartNumberOfMeasures = 0;

  // multi-measure rests
  fPartContainsMultiMeasureRests = false;

  // drawing measure position
  fPartCurrentDrawingMeasurePosition = msrWholeNotes (0, 1);

  // part shortest note wholeNotes
  fPartShortestNoteWholeNotes = msrWholeNotes (INT_MAX, 1);

  // part shortest note tuplet factor
  fPartShortestNoteTupletFactor = mfRational (1, 1);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "The created part contains: " << asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrPart::~msrPart ()
{}

S_msrScore  msrPart::fetchPartUpLinkToScore () const
{
  S_msrScore result;

  if (fPartUpLinkToPartGroup) {
    result =
      fPartUpLinkToPartGroup->
        getPartGroupUpLinkToScore ();
  }

  return result;
}

S_msrPart msrPart::createPartNewbornClone (const S_msrPartGroup& partGroupClone)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partGroupClone != nullptr,
    "partGroupClone is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrPart
    newbornClone =
      msrPart::create (
        fInputStartLineNumber,
        fPartID,
        partGroupClone);

  newbornClone->fPartMsrName =
    fPartMsrName;

  newbornClone->fPartName =
    fPartName;
  newbornClone->fPartAlphabeticName =
    fPartAlphabeticName;

  newbornClone->fPartNameDisplayText =
    fPartNameDisplayText;

  newbornClone->fPartAbbreviation =
    fPartAbbreviation;
  newbornClone->fPartAbbreviationDisplayText =
    fPartAbbreviationDisplayText;

  newbornClone->fPartInstrumentName =
    fPartInstrumentName;
  newbornClone->fPartInstrumentAbbreviation =
    fPartInstrumentAbbreviation;

  // set the newbornClone's number of measures
  newbornClone->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);

  return newbornClone;
}

void msrPart::registerStaffInPart (
  const S_msrStaff& staff)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Registering staff \"" <<
      staff->getStaffName () <<
      ", \"" <<
      staff->getStaffKind () <<
      "\" under number " <<
      staff->getStaffNumber () <<
      " in part " <<
      fetchPartCombinedName () <<
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register staff in the all staves list
  fPartAllStavesList.push_back (staff);

  // the staves may be regitered in disorder, for example in mxml2msr,
  // so we have to sort them
//   sortStavesByIncreasingNumber (); // JMI v0.9.72

  // register staff number in the staves numbers to staves map
  fPartStavesMap [staff->getStaffNumber ()] = staff;

  // register staff in the staves and voices bidimensional vector
//   fPartStavesAndVoicesVector [staff->getStaffNumber ()] = staff;

  // register staff in adhoc staves lists
  switch (staff->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
      // register regular staff in the regular staves list
      fPartRegularStavesList.push_back (staff);

      // register regular staff in the
      // non harmonies nor figured bass staves list
      fPartNonHarmoniesNorFiguredBassStavesList.push_back (staff);
      break;

    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      // register staff in the
      // non harmonies nor figured bass staves list
      fPartNonHarmoniesNorFiguredBassStavesList.push_back (staff);
      break;

    case msrStaffKind::kStaffKindHarmonies:
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      if (fPartHarmoniesStaff) {
        std::stringstream ss;

        ss <<
          "a harmonies staff already exists in part " <<
          fetchPartCombinedName () <<
          ", line " << staff->getInputStartLineNumber ();

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          staff->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      // register harmonies staff
      fPartHarmoniesStaff = staff;
      break;

    case msrStaffKind::kStaffKindFiguredBass:
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      if (fPartFiguredBassStaff) {
        std::stringstream ss;

        ss <<
          "a figured bass staff already exists in part " <<
          fetchPartCombinedName () <<
          ", line " << staff->getInputStartLineNumber ();

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          staff->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      // register figured bass staff
      fPartFiguredBassStaff = staff;
      break;
  } // switch
}

void msrPart::setPartCurrentDrawingMeasurePosition (
  int                  inputLineNumber,
  const msrWholeNotes& measurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Setting the drawing measure position in part " <<
      fetchPartCombinedName () <<
      " to " <<
      measurePosition <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (measurePosition.getNumerator () < 0) {
    std::stringstream ss;

    ss <<
      "cannot set part current measure position to " <<
      measurePosition <<
      " in part " <<
      fetchPartCombinedName () <<
      " since it is negative" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fPartCurrentDrawingMeasurePosition = measurePosition;
}

void msrPart::resetPartCurrentDrawingMeasurePosition (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Resetting part drawing measure position to 0 in part " <<
      fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartCurrentDrawingMeasurePosition = msrWholeNotes (0, 1);
}

void msrPart::incrementPartCurrentDrawingMeasurePosition (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotesDelta)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Incrementing the drawing measure position in part " <<
      fetchPartCombinedName () <<
      " by " <<
      wholeNotesDelta <<
      ", fPartCurrentDrawingMeasurePosition: " <<
      fPartCurrentDrawingMeasurePosition <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartCurrentDrawingMeasurePosition += wholeNotesDelta;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "The new part drawing measure position is " <<
      "fPartCurrentDrawingMeasurePosition: " <<
      fPartCurrentDrawingMeasurePosition <<
      " in part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrPart::decrementPartCurrentDrawingMeasurePosition (
  int                  inputLineNumber,
  const msrWholeNotes& wholeNotesDelta)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Decrementing the drawing measure position in part " <<
      fetchPartCombinedName () <<
      " by " <<
      wholeNotesDelta <<
      ", fPartCurrentDrawingMeasurePosition: " <<
      fPartCurrentDrawingMeasurePosition <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fPartCurrentDrawingMeasurePosition.getNumerator () < 0) {
    std::stringstream ss;

    ss <<
      "cannot decrement part current measure position by " <<
      wholeNotesDelta <<
      " in part " <<
      fetchPartCombinedName () <<
      " since that sets it to " <<
      fPartCurrentDrawingMeasurePosition <<
      ", which is negative " <<
      ", line " << inputLineNumber;

//     msrInternalError (
    msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
//       __FILE__, __LINE__,
      ss.str ());
  }

  fPartCurrentDrawingMeasurePosition -= wholeNotesDelta;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "The new part drawing measure position is " <<
      "fPartCurrentDrawingMeasurePosition: " <<
      fPartCurrentDrawingMeasurePosition <<
      " in part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrPart::setPartShortestNoteWholeNotes (
  const msrWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gMsrOahGroup->getTraceMsrNotesDurations ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the shortest note wholeNotes of part \"" <<
      fPartName <<
      "\" to " <<
      wholeNotes.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartShortestNoteWholeNotes = wholeNotes;
}

void msrPart::setPartShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceNotes ()
      ||
    gMsrOahGroup->getTraceMsrNotesDurations ()
  ) {
    std::stringstream ss;

    ss <<
      "Setting the shortest note tuplet factor of part " <<
      fPartName <<
      " to " <<
      noteTupletFactor;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartShortestNoteTupletFactor = noteTupletFactor;
}

void msrPart::assignSequentialNumbersToRegularVoicesInPart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Assigning sequential numbers to the staves in part \"" <<
      fPartID <<
      ", \"" <<
      fPartName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      assignSequentialNumbersToRegularVoicesInStaff (
        inputLineNumber);
  } // for
}

void msrPart::setPartMsrName (const std::string& partMsrName)
{
  // is this part name in the part renaming map?
  std::map <std::string, std::string>::const_iterator
    it =
      gMsrOahGroup->getMsrPartsRenamingMap ().find (fPartMsrName);

  if (it != gMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceParts ()) {
      std::stringstream ss;

        ss <<
        "Setting part name of " << fetchPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceParts ()) {
      std::stringstream ss;

        ss <<
        "Keeping partID \"" << partMsrName <<
        "\" as part name  for " << fetchPartCombinedName () <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

std::string msrPart::getPartIDAndName () const
{
  std::stringstream ss;

  ss << '[';

  if (! fPartMsrName.size ()) {
    ss <<
      "empty name";
  }
  else {
    ss <<
      fPartMsrName;
  }

  ss <<
    " \"" << fPartID << "\"";

  if (fPartName.size ()) {
    ss <<
      ", \"" << fPartName << "\"";
  }

  ss <<
    ']';

  return ss.str ();
}

std::string msrPart::fetchPartCombinedName () const
{
  std::stringstream ss;

  ss << '[';

  if (! fPartMsrName.size ()) {
    ss <<
      "empty part name";
  }
  else {
    ss <<
      fPartMsrName;
  }

  ss <<
    ", partID: \"" << fPartID << "\"";

  if (fPartName.size ()) {
    ss <<
      ", partName: \"" << fPartName << "\"";
  }

  ss <<
    ']';

  return ss.str ();
}

void msrPart::createAMeasureAndAppendItToPart (
  int                inputLineNumber,
  int                previousMeasureEndInputLineNumber,
  const std::string& measureNumber,
  msrMeasureImplicitKind
                     measureImplicitKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to part " <<
      fetchPartCombinedName () <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set part current measure number
  fPartCurrentMeasureNumber = measureNumber;

  // create and append measure in all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      createAMeasureAndAppendItToStaff (
        inputLineNumber,
        previousMeasureEndInputLineNumber,
        measureNumber,
        measureImplicitKind);
  } // for

  --gIndenter;
}

void msrPart::setNextMeasureNumberInPart (
  int                inputLineNumber,
  const std::string& nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in part " <<
      fetchPartCombinedName () <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set next measure number in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      setNextMeasureNumberInStaff (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  --gIndenter;
}

msrWholeNotes msrPart::fetchPartMeasuresWholeNotesVectorAt (
  int inputLineNumber,
  int indexValue) const
{
  msrWholeNotes result;

  size_t
    partMeasuresWholeNotesVectorSize =
      fPartMeasuresWholeNotesVector.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceWholeNoteDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositionsDetails ()
  ) {
    std::stringstream ss;

    ss <<
      "fetchPartMeasuresWholeNotesVectorAt() in part \"" <<
      fetchPartCombinedName () <<
      "\"" <<
      ", partMeasuresWholeNotesVectorSize: " <<
      partMeasuresWholeNotesVectorSize <<
      ", indexValue: " << indexValue;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // has this measureOrdinalNumber been registered already?
  try {
    msrWholeNotes
      currentWholeNotesValue =
        fPartMeasuresWholeNotesVector.at (indexValue);

    // yes
    result = currentWholeNotesValue;
  }

  catch (const std::out_of_range& e) {
    // no

    std::stringstream ss;

    ss <<
      "fetchPartMeasuresWholeNotesVectorAt() in part \"" <<
      fetchPartCombinedName () <<
      "\"" <<
      ", partMeasuresWholeNotesVectorSize: " <<
      partMeasuresWholeNotesVectorSize <<
      ", indexValue: " << indexValue << " is out of bounds" <<
      ", line " << inputLineNumber;

//     msrInternalWarningithLocationsDetails (
     msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());

      result = msrWholeNotes (15, 8); // TEMP JMI v0.9.61
  }

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceWholeNoteDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositionsDetails ()
  ) {
    std::stringstream ss;

    ss <<
      "fetchPartMeasuresWholeNotesVectorAt() returns \"" <<
      result.asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

void msrPart::registerShortestNoteInPartIfRelevant (const S_msrNote& note)
{
  // is note the shortest one in this part?
  msrWholeNotes
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

/* JMI
  msrWholeNotes
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();
      */

  if (noteSoundingWholeNotes < fPartShortestNoteWholeNotes) {
    // set the part shortest note wholeNotes
    this ->
      setPartShortestNoteWholeNotes (
        noteSoundingWholeNotes);

/* JMI
    // set the part shortest note tuplet factor // JMI ???
    fPartShortestNoteTupletFactor =
      note->
        getNoteTupletFactor ();

    this ->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
*/

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

        ss <<
        "The new shortest note in part \"" << fetchPartCombinedName () << "\"" <<
        " becomes " << note->asString ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }

/* JMI
  if (noteDisplayWholeNotes < fVoiceShortestNoteWholeNotes) {
    fVoiceShortestNoteWholeNotes = noteDisplayWholeNotes;
  }
  */
}

void msrPart::setPartNumberOfMeasures (size_t partNumberOfMeasures)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting the number of measures in part " <<
      fetchPartCombinedName () <<
      " to " <<
      partNumberOfMeasures <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  fPartNumberOfMeasures = partNumberOfMeasures;

  size_t
    fPartMeasuresWholeNotesVectorSize =
      fPartMeasuresWholeNotesVector.size ();

  if (partNumberOfMeasures > fPartMeasuresWholeNotesVectorSize) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "Resizing fPartMeasuresWholeNotesVector in part " << // JMI ???
        fetchPartCombinedName () <<
        " from " <<
        fPartMeasuresWholeNotesVectorSize <<
        " to " <<
        partNumberOfMeasures <<
        " measures";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPartMeasuresWholeNotesVector.clear ();
    fPartMeasuresWholeNotesVector.resize (
      fPartNumberOfMeasures,
      msrWholeNotes (0, 1));
  }
}

void msrPart::registerOrdinalMeasureNumberWholeNotes (
  int                  inputLineNumber,
  int                  measureOrdinalNumber,
  const msrWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Registering the whole notes duration of the measure with ordinal number '" <<
      measureOrdinalNumber <<
      "' as " <<
      wholeNotes.asString () <<
      " in part " << fetchPartCombinedName () <<
      ", measureOrdinalNumber: " << measureOrdinalNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresWholeNotesVectors ()) {
    printPartMeasuresWholeNotesVector (
      gLog,
      40,
      ", registerOrdinalMeasureNumberWholeNotes() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  size_t index = measureOrdinalNumber - 1;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresWholeNotesVectors ()) {
    std::stringstream ss;

    ss <<
      "measureOrdinalNumber: " << measureOrdinalNumber <<
      ", index: " << index <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // has this measureOrdinalNumber been registered already?
  try {
    msrWholeNotes
      currentWholeNotesValue =
        fPartMeasuresWholeNotesVector.at (index);

    // yes

    // allow for polymetrics in non-MusicXML contexts? JMI
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasures ()) {
      std::stringstream ss;

      ss <<
        "The measure with ordinal number " <<
        measureOrdinalNumber <<
        " was known with a whole notes duration of " <<
        currentWholeNotesValue.asString () <<
        ", now registering it with a duration of " <<
        wholeNotes.asString () <<
        " in part " << fetchPartCombinedName ();

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPartMeasuresWholeNotesVector [index] =
      wholeNotes;
  }

  catch (const std::out_of_range& e) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceMeasures ()) { // JMI v0.9.67
//       std::stringstream ss;
//
//         ss <<
//         "The measure with ordinal number " <<
//         measureOrdinalNumber <<
//         " is now registered with a duration of " <<
//         wholeNotes.asString () <<
//         " in part " << fetchPartCombinedName () <<
//         ", fPartMeasuresWholeNotesVector.size (): " <<
//         fPartMeasuresWholeNotesVector.size () <<
//         std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
  }

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceWholeNoteDurations ()
      ||
    gTraceOahGroup->getTraceMeasurePositionsDetails ()
  ) {
    printPartMeasuresWholeNotesVector (
      gLog,
      40,
      ", registerOrdinalMeasureNumberWholeNotes() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrPart::appendStaffDetailsToPart (
  const S_msrStaffDetails& staffDetails)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesDetails ()) {
    std::stringstream ss;

    ss <<
      "Appending staff details\"" <<
      staffDetails->asShortString () <<
      "\" to part " << fetchPartCombinedName () <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // register staff details in part
  fCurrentPartStaffDetails = staffDetails;

  // append staff details to regular staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendStaffDetailsToStaff (
        staffDetails);
  } // for
}

// void msrPart::appendClefKeyTimeSignatureGroupToPart (
//   const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceClefs ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending clefKeyTimeSignatureGroup " <<
//       clefKeyTimeSignatureGroup->asString () <<
//       " to part " << fetchPartCombinedName () <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // append clefKeyTimeSignatureGroup to part-level staves if any
//   if (fPartHarmoniesStaff) {
//     fPartHarmoniesStaff->
//       appendClefKeyTimeSignatureGroupToStaff (
//         clefKeyTimeSignatureGroup);
//   }
//   if (fPartFiguredBassStaff) {
//     fPartFiguredBassStaff->
//       appendClefKeyTimeSignatureGroupToStaff (
//         clefKeyTimeSignatureGroup);
//   }
//
//   // append clef to regular staves
//   for (S_msrStaff staff : fPartRegularStavesList) {
//     staff->
//       appendClefKeyTimeSignatureGroupToStaff (
//         clefKeyTimeSignatureGroup);
//   } // for
//
//   --gIndenter;
// }

void msrPart::appendClefToPart (
  int              groupInputLineNumber,
  const S_msrClef& clef)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    std::stringstream ss;

    ss <<
      "Appending clef " <<
      clef->asString () <<
      " to part " << fetchPartCombinedName () <<
      ", groupInputLineNumber: " << groupInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append clef to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendClefToStaff (
        groupInputLineNumber,
        clef);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendClefToStaff (
        groupInputLineNumber,
        clef);
  }

  // append clef to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendClefToStaff (
        groupInputLineNumber,
        clef);
  } // for

  --gIndenter;
}

void msrPart::appendKeyToPart (
  int             groupInputLineNumber,
  const S_msrKey& key)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Appending key " <<
      key->asString () <<
      " to part " << fetchPartCombinedName () <<
      ", groupInputLineNumber: " << groupInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set part key
  fPartCurrentKey = key;

  ++gIndenter;

  // append key to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendKeyToStaff (
        groupInputLineNumber,
        key);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendKeyToStaff (
        groupInputLineNumber,
        key);
  }

  // append key to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendKeyToStaff (
        groupInputLineNumber,
        key);
  } // for

  --gIndenter;
}

void msrPart::appendTimeSignatureToPart (
  int                       groupInputLineNumber,
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature " <<
      timeSignature->asString () <<
      " to part " << fetchPartCombinedName () <<
      ", groupInputLineNumber: " << groupInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set part time signature
  fPartCurrentTimeSignature = timeSignature;

  ++gIndenter;

  // append timeSignature to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendTimeSignatureToStaff (
        groupInputLineNumber,
        timeSignature);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendTimeSignatureToStaff (
        groupInputLineNumber,
        timeSignature);
  }

  // append timeSignature to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendTimeSignatureToStaff (
        groupInputLineNumber,
        timeSignature);
  } // for

  --gIndenter;
}

// void msrPart::appendTimeSignatureToPartClone (
//   const S_msrTimeSignature& timeSignature)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTimeSignatures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending time signature " <<
//       timeSignature
//       " to part clone " << fetchPartCombinedName () <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // set part time signature
//   fPartCurrentTimeSignature = timeSignature;
//
//   ++gIndenter;
//
//   // propagate it to regular staves
//   for (S_msrStaff staff : fPartRegularStavesList) {
//     staff->
//       appendTimeSignatureToStaffClone (timeSignature);
//   } // for
//
//   --gIndenter;
// }

void msrPart::appendClefKeyTimeSignatureGroupToPartClone (
  const S_msrClefKeyTimeSignatureGroup& clefKeyTimeSignatureGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending clefKeyTimeSignatureGroup " <<
      clefKeyTimeSignatureGroup->asString () <<
      " to part clone " << fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   // set part time signature
//   fPartCurrentTimeSignature = timeSignature;

  ++gIndenter;

  // propagate it to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendClefKeyTimeSignatureGroupToStaffClone (clefKeyTimeSignatureGroup);
  } // for

  --gIndenter;
}

void msrPart::appendTempoToPart (
  const S_msrTempo& tempo)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Appending tempo " << tempo->asString () <<
      " to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append tempo to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendTempoToStaff (tempo);
  } // for
}

void msrPart::appendRehearsalMarkToPart (
  const S_msrRehearsalMark& rehearsalMark)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRehearsalMarks ()) {
    std::stringstream ss;

    ss <<
      "Appending rehearsal mark " << rehearsalMark->asString () <<
      " to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append rehearsal mark to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendRehearsalMarkToStaff (rehearsalMark);
  } // for
}


void msrPart::appendLineBreakToPart (
  const S_msrLineBreak& lineBreak)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLineBreaks ()) {
    std::stringstream ss;

    ss <<
      "Appending line break " << lineBreak->asString () <<
      " to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append line break to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendLineBreakToStaff (lineBreak);
  } // for
}

void msrPart::appendPageBreakToPart (
  const S_msrPageBreak& pageBreak)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePageBreaks ()) {
    std::stringstream ss;

    ss <<
      "Appending page break " << pageBreak->asString () <<
      " to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append page break to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPageBreakToStaff (pageBreak);
  } // for
}

void msrPart::insertHiddenMeasureAndBarLineInPartClone (
  int             inputLineNumber,
  const msrWholeNotes& measurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition.asString () <<
      "' in part clone " << fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade it to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      insertHiddenMeasureAndBarLineInStaffClone (
        inputLineNumber,
        measurePosition);
  } // for

  --gIndenter;
}

void msrPart::appendTranspositionToPart (
  const S_msrTransposition& transposition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    std::stringstream ss;

    ss <<
      "Appending transposition \"" <<
      transposition->asString () <<
      "\" to part " << fetchPartCombinedName () <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // set part current transposition
  fPartCurrentTransposition = transposition;

  // cascade it to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendTranspositionToStaff (transposition);
  } // for
}

/* JMI
void msrPart::nestContentsIntoNewRepeatInPart (
  int inputLineNumber)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    (*i)->
      nestContentsIntoNewRepeatInStaff (
        inputLineNumber);
  } // for
}
*/

void msrPart::handleRepeatStartInPart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat start in part \"" <<
      fetchPartCombinedName () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatStartInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::handleRepeatEndInPart (
  int                inputLineNumber,
  const std::string& measureNumber,
  int                repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat end in part \"" <<
      fetchPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatEndInStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}

void msrPart::handleRepeatEndingStartInPart (
  int    inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a repeat ending start in part \"" <<
      fetchPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatEndingStartInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::handleRepeatEndingEndInPart (
  int                 inputLineNumber,
  const std::string&  repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Handling a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in part \"" <<
      fetchPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatEndingEndInStaff (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  --gIndenter;
}

/* JMI
void msrPart::finalizeRepeatEndInPart (
  int    inputLineNumber,
  const std::string& measureNumber,
  int    repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Finalizing a repeat upon its end in part \"" <<
      fetchPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create repeat and append it to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeRepeatEndInStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}
*/

void msrPart::appendRepeatCloneToPart (
  int                inputLineNumber,
  const S_msrRepeat& repeatCLone)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    switch (staff->getStaffKind ()) {
      case msrStaffKind::kStaffKindRegular:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case msrStaffKind::kStaffKindTablature:
        break;

      case msrStaffKind::kStaffKindHarmonies:
        break;

      case msrStaffKind::kStaffKindFiguredBass:
        break;

      case msrStaffKind::kStaffKindDrum:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case msrStaffKind::kStaffKindRythmic:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;
    } // switch
  } // for
}

void msrPart::appendRepeatEndingCloneToPart (
  const S_msrRepeatEnding& repeatEndingClone)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendRepeatEndingCloneToStaff (repeatEndingClone);
  } // for
}

void msrPart::createMeasureRepeatFromItsFirstMeasuresInPart (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  // create measures repeat from its first measure in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      createMeasureRepeatFromItsFirstMeasuresInStaff (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashes);
  } // for
}

void msrPart::appendPendingMeasureRepeatToPart (
  int inputLineNumber)
{
  // append pending measures repeat to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPendingMeasureRepeatToStaff (
        inputLineNumber);
  } // for
}

void msrPart::appendMultiMeasureRestToPart (
  int inputLineNumber,
  int multiMeasureRestsMeasuresNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending a multi-measure rest for " <<
      mfSingularOrPlural (
        multiMeasureRestsMeasuresNumber, "measure", "measures") <<
      " to part " <<
      fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartContainsMultiMeasureRests = true;

  // create multiple rest in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendMultiMeasureRestToStaff (
        inputLineNumber,
        multiMeasureRestsMeasuresNumber);
  } // for
}

void msrPart::replicateLastAppendedMeasureInPart (
  int inputLineNumber,
  int replicatasNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Replicating last appended measure in part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // add multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      replicateLastAppendedMeasureInStaff (
        inputLineNumber,
        replicatasNumber);
  } // for
}

void msrPart::appendEmptyMeasuresToPart (
  int                inputLineNumber,
  const std::string& previousMeasureNumber,
  int                measureRestsNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Adding " <<
      mfSingularOrPlural (
        measureRestsNumber, "multi-measure rest", "multi-measure rests") <<
      " to part " <<
      fetchPartCombinedName () <<
      ", ";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartContainsMultiMeasureRests = true;

  // add multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendEmptyMeasuresToStaff (
        inputLineNumber,
        previousMeasureNumber,
        measureRestsNumber);
  } // for
}

void msrPart::appendPendingMultiMeasureRestsToPart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending the pending multiple rest to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append pending multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPendingMultiMeasureRestsToStaff (
        inputLineNumber);
  } // for
}

void msrPart::appendMultiMeasureRestCloneToPart (
  int                          inputLineNumber,
  const S_msrMultiMeasureRest& multiMeasureRests)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending multiple rest '" <<
      multiMeasureRests->asString () <<
      "' to part clone " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendMultiMeasureRestCloneToStaff (
        inputLineNumber,
        multiMeasureRests);
  } // for
}

void msrPart::appendBarLineToPart (
  const S_msrBarLine& barLine)
{
 #ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBarLines ()) {
    std::stringstream ss;

    ss <<
      "Appending barLine " << barLine->asString () <<
      " to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append barLine to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendBarLineToStaff (barLine);
  } // for
}

S_msrStaff msrPart::addStaffToPartByItsNumber (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    std::stringstream ss;

    ss <<
      "staffNumber " << staffNumber <<
      " already exists in part " << fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());

    return fPartStavesMap [staffNumber];
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Adding " <<
      msrStaffKindAsString (staffKind) <<
      " staff " << staffNumber <<
      " to part " << fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        staffKind,
        staffNumber,
        this);

  // register staff in this part
  registerStaffInPart (
    staff);

  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindHarmonies:
    case msrStaffKind::kStaffKindFiguredBass:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch

  return staff;
}

S_msrStaff msrPart::addHarmoniesStaffToPart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Adding harmonies staff " <<
      " to part " << fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the staff
  int partHarmoniesStaffNumber =
    K_PART_HARMONIES_STAFF_NUMBER;

  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        msrStaffKind::kStaffKindHarmonies,
        partHarmoniesStaffNumber,
        this);

  // register staff in this part
  registerStaffInPart (
    staff);

/* JMI
  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindHarmonies:
    case msrStaffKind::kStaffKindFiguredBass:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch
*/

  return staff;
}

S_msrStaff msrPart::addHFiguredBassStaffToPart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Adding figured bass staff " <<
      " to part " << fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the staff
  int partFiguredBassStaffNumber =
    K_PART_FIGURED_BASS_STAFF_NUMBER;

  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        msrStaffKind::kStaffKindFiguredBass,
        partFiguredBassStaffNumber,
        this);

  // register staff in this part
  registerStaffInPart (
    staff);

/* JMI
  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindHarmonies:
    case msrStaffKind::kStaffKindFiguredBass:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch
*/

  return staff;
}

void msrPart::addStaffToPartCloneByItsNumber (const S_msrStaff& staff)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Adding staff \"" << staff->getStaffName () <<
      "\" to part clone " << fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register staff in this part
  registerStaffInPart (
    staff);
}

void msrPart::sortStavesByIncreasingNumber ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceStaves ()) {
    std::stringstream ss;

    ss <<
      "Sorting the staves in part " <<
      fetchPartCombinedName () <<
      ", " <<
      fPartID <<
      ", " <<
      fPartName <<
      " by increasing number";
//       ", line " << voice->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());

    print (gLog);

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  fPartAllStavesList.sort ( // JMI EVENTS
    msrStaff::compareStavesByIncreasingNumber);
//   fPartAllStavesList.sort ( // JMI EVENTS
//     msrStaff::compareStavesToHaveFiguredBassesBelowCorrespondingPart);

  // CAUTION: the non-harmony nor -figured bass voices list have to be sorted too
  fPartNonHarmoniesNorFiguredBassStavesList.sort ( // JMI EVENTS
    msrStaff::compareStavesByIncreasingNumber);
}

S_msrStaff msrPart::fetchStaffFromPart (
  int staffNumber)
{
  S_msrStaff result;

  if (fPartStavesMap.count (staffNumber)) {
    result =
      fPartStavesMap [staffNumber];
  }

  return result;
}

void msrPart::registerVoiceInPartVoicesList (
  const S_msrVoice& voice)
{
  // register voice in this part
  fPartVoicesList.push_back (voice);

  // set its regular voice ordinal number
  ++fPartRegularVoicesCounter;
  voice->
    setRegularVoiceOrdinalNumberInPart (
      fPartRegularVoicesCounter);
}

void msrPart::registerVoiceInPartStaffVoicesMap (
  const S_msrVoice& voice)
{
  int
    staffNumber =
      voice->getVoiceUpLinkToStaff ()->getStaffNumber (),
    voiceNumber =
      voice->getVoiceNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Registering voice " <<
      voiceNumber <<
      ", " <<
      voice->getVoiceName () <<
      "\" in staff " <<
      staffNumber <<
      ", " <<
      "\" in part " <<
      fPartID <<
      ", " <<
      fPartName <<
      ", line " << voice->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register voice in the all voices map
  if (fPartStaffVoicesMap.count (voiceNumber)) {
    std::stringstream ss;

    ss <<
      "Voice " <<
      voiceNumber <<
      ", " <<
      voice->getVoiceName () <<
      "\", has already been registered in the voices map of part \"" << // JMI v0.9.70
      getPartName () <<
      "\", line " << voice->getInputStartLineNumber ();

//     msrError (
    msrWarning (
      gServiceRunData->getInputSourceName (),
      voice->getInputStartLineNumber (),
//       __FILE__, __LINE__,
      ss.str ());
  }

  fPartStaffVoicesMap
    [staffNumber][voiceNumber] = voice;

  // register part minimum and maximum voice numbers
  if (voiceNumber < fPartMinimumVoiceNumber) {
    fPartMinimumVoiceNumber = voiceNumber;
  }
  if (voiceNumber > fPartMaximumVoiceNumber) {
    fPartMaximumVoiceNumber = voiceNumber;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
    displayPartStaffVoicesMap (
      voice->getInputStartLineNumber (),
      "msrPart::registerVoiceInPartVoicesList()");
  }
#endif
}

// void msrPart::registerVoiceInRegularVoicesMap (
//   const S_msrVoice& voice)
// {
//   int voiceNumber = voice->getVoiceNumber ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Registering regular voice " <<
//       voiceNumber <<
//       ", \"" <<
//       voice->getVoiceName () <<
//       "\" in part \"" <<
//       fPartID <<
//       ", \"" <<
//       fPartName <<
//       "\"" <<
//       ", line " << voice->getInputStartLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // register regular voice in the regular voices map
//   if (fPartRegularVoicesMap.count (voiceNumber)) {
//     std::stringstream ss;
//
//     ss <<
//       "Regular voice " <<
//       voiceNumber <<
//       ", \"" <<
//       voice->getVoiceName () <<
//       "\", has already been registered in the regular voices map of part \"" << // JMI v0.9.70
//       getPartName () <<
//       "\", line " << voice->getInputStartLineNumber ();
//
//     msrError (
//       gServiceRunData->getInputSourceName (),
//       voice->getInputStartLineNumber (),
//       __FILE__, __LINE__,
//       ss.str ());
//   }
//
//   fPartRegularVoicesMap [voiceNumber] = voice;
//
//   // register it in the partgroup uplink
//   fPartUpLinkToPartGroup->
//     registerVoiceInPartGroupAllVoicesList (voice);
// }

// void msrPart::registerVoiceInRegularVoicesMap (
//   const S_msrVoice& voice)
// {
//   int voiceNumber = voice->getVoiceNumber ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Registering regular voice " <<
//       voiceNumber <<
//       ", \"" <<
//       voice->getVoiceName () <<
//       "\" in part \"" <<
//       fPartID <<
//       ", \"" <<
//       fPartName <<
//       "\"" <<
//       ", line " << voice->getInputStartLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // register regular voice in the regular voices map
//   if (fPartRegularVoicesMap.count (voiceNumber)) {
//     std::stringstream ss;
//
//     ss <<
//       "Regular voice " <<
//       voiceNumber <<
//       ", \"" <<
//       voice->getVoiceName () <<
//       "\", has already been registered in the regular voices map of part \"" << // JMI v0.9.70
//       getPartName () <<
//       "\", line " << voice->getInputStartLineNumber ();
//
//     msrError (
//       gServiceRunData->getInputSourceName (),
//       voice->getInputStartLineNumber (),
//       __FILE__, __LINE__,
//       ss.str ());
//   }
//
//   fPartRegularVoicesMap [voiceNumber] = voice;
//
//   // register part minimum and maximum voice numbers
//   if (voiceNumber < fPartMinimumVoiceNumber) {
//     fPartMinimumVoiceNumber = voiceNumber;
//   }
//   if (voiceNumber > fPartMaximumVoiceNumber) {
//     fPartMaximumVoiceNumber = voiceNumber;
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
//     displayPartRegularVoicesMap (
//       voice->getInputStartLineNumber (),
//       "msrPart::registerVoiceInRegularVoicesMap()");
//   }
// #endif
// }

S_msrVoice msrPart::createPartHarmoniesVoice (
  int                inputLineNumber,
  const std::string& currentMeasureNumber)
{
  if (fPartHarmoniesVoice) {
    std::stringstream ss;

    ss <<
      "Part \"" <<
      fetchPartCombinedName () <<
      "\" already has a harmonies voice" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  // create the part harmonies staff
#ifdef MF_TRACE_IS_ENABLED
  int partHarmoniesStaffNumber =
    K_PART_HARMONIES_STAFF_NUMBER;

  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Creating harmonies staff for part \"" <<
      fetchPartCombinedName () <<
      "\" with staff number " <<
      partHarmoniesStaffNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesStaff =
    addHarmoniesStaffToPart (
      inputLineNumber);

  // create the part harmonies voice
  int partHarmoniesVoiceNumber =
    K_PART_HARMONIES_VOICE_NUMBER;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Creating harmonies voice for part \"" <<
      fetchPartCombinedName () <<
      "\" with voice number " <<
      partHarmoniesVoiceNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindHarmonies,
      partHarmoniesVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fPartHarmoniesStaff);

  // register the part harmonies voice in part harmonies staff
  fPartHarmoniesStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartHarmoniesVoice);

//* JMI v0.9.66
  // set backward link
 // ->
//     setHarmoniesVoicePartBackwardLink (fPartHarmoniesVoice);
//*/

  return fPartHarmoniesVoice;
}

void msrPart::appendHarmonyToPart (
  int                  inputLineNumber,
  const S_msrHarmony&  harmony,
  const msrWholeNotes& measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " <<
      harmony->asString () <<
      " to part " <<
      fetchPartCombinedName () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesVoice->
    appendHarmonyToVoice (
      inputLineNumber,
      harmony,
      measurePositionToAppendAt);
}

void msrPart::appendHarmoniesListToPart (
  int                            inputLineNumber,
  const std::list <S_msrHarmony>& harmoniesList,
  const msrWholeNotes&           measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmonies list to part " << // JMI v0.9.67 HARMFUL
      fetchPartCombinedName () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesVoice->
    appendHarmoniesListToVoice (
      inputLineNumber,
      harmoniesList,
      measurePositionToAppendAt);
}

void msrPart::appendFiguredBassesListToPart (
  int                                inputLineNumber,
  const std::list <S_msrFiguredBass>& figuredBasssesList,
  const msrWholeNotes&               measurePositionToAppendAt)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending figured basses list \"" <<
//       figuredBasssesList->asString () << // JMI v0.9.67 HARMFUL
      "\" to part " <<
      fetchPartCombinedName () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesVoice->
    appendFiguredBassesListToVoice (
      inputLineNumber,
      figuredBasssesList,
      measurePositionToAppendAt);
}

S_msrVoice msrPart::createPartFiguredBassVoice (
  int                inputLineNumber,
  const std::string& currentMeasureNumber)
{
  if (fPartFiguredBassVoice) {
    std::stringstream ss;

    ss <<
      "Part \"" <<
      fetchPartCombinedName () <<
      "\" already has a figured bass voice" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  // create the part figured bass staff
#ifdef MF_TRACE_IS_ENABLED
  int partFiguredBassStaffNumber =
    K_PART_FIGURED_BASS_STAFF_NUMBER;

  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating figured bass staff for part \"" <<
      fetchPartCombinedName () <<
      "\" with staff number " <<
      partFiguredBassStaffNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartFiguredBassStaff =
    addHFiguredBassStaffToPart (
      inputLineNumber);

  // create the figured bass voice
  int partFiguredBassVoiceNumber =
    K_PART_FIGURED_BASS_VOICE_NUMBER;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating figured bass voice for part \"" <<
      fetchPartCombinedName () <<
      "\" with voice number " <<
      partFiguredBassVoiceNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartFiguredBassVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindFiguredBass,
      partFiguredBassVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fPartFiguredBassStaff);

  // register the figured bass voice in the part figured bass staff
  fPartFiguredBassStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartFiguredBassVoice);

///* JMI v0.9.66
  // set backward link
//   ->
//     setFiguredBassVoicePartBackwardLink (fPartFiguredBassVoice);
//*/

  return fPartFiguredBassVoice;
}

void msrPart::appendFiguredBassToPart (
  int                     inputLineNumber,
  const S_msrFiguredBass& figuredBass,
  const msrWholeNotes&    measurePositionToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass \"" <<
      figuredBass->asString () <<
      "\" to part " <<
      fetchPartCombinedName () <<
      ", measurePositionToAppendAt: " << measurePositionToAppendAt <<
      ", line " << figuredBass->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartFiguredBassVoice->
    appendFiguredBassToVoice (
      inputLineNumber,
      figuredBass,
      measurePositionToAppendAt);
}

// void msrPart::appendFiguredBassToPart (
//   const S_msrVoice&       figuredBassSupplierVoice,
//   const S_msrFiguredBass& figuredBass)
// {
//   ++gIndenter;
//
//   switch (figuredBassSupplierVoice->getVoiceKind ()) {
//     case msrVoiceKind::kVoiceKindRegular:
//       // append the figured bass to the part figured bass voice
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTraceFiguredBasses ()) {
//         std::stringstream ss;
//
//     ss <<
//           "Appending figured bass " <<
//           figuredBass->asString () <<
//           " to part " <<
//           fetchPartCombinedName () <<
//           ", line " << figuredBass->getInputStartLineNumber () <<
//           std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, __LINE__,
//             ss.str (),
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       fPartFiguredBassVoice->
//         appendFiguredBassToVoice (
//           figuredBass->getInputStartLineNumber (),
//           figuredBass);
//       break;
//
//     case msrVoiceKind::kVoiceKindDynamics:
//       break;
//
//     case msrVoiceKind::kVoiceKindHarmonies:
//     case msrVoiceKind::kVoiceKindFiguredBass:
//       {
//         std::stringstream ss;
//
//         ss <<
//           "figured bass cannot by appended to part by " <<
//           msrVoiceKindAsString (
//             figuredBassSupplierVoice->getVoiceKind ()) <<
//           " voice \" " <<
//           figuredBassSupplierVoice->getVoiceName () <<
//           "\"" <<
//           ", line " << figuredBass->getInputStartLineNumber ();
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           figuredBass->getInputStartLineNumber (),
//           __FILE__, __LINE__,
//           ss.str ());
//       }
//       break;
//   } // switch
//
//   --gIndenter;
// }

void msrPart::appendFiguredBassToPartClone (
  const S_msrVoice&       figuredBassSupplierVoice,
  const S_msrFiguredBass& figuredBass)
{
  ++gIndenter;

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      // append the figured bass to the part figured bass voice
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceFiguredBasses ()) {
        std::stringstream ss;

    ss <<
          "Appending figured bass " <<
          figuredBass->asString () <<
          " to part clone " <<
          fetchPartCombinedName () <<
          ", line " << figuredBass->getInputStartLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fPartFiguredBassVoice->
        appendFiguredBassToVoiceClone (figuredBass);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        std::stringstream ss;

        ss <<
          "figured bass cannot by appended to part clone by " <<
          msrVoiceKindAsString (
            figuredBassSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          figuredBassSupplierVoice->getVoiceName () <<
          "\"" <<
          ", line " << figuredBass->getInputStartLineNumber ();

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          figuredBass->getInputStartLineNumber (),
          __FILE__, __LINE__,
          ss.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrPart::appendScordaturaToPart (
  const S_msrScordatura& scordatura)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendScordaturaToStaff (
        scordatura);
  } // for
}

void msrPart::appendAccordionRegistrationToPart (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendAccordionRegistrationToStaff (
        accordionRegistration);
  } // for
}

void msrPart::appendHarpPedalsTuningToPart (
  const S_msrHarpPedalsTuning&
    harpPedalsTuning)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to part " <<
      fetchPartCombinedName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendHarpPedalsTuningToStaff (
        harpPedalsTuning);
  } // for
}

void msrPart::addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded (
  const S_msrVoice&           graceNotesGroupOriginVoice,
  const S_msrGraceNotesGroup& skipGraceNotesGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceMeasures ()
      ||
    gTraceOahGroup->getTraceGraceNotes ()
      ||
    gTraceOahGroup->getTraceParts ()
    ) {
    std::stringstream ss;

    ss <<
      "addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded () in " <<
      fetchPartCombinedName () <<
      ", line " << skipGraceNotesGroup->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartAllStavesList) {
    std::map <int, S_msrVoice>
      staffAllVoicesMap =
        staff->
          getStaffVoiceNumbersToAllVoicesMap ();

    for (std::pair <int, S_msrVoice> thePair : staffAllVoicesMap) {
      S_msrVoice voice = thePair.second;

      if (voice != graceNotesGroupOriginVoice) {
        // add skip grace notes group ahead of voice
        voice->
          addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
            skipGraceNotesGroup);
      }
    } // for
  } // for
}

// void msrPart::handleBackupInPart (
//   int                  inputLineNumber,
//   const msrWholeNotes& backupStepLength)
// {
//   // account for backup in part
//   decrementPartCurrentDrawingMeasurePosition (
//     inputLineNumber,
//     backupStepLength);
// }

void msrPart::finalizeLastAppendedMeasureInPart (
  int    inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing last appended measure in part " <<
      fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // finalize current measure in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeLastAppendedMeasureInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::setPartInstrumentNamesMaxLengthes ()
{
  S_msrScore
    score =
      fPartUpLinkToPartGroup->
        getPartGroupUpLinkToScore ();

  size_t partInstrumentNameLength =
    fPartInstrumentName.size ();

  if (
    partInstrumentNameLength
      >
    score->getScoreInstrumentNamesMaxLength ()
  ) {
    score->
      setIdentificationScoreInstrumentNamesMaxLength (
        partInstrumentNameLength);
  }

  size_t partInstrumentAbbreviationLength =
    fPartInstrumentAbbreviation.size ();

  if (
    partInstrumentAbbreviationLength
      >
    score->getScoreInstrumentAbbreviationsMaxLength ()
  ) {
    score->
      setIdentificationScoreInstrumentAbbreviationsMaxLength (
        partInstrumentAbbreviationLength);
  }
}

void msrPart::displayPartStaffVoicesMap (
  int                inputLineNumber,
  const std::string& context) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    gLog <<
      ">>> The fPartStaffVoicesMap of part \"" <<
      getPartName () <<
      "\" , context: " << context <<
      ", contains:" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  size_t
    partStaffVoicesMapSize =
      fPartStaffVoicesMap.size ();

  gLog <<
    std::endl <<
    "fPartStaffVoicesMap contains " <<
    mfSingularOrPlural (
      partStaffVoicesMapSize, "stave", "staves") <<
    ", context: " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (partStaffVoicesMapSize) {
    ++gIndenter;

    for (
      std::pair <int, std::map <int, S_msrVoice>> primaryPair :
        fPartStaffVoicesMap
    ) {
      int
        staffNumber = primaryPair.first;

      for (
        std::pair <int, S_msrVoice> secondaryPair :
          primaryPair.second
      ) {
        int
          voiceNumber = secondaryPair.first;
        S_msrVoice
          voice = secondaryPair.second;

        gLog <<
          "staffNumber " << staffNumber <<
          ", voiceNumber " <<  voiceNumber <<
          ":" <<
          std::endl;

        ++gIndenter;

        gLog <<
          voice <<
          std::endl;

        --gIndenter;

        gLog << std::endl;
      } // for
    } // for

    --gIndenter;
  }

  gLog << std::endl;
}




// void msrPart::displayPartStavesAndVoicesVector (
//   int                inputLineNumber,
//   const std::string& context) const
// {
//   gLog <<
//     ">>> The fPartStavesAndVoicesVector of part " <<
//     getPartName () <<
//     " contains:" <<
//     std::endl;
//
//   ++gIndenter;
//
//   for (int staffIndex = 0; staffIndex < fPartStavesAndVoicesVector.size (); ++staffIndex) {
//     const std::vector <S_msrVoice>&
//       staffVoicesVector =
//         fPartStavesAndVoicesVector [staffIndex];
//
//     for (int voiceIndex = 0; voiceIndex < staffVoicesVector.size (); ++voiceIndex) {
//       S_msrVoicevoice = staffVoicesVector [voiceIndex];
//
//       gLog <<
//         "staff " <<
//         staffIndex <<
//         ", voice" <<
//         voice <<
//         ": " <<
//         voice->asShortString () <<
//         std::endl;
//     } // for
//   } // for
//
//   --gIndenter;
//
//   gLog <<
//     "<<<" <<
//     std::endl;
// }

void msrPart::finalizePart (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Finalizing part " <<
      fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (! fPartAllStavesList.size ()) {
    std::stringstream ss;

    ss <<
      "Part " <<
      fetchPartCombinedName () <<
      " appears in the part list, but doesn't contain any staff";

    msrWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str ());
  }

  else {
    // sort the staves to have harmonies above and
    // figured bass below the part
  if (false) // JMI v0.9.67
    fPartAllStavesList.sort (
      msrStaff::compareStavesToHaveFiguredBassesBelowCorrespondingPart);

    // finalize all the staves
    for (S_msrStaff staff : fPartAllStavesList) {
      staff->
        finalizeStaff (
          inputLineNumber);
      } // for
  }

  // set score instrument names max lengthes if relevant
  setPartInstrumentNamesMaxLengthes ();

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);

  --gIndenter;
}

void msrPart::finalizePartClone (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Finalizing part clone " <<
      fetchPartCombinedName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI v0.9.67 ???
    // sort the staves to have harmonies above and figured bass below the part
  if (false) // JMI v0.9.67
    fPartAllStavesList.sort (
       msrStaff::compareStavesToHaveFiguredBassesBelowCorrespondingPart);

    // finalize all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeStaff (
        inputLineNumber);
    } // for

  // set score instrument names max lengthes if relevant
  setPartInstrumentNamesMaxLengthes ();

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);
}

void msrPart::finalizePartAndAllItsMeasures (
  int inputLineNumber)
{
  return; // JMI TEMP NOEL

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Finalizing all the measures of part \"" <<
      fetchPartCombinedName () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrVoice voice : fPartVoicesList) {
    voice->
      finalizeVoiceAndAllItsMeasures (
        inputLineNumber);
  } // for

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);
}

void msrPart::collectPartMeasuresSlices (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    size_t
      partAllStavesListSize =
        fPartAllStavesList.size ();

    std::stringstream ss;

    ss <<
      "Collecting the measures slices of part \"" <<
      fetchPartCombinedName () <<
      "\", " <<
      mfSingularOrPluralWithoutNumber (
        partAllStavesListSize, "there is", "there are") <<
      ' ' <<
      mfSingularOrPlural (
        partAllStavesListSize, "voice", "voices") <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
    mfAssert (
      __FILE__, __LINE__,
      fPartMeasuresSlicesSequence == nullptr,
      "fPartMeasuresSlicesSequence is not null");
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

#endif // MF_SANITY_CHECKS_ARE_ENABLED

  ++gIndenter;

  // create the part measures slices sequence
  fPartMeasuresSlicesSequence =
    msrMeasuresSlicesSequence::create (
      fPartName); // origin

  // populate it
  for (S_msrStaff staff : fPartAllStavesList) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresSlices ()) {
      std::stringstream ss;

        ss <<
        "---> staff \"" <<
        staff->getStaffName () <<
        "\":";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    ++gIndenter;

    S_msrMeasuresSlicesSequence
      staffMeasuresSlicesSequence =
        staff->
          getStaffMeasuresSlicesSequence ();

    if (! staffMeasuresSlicesSequence) {
      std::stringstream ss;

        ss <<
        "The staffMeasuresSlicesSequence of staff \"" <<
        staff->getStaffName () <<
        "\" is null";

      msrWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str ());
    }
    else {
      fPartMeasuresSlicesSequence->
        mergeWithMeasuresSlicesSequence (
          inputLineNumber,
          fetchPartCombinedName (),
          staffMeasuresSlicesSequence);
    }

    --gIndenter;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    std::stringstream ss;

    ss <<
      "fPartMeasuresSlicesSequence:" <<
      std::endl;

    ++gIndenter;
    ss <<
      fPartMeasuresSlicesSequence;
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

void msrPart::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPart::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPart::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPart::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPart::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPart::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED // JMI
  if (gMsrOahGroup->getTraceMsrVisitors ()) { // JMI TEMP
    std::stringstream ss;

    ss <<
      "++++++++ fPartAllStavesList.size(): " <<
      fPartAllStavesList.size () <<
      std::endl;

    if (fPartAllStavesList.size ()) {
      for (S_msrStaff staff : fPartAllStavesList) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffName () << "\"" <<
          std::endl;
      } // for
    }

    ss <<
      "++++++++ fPartNonHarmoniesNorFiguredBassStavesList.size(): " <<
      fPartNonHarmoniesNorFiguredBassStavesList.size () <<
      std::endl;

    if (fPartNonHarmoniesNorFiguredBassStavesList.size ()) {
      for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffName () << "\"" <<
          std::endl;
      } // for
    }
  }
#endif // MF_TRACE_IS_ENABLED

  /* don't enforce any order here, leave it to the client thru sorting ??? JMI v0.9.66 */

//   // browse the part harmonies staff if any right now, JMI
//   // to place it before the corresponding part
//   if (fPartHarmoniesStaff) {
//     msrBrowser<msrStaff> browser (v);
//     browser.browse (*fPartHarmoniesStaff);
//   }

  switch (fetchPartUpLinkToScore ()->getStavesBrowingOrderKind ()) {
    case msrStavesBrowingOrderKind::kStavesBrowingOrder_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          "staves browsing order is unknown in score, part: \"" <<
          fetchPartCombinedName () <<
          "\"" <<
          ", line " << fInputStartLineNumber;

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    // MusicXML harmonies

    case msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesRegularsFiguredBasses:
      // browse the part harmonies staff if any right now, JMI
      // to place it before the corresponding part
      if (fPartHarmoniesStaff) {
        msrBrowser<msrStaff> browser (v);
        browser.browse (*fPartHarmoniesStaff);
      }

      // browse all non harmonies and non figured bass staves
      for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
        // browse the staff
        msrBrowser<msrStaff> browser (v);
        browser.browse (*staff);
      } // for

      // browse the part figured bass staff if any only now, JMI
      // to place it after the corresponding part
      if (fPartFiguredBassStaff) {
        msrBrowser<msrStaff> browser (v);
        browser.browse (*fPartFiguredBassStaff);
      }
      break;

    case msrStavesBrowingOrderKind::kStavesBrowingOrderRegularsHarmoniesFiguredBasses:
      // browse all non harmonies and non figured bass staves
      for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
        // browse the staff
        msrBrowser<msrStaff> browser (v);
        browser.browse (*staff);
      } // for

      // browse the part harmonies staff if any right now, JMI
      // to place it before the corresponding part
      if (fPartHarmoniesStaff) {
        msrBrowser<msrStaff> browser (v);
        browser.browse (*fPartHarmoniesStaff);
      }

      // browse the part figured bass staff if any only now, JMI
      // to place it after the corresponding part
      if (fPartFiguredBassStaff) {
        msrBrowser<msrStaff> browser (v);
        browser.browse (*fPartFiguredBassStaff);
      }
      break;

    case msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesFiguredBassesRegulars:
      // browse the part harmonies staff if any right now, JMI
      // to place it before the corresponding part
      if (fPartHarmoniesStaff) {
        msrBrowser<msrStaff> browser (v);
        browser.browse (*fPartHarmoniesStaff);
      }

      // browse the part figured bass staff if any only now, JMI
      // to place it after the corresponding part
      if (fPartFiguredBassStaff) {
        msrBrowser<msrStaff> browser (v);
        browser.browse (*fPartFiguredBassStaff);
      }

      // browse all non harmonies and non figured bass staves
      for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
        // browse the staff
        msrBrowser<msrStaff> browser (v);
        browser.browse (*staff);
      } // for
      break;
  } // switch
}

std::string msrPart::asString () const
{
  std::stringstream ss;

  ss <<
    "[Part" <<
    ", partID: \"" <<
    fPartID <<
    "\", partName: \"" <<
    fPartName <<
    "\", fPartAlphabeticName: \"" <<
    fPartAlphabeticName <<
    "\", fPartMsrName: " << fPartMsrName <<
    fPartName <<
    "\", fPartAllStavesList.size (): " << fPartAllStavesList.size () <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrPart::printPartMeasuresWholeNotesVector (
  std::ostream&      os,
  int                fieldWidth,
  const std::string& context) const
{
  os <<
    "fPartMeasuresWholeNotesVector" <<
    ", context: " << context <<
    ": " ;

  if (fPartNumberOfMeasures == 0) {
    os << "[EMPTY]" << std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    for (size_t i = 0; i < fPartNumberOfMeasures; ++i) {
      int ordinalNumber = i + 1; // indices start at 0

      os << std::left <<
        "ordinalNumber " <<
        std::setw (3) << std::right <<
        ordinalNumber << ": " <<
        std::setw (4) <<
        fPartMeasuresWholeNotesVector [ i ].toString () <<
        std::endl;
    } // for

    --gIndenter;
  }
}

void msrPart::printFull (std::ostream& os) const
{
  os <<
    "[Part printfull()" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 37;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fUpLinkToPartGroup" << ": ";
  if (fPartUpLinkToPartGroup) {
    // it may be empty
    os <<
      fPartUpLinkToPartGroup->fetchPartGroupCombinedName ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartID" << ": \"" <<
    fPartID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartAlphabeticName" << ": \"" <<
    fPartAlphabeticName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNameDisplayText" << ": \"" <<
    fPartNameDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbbrevation" << ": \"" <<
    fPartAbbreviation << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartAbbreviationDisplayText" << ": \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbsoluteNumber" << ": " <<
    fPartAbsoluteNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartInstrumentName" << ": \"" <<
    fPartInstrumentName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartInstrumentAbbreviation" << ": \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNumberOfMeasures" << ": " <<
    fPartNumberOfMeasures <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartContainsMultiMeasureRests" << ": " <<
    fPartContainsMultiMeasureRests <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartCurrentDrawingMeasurePosition" << ": " <<
    fPartCurrentDrawingMeasurePosition <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesStaff" << ": ";
    if (fPartHarmoniesStaff) {
      os <<
        fPartHarmoniesStaff->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesVoice" << ": ";
    if (fPartHarmoniesVoice) {
      os <<
        fPartHarmoniesVoice->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassStaff" << ": ";
    if (fPartFiguredBassStaff) {
      os <<
        fPartFiguredBassStaff->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassVoice" << ": ";
    if (fPartFiguredBassVoice) {
      os <<
        fPartFiguredBassVoice->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  // print current the part clef if any
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fPartCurrentClef" << ": ";

    if (fPartCurrentClef) {
      os <<
        '\'' <<
        fPartCurrentClef->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NULL]";
    }

    os << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the current part key if any
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fPartCurrentKey" << ": ";

    if (fPartCurrentKey) {
      os <<
        '\'' <<
        fPartCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NULL]";
    }

    os << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the current part time if any
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fPartCurrentTimeSignature" << ": ";

    if (fPartCurrentTimeSignature) {
      os <<
        '\'' <<
        fPartCurrentTimeSignature->asShortString () <<
        "'";
    }
    else {
      os << "[NULL]";
    }

    os << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  os << std::left <<
    std::setw (fieldWidth) << "fPartShortestNoteWholeNotes" << ": " <<
    fPartShortestNoteWholeNotes <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) << "fPartShortestNoteTupletFactor" << ": " <<
    std::endl;

  ++gIndenter;
  os << fPartShortestNoteTupletFactor << std::endl;
  --gIndenter;

  // print the staves list
  size_t partAllStavesListSize =
    fPartAllStavesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartAllStavesList" << ": ";

  if (partAllStavesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartAllStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the regular staves list
  size_t partRegularStavesListSize =
    fPartRegularStavesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartRegularStavesList" << ": ";

  if (partRegularStavesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartRegularStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the non harmonies nor figured bass staves list
  size_t partNonHarmoniesNorFiguredBassStavesListSize =
    fPartNonHarmoniesNorFiguredBassStavesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartNonHarmoniesNorFiguredBassStavesList" << ": ";

  if (partNonHarmoniesNorFiguredBassStavesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  // print part part all voices
  size_t partVoicesListSize =
    fPartVoicesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "Voice names in fPartVoicesList" << ": ";

  if (partVoicesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrVoice voice : fPartVoicesList) {
      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::endl;

  // print the part harmonies staff if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesStaff" << ": ";
  if (fPartHarmoniesStaff) {
    os << std::endl;
    ++gIndenter;
    os << fPartHarmoniesStaff;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::endl;

  // print the part figured bass staff if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassStaff" << ": ";
  if (fPartFiguredBassStaff) {
    os << std::endl;

    ++gIndenter;
    os << fPartFiguredBassStaff;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::endl;

  // print the part measures whole notes durations vector
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresWholeNotesVectors ()) {
    printPartMeasuresWholeNotesVector (
      os,
      fieldWidth,
      "msrPart::printFull()");
  }
#endif // MF_TRACE_IS_ENABLED

  os << std::endl;

  // print all the staves
  if (fPartAllStavesList.size ()) {
    std::list <S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        staff != nullptr,
        "staff is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case msrStaffKind::kStaffKindRegular:
          staff->printFull (os);
          break;

        case msrStaffKind::kStaffKindTablature:
          staff->printFull (os);
          break;

        case msrStaffKind::kStaffKindHarmonies:
    // JMI      if (gMsrOahGroup->getShowHarmoniesVoices ()) {}
          staff->printFull (os);
          break;

        case msrStaffKind::kStaffKindFiguredBass:
    // JMI      if (gMsrOahGroup->getShowFiguredBassVoices ()) {}
          staff->printFull (os);
          break;

        case msrStaffKind::kStaffKindDrum:
          staff->printFull (os);
          break;

        case msrStaffKind::kStaffKindRythmic:
          staff->printFull (os);
          break;
      } // switch

      if (++i == iEnd) break;

      os << std::endl;
    } // for
  }

  os << std::endl;

  // print the part measures slices sequence
  os <<
    std::setw (fieldWidth) <<
    "fPartMeasuresSlicesSequence" << ": ";

  if (fPartMeasuresSlicesSequence) {
    os << std::endl;
    ++gIndenter;
    os << fPartMeasuresSlicesSequence;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrPart::print (std::ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;
//abort ();
  const int fieldWidth = 39;

/*
  os << std::left <<
    std::setw (fieldWidth) <<
    "fUpLinkToPartGroup" << ": ";
  if (fPartUpLinkToPartGroup) {
    // it may be empty
    os <<
      fPartUpLinkToPartGroup->fetchPartGroupCombinedName ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;
*/

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartID" << ": \"" <<
    fPartID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartAlphabeticName" << ": \"" <<
    fPartAlphabeticName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbsoluteNumber" << ": " <<
    fPartAbsoluteNumber <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesStaff" << ": ";
    if (fPartHarmoniesStaff) {
      os <<
        fPartHarmoniesStaff->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesVoice" << ": ";
    if (fPartHarmoniesVoice) {
      os <<
        fPartHarmoniesVoice->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassStaff" << ": ";
    if (fPartFiguredBassStaff) {
      os <<
        fPartFiguredBassStaff->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassVoice" << ": ";
    if (fPartFiguredBassVoice) {
      os <<
        fPartFiguredBassVoice->asShortString ();
    }
    else {
      os << "[NULL]";
    }
  os << std::endl;

  // print the part measure' whole notes durations vector
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresWholeNotesVectors ()) {
    printPartMeasuresWholeNotesVector (
      os,
      fieldWidth,
      ", msrPart::print ()");
  }
#endif // MF_TRACE_IS_ENABLED

  // print all the staves
  if (fPartAllStavesList.size ()) {
    os << std::endl;

    std::list <S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        staff != nullptr,
        "staff is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI v0.9.66
        case msrStaffKind::kStaffKindRegular:
          os << staff;
          break;

        case msrStaffKind::kStaffKindTablature:
          os << staff;
          break;

        case msrStaffKind::kStaffKindHarmonies:
          os << staff;
          break;

        case msrStaffKind::kStaffKindFiguredBass:
          os << staff;
          break;

        case msrStaffKind::kStaffKindDrum:
          os << staff;
          break;

        case msrStaffKind::kStaffKindRythmic:
          os << staff;
          break;
      } // switch

      if (++i == iEnd) break;

      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrPart::printSummary (std::ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ", " <<
    mfSingularOrPlural (
      fPartNumberOfMeasures, "measure", "measure") <<
    ")" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 28;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartID" << ": \"" <<
    fPartID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartAlphabeticName" << ": \"" <<
    fPartAlphabeticName << "\"" <<
    std::endl <<


    std::setw (fieldWidth) <<
    "fPartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbsoluteNumber" << ": " <<
    fPartAbsoluteNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNameDisplayText" << ": \"" <<
    fPartNameDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbbrevation" << ": \"" <<
    fPartAbbreviation << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartAbbreviationDisplayText" << ": \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartInstrumentName" << ": \"" <<
    fPartInstrumentName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartInstrumentAbbreviation" << ": \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNumberOfMeasures" << ": " <<
    fPartNumberOfMeasures <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartContainsMultiMeasureRests" << ": " <<
    fPartContainsMultiMeasureRests <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartCurrentDrawingMeasurePosition" << ": " <<
    fPartCurrentDrawingMeasurePosition <<
    std::endl;

  // print all the staves
  if (fPartAllStavesList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fPartAllStavesList" << ": " <<
      std::endl;

    ++gIndenter;

    for (S_msrStaff staff : fPartAllStavesList) {
      staff->printSummary (os);
    } // for

    --gIndenter;
  }

  // print the harmonies staff if any
  if (fPartHarmoniesStaff) {
    os <<
      std::setw (fieldWidth) <<
      "fPartHarmoniesStaff" << ": " <<
      std::endl;

    ++gIndenter;
    os <<
      fPartHarmoniesStaff <<
      std::endl;
    --gIndenter;
  }

  // print the figured bass staff if any
  if (fPartFiguredBassStaff) {
    os <<
      std::setw (fieldWidth) <<
      "fPartFiguredBassStaff" << ": " <<
      std::endl;

    ++gIndenter;
    os <<
      fPartFiguredBassStaff <<
      std::endl;
    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrPart::printSlices (std::ostream& os) const
{
  os << "MSR part slices";

  os << std::endl << std::endl;

  ++gIndenter;

  if (fPartAllStavesList.size ()) {
    std::list <S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSlices (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrPart& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
