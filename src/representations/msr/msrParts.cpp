/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrBarLines.h"
#include "msrLineBreaks.h"
#include "msrPageBreaks.h"
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
  const mfInputLineNumber& inputLineNumber,
  const std::string&       partMusicXMLID)
{
  msrPart* obj =
    new msrPart (
      inputLineNumber,
      partMusicXMLID);
  assert (obj != nullptr);
  return obj;
}

S_msrPart msrPart::create (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       partMusicXMLID,
  const S_msrPartGroup&    partUpLinkToPartGroup)
{
  msrPart* obj =
    new msrPart (
      inputLineNumber,
      partMusicXMLID);
  assert (obj != nullptr);

  // set part's part group upLink
  obj->fPartUpLinkToPartGroup = partUpLinkToPartGroup;

//   obj->
//     setPartUpLinkToPartGroup (
//     partUpLinkToPartGroup);

  return obj;
}

msrPart::msrPart (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       partMusicXMLID)
    : msrPartGroupElement (inputLineNumber)
{
  // replace spaces by underscores in partMusicXMLID to set fPartMusicXMLID
  for_each (
    partMusicXMLID.begin (),
    partMusicXMLID.end (),
    mfStringSpaceReplacer (fPartMusicXMLID, '_'));

/* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    partUpLinkToPartGroup != nullptr,
    "partUpLinkToPartGroup is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  // set part absolute number
  fPartSequentialNumber = ++sPartsCounter;

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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is this part name in the MSR OAH part renaming map?
  std::map <std::string, std::string>::const_iterator
    it =
      gMsrOahGroup->getMsrPartsRenamingMap ().find (fPartMusicXMLID);

  if (it != gMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    std::string chosenPartPathLikeName = (*it).second;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceParts ()) {
      std::stringstream ss;

      ss <<
        "Renaming MSR part " <<
        fetchPartNameForTrace () <<
        " to \"" <<
        chosenPartPathLikeName <<
        "\"";

      msrWarning (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPartPathLikeName = chosenPartPathLikeName;
  }

  else {
    // coin the names from the argument
    fPartPathLikeName =
      "Part_" + mfStringNumbersToEnglishWords (fPartMusicXMLID);
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

  // initialize part's first measure number
  fPartFirstMeasureNumber = K_MEASURE_NUMBER_UNKNOWN_;

  // initialize part's number of measures
  fPartNumberOfMeasures = 0;

  // multiple measure rests
  fPartContainsMultipleMeasureRests = false;

  // drawing measure position
  fPartCurrentDrawingPositionInMeasure = K_POSITION_IN_MEASURE_ZERO;

  // part shortest note wholeNotes
  fPartShortestNoteWholeNotes = mfWholeNotes (INT_MAX, 1);

  // part shortest note tuplet factor
  fPartShortestNoteTupletFactor = mfRational (1, 1);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "The created part contains: " << asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrPart::~msrPart ()
{}

void msrPart::setPartCurrentMeasureNumber (
  const mfMeasureNumber& measureNumber)
{
  fPartCurrentMeasureNumber = measureNumber;

  // is this the first measure number assigned in this part?
  if (fPartFirstMeasureNumber == K_MEASURE_NUMBER_UNKNOWN_) {
    fPartFirstMeasureNumber = fPartCurrentMeasureNumber;
  }
}

S_msrScore msrPart::fetchPartUpLinkToScore () const
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    partGroupClone != nullptr,
    "partGroupClone is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrPart
    newbornClone =
      msrPart::create (
        fInputLineNumber,
        fPartMusicXMLID,
        partGroupClone);

  newbornClone->fPartPathLikeName =
    fPartPathLikeName;

  newbornClone->fPartName =
    fPartName;

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
  mfStaffNumber
    staffNumber =
      staff->getStaffNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesBasics ()) {
    std::stringstream ss;

    ss <<
      "Registering staff \"" <<
      staff->getStaffPathLikeName () <<
      ", \"" <<
      staff->getStaffKind () <<
      "\" under number " <<
      staffNumber <<
      " in part " <<
      fetchPartNameForTrace () <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register staff in the all staves list
  fPartAllStavesList.push_back (staff);

  // the staves may be regitered in disorder, for example in mxml2msr,
  // so we have to sort them
//   sortStavesByIncreasingNumber (); // JMI 0.9.72 ???

  // register staff number in the staves map
  fPartStavesMap [staffNumber] = staff;

  // register staff in the staves and voices bidimensional vector
//   fPartStavesAndVoicesVector.at (staffNumber) = staff;

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
          fetchPartNameForTrace () <<
          ", line " << staff->getInputLineNumber ();

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          staff->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
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
          fetchPartNameForTrace () <<
          ", line " << staff->getInputLineNumber ();

        msrInternalError ( // JMI ???
          gServiceRunData->getInputSourceName (),
          staff->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      // register figured bass staff
      fPartFiguredBassStaff = staff;
      break;
  } // switch
}

void msrPart::setPartCurrentDrawingPositionInMeasure (
  const mfInputLineNumber& inputLineNumber,
  const mfPositionInMeasure& positionInMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting the part current drawing measure position in part " <<
      fetchPartNameForTrace () <<
      " to " <<
      positionInMeasure <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     positionInMeasure != K_POSITION_IN_MEASURE_UNKNOWN_,
//     "positionInMeasure == K_POSITION_IN_MEASURE_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  if (positionInMeasure.getNumerator () < 0) {
    std::stringstream ss;

    ss <<
      "cannot set part current drawing position in measure to " <<
      positionInMeasure <<
      " in part " <<
      fetchPartNameForTrace () <<
      " since it is negative" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  fPartCurrentDrawingPositionInMeasure = positionInMeasure;
}

void msrPart::resetPartCurrentDrawingPositionInMeasure (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    std::stringstream ss;

    ss <<
      "Resetting part current drawing measure position to 0 in part " <<
      fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartCurrentDrawingPositionInMeasure = K_POSITION_IN_MEASURE_ZERO;
}

void msrPart::incrementPartCurrentDrawingPositionInMeasure (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotesDelta)
{
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   if (wholeNotesDelta.getNumerator () <= 0) {
//     std::stringstream ss;
//
//     ss <<
//       "cannot increment part current drawing positino in measure " <<
//       fPartCurrentDrawingPositionInMeasure <<
//       " by wholeNotesDelta " <<
//       wholeNotesDelta <<
//       " in part " <<
//       fetchPartNameForTrace () <<
//       ", line " << inputLineNumber;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  mfPositionInMeasure
    newPartCurrentDrawingPositionInMeasure =
      fPartCurrentDrawingPositionInMeasure + wholeNotesDelta;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    std::stringstream ss;

    ss <<
      "Incrementing the part current drawing position in measure " <<
      fPartCurrentDrawingPositionInMeasure <<
      " in part " <<
      fetchPartNameForTrace () <<
      " by " <<
      wholeNotesDelta <<
      ", making it newPartCurrentDrawingPositionInMeasure: " <<
      newPartCurrentDrawingPositionInMeasure <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartCurrentDrawingPositionInMeasure = newPartCurrentDrawingPositionInMeasure;
}

void msrPart::decrementPartCurrentDrawingPositionInMeasure (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotesDelta)
{
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   if (wholeNotesDelta.getNumerator () <= 0) {
//     std::stringstream ss;
//
//     ss <<
//       "cannot decrement part current drawing positino in measure " <<
//       fPartCurrentDrawingPositionInMeasure <<
//       " by wholeNotesDelta " <<
//       wholeNotesDelta <<
//       " in part " <<
//       fetchPartNameForTrace () <<
//       ", line " << inputLineNumber;
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_SANITY_CHECKS_ARE_ENABLED

  mfPositionInMeasure
    newPartCurrentDrawingPositionInMeasure =
      fPartCurrentDrawingPositionInMeasure - wholeNotesDelta;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    std::stringstream ss;

    ss <<
      "Decrementing the part current drawing position in measure " <<
      fPartCurrentDrawingPositionInMeasure <<
      " in part " <<
      fetchPartNameForTrace () <<
      " by " <<
      wholeNotesDelta <<
      ", making it newPartCurrentDrawingPositionInMeasure: " <<
      newPartCurrentDrawingPositionInMeasure <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (newPartCurrentDrawingPositionInMeasure.getNumerator () < 0) {
    std::stringstream ss;

    ss <<
      "cannot decrement part current drawing position in measure  " <<
      fPartCurrentDrawingPositionInMeasure <<
      " by " <<
      wholeNotesDelta <<
      " in part " <<
      fetchPartNameForTrace () <<
      " since that sets it to newPartCurrentDrawingPositionInMeasure " <<
      newPartCurrentDrawingPositionInMeasure <<
      ", which is negative " <<
      ", line " << inputLineNumber;

//     msrInternalError (
    msrInternalWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fPartCurrentDrawingPositionInMeasure = newPartCurrentDrawingPositionInMeasure;
}

void msrPart::setPartShortestNoteWholeNotes (
  const mfWholeNotes& wholeNotes)
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
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartShortestNoteTupletFactor = noteTupletFactor;
}

void msrPart::assignSequentialNumbersToRegularVoicesInPart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Assigning sequential numbers to the staves in part \"" <<
      fPartMusicXMLID <<
      ", \"" <<
      fPartName <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      assignSequentialNumbersToRegularVoicesInStaff (
        inputLineNumber);
  } // for
}

void msrPart::setPartPathLikeName (const std::string& partPathLikeName)
{
  // is this part name in the part renaming map?
  std::map <std::string, std::string>::const_iterator
    it =
      gMsrOahGroup->getMsrPartsRenamingMap ().find (fPartPathLikeName);

  if (it != gMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    fPartPathLikeName = (*it).second;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceParts ()) {
      std::stringstream ss;

        ss <<
        "Setting part name of " << fetchPartNameForTrace () <<
        " to \"" << fPartPathLikeName << "\"" <<
         std::endl;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
  else {
    // use the argument
    fPartPathLikeName = partPathLikeName;

#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceParts ()) {
      std::stringstream ss;

        ss <<
        "Keeping partMusicXMLID \"" << partPathLikeName <<
        "\" as part name  for " << fetchPartNameForTrace () <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }
}

std::string msrPart::fetchPartIDAndName () const
{
  std::stringstream ss;

  ss << '[';

  if (fPartPathLikeName.empty ()) {
    ss <<
      "empty name";
  }
  else {
    ss <<
      fPartPathLikeName;
  }

  ss <<
    " \"" << fPartMusicXMLID << "\"";

  if (! fPartName.empty ()) {
    ss <<
      ", \"" << fPartName << "\"";
  }

  ss <<
    ']';

  return ss.str ();
}

std::string msrPart::fetchPartNameForTrace () const
{
  std::stringstream ss;

  ss <<
    '[' <<
    fPartPathLikeName <<
    ", fPartMusicXMLID: \"" << fPartMusicXMLID << "\"";

  if (! fPartName.empty ()) {
    ss <<
      ", fPartName: \"" << fPartName << "\"";
  }

  ss << ']';

  return ss.str ();
}

void msrPart::cascadeCreateAMeasureAndAppendItInPart (
  const mfInputLineNumber& inputLineNumber,
  int                    previousMeasureEndInputLineNumber,
  const mfMeasureNumber& measureNumber,
  msrMeasureImplicitKind measureImplicitKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Cascading creating a measure '" <<
      measureNumber <<
      "' and appending it to part " <<
      fetchPartNameForTrace () <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set part current measure number
  fPartCurrentMeasureNumber = measureNumber;

  // create and append measure in all the staves
  if (! fPartAllStavesList.empty ()) {
    for (S_msrStaff staff : fPartAllStavesList) {
      staff->
        cascadeCreateAMeasureAndAppendItInStaff (
          inputLineNumber,
          previousMeasureEndInputLineNumber,
          measureNumber,
          measureImplicitKind);
    } // for
  }

  --gIndenter;
}

void msrPart::cascadeNetNextMeasureNumberInPart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   nextMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in part " <<
      fetchPartNameForTrace () <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set next measure number in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeNetNextMeasureNumberInStaff (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  --gIndenter;
}

mfWholeNotes msrPart::fetchPartMeasuresWholeNotesVectorAt (
  const mfInputLineNumber& inputLineNumber,
  int indexValue) const
{
  mfWholeNotes result;

  size_t
    partMeasuresWholeNotesVectorSize =
      fPartMeasuresWholeNotesVector.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceDurations ()
      ||
    gTraceOahGroup->getTracePositionInMeasuresDetails ()
  ) {
    std::stringstream ss;

    ss <<
      "fetchPartMeasuresWholeNotesVectorAt() in part \"" <<
      fetchPartNameForTrace () <<
      "\"" <<
      ", partMeasuresWholeNotesVectorSize: " <<
      partMeasuresWholeNotesVectorSize <<
      ", indexValue: " << indexValue;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // has this measureOrdinalNumber been registered already?
  try {
    mfWholeNotes
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
      fetchPartNameForTrace () <<
      "\"" <<
      ", partMeasuresWholeNotesVectorSize: " <<
      partMeasuresWholeNotesVectorSize <<
      ", indexValue: " << indexValue << " is out of bounds" <<
      ", line " << inputLineNumber;

//     msrInternalWarningithLocationsDetails (
     msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());

      result = mfWholeNotes (15, 8); // TEMP JMI 0.9.61
  }

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceDurations ()
      ||
    gTraceOahGroup->getTracePositionInMeasuresDetails ()
  ) {
    std::stringstream ss;

    ss <<
      "fetchPartMeasuresWholeNotesVectorAt() returns \"" <<
      result.asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

void msrPart::registerShortestNoteInPartIfRelevant (const S_msrNote& note)
{
  // is note the shortest one in this part?
  mfWholeNotes
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

/* JMI
  mfWholeNotes
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
        "The new shortest note in part \"" << fetchPartNameForTrace () << "\"" <<
        " becomes " << note->asString ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace () <<
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
        fetchPartNameForTrace () <<
        " from " <<
        fPartMeasuresWholeNotesVectorSize <<
        " to " <<
        partNumberOfMeasures <<
        " measures";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPartMeasuresWholeNotesVector.clear ();
    fPartMeasuresWholeNotesVector.resize (
      fPartNumberOfMeasures + 1, // indexes start at 0 JMI 0.9.75
      K_WHOLE_NOTES_ZERO);
  }
}

void msrPart::registerOrdinalMeasureNumberWholeNotes (
  const mfInputLineNumber& inputLineNumber,
  int                 measureOrdinalNumber,
  const mfWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Registering the whole notes duration of the measure with ordinal number '" <<
      measureOrdinalNumber <<
      "' as " <<
      wholeNotes.asString () <<
      " in part " << fetchPartNameForTrace () <<
      ", measureOrdinalNumber: " << measureOrdinalNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // has this measureOrdinalNumber been registered already?
  try {
    mfWholeNotes
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
        " in part " << fetchPartNameForTrace ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fPartMeasuresWholeNotesVector.at (index) =
      wholeNotes;
  }

  catch (const std::out_of_range& e) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceMeasures ()) { // JMI 0.9.67
//       std::stringstream ss;
//
//         ss <<
//         "The measure with ordinal number " <<
//         measureOrdinalNumber <<
//         " is now registered with a duration of " <<
//         wholeNotes.asString () <<
//         " in part " << fetchPartNameForTrace () <<
//         ", fPartMeasuresWholeNotesVector.size (): " <<
//         fPartMeasuresWholeNotesVector.size () <<
//         std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str (),
//     }
// #endif // MF_TRACE_IS_ENABLED
  }

#ifdef MF_TRACE_IS_ENABLED
  if (
    gTraceOahGroup->getTraceDurations ()
      ||
    gTraceOahGroup->getTracePositionInMeasuresDetails ()
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
      "\" to part " << fetchPartNameForTrace () <<
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
//       " to part " << fetchPartNameForTrace () <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
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
      " to part " << fetchPartNameForTrace () <<
      ", groupInputLineNumber: " << groupInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      " to part " << fetchPartNameForTrace () <<
      ", groupInputLineNumber: " << groupInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      " to part " << fetchPartNameForTrace () <<
      ", groupInputLineNumber: " << groupInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
//       " to part clone " << fetchPartNameForTrace () <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
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
      " to part clone " << fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfInputLineNumber& inputLineNumber,
  const mfPositionInMeasure& positionInMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure.asString () <<
      "' in part clone " << fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade it to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      insertHiddenMeasureAndBarLineInStaffClone (
        inputLineNumber,
        positionInMeasure);
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
      "\" to part " << fetchPartNameForTrace () <<
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
  const mfInputLineNumber& inputLineNumber)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    (*i)->
      nestContentsIntoNewRepeatInStaff (
        inputLineNumber);
  } // for
}
*/

void msrPart::cascadeHandleRepeatStartInPart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Cascading handling a repeat start in part \"" <<
      fetchPartNameForTrace () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeHandleRepeatStartInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::cascadeHandleRepeatEndInPart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   measureNumber,
  int                      repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Cascading handling a repeat end in part \"" <<
      fetchPartNameForTrace () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeHandleRepeatEndInStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}

