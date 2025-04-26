/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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
#include "mnx2mnxsrInterpreterComponent.h"

#include "mnx2mnxsrInterpreterInterface.h"

#include "mnx2mnxsrInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mnx2mnxsrInterpreterInsiderHandler mnx2mnxsrInterpreterInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  mnx2mnxsrInterpreterInsiderHandler* obj = new
    mnx2mnxsrInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj != nullptr);

  return obj;
}

mnx2mnxsrInterpreterInsiderHandler::mnx2mnxsrInterpreterInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the MNX2MNXSR interpreter,
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

  // create the mnx2mnxsr prefixes
  createTheMnx2mnxsrInterpreterPrefixes ();

  // create the mnx2mnxsr option groups
  createTheMnx2mnxsrInterpreterOptionGroups (
    serviceName);
}

mnx2mnxsrInterpreterInsiderHandler::~mnx2mnxsrInterpreterInsiderHandler ()
{}

void mnx2mnxsrInterpreterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createMnx2mnxsrInterpreterComponent ();
}

std::string mnx2mnxsrInterpreterInsiderHandler::usageInformation ()
{
  std::stringstream ss;

  ss <<
R"(Usage: mnx2mnxsr [option]*
)" <<
    endl;

  return ss.str ();
}

std::string mnx2mnxsrInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    mnx2mnxsrInterpreterAboutInformation ();
}

std::string mnx2mnxsrInterpreterInsiderHandler::mnx2mnxsrInterpreterAboutInformation () const
{
  std::string result =
R"(What mnx2mnxsr does:

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
void mnx2mnxsrInterpreterInsiderHandler::createTheMnx2mnxsrInterpreterPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the mnx2mnxsr prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderHandler::createTheMnx2mnxsrInterpreterOptionGroups (
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

  // JMI ??? 0.9.67

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the mnx2mnxsr OAH group
  appendGroupToHandler (
    createGlobalMnx2mnxsrInterpreterOahGroup ());

  // create the mnx2mnxsr insider OAH group
  appendGroupToHandler (
    createGlobalMnx2mnxsrInterpreterInsiderOahGroup ());
}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderHandler::checkOptionsAndArguments () const
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
    checkSingleInputSourceInArgumentsVector ();
  }
  else if (MSDR_STANDARD_INPUT_NAME == std::string ("")) {
    checkNoOrOneInputSourceInArgumentsVector ();
  }
  else {
    oahInternalError (
      "MSDR_STANDARD_INPUT_NAME should be empty or contain a single '-'");
  }
}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalMnx2mnxsrInterpreterInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    std::stringstream ss;

    ss <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    std::stringstream ss;

    ss <<
      "mnx2mnxsrInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gServiceRunData->getInputSourceName ()) {
    std::stringstream ss;

    ss <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mnx2mnxsrInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mnx2mnxsrInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mnx2mnxsrInterpreterInsiderOahGroup>*> (v)) {
        S_mnx2mnxsrInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mnx2mnxsrInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mnx2mnxsrInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mnx2mnxsrInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mnx2mnxsrInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mnx2mnxsrInterpreterInsiderOahGroup>*> (v)) {
        S_mnx2mnxsrInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mnx2mnxsrInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mnx2mnxsrInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mnx2mnxsrInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "mnx2mnxsrInterpreterInsiderHandler:" <<
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

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInterpreterInsiderHandler& elt)
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
S_mnx2mnxsrInterpreterInsiderOahGroup gGlobalMnx2mnxsrInterpreterInsiderOahGroup;

S_mnx2mnxsrInterpreterInsiderOahGroup mnx2mnxsrInterpreterInsiderOahGroup::create ()
{
  mnx2mnxsrInterpreterInsiderOahGroup* obj = new mnx2mnxsrInterpreterInsiderOahGroup ();
  assert (obj != nullptr);

  return obj;
}

mnx2mnxsrInterpreterInsiderOahGroup::mnx2mnxsrInterpreterInsiderOahGroup ()
  : oahGroup (
      "mnx2mnxsr",
      "help-mnx2mnxsr-group", "hmnx2mnxsrg",
R"(Options that are used by mnx2mnxsr are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMnx2mnxsrInterpreterInsiderOahGroup ();
}

mnx2mnxsrInterpreterInsiderOahGroup::~mnx2mnxsrInterpreterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void mnx2mnxsrInterpreterInsiderOahGroup::initializeMnx2mnxsrInterpreterInsiderOahGroup ()
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
void mnx2mnxsrInterpreterInsiderOahGroup::printMnx2mnxsrInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The mnx2mnxsr options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_mnx2mnxsrInterpreterInsiderOahGroup createGlobalMnx2mnxsrInterpreterInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global mnx2mnxsr insider OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMnx2mnxsrInterpreterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMnx2mnxsrInterpreterInsiderOahGroup =
      mnx2mnxsrInterpreterInsiderOahGroup::create ();
    assert (gGlobalMnx2mnxsrInterpreterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMnx2mnxsrInterpreterInsiderOahGroup;
}


}
