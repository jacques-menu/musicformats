/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msdlParserWaeHandlersGerman.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msdlParserWaeHandlerGerman msdlParserWaeHandlerGerman::create ()
{
  msdlParserWaeHandlerGerman* o =
    new msdlParserWaeHandlerGerman ();
  assert (o != nullptr);
  return o;
}

msdlParserWaeHandlerGerman::msdlParserWaeHandlerGerman ()
{}

msdlParserWaeHandlerGerman::~msdlParserWaeHandlerGerman ()
{}

void msdlParserWaeHandlerGerman::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::unexpectedToken (
  const msdlToken&   theToken,
  const std::string& context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::ignoringToken (
  const msdlToken&   theToken,
  const std::string& context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserWaeHandlerGerman::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserWaeHandlerGerman::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserWaeHandlerGerman::print (std::ostream& os) const
{
  os <<
    "msdlParserWaeHandlerGerman" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerGerman& elt)
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