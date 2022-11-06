/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <regex>

#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfStringsHandling.h"


#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "oahRegularHandlers.h"



namespace MusicFormats
{

//______________________________________________________________________________
/* this class   is purely virtual
S_oahRegularHandler oahRegularHandler::create (
  string            serviceName,
  const string&     handlerHeader,
  S_oahHandler      insiderOahHandler)
{
  // create the regular handler
  oahRegularHandler* o = new
    oahRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (o != nullptr);

  return o;
}
*/

oahRegularHandler::oahRegularHandler (
  string            serviceName,
  const string&     handlerHeader,
  S_oahHandler      insiderOahHandler)
  : oahHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler->getHandlerDescription (),
      insiderOahHandler->getHandlerUsage ())
{
  // create the insider handler
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the insider handler for \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  fInsiderHandler = insiderOahHandler;

  // initializeOahRegularHandler() should be called in sub-classes
  // after the corresponding constructor has been executed,
  // because it uses oahRegularHandler's pure virtual methods
}

oahRegularHandler::~oahRegularHandler ()
{}

void oahRegularHandler::initializeOahRegularHandler ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing the regular handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // get the prefixes from fInsiderHandler
  fHandlerPrefixesMap =
    fInsiderHandler->
      getHandlerPrefixesMap ();

  // createRegularHandlerGroups() is executed
  // in the sub-classes for code homogeneity

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
  // print the options handler initial state
  gLogStream <<
    "oahRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    endl;

  ++gIndenter;

//  this->printShort (gLogStream);
  this->printHelp (gOutputStream); // JMI

  gLogStream <<
    endl << endl;

  --gIndenter;
  }
#endif
}

void oahRegularHandler::appendGroupToRegulalHandler (
  S_oahGroup group)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    gLogStream <<
      "Appending group \"" <<
      group->getGroupHeader () <<
      "\" to regular handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // a regular group should not be displayed
  group->
    setGroupHeaderIsToBeWritten (false);

  // append group to elements list
  group->
    appendGroupToElementsList (this);

  // append group to groups list
  fHandlerGroupsList.push_back (group);

  // set the group upLink to this handler
  group->
    setUpLinkToHandler (this);
}

void oahRegularHandler::prependGroupToRegularHandler (
  S_oahGroup group)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Prepending group \"" <<
      group->getGroupHeader () <<
      "\" to regular handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  // a regular group should not be displayed
  group->
    setGroupHeaderIsToBeWritten (false);

  // prepend group  to elements list
  group->
    appendGroupToElementsList (this);

  // prepend group to the groups list
  fHandlerGroupsList.push_front (group);

  // set the group upLink to this handler
  group->
    setUpLinkToHandler (this);
}

void oahRegularHandler::registerAtomInRegularSubgroup (
  const string& atomName,
  S_oahSubGroup subGroup)
{
  S_oahElement
    insiderElement =
      fInsiderHandler->
        fetchElementByNameInHandler (
          atomName);

  if (insiderElement) {
    // atomName is the name of an option
    if (false) { // JMI
      gLogStream <<
        "===> insiderElement:" <<
        endl;
      ++gIndenter;
      gLogStream <<
        insiderElement <<
        endl;
      --gIndenter;
    }

    if (
      // atom?
      S_oahAtom
        atom =
          dynamic_cast<oahAtom*>(&(*insiderElement))
      ) {
        // append atom to subgroup, thus modify its fUpLinkToSubGroup
        // that previous linked it to an insider handler subgroup
        subGroup->
          appendAtomToSubGroup (atom);
    }

    else {
      unknownOptionNameError (
        atomName,
        "not that of an atom when registered in a subgroup");
    }
  }

  else {
    stringstream s;

    s <<
      "atom name \"" << atomName <<
      "\" cannot be registered in OAH regular handler \"" <<
      fHandlerHeader <<
      "\", it is unknown to insider handler \"" <<
      fInsiderHandler->getHandlerHeader ()<<
      "\"";

 // JMI   oahError (s.str ());
//     oahWarning (s.str ()); // JMI TEMP v0.9.65
  }
}

//______________________________________________________________________________
string oahRegularHandler::asString () const
{
  stringstream s;

  s <<
    "oahRegularHandler \"" << fHandlerHeader; // JMI v0.9.65

  return s.str ();
}

void oahRegularHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "oahRegularHandler \"" << fHandlerHeader << "\":" <<
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
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

if (false) { // JMI
  // print the known options
  os <<
    "oahRegularHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    endl <<
    "oahRegularHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    endl;
  displayNamesToElementsMap (os);
}

  --gIndenter;

  os << endl;
}

ostream& operator << (ostream& os, const S_oahRegularHandler& elt)
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

/* JMI
// optional values style
//______________________________________________________________________________

map<string, oahOptionalValuesStyleKind>
  gGlobalOahOptionalValuesStyleKindsMap;

string oahOptionalValuesStyleKindAsString (
  oahOptionalValuesStyleKind optionalValuesStyleKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options

  switch (optionalValuesStyleKind) {
    case kOptionalValuesStyleGNU: // default value
      result = "gnu";
      break;
    case kOptionalValuesStyleOAH:
      result = "oah";
      break;
  } // switch

  return result;
}

void initializeOahOptionalValuesStyleKindsMap ()
{
  // register the optional values style kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalOahOptionalValuesStyleKindsMap ["gnu"] = kOptionalValuesStyleGNU;
  gGlobalOahOptionalValuesStyleKindsMap ["oah"] = kOptionalValuesStyleOAH;
}

string existingOahOptionalValuesStyleKinds (size_t namesListMaxLength)
{
  stringstream s;

  size_t
    optionalValuesStyleKindsMapSize =
      gGlobalOahOptionalValuesStyleKindsMap.size ();

  if (optionalValuesStyleKindsMapSize) {
    int
      nextToLast =
        optionalValuesStyleKindsMapSize - 1;

    int count = 0;
    int cumulatedLength = 0;

    for (
      map<string, oahOptionalValuesStyleKind>::const_iterator i =
        gGlobalOahOptionalValuesStyleKindsMap.begin ();
      i != gGlobalOahOptionalValuesStyleKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != optionalValuesStyleKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}
*/
