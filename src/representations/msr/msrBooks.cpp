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

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrBook msrBook::create (
  int                inputLineNumber,
  const std::string& bookName)
{
  msrBook* obj =
    new msrBook (
      inputLineNumber,
      bookName);
  assert (obj != nullptr);
  return obj;
}

msrBook::msrBook (
  int                inputLineNumber,
  const std::string& bookName)
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

  // set instrument names max lengths
  fBookInstrumentNamesMaxLength         = 0;
  fBookInstrumentAbbreviationsMaxLength = 0;

  // grace notes groups before should be browsed by default
  fInhibitGraceNotesGroupsBeforeBrowsing = false;
  fInhibitGraceNotesGroupsAfterBrowsing  = false;

  // measure repeats replicas should be browsed by default
  fInhibitMeasureRepeatReplicasBrowsing = false;

  // multiple measure rests should be browsed by default
  fInhibitMultipleMeasureRestsBrowsing = false;
}

msrBook::~msrBook ()
{}

S_msrBook msrBook::createBookNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBooks ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of a score";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

  // instrument names max lengths

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

  newbornClone->fInhibitMultipleMeasureRestsBrowsing =
    fInhibitMultipleMeasureRestsBrowsing;

  return newbornClone;
}

void msrBook::setBookMasterVoice (
  const S_msrVoice& masterVoice)
{ fBookMasterVoice = masterVoice; }

S_msrVoice msrBook::getBookMasterVoice () const
{ return fBookMasterVoice; }

void msrBook::registerVoiceInBookAllVoicesList (
  const S_msrVoice& voice)
{
  // register voice in this staff
  fBookAllVoicesList.push_back (voice);
}

void msrBook::addBookElementToBook (
  const S_msrBookElement& bookElement)
{
  if (fBookElementsSet.count (bookElement)) {
    std::stringstream ss;

    ss <<
      "part group '" <<
      bookElement <<
      "' already exists in this score";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      bookElement->getInputLineNumber (),
      __FILE__, __LINE__,
      ss.str ());
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

void msrBook::appendCreditToBook (
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
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fCreditsList.push_back (credit);
}

void msrBook::fetchIdentificationFromCreditsIfAny ( // THROW AWAY JMI ??? v0.9.66
  int inputLineNumber)
{
/* JMI
  if (
    fIdentification->getIdentificationWorkCreditTypeTitle ().size ()
      &&
    gMsrOahGroup->getUseFilenameAsWorkCreditTypeTitle ()
  ) {
    std::string
      inputSourceName =
        gServiceRunData->getInputSourceName ();

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
    std::list <S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;

    int topCreditsCounter    = 0;
    int bottomCreditsCounter = 0;

    for ( ; ; ) {
      S_msrCredit credit = (*i);

      if (credit->getCreditPageNumber () == 1) {
        const std::vector <S_msrCreditWords>&
          creditWordsVector =
            credit->
              getCreditWordsList ();

        if (creditWordsVector.size () >= 1) {
          S_msrCreditWords
            creditWords =
              creditWordsVector.front ();

          std::string
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
#ifdef MF_TRACE_IS_ENABLED
                  if (gTraceOahGroup->getTraceCredits ()) {
                    gLog <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as score title" <<
                      std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

                  fIdentification->
                    setIdentificationWorkCreditTypeTitle (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                case 2:
#ifdef MF_TRACE_IS_ENABLED
                  if (gTraceOahGroup->getTraceCredits ()) {
                    gLog <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as movement title" <<
                      std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
                  if (gTraceOahGroup->getTraceCredits ()) {
                    gLog <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as composer" <<
                      std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

                  fIdentification->
                    appendComposer (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                case 2:
#ifdef MF_TRACE_IS_ENABLED
                  if (gTraceOahGroup->getTraceCredits ()) {
                    gLog <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as poet" <<
                      std::endl;
              }
#endif // MF_TRACE_IS_ENABLED

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
//      ss << ", ";
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBook::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBook>*
    p =
      dynamic_cast<visitor<S_msrBook>*> (v)) {
        S_msrBook elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBook::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBook::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBook::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBook>*
    p =
      dynamic_cast<visitor<S_msrBook>*> (v)) {
        S_msrBook elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBook::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBook::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBook::browseData ()";

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
    std::list <S_msrCredit>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    ++i
  ) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrBookElement>::const_iterator i = fBookElementsList.begin ();
    i != fBookElementsList.end ();
    ++i
  ) {
    // browse the part group
    msrBrowser<msrBookElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrBook::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrBook::printFull (std::ostream& os) const
{
  os <<
    "[MSR book" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 38;

  size_t bookElementsListSize =
    fBookElementsList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "bookElementsListSize" << ": " <<
    bookElementsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fBookNumberOfMeasures" << ": " <<
    fBookNumberOfMeasures <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fBookPartGroupNamesMaxLength" <<  ": " <<
    fBookElementNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fBookCreditTypePartNamesMaxLength" <<  ": " <<
    fBookElementNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fBookInstrumentNamesMaxLength" <<  ": " <<
    fBookInstrumentNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fBookInstrumentAbbreviationsMaxLength" <<  ": " <<
    fBookInstrumentAbbreviationsMaxLength <<
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
  size_t scoreAllVoicesListSize = fBookAllVoicesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "BookAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrVoice>::const_iterator
      iBegin = fBookAllVoicesList.begin (),
      iEnd   = fBookAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << std::endl;
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
  if (bookElementsListSize) {
    std::list <S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
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

void msrBook::print (std::ostream& os) const
{
  os <<
    "[MSR book, short version" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 38;

  size_t bookElementsListSize =
    fBookElementsList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "bookElementsListSize" << ": " <<
    bookElementsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fBookNumberOfMeasures" << ": " <<
    fBookNumberOfMeasures <<
    std::endl;

/*
  os << std::left <<
    std::setw (fieldWidth) <<
    "fBookPartGroupNamesMaxLength" <<  ": " <<
    fBookElementNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fBookCreditTypePartNamesMaxLength" <<  ": " <<
    fBookElementNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fBookInstrumentNamesMaxLength" <<  ": " <<
    fBookInstrumentNamesMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "fBookInstrumentAbbreviationsMaxLength" <<  ": " <<
    fBookInstrumentAbbreviationsMaxLength <<
    std::endl<<

    std::setw (fieldWidth) <<
    "inhibitGraceNotesGroupsBeforeBrowsing" <<  ": " <<
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

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }
*/

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

/*
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
  size_t scoreAllVoicesListSize = fBookAllVoicesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "BookAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrVoice>::const_iterator
      iBegin = fBookAllVoicesList.begin (),
      iEnd   = fBookAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << std::endl;
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
*/

  // print the part groups if any
  if (bookElementsListSize) {
    std::list <S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
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

void msrBook::printSummary (std::ostream& os) const
{
  os <<
    "[MSR book summary" <<
    std::endl << std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 38;

  size_t bookElementsListSize =
    fBookElementsList.size ();

  // print global information
  os << std::left <<
    std::setw (fieldWidth) <<
    "bookElementsListSize" << ": " <<
    bookElementsListSize <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fBookNumberOfMeasures" << ": " <<
    fBookNumberOfMeasures <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fBookInstrumentNamesMaxLength" <<  ": " <<
    fBookInstrumentNamesMaxLength <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fBookInstrumentAbbreviationsMaxLength" <<  ": " <<
    fBookInstrumentAbbreviationsMaxLength <<
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

  if (bookElementsListSize) {
    std::list <S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBook& elt)
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
