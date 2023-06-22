/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

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
#include "stringFilterInterpreterComponent.h"

#include "stringFilterInterpreterInterface.h"

#include "stringFilterInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_stringFilterInterpreterInsiderHandler stringFilterInterpreterInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  stringFilterInterpreterInsiderHandler* obj = new
    stringFilterInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj != nullptr);

  return obj;
}

stringFilterInterpreterInsiderHandler::stringFilterInterpreterInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the stringFilter interpreter,
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

  // create the stringFilter prefixes
  createThestringFilterInterpreterPrefixes ();

  // create the stringFilter option groups
  createThestringFilterInterpreterOptionGroups (
    serviceName);
}

stringFilterInterpreterInsiderHandler::~stringFilterInterpreterInsiderHandler ()
{}

void stringFilterInterpreterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createstringFilterInterpreterComponent ();
}

std::string stringFilterInterpreterInsiderHandler::usageInformation ()
{
  stringstream s;

  s <<
R"(Usage: stringFilter [option]*
)" <<
    endl;

  return s.str ();
}

std::string stringFilterInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    stringFilterInterpreterAboutInformation ();
}

std::string stringFilterInterpreterInsiderHandler::stringFilterInterpreterAboutInformation () const
{
  std::string result =
R"(What stringFilter does:

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
void stringFilterInterpreterInsiderHandler::createThestringFilterInterpreterPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the stringFilter prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void stringFilterInterpreterInsiderHandler::createThestringFilterInterpreterOptionGroups (
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

  // create the stringFilter OAH group
  appendGroupToHandler (
    createGlobalstringFilterInterpreterOahGroup ());

  // create the stringFilter insider OAH group
  appendGroupToHandler (
    createGlobalstringFilterInterpreterInsiderOahGroup ());
}

//______________________________________________________________________________
void stringFilterInterpreterInsiderHandler::checkOptionsAndArguments () const
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
void stringFilterInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void stringFilterInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalstringFilterInterpreterInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void stringFilterInterpreterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void stringFilterInterpreterInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "stringFilterInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void stringFilterInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringFilterInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_stringFilterInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_stringFilterInterpreterInsiderOahGroup>*> (v)) {
        S_stringFilterInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching stringFilterInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void stringFilterInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringFilterInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_stringFilterInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_stringFilterInterpreterInsiderOahGroup>*> (v)) {
        S_stringFilterInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching stringFilterInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void stringFilterInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringFilterInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void stringFilterInterpreterInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "stringFilterInterpreterInsiderHandler:" <<
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

std::ostream& operator<< (std::ostream& os, const S_stringFilterInterpreterInsiderHandler& elt)
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
S_stringFilterInterpreterInsiderOahGroup gGlobalstringFilterInterpreterInsiderOahGroup;

S_stringFilterInterpreterInsiderOahGroup stringFilterInterpreterInsiderOahGroup::create ()
{
  stringFilterInterpreterInsiderOahGroup* obj = new stringFilterInterpreterInsiderOahGroup ();
  assert (obj != nullptr);

  return obj;
}

stringFilterInterpreterInsiderOahGroup::stringFilterInterpreterInsiderOahGroup ()
  : oahGroup (
      "stringFilter",
      "help-stringFilter-group", "hstringFilterg",
R"(Options that are used by stringFilter are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializestringFilterInterpreterInsiderOahGroup ();
}

stringFilterInterpreterInsiderOahGroup::~stringFilterInterpreterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void stringFilterInterpreterInsiderOahGroup::initializestringFilterInterpreterInsiderOahGroup ()
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
void stringFilterInterpreterInsiderOahGroup::printstringFilterInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The stringFilter options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_stringFilterInterpreterInsiderOahGroup createGlobalstringFilterInterpreterInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global stringFilter insider OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalstringFilterInterpreterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalstringFilterInterpreterInsiderOahGroup =
      stringFilterInterpreterInsiderOahGroup::create ();
    assert (gGlobalstringFilterInterpreterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalstringFilterInterpreterInsiderOahGroup;
}


}
