/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msdlParserWaeHandlersFrench.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msdlParserWaeHandlerFrench msdlParserWaeHandlerFrench::create ()
{
  msdlParserWaeHandlerFrench* o =
    new msdlParserWaeHandlerFrench ();
  assert (o != nullptr);
  return o;
}

msdlParserWaeHandlerFrench::msdlParserWaeHandlerFrench ()
{}

msdlParserWaeHandlerFrench::~msdlParserWaeHandlerFrench ()
{}

void msdlParserWaeHandlerFrench::multipleIdentifications () const
{
  gLogStream <<
    "### une seule section d'identification est possible, au début de la spécification ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::stringExpectedAsTitle () const
{
  gLogStream <<
    "### une chaîne est attendue comme titre ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::stringExpectedAsComposer () const
{
  gLogStream <<
    "### une chaîne est attendue comme compositeur-trice ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::stringExpectedAsOpus () const
{
  gLogStream <<
    "### une chaîne est attendue comme opus ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::unexpectedToken (
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

void msdlParserWaeHandlerFrench::ignoringToken (
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

void msdlParserWaeHandlerFrench::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "Le texte MSDL est syntaxiquement correct" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### Le texte MSDL est syntaxiquement incorrect ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### On ne peut pas mélanger les virgules et apostrophes dans les indications d'octave ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::malformedNoteDuration () const
{
  gLogStream <<
    "### Durée de note mal formée ###" <<
    std::endl;
}

std::string msdlParserWaeHandlerFrench::asString () const
{
  std::stringstream s;

  s <<
    "gestionnaire WAE pour MSDL en français";

  return s.str ();
}

void msdlParserWaeHandlerFrench::print (std::ostream& os) const
{
  os <<
    "msdlParserWaeHandlerFrench" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerFrench& elt)
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