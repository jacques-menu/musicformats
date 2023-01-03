/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>  // for setw()
#include <cstdlib>  // for stod()



#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "mfAssert.h"
#include "mfConstants.h"
#include "mfStringsHandling.h"
#include "mfServiceRunData.h"

#include "msrOah.h"

#include "msdl2msrOah.h"

#include "msdlConverterComponent.h"

#include "msdlParserWaeHandlersDutch.h"
#include "msdlParserWaeHandlersEnglish.h"
#include "msdlParserWaeHandlersFrench.h"
#include "msdlParserWaeHandlersGerman.h"
#include "msdlParserWaeHandlersItalian.h"
#include "msdlParserWaeHandlersSpanish.h"

#include "msdlParser.h"

#include "msdl2msrOah.h"


namespace MusicFormats
{

//________________________________________________________________________
S_msdlParser msdlParser::create (std::istream& inputStream)
{
  msdlParser* o =
    new msdlParser (
      inputStream);
  assert (o != nullptr);
  return o;
}

msdlParser::msdlParser (std::istream& inputStream)
    : fScanner (
        inputStream),
      fCurrentToken (
        fScanner.getCurrentToken ())
{
  // trace
#ifdef OAH_TRACING_IS_ENABLED
  fTraceSyntax        =
    gGlobalMsdl2msrOahGroup->getTraceSyntax ();
  fTraceSyntaxDetails =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxDetails ();

  fTraceSyntaxErrorRecovery =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxErrorRecovery ();

  fTraceSyntaxErrorRecoveryDetails =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxErrorRecoveryDetails ();
#endif

  // software
  fSoftware =
    std::string ("MSDL converter ")
      +
    createMsdlConverterConverterComponent ()->
      fetchComponentMostRecentVersionNumber ()->
        asString ();

  // user language
  fUserLanguageKind =
    gGlobalMsdl2msrOahGroup->
      getMsdlUserLanguageKind ();

  // keywords language
  fKeywordsInputLanguageKind =
    gGlobalMsdl2msrOahGroup->
      getMsdlKeywordsInputLanguageKind ();

  // pitches language
  fPitchesLanguageKind =
    gGlobalMsrOahGroup->
      getMsrQuarterTonesPitchesLanguageKind ();

  // pitches octaves
  fOctavesEntryKind =
    msrOctaveEntryKind::kOctaveEntryAbsolute; // default value

  // warnings and errors
  switch (gGlobalMsdl2msrOahGroup->getMsdlUserLanguageKind ()) {
    case msdlUserLanguageKind::kUserLanguageEnglish:
      fParserWaeHandler = msdlParserWaeHandlerEnglish::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageFrench:
      fParserWaeHandler = msdlParserWaeHandlerFrench::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageItalian:
      fParserWaeHandler = msdlParserWaeHandlerItalian::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageGerman:
      fParserWaeHandler = msdlParserWaeHandlerGerman::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageSpanish:
      fParserWaeHandler = msdlParserWaeHandlerSpanish::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageDutch:
      fParserWaeHandler = msdlParserWaeHandlerDutch::create ();
      break;
  } // switch

  // tokens
  initializeTokensHandling ();

  // syntax correctness
  fSourceIsSyntacticallyCorrect = true;

  // the MSR being built
  fAnacrusis = false;

  fCurrentScoreNumber = 0;
  fCurrentPartGroupNumber = 0;
  fCurrentPartNumber = 0;
  fCurrentStaffNumber = 0;
  fCurrentVoiceNumber = 0;
  fCurrentMeasureNumber = 0;
}

void msdlParser::initializeTokensHandling ()
{
  fIgnoreSeparatorTokensKind = msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensNo; // just to initialize it

  // the empty token kinds set
  fEmptyTokenKindsSet =
    msdlTokenKindsSet::create ();

  // the token kinds set containing only f_TokenEOF
  fTokenEOFTokenKindsSet =
    msdlTokenKindsSet::create ({
      msdlTokenKind::k_TokenEOF});

 // Note
  fNoteFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenName});

  fNoteFOLLOW =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMeasure});

  fPitchFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenName});

  fOctavesFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenOctaves});

  fOctaveIndicationFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenComma,
      msdlTokenKind::kTokenQuote});

  fNoteDurationFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenInteger,
      msdlTokenKind::kTokenName});

  // Clef
  fClefFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenClef});

  fClefFOLLOW =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenRightBracket});

  // Measure
  fMeasureFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMeasure});

  fMeasureFOLLOW =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenRightBracket});

  // MeasuresSequence
  fMeasuresSequenceFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMeasure});

  fMeasuresSequenceFOLLOW =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenRightBracket});

  // Music
  fMusicFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenMusic});

  // Part
  fPartFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fPartFOLLOW = fTokenEOFTokenKindsSet;

  // PartGroup
  fPartGroupFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fPartGroupFOLLOW = fTokenEOFTokenKindsSet;

  // Score
  fScoreFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fScoreFOLLOW = fTokenEOFTokenKindsSet;

  // Book
  fBookFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenBook,
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});
  fBookFOLLOW = fTokenEOFTokenKindsSet;

  // Structure
  fStructureFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenBook,
      msdlTokenKind::kTokenScore,
      msdlTokenKind::kTokenPartGroup,
      msdlTokenKind::kTokenPart,
      msdlTokenKind::kTokenMusic});

  fStructureFOLLOW = fEmptyTokenKindsSet;

  // Identification
  fIdentificationFIRST =
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenTitle,
      msdlTokenKind::kTokenComposer,
      msdlTokenKind::kTokenOpus});

  fIdentificationFOLLOW =
    fStructureFIRST
      +
    msdlTokenKindsSet::create ({
      msdlTokenKind::kTokenPitches});
}

msdlParser::~ msdlParser ()
{}

