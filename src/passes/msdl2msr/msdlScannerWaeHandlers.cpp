/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStringsHandling.h"

#include "mfPreprocessorSettings.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "msdlScannerWaeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class is purely virtual
S_msdlScannerWaeHandler msdlScannerWaeHandler::create ()
{
  msdlScannerWaeHandler* obj =
    new msdlScannerWaeHandler ();
  assert (obj != nullptr);
  return obj;
}
*/

msdlScannerWaeHandler::msdlScannerWaeHandler ()
{}

msdlScannerWaeHandler::~msdlScannerWaeHandler ()
{}

std::string msdlScannerWaeHandler::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL WAE lexical handler";

  return ss.str ();
}

void msdlScannerWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlScannerWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlScannerWaeHandlerEnglish msdlScannerWaeHandlerEnglish::create ()
{
  msdlScannerWaeHandlerEnglish* obj =
    new msdlScannerWaeHandlerEnglish ();
  assert (obj != nullptr);
  return obj;
}

msdlScannerWaeHandlerEnglish::msdlScannerWaeHandlerEnglish ()
{}

msdlScannerWaeHandlerEnglish::~msdlScannerWaeHandlerEnglish ()
{}

void msdlScannerWaeHandlerEnglish::illegalCharacter (char theChar) const
{
  gLog <<
    "### illegal character: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerEnglish::illegalCharacters (const std::string& theString) const
{
  gLog <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    std::endl;
}

void msdlScannerWaeHandlerEnglish::digitExpectedAfterDecimalDot (char theChar) const
{
  gLog <<
    "### a digit is expected after decimal dot, found: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}
void msdlScannerWaeHandlerEnglish::digitExpectedInExponent (char theChar) const
{
  gLog <<
    "### a digit is expected in exponent, found: " <<
    '\'' << theChar << "' " <<
    std::endl;
}

void msdlScannerWaeHandlerEnglish::illegalEscapedCharacterInString (char theChar) const
{
  gLog <<
    "### illegal escaped character after '\': " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerEnglish::inputIsLexicallyCorrect (int numberOfTokens) const
{
  gLog <<
    "The MSDL input is lexically correct. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    std::endl;
}

void msdlScannerWaeHandlerEnglish::inputIsLexicallyIncorrect (int numberOfTokens) const
{
  gLog <<
    "### The MSDL input is lexically incorrect. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " ###" <<
    std::endl;
}

void msdlScannerWaeHandlerEnglish::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsConversionLanguage) const
{
  gLog <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsConversionLanguage) <<
    "\", there is no need to translate it ###" <<
    std::endl;
}

std::string msdlScannerWaeHandlerEnglish::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL lexical english WAE handler";

  return ss.str ();
}

