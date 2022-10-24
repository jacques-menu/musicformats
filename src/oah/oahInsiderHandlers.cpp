/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStringsHandling.h"

#include "enableHarmoniesExtraOahIfDesired.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "oahInsiderHandlers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
/* this class is purely virtual
S_oahInsiderHandler oahInsiderHandler::create (
  const string& serviceName,
  const string& handlerHeader,
  const string&           handlerDescription,
  const string&           handlerUsage)
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
  const string& serviceName,
  const string& handlerHeader,
  const string&           handlerDescription,
  const string&           handlerUsage)
  : oahHandler (
      serviceName,
      handlerHeader,
      handlerDescription,
      handlerUsage)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" insider handler" <<
      endl;
  }
#endif
}

oahInsiderHandler::~oahInsiderHandler ()
{}

string oahInsiderHandler::asString () const
{
  stringstream s;

  s <<
    "oahInsiderHandler \"" << fHandlerHeader; // JMI v0.9.65

  return s.str ();
}

void oahInsiderHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "oahInsiderHandler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << endl;

  os <<
    "Options groups (" <<
    mfSingularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

ostream& operator << (ostream& os, const S_oahInsiderHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