//________________________________________________________________________
void msdlParser::displayTokenKindsSetsStack (const std::string& context)
{
  size_t
    tokensSetsStackSize = fMsdlTokensSetsStack.size ();

  gLogStream <<
    std::endl <<
    context << ": tokens stack contents (" <<
    mfSingularOrPlural (
      tokensSetsStackSize, "set", "sets") <<
    "):" <<
    " [" << context << ']' <<
    std::endl;

  if (tokensSetsStackSize) {
    std::list<S_msdlTokenKindsSet>::const_iterator
      iBegin = fMsdlTokensSetsStack.begin (),
      iEnd   = fMsdlTokensSetsStack.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      S_msdlTokenKindsSet tokenKindsSet = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        tokenKindsSet != nullptr,
        "tokenKindsSet is null");

      gLogStream <<
        "v " <<
        tokenKindsSet->asString () <<
        std::endl;

      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }

  gLogStream <<
    "    ---------------------------------------" <<
    std::endl;
}

//________________________________________________________________________
std::string msdlParser::currentTokenAsString () const
{
  return
    fCurrentToken.asString ();
}

std::string msdlParser::currentTokenAsMsdlString () const
{
  return
    fCurrentToken.asMsdlString (
      fKeywordsInputLanguageKind,
      msdlCommentsTypeKind::kCommentsTypePercent);
}

//________________________________________________________________________
Bool msdlParser::isCurrentTokenKindInSetsStack (const std::string& context)
{
  Bool result (false);

  size_t
    tokensSetsStackSize = fMsdlTokensSetsStack.size ();

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "-->isCurrentTokenKindInSetsStack()" <<
      context << ": tokens stack contents (" <<
      mfSingularOrPlural (
        tokensSetsStackSize, "set", "sets") <<
      "):" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      " [" << context << ']' <<
      std::endl;
  }
#endif

  // the stack cannot be empty JMI ???
  // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

  if (tokensSetsStackSize) {
    std::list<S_msdlTokenKindsSet>::const_iterator
      iBegin = fMsdlTokensSetsStack.begin (),
      iEnd   = fMsdlTokensSetsStack.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msdlTokenKindsSet stoppersSet = ((*i));

  #ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntaxErrorRecoveryDetails) {
        gLogStream <<
          std::endl <<
          "-->isCurrentTokenKindInSetsStack()" <<
          ", fCurrentToken: " << currentTokenAsString () <<
          " [" << context << ']' <<
          std::endl;

        displayTokenKindsSetsStack ("---isCurrentTokenKindInSetsStack()");
      }
  #endif

      if (stoppersSet->containsTokenKind (fCurrentTokenKind)) {
        // found it
        break;
      }

      if (++i == iEnd) break;
      // no std::endl here
    } // for
  }

  return result;
}

// --------------------------------------------------------------------------
//  msdlParser::fetchNextToken
// --------------------------------------------------------------------------

void msdlParser::fetchNextToken ()
{
  fCurrentTokenKind =
    fScanner.fetchNextToken (
      fIgnoreSeparatorTokensKind);

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "*** fetchNextToken()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
//  msdlParser::checkMandatoryTokenKind
// --------------------------------------------------------------------------

Bool msdlParser::checkMandatoryTokenKind (
  const std::string& fileName,
  int           lineNumber,
  msdlTokenKind tokenKind,
  const std::string& context)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "-->checkMandatoryTokenKind()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
      " [" << context << ']' <<
      std::endl;
  }
#endif

  Bool result (false);

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkMandatoryTokenKind()");
  }
#endif

  // is the current token kind tokenKind?
  if (fCurrentTokenKind == tokenKind) {
    // yes, we found it
    result = true;
  }

  else {
    // no, error, fCurrentToken is not the one we expected
    fParserWaeHandler->
      unexpectedToken (fCurrentToken, context);

    if (isCurrentTokenKindInSetsStack (context)) {
      // don't overtake it, we stop at it
    }

    else {
      // consume tokens if needed until one is found that is either
      // tokenKind or in the stopperTokensSet sets in the stack,
      // in order to re-synchronize the parser with a token it is expecting

      // the stack cannot be empty
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

      while (true) {
        // let's ignore fCurrentToken
#ifdef OAH_TRACING_IS_ENABLED
        if (fTraceSyntaxErrorRecovery) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
#endif

        // fetch next token
        fetchNextToken ();

        if (fCurrentTokenKind == tokenKind) {
          // we found it
          result = true;
          break;
        }
      } // while
    }
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "<-- checkMandatoryTokenKind()" <<
      ", context: [" << context << ']' <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
      ", result: " << result <<
      std::endl;
  }
#endif

  return result;
}

//________________________________________________________________________
//  msdlParser::checkMandatoryTokenKindsSet
// --------------------------------------------------------------------------

Bool msdlParser:: checkMandatoryTokenKindsSet (
  const std::string&       fileName,
  int                 lineNumber,
  const S_msdlTokenKindsSet& tokenKindsSet,
  const std::string&       context)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "-->checkMandatoryTokenKindsSet()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", context: [" << context << ']' <<
      ", fCurrentToken: " << currentTokenAsString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      std::endl;
  }
#endif

  Bool result (false);

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkMandatoryTokenKindsSet()");
  }
#endif

  // is the current token kind in tokenKindsSet?
  if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
    // fCurrentTokenKind is in tokenKindsSet
    result = true;
  }

  else {
    // no, error, fCurrentToken is not the one we expected
    fParserWaeHandler->
      unexpectedToken (fCurrentToken, context);

    if (isCurrentTokenKindInSetsStack (context)) {
      // don't overtake it, we stop at it
    }

    else {
      // consume tokens if needed until one is found that is either
      // tokenKind or in the stopperTokensSet sets in the stack,
      // in order to re-synchronize the parser with a token it is expecting

      // the stack cannot be empty
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

      while (true) {
        // let's ignore fCurrentToken
#ifdef OAH_TRACING_IS_ENABLED
        if (fTraceSyntaxErrorRecovery) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
#endif
        fetchNextToken ();

        if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
          // fCurrentTokenKind is in tokenKindsSet
          result = true;
          break;
        }
      } // while
    }
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "<-- checkMandatoryTokenKindsSet()" <<
      ", context: [" << context << ']' <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      ", result: " << result <<
      std::endl;
  }
#endif

  return result;
}

//________________________________________________________________________
//  msdlParser::checkOptionalTokenKind
// --------------------------------------------------------------------------

Bool msdlParser::checkOptionalTokenKind (
  const std::string& fileName,
  int           lineNumber,
  msdlTokenKind tokenKind,
  const std::string& context)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "-->checkOptionalTokenKind()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
  //    " context: " << context <<
      std::endl;
  }
