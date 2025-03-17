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

#include "mfServices.h"
#include "mfExceptions.h"
#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "waeInterface.h"

#include "mfPreprocessorSettings.h"

#include "msdlScannerWaeHandlers.h"

#include "msdlScanner.h"

#include "msdl2msrOah.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string msdlIgnoreSeparatorTokensKindAsString (
  msdlIgnoreSeparatorTokensKind ignoreSeparatorTokensKind);
void displayStreamState (const std::ios& stream) {
  gLog <<
    "good (): " << stream.good () <<
    std::endl <<
    "eof (): " << stream.eof () <<
    std::endl <<
    "fail (): " << stream.fail () <<
    std::endl <<
    "bad (): " << stream.bad () <<
    std::endl;
}

//________________________________________________________________________
std::string msdlIgnoreSeparatorTokensKindAsString (
  msdlIgnoreSeparatorTokensKind ignoreSeparatorTokensKind)
{
  std::string result;

  switch (ignoreSeparatorTokensKind) {
    case msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensNo:
      result = "ignoreSeparatorTokensNo";
      break;
    case msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensYes:
      result = "ignoreSeparatorTokensYes";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
S_msdlScanner msdlScanner::create (std::istream& inputStream)
{
  msdlScanner* obj =
    new msdlScanner (
      inputStream);
  assert (obj != nullptr);
  return obj;
}

msdlScanner::msdlScanner (std::istream& inputStream)
    : fInputStream (
        inputStream),
      fCurrentToken (
        ),
      fCurrentTokenKind (
        fCurrentToken.getTokenKindNonConst ()),
      fCurrentTokenDescription (
        fCurrentToken.getTokenDescriptionNonConst ())
{
  // trace
#ifdef MF_TRACE_IS_ENABLED
  fTraceTokens = gGlobalMsdl2msrOahGroup->getTraceTokens ();
  fTraceTokensDetails = gGlobalMsdl2msrOahGroup->getTraceTokensDetails ();
#endif // MF_TRACE_IS_ENABLED

  // user language
  fUserLanguageKind =
    gGlobalMsdl2msrOahGroup->getMsdlUserLanguageKind ();

  // input stream
  fInputIsEmpty = true;

  // input std::string
  fInputStringSize = 0;

  // lines
  fCurrentLineNumber = 1;
  fCurrentLineSize = -1;

  fCurrentLinePositionInInput = 0;

  fCurrentPositionInLine = 0;

  // characters
  fCurrentPositionInInput = -1;

  fCurrentCharacter = '@'; // any illegal character, just to get it initialized
  fNextCharacterIsAvailable = false;

  // tokens
  fCurrentTokenPositionInInput = -1;

  fCurrentTokenLineNumber = 1;
  fCurrentTokenPositionInLine = 0;

  fTokensCounter = 0;

  // keywords
  fKeywordsInputLanguageKind =
    gGlobalMsdl2msrOahGroup->getMsdlKeywordsInputLanguageKind ();

  // tokens list
  fAppendTokensToList = false;

  // warnings and errors
  switch (fUserLanguageKind) {
    case msdlUserLanguageKind::kUserLanguageEnglish:
      fScannerWaeHandler = msdlScannerWaeHandlerEnglish::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageFrench:
      fScannerWaeHandler = msdlScannerWaeHandlerFrench::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageItalian:
      fScannerWaeHandler = msdlScannerWaeHandlerItalian::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageGerman:
      fScannerWaeHandler = msdlScannerWaeHandlerGerman::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageSpanish:
      fScannerWaeHandler = msdlScannerWaeHandlerSpanish::create ();
      break;
    case msdlUserLanguageKind::kUserLanguageDutch:
      fScannerWaeHandler = msdlScannerWaeHandlerDutch::create ();
      break;
  } // switch

  // lexical correctness
  fSourceIsLexicallyCorrect = true;

  populateInputString (fInputStream);
}

msdlScanner::~ msdlScanner ()
{}

// --------------------------------------------------------------------------
//  msdlScanner::populateInputString
// --------------------------------------------------------------------------

void msdlScanner::populateInputString (std::istream& inputStream)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "Populating the input std::string";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for ( ; ; ) {
    char character;

    inputStream.get (character);

    if (inputStream.eof () || inputStream.fail ()) {
      break;
    }

    fInputString.push_back (character);
  } // for

  fInputStringSize = int (fInputString.size ());

  fInputIsEmpty = fInputStringSize == 0;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "inputStringSize: " << fInputStringSize <<
      std::endl <<
      "inputString: \"" <<
      fInputString <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlScanner::currentCharacterAsString
// --------------------------------------------------------------------------

std::string msdlScanner::currentCharacterAsString () const
{
  std::stringstream ss;

  if (fCurrentCharacter == EOF) {
    ss << "EOF";
  }
  else {
    ss << '\'' << fCurrentCharacter << "'";
  }

  return ss.str ();
}

// --------------------------------------------------------------------------
//  msdlScanner::fetchNextCharacter
// --------------------------------------------------------------------------

char msdlScanner::fetchNextCharacter ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "--> fetchNextCharacter()" <<
      ", fCurrentLineNumber: " <<
      fCurrentLineNumber <<
      ", fNextCharacterIsAvailable: " <<
      fNextCharacterIsAvailable <<
      ", fCurrentPositionInInput: " <<
      fCurrentPositionInInput <<
      ", fCurrentTokenPositionInInput: " <<
      fCurrentTokenPositionInInput <<
      ", fCurrentLinePositionInInput: " <<
      fCurrentLinePositionInInput <<
      ", fCurrentPositionInLine: " <<
      fCurrentPositionInLine;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // has the end of input been reached?
  if (fCurrentPositionInInput == fInputStringSize - 1) {
    fCurrentCharacter = EOF;
  }
  else {
    // update positions
    ++fCurrentPositionInInput;
    fCurrentLinePositionInInput = fCurrentPositionInInput + 1;
    ++fCurrentPositionInLine;

    // fetch new current character
    fCurrentCharacter = fInputString [fCurrentPositionInInput];
  }

  fNextCharacterIsAvailable = true;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "<-- fetchNextCharacter() returns: " <<
      currentCharacterAsString ();

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return fCurrentCharacter;
}

// --------------------------------------------------------------------------
//  msdlScanner::handleEndOfLine
// --------------------------------------------------------------------------

void msdlScanner::handleEndOfLine (const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "--> handleEndOfLine(): " <<
      ", context: " << context <<
      ", fCurrentLineNumber: " << fCurrentLineNumber <<
      ", fCurrentLinePositionInInput: " << fCurrentLinePositionInInput <<
      ", fCurrentPositionInLine: " << fCurrentPositionInLine;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++fCurrentLineNumber;
  fCurrentLinePositionInInput = fCurrentPositionInInput + 1;

  fCurrentPositionInLine = 0;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "<-- handleEndOfLine(): " <<
      ", context: " << context <<
      ", fCurrentLineNumber: " << fCurrentLineNumber <<
      ", fCurrentLinePositionInInput: " << fCurrentLinePositionInInput <<
      ", fCurrentPositionInLine: " << fCurrentPositionInLine <<
      std::endl << std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlScanner::fetchNextToken
// --------------------------------------------------------------------------

msdlTokenKind msdlScanner::fetchNextToken (
  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokens)
{
  ++fTokensCounter;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokens) {
    std::stringstream ss;

    ss <<
      std::endl <<
      fTokensCounter << " ==> fetchNextToken()" <<
      ", ignoreSeparatorTokens: " <<
      msdlIgnoreSeparatorTokensKindAsString (ignoreSeparatorTokens) <<
      std::endl;

    ++gIndenter;

    int fieldWidth = 28;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fNextCharacterIsAvailable: " << ": " <<
      fNextCharacterIsAvailable <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentPositionInInput" << ": " << fCurrentPositionInInput <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentPositionInLine" << ": " << fCurrentPositionInLine <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fNextCharacterIsAvailable" << ": " << fNextCharacterIsAvailable <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // fetch the first token character if needed
  if (! fNextCharacterIsAvailable) {
    fetchNextCharacter ();
  }

  // do the actual work at least once, ignoring separator tokens if needed
  for ( ; ; ) {

    // put the token position aside
    fCurrentTokenPositionInInput = fCurrentPositionInInput;

    fCurrentTokenLineNumber = fCurrentLineNumber;
    fCurrentTokenPositionInLine = fCurrentPositionInLine;

    switch (fCurrentCharacter) {
      case EOF:
#ifdef MF_TRACE_IS_ENABLED
        if (fTraceTokensDetails) {
          gLog << "EOF has been reached" << std::endl;
        }
#endif // MF_TRACE_IS_ENABLED

        fCurrentTokenKind = msdlTokenKind::k_TokenEOF;
        fNextCharacterIsAvailable = true; // don't go ahead anymore
        break;

      case ' ' :
        fCurrentTokenKind = msdlTokenKind::kTokenSpace;
        fNextCharacterIsAvailable = false;
        break;
      case '\t' :
        fCurrentTokenKind = msdlTokenKind::kTokenTabSeparator;
        fNextCharacterIsAvailable = false;
        break;
      case '\xD' :
        fCurrentTokenKind = msdlTokenKind::kTokenCarriageReturn;
        fNextCharacterIsAvailable = false;
        break;

      case '\n' :
        fCurrentTokenKind = msdlTokenKind::kTokenEndOfLine;
        fNextCharacterIsAvailable = false;

        handleEndOfLine ("regular \\n");
        break;

      case '/':
        handleSlash ();
        break;

      case '%':
        handlePercent ();
        break;

      case '=' :
        fCurrentTokenKind = msdlTokenKind::kTokenEqualSign;
        fNextCharacterIsAvailable = false;
        break;

      case ',' :
        fCurrentTokenKind = msdlTokenKind::kTokenComma;
        fNextCharacterIsAvailable = false;
        break;

      case ':' :
        fetchNextCharacter ();

        if (fCurrentCharacter == '|') {
          fetchNextCharacter (); // ':|' up to this point

          if (fCurrentCharacter == '|') { // ':||' up to this point
            fCurrentTokenKind = msdlTokenKind::kTokenFinalBar;
            fNextCharacterIsAvailable = false;
          }
          else {
            fScannerWaeHandler->illegalCharacters (":|");
            fSourceIsLexicallyCorrect = false;
            fNextCharacterIsAvailable = true;
          }
        }
        else {
          fCurrentTokenKind = msdlTokenKind::kTokenColon;
          fNextCharacterIsAvailable = true;
        }
        break;

      case ';' :
        fCurrentTokenKind = msdlTokenKind::kTokenSemiColon;
        fNextCharacterIsAvailable = false;
        break;

      case '+' :
        fCurrentTokenKind = msdlTokenKind::kTokenPlus;
        fNextCharacterIsAvailable = false;
        break;
      case '-' :
        fCurrentTokenKind = msdlTokenKind::kTokenMinus;
        fNextCharacterIsAvailable = false;
        break;
      case '*' :
        fCurrentTokenKind = msdlTokenKind::kTokenStar;
        fNextCharacterIsAvailable = false;
        break;

      case '!':
        fetchNextCharacter ();

        if (fCurrentCharacter == '!') { // '!!' up to this point
          fCurrentTokenKind = msdlTokenKind::kTokenConcat;
          fNextCharacterIsAvailable = false;
        }
        else {
          fScannerWaeHandler->illegalCharacter ('!');
          fSourceIsLexicallyCorrect = false;
          fNextCharacterIsAvailable = true;
        }
        break;

      case '?' :
        fCurrentTokenKind = msdlTokenKind::kTokenQuestionMark;
        fNextCharacterIsAvailable = false;
        break;

      case '~' :
        fCurrentTokenKind = msdlTokenKind::kTokenTilda;
        fNextCharacterIsAvailable = false;
        break;

      case '<' :
        fCurrentTokenKind = msdlTokenKind::kTokenLeftAngle;
        fNextCharacterIsAvailable = false;
        break;
      case '>' :
        fCurrentTokenKind = msdlTokenKind::kTokenRightAngle;
        fNextCharacterIsAvailable = false;
        break;

      case '(' :
        fCurrentTokenKind = msdlTokenKind::kTokenLeftParenthesis;
        fNextCharacterIsAvailable = false;
        break;
      case ')' :
        fCurrentTokenKind = msdlTokenKind::kTokenRightParenthesis;
        fNextCharacterIsAvailable = false;
        break;

      case '{' :
        fCurrentTokenKind = msdlTokenKind::kTokenLeftBracket;
        fNextCharacterIsAvailable = false;
        break;
      case '}' :
        fCurrentTokenKind = msdlTokenKind::kTokenRightBracket;
        fNextCharacterIsAvailable = false;
        break;

     case '\'':
        fCurrentTokenKind = msdlTokenKind::kTokenQuote;
        fNextCharacterIsAvailable = false;
        break;

      case '.' :
        fCurrentTokenKind = msdlTokenKind::kTokenDot;
        fNextCharacterIsAvailable = false;
        break;

       case '|' :
        fetchNextCharacter ();

        if (fCurrentCharacter == '|') { // '||' up to this point
          fetchNextCharacter ();

          if (fCurrentCharacter == '|') { // '|||' up to this point
            fCurrentTokenKind = msdlTokenKind::kTokenFinalBar;
            fNextCharacterIsAvailable = false;
          }
          else {
            fCurrentTokenKind = msdlTokenKind::kTokenDoubleBar;
            fNextCharacterIsAvailable = true;
          }
        }
        else {
          fCurrentTokenKind = msdlTokenKind::kTokenMeasure;
          fNextCharacterIsAvailable = true;
        }
        break;

      case '"': //  double-quoted std::string
        acceptAString ();
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        acceptAnInteger ();
        break;

      default:
        if (isalpha (fCurrentCharacter)) { // name
          acceptAName ();
        }

        else { // illegal character
          fScannerWaeHandler->illegalCharacter (fCurrentCharacter);
          fSourceIsLexicallyCorrect = false;

          std::stringstream ss;

          ss << fCurrentCharacter;

          fCurrentTokenKind = msdlTokenKind::k_TokenMalformed;
          fCurrentTokenDescription.setString (ss.str ());

          fNextCharacterIsAvailable = false;
        }
    } // switch (fCurrentCharacter)

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceTokensDetails) {
      std::stringstream ss;

      ss <<
        fCurrentToken.asString ();

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // should we loop once more?
    Bool onceMore;

    switch (fCurrentTokenKind) {
      case msdlTokenKind::k_TokenEOF:
        break;

      case msdlTokenKind::kToken_UNKNOWN_:
        // should not occur
        break;

      case msdlTokenKind::k_TokenMalformed:
        onceMore = true; // false for error recovery??? JMI
        break;

      // separators, for use by MSDL whole input translation
      // ------------------------------------

      case msdlTokenKind::kTokenSpace:
      case msdlTokenKind::kTokenTabSeparator:
      case msdlTokenKind::kTokenCarriageReturn:
      case msdlTokenKind::kTokenEndOfLine:
      case msdlTokenKind::kTokenParenthesizedComment:
      case msdlTokenKind::kTokenCommentToEndOfLine:
        onceMore =
          ignoreSeparatorTokens
            ==
          msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensYes;
        break;

      default:
        ;
    } // switch

#ifdef MF_TRACE_IS_ENABLED
    if (fTraceTokensDetails) {
      std::stringstream ss;

      ss <<
        "--- fetchNextToken(): onceMore: " << onceMore;

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    if (onceMore) {
      if (! fNextCharacterIsAvailable) {
        // fetch the next character
        fetchNextCharacter ();
      }

      // loop once more
    }

    else {
      break; // quit the loop
    }

  } // for

  --gIndenter;

  // register current token position
  fCurrentToken.setTokenLineNumber (fCurrentTokenLineNumber);
  fCurrentToken.setTokenPositionInLine (fCurrentTokenPositionInLine);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokens) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "#" << fTokensCounter << " <== fetchNextToken()" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "fCurrentToken: " <<
      std::endl;

    ++gIndenter;
    gLog << fCurrentToken;
    --gIndenter;

    int fieldWidth = 28;

    gLog << std::left <<
      std::setw (fieldWidth) <<
      "fNextCharacterIsAvailable: " << ": " <<
      fNextCharacterIsAvailable <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentPositionInInput" << ": " << fCurrentPositionInInput <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentPositionInLine" << ": " << fCurrentPositionInLine <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
      std::endl <<
      std::setw (fieldWidth) <<
      "fNextCharacterIsAvailable" << ": " << fNextCharacterIsAvailable <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // should the current token be appended to the tokens list?
  if (fAppendTokensToList) {
    fTokensList.appendTokenToTokensList (fCurrentToken);
  }

  return fCurrentTokenKind;
}

// --------------------------------------------------------------------------
//  msdlScanner::handlePercent
// --------------------------------------------------------------------------

void msdlScanner::handlePercent ()
{
  // consume the '%'
  fetchNextCharacter ();

  switch (fCurrentCharacter) {
    case '{':  // '%{' up to this point, parenthesized comment
      {
        fetchNextCharacter ();

        int percentParenthesizedCommentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '%') {
            if (fCurrentCharacter == '\n') {
              handleEndOfLine ("percent parenthesized comment");

              fNextCharacterIsAvailable = false;
            } // while

            // consume this comment character
            fetchNextCharacter ();
          } // while

          // consume the comment character
          fetchNextCharacter ();
        } // do
        while (fCurrentCharacter != '}');

        // the end of the comment has been reached
        fNextCharacterIsAvailable = false;

#ifdef MF_TRACE_IS_ENABLED
        if (fTraceTokensDetails) {
          std::stringstream ss;

          ss <<
            "percentParenthesizedCommentStartPositionInInput: " <<
            percentParenthesizedCommentStartPositionInInput <<
            std::endl <<
            "fCurrentPositionInInput: " <<
            fCurrentPositionInInput;

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        std::string commentString =
          fInputString.substr (
            percentParenthesizedCommentStartPositionInInput + 2,
            fCurrentPositionInInput - percentParenthesizedCommentStartPositionInInput - 3);

        fCurrentTokenKind = msdlTokenKind::kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);
      }
      break;

    default: // '%' up to this point, comment to end of line
      {
        int parcentCommentToEndOfLineStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          // consume this comment character
          fetchNextCharacter ();
        } // while

        // the end of the comment to end of line has been overtaken
        fNextCharacterIsAvailable = true;

        std::string commentString =
          fInputString.substr (
            parcentCommentToEndOfLineStartPositionInInput + 1,
            fCurrentPositionInInput - parcentCommentToEndOfLineStartPositionInInput - 1);

        fCurrentTokenKind = msdlTokenKind::kTokenCommentToEndOfLine;
        fCurrentTokenDescription.setString (commentString);

// JMI        handleEndOfLine ("percent comment to end of line");
      }
      break;
  } // switch
}