void msrPart::cascadeHandleRepeatEndingStartInPart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Cascading handling a repeat ending start in part \"" <<
      fetchPartNameForTrace () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeHandleRepeatEndingStartInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::cascadeHandleRepeatEndingEndInPart (
  const mfInputLineNumber& inputLineNumber,
  const std::string&  repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Cascading handling a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in part \"" <<
      fetchPartNameForTrace () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeHandleRepeatEndingEndInStaff (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  --gIndenter;
}

/* JMI
void msrPart::finalizeRepeatEndInPart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  int    repeatTimes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Finalizing a repeat upon its end in part \"" <<
      fetchPartNameForTrace () <<
      "\"" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfInputLineNumber& inputLineNumber,
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

void msrPart::cascadeCreateAMeasureRepeatAndAppendItToPart (
  const mfInputLineNumber& inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashesNumber)
{
  // create measures repeat in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeCreateAMeasureRepeatAndAppendItToStaff (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashesNumber);
  } // for
}

void msrPart::appendPendingMeasureRepeatToPart (
  const mfInputLineNumber& inputLineNumber)
{
  // append pending measures repeat to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPendingMeasureRepeatToStaff (
        inputLineNumber);
  } // for
}

void msrPart::cascadeAppendMultipleMeasureRestToPart (
  const mfInputLineNumber& inputLineNumber,
  int               multipleMeasureRestMeasuresNumber,
  int               multipleMeasureRestSlashesNumber,
  msrUseSymbolsKind multipleMeasureRestUseSymbolsKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Cascading apending a multiple measure rest for " <<
      mfSingularOrPlural (
        multipleMeasureRestMeasuresNumber, "measure", "measures") <<
      " to part " <<
      fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartContainsMultipleMeasureRests = true;

  // create multiple rest in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeAppendMultipleMeasureRestToStaff (
        inputLineNumber,
        multipleMeasureRestSlashesNumber,
        multipleMeasureRestMeasuresNumber,
        multipleMeasureRestUseSymbolsKind);
  } // for
}

void msrPart::replicateLastAppendedMeasureInPart (
  const mfInputLineNumber& inputLineNumber,
  int replicatasNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Replicating last appended measure in part " <<
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

void msrPart::cascadeAppendEmptyMeasuresToPart (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   previousMeasureNumber,
  int                      measureRestsNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Adding " <<
      mfSingularOrPlural (
        measureRestsNumber, "multiple measure rest", "multiple measure rests") <<
      " to part " <<
      fetchPartNameForTrace () <<
      ", ";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartContainsMultipleMeasureRests = true;

  // add multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      cascadeAppendEmptyMeasuresToStaff (
        inputLineNumber,
        previousMeasureNumber,
        measureRestsNumber);
  } // for
}

void msrPart::appendPendingMultipleMeasureRestsToPart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending the pending multiple rest to part " <<
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append pending multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPendingMultipleMeasureRestsToStaff (
        inputLineNumber);
  } // for
}

void msrPart::appendMultipleMeasureRestCloneToPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRests)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending multiple rest '" <<
      multipleMeasureRests->asString () <<
      "' to part clone " <<
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendMultipleMeasureRestCloneToStaff (
        inputLineNumber,
        multipleMeasureRests);
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

 // append barLine to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendBarLineToStaff (barLine);
  } // for
}

S_msrStaff msrPart::addRegularStaffToPartByItsNumber (
  const mfInputLineNumber& inputLineNumber,
  msrStaffKind             staffKind,
  const mfStaffNumber&     staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    std::stringstream ss;

    ss <<
      "Regular staff number " << staffNumber <<
      " already exists in part " << fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesBasics ()) {
    std::stringstream ss;

    ss <<
      "Adding regular staff number " <<
      staffNumber <<
      " (" << staffKind << ")" <<
      " to part " << fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesBasics ()) {
    std::stringstream ss;

    ss <<
      "Adding harmonies staff " <<
      " to part " << fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStavesBasics ()) {
    std::stringstream ss;

    ss <<
      "Adding figured bass staff " <<
      " to part " << fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  if (gTraceOahGroup->getTraceStavesBasics ()) {
    std::stringstream ss;

    ss <<
      "Adding staff \"" << staff->getStaffPathLikeName () <<
      "\" to part clone " << fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceStavesBasics ()) {
    std::stringstream ss;

    ss <<
      "Sorting the staves in part " <<
      fetchPartNameForTrace () <<
      ", " <<
      fPartMusicXMLID <<
      ", " <<
      fPartName <<
      " by increasing number";
//       ", line " << voice->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfStaffNumber& staffNumber)
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

// void msrPart::registerVoiceInPartStavesVoicesMapMap (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrVoice& voice)
// {
//   int
//     staffNumber =
//       voice->getVoiceUpLinkToStaff ()->getStaffNumber (),
//     voiceNumber =
//       voice->getVoiceNumber ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceParts () || gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Registering voice " <<
//       voiceNumber <<
//       ", " <<
//       voice->getVoiceName () <<
//       "\" in staff " <<
//       staffNumber <<
//       ", " <<
//       "\" in part " <<
//       fPartMusicXMLID <<
//       ", " <<
//       fPartName <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // fetch the voices map for staff staffNumber
//   std::map <int, std::map <mfVoiceNumber, S_msrVoice>>::iterator
//     partStaffIterator =
//       fPartStavesVoicesMapMap.find (staffNumber);
//
//   if (partStaffIterator == fPartStavesVoicesMapMap.end ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Staff number " <<
//       staffNumber <<
//       " cannot be found in fPartStavesVoicesMapMap";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // fetch the voice for voiceNumber in the staff
//   std::map <mfVoiceNumber, S_msrVoice>::iterator
//     partStaffVoiceIterator =
//       (*partStaffIterator).second.find (voiceNumber);
//
//   if (partStaffVoiceIterator != (*partStaffIterator).second.end ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Voice " <<
//       voiceNumber <<
//       ", " <<
//       voice->getVoiceName () <<
//       "\", has already been registered in staff " <<
//       staffNumber <<
//       " in part \"" <<
//       getPartName () <<
//       "\", line " << inputLineNumber;
//
//     msrError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   // register voice in fPartStavesVoicesMapMap map
//   fPartStavesVoicesMapMap
//     [staffNumber] [voiceNumber] = voice;
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
//     displayPartStavesVoicesMapMap (
//       inputLineNumber,
//       "msrPart::registerVoiceInPartVoicesList()");
//   }
// #endif
// }

// void msrPart::registerVoiceInRegularVoicesMap (
//   const S_msrVoice& voice)
// {
//   mfVoiceNumber voiceNumber = voice->getVoiceNumber ();
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
//       fPartMusicXMLID <<
//       ", \"" <<
//       fPartName <<
//       "\"" <<
//       ", line " << voice->getInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
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
//       "\", has already been registered in the regular voices map of part \"" << // JMI 0.9.70
//       getPartName () <<
//       "\", line " << voice->getInputLineNumber ();
//
//     msrError (
//       gServiceRunData->getInputSourceName (),
//       voice->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
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
//   mfVoiceNumber voiceNumber = voice->getVoiceNumber ();
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
//       fPartMusicXMLID <<
//       ", \"" <<
//       fPartName <<
//       "\"" <<
//       ", line " << voice->getInputLineNumber ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
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
//       "\", has already been registered in the regular voices map of part \"" << // JMI 0.9.70
//       getPartName () <<
//       "\", line " << voice->getInputLineNumber ();
//
//     msrError (
//       gServiceRunData->getInputSourceName (),
//       voice->getInputLineNumber (),
//       __FILE__, mfInputLineNumber (__LINE__),
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
//       voice->getInputLineNumber (),
//       "msrPart::registerVoiceInRegularVoicesMap()");
//   }
// #endif
// }

S_msrVoice msrPart::createPartHarmoniesVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   currentMeasureNumber)
{
  if (fPartHarmoniesVoice) {
    std::stringstream ss;

    ss <<
      "Part \"" <<
      fetchPartNameForTrace () <<
      "\" already has a harmonies voice" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace () <<
      "\" with staff number " <<
      partHarmoniesStaffNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace () <<
      "\" with voice number " <<
      partHarmoniesVoiceNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

//* JMI 0.9.66
  // set backward link
 // ->
//     setHarmoniesVoicePartBackwardLink (fPartHarmoniesVoice);
//*/

  return fPartHarmoniesVoice;
}

