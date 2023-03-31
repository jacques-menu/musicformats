/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <regex>

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
#include "oahWae.h"

#include "oahEarlyOptions.h"

#include "oahRegularHandlers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
/* this class is purely virtual
S_oahRegularHandler oahRegularHandler::create (
  std::string       serviceName,
  const std::string&     handlerHeader,
  const S_oahHandler&      insiderOahHandler)
{
  // create the regular handler
  oahRegularHandler* obj = new
    oahRegularHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler);
  assert (obj != nullptr);
  return obj;
}
*/

oahRegularHandler::oahRegularHandler (
  std::string       serviceName,
  const std::string&     handlerHeader,
  const S_oahHandler&      insiderOahHandler)
  : oahHandler (
      serviceName,
      handlerHeader,
      insiderOahHandler->getHandlerDescription (),
      insiderOahHandler->getHandlerUsage ())
{
  // create the insider handler
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the insider handler for \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fInsiderHandler = insiderOahHandler;

  // initializeOahRegularHandler() should be called in sub-classes
  // after the corresponding constructor has been executed,
  // because it uses oahRegularHandler's pure virtual methods
}

oahRegularHandler::~oahRegularHandler ()
{}

void oahRegularHandler::initializeOahRegularHandler ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing the regular handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the prefixes from fInsiderHandler
  fHandlerPrefixesMap =
    fInsiderHandler->
      getHandlerPrefixesMap ();

  // createRegularHandlerGroups() is executed
  // in the sub-classes for code homogeneity

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
  // print the options handler initial state
  gLog <<
    "oahRegularHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    std::endl;

  ++gIndenter;

//  this->print (gLog); JMI v0.9.66
  this->printHelp (gOutput); // JMI

  gLog <<
    std::endl << std::endl;

  --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

void oahRegularHandler::appendGroupToRegulalHandler (
  const S_oahGroup& group)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    std::stringstream ss;

    ss <<
      "Appending group \"" <<
      group->getGroupHeader () <<
      "\" to regular handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const S_oahGroup& group)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    group != nullptr,
    "group is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Prepending group \"" <<
      group->getGroupHeader () <<
      "\" to regular handler \"" <<
      fHandlerHeader <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
  const std::string& atomName,
  const S_oahSubGroup& subGroup)
{
  S_oahElement
    insiderElement =
      fInsiderHandler->
        fetchElementByNameInHandler (
          atomName);

  if (insiderElement) {
    // atomName is the name of an option
#ifdef MF_TRACE_IS_ENABLED
    if (false) { // JMI
      std::stringstream ss;

      ss <<
        "===> insiderElement:" <<
        std::endl;
      ++gIndenter;
      gLog <<
        insiderElement <<
        std::endl;
      --gIndenter;

      gWaeHandler->waeTraceWithoutLocationDetails (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "atom name \"" << atomName <<
      "\" cannot be registered in OAH regular handler \"" <<
      fHandlerHeader <<
      "\", it is unknown to insider handler \"" <<
      fInsiderHandler->getHandlerHeader ()<<
      "\"";

 // JMI   oahError (ss.str ());
//     oahWarning (ss.str ()); // JMI TEMP v0.9.65
  }
}

//______________________________________________________________________________
std::string oahRegularHandler::asString () const
{
  std::stringstream ss;

  ss <<
    "oahRegularHandler \"" << fHandlerHeader; // JMI v0.9.65

  return ss.str ();
}

void oahRegularHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "oahRegularHandler \"" << fHandlerHeader << "\":" <<
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
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

if (false) { // JMI
  // print the known options
  os <<
    "oahRegularHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    std::endl <<
    "oahRegularHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    std::endl;
  displayNamesToElementsMap (os);
}

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_oahRegularHandler& elt)
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

/* JMI
// optional values style
//______________________________________________________________________________

std::map<std::string, oahOptionalValuesStyleKind>
  gGlobalOahOptionalValuesStyleKindsMap;

std::string oahOptionalValuesStyleKindAsString (
  oahOptionalValuesStyleKind optionalValuesStyleKind)
{
  std::string result;

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

std::string availableOahOptionalValuesStyleKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

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
      std::map<std::string, oahOptionalValuesStyleKind>::const_iterator i =
        gGlobalOahOptionalValuesStyleKindsMap.begin ();
      i != gGlobalOahOptionalValuesStyleKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != optionalValuesStyleKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}
*/