#endif

  Bool result (false);

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkOptionalTokenKind()");
  }
#endif

  if (fCurrentTokenKind == msdlTokenKind::k_TokenEOF) {
    result = false;
  }

  else {
    // is the current token kind tokenKind?
    if (fCurrentTokenKind == tokenKind) {
      // yes
      result = true;
    }

    else if (false) { // JMI
      // no

      // consume tokens until one is found that is either
      // tokenKind or in the stopperTokensSet sets in the stack,
      // in order to re-synchronize the parser with a token it is expecting

      // the stack cannot be empty
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

      std::list<S_msdlTokenKindsSet>::const_iterator
        iBegin = fMsdlTokensSetsStack.begin (),
        iEnd   = fMsdlTokensSetsStack.end (),
        i      = iBegin;

      ++gIndenter;

      // this loop cannot run forever
      // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack
      for ( ; ; ) {
        fetchNextToken ();

        if (fCurrentTokenKind == tokenKind) {
          result = true;
          break;
        }

        else {
          S_msdlTokenKindsSet stoppersSet = ((*i));

          if (stoppersSet->containsTokenKind (fCurrentTokenKind)) {
            // fCurrentTokenKind is in this set
            break;
          }
        }

  #ifdef OAH_TRACING_IS_ENABLED
        if (fTraceSyntaxErrorRecovery) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
  #endif

        if (++i == iEnd) break;
        // no std::endl here
      } // for

      --gIndenter;
    }
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "<-- checkOptionalTokenKind()" <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
  //    ", context: " << context <<
      ", result: " << result <<
      std::endl;
  }
#endif

  return result;
}

//________________________________________________________________________
//  msdlParser::checkOptionalTokenKindsSet
// --------------------------------------------------------------------------

Bool msdlParser:: checkOptionalTokenKindsSet (
  const std::string&       fileName,
  int                 lineNumber,
  const S_msdlTokenKindsSet& tokenKindsSet,
  const std::string&       context)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "-->checkOptionalTokenKindsSet()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", context: [" << context << ']' <<
      ", fCurrentToken: " << currentTokenAsString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      std::endl;
  }
#endif

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkOptionalTokenKindsSet()");
  }
#endif

  Bool result (false);

  if (fCurrentTokenKind == msdlTokenKind::k_TokenEOF) {
    result = false;
  }

  else {
    // is the current token kind in tokenKindsSet?
    if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
      // fCurrentTokenKind is in tokenKindsSet
      result = true;
    }

    else {
      // no, fCurrentToken is not the one we expected

      if (isCurrentTokenKindInSetsStack (context)) {
        // don't overtake it, we stop at it
      }

      else {
        // consume tokens if needed until one is found that is either
        // tokenKind or in the stopperTokensSet sets in the stack,
        // in order to re-synchronize the parser with a token it is expecting

        // the stack cannot be empty
        // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

        while (false) { // JMI BLARK
          // let's ignore fCurrentToken
  #ifdef OAH_TRACING_IS_ENABLED
          if (fTraceSyntaxErrorRecovery) {
            fParserWaeHandler->
              ignoringToken (fCurrentToken, context);
          }
  #endif
          fetchNextToken ();

          if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
            // fCurrentTokenKind is in tokenKindsSet
            result = true;
            break;
          }
        } // while
      }
    }
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    gLogStream <<
      std::endl <<
      "<-- checkOptionalTokenKindsSet()" <<
      ", context: [" << context << ']' <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      ", result: " << result <<
      std::endl;
  }
#endif

  return result;
}

// --------------------------------------------------------------------------
// private score building methods
// --------------------------------------------------------------------------

void msdlParser::createMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
  createVoiceIfNeeded (inputLineNumber);

  std::stringstream s;

  s << measureNumber;

  // create the measure and append it to the voice
  fCurrentMeasure =
    fCurrentVoice->
      createAMeasureAndAppendItToVoice (
        inputLineNumber,
        333, //         previousMeasureEndInputLineNumber, v0.9.62
        s.str (),
        msrMeasureImplicitKind::kMeasureImplicitKindNo);
}

void msdlParser::createVoiceIfNeeded (int inputLineNumber)
{
  if (! fCurrentStaff) {
    createStaffIfNeeded (inputLineNumber);

    // create the voice
    fCurrentVoice =
      msrVoice::createRegularVoice (
        inputLineNumber,
        ++fCurrentVoiceNumber,
        fCurrentStaff);

    // append it to the staff
    fCurrentStaff->
      registerVoiceInStaff (
        inputLineNumber,
        fCurrentVoice);
  }
}

void msdlParser::createStaffIfNeeded (int inputLineNumber)
{
  if (! fCurrentStaff) {
    createPartIfNeeded (inputLineNumber);

    // create the staff
    fCurrentStaff =
      msrStaff::create (
        __LINE__,
        msrStaffKind::kStaffKindRegular,
        ++fCurrentStaffNumber,
        fCurrentPart);

    // append it to the part
    fCurrentPart ->
      addStaffToPartCloneByItsNumber ( // JMI NOT clone???
        fCurrentStaff);
  }
}

void msdlParser::createPartIfNeeded (int inputLineNumber)
{
  if (! fCurrentPart) {
    createPartGroupIfNeeded (inputLineNumber);

    // create the part
    ++fCurrentPartNumber;

    std::string partName =
      "Part_" + mfInt2EnglishWord (fCurrentPartNumber);

    fCurrentPart =
      msrPart::create (
        inputLineNumber,
        partName,
        fCurrentPartGroup);

    // append it to the part group
    fCurrentPartGroup->
      appendPartToPartGroup (
        fCurrentPart);
  }
}

void msdlParser::createPartGroupIfNeeded (int inputLineNumber)
{
  if (! fCurrentPartGroup) {
    createScoreIfNeeded (inputLineNumber);

    // create the part group
    fCurrentPartGroup =
      msrPartGroup::create (
        inputLineNumber,
          1, 1,
          "OnlyPartGroup",
          nullptr,
          fCurrentScore);

    // add the part group to the score
    fCurrentScore->
      addPartGroupToScore (fCurrentPartGroup);
  }
}

