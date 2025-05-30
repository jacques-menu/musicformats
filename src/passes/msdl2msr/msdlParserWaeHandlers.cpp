/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

#include "oahOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "msdlParserWaeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class is purely virtual
S_msdlParserWaeHandler msdlParserWaeHandler::create ()
{
  msdlParserWaeHandler* obj =
    new msdlParserWaeHandler ();
  assert (obj != nullptr);
  return obj;
}
*/

msdlParserWaeHandler::msdlParserWaeHandler ()
{}

msdlParserWaeHandler::~msdlParserWaeHandler ()
{}

std::string msdlParserWaeHandler::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL WAE lexical handler";

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


} // namespace
