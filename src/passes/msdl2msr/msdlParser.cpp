/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>  // for setw()
#include <cstdlib>  // for stod()

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfStringsHandling.h"
#include "mfServices.h"

#include "oahEarlyOptions.h"

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
  msdlParser* obj =
    new msdlParser (
      inputStream);
  assert (obj != nullptr);
  return obj;
}

msdlParser::msdlParser (std::istream& inputStream)
    : fScanner (
        inputStream),
      fCurrentToken (
        fScanner.getCurrentToken ())
{
  // trace
#ifdef MF_TRACE_IS_ENABLED
  fTraceSyntax        =
    gGlobalMsdl2msrOahGroup->getTraceSyntax ();
  fTraceSyntaxDetails =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxDetails ();

  fTraceSyntaxErrorRecovery =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxErrorRecovery ();

  fTraceSyntaxErrorRecoveryDetails =
    gGlobalMsdl2msrOahGroup->getTraceSyntaxErrorRecoveryDetails ();
#endif // MF_TRACE_IS_ENABLED

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
    gMsrOahGroup->
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

  fNoteNotesDurationFIRST =
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

  gLog <<
    std::endl <<
    context << ": tokens stack contents (" <<
    mfSingularOrPlural (
      tokensSetsStackSize, "set", "sets") <<
    "):" <<
    " [" << context << ']' <<
    std::endl;

  if (tokensSetsStackSize) {
    std::list <S_msdlTokenKindsSet>::const_iterator
      iBegin = fMsdlTokensSetsStack.begin (),
      iEnd   = fMsdlTokensSetsStack.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      S_msdlTokenKindsSet tokenKindsSet = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
      mfAssert (
        __FILE__, __LINE__,
        tokenKindsSet != nullptr,
        "tokenKindsSet is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      gLog <<
        "v " <<
        tokenKindsSet->asString () <<
        std::endl;

      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }

  gLog <<
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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "-->isCurrentTokenKindInSetsStack()" <<
      context << ": tokens stack contents (" <<
      mfSingularOrPlural (
        tokensSetsStackSize, "set", "sets") <<
      "):" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      " [" << context << ']';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the stack cannot be empty JMI ???
  // since msdlTokenKind::kTokenEOF is in the set at the bottom of the stack

  if (tokensSetsStackSize) {
    std::list <S_msdlTokenKindsSet>::const_iterator
      iBegin = fMsdlTokensSetsStack.begin (),
      iEnd   = fMsdlTokensSetsStack.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msdlTokenKindsSet stoppersSet = ((*i));

  #ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntaxErrorRecoveryDetails) {
        gLog <<
          std::endl <<
          "-->isCurrentTokenKindInSetsStack()" <<
          ", fCurrentToken: " << currentTokenAsString () <<
          " [" << context << ']' <<
          std::endl;

        displayTokenKindsSetsStack ("---isCurrentTokenKindInSetsStack()");
      }
  #endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "*** fetchNextToken()" <<
      ", fCurrentToken: " << currentTokenAsString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "-->checkMandatoryTokenKind()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
      " [" << context << ']';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  Bool result (false);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkMandatoryTokenKind()");
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
        if (fTraceSyntaxErrorRecovery) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- checkMandatoryTokenKind()" <<
      ", context: [" << context << ']' <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
      ", result: " << result;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

//________________________________________________________________________
//  msdlParser::checkMandatoryTokenKindsSet
// --------------------------------------------------------------------------

Bool msdlParser::checkMandatoryTokenKindsSet (
  const std::string&       fileName,
  int                 lineNumber,
  const S_msdlTokenKindsSet& tokenKindsSet,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "-->checkMandatoryTokenKindsSet()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", context: [" << context << ']' <<
      ", fCurrentToken: " << currentTokenAsString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  Bool result (false);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkMandatoryTokenKindsSet()");
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
        if (fTraceSyntaxErrorRecovery) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
#endif // MF_TRACE_IS_ENABLED
        fetchNextToken ();

        if (tokenKindsSet->containsTokenKind (fCurrentTokenKind)) {
          // fCurrentTokenKind is in tokenKindsSet
          result = true;
          break;
        }
      } // while
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- checkMandatoryTokenKindsSet()" <<
      ", context: [" << context << ']' <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      ", result: " << result;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "--> checkOptionalTokenKind()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind);
  //    " context: " << context;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  Bool result (false);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkOptionalTokenKind()");
  }
#endif // MF_TRACE_IS_ENABLED

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

      std::list <S_msdlTokenKindsSet>::const_iterator
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

  #ifdef MF_TRACE_IS_ENABLED
        if (fTraceSyntaxErrorRecovery) {
          fParserWaeHandler->
            ignoringToken (fCurrentToken, context);
        }
  #endif // MF_TRACE_IS_ENABLED

        if (++i == iEnd) break;
        // no std::endl here
      } // for

      --gIndenter;
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- checkOptionalTokenKind()" <<
      ", tokenKind: " << msdlTokenKindAsString (tokenKind) <<
  //    ", context: " << context <<
      ", result: " << result;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

//________________________________________________________________________
//  msdlParser::checkOptionalTokenKindsSet
// --------------------------------------------------------------------------

Bool msdlParser::checkOptionalTokenKindsSet (
  const std::string&       fileName,
  int                 lineNumber,
  const S_msdlTokenKindsSet& tokenKindsSet,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "-->checkOptionalTokenKindsSet()" <<
      ", @" << mfBaseName (fileName) << ":" << lineNumber <<
      ", context: [" << context << ']' <<
      ", fCurrentToken: " << currentTokenAsString () <<
      ", tokenKindsSet: " << tokenKindsSet->asString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    displayTokenKindsSetsStack ("checkOptionalTokenKindsSet()");
  }
#endif // MF_TRACE_IS_ENABLED

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
  #ifdef MF_TRACE_IS_ENABLED
          if (fTraceSyntaxErrorRecovery) {
            fParserWaeHandler->
              ignoringToken (fCurrentToken, context);
          }
  #endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecoveryDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- checkOptionalTokenKindsSet()" <<
      ", context: [" << context << ']' <<
      ", tokenKindsSet: " << tokenKindsSet->asString () <<
      ", result: " << result;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

// --------------------------------------------------------------------------
// private score building methods
// --------------------------------------------------------------------------

void msdlParser::createMeasureNumber (
  const mfInputLineNumber& inputLineNumber,
  int measureNumber)
{
  createVoiceIfNeeded (inputLineNumber);

  std::stringstream ss;

  ss << measureNumber;

  // create the measure and append it to the voice
  fCurrentMeasure =
    fCurrentVoice->
      cascadeCreateAMeasureAndAppendItInVoice (
        inputLineNumber,
        333, //         previousMeasureEndInputLineNumber, 0.9.62
        ss.str (),
        msrMeasureImplicitKind::kMeasureImplicitKindNo);
}

void msdlParser::createVoiceIfNeeded (const mfInputLineNumber& inputLineNumber)
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

void msdlParser::createStaffIfNeeded (const mfInputLineNumber& inputLineNumber)
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

void msdlParser::createPartIfNeeded (const mfInputLineNumber& inputLineNumber)
{
  if (! fCurrentPart) {
    createPartGroupIfNeeded (inputLineNumber);

    // create the part
    ++fCurrentPartNumber;

    std::string partName =
      "Part_" + mfIntToEnglishWord (fCurrentPartNumber);

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

void msdlParser::createPartGroupIfNeeded (const mfInputLineNumber& inputLineNumber)
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

void msdlParser::createScoreIfNeeded (const mfInputLineNumber& inputLineNumber)
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

void msdlParser::createIdentificationIfNeeded (const mfInputLineNumber& inputLineNumber)
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
        gServiceRunData->getRunDateFull ();

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "--- createIdentificationIfNeeded()" <<
        ", RunDate: " << RunDate;

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fMsrIdentification->
      setIdentificationEncodingDate (
        inputLineNumber,
        RunDate);
  }
}

void msdlParser::createBookIfNeeded (const mfInputLineNumber& inputLineNumber)
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
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
      mfAssert (
        __FILE__, __LINE__,
        gGlobalLpsr2lilypondOahGroup->
        getFixedOctaveEntrySemiTonesPitchAndOctave () != nullptr,
       "gGlobalLpsr2lilypondOahGroup->getFixedOctaveEntrySemiTonesPitchAndOctave () is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      fCurrentOctaveEntryReference =
        msrNote::createNoteFromSemiTonesPitchAndOctave (
          K_MF_INPUT_LINE_UNKNOWN_,
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
        K_MF_INPUT_LINE_UNKNOWN_,
        gGlobalLpsr2lilypondOahGroup->
          getRelativeOctaveEntrySemiTonesPitchAndOctave ());
  }
  else {
    fCurrentOctaveEntryReference = nullptr;
    // the first note in the voice will become the initial reference
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    gLog <<
      "setCurrentOctaveEntryReferenceFromTheLilypondOah()" <<
      ", octaveEntryKind is" <<
      msrOctaveEntryKindAsString (gGlobalLpsr2lilypondOahGroup->getOctaveEntryKind ()) <<
      std::endl <<
      "Initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      gLog <<
        '\'' <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      gLog << "[NULL]";
    }
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
*/
}

