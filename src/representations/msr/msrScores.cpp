/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // for 'setw()'

#include "visitor.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "mfPreprocessorSettings.h"

#include "msrScores.h"

#include "oahOah.h"

#include "msrOah.h"

#ifdef MF_TRACE_IS_ENABLED
  #include "lpsrOah.h"
#endif // MF_TRACE_IS_ENABLED

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrStavesBrowingOrderKindAsString (
  msrStavesBrowingOrderKind stavesBrowingOrderKind)
{
  std::string result;

  switch (stavesBrowingOrderKind) {
    case msrStavesBrowingOrderKind::kStavesBrowingOrder_UNKNOWN_:
      result = "kStavesBrowingOrder_UNKNOWN_";
      break;

    // MusicXML harmonies

    case msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesRegularsFiguredBasses:
      result = "kStavesBrowingOrderHarmoniesRegularsFiguredBasses";
      break;
    case msrStavesBrowingOrderKind::kStavesBrowingOrderRegularsHarmoniesFiguredBasses:
      result = "kStavesBrowingOrderRegularsHarmoniesFiguredBasses";
      break;
    case msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesFiguredBassesRegulars:
      result = "kStavesBrowingOrderHarmoniesFiguredBassesRegulars";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrStavesBrowingOrderKind& elt)
{
  os << msrStavesBrowingOrderKindAsString (elt);
  return os;
}


//______________________________________________________________________________
S_msrScore msrScore::create (
  const mfInputLineNumber& inputLineNumber,
  const std::string& scoreName)
{
  msrScore* obj =
    new msrScore (
      inputLineNumber,
      scoreName);
  assert (obj != nullptr);
  return obj;
}

msrScore::msrScore (
  const mfInputLineNumber& inputLineNumber,
  const std::string& scoreName)
    : msrBookElement (inputLineNumber)
{
  // set the name
  fScoreName = scoreName;

  // create the identification
  fIdentification =
    msrIdentification::create (
      inputLineNumber);

  // number of measures
  fScoreMeasuresNumber = 0;

  // part group names max length
  fScorePartGroupNamesMaxLength = 0;

  // part names max length
  fScorePartNamesMaxLength = 0;

  // set instrument names max lengths
  fScoreInstrumentNamesMaxLength = 0;
  fScoreInstrumentAbbreviationsMaxLength = 0;

  // grace notes groups before should be browsed by default
  fInhibitGraceNotesGroupsBeforeBrowsing = false;
  fInhibitGraceNotesGroupsAfterBrowsing = false;

  // measure repeats replicas should be browsed by default
  fInhibitMeasureRepeatReplicasBrowsing = false;

  // multiple measure rests should be browsed by default
  fInhibitMultipleMeasureRestsBrowsing = false;

   // staves browsing order
  fStavesBrowingOrderKind =
    msrStavesBrowingOrderKind::kStavesBrowingOrder_UNKNOWN_;
}

msrScore::~msrScore ()
{}

S_msrScore msrScore::createScoreNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceScores ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of a score";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrScore
    newbornClone =
      msrScore::create (
        fInputLineNumber,
        fScoreName);

  // number of measures
  newbornClone->fScoreMeasuresNumber =
    fScoreMeasuresNumber;

  newbornClone->fScoreFirstMeasureNumber =
    fScoreFirstMeasureNumber;
  newbornClone->fScoreLastMeasureNumber =
    fScoreLastMeasureNumber;

  // part group names max length

  newbornClone->fScorePartGroupNamesMaxLength =
    fScorePartGroupNamesMaxLength;

  // part names max length

  newbornClone->fScorePartNamesMaxLength =
    fScorePartNamesMaxLength;

  // instrument names max lengths

  newbornClone->fScoreInstrumentNamesMaxLength =
    fScoreInstrumentNamesMaxLength;

  newbornClone->fScoreInstrumentAbbreviationsMaxLength =
    fScoreInstrumentAbbreviationsMaxLength;

  // inhibiting browsing

  newbornClone->fInhibitGraceNotesGroupsBeforeBrowsing =
    fInhibitGraceNotesGroupsBeforeBrowsing;
  newbornClone->fInhibitGraceNotesGroupsAfterBrowsing =
    fInhibitGraceNotesGroupsAfterBrowsing;

  newbornClone->fInhibitMeasureRepeatReplicasBrowsing =
    fInhibitMeasureRepeatReplicasBrowsing;

  newbornClone->fInhibitMultipleMeasureRestsBrowsing =
    fInhibitMultipleMeasureRestsBrowsing;

  return newbornClone;
}