void msdlParser::createScoreIfNeeded (int inputLineNumber)
{
  if (! fCurrentScore) {
    // create the book if needed
    createBookIfNeeded (inputLineNumber);

    // create the score
    fCurrentScore =
      msrScore::create (
        inputLineNumber,
        "msrScore::create()");

    // create the identification if needed
    createIdentificationIfNeeded (inputLineNumber);

    // register it in the score
    fCurrentScore->
      setIdentification (
        fMsrIdentification);
  }

  // add the score to the book
  fCurrentBook->
    addBookElementToBook (fCurrentScore);
}

void msdlParser::createIdentificationIfNeeded (int inputLineNumber)
{
  if (! fMsrIdentification) {
/* JMI
    fParserWaeHandler->
      multipleIdentifications ();
*/

    // create the MSR identification
    fMsrIdentification =
      msrIdentification::create (inputLineNumber);

    // append the MSDL converter as software to it
    fMsrIdentification->
      msrIdentification::appendSoftware (
        inputLineNumber,
        fSoftware);

    // set the encoding date in it
    std::string
      RunDate =
        gGlobalServiceRunData->getRunDateFull ();

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "--- createIdentificationIfNeeded()" <<
        ", RunDate: " << RunDate <<
        std::endl;
    }
#endif

    fMsrIdentification->
      setIdentificationEncodingDate (
        inputLineNumber,
        RunDate);
  }
}

void msdlParser::createBookIfNeeded (int inputLineNumber)
{
  if (! fCurrentBook) {
    fCurrentBook =
      msrBook::create (
        inputLineNumber,
        "msrBook::create()");
  }
}

//________________________________________________________________________
void msdlParser::setCurrentOctaveEntryReference ()
{
/*

  switch (gGlobalLpsr2lilypondOahGroup->getOctaveEntryKind ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      setCurrentOctaveEntryReferenceFromTheLilypondOah ();
      break;

    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      fCurrentOctaveEntryReference = nullptr;
      break;

    case msrOctaveEntryKind::kOctaveEntryFixed:
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        gGlobalLpsr2lilypondOahGroup->
        getFixedOctaveEntrySemiTonesPitchAndOctave () != nullptr,
       "gGlobalLpsr2lilypondOahGroup->getFixedOctaveEntrySemiTonesPitchAndOctave () is null");

      fCurrentOctaveEntryReference =
        msrNote::createNoteFromSemiTonesPitchAndOctave (
          K_MF_INPUT_LINE_UNKNOWN,
          gGlobalLpsr2lilypondOahGroup->getFixedOctaveEntrySemiTonesPitchAndOctave ());
      break;
  } // switch



  if (
    gGlobalLpsr2lilypondOahGroup->
      getRelativeOctaveEntrySemiTonesPitchAndOctave ()
  ) {
    // option '-rel, -relative' has been used:
    fCurrentOctaveEntryReference =
      msrNote::createNoteFromSemiTonesPitchAndOctave (
        K_MF_INPUT_LINE_UNKNOWN,
        gGlobalLpsr2lilypondOahGroup->
          getRelativeOctaveEntrySemiTonesPitchAndOctave ());
  }
  else {
    fCurrentOctaveEntryReference = nullptr;
    // the first note in the voice will become the initial reference
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream <<
      "setCurrentOctaveEntryReferenceFromTheLilypondOah()" <<
      ", octaveEntryKind is" <<
      msrOctaveEntryKindAsString (gGlobalLpsr2lilypondOahGroup->getOctaveEntryKind ()) <<
      std::endl <<
      "Initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      gLogStream <<
        "'" <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      gLogStream << "[NONE]";
    }
    gLogStream << std::endl;
  }
#endif
*/
}

// --------------------------------------------------------------------------
//  msdlParser::lilypondOctaveInRelativeEntryMode
// --------------------------------------------------------------------------

std::string msdlParser::lilypondOctaveInRelativeEntryMode (
  const S_msrNote& note)
{
/*
  int inputLineNumber =
    note->getInputLineNumber ();

  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C
  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      note->
        noteDiatonicPitchKind (
          inputLineNumber);

  msrDiatonicPitchKind
    referenceDiatonicPitchKind =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKind (
          inputLineNumber);

  std::string
    referenceDiatonicPitchKindAsString =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKindAsString (
          inputLineNumber);

  msrOctaveKind
    referenceAbsoluteOctave =
      fCurrentOctaveEntryReference->
        getNoteOctaveKind ();

  / *
    If no octave changing mark is used on a pitch, its octave is calculated
    so that the interval with the previous note is less than a fifth.
    This interval is determined without considering accidentals.
  * /

  int
    noteAboluteDiatonicOrdinal =
      (int) noteAbsoluteOctave * 7 // JMI FOO
        +
      noteDiatonicPitchKind - msrDiatonicPitchKind::kDiatonicPitchC,

    referenceAboluteDiatonicOrdinal =
      (int) referenceAbsoluteOctave * 7 // JMI FOO
        +
      referenceDiatonicPitchKind - msrDiatonicPitchKind::kDiatonicPitchC;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    const int fieldWidth = 28;

    gLogStream << std::left <<
      "lilypondOctaveInRelativeEntryMode() 1" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      " = " <<
      referenceDiatonicPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAbsoluteOctave" <<
       " = " <<
      msrOctaveKindAsString (referenceAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      " = " <<
      referenceAboluteDiatonicOrdinal <<
      std::endl << std::endl;
  }
#endif

  std::stringstream s;

  // generate the octaves as needed
  if (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
    noteAboluteDiatonicOrdinal -= 4;
    while (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
      s << "'";
      noteAboluteDiatonicOrdinal -= 7;
    } // while
  }

  else {
    noteAboluteDiatonicOrdinal += 4;
    while (noteAboluteDiatonicOrdinal <= referenceAboluteDiatonicOrdinal) {
      s << ",";
      noteAboluteDiatonicOrdinal += 7;
    } // while
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream <<
      "lilypondOctaveInRelativeEntryMode() 2" <<
      ", result: " << s.str () <<
      std::endl << std::endl;
  }
#endif
  return s.str ();
*/

  return std::string (""); // JMI v0.9.66
}

// --------------------------------------------------------------------------
//  msdlParser::lilypondOctaveInFixedEntryMode
// --------------------------------------------------------------------------