void msrPart::appendHarmonyToPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrHarmony&        harmony,
  const mfPositionInMeasure& positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmoniesBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " <<
      harmony->asString () <<
      " to part " <<
      fetchPartNameForTrace () <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesVoice->
     cascadeAppendHarmonyToVoice (
      inputLineNumber,
      harmony,
      positionInMeasureToAppendAt);
}

void msrPart::appendHarmoniesListToPart (
  const mfInputLineNumber&        inputLineNumber,
  const std::list <S_msrHarmony>& harmoniesList,
  const mfPositionInMeasure&      positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmoniesBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending harmonies list to part " << // JMI 0.9.67 HARMFUL
      fetchPartNameForTrace () <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesVoice->
    appendHarmoniesListToVoice (
      inputLineNumber,
      harmoniesList,
      positionInMeasureToAppendAt);
}

void msrPart::cascadeAppendFiguredBassesListToPart (
  const mfInputLineNumber&            inputLineNumber,
  const std::list <S_msrFiguredBass>& figuredBasssesList,
  const mfPositionInMeasure&          positionInMeasureToAppendAt)
{

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBassesBasics ()) {
    std::stringstream ss;

    ss <<
      "Cascading appending figured basses list \"" <<
//       figuredBasssesList->asString () << // JMI 0.9.67 HARMFUL
      "\" to part " <<
      fetchPartNameForTrace () <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartHarmoniesVoice->
    cascadeAppendFiguredBassesListToVoice (
      inputLineNumber,
      figuredBasssesList,
      positionInMeasureToAppendAt);
}