void msrScore::setScoreMeasuresNumber (
  int scoreMeasuresNumber)
{
  fScoreMeasuresNumber = scoreMeasuresNumber;

  // register the number of measures in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setScoreMeasuresNumber (
      fScoreMeasuresNumber);
}

void msrScore::setScoreMasterVoice (
  const S_msrVoice& masterVoice)
{ fScoreMasterVoice = masterVoice; }

S_msrVoice msrScore::getScoreMasterVoice () const
{ return fScoreMasterVoice; }

void msrScore::registerVoiceInScoreAllVoicesList (
  const S_msrVoice& voice)
{
  // register voice in this staff
  fScoreAllVoicesList.push_back (voice);
}

void msrScore::addPartGroupToScore (const S_msrPartGroup& partGroup)
{
  if (fScorePartGroupsSet.count (partGroup)) {
    std::stringstream ss;

    ss <<
      "part group '" <<
      partGroup->fetchPartGroupInformationForTrace () <<
      "' already exists in this score";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      partGroup->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // register partGroup in this score
  fScorePartGroupsSet.insert (partGroup);

  fPartGroupsList.push_back (partGroup);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupsList (
      "addPartGroupToScore()");
  }
#endif // MF_TRACE_IS_ENABLED

//   if (fPartGroupsList.size () == 2) abort (); // JMI 0.9.69
}