// --------------------------------------------------------------------------
//  msdlParser::lilypondOctaveInRelativeEntryMode
// --------------------------------------------------------------------------

std::string msdlParser::lilypondOctaveInRelativeEntryMode (
  const S_msrNote& note)
{
/*
  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C
  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      note->
        noteDiatonicPitchKind (
          note->getInputLineNumber ());

  msrDiatonicPitchKind
    referenceDiatonicPitchKind =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKind (
          note->getInputLineNumber ());

  std::string
    referenceDiatonicPitchKindAsString =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKindAsString (
          note->getInputLineNumber ());

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    constexpr int fieldWidth = 28;

    gLog << std::left <<
      "lilypondOctaveInRelativeEntryMode() 1" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      ": " <<
      noteAboluteDiatonicOrdinal <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      ": " <<
      referenceDiatonicPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAbsoluteOctave" <<
      ": " <<
      msrOctaveKindAsString (referenceAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      ": " <<
      referenceAboluteDiatonicOrdinal <<
      std::endl << std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  // generate the octaves as needed
  if (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
    noteAboluteDiatonicOrdinal -= 4;
    while (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
      ss << "'";
      noteAboluteDiatonicOrdinal -= 7;
    } // while
  }

  else {
    noteAboluteDiatonicOrdinal += 4;
    while (noteAboluteDiatonicOrdinal <= referenceAboluteDiatonicOrdinal) {
      ss << ",";
      noteAboluteDiatonicOrdinal += 7;
    } // while
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    gLog <<
      "lilypondOctaveInRelativeEntryMode() 2" <<
      ", result: " << ss.str () <<
      std::endl << std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
  return ss.str ();
*/

  return std::string (""); // JMI 0.9.66
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    gLog << std::left <<
      "% noteAbsoluteOctave: " <<
      msrOctaveKindAsString (noteAbsoluteOctave) <<
      ", referenceAbsoluteOctave: " <<
      msrOctaveKindAsString (referenceAbsoluteOctave) <<
      ", referenceAbsoluteOctave: " <<
      absoluteOctavesDifference;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  // generate the octaves as needed
  switch (absoluteOctavesDifference) {
    case -12:
      ss << ",,,,,,";
      break;
    case -11:
      ss << ",,,,,,";
      break;
    case -10:
      ss << ",,,,,";
      break;
    case -9:
      ss << ",,,,,";
      break;
    case -8:
      ss << ",,,,";
      break;
    case -7:
      ss << ",,,,";
      break;
    case -6:
      ss << ",,,";
      break;
    case -5:
      ss << ",,,";
      break;
    case -4:
      ss << ",,";
      break;
    case -3:
      ss << ",,";
      break;
    case -2:
      ss << ",";
      break;
    case -1:
      ss << ",";
      break;
    case 0:
      break;
    case 1:
      ss << "'";
      break;
    case 2:
      ss << "''";
      break;
    case 3:
      ss << "'''";
      break;
    case 4:
      ss << "''''";
      break;
    case 5:
      ss << "'''''";
      break;
    case 6:
      ss << "''''''";
      break;
    case 7:
      ss << "'''''''";
      break;
    case 8:
      ss << "''''''''";
      break;
    case 9:
      ss << "'''''''''";
      break;
    case 10:
      ss << "''''''''''";
      break;
    case 11:
      ss << "'''''''''''";
      break;
    case 12:
      ss << "''''''''''''";
      break;
    default:
      ss << "!!!";
  } // switch

  return ss.str ();
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
#ifdef MF_TRACE_IS_ENABLED
    gLog <<
      "Input doesn't contain any token" <<
      std::endl;
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    // let's go ahead

    ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "==> parse()";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // do the parsing of the main non-terminal notions
    Specification (
      fTokenEOFTokenKindsSet);

    // are we at the and of the input?
    if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::k_TokenEOF,
      "Specification")
    ) {
#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "<== parse()" <<
          ", msdlTokenKind::k_TokenEOF has been reached" <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
    }