// --------------------------------------------------------------------------
//  msdlScanner::handleSlash
// --------------------------------------------------------------------------

void msdlScanner::handleSlash ()
{
  // consume the '/'
  fetchNextCharacter ();

  switch (fCurrentCharacter) {
    case '*': // '/*' up to this point, parenthesized comment
      {
       fetchNextCharacter ();

        unsigned slashParenthesizedCommentStartPositionInInput = fCurrentTokenPositionInInput;

        do {
          while (fCurrentCharacter != '*') {
            if (fCurrentCharacter == '\n') {
              handleEndOfLine ("star parenthesized comment");

//              fNextCharacterIsAvailable = false;
              fetchNextCharacter ();
            } // while

            // consume this comment character
            fetchNextCharacter ();
          } // while

          // consume the '*'
          fetchNextCharacter ();
        } // do
        while (fCurrentCharacter != '/');

        fNextCharacterIsAvailable = false;

#ifdef MF_TRACE_IS_ENABLED
        if (fTraceTokensDetails) {
          std::stringstream ss;

          ss <<
            "slashParenthesizedCommentStartPositionInInput: " <<
            slashParenthesizedCommentStartPositionInInput << std::endl <<
            "fCurrentPositionInInput: " <<
            fCurrentPositionInInput;

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        std::string commentString =
          fInputString.substr (
            slashParenthesizedCommentStartPositionInInput + 2,
            fCurrentPositionInInput - slashParenthesizedCommentStartPositionInInput - 3);

        fCurrentTokenKind = msdlTokenKind::kTokenParenthesizedComment;
        fCurrentTokenDescription.setString (commentString);

        fNextCharacterIsAvailable = false;
      }
      break;

    case '/': // '//' up to this point, comment to end of line
      {
        // consume the second '/'
        fetchNextCharacter ();

        unsigned slashCommentToEndOfLineStartPositionInInput = fCurrentTokenPositionInInput;

        while (fCurrentCharacter != '\n') {
          // consume the std::endl
          fetchNextCharacter ();
        } // while

        // the end of the comment to end of line has been overtaken
        fNextCharacterIsAvailable = true;

        std::string commentString =
          fInputString.substr (
            slashCommentToEndOfLineStartPositionInInput + 2,
            fCurrentPositionInInput - slashCommentToEndOfLineStartPositionInInput - 3);

        fCurrentTokenKind = msdlTokenKind::kTokenCommentToEndOfLine;
        fCurrentTokenDescription.setString (commentString);

// JMI        handleEndOfLine ("slash comment to end of line");
      }
      break;

    default:
      fCurrentTokenKind = msdlTokenKind::kTokenSlash;
      fNextCharacterIsAvailable = false;
  } // switch
}

// --------------------------------------------------------------------------
//  msdlScanner::acceptAName
// --------------------------------------------------------------------------

void msdlScanner::acceptAName ()
{
/*
  pitches such as c2 prevent identifiers from being tokens in MSDL
  as is usual in programming languages,
  so we handle actual identifiers such ar part1 at the syntax level
*/

  // accept all alphanumeric characters, the first character is available

  size_t
    nameStartPositionInInput =
      fCurrentTokenPositionInInput;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "Accepting a name" <<
      ", nameStartPositionInInput: " <<
      nameStartPositionInInput;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // a name can be a keyword

  Bool nonNameCharacterFound (false);

  while (! nonNameCharacterFound) {
    char character = fetchNextCharacter ();

    switch (character) {
      case EOF:
        nonNameCharacterFound = true;
        break;

      case '_':
        break;

      default:
        nonNameCharacterFound = ! isalpha (fCurrentCharacter);
    } // switch
  } // while

  // the end of the name has been overtaken
  fNextCharacterIsAvailable = true;

  std::string nameString =
    fInputString.substr (
      nameStartPositionInInput,
      fCurrentPositionInInput - nameStartPositionInInput);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "--- acceptAName()" <<
      ", nameString: \"" << nameString << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
  // is nameString the name of a keyword?
  msdlKeywordKind
    keyWordKind =
      msdlKeywordKindFromString (
        fKeywordsInputLanguageKind,
        nameString);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "--- acceptAName()" <<
      ", nameString: \"" << nameString << "\"" <<
      ", keyWordKind: \"" << msdlKeywordKindAsString (keyWordKind) << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (keyWordKind) {
    case msdlKeywordKind::kKeyword_UNKNOWN_: // no, it is a name
      fCurrentTokenKind = msdlTokenKind::kTokenName;
      fCurrentTokenDescription.setString (nameString);
      break;

      // language-dependent keywords
      // ------------------------------------

    case msdlKeywordKind::kKeywordTitle:
      fCurrentTokenKind = msdlTokenKind::kTokenTitle;
      break;
    case msdlKeywordKind::kKeywordComposer:
      fCurrentTokenKind = msdlTokenKind::kTokenComposer;
      break;
    case msdlKeywordKind::kKeywordOpus:
      fCurrentTokenKind = msdlTokenKind::kTokenOpus;
      break;

    case msdlKeywordKind::kKeywordPitches:
      fCurrentTokenKind = msdlTokenKind::kTokenPitches;
      break;
    case msdlKeywordKind::kKeywordOctaves:
      fCurrentTokenKind = msdlTokenKind::kTokenOctaves;
      break;

    case msdlKeywordKind::kKeywordAnacrusis:
      fCurrentTokenKind = msdlTokenKind::kTokenAnacrusis;
      break;

    case msdlKeywordKind::kKeywordBook:
      fCurrentTokenKind = msdlTokenKind::kTokenBook;
      break;
    case msdlKeywordKind::kKeywordScore:
      fCurrentTokenKind = msdlTokenKind::kTokenScore;
      break;
    case msdlKeywordKind::kKeywordPartGroup:
      fCurrentTokenKind = msdlTokenKind::kTokenPartGroup;
      break;
    case msdlKeywordKind::kKeywordPart:
      fCurrentTokenKind = msdlTokenKind::kTokenPart;
      break;
    case msdlKeywordKind::kKeywordMusic:
      fCurrentTokenKind = msdlTokenKind::kTokenMusic;
      break;
    case msdlKeywordKind::kKeywordFragment:
      fCurrentTokenKind = msdlTokenKind::kTokenFragment;
      break;

    case msdlKeywordKind::kKeywordClef:
      fCurrentTokenKind = msdlTokenKind::kTokenClef;
      break;
    case msdlKeywordKind::kKeywordTreble:
      fCurrentTokenKind = msdlTokenKind::kTokenTreble;
      break;
    case msdlKeywordKind::kKeywordSoprano:
      fCurrentTokenKind = msdlTokenKind::kTokenSoprano;
      break;
    case msdlKeywordKind::kKeywordAlto:
      fCurrentTokenKind = msdlTokenKind::kTokenAlto;
      break;
    case msdlKeywordKind::kKeywordTenor:
      fCurrentTokenKind = msdlTokenKind::kTokenTenor;
      break;
    case msdlKeywordKind::kKeywordBaryton:
      fCurrentTokenKind = msdlTokenKind::kTokenBaryton;
      break;
    case msdlKeywordKind::kKeywordBass:
      fCurrentTokenKind = msdlTokenKind::kTokenBass;
      break;
    case msdlKeywordKind::kKeywordTab:
      fCurrentTokenKind = msdlTokenKind::kTokenTab;
      break;

    case msdlKeywordKind::kKeywordKey:
      fCurrentTokenKind = msdlTokenKind::kTokenKey;
      break;

    case msdlKeywordKind::kKeywordTime:
      fCurrentTokenKind = msdlTokenKind::kTokenTime;
      break;
  } // switch

  // set the token description keyword kind if relevant
  if (fCurrentTokenKind != msdlTokenKind::kTokenName) {
    fCurrentTokenDescription.setKeywordKind (keyWordKind);
  }

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "Accepting a name, found: \"" << nameString << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlScanner::acceptAnInteger
// --------------------------------------------------------------------------

void msdlScanner::acceptAnInteger ()
{
  // accept digits sequence, the first digit is available

  size_t
    integerStartPositionInInput =
      fCurrentTokenPositionInInput;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "Accepting an integer" <<
      ", integerStartPositionInInput: " <<
      integerStartPositionInInput;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  Bool nonIntegerCharacterFound (false);

  while (! nonIntegerCharacterFound) {
    char character = fetchNextCharacter ();

    switch (character) {
      case EOF:
        nonIntegerCharacterFound = true;
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        break;

      default:
        nonIntegerCharacterFound = true;
    } // switch
  } // while

  // the end of the integer has been overtaken
  fNextCharacterIsAvailable = true;

  std::string integerString =
    fInputString.substr (
      integerStartPositionInInput,
      fCurrentPositionInInput - integerStartPositionInInput);

  fCurrentTokenKind =  msdlTokenKind::kTokenInteger;
  fCurrentTokenDescription.setInteger (stoi (integerString));

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "Accepting an integer, found: \"" << integerString << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlScanner::acceptAString
// --------------------------------------------------------------------------
void msdlScanner::acceptAString ()
{
  // accept all the std::string characters
  // we're on the initial double quote

  size_t
    stringStartPositionInInput =
      fCurrentTokenPositionInInput;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Accepting a std::string" <<
      ", stringStartPositionInInput: " << stringStartPositionInInput;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // the std::string we're building
  std::string theString;

  // consume the '"'
  fetchNextCharacter ();

  Bool endOfStringHasBeenReached (false);

  int chunkStartPositionInInput =
    stringStartPositionInInput + 1;

  while (! endOfStringHasBeenReached) {
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "--- acceptAString()" <<
      ", fCurrentCharacter: " <<
      currentCharacterAsString () <<
      ", fNextCharacterIsAvailable: " <<
      fNextCharacterIsAvailable <<
      ", chunkStartPositionInInput: " <<
      chunkStartPositionInInput <<
      ", fCurrentPositionInInput: " <<
      fCurrentPositionInInput <<
      ", fCurrentPositionInLine: " <<
      fCurrentPositionInLine;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    switch (fCurrentCharacter) {
      case '"':
        fNextCharacterIsAvailable = false;
        endOfStringHasBeenReached = true;
        break;

      case '\\': // escaped characted
        {
          // append the current chunk to theString
#ifdef MF_TRACE_IS_ENABLED
          if (fTraceTokensDetails) {
            gLog <<
              "--- acceptAString()" <<
              ", chunkStartPositionInInput: " <<
              chunkStartPositionInInput <<
              ", fCurrentPositionInInput: " <<
              fCurrentPositionInInput <<
              ", fCurrentPositionInInput - chunkStartPositionInInput: " <<
              fCurrentPositionInInput - chunkStartPositionInInput <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          std::string currentChunk =
            fInputString.substr (
              chunkStartPositionInInput,
              fCurrentPositionInInput - chunkStartPositionInInput);

#ifdef MF_TRACE_IS_ENABLED
          if (fTraceTokensDetails) {
            gLog <<
              "--- acceptAString(), escaped character" <<
              ", currentChunk: \"" <<
              currentChunk <<
              "\"" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          theString += currentChunk;
#ifdef MF_TRACE_IS_ENABLED
          if (fTraceTokensDetails) {
            gLog <<
              "--- acceptAString(), theString with chunk appended" <<
              ", theString: \"" <<
              theString <<
              "\"" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          // consume the escape '\'
          fetchNextCharacter ();

          // append the escaped character to theString
#ifdef MF_TRACE_IS_ENABLED
          if (fTraceTokensDetails) {
            gLog <<
              ", escaped character: " <<
              currentCharacterAsString () <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          switch (fCurrentCharacter) {
            case '"':
              theString += '"';
              break;
            case '\\':
              theString += '\\';
              break;
            case 'n':
              handleEndOfLine ("std::string 1");

              theString += '\n';
              break;
            case 't':
              theString += '\t';
              break;
            default:
              fScannerWaeHandler->
                illegalEscapedCharacterInString (fCurrentCharacter);
              fSourceIsLexicallyCorrect = false;
              fNextCharacterIsAvailable = false;
          } // switch

#ifdef MF_TRACE_IS_ENABLED
          if (fTraceTokensDetails) {
            gLog <<
              "--- acceptAString(), theString with escaped character appended" <<
              ", theString: \"" <<
              theString <<
              "\"" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          // starting a new chunk
          chunkStartPositionInInput = fCurrentPositionInInput;
        }
        break;

// JMI multi-line strings ???
      case '\n':
        handleEndOfLine ("std::string 2");

        theString += "\n";
        fNextCharacterIsAvailable = false;

        fCurrentPositionInLine = 0;
        break;

      default:
        ; // the currentChunk will receive this character
      } //  switch

    if (endOfStringHasBeenReached) {
      break;
    }

    // consume this std::string character
    fetchNextCharacter ();
  } // while

  // append the last chunk to theString
  std::string currentChunk =
    fInputString.substr (
      chunkStartPositionInInput,
      fCurrentPositionInInput - chunkStartPositionInInput);
  theString += currentChunk;

  fCurrentTokenKind = msdlTokenKind::kTokenString;
  fCurrentTokenDescription.setString (theString);

  --gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Accepting a std::string, found: \"" << theString << "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlScanner::currentLocationAsString
// --------------------------------------------------------------------------

std::string msdlScanner::currentLocationAsString () const
{
  std::string separateur = // JMI
          "-----------------------------------------------------";

  std::stringstream ss;

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokens) {
    ss <<
      "==> fCurrentTokenPositionInInput:" << fCurrentTokenPositionInInput <<
      ", fCurrentPositionInInput:" << fCurrentPositionInInput;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the line indication
  std::string lineIndication;

  {
    std::stringstream ss;

    ss <<
      gServiceRunData->getInputSourceName () << ":" << fCurrentLineNumber << ' ';

    lineIndication = ss.str ();
  }

  // the line beginning
  int
    lineBeginningLength =
      fNextCharacterIsAvailable
        ? fCurrentPositionInInput - fCurrentTokenPositionInInput - 1
        : fCurrentPositionInInput - fCurrentTokenPositionInInput;

  std::string
    lineBeginning =
      fInputString.substr (
        fCurrentTokenPositionInInput,
        lineBeginningLength);

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokens) {
    ss <<
      "==> lineBeginningLength:" << lineBeginningLength <<
      std::endl <<
      "==> lineBeginning:" <<
      std::endl <<
      "|" <<
      lineBeginning <<
      "|";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ss <<
    lineIndication <<
    lineBeginning <<
    std::endl <<
    mfReplicateChar (' ', lineIndication.size ()) <<
    mfReplicateChar (' ', lineBeginning.size ()) <<
    "^" <<
    std::endl;

  return ss.str ();
}

// --------------------------------------------------------------------------
//  msdlScanner::scanAllTheInputAtOnce
// --------------------------------------------------------------------------

void msdlScanner::scanAllTheInputAtOnce (
  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokens)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "==> scanAllTheInputAtOnce()";
  }

  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      ", ignoreSeparatorTokens: " <<
      msdlIgnoreSeparatorTokensKindAsString (ignoreSeparatorTokens) <<
      std::endl;
  }
  else {
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (fInputIsEmpty) {
#ifdef MF_TRACE_IS_ENABLED
    gLog <<
      "Input is empty " <<
      std::endl;
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    // let's go ahead

    ++gIndenter;

    int numberOfTokens = 0;

    try {
      msdlTokenKind tokenKind = msdlTokenKind::kToken_UNKNOWN_;

      // fetch the first input character
      fetchNextCharacter ();

      do { // loop till end of stream
        tokenKind = fetchNextToken (ignoreSeparatorTokens);
        ++numberOfTokens;

#ifdef MF_TRACE_IS_ENABLED
        if (fTraceTokensDetails) {
          int fieldWidth = 28;

          gLog <<
            std::endl <<
            "--- scanAllTheInputAtOnce()" <<
            std::endl;

          ++gIndenter;

          gLog << std::left <<
            std::setw (fieldWidth) <<
            "fCurrentTokenPositionInInput" << ": " << fCurrentTokenPositionInInput <<
            std::endl <<
            std::setw (fieldWidth) <<
            "fCurrentCharacter" << ": " << currentCharacterAsString () <<
            std::endl <<
            std::setw (fieldWidth) <<
            "fNextCharacterIsAvailable" << ": " << fNextCharacterIsAvailable <<
            std::endl <<

            std::setw (fieldWidth) <<
            "fCurrentToken" << ": " <<
            std::endl;

          ++gIndenter;
          gLog << fCurrentToken << std::endl;
          --gIndenter;

          --gIndenter;
        }
#endif // MF_TRACE_IS_ENABLED
      }
      while (tokenKind != msdlTokenKind::k_TokenEOF); // do
    } // try

    catch (std::out_of_range& e) {
      mfDisplayException (e, gOutput);
    }
    catch (std::exception& e) {
      mfDisplayException (e, gOutput);
    }

    if (fSourceIsLexicallyCorrect) {
      fScannerWaeHandler->
        inputIsLexicallyCorrect (numberOfTokens);
    }
    else {
      fScannerWaeHandler->
        inputIsLexicallyIncorrect (numberOfTokens);
    }

    --gIndenter;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<== scanAllTheInputAtOnce()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlScanner::translateAllTheInputToKeywordsLanguage
// --------------------------------------------------------------------------

void msdlScanner::translateAllTheInputToKeywordsLanguage (
  msdlKeywordsLanguageKind keywordsConversionLanguage,
  msdlCommentsTypeKind     commentsTypeKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokens) {
    std::stringstream ss;

    ss <<
      "==> translateAllTheInputToKeywordsLanguage()" <<
      ", keywordsConversionLanguage: " <<
      msdlKeywordsLanguageKindAsString (keywordsConversionLanguage);

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fInputIsEmpty) {
#ifdef MF_TRACE_IS_ENABLED
    gLog <<
      "Input is empty " <<
      std::endl;
#endif // MF_TRACE_IS_ENABLED

    return;
  }

  // let's go ahead

  // is the MSDL keywords translation language needed?
  if (keywordsConversionLanguage == fKeywordsInputLanguageKind) {
    fScannerWaeHandler->
      inputIsAlreadyInMsdlKeywordsLanguage (
        keywordsConversionLanguage);

    return;
  }

//   int numberOfTokens = 0; JMI v0.9.67

  try {
    msdlTokenKind tokenKind = msdlTokenKind::kToken_UNKNOWN_;

    // fetch the first input character
    fetchNextCharacter ();

    do { // loop till end of stream
      // fetch the next token
      tokenKind = fetchNextToken (msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensNo);
//       ++numberOfTokens;

#ifdef MF_TRACE_IS_ENABLED
      if (fTraceTokens) {
        int fieldWidth = 23;

        gLog <<
          std::endl <<
          "--- translateAllTheInputToKeywordsLanguage()" <<
          std::endl;

        ++gIndenter;

        gLog << std::left <<
          std::setw (fieldWidth) <<
          "fCurrentToken" << ": " <<
          std::endl;

        ++gIndenter;
        gLog << fCurrentToken << std::endl;
        --gIndenter;

        --gIndenter;

        gLog << std::left <<
          std::setw (fieldWidth) <<
          "fCurrentTokenPositionInInput" << ": " << fCurrentTokenPositionInInput <<
          std::endl <<
          std::setw (fieldWidth) <<
          "fCurrentCharacter" << " : '" << currentCharacterAsString () <<
          std::endl <<
          std::setw (fieldWidth) <<
          "fNextCharacterIsAvailable" << ": " << fNextCharacterIsAvailable <<
          std::endl <<

          std::setw (fieldWidth) <<
          "fCurrentToken" << ": " <<
          std::endl;

        ++gIndenter;
        gLog << fCurrentToken << std::endl;
        --gIndenter;
      }
#endif // MF_TRACE_IS_ENABLED

      // write current token translated to standard output
//      gOutput <<
      gOutput << // JMI
        fCurrentToken.asMsdlString (
          keywordsConversionLanguage,
          commentsTypeKind);
    }
    while (tokenKind != msdlTokenKind::k_TokenEOF); // do
  } // try

  catch ( std::out_of_range& e) {
    mfDisplayException (e, gOutput);
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokens) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "<== translateAllTheInputToKeywordsLanguage()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

// --------------------------------------------------------------------------
//  msdlScanner::scanWholeInputAtOnce
// --------------------------------------------------------------------------

void msdlScanner::scanWholeInputAtOnce ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceTokensDetails) {
    std::stringstream ss;

    ss <<
      "==> scanWholeInputAtOnce()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fInputIsEmpty) {
#ifdef MF_TRACE_IS_ENABLED
    gLog <<
      "Input is empty " <<
      std::endl;
#endif // MF_TRACE_IS_ENABLED

    return;
  }

  // let's go ahead

  // should we ignore separator tokens?
  Bool
    ignoreSeparatorTokens =
      gGlobalMsdl2msrOahGroup->
        getIgnoreSeparatorTokens ();

  msdlIgnoreSeparatorTokensKind
    ignoreSeparatorTokensKind =
      ignoreSeparatorTokens
        ? msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensYes
        : msdlIgnoreSeparatorTokensKind::kIgnoreSeparatorTokensNo;

  // should we translate the keywords?
  msdlKeywordsLanguageKind
    keywordsConversionLanguageKind =
      gGlobalMsdl2msrOahGroup->
        getMsdlKeywordsConversionLanguageKind ();

  // what comments type should we use in the translation output?
  msdlCommentsTypeKind
    commentsTypeKind =
      gGlobalMsdl2msrOahGroup->
        getMsdlCommentsTypeKind ();

  // do the job
  switch (keywordsConversionLanguageKind) {
    case msdlKeywordsLanguageKind::kKeywordsLanguage_UNKNOWN_:
      scanAllTheInputAtOnce (
        ignoreSeparatorTokensKind);
      break;

    default:
      translateAllTheInputToKeywordsLanguage (
        keywordsConversionLanguageKind,
        commentsTypeKind);
  } // switch
}


}
