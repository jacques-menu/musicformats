/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

#include "mfStringsHandling.h"

#include "oahWae.h"

#include "oahEarlyOptions.h"

#include "oahInsiderHandlers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
/* this class is purely virtual
S_oahInsiderHandler oahInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader,
  const std::string&      handlerDescription,
  const std::string&      handlerUsage)
{
  // create the insider handler
  oahInsiderHandler* obj = new
    oahInsiderHandler (
      serviceName,
      handlerHeader,
      handlerDescription,
      handlerUsage);
  assert (obj != nullptr);
  return obj;
}
*/

oahInsiderHandler::oahInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader,
  const std::string&      handlerDescription,
  const std::string&      handlerUsage)
  : oahHandler (
      serviceName,
      handlerHeader,
      handlerDescription,
      handlerUsage)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" insider handler";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

oahInsiderHandler::~oahInsiderHandler ()
{}

std::string oahInsiderHandler::asString () const
{
  std::stringstream ss;

  ss <<
    "oahInsiderHandler \"" << fHandlerHeader; // JMI 0.9.65

  return ss.str ();
}

void oahInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "oahInsiderHandler:" <<
    std::endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << std::endl;

  os <<
    "Options groups (" <<
    mfSingularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    std::endl;

  if (fHandlerGroupsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahInsiderHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
