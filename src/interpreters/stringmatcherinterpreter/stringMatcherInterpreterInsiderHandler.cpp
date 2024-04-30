/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "mfServices.h"

#include "mfStringsHandling.h"

// OAH
#include "oahOah.h"
#include "waeOah.h"
#include "oahDisplayOah.h"

// early options
#include "oahEarlyOptions.h"

// WAE
#include "waeHandlers.h"
#include "oahWae.h"

// components
#include "stringMatcherInterpreterComponent.h"

#include "stringMatcherInterpreterInterface.h"

#include "stringMatcherInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_stringMatcherInterpreterInsiderHandler stringMatcherInterpreterInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  stringMatcherInterpreterInsiderHandler* obj = new
    stringMatcherInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj != nullptr);

  return obj;
}

stringMatcherInterpreterInsiderHandler::stringMatcherInterpreterInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the stringMatcher interpreter,
     a generator of Guido, MusicXML, LilyPond or Braille
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
      usageInformation ())
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the stringMatcher prefixes
  createThestringMatcherInterpreterPrefixes ();

  // create the stringMatcher option groups
  createThestringMatcherInterpreterOptionGroups (
    serviceName);
}

stringMatcherInterpreterInsiderHandler::~stringMatcherInterpreterInsiderHandler ()
{}

void stringMatcherInterpreterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createstringMatcherInterpreterComponent ();
}

std::string stringMatcherInterpreterInsiderHandler::usageInformation ()
{
  stringstream s;

  s <<
R"(Usage: stringMatcher [option]*
)" <<
    endl;

  return s.str ();
}

std::string stringMatcherInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    stringMatcherInterpreterAboutInformation ();
}

std::string stringMatcherInterpreterInsiderHandler::stringMatcherInterpreterAboutInformation () const
{
  std::string result =
R"(What stringMatcher does:

    This interpreter reads text input containing
    a service name, an input source name, keywords and options,
    and launches the specified service
    with these options applied to the input source name.

    The input can come from a file or from standard input,
    allowing the interpreter to be used interactively or in shell pipes.

    The activity log and warning/error messages go to standard error.)";

  return
    result;
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderHandler::createThestringMatcherInterpreterPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the stringMatcher prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderHandler::createThestringMatcherInterpreterOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize options handling, phase 1
  // ------------------------------------------------------

  // create the OAH OAH group first
  appendGroupToHandler (
    createGlobalOahOahGroup (
      serviceName));

  // create the WAE OAH group
  appendGroupToHandler (
    createGlobalWaeOahGroup ());

#ifdef MF_TRACE_IS_ENABLED
  // create the tracing OAH group
  appendGroupToHandler (
    createGlobalTraceOahGroup (
      this));
#endif // MF_TRACE_IS_ENABLED

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  // JMI ??? v0.9.67

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the stringMatcher OAH group
  appendGroupToHandler (
    createGlobalstringMatcherInterpreterOahGroup ());

  // create the stringMatcher insider OAH group
  appendGroupToHandler (
    createGlobalstringMatcherInterpreterInsiderOahGroup ());
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (MSDR_STANDARD_INPUT_NAME == std::string ("-")) {
    checkOneArgumentInArgumentsVector ();
  }
  else if (MSDR_STANDARD_INPUT_NAME == std::string ("")) {
    checkOneArgumentAndNoOrOneInputSourceInArgumentsVector ();
  }
  else {
    oahInternalError (
      "MSDR_STANDARD_INPUT_NAME should be empty or contain a single '-'");
  }
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalstringMatcherInterpreterInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    stringstream s;

    s <<
      "stringMatcherInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gServiceRunData->getInputSourceName ()) {
    stringstream s;

    s <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringMatcherInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_stringMatcherInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_stringMatcherInterpreterInsiderOahGroup>*> (v)) {
        S_stringMatcherInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching stringMatcherInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void stringMatcherInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringMatcherInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_stringMatcherInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_stringMatcherInterpreterInsiderOahGroup>*> (v)) {
        S_stringMatcherInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching stringMatcherInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void stringMatcherInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringMatcherInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "stringMatcherInterpreterInsiderHandler:" <<
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

std::ostream& operator<< (std::ostream& os, const S_stringMatcherInterpreterInsiderHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }

  return os;
}

//_______________________________________________________________________________
S_stringMatcherInterpreterInsiderOahGroup gGlobalstringMatcherInterpreterInsiderOahGroup;

S_stringMatcherInterpreterInsiderOahGroup stringMatcherInterpreterInsiderOahGroup::create ()
{
  stringMatcherInterpreterInsiderOahGroup* obj = new stringMatcherInterpreterInsiderOahGroup ();
  assert (obj != nullptr);

  return obj;
}

stringMatcherInterpreterInsiderOahGroup::stringMatcherInterpreterInsiderOahGroup ()
  : oahGroup (
      "stringMatcher",
      "help-stringMatcher-group", "hstringMatcherg",
R"(Options that are used by stringMatcher are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializestringMatcherInterpreterInsiderOahGroup ();
}

stringMatcherInterpreterInsiderOahGroup::~stringMatcherInterpreterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void stringMatcherInterpreterInsiderOahGroup::initializestringMatcherInterpreterInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog << left <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI ???
}

//______________________________________________________________________________
void stringMatcherInterpreterInsiderOahGroup::printstringMatcherInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The stringMatcher options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_stringMatcherInterpreterInsiderOahGroup createGlobalstringMatcherInterpreterInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global stringMatcher insider OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalstringMatcherInterpreterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalstringMatcherInterpreterInsiderOahGroup =
      stringMatcherInterpreterInsiderOahGroup::create ();
    assert (gGlobalstringMatcherInterpreterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalstringMatcherInterpreterInsiderOahGroup;
}


}
