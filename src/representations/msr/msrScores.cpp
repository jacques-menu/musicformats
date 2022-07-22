/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // for 'setw()'

#include "visitor.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrScores.h"

#include "oahOah.h"

#include "msrOah.h"

#ifdef TRACING_IS_ENABLED
  #include "lpsrOah.h"
#endif

#include "msrBrowsers.h"


using namespace std;


namespace MusicFormats
{

//______________________________________________________________________________
S_msrScore msrScore::create (
  int           inputLineNumber,
  const string& scoreName)
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
  const string& scoreName)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceScores ()) {
    gLogStream <<
      "Creating a newborn clone of a score" <<
      endl;
  }
#endif

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
  S_msrVoice masterVoice)
{ fScoreMasterVoice = masterVoice; }

S_msrVoice msrScore::getScoreMasterVoice () const
{ return fScoreMasterVoice; }

void msrScore::registerVoiceInScoreAllVoicesList (
  S_msrVoice voice)
{
  // register voice in this staff
  fScoreAllVoicesList.push_back (voice);
}

void msrScore::addPartGroupToScore (S_msrPartGroup partGroup)
{
  if (fScorePartGroupsSet.count (partGroup)) {
    stringstream s;

    s <<
      "part group '" <<
      partGroup->getPartGroupCombinedName () <<
      "' already exists in this score";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      partGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register it in this score
  fScorePartGroupsSet.insert (partGroup);
  fPartGroupsList.push_back (partGroup);
}

void msrScore::appendCreditToScore (S_msrCredit credit)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceCredits ()) {
    gLogStream <<
      "Appending credit '" <<
      credit->asString () <<
      "' to score" <<
      endl;
  }
#endif

  fCreditsList.push_back (credit);
}

S_msrPart msrScore::fetchPartFromScoreByItsPartID (
  int           inputLineNumber,
  const string& partID)
{
  S_msrPart result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroupsDetails ()) {
    gLogStream <<
      "fetchPartFromScoreByItsPartID(" << partID << "), fPartGroupsList contains:" <<
      endl;

    ++gIndenter;

    for (
      list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
        i != fPartGroupsList.end ();
        ++i
      ) {
      gLogStream <<
        (*i)->getPartGroupCombinedName () <<
        ", " <<
        (*i)->getPartGroupName () <<
        endl;
    } // for

    --gIndenter;

    gLogStream <<
      "<=- fetchPartFromScoreByItsPartID(" << partID << ")" <<
      endl << endl;
  }
#endif

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
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
  list<S_msrPart>& partsList)
{
  S_msrPart result;

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScore::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScore::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScore::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrScore::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrScore::browseData ()" <<
      endl;
  }

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
    list<S_msrCredit>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    ++i
  ) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    ++i
  ) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrScore::browseData ()" <<
      endl;
  }
}

void msrScore::print (ostream& os) const
{
  os <<
    "[MSR Score" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  os << left <<
    setw (fieldWidth) <<
    "partGroupsListSize" << " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    "fScoreNumberOfMeasures" << " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "fScorePartGroupNamesMaxLength" <<  " : " <<
    fScorePartGroupNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fScorePartNamesMaxLength" <<  " : " <<
    fScorePartNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fScoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fScoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fIinhibitGraceNotesGroupsBeforeBrowsing" <<  " : " <<
    fInhibitGraceNotesGroupsBeforeBrowsing <<
    endl<<
    setw (fieldWidth) <<
    "fIinhibitGraceNotesGroupsAfterBrowsing" <<  " : " <<
    fInhibitGraceNotesGroupsAfterBrowsing <<
    endl<<

    setw (fieldWidth) <<
    "fIinhibitMeasureRepeatReplicasBrowsing" <<  " : " <<
    fInhibitMeasureRepeatReplicasBrowsing <<
    endl<<

    setw (fieldWidth) <<
    "fIinhibitMultipleFullBarRestsBrowsing" <<  " : " <<
    fInhibitMultipleFullBarRestsBrowsing <<
    endl<<
    endl;

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      endl;
  }

  os << endl;

  // print the credits if any
  size_t creditsListSize = fCreditsList.size ();

  os <<
    setw (fieldWidth) <<
    "CreditsList";
  if (creditsListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrCredit credit = (*i);

      os << credit;
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
  os << endl;

  // print all the voices if any
  size_t scoreAllVoicesListSize = fScoreAllVoicesList.size ();

  os <<
    setw (fieldWidth) <<
    "fScoreAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrVoice>::const_iterator
      iBegin = fScoreAllVoicesList.begin (),
      iEnd   = fScoreAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << "\"" << voice->getVoiceName () << "\"" << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }

  // print the part groups if any
  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrScore::printShort (ostream& os) const
{
  os <<
    "MSR Score, short version" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  os << left <<
    setw (fieldWidth) <<
    "partGroupsListSize" << " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    "fScoreNumberOfMeasures" << " : " <<
    fScoreNumberOfMeasures <<
    endl;

  os << endl;

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      endl;
  }

  // print the credits if any
  size_t creditsListSize = fCreditsList.size ();

  os <<
    setw (fieldWidth) <<
    "CreditsList";
  if (creditsListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrCredit credit = (*i);

      os << credit;
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
  os << endl;

  // print the part groups if any
  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      // no endl here
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      endl;
  }

  --gIndenter;
}

void msrScore::printSummary (ostream& os) const
{
#ifdef TRACING_IS_ENABLED
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
#endif

  os << endl << endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t partGroupsListSize =
    fPartGroupsList.size ();

  // print global information
  os << left <<
    setw (fieldWidth) <<
    "partGroupsListSize" << " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    "fScoreNumberOfMeasures" << " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "fScoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl <<

    setw (fieldWidth) <<
    "fScoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fInhibitGraceNotesGroupsBeforeBrowsing" <<  " : " <<
    fInhibitGraceNotesGroupsBeforeBrowsing <<
    endl<<
    setw (fieldWidth) <<
    "fInhibitGraceNotesGroupsAfterBrowsing" <<  " : " <<
    fInhibitGraceNotesGroupsAfterBrowsing <<
    endl<<

    setw (fieldWidth) <<
    "fInhibitMeasureRepeatReplicasBrowsing" <<  " : " <<
    fInhibitMeasureRepeatReplicasBrowsing <<
    endl<<

    setw (fieldWidth) <<
    "fInhibitMultipleFullBarRestsBrowsing" <<  " : " <<
    fInhibitMultipleFullBarRestsBrowsing <<
    endl<<
    endl;

  // print the implicit part group contents
  // it is the only element in fPartGroupsList JMI single variable
  if (partGroupsListSize) {
    os <<
      "Part groups list:" <<
      endl;

    ++gIndenter;

    fPartGroupsList.front () ->
      printPartGroupElementsList (
        fInputLineNumber,
        os);

    --gIndenter;

    os << endl;
  }

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification <<
      endl;
  }

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      endl;
  }

  // print the credits if any
  if (fCreditsList.size ()) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
  }

  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;
}

void msrScore::printSlices (ostream& os) const
{
  os << "MSR score slices";

  os << endl << endl;

  ++gIndenter;

  if (fPartGroupsList.size ()) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSlices (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}


}
