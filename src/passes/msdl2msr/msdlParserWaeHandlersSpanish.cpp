/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msdlParserWaeHandlersSpanish.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msdlParserWaeHandlerSpanish msdlParserWaeHandlerSpanish::create ()
{
  msdlParserWaeHandlerSpanish* o =
    new msdlParserWaeHandlerSpanish ();
  assert (o != nullptr);
  return o;
}

msdlParserWaeHandlerSpanish::msdlParserWaeHandlerSpanish ()
{}

msdlParserWaeHandlerSpanish::~msdlParserWaeHandlerSpanish ()
{}

void msdlParserWaeHandlerSpanish::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserWaeHandlerSpanish::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    std::endl;
}

void msdlParserWaeHandlerSpanish::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    std::endl;
}

void msdlParserWaeHandlerSpanish::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    std::endl;
}

void msdlParserWaeHandlerSpanish::unexpectedToken (
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

void msdlParserWaeHandlerSpanish::ignoringToken (
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

void msdlParserWaeHandlerSpanish::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    std::endl;
}

void msdlParserWaeHandlerSpanish::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserWaeHandlerSpanish::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserWaeHandlerSpanish::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserWaeHandlerSpanish::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserWaeHandlerSpanish::print (std::ostream& os) const
{
  os <<
    "msdlParserWaeHandlerSpanish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerSpanish& elt)
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