/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include "mfEnableTracingIfDesired.h"
// #ifdef OAH_TRACING_IS_ENABLED
//   #include "mfTracingOah.h"
// #endif
//
// #include "oahOah.h"
//
// #include "msrOah.h"
// #include "msr2lpsrOah.h"
//
// #include "msdlKeywords.h"
// #include "msdlTokens.h"

#include "msdlParserWaeHandlersFrench.h"


namespace MusicFormats
{

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
  const msdlToken&   theToken,
  const std::string& context) const
{

  gLogStream <<
    "### " << context << ": " <<
    "terminal inattendu: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserFrenchWaeHandler::ignoringToken (
  const msdlToken&   theToken,
  const std::string& context) const
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


} // namespace
