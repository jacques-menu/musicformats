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
  gLog <<
    "### une seule section d'identification est possible, au début de la spécification ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::stringExpectedAsTitle () const
{
  gLog <<
    "### une chaîne est attendue comme titre ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::stringExpectedAsComposer () const
{
  gLog <<
    "### une chaîne est attendue comme compositeur-trice ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::stringExpectedAsOpus () const
{
  gLog <<
    "### une chaîne est attendue comme opus ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::unexpectedToken (
  const msdlToken&   theToken,
  const std::string& context) const
{

  gLog <<
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
  gLog <<
    "### " << context << ": " <<
    "on ignore le terminal: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::inputIsSyntacticallyCorrect () const
{
  gLog <<
    "Le texte MSDL est syntaxiquement correct" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::inputIsSyntacticallyIncorrect () const
{
  gLog <<
    "### Le texte MSDL est syntaxiquement incorrect ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::mixedCommasAndQuotesInOctaveIndication () const
{
  gLog <<
    "### On ne peut pas mélanger les virgules et apostrophes dans les indications d'octave ###" <<
    std::endl;
}

void msdlParserWaeHandlerFrench::malformedNoteDuration () const
{
  gLog <<
    "### Durée de note mal formée ###" <<
    std::endl;
}

std::string msdlParserWaeHandlerFrench::asString () const
{
  std::stringstream ss;

  ss <<
    "gestionnaire WAE pour MSDL en français";

  return ss.str ();
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