void msrScore::removePartGroupFromScore (const S_msrPartGroup& partGroup)
{
  if (fScorePartGroupsSet.count (partGroup)) {
    std::stringstream ss;

    ss <<
      "part group '" <<
      partGroup->fetchPartGroupInformationForTrace () <<
      "' is not present in this score";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      partGroup->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // unregister partGroup in this score
  fScorePartGroupsSet.erase (partGroup);

  for (
    std::list <S_msrPartGroup>::iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    ++i
  ) {
    if ((*i) == partGroup) {
      // found partGroup, erase it
      i = fPartGroupsList.erase (i);
    }
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    displayPartGroupsList (
      "removePartGroupFromScore()");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrScore::appendCreditToScore (
  const S_msrCredit& credit)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceCredits ()) {
    std::stringstream ss;

    ss <<
      "Appending credit '" <<
      credit->asString () <<
      "' to score";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCreditsList.push_back (credit);
}

S_msrPart msrScore::fetchPartFromScoreByItsPartID (
  const mfInputLineNumber& inputLineNumber,
  const std::string& partMusicXMLID)
{
  S_msrPart result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroupsDetails ()) {
    std::stringstream ss;

    ss <<
      "fetchPartFromScoreByItsPartID(" << partMusicXMLID << "), fPartGroupsList contains:" <<
      std::endl;

    ++gIndenter;

    for (
      std::list <S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
      i != fPartGroupsList.end ();
      ++i
    ) {
      std::stringstream ss;

      ss <<
        (*i)->fetchPartGroupInformationForTrace () <<
        ", " <<
        (*i)->getPartGroupName () <<
        std::endl;
    } // for

    --gIndenter;

    ss <<
      "<=- fetchPartFromScoreByItsPartID(" << partMusicXMLID << ")" <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    ++i
  ) {
    S_msrPart
      part =
        (*i)->
          fetchPartFromPartGroupByItsPartID (
            inputLineNumber,
            partMusicXMLID);

    if (part) {
      result = part;
      break;
    }
  } // for

  return result;
}

void msrScore::collectScorePartsInList (
  const mfInputLineNumber& inputLineNumber,
  std::list <S_msrPart>& partsList)
{
  for (S_msrPartGroup partGroup : fPartGroupsList ) {
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
  } // for
}

void msrScore::collectScorePartsInMap (
  std::map <std::string, S_msrPart>& partsMap)
{
  for (S_msrPartGroup partGroup : fPartGroupsList ) {
    partGroup->
      collectPartGroupParts (
        partsMap);
  } // for
}

/*
S_msrPartGroup msrScore::fetchScorePartGroup ( JMI
  int partGroupNumber)
{
  S_msrPartGroup result;

  if (fScorePartGroupsMap.count (partGroupNumber)) {
    result = fScorePartGroupsMap [partGroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrScore::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrScore::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrScore::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrScore::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrScore::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fIdentification) {
    // browse identification
    msrBrowser<msrIdentification> browser (v);
    browser.browse (*fIdentification);
  }

  if (fScaling) {
    // browse scaling
    msrBrowser<msrScaling> browser (v);
    browser.browse (*fScaling);
  }

  if (fPageLayout) {
    // browse page layout
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

  if (fSystemLayout) {
    // browse system layout
    msrBrowser<msrSystemLayout> browser (v);
    browser.browse (*fSystemLayout);
  }

  if (fStaffLayout) {
    // browse staff layout
    msrBrowser<msrStaffLayout> browser (v);
    browser.browse (*fStaffLayout);
  }

  if (fAppearance) {
    // browse appearance
    msrBrowser<msrAppearance> browser (v);
    browser.browse (*fAppearance);
  }

  for (S_msrCredit credit : fCreditsList) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*credit);
  } // for

  for (S_msrPartGroup partGroup : fPartGroupsList) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*partGroup);
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrScore::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrScore::displayPartGroupsList (
  const std::string& context)
{
  size_t partGroupsListSize = fPartGroupsList.size ();

  gLog <<
    std::endl <<
    ">>++++++++++++++++ " <<
    "The part groups list contains " <<
    mfSingularOrPlural (
      partGroupsListSize, "element", "elements") <<
    " (" << context << "):" <<
    std::endl;

  if (partGroupsListSize) {
    ++gIndenter;

    int counter = 1;
    for (S_msrPartGroup partGroup : fPartGroupsList) {
      gLog <<
        counter << " :" <<
        std::endl;

      ++gIndenter;
      partGroup->printSummary (gLog);
//       gLog << // JMI 0.9.69
//         gTab << partGroup->asString () <<
//         partGroup <<
//         std::endl;
      --gIndenter;

      ++counter;
    } // for

    --gIndenter;
  }

  gLog <<
    " <<++++++++++++++++ " <<
    std::endl << std::endl;

//   if (partGroupsListSize == 2) abort (); // JMI 0.9.69
}

void msrScore::printFull (std::ostream& os) const
{
  os <<
    "[MSR Score printfull()" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "partGroupsListSize" << ": " <<
    partGroupsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreMeasuresNumber" << ": " <<
    fScoreMeasuresNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreFirstMeasureNumber" << ": " <<
    fScoreFirstMeasureNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fScoreLastMeasureNumber" << ": " <<
    fScoreLastMeasureNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScorePartGroupNamesMaxLength" <<  ": " <<
    fScorePartGroupNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fScorePartNamesMaxLength" <<  ": " <<
    fScorePartNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fScoreInstrumentNamesMaxLength" <<  ": " <<
    fScoreInstrumentNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fScoreInstrumentAbbreviationsMaxLength" <<  ": " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fIinhibitGraceNotesGroupsBeforeBrowsing" <<  ": " <<
    fInhibitGraceNotesGroupsBeforeBrowsing <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fIinhibitGraceNotesGroupsAfterBrowsing" <<  ": " <<
    fInhibitGraceNotesGroupsAfterBrowsing <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fIinhibitMeasureRepeatReplicasBrowsing" <<  ": " <<
    fInhibitMeasureRepeatReplicasBrowsing <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fIinhibitMultipleMeasureRestsBrowsing" <<  ": " <<
    fInhibitMultipleMeasureRestsBrowsing <<
    std::endl<<
    std::endl;

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      std::endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      std::endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      std::endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      std::endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      std::endl;
  }

  os << std::endl;

  // print the credits if any
  size_t creditsListSize = fCreditsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "CreditsList";
  if (creditsListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrCredit credit = (*i);

      os << credit;
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": [EMPTY]" <<
      std::endl;
  }
  os << std::endl;

  // print all the voices if any
  size_t scoreAllVoicesListSize = fScoreAllVoicesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fScoreAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrVoice>::const_iterator
      iBegin = fScoreAllVoicesList.begin (),
      iEnd   = fScoreAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
      if (++i == iEnd) break;
      // os << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
      std::endl;
  }

  // print the part groups if any
  os <<
    "fPartGroupsList contains " <<
    mfSingularOrPlural (
      partGroupsListSize, "element", "elements");
  if (partGroupsListSize) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrPartGroup
        partGroup = (*i);

      partGroup->printFull (os);

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": [EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrScore::print (std::ostream& os) const
{
  os <<
    "MSR Score, short version" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "partGroupsListSize" << ": " <<
    partGroupsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreMeasuresNumber" << ": " <<
    fScoreMeasuresNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreFirstMeasureNumber" << ": " <<
    fScoreFirstMeasureNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fScoreLastMeasureNumber" << ": " <<
    fScoreLastMeasureNumber <<
    std::endl;

  os << std::endl;

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      std::endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      std::endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      std::endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      std::endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      std::endl;
  }

  // print the credits if any
  size_t creditsListSize = fCreditsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fCreditsList";
  if (creditsListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrCredit credit = (*i);

      os << credit;
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": [EMPTY]" <<
      std::endl;
  }
  os << std::endl;

  // print the part groups if any
  os <<
    "fPartGroupsList contains " <<
    mfSingularOrPlural (
      partGroupsListSize, "element", "elements");
  if (partGroupsListSize) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
//       os << std::endl << "FAA before partGroup" << std::endl;
      os << (*i);
//       os << std::endl << "FOO after partGroup" << std::endl;
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": [EMPTY]" <<
      std::endl;
  }

  --gIndenter;
}

void msrScore::printSummary (std::ostream& os) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getDisplayLpsr ()) {
    os << "MSR score summary";
  }
  else if (gLpsrOahGroup->getDisplayLpsr ()) { // JMI
    os << "MSR full score";
  }
  else {
    os << "MSR score summary";
  }
#else
  os << "MSR score";
#endif // MF_TRACE_IS_ENABLED

  os << std::endl << std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 39;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  // print global information
  os << std::left <<
    std::setw (fieldWidth) <<
    "partGroupsListSize" << ": " <<
    partGroupsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreMeasuresNumber" << ": " <<
    fScoreMeasuresNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreFirstMeasureNumber" << ": " <<
    fScoreFirstMeasureNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fScoreLastMeasureNumber" << ": " <<
    fScoreLastMeasureNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreInstrumentNamesMaxLength" <<  ": " <<
    fScoreInstrumentNamesMaxLength <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreInstrumentAbbreviationsMaxLength" <<  ": " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fInhibitGraceNotesGroupsBeforeBrowsing" <<  ": " <<
    fInhibitGraceNotesGroupsBeforeBrowsing <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fInhibitGraceNotesGroupsAfterBrowsing" <<  ": " <<
    fInhibitGraceNotesGroupsAfterBrowsing <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fInhibitMeasureRepeatReplicasBrowsing" <<  ": " <<
    fInhibitMeasureRepeatReplicasBrowsing <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fInhibitMultipleMeasureRestsBrowsing" <<  ": " <<
    fInhibitMultipleMeasureRestsBrowsing <<
    std::endl<<
    std::endl;

  // print the implicit outer-most part group contents
  // it is the only element in fPartGroupsList JMI single variable JMI 0.9.69
  if (partGroupsListSize) {
    os <<
      "Part groups list:" <<
      std::endl;

    ++gIndenter;

    fPartGroupsList.front () ->
      displayPartGroupElementsList (
        fInputLineNumber);

    --gIndenter;

    os << std::endl;
  }

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification <<
      std::endl;
  }

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      std::endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      std::endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      std::endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      std::endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      std::endl;
  }

  // print the credits if any
  if (fCreditsList.size ()) {
    std::list <S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
    os << std::endl;
  }

  if (partGroupsListSize) {
    std::list <S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;
}

void msrScore::printSlices (std::ostream& os) const
{
  os << "MSR score slices";

  os << std::endl << std::endl;

  ++gIndenter;

  if (fPartGroupsList.size ()) {
    std::list <S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
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

std::ostream& operator << (std::ostream& os, const S_msrScore& elt)
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
