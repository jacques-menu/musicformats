/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStringsHandling.h"

#include "mfEnableHarmoniesExtra.h"
#include "oahWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "oahInsiderHandlers.h"


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
  oahInsiderHandler* o = new
    oahInsiderHandler (
      serviceName,
      handlerHeader,
      handlerDescription,
      handlerUsage);
  assert (o != nullptr);
  return o;
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" insider handler" <<
      std::endl;
  }
#endif
}

oahInsiderHandler::~oahInsiderHandler ()
{}

std::string oahInsiderHandler::asString () const
{
  std::stringstream s;

  s <<
    "oahInsiderHandler \"" << fHandlerHeader; // JMI v0.9.65

  return s.str ();
}

void oahInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

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

    std::list<S_oahGroup>::const_iterator
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
