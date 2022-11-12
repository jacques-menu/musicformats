/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "msdlParserWaeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class   is purely virtual
S_msdlParserWaeHandler msdlParserWaeHandler::create ()
{
  msdlParserWaeHandler* o =
    new msdlParserWaeHandler ();
  assert (o != nullptr);
  return o;
}
*/

msdlParserWaeHandler::msdlParserWaeHandler ()
{}

msdlParserWaeHandler::~msdlParserWaeHandler ()
{}

std::string msdlParserWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL WAE lexical handler";

  return s.str ();
}

void msdlParserWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlParserWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlParserEnglishWaeHandler msdlParserEnglishWaeHandler::create ()
{
  msdlParserEnglishWaeHandler* o =
    new msdlParserEnglishWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserEnglishWaeHandler::msdlParserEnglishWaeHandler ()
{}

msdlParserEnglishWaeHandler::~msdlParserEnglishWaeHandler ()
{}

void msdlParserEnglishWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a std::string is expected as title ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a std::string is expected as composer ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a std::string is expected as opus ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::ignoringToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserEnglishWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserEnglishWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserEnglishWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlParserEnglishWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserEnglishWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlParserFrenchWaeHandler msdlParserFrenchWaeHandler::create ()
{
  msdlParserFrenchWaeHandler* o =
    new msdlParserFrenchWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserFrenchWaeHandler::msdlParserFrenchWaeHandler ()
{}

msdlParserFrenchWaeHandler::~msdlParserFrenchWaeHandler ()
{}

void msdlParserFrenchWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### une seule section d'identification est possible, au début de la spécification ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### une chaîne est attendue comme titre ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### une chaîne est attendue comme compositeur-trice ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### une chaîne est attendue comme opus ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  const std::string&    context) const
{

  gLogStream <<
    "### " << context << ": " <<
    "terminal inattendu: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::ignoringToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "on ignore le terminal: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "Le texte MSDL est syntaxiquement correct" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### Le texte MSDL est syntaxiquement incorrect ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### On ne peut pas mélanger les virgules et apostrophes dans les indications d'octave ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Durée de note mal formée ###" <<
    std::endl;
}

std::string msdlParserFrenchWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "gestionnaire WAE pour MSDL en français";

  return s.str ();
}

void msdlParserFrenchWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlParserFrenchWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserFrenchWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlParserItalianWaeHandler msdlParserItalianWaeHandler::create ()
{
  msdlParserItalianWaeHandler* o =
    new msdlParserItalianWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserItalianWaeHandler::msdlParserItalianWaeHandler ()
{}

msdlParserItalianWaeHandler::~msdlParserItalianWaeHandler ()
{}

void msdlParserItalianWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a std::string is expected as title ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a std::string is expected as composer ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a std::string is expected as opus ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::ignoringToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    std::endl;
}

void msdlParserItalianWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserItalianWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserItalianWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserItalianWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlParserItalianWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserItalianWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlParserGermanWaeHandler msdlParserGermanWaeHandler::create ()
{
  msdlParserGermanWaeHandler* o =
    new msdlParserGermanWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserGermanWaeHandler::msdlParserGermanWaeHandler ()
{}

msdlParserGermanWaeHandler::~msdlParserGermanWaeHandler ()
{}

void msdlParserGermanWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a std::string is expected as title ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a std::string is expected as composer ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a std::string is expected as opus ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::ignoringToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    std::endl;
}

void msdlParserGermanWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserGermanWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserGermanWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserGermanWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlParserGermanWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserGermanWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlParserSpanishWaeHandler msdlParserSpanishWaeHandler::create ()
{
  msdlParserSpanishWaeHandler* o =
    new msdlParserSpanishWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserSpanishWaeHandler::msdlParserSpanishWaeHandler ()
{}

msdlParserSpanishWaeHandler::~msdlParserSpanishWaeHandler ()
{}

void msdlParserSpanishWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a std::string is expected as title ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a std::string is expected as composer ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a std::string is expected as opus ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::ignoringToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserSpanishWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserSpanishWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlParserSpanishWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserSpanishWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdlParserDutchWaeHandler msdlParserDutchWaeHandler::create ()
{
  msdlParserDutchWaeHandler* o =
    new msdlParserDutchWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserDutchWaeHandler::msdlParserDutchWaeHandler ()
{}

msdlParserDutchWaeHandler::~msdlParserDutchWaeHandler ()
{}

void msdlParserDutchWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a std::string is expected as title ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a std::string is expected as composer ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a std::string is expected as opus ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::ignoringToken (
  const msdlToken& theToken,
  const std::string&    context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    std::endl;
}

void msdlParserDutchWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserDutchWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserDutchWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserDutchWaeHandler::print (std::ostream& os) const
{
  os <<
    "msdlParserDutchWaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserDutchWaeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}



} // namespace