std::string msdlParser::lilypondOctaveInFixedEntryMode (
  const S_msrNote& note)
{
/*
  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C

  / *
    Pitches in fixed octave entry mode only need commas or quotes
    when they are above or below fCurrentOctaveEntryReference,
    hence when their octave is different than the latter's.
  * /

  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  msrOctaveKind
    referenceAbsoluteOctave =
      fCurrentOctaveEntryReference->
        getNoteOctaveKind ();

  int absoluteOctavesDifference =
    (int) noteAbsoluteOctave - (int) referenceAbsoluteOctave;

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream << std::left <<
      "% noteAbsoluteOctave = " <<
      msrOctaveKindAsString (noteAbsoluteOctave) <<
      ", referenceAbsoluteOctave = " <<
      msrOctaveKindAsString (referenceAbsoluteOctave) <<
      ", referenceAbsoluteOctave = " <<
      absoluteOctavesDifference <<
      std::endl;
  }
#endif

  std::stringstream s;

  // generate the octaves as needed
  switch (absoluteOctavesDifference) {
    case -12:
      s << ",,,,,,";
      break;
    case -11:
      s << ",,,,,,";
      break;
    case -10:
      s << ",,,,,";
      break;
    case -9:
      s << ",,,,,";
      break;
    case -8:
      s << ",,,,";
      break;
    case -7:
      s << ",,,,";
      break;
    case -6:
      s << ",,,";
      break;
    case -5:
      s << ",,,";
      break;
    case -4:
      s << ",,";
      break;
    case -3:
      s << ",,";
      break;
    case -2:
      s << ",";
      break;
    case -1:
      s << ",";
      break;
    case 0:
      break;
    case 1:
      s << "'";
      break;
    case 2:
      s << "''";
      break;
    case 3:
      s << "'''";
      break;
    case 4:
      s << "''''";
      break;
    case 5:
      s << "'''''";
      break;
    case 6:
      s << "''''''";
      break;
    case 7:
      s << "'''''''";
      break;
    case 8:
      s << "''''''''";
      break;
    case 9:
      s << "'''''''''";
      break;
    case 10:
      s << "''''''''''";
      break;
    case 11:
      s << "'''''''''''";
      break;
    case 12:
      s << "''''''''''''";
      break;
    default:
      s << "!!!";
  } // switch

  return s.str ();
  */

  return std::string ("");
}


// --------------------------------------------------------------------------
//  msdlParser::parse
// --------------------------------------------------------------------------

void msdlParser::parse ()
{
  // ignore separator tokens
  fIgnoreSeparatorTokensKind = msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensYes;

  gGlobalMsdl2msrOahGroup->
    setIgnoreSeparatorTokens (true); // JMI ???

  // fetch the first token
  fetchNextToken ();

  if (fCurrentTokenKind == msdlTokenKind::k_TokenEOF) {
#ifdef OAH_TRACING_IS_ENABLED
    gLogStream <<
      "Input doesn't contain any token" <<
      std::endl;
#endif
  }

  else {
    // let's go ahead

    ++gIndenter;

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "==> parse()" <<
        std::endl;
    }
#endif

    // do the parsing of the main non-terminal notions
    Specification (
      fTokenEOFTokenKindsSet);

    // are we at the and of the input?
    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::k_TokenEOF,
      "Specification")
    ) {
#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          std::endl <<
          "<== parse()" <<
          ", msdlTokenKind::k_TokenEOF has been reached" <<
          std::endl << std::endl;
      }
#endif
    }

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      if (fSourceIsSyntacticallyCorrect) {
        fParserWaeHandler->
          inputIsSyntacticallyCorrect ();
      }
      else {
        fParserWaeHandler->
          inputIsSyntacticallyIncorrect ();
      }
    }
#endif

    // should we display the MSR score?
    if (gGlobalMsrOahGroup->getDisplayMsr ()) { // JMI CAFE
      gLogStream <<
        std::endl <<
        "*** Built MSR score: ***" <<
        std::endl <<
        "========================" <<
        std::endl << std::endl;

      ++gIndenter;

      if (fCurrentScore) {
        gLogStream <<
          fCurrentScore <<
          std::endl;
      }
      else {
        gLogStream << "[NONE]" << std::endl;
      }

      --gIndenter;
    }

    --gIndenter;
  }
}

// --------------------------------------------------------------------------
//  msdlParser::Specification
// --------------------------------------------------------------------------

void msdlParser::Specification (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Specification");
  }
#endif

  ++gIndenter;

  // the first token is already available

  // the optional Identification section
  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fIdentificationFIRST,
      "Specification")
  ) {
    Identification (
      fStructureFIRST
        +
      msdlTokenKindsSet::create ({
        msdlTokenKind::kTokenPitches,
        msdlTokenKind::kTokenAnacrusis}));
  }

  // the optional PitchesLanguage section
  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenPitches,
      "Specification")
  ) {
    Pitches (
      fStructureFIRST
        +
      msdlTokenKindsSet::create ({
        msdlTokenKind::kTokenAnacrusis}));
  }

  // the optional Anacrusis section
  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenAnacrusis,
      "Specification")
  ) {
    Anacrusis (
      fStructureFIRST);
  }

  // the mandatory Structure section
  if (
    checkMandatoryTokenKindsSet (
      __FILE__, __LINE__,
      fStructureFIRST,
      "Specification")
  ) {
    Structure (
      fEmptyTokenKindsSet);
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }
}

// --------------------------------------------------------------------------
//  msdlParser::Identification
// --------------------------------------------------------------------------

void msdlParser::Identification (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Identification()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Identification");
  }
#endif

  ++gIndenter;

  createIdentificationIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // there can be a title, a composer and an opus
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fIdentificationFIRST,
      "Identification")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenTitle :
        Title (fIdentificationFOLLOW);
        break;
      case msdlTokenKind::kTokenComposer :
        Composer (fIdentificationFOLLOW);
        break;
      case msdlTokenKind::kTokenOpus :
        Opus (fIdentificationFOLLOW);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Identification()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Title
// --------------------------------------------------------------------------

void msdlParser::Title (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Title()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Title");
  }
