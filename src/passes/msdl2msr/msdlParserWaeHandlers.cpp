/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

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
  msdlParserWaeHandler* o =
    new msdlParserWaeHandler ();
  assert (o != nullptr);
  return o;
}
*/

msdlParserWaeHandler::msdlParserWaeHandler ()
{}

msdlParserWaeHandler::~msdlParserWaeHandler ()
{}

std::string msdlParserWaeHandler::asString () const
{
  std::stringstream s;

  s <<
    "MSDL WAE lexical handler";

  return s.str ();
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


} // namespace
