/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msdlParserWaeHandlersEnglish.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msdlParserWaeHandlerEnglish msdlParserWaeHandlerEnglish::create ()
{
  msdlParserWaeHandlerEnglish* o =
    new msdlParserWaeHandlerEnglish ();
  assert (o != nullptr);
  return o;
}

msdlParserWaeHandlerEnglish::msdlParserWaeHandlerEnglish ()
{}

msdlParserWaeHandlerEnglish::~msdlParserWaeHandlerEnglish ()
{}

void msdlParserWaeHandlerEnglish::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserWaeHandlerEnglish::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    std::endl;
}

void msdlParserWaeHandlerEnglish::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    std::endl;
}

void msdlParserWaeHandlerEnglish::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    std::endl;
}

void msdlParserWaeHandlerEnglish::unexpectedToken (
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

void msdlParserWaeHandlerEnglish::ignoringToken (
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

void msdlParserWaeHandlerEnglish::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    std::endl;
}

void msdlParserWaeHandlerEnglish::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserWaeHandlerEnglish::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserWaeHandlerEnglish::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserWaeHandlerEnglish::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL lexical english WAE handler";

  return ss.str ();
}

void msdlParserWaeHandlerEnglish::print (std::ostream& os) const
{
  os <<
    "msdlParserWaeHandlerEnglish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerEnglish& elt)
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