void msdlScannerWaeHandlerEnglish::print (std::ostream& os) const
{
  os <<
    "msdlScannerWaeHandlerEnglish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerEnglish& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlScannerWaeHandlerFrench msdlScannerWaeHandlerFrench::create ()
{
  msdlScannerWaeHandlerFrench* obj =
    new msdlScannerWaeHandlerFrench ();
  assert (obj != nullptr);
  return obj;
}

msdlScannerWaeHandlerFrench::msdlScannerWaeHandlerFrench ()
{}

msdlScannerWaeHandlerFrench::~msdlScannerWaeHandlerFrench ()
{}

void msdlScannerWaeHandlerFrench::illegalCharacter (char theChar) const
{
  gLog <<
    "### caractère illégal: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerFrench::illegalCharacters (const std::string& theString) const
{
  gLog <<
    "### caractères illégaux: " <<
    "\"" << theString << "\" ###" <<
    std::endl;
}

void msdlScannerWaeHandlerFrench::digitExpectedAfterDecimalDot (char theChar) const
{
  gLog <<
    "### un chiffre est attendu après un point décimal, trouvé: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}
void msdlScannerWaeHandlerFrench::digitExpectedInExponent (char theChar) const
{
  gLog <<
    "### un chiffre est attendu dans un exposant, trouvé: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerFrench::illegalEscapedCharacterInString (char theChar) const
{
  gLog <<
    "### caractère d'échappement illégal après '\': " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerFrench::inputIsLexicallyCorrect (int numberOfTokens) const
{
  gLog <<
    "Le texte MSDL est lexicalement correct. Il contient " <<
    mfSingularOrPlural (
      numberOfTokens, "terminal", "terminaux") <<
    std::endl;
}

void msdlScannerWaeHandlerFrench::inputIsLexicallyIncorrect (int numberOfTokens) const
{
  gLog <<
    "### Le texte MSDL est lexicalement incorrect. Il contient " <<
    mfSingularOrPlural (
      numberOfTokens, "terminal", "terminaux") <<
    " ###" <<
    std::endl;
}

void msdlScannerWaeHandlerFrench::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsConversionLanguage) const
{
  gLog <<
    "### Le texte MSDL est déjà dans la langue \"" <<
    msdlKeywordsLanguageKindAsString (keywordsConversionLanguage) <<
    "\" pour les mots clés, il n'a a pas besoin de le traduire ###" <<
    std::endl;
}

std::string msdlScannerWaeHandlerFrench::asString () const
{
  std::stringstream ss;

  ss <<
    "gestionnaire WAE pour MSDL en français";

  return ss.str ();
}

void msdlScannerWaeHandlerFrench::print (std::ostream& os) const
{
  os <<
    "msdlScannerWaeHandlerFrench" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerFrench& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlScannerWaeHandlerItalian msdlScannerWaeHandlerItalian::create ()
{
  msdlScannerWaeHandlerItalian* obj =
    new msdlScannerWaeHandlerItalian ();
  assert (obj != nullptr);
  return obj;
}

msdlScannerWaeHandlerItalian::msdlScannerWaeHandlerItalian ()
{}

msdlScannerWaeHandlerItalian::~msdlScannerWaeHandlerItalian ()
{}

void msdlScannerWaeHandlerItalian::illegalCharacter (char theChar) const
{
  gLog <<
    "### illegal character: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerItalian::illegalCharacters (const std::string& theString) const
{
  gLog <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    std::endl;
}

void msdlScannerWaeHandlerItalian::digitExpectedAfterDecimalDot (char theChar) const
{
  gLog <<
    "### a digit is expected after decimal dot, found: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}
void msdlScannerWaeHandlerItalian::digitExpectedInExponent (char theChar) const
{
  gLog <<
    "### a digit is expected in exponent, found: " <<
    '\'' << theChar << "' " <<
    std::endl;
}

void msdlScannerWaeHandlerItalian::illegalEscapedCharacterInString (char theChar) const
{
  gLog <<
    "### illegal escaped character after '\': " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerItalian::inputIsLexicallyCorrect (int numberOfTokens) const
{
  gLog <<
    "The MSDL input is lexically correct. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    std::endl;
}

void msdlScannerWaeHandlerItalian::inputIsLexicallyIncorrect (int numberOfTokens) const
{
  gLog <<
    "### The MSDL input is lexically incorrect. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " ###" <<
    std::endl;
}

void msdlScannerWaeHandlerItalian::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsConversionLanguage) const
{
  gLog <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsConversionLanguage) <<
    "\", there is no need to translate it ###" <<
    std::endl;
}

std::string msdlScannerWaeHandlerItalian::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL lexical english WAE handler";

  return ss.str ();
}

void msdlScannerWaeHandlerItalian::print (std::ostream& os) const
{
  os <<
    "msdlScannerWaeHandlerItalian" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerItalian& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlScannerWaeHandlerGerman msdlScannerWaeHandlerGerman::create ()
{
  msdlScannerWaeHandlerGerman* obj =
    new msdlScannerWaeHandlerGerman ();
  assert (obj != nullptr);
  return obj;
}

msdlScannerWaeHandlerGerman::msdlScannerWaeHandlerGerman ()
{}

msdlScannerWaeHandlerGerman::~msdlScannerWaeHandlerGerman ()
{}

void msdlScannerWaeHandlerGerman::illegalCharacter (char theChar) const
{
  gLog <<
    "### illegal character: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerGerman::illegalCharacters (const std::string& theString) const
{
  gLog <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    std::endl;
}

void msdlScannerWaeHandlerGerman::digitExpectedAfterDecimalDot (char theChar) const
{
  gLog <<
    "### a digit is expected after decimal dot, found: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}
void msdlScannerWaeHandlerGerman::digitExpectedInExponent (char theChar) const
{
  gLog <<
    "### a digit is expected in exponent, found: " <<
    '\'' << theChar << "' " <<
    std::endl;
}

void msdlScannerWaeHandlerGerman::illegalEscapedCharacterInString (char theChar) const
{
  gLog <<
    "### illegal escaped character after '\': " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerGerman::inputIsLexicallyCorrect (int numberOfTokens) const
{
  gLog <<
    "The MSDL input is lexically correct. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " tokens" <<
    std::endl;
}

void msdlScannerWaeHandlerGerman::inputIsLexicallyIncorrect (int numberOfTokens) const
{
  gLog <<
    "### The MSDL input is lexically incorrect. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " ###" <<
    std::endl;
}

void msdlScannerWaeHandlerGerman::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsConversionLanguage) const
{
  gLog <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsConversionLanguage) <<
    "\", there is no need to translate it ###" <<
    std::endl;
}

std::string msdlScannerWaeHandlerGerman::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL lexical english WAE handler";

  return ss.str ();
}

void msdlScannerWaeHandlerGerman::print (std::ostream& os) const
{
  os <<
    "msdlScannerWaeHandlerGerman" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerGerman& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlScannerWaeHandlerSpanish msdlScannerWaeHandlerSpanish::create ()
{
  msdlScannerWaeHandlerSpanish* obj =
    new msdlScannerWaeHandlerSpanish ();
  assert (obj != nullptr);
  return obj;
}

msdlScannerWaeHandlerSpanish::msdlScannerWaeHandlerSpanish ()
{}

msdlScannerWaeHandlerSpanish::~msdlScannerWaeHandlerSpanish ()
{}

void msdlScannerWaeHandlerSpanish::illegalCharacter (char theChar) const
{
  gLog <<
    "### illegal character: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerSpanish::illegalCharacters (const std::string& theString) const
{
  gLog <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    std::endl;
}

void msdlScannerWaeHandlerSpanish::digitExpectedAfterDecimalDot (char theChar) const
{
  gLog <<
    "### a digit is expected after decimal dot, found: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}
void msdlScannerWaeHandlerSpanish::digitExpectedInExponent (char theChar) const
{
  gLog <<
    "### a digit is expected in exponent, found: " <<
    '\'' << theChar << "' " <<
    std::endl;
}

void msdlScannerWaeHandlerSpanish::illegalEscapedCharacterInString (char theChar) const
{
  gLog <<
    "### illegal escaped character after '\': " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerSpanish::inputIsLexicallyCorrect (int numberOfTokens) const
{
  gLog <<
    "The MSDL input is lexically correct. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " tokens" <<
    std::endl;
}

void msdlScannerWaeHandlerSpanish::inputIsLexicallyIncorrect (int numberOfTokens) const
{
  gLog <<
    "### The MSDL input is lexically incorrect. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " ###" <<
    std::endl;
}

void msdlScannerWaeHandlerSpanish::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsConversionLanguage) const
{
  gLog <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsConversionLanguage) <<
    "\", there is no need to translate it ###" <<
    std::endl;
}

std::string msdlScannerWaeHandlerSpanish::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL lexical english WAE handler";

  return ss.str ();
}

void msdlScannerWaeHandlerSpanish::print (std::ostream& os) const
{
  os <<
    "msdlScannerWaeHandlerSpanish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerSpanish& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlScannerWaeHandlerDutch msdlScannerWaeHandlerDutch::create ()
{
  msdlScannerWaeHandlerDutch* obj =
    new msdlScannerWaeHandlerDutch ();
  assert (obj != nullptr);
  return obj;
}

msdlScannerWaeHandlerDutch::msdlScannerWaeHandlerDutch ()
{}

msdlScannerWaeHandlerDutch::~msdlScannerWaeHandlerDutch ()
{}

void msdlScannerWaeHandlerDutch::illegalCharacter (char theChar) const
{
  gLog <<
    "### illegal character: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerDutch::illegalCharacters (const std::string& theString) const
{
  gLog <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    std::endl;
}

void msdlScannerWaeHandlerDutch::digitExpectedAfterDecimalDot (char theChar) const
{
  gLog <<
    "### a digit is expected after decimal dot, found: " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}
void msdlScannerWaeHandlerDutch::digitExpectedInExponent (char theChar) const
{
  gLog <<
    "### a digit is expected in exponent, found: " <<
    '\'' << theChar << "' " <<
    std::endl;
}

void msdlScannerWaeHandlerDutch::illegalEscapedCharacterInString (char theChar) const
{
  gLog <<
    "### illegal escaped character after '\': " <<
    '\'' << theChar << "' ###" <<
    std::endl;
}

void msdlScannerWaeHandlerDutch::inputIsLexicallyCorrect (int numberOfTokens) const
{
  gLog <<
    "The MSDL input is lexically correct. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " tokens" <<
    std::endl;
}

void msdlScannerWaeHandlerDutch::inputIsLexicallyIncorrect (int numberOfTokens) const
{
  gLog <<
    "### The MSDL input is lexically incorrect. It contains " <<
    mfSingularOrPlural (
      numberOfTokens, "token", "tokens") <<
    " ###" <<
    std::endl;
}

void msdlScannerWaeHandlerDutch::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsConversionLanguage) const
{
  gLog <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsConversionLanguage) <<
    "\", there is no need to translate it ###" <<
    std::endl;
}

std::string msdlScannerWaeHandlerDutch::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL lexical english WAE handler";

  return ss.str ();
}

void msdlScannerWaeHandlerDutch::print (std::ostream& os) const
{
  os <<
    "msdlScannerWaeHandlerDutch" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlScannerWaeHandlerDutch& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}



} // namespace
