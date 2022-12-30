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

#include "msdlParserWaeHandlersSpanish.h"


namespace MusicFormats
{

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
    "### a string is expected as title ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    std::endl;
}

void msdlParserSpanishWaeHandler::unexpectedToken (
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

void msdlParserSpanishWaeHandler::ignoringToken (
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


} // namespace