#ifdef MF_TRACE_IS_ENABLED
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
#endif // MF_TRACE_IS_ENABLED

    // should we display the MSR score?
    if (gMsrOahGroup->getDisplayMsr ()) { // JMI 0.9.67
      gLog <<
        std::endl <<
        "*** Built MSR score: ***" <<
        std::endl <<
        "========================" <<
        std::endl << std::endl;

      ++gIndenter;

      if (fCurrentScore) {
        gLog <<
          fCurrentScore <<
          std::endl;
      }
      else {
        gLog << "[NULL]" << std::endl;
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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("Specification");
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Identification()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Title
// --------------------------------------------------------------------------

void msdlParser::Title (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "=== Title()" <<
      ", title: \"" << title << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Title()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Composer
// --------------------------------------------------------------------------

void msdlParser::Composer (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "=== Composer()" <<
      ", composer: \"" << composer << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "=================================================================" <<
      std::endl <<
      "<-- Composer()" <<
      ", fCurrentToken: " << currentTokenAsString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Opus
// --------------------------------------------------------------------------

void msdlParser::Opus (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "=== Opus()" <<
      ", opus: \"" << opus << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Opus()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Identifier()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Pitches
// --------------------------------------------------------------------------

void msdlParser::Pitches (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          "=== Pitches()" <<
          ", pitchesLanguageName: \"" << pitchesLanguageName << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // set the pitches language name // also in the MSR identification ??? JMI
      fPitchesLanguageKind =
        msrQuarterTonesPitchesLanguageKindFromString (
          pitchesLanguageName);

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          "=== Pitches()" <<
          ", fPitchesLanguageKind: \"" <<
          fPitchesLanguageKind <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fetchNextToken ();
    }
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Pitches()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Octaves
// --------------------------------------------------------------------------

void msdlParser::Octaves (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          "=== Octaves()" <<
          ", pitchesLanguageName: \"" << pitchesLanguageName << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // set the pitches language name // also in the MSR identification ??? JMI
      fPitchesLanguageKind =
        msrQuarterTonesPitchesLanguageKindFromString (
          pitchesLanguageName);

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          "=== Octaves()" <<
          ", fPitchesLanguageKind: \"" <<
          fPitchesLanguageKind <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fetchNextToken ();
    }
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Pitches()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::PitchesLanguage
// --------------------------------------------------------------------------

void msdlParser::PitchesLanguage (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- PitchesLanguage()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::PitchesOctaveEntry
// --------------------------------------------------------------------------

void msdlParser::PitchesOctaveEntry (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- PitchesOctaveEntry()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Anacrusis
// --------------------------------------------------------------------------

void msdlParser::Anacrusis (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  if (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenAnacrusis,
      "Anacrusis")
  ) {
//  if (fCurrentTokenKind == msdlTokenKind::kTokenAnacrusis) {
#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "=== Anacrusis()" <<
        ", anacrusis: present";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // consume the anacrusis token
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Anacrusis()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Structure
// --------------------------------------------------------------------------

void msdlParser::Structure (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Structure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Book
// --------------------------------------------------------------------------

void msdlParser::Book (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Book()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Score
// --------------------------------------------------------------------------

void msdlParser::Score (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Score()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::PartGroup
// --------------------------------------------------------------------------

void msdlParser::PartGroup (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- PartGroup()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Part
// --------------------------------------------------------------------------

void msdlParser::Part (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Part()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Music
// --------------------------------------------------------------------------

void msdlParser::Music (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "=== Music()" <<
        ", musicName: \"" << musicName << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "=================================================================" <<
      std::endl <<
      "<-- Music()" <<
      ", fCurrentToken: " << currentTokenAsString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Fragment
// --------------------------------------------------------------------------

void msdlParser::Fragment (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          "=== Fragment()" <<
          ", Fragment: \"" << fragmentName << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- Fragment()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::MeasuresSequence
// --------------------------------------------------------------------------

void msdlParser::MeasuresSequence (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- MeasuresSequence()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Measure
// --------------------------------------------------------------------------

void msdlParser::Measure (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- Measure()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::MeasureNumber
// --------------------------------------------------------------------------

void msdlParser::MeasureNumber (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "=== MeasureNumber()" <<
        ", measureNumber: \"" << measureNumber << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<-- MeasureNumber()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Clef
// --------------------------------------------------------------------------

void msdlParser::Clef (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- Clef()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Key
// --------------------------------------------------------------------------

void msdlParser::Key (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- Key()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Time
// --------------------------------------------------------------------------

void msdlParser::Time (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;



  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- Time()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Note
// --------------------------------------------------------------------------

void msdlParser::Note (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // there should be a pitch name
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind = msrQuarterTonesPitchKind::kQTP_UNKNOWN_;

  if (checkMandatoryTokenKind (
    __FILE__, __LINE__,
    msdlTokenKind::kTokenName,
    "Note")
  ) {
    // get the title
    std::string pitchName =
      fCurrentToken.getTokenDescription ().getString ();

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "=== Note()" <<
        ", pitchName: \"" << pitchName << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    noteQuarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        fPitchesLanguageKind,
        pitchName);

#ifdef MF_TRACE_IS_ENABLED
    if (false) { // JMI
      std::stringstream ss;

      ss <<
        "=== Note()" <<
        ", noteQuarterTonesPitchKind: \"" <<
        msrQuarterTonesPitchKindAsString (noteQuarterTonesPitchKind) <<
        "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // consume the pitch indentifier
    fetchNextToken ();
  }

  // is there an octave indication?
#ifdef MF_TRACE_IS_ENABLED
  if (false) { // JMI
    std::stringstream ss;

    ss <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "------------------->>>>>>>>>>>>>>>> Note()" <<
      ", fCurrentToken: " << currentTokenAsString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // an octave indication is a sequence of commas or quotes, respectively,
  // or empty, in which case octave 3 (one below Midlle C) is implicit
  // we thus call OctaveIndication() unconditionally
  msrOctaveKind
    noteOctaveKind =
      OctaveIndication (
        stopperTokensSet);

  // fetch the note duration?
#ifdef MF_TRACE_IS_ENABLED
  if (false) { // JMI
    std::stringstream ss;

    ss <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "----------->>>>>>>>> Note()" <<
      ", fCurrentToken: " << currentTokenAsString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteNotesDurationFIRST,
      "Note")
  ) {
    NoteNotesDuration (
      stopperTokensSet);
  }

  // create the note
  std::stringstream ss;

  ss << fCurrentMeasureNumber;

  std::string currentMeasureNumberAsString = ss.str ();

  S_msrNote
    note =
      msrNote::createRegularNote (
        fCurrentToken.getTokenLineNumber (),
        currentMeasureNumberAsString,
        noteQuarterTonesPitchKind,
        noteOctaveKind,
        fCurrentNoteSoundingWholeNotes,
        fCurrentNoteDisplayWholeNotes,
        fCurrentNoteDotsNumber);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "=== Note()" <<
      ", note: " << note->asString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append it to the current measure
  fCurrentMeasure->
    appendNoteToMeasure (note);

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- Note()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::Pitch
// --------------------------------------------------------------------------

void msdlParser::Pitch (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "=== Pitch()" <<
        ", pitchName: \"" << pitchName << "\"";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // consume the pitch indentifier
    fetchNextToken ();
  }

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- Pitch()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlParser::OctaveIndication
// --------------------------------------------------------------------------

msrOctaveKind msdlParser::OctaveIndication (S_msdlTokenKindsSet stopperTokensSet)
{
  msrOctaveKind result = msrOctaveKind::kOctave_UNKNOWN_;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
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
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "=== OctaveIndication()" <<
        ", commasCounter: " << commasCounter <<
        ", quotesCounter: " << quotesCounter;

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (commasCounter > 0 && quotesCounter > 0) {
    fParserWaeHandler->
      mixedCommasAndQuotesInOctaveIndication ();
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceSyntax) {
      std::stringstream ss;

      ss <<
        "=== OctaveIndication()" <<
        ", result: " << msrOctaveKindAsString (result);

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- OctaveIndication()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl <<
      "=================================================================";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

// --------------------------------------------------------------------------
//  msdlParser::NoteNotesDuration
// --------------------------------------------------------------------------

void msdlParser::NoteNotesDuration (S_msdlTokenKindsSet stopperTokensSet)
{
  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.push_front (stopperTokensSet);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "=================================================================" <<
      std::endl <<
      "--> NoteNotesDuration()" <<
      ", fCurrentToken: " << currentTokenAsString () <<
      std::endl;
  }

  if (fTraceSyntaxErrorRecovery) {
    displayTokenKindsSetsStack ("NoteNotesDuration");
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // there should be an integer or a name such as "maxima"
  mfDurationKind notesDurationKind = mfDurationKind::kDuration_UNKNOWN_;

  if (
    checkOptionalTokenKindsSet (
      __FILE__, __LINE__,
      fNoteNotesDurationFIRST,
      "NoteNotesDuration")
  ) {
    if (fCurrentTokenKind == msdlTokenKind::kTokenInteger) {
      // get the fraction
      int durationInteger = fCurrentToken.getTokenDescription ().getInteger ();

      notesDurationKind =
        mfDurationKindFromInteger (
          fCurrentToken.getTokenLineNumber (),
          durationInteger);

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          "=== NoteNotesDuration()" <<
          ", durationInteger: \"" << durationInteger << "\"" <<
          ", notesDurationKind: \"" << mfDurationKindAsString (notesDurationKind) << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

    else if (checkMandatoryTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenName,
      "NoteNotesDuration")
    ) {
      // get the duration name
      std::string durationName =
        fCurrentToken.getTokenDescription ().getString ();

      notesDurationKind =
        mfDurationKindFromString (
          fCurrentToken.getTokenLineNumber (),
          durationName);

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceSyntax) {
        std::stringstream ss;

        ss <<
          "=== NoteNotesDuration()" <<
          ", durationName: \"" << durationName << "\"" <<
          ", notesDurationKind: \"" << mfDurationKindAsString (notesDurationKind) << "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
    }

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
    switch (notesDurationKind) {
      case mfDurationKind::kDuration_UNKNOWN_:
        fParserWaeHandler->
          malformedNoteNotesDuration ();
        break;
      default:
        ;
    } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    // consume the duration integer or name
    fetchNextToken ();
  } // while

  // are there dots?
  fCurrentNoteDotsNumber = 0;

  while (
    checkOptionalTokenKind (
      __FILE__, __LINE__,
      msdlTokenKind::kTokenDot,
      "NoteNotesDuration")
  ) {
    ++fCurrentNoteDotsNumber;

    // consume the dot
    fetchNextToken ();
  } // while

  fCurrentNoteSoundingWholeNotes =
    wholeNotesFromNotesDurationKindAndDotsNumber (
      notesDurationKind,
      fCurrentNoteDotsNumber);

  fCurrentNoteDisplayWholeNotes =
    mfDurationKindAsWholeNotes (
      notesDurationKind);

  --gIndenter;

  if (stopperTokensSet->getTokenKindsSetSize ()) {
    fMsdlTokensSetsStack.pop_front ();
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceSyntax) {
    std::stringstream ss;

    ss <<
      "<-- NoteNotesDuration()" <<
      ", fCurrentNoteSoundingWholeNotes: " << fCurrentNoteSoundingWholeNotes .asFractionString ()<<
      ", fCurrentNoteDisplayWholeNotes: " << fCurrentNoteDisplayWholeNotes.asFractionString () <<
      ", fCurrentNoteDotsNumber: " << fCurrentNoteDotsNumber;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


}
