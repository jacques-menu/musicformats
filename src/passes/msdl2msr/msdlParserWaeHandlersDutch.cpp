/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msdlParserWaeHandlersDutch.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msdlParserWaeHandlerDutch msdlParserWaeHandlerDutch::create ()
{
  msdlParserWaeHandlerDutch* obj =
    new msdlParserWaeHandlerDutch ();
  assert (obj != nullptr);
  return obj;
}

msdlParserWaeHandlerDutch::msdlParserWaeHandlerDutch ()
{}

msdlParserWaeHandlerDutch::~msdlParserWaeHandlerDutch ()
{}

void msdlParserWaeHandlerDutch::multipleIdentifications () const
{
  gLog <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::stringExpectedAsTitle () const
{
  gLog <<
    "### a string is expected as title ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::stringExpectedAsComposer () const
{
  gLog <<
    "### a string is expected as composer ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::stringExpectedAsOpus () const
{
  gLog <<
    "### a string is expected as opus ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::unexpectedToken (
  const msdlToken&   theToken,
  const std::string& context) const
{
  gLog <<
    "### " << context << ": " <<
    "unexpected token: " <<
    '\'' << theToken.asString () <<  '\'' <<
    " ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::ignoringToken (
  const msdlToken&   theToken,
  const std::string& context) const
{
  gLog <<
    "### " << context << ": " <<
    "ignoring token: " <<
    '\'' << theToken.asString () <<  '\'' <<
    " ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::inputIsSyntacticallyCorrect () const
{
  gLog <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::inputIsSyntacticallyIncorrect () const
{
  gLog <<
    "### The MSDL input is syntactically incorrect ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::mixedCommasAndQuotesInOctaveIndication () const
{
  gLog <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    std::endl;
}

void msdlParserWaeHandlerDutch::malformedNoteNotesDuration () const
{
  gLog <<
    "### Malformed note duration ###" <<
    std::endl;
}

std::string msdlParserWaeHandlerDutch::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL lexical english WAE handler";

  return ss.str ();
}

void msdlParserWaeHandlerDutch::print (std::ostream& os) const
{
  os <<
    "msdlParserWaeHandlerDutch" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdlParserWaeHandlerDutch& elt)
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