#endif

  ++gIndenter;

  // consume the title token
  fetchNextToken ();

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenString,
    "Title")
  ) {
    // get the title
    std::string title = fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=== Title()" <<
      ", title: \"" << title << "\"" <<
      std::endl;
  }
#endif

    // set the title in the MSR identification
    fMsrIdentification->setIdentificationWorkCreditTypeTitle (
      fCurrentToken.getTokenLineNumber (),
      title);

    fetchNextToken ();
  }
  else {
//    fParserWaeHandler->stringExpectedAsTitle ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Title()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Composer
// --------------------------------------------------------------------------

void msdlParser::Composer (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Composer()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Composer");
  }
#endif

  ++gIndenter;

  // consume the composer token
  fetchNextToken ();

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenString,
    "Composer")
  ) {
    // get the composer
    std::string composer =
      fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=== Composer()" <<
      ", composer: \"" << composer << "\"" <<
      std::endl;
  }
#endif

    // append the composer to the MSR identification
    fMsrIdentification->appendComposer (
      fCurrentToken.getTokenLineNumber (),
      composer);

    fetchNextToken ();
  }
  else {
//    fParserWaeHandler->stringExpectedAsComposer ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=================================================================" <<
      std::endl <<
      "<-- Composer()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Opus
// --------------------------------------------------------------------------

void msdlParser::Opus (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "--> Opus()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Opus");
  }
#endif

  ++gIndenter;

  // consume the opus token
  fetchNextToken ();

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenString,
    "Opus")
  ) {
    // get the opus
    std::string opus =
      fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=== Opus()" <<
      ", opus: \"" << opus << "\"" <<
      std::endl;
  }
#endif

    // set the opus in the MSR identification
    fMsrIdentification->setIdentificationOpus (
      fCurrentToken.getTokenLineNumber (),
      opus);

    fetchNextToken ();
  }
  else {
//    fParserWaeHandler->stringExpectedAsOpus ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Opus()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Identifier
// --------------------------------------------------------------------------

void msdlParser::Identifier (S_msdlTokenKindsSet stopperTokensSet)
{
/*
  pitches such as c2 prevent identifiers from being tokens in MSDL
  as is usual in programming languages,
  so we handle actual identifiers such ar part1 at the syntax level
*/

  // accept a sequence of contiguous names and integers,

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Identifier()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Identifier");
  }
#endif



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Identifier()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Pitches
// --------------------------------------------------------------------------

void msdlParser::Pitches (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Pitches()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Pitches");
  }
#endif

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenPitches,
      "Pitches")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenPitches) {
    // consume the pitches token
    fetchNextToken ();

    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "Pitches")
    ) {
      // get the pitches language name
      std::string pitchesLanguageName =
        fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== Pitches()" <<
          ", pitchesLanguageName: \"" << pitchesLanguageName << "\"" <<
          std::endl;
      }
#endif

      // set the pitches language name // also in the MSR identification ??? JMI
      fPitchesLanguageKind =
        msrQuarterTonesPitchesLanguageKindFromString (
          pitchesLanguageName);

#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== Pitches()" <<
          ", fPitchesLanguageKind: \"" <<
          msrQuarterTonesPitchesLanguageKindAsString (fPitchesLanguageKind) <<
          "\"" <<
          std::endl;
      }
#endif

      fetchNextToken ();
    }
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Pitches()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Octaves
// --------------------------------------------------------------------------

void msdlParser::Octaves (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Pitches()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Octaves");
  }
#endif

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenOctaves,
      "Octaves")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenOctaves) {
    // consume the octaves token
    fetchNextToken ();

    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "Octaves")
    ) {
      // get the pitches language name
      std::string pitchesLanguageName =
        fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== Octaves()" <<
          ", pitchesLanguageName: \"" << pitchesLanguageName << "\"" <<
          std::endl;
      }
#endif

      // set the pitches language name // also in the MSR identification ??? JMI
      fPitchesLanguageKind =
        msrQuarterTonesPitchesLanguageKindFromString (
          pitchesLanguageName);

#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== Octaves()" <<
          ", fPitchesLanguageKind: \"" <<
          msrQuarterTonesPitchesLanguageKindAsString (fPitchesLanguageKind) <<
          "\"" <<
          std::endl;
      }
#endif

      fetchNextToken ();
    }
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Pitches()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::PitchesLanguage
// --------------------------------------------------------------------------

void msdlParser::PitchesLanguage (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> PitchesLanguage()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("PitchesLanguage");
  }
#endif



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- PitchesLanguage()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::PitchesOctaveEntry
// --------------------------------------------------------------------------

void msdlParser::PitchesOctaveEntry (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> PitchesOctaveEntry()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("PitchesOctaveEntry");
  }
#endif



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- PitchesOctaveEntry()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Anacrusis
// --------------------------------------------------------------------------

void msdlParser::Anacrusis (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Anacrusis()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  ++gIndenter;

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Anacrusis");
  }
#endif

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenAnacrusis,
      "Anacrusis")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenAnacrusis) {
#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== Anacrusis()" <<
        ", anacrusis: present" <<
        std::endl;
    }
#endif

    // consume the anacrusis token
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Anacrusis()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Structure
// --------------------------------------------------------------------------

void msdlParser::Structure (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Structure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Structure");
  }
#endif

  ++gIndenter;

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fStructureFIRST,
      "Structure")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenBook :
        Book (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenScore :
        Score (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenPartGroup :
        PartGroup (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenPart :
        Part (fStructureFOLLOW);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (fStructureFOLLOW);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Structure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Book
// --------------------------------------------------------------------------

void msdlParser::Book (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Book()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Book");
  }
#endif

  ++gIndenter;

  // create the book if needed
  createBookIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fBookFIRST,
      "Book")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenScore :
        Score (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPartGroup :
        PartGroup (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPart :
        Part (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Book()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Score
// --------------------------------------------------------------------------

void msdlParser::Score (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Score()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Score");
  }
#endif

  ++gIndenter;

  // create the score if needed
  createScoreIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fScoreFIRST,
      "Score")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenPartGroup :
        PartGroup (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenPart :
        Part (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Score()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::PartGroup
// --------------------------------------------------------------------------

void msdlParser::PartGroup (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> PartGroup()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("PartGroup");
  }
#endif

  ++gIndenter;

  // create the part group if needed
  createPartGroupIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fPartGroupFIRST,
      "PartGroup")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenPart :
        Part (stopperTokensSet);
        break;
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- PartGroup()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Part
// --------------------------------------------------------------------------

void msdlParser::Part (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Part()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Part");
  }
#endif

  ++gIndenter;

  // create the part if needed
  createPartIfNeeded (
    fCurrentToken.getTokenLineNumber ());

  // do the job
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fPartFIRST,
      "Part")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenMusic :
        Music (stopperTokensSet);
        break;
      default:
        ;
    } // switch
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Part()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Music
// --------------------------------------------------------------------------

void msdlParser::Music (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Music()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Music");
  }
#endif

  ++gIndenter;

  // consume the music token
  fetchNextToken ();

/* JMI
  // there can be a music name
  if (
    checkOptionalTokenKind (
      msdlTokenKind::kTokenName,
      "Music")
  ) {
    // get the music name
    std::string musicName =
      fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== Music()" <<
        ", musicName: \"" << musicName << "\"" <<
        std::endl;
    }
#endif
  }
*/

  // there should be a left bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenLeftBracket,
    "Music")
  ) {
    fetchNextToken ();
  }

  // there can be measures
  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fMeasuresSequenceFIRST,
      "Music")
  ) {
    MeasuresSequence (
      fMeasuresSequenceFOLLOW);
  }

  // there should be a right bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenRightBracket,
    "Music")
  ) {
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=================================================================" <<
      std::endl <<
      "<-- Music()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Fragment
// --------------------------------------------------------------------------

void msdlParser::Fragment (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Fragment()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Fragment");
  }
#endif

  ++gIndenter;

  // consume the fragment token
  fetchNextToken ();

  // there should be a left bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenLeftBracket,
    "Fragment")
  ) {
    fetchNextToken ();
  }

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenMeasure,
      "Fragment")
  ) {
    MeasuresSequence (
      fMeasuresSequenceFOLLOW);

    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "Fragment")
    ) {
      // get the fragment name
      std::string fragmentName =
        fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== Fragment()" <<
          ", Fragment: \"" << fragmentName << "\"" <<
          std::endl;
      }
#endif

/*
      // set the fragment in the MSR identification ??? JMI
      fMusicLanguageKind =
        msdlMusicLanguageKindFromString (
          musicName);

      fetchNextToken ();
*/    }
  }

  // there should be a right bracket
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenRightBracket,
    "Fragment")
  ) {
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- Fragment()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::MeasuresSequence
// --------------------------------------------------------------------------

void msdlParser::MeasuresSequence (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> MeasuresSequence()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("MeasuresSequence");
  }
#endif

  ++gIndenter;

  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fMeasuresSequenceFIRST,
      "MeasuresSequence")
  ) {
    Measure (
      fMeasureFOLLOW);
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- MeasuresSequence()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Measure
// --------------------------------------------------------------------------

void msdlParser::Measure (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Measure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Measure");
  }
#endif

  ++gIndenter;

  // there should be a '|'
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenMeasure,
    "Measure")
  ) {
    MeasureNumber (stopperTokensSet);
  }

  // accept a sequence of notes and bars
  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteFIRST
        +
      msdlTokenKind::kTokenFinalBar,
      "Measure")
  ) {
    if (fCurrentTokenKind == msdlTokenKind::kTokenFinalBar) {
      // consume final bar
      fetchNextToken ();
    }
    else {
      Note (
        fNoteFOLLOW);
    }
  } // while

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Measure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::MeasureNumber
// --------------------------------------------------------------------------

void msdlParser::MeasureNumber (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> MeasureNumber()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("MeasureNumber");
  }
#endif

  ++gIndenter;

  // consume the measure token
  fetchNextToken ();

  int measureNumber = 1; // TEMP JMI

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenInteger,
    "MeasureNumber")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenInteger) {
    // consume the left parenthesis token

    // get the measure number
    measureNumber = fCurrentToken.getTokenDescription ().getInteger ();

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== MeasureNumber()" <<
        ", measureNumber: \"" << measureNumber << "\"" <<
        std::endl;
    }
#endif

    // consume the measure integer token
    fetchNextToken ();
  }

  // create a measure
  createMeasureNumber (
    fCurrentToken.getTokenLineNumber (),
    measureNumber); // JMI ++fCurrentMeasureNumber);

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "<-- MeasureNumber()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Clef
// --------------------------------------------------------------------------

void msdlParser::Clef (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Clef()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Clef");
  }
#endif

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Clef()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Key
// --------------------------------------------------------------------------

void msdlParser::Key (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Key()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Key");
  }
#endif

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Key()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Time
// --------------------------------------------------------------------------

void msdlParser::Time (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Time()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Time");
  }
#endif

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Time()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Note
// --------------------------------------------------------------------------

void msdlParser::Note (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Note()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Note");
  }
#endif

  ++gIndenter;

  // get the note pitch input line number
  int inputLineNumber =
    fCurrentToken.getTokenLineNumber ();

  // there should be a pitch name
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind = msrQuarterTonesPitchKind::kQTP_UNKNOWN;

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenName,
    "Note")
  ) {
    // get the title
    std::string pitchName =
      fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== Note()" <<
        ", pitchName: \"" << pitchName << "\"" <<
        std::endl;
    }
#endif

    noteQuarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        fPitchesLanguageKind,
        pitchName);

#ifdef OAH_TRACING_IS_ENABLED
  if (false) { // JMI
      gLogStream <<
        "=== Note()" <<
        ", noteQuarterTonesPitchKind: \"" <<
        msrQuarterTonesPitchKindAsString (noteQuarterTonesPitchKind) <<
        "\"" <<
        std::endl;
    }
#endif

    // consume the pitch indentifier
    fetchNextToken ();
  }

  // is there an octave indication?
#ifdef OAH_TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "------------------->>>>>>>>>>>>>>>> Note()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }
#endif

  // an octave indication is a sequence of commas or quotes, respectively,
  // or empty, in which case octave 3 (one below Midlle C) is implicit
  // we thus call OctaveIndication() unconditionally
  msrOctaveKind
    noteOctaveKind =
      OctaveIndication (
        stopperTokensSet);

  // fetch the note duration?
#ifdef OAH_TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "----------->>>>>>>>> Note()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }
#endif

  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteDurationFIRST,
      "Note")
  ) {
    NoteDuration (
      stopperTokensSet);
  }

  // create the note
  std::stringstream s;

  s << fCurrentMeasureNumber;

  std::string currentMeasureNumberAsString = s.str ();

  S_msrNote
    note =
      msrNote::createRegularNote (
        inputLineNumber,
        currentMeasureNumberAsString,
        noteQuarterTonesPitchKind,
        noteOctaveKind,
        fCurrentNoteSoundingWholeNotes,
        fCurrentNoteDisplayWholeNotes,
        fCurrentNoteDotsNumber);

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "=== Note()" <<
      ", note: " << note->asString () <<
      std::endl;
  }
#endif

  // append it to the current measure
  fCurrentMeasure->
    appendNoteOrPaddingToMeasure (note);

 --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Note()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::Pitch
// --------------------------------------------------------------------------

void msdlParser::Pitch (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> Pitch()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Pitch");
  }
#endif

  ++gIndenter;

  // there should be an indentifier
  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenName,
    "Pitch")
  ) {
    // get the title
    std::string pitchName =
      fCurrentToken.getTokenDescription ().getString ();

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== Pitch()" <<
        ", pitchName: \"" << pitchName << "\"" <<
        std::endl;
    }
#endif

    // consume the pitch indentifier
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- Pitch()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif
}

// --------------------------------------------------------------------------
//  msdlParser::OctaveIndication
// --------------------------------------------------------------------------

msrOctaveKind msdlParser::OctaveIndication (S_msdlTokenKindsSet stopperTokensSet)
{
  msrOctaveKind result = msrOctaveKind::kOctave_UNKNOWN;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> OctaveIndication()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("OctaveIndication");
  }
#endif

  ++gIndenter;

  // there can be commas and single quotes
  int commasCounter = 0;
  int quotesCounter = 0;

  while (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fOctaveIndicationFIRST,
      "OctaveIndication")
  ) {
    // do the Identification parsing
    switch (fCurrentTokenKind) {
      case msdlTokenKind::kTokenComma :
        ++commasCounter;
        break;
      case msdlTokenKind::kTokenQuote :
        ++quotesCounter;
        break;
      default:
        ;
    } // switch

    // consume the comma or quote
    fetchNextToken ();
  } // while

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== OctaveIndication()" <<
        ", commasCounter: " << commasCounter <<
        ", quotesCounter: " << quotesCounter <<
        std::endl;
    }
#endif

  // sanity check
  if (commasCounter > 0 && quotesCounter > 0) {
    fParserWaeHandler->
      mixedCommasAndQuotesInOctaveIndication ();
  }

  // compute result - the octave starting at Middle C (c') has number 4
  if (commasCounter > 0) {
    result =
      msrOctaveKind (
        (int) msrOctaveKind::kOctave3 - commasCounter);
  }
  else if (quotesCounter > 0) {
    result =
      msrOctaveKind (
        (int) msrOctaveKind::kOctave3 + quotesCounter);
  }
  else {
    result = msrOctaveKind::kOctave3;
  }

#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceSyntax) {
      gLogStream <<
        "=== OctaveIndication()" <<
        ", result: " << msrOctaveKindAsString (result) <<
        std::endl;
    }
#endif

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- OctaveIndication()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================" <<
      std::endl;
  }
#endif

  return result;
}

// --------------------------------------------------------------------------
//  msdlParser::NoteDuration
// --------------------------------------------------------------------------

void msdlParser::NoteDuration (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> NoteDuration()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("NoteDuration");
  }
#endif

  ++gIndenter;

  // there should be an integer or a name such as "maxima"
  msrDurationKind durationKind = msrDurationKind::kDuration_UNKNOWN;

  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteDurationFIRST,
      "NoteDuration")
  ) {
    if (fCurrentTokenKind == msdlTokenKind::kTokenInteger) {
      // get the fraction
      int durationInteger = fCurrentToken.getTokenDescription ().getInteger ();

      durationKind =
        msrDurationKindFromInteger (
          fCurrentToken.getTokenLineNumber (),
          durationInteger);

#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== NoteDuration()" <<
          ", durationInteger: \"" << durationInteger << "\"" <<
          ", durationKind: \"" << msrDurationKindAsString (durationKind) << "\"" <<
          std::endl;
      }
#endif
    }

    else if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "NoteDuration")
    ) {
      // get the duration name
      std::string durationName =
        fCurrentToken.getTokenDescription ().getString ();

      durationKind =
        msrDurationKindFromString (
          fCurrentToken.getTokenLineNumber (),
          durationName);

#ifdef OAH_TRACING_IS_ENABLED
      if (fTraceSyntax) {
        gLogStream <<
          "=== NoteDuration()" <<
          ", durationName: \"" << durationName << "\"" <<
          ", durationKind: \"" << msrDurationKindAsString (durationKind) << "\"" <<
          std::endl;
      }
#endif
    }

    // sanity check
    switch (durationKind) {
      case msrDurationKind::kDuration_UNKNOWN:
        fParserWaeHandler->
          malformedNoteDuration ();
        break;
      default:
        ;
    } // switch

    // consume the duration integer or name
    fetchNextToken ();
  } // while

  // are there dots?
  fCurrentNoteDotsNumber = 0;

  while (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenDot,
      "NoteDuration")
  ) {
    ++fCurrentNoteDotsNumber;

    // consume the dot
    fetchNextToken ();
  } // while

  fCurrentNoteSoundingWholeNotes =
    rationalFromDurationKindAndDotsNumber (
      durationKind,
      fCurrentNoteDotsNumber);

  fCurrentNoteDisplayWholeNotes =
    msrDurationKindAsWholeNotes (
      durationKind);

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceSyntax) {
    gLogStream <<
      "<-- NoteDuration()" <<
      ", fCurrentNoteSoundingWholeNotes: " << fCurrentNoteSoundingWholeNotes <<
      ", fCurrentNoteDisplayWholeNotes: " << fCurrentNoteDisplayWholeNotes <<
      ", fCurrentNoteDotsNumber: " << fCurrentNoteDotsNumber <<
      std::endl;
  }
#endif
}


}