S_msrVoice msrPart::createPartFiguredBassVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber&   currentMeasureNumber)
{
  if (fPartFiguredBassVoice) {
    std::stringstream ss;

    ss <<
      "Part \"" <<
      fetchPartNameForTrace () <<
      "\" already has a figured bass voice" <<
      ", line " << inputLineNumber;

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace () <<
      "\" with staff number " <<
      partFiguredBassStaffNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace () <<
      "\" with voice number " <<
      partFiguredBassVoiceNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

///* JMI 0.9.66
  // set backward link
//   ->
//     setFiguredBassVoicePartBackwardLink (fPartFiguredBassVoice);
//*/

  return fPartFiguredBassVoice;
}

void msrPart::appendFiguredBassToPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrFiguredBass&    figuredBass,
  const mfPositionInMeasure& positionInMeasureToAppendAt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending figured bass \"" <<
      figuredBass->asString () <<
      "\" to part " <<
      fetchPartNameForTrace () <<
      ", positionInMeasureToAppendAt: " << positionInMeasureToAppendAt <<
      ", line " << figuredBass->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fPartFiguredBassVoice->
    appendFiguredBassToVoice (
      inputLineNumber,
      figuredBass,
      positionInMeasureToAppendAt);
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
//           fetchPartNameForTrace () <<
//           ", line " << figuredBass->getInputLineNumber () <<
//           std::endl;
//
//           gWaeHandler->waeTrace (
//             __FILE__, mfInputLineNumber (__LINE__),
//             ss.str (),
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       fPartFiguredBassVoice->
//         appendFiguredBassToVoice (
//           figuredBass->getInputLineNumber (),
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
//           ", line " << figuredBass->getInputLineNumber ();
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           figuredBass->getInputLineNumber (),
//           __FILE__, mfInputLineNumber (__LINE__),
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
          fetchPartNameForTrace () <<
          ", line " << figuredBass->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
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
          ", line " << figuredBass->getInputLineNumber ();

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          figuredBass->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
      fetchPartNameForTrace ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  if (gTraceOahGroup->getTraceDurations ()) {
    std::stringstream ss;

    ss <<
      "addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded () in " <<
      fetchPartNameForTrace () <<
      ", line " << skipGraceNotesGroup->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (S_msrStaff staff : fPartAllStavesList) {
    std::map <mfVoiceNumber, S_msrVoice>
      staffAllVoicesMap =
        staff->
          getStaffAllVoicesMap ();

    for (std::pair <mfVoiceNumber, S_msrVoice> thePair : staffAllVoicesMap) {
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
//   const mfInputLineNumber& inputLineNumber,
//   const mfWholeNotes& backupStepLength)
// {
//   // account for backup in part
//   decrementPartCurrentDrawingPositionInMeasure (
//     inputLineNumber,
//     backupStepLength);
// }

void msrPart::finalizeLastAppendedMeasureInPart (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
      "Finalizing last appended measure in part " <<
      fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

void msrPart::displayPartStavesMap (
  const mfInputLineNumber& inputLineNumber,
  const std::string& context) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    gLog <<
      ">>> The fPartStavesMap of part \"" <<
      getPartName () <<
      "\" , context: " << context <<
      ", contains:" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  size_t
    partStaffVoicesMapSize =
      fPartStavesMap.size ();

  gLog <<
    std::endl <<
    "fPartStavesMap contains " <<
    mfSingularOrPlural (
      partStaffVoicesMapSize, "stave", "staves") <<
    ", context: " << context <<
    ", line " << inputLineNumber <<
    ":" <<
    std::endl;

  if (partStaffVoicesMapSize) {
    ++gIndenter;

    for (
      std::pair <mfStaffNumber, S_msrStaff> primaryPair : fPartStavesMap
    ) {
      mfStaffNumber
        staffNumber = primaryPair.first;

      S_msrStaff
        staff = primaryPair.second;

      for (
        std::pair <mfVoiceNumber, S_msrVoice> secondaryPair : staff->getStaffAllVoicesMap ()
      ) {
        mfVoiceNumber
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
//   const mfInputLineNumber& inputLineNumber,
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
//         fPartStavesAndVoicesVector.at (staffIndex);
//
//     for (int voiceIndex = 0; voiceIndex < staffVoicesVector.size (); ++voiceIndex) {
//       S_msrVoicevoice = staffVoicesVector.at (voiceIndex);
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
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Finalizing part " <<
      fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  if (fPartAllStavesList.empty ()) {
    std::stringstream ss;

    ss <<
      "Part " <<
      fetchPartNameForTrace () <<
      " appears in the part list, but doesn't contain any staff";

    msrWarning (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      ss.str ());
  }

  else {
    // sort the staves to have harmonies above and
    // figured bass below the part
  if (false) // JMI 0.9.67
    fPartAllStavesList.sort (
      msrStaff::compareStavesToHaveFiguredBassesBelowCorrespondingPart);

    // finalize all the staves
    for (S_msrStaff staff : fPartAllStavesList) {
      staff->
        finalizeStaff (
          inputLineNumber);
      } // for
  }

  // set score instrument names max lengths if relevant
  setPartInstrumentNamesMaxLengthes ();

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);

  --gIndenter;
}

void msrPart::finalizePartClone (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Finalizing part clone " <<
      fetchPartNameForTrace () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI 0.9.67 ???
    // sort the staves to have harmonies above and figured bass below the part
  if (false) // JMI 0.9.67
    fPartAllStavesList.sort (
       msrStaff::compareStavesToHaveFiguredBassesBelowCorrespondingPart);

    // finalize all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeStaff (
        inputLineNumber);
    } // for

  // set score instrument names max lengths if relevant
  setPartInstrumentNamesMaxLengthes ();

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);
}

void msrPart::finalizePartAndAllItsMeasures (
  const mfInputLineNumber& inputLineNumber)
{
  return; // JMI TEMP NOEL

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceParts ()) {
    std::stringstream ss;

    ss <<
      "Finalizing all the measures of part \"" <<
      fetchPartNameForTrace () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    size_t
      partAllStavesListSize =
        fPartAllStavesList.size ();

    std::stringstream ss;

    ss <<
      "Collecting the measures slices of part \"" <<
      fetchPartNameForTrace () <<
      "\", " <<
      mfSingularOrPluralWithoutNumber (
        partAllStavesListSize, "there is", "there are") <<
      ' ' <<
      mfSingularOrPlural (
        partAllStavesListSize, "voice", "voices") <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
    mfAssert (
      __FILE__, mfInputLineNumber (__LINE__),
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
        staff->getStaffPathLikeName () <<
        "\":";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
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
        staff->getStaffPathLikeName () <<
        "\" is NULL";

      msrWarning (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        ss.str ());
    }
    else {
      fPartMeasuresSlicesSequence->
        mergeWithMeasuresSlicesSequence (
          inputLineNumber,
          fetchPartNameForTrace (),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
            __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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
            __FILE__, mfInputLineNumber (__LINE__),
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
      __FILE__, mfInputLineNumber (__LINE__),
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

    if (! fPartAllStavesList.empty ()) {
      for (S_msrStaff staff : fPartAllStavesList) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffPathLikeName () << "\"" <<
          std::endl;
      } // for
    }

    ss <<
      "++++++++ fPartNonHarmoniesNorFiguredBassStavesList.size(): " <<
      fPartNonHarmoniesNorFiguredBassStavesList.size () <<
      std::endl;

    if (! fPartNonHarmoniesNorFiguredBassStavesList.empty ()) {
      for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffPathLikeName () << "\"" <<
          std::endl;
      } // for
    }
  }
#endif // MF_TRACE_IS_ENABLED

  /* don't enforce any order here, leave it to the client thru sorting ??? JMI 0.9.66 */

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
          fetchPartNameForTrace () <<
          "\"" <<
          ", line " << fInputLineNumber;

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
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
    ", partMusicXMLID: \"" <<
    fPartMusicXMLID <<
    "\", partName: \"" <<
    fPartName <<
    "\", fPartPathLikeName: \"" <<
    fPartPathLikeName <<
    "\", fPartPathLikeName: " << fPartPathLikeName <<
    fPartName <<
    "\", fPartAllStavesList.size (): " << fPartAllStavesList.size () <<
    ", line " << fInputLineNumber <<
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
    ": ";

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
        fPartMeasuresWholeNotesVector.at (i).toString () <<
        std::endl;
    } // for

    --gIndenter;
  }
}

