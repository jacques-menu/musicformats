/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msdlParserWaeHandlersItalian.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msdlParserWaeHandlerItalian msdlParserWaeHandlerItalian::create ()
{
  msdlParserWaeHandlerItalian* o =
    new msdlParserWaeHandlerItalian ();
  assert (o != nullptr);
  return o;
}

msdlParserWaeHandlerItalian::msdlParserWaeHandlerItalian ()
{}

msdlParserWaeHandlerItalian::~msdlParserWaeHandlerItalian ()
{}

void msdlParserWaeHandlerItalian::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserWaeHandlerItalian::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    std::endl;
}

void msdlParserWaeHandlerItalian::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    std::endl;
}

void msdlParserWaeHandlerItalian::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    std::endl;
}

void msdlParserWaeHandlerItalian::unexpectedToken (
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

void msdlParserWaeHandlerItalian::ignoringToken (
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

void msdlParserWaeHandlerItalian::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    std::endl;
}

void msdlParserWaeHandlerItalian::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserWaeHandlerItalian::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserWaeHandlerItalian::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserWaeHandlerItalian::asString () const
{
  std::stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserWaeHandlerItalian::print (std::ostream& os) const
{
  os <<
    "msdlParserWaeHandlerItalian" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerItalian& elt)
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