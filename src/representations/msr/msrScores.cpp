/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

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

#include "mfStaticSettings.h"

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
S_msrScore msrScore::create (
  int           inputLineNumber,
  const std::string& scoreName)
{
  msrScore* o =
    new msrScore (
      inputLineNumber,
      scoreName);
  assert (o != nullptr);
  return o;
}

msrScore::msrScore (
  int           inputLineNumber,
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
  fScoreNumberOfMeasures = 0;

  // part group names max length
  fScorePartGroupNamesMaxLength = 0;

  // part names max length
  fScorePartNamesMaxLength = 0;

  // set instrument names max lengthes
  fScoreInstrumentNamesMaxLength         = 0;
  fScoreInstrumentAbbreviationsMaxLength = 0;

  // grace notes groups before should be browsed by default
  fInhibitGraceNotesGroupsBeforeBrowsing = false;
  fInhibitGraceNotesGroupsAfterBrowsing  = false;

  // measure repeats replicas should be browsed by default
  fInhibitMeasureRepeatReplicasBrowsing = false;

  // multiple full-bar rests should be browsed by default
  fInhibitMultipleFullBarRestsBrowsing = false;
}

msrScore::~msrScore ()
{}

S_msrScore msrScore::createScoreNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceScores ()) {
		std::stringstream ss;

    ss <<
      "Creating a newborn clone of a score" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrScore
    newbornClone =
      msrScore::create (
        fInputLineNumber,
        fScoreName);

  // number of measures
  newbornClone->fScoreNumberOfMeasures =
    fScoreNumberOfMeasures;

  // part group names max length

  newbornClone->fScorePartGroupNamesMaxLength =
    fScorePartGroupNamesMaxLength;

  // part names max length

  newbornClone->fScorePartNamesMaxLength =
    fScorePartNamesMaxLength;

  // instrument names max lengthes

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

  newbornClone->fInhibitMultipleFullBarRestsBrowsing =
    fInhibitMultipleFullBarRestsBrowsing;

  return newbornClone;
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
      partGroup->getPartGroupCombinedName () <<
      "' already exists in this score";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      partGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
  }

  // register it in this score
  fScorePartGroupsSet.insert (partGroup);
  fPartGroupsList.push_back (partGroup);
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
      "' to score" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCreditsList.push_back (credit);
}

S_msrPart msrScore::fetchPartFromScoreByItsPartID (
  int           inputLineNumber,
  const std::string& partID)
{
  S_msrPart result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroupsDetails ()) {
		std::stringstream ss;

    ss <<
      "fetchPartFromScoreByItsPartID(" << partID << "), fPartGroupsList contains:" <<
      std::endl;

    ++gIndenter;

    for (
      std::list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
        i != fPartGroupsList.end ();
        ++i
    ) {
      std::stringstream ss;

      ss <<
        (*i)->getPartGroupCombinedName () <<
        ", " <<
        (*i)->getPartGroupName () <<
        std::endl;
    } // for

    --gIndenter;

    ss <<
      "<=- fetchPartFromScoreByItsPartID(" << partID << ")" <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    ++i
  ) {
    S_msrPart
      part =
        (*i)->
          fetchPartFromPartGroupByItsPartID (
            inputLineNumber,
            partID);

    if (part) {
      result = part;
      break;
    }
  } // for

  return result;
}

void msrScore::collectScorePartsList (
  int              inputLineNumber,
  std::list<S_msrPart>& partsList)
{
  S_msrPart result;

  for (
    std::list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    ++i
  ) {
    S_msrPartGroup
      partGroup = (*i);
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrScore::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrScore::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrScore::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrScore::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrScore::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
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

  for (
    std::list<S_msrCredit>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    ++i
  ) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    ++i
  ) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% <== msrScore::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrScore::printFull (std::ostream& os) const
{
  os <<
    "[MSR Score" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "partGroupsListSize" << ": " <<
    partGroupsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreNumberOfMeasures" << ": " <<
    fScoreNumberOfMeasures <<
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
    "fIinhibitMultipleFullBarRestsBrowsing" <<  ": " <<
    fInhibitMultipleFullBarRestsBrowsing <<
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

    std::list<S_msrCredit>::const_iterator
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
      ": " << "[NONE]" <<
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

    std::list<S_msrVoice>::const_iterator
      iBegin = fScoreAllVoicesList.begin (),
      iEnd   = fScoreAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      const S_msrVoice& voice = (*i);

      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
      if (++i == iEnd) break;
      // os << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os <<
      ": " << "[NONE]" <<
      std::endl;
  }

  // print the part groups if any
  if (partGroupsListSize) {
    std::list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrScore::print(std::ostream& os) const
{
  os <<
    "MSR Score, short version" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "partGroupsListSize" << ": " <<
    partGroupsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreNumberOfMeasures" << ": " <<
    fScoreNumberOfMeasures <<
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
    "CreditsList";
  if (creditsListSize) {
    os << std::endl;
    ++gIndenter;

    std::list<S_msrCredit>::const_iterator
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
      ": " << "[NONE]" <<
      std::endl;
  }
  os << std::endl;

  // print the part groups if any
  if (partGroupsListSize) {
    std::list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      std::endl;
  }

  --gIndenter;
}

void msrScore::printSummary (std::ostream& os) const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getDisplayLpsr ()) {
    os << "MSR score summary";
  }
  else if (gGlobalLpsrOahGroup->getDisplayLpsr ()) { // JMI
    os << "MSR full score";
  }
  else {
    os << "MSR score";
  }
#else
  os << "MSR score";
#endif // MF_TRACE_IS_ENABLED

  os << std::endl << std::endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  // print global information
  os << std::left <<
    std::setw (fieldWidth) <<
    "partGroupsListSize" << ": " <<
    partGroupsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fScoreNumberOfMeasures" << ": " <<
    fScoreNumberOfMeasures <<
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
    "fInhibitMultipleFullBarRestsBrowsing" <<  ": " <<
    fInhibitMultipleFullBarRestsBrowsing <<
    std::endl<<
    std::endl;

  // print the implicit part group contents
  // it is the only element in fPartGroupsList JMI single variable
  if (partGroupsListSize) {
    os <<
      "Part groups list:" <<
      std::endl;

    ++gIndenter;

    fPartGroupsList.front () ->
      printPartGroupElementsList (
        fInputLineNumber,
        os);

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
    std::list<S_msrCredit>::const_iterator
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
    std::list<S_msrPartGroup>::const_iterator
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
    std::list<S_msrPartGroup>::const_iterator
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
