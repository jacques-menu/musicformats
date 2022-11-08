/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // for 'setw()'

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msr.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrBook msrBook::create (
  int           inputLineNumber,
  const string& bookName)
{
  msrBook* o =
    new msrBook (
      inputLineNumber,
      bookName);
  assert (o != nullptr);
  return o;
}

msrBook::msrBook (
  int           inputLineNumber,
  const string& bookName)
    : msrElement (inputLineNumber)
{
  // set the name
  fBookName = bookName;

  // create the identification
  fIdentification =
    msrIdentification::create (
      inputLineNumber);

  // number of measures
  fBookNumberOfMeasures = 0;

  // part group names max length
  fBookElementNamesMaxLength = 0;

  // part names max length
  fBookElementNamesMaxLength = 0;

  // set instrument names max lengthes
  fBookInstrumentNamesMaxLength         = 0;
  fBookInstrumentAbbreviationsMaxLength = 0;

  // grace notes groups before should be browsed by default
  fInhibitGraceNotesGroupsBeforeBrowsing = false;
  fInhibitGraceNotesGroupsAfterBrowsing  = false;

  // measure repeats replicas should be browsed by default
  fInhibitMeasureRepeatReplicasBrowsing = false;

  // multiple full-bar rests should be browsed by default
  fInhibitMultipleFullBarRestsBrowsing = false;
}

msrBook::~msrBook ()
{}

S_msrBook msrBook::createBookNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBooks ()) {
    gLogStream <<
      "Creating a newborn clone of a score" <<
      endl;
  }
#endif

  S_msrBook
    newbornClone =
      msrBook::create (
        fInputLineNumber,
        fBookName);

  // number of measures
  newbornClone->fBookNumberOfMeasures =
    fBookNumberOfMeasures;

  // part group names max length

  newbornClone->fBookElementNamesMaxLength =
    fBookElementNamesMaxLength;

  // part names max length

  newbornClone->fBookElementNamesMaxLength =
    fBookElementNamesMaxLength;

  // instrument names max lengthes

  newbornClone->fBookInstrumentNamesMaxLength =
    fBookInstrumentNamesMaxLength;

  newbornClone->fBookInstrumentAbbreviationsMaxLength =
    fBookInstrumentAbbreviationsMaxLength;

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

void msrBook::setBookMasterVoice (
  S_msrVoice masterVoice)
{ fBookMasterVoice = masterVoice; }

S_msrVoice msrBook::getBookMasterVoice () const
{ return fBookMasterVoice; }

void msrBook::registerVoiceInBookAllVoicesList (
  S_msrVoice voice)
{
  // register voice in this staff
  fBookAllVoicesList.push_back (voice);
}

void msrBook::addBookElementToBook (
  S_msrBookElement bookElement)
{
  if (fBookElementsSet.count (bookElement)) {
    stringstream s;

    s <<
      "part group '" <<
      bookElement <<
      "' already exists in this score";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      bookElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register it in this score
  fBookElementsSet.insert (bookElement);
  fBookElementsList.push_back (bookElement);

  // is bookElement an msrScore instance? (only possibility currently JMI)
  if (
    S_msrScore
      score =
        dynamic_cast<msrScore*>(&(*bookElement))
  ) {
    score->
      setScoreUpLinkToBook (this);
  }
}

void msrBook::appendCreditToBook (S_msrCredit credit)
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

void msrBook::fetchIdentificationFromCreditsIfAny ( // THROW AWAY JMI ???
  int inputLineNumber)
{
/* JMI
  if (
    fIdentification->getIdentificationWorkCreditTypeTitle ().size ()
      &&
    gGlobalMsrOahGroup->getUseFilenameAsWorkCreditTypeTitle ()
  ) {
    string
      inputSourceName =
        gGlobalServiceRunData->getInputSourceName ();

    if (inputSourceName == "-") {
      inputSourceName = "Standard_input";
    }
  }
*/

  /*
  <credit page="1">
    <credit-words default-x="548" default-y="1382" font-family="FreeSerif" font-size="26" font-weight="bold" justify="center" valign="top" xml:space="preserve">"R E Q U I E M"    from    D E A T H N O T E</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="548" default-y="1331" font-family="FreeSerif" font-size="16" font-style="italic" justify="center" valign="top">Theme from L's death. For SATB choir.</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="1046" default-y="1253" font-family="FreeSerif" font-size="12" justify="right" valign="bottom">Yoshihisa Hirano &amp; Hideki Taniuchi</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="51" default-y="1253" font-family="FreeSerif" font-size="12" valign="bottom" xml:lang="fr">arrangement and lyrics by Andrés Rojas</credit-words>
  </credit>

  */

  // credits on top of page one can be used as identification
  if (fCreditsList.size () >= 1) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;

    int topCreditsCounter    = 0;
    int bottomCreditsCounter = 0;

    for ( ; ; ) {
      S_msrCredit credit = (*i);

      if (credit->getCreditPageNumber () == 1) {
        const vector<S_msrCreditWords>&
          creditWordsVector =
            credit->
              getCreditWordsList ();

        if (creditWordsVector.size () >= 1) {
          S_msrCreditWords
            creditWords =
              creditWordsVector.front ();

          string
            creditWordsContents =
              creditWords->
                getCreditWordsContents ();

          switch (creditWords->getCreditWordsVerticalAlignmentKind ()) {
            case msrVerticalAlignmentKind::kVerticalAlignmentNone:
              break;

            case msrVerticalAlignmentKind::kVerticalAlignmentTop:
              ++topCreditsCounter;

              switch (topCreditsCounter) {
                case 1:
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTracingOahGroup->getTraceCredits ()) {
                    gLogStream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as score title" <<
                      endl;
              }
#endif

                  fIdentification->
                    setIdentificationWorkCreditTypeTitle (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                case 2:
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTracingOahGroup->getTraceCredits ()) {
                    gLogStream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as movement title" <<
                      endl;
              }
#endif

                  fIdentification->
                    setIdentificationMovementTitle (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                default:
                  ;
              } // switch
              break;

            case msrVerticalAlignmentKind::kVerticalAlignmentMiddle:
              break;

            case msrVerticalAlignmentKind::kVerticalAlignmentBottom:
              ++bottomCreditsCounter;

              switch (bottomCreditsCounter) {
                case 1:
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTracingOahGroup->getTraceCredits ()) {
                    gLogStream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as composer" <<
                      endl;
              }
#endif

                  fIdentification->
                    appendComposer (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                case 2:
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTracingOahGroup->getTraceCredits ()) {
                    gLogStream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as poet" <<
                      endl;
              }
#endif

                  fIdentification->
                    appendPoet (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                default:
                  ;
              } // switch
              break;
          } // switch
        }
      }

      if (++i == iEnd) break;
//      s << ", ";
    } // for
  }
}