void msrPart::print (std::ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartPathLikeName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;
//abort ();
  constexpr int fieldWidth = 39;

/*
  os << std::left <<
    std::setw (fieldWidth) <<
    "fUpLinkToPartGroup" << ": ";
  if (fPartUpLinkToPartGroup) {
    // it may be empty
    os <<
      fPartUpLinkToPartGroup->fetchPartGroupInformationForTrace ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;
*/

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartMusicXMLID" << ": \"" <<
    fPartMusicXMLID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartPathLikeName" << ": \"" <<
    fPartPathLikeName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartSequentialNumber" << ": " <<
    fPartSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartFirstMeasureNumber" << ": " <<
    fPartFirstMeasureNumber <<
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
  if (! fPartAllStavesList.empty ()) {
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
        __FILE__, mfInputLineNumber (__LINE__),
        staff != nullptr,
        "staff is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI 0.9.66
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

void msrPart::printFull (std::ostream& os) const
{
  os <<
    "[Part printfull()" << ' ' << fPartPathLikeName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 37;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fUpLinkToPartGroup" << ": ";
  if (fPartUpLinkToPartGroup) {
    // it may be empty
    os <<
      fPartUpLinkToPartGroup->fetchPartGroupInformationForTrace ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartMusicXMLID" << ": \"" <<
    fPartMusicXMLID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartPathLikeName" << ": \"" <<
    fPartPathLikeName << "\"" <<
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
    "fPartSequentialNumber" << ": " <<
    fPartSequentialNumber <<
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
    "fPartFirstMeasureNumber" << ": " <<
    fPartFirstMeasureNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNumberOfMeasures" << ": " <<
    fPartNumberOfMeasures <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartContainsMultipleMeasureRests" << ": " <<
    fPartContainsMultipleMeasureRests <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartCurrentDrawingPositionInMeasure" << ": " <<
    fPartCurrentDrawingPositionInMeasure <<
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
      os << "\"" << staff->getStaffPathLikeName () << "\"" << std::endl;
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
      os << "\"" << staff->getStaffPathLikeName () << "\"" << std::endl;
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
      os << "\"" << staff->getStaffPathLikeName () << "\"" << std::endl;
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
  if (! fPartAllStavesList.empty ()) {
    std::list <S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, mfInputLineNumber (__LINE__),
        staff != nullptr,
        "staff is NULL");
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

void msrPart::printSummary (std::ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartPathLikeName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ", " <<
    mfSingularOrPlural (
      fPartNumberOfMeasures, "measure", "measure") <<
    ")" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 28;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartMusicXMLID" << ": \"" <<
    fPartMusicXMLID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartPathLikeName" << ": \"" <<
    fPartPathLikeName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartSequentialNumber" << ": " <<
    fPartSequentialNumber <<
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
    "fPartContainsMultipleMeasureRests" << ": " <<
    fPartContainsMultipleMeasureRests <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartCurrentDrawingPositionInMeasure" << ": " <<
    fPartCurrentDrawingPositionInMeasure <<
    std::endl;

  // print all the staves
  if (! fPartAllStavesList.empty ()) {
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

  if (! fPartAllStavesList.empty ()) {
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