/*
S_msrBookElement msrBook::fetchBookElement (
  int partGroupNumber)
{
  S_msrBookElement result;

  if (fBookElementsMap.count (partGroupNumber)) {
    result = fBookElementsMap [partGroupNumber];
  }

  return result;
}
*/

void msrBook::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBook::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBook>*
    p =
      dynamic_cast<visitor<S_msrBook>*> (v)) {
        S_msrBook elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBook::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBook::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBook::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBook>*
    p =
      dynamic_cast<visitor<S_msrBook>*> (v)) {
        S_msrBook elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBook::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBook::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBook::browseData ()" <<
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
    list<S_msrBookElement>::const_iterator i = fBookElementsList.begin ();
    i != fBookElementsList.end ();
    ++i
  ) {
    // browse the part group
    msrBrowser<msrBookElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrBook::browseData ()" <<
      endl;
  }
}

void msrBook::print (ostream& os) const
{
  os <<
    "[MSR book" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t bookElementsListSize =
    fBookElementsList.size ();

  os << left <<
    setw (fieldWidth) <<
    "bookElementsListSize" << " : " <<
    bookElementsListSize <<
    endl <<

    setw (fieldWidth) <<
    "fBookNumberOfMeasures" << " : " <<
    fBookNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "fBookPartGroupNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fBookCreditTypePartNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fBookInstrumentNamesMaxLength" <<  " : " <<
    fBookInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fBookInstrumentAbbreviationsMaxLength" <<  " : " <<
    fBookInstrumentAbbreviationsMaxLength <<
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
      " : " << "[NONE]" <<
      endl;
  }
  os << endl;

  // print all the voices if any
  size_t scoreAllVoicesListSize = fBookAllVoicesList.size ();

  os <<
    setw (fieldWidth) <<
    "BookAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrVoice>::const_iterator
      iBegin = fBookAllVoicesList.begin (),
      iEnd   = fBookAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os <<
      " : " << "[NONE]" <<
      endl;
  }

  // print the part groups if any
  if (bookElementsListSize) {
    list<S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
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

void msrBook::printShort (ostream& os) const
{
  os <<
    "[MSR book, short version" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t bookElementsListSize =
    fBookElementsList.size ();

  os << left <<
    setw (fieldWidth) <<
    "bookElementsListSize" << " : " <<
    bookElementsListSize <<
    endl <<

    setw (fieldWidth) <<
    "fBookNumberOfMeasures" << " : " <<
    fBookNumberOfMeasures <<
    endl;

/*
  os << left <<
    setw (fieldWidth) <<
    "fBookPartGroupNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fBookCreditTypePartNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fBookInstrumentNamesMaxLength" <<  " : " <<
    fBookInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "fBookInstrumentAbbreviationsMaxLength" <<  " : " <<
    fBookInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitGraceNotesGroupsBeforeBrowsing" <<  " : " <<
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

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }
*/

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

/*
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
      " : " << "[NONE]" <<
      endl;
  }
  os << endl;

  // print all the voices if any
  size_t scoreAllVoicesListSize = fBookAllVoicesList.size ();

  os <<
    setw (fieldWidth) <<
    "BookAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrVoice>::const_iterator
      iBegin = fBookAllVoicesList.begin (),
      iEnd   = fBookAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os <<
      " : " << "[NONE]" <<
      endl;
  }
*/

  // print the part groups if any
  if (bookElementsListSize) {
    list<S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
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

  os << ']' << endl;
}

void msrBook::printSummary (ostream& os) const
{
  os <<
    "[MSR book summary" <<
    endl << endl;

  ++gIndenter;

  const int fieldWidth = 38;

  size_t bookElementsListSize =
    fBookElementsList.size ();

  // print global information
  os << left <<
    setw (fieldWidth) <<
    "bookElementsListSize" << " : " <<
    bookElementsListSize <<
    endl <<

    setw (fieldWidth) <<
    "fBookNumberOfMeasures" << " : " <<
    fBookNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "fBookInstrumentNamesMaxLength" <<  " : " <<
    fBookInstrumentNamesMaxLength <<
    endl <<

    setw (fieldWidth) <<
    "fBookInstrumentAbbreviationsMaxLength" <<  " : " <<
    fBookInstrumentAbbreviationsMaxLength <<
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

  if (bookElementsListSize) {
    list<S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrBook& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
