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
#include "mfslInterpreterComponent.h"

#include "mfslInterpreterInterface.h"

#include "mfslInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfslInterpreterInsiderHandler mfslInterpreterInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  mfslInterpreterInsiderHandler* obj = new
    mfslInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj != nullptr);

  return obj;
}

mfslInterpreterInsiderHandler::mfslInterpreterInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the MFSL interpreter,
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

  // create the mfsl prefixes
  createTheMfslInterpreterPrefixes ();

  // create the mfsl option groups
  createTheMfslInterpreterOptionGroups (
    serviceName);
}

mfslInterpreterInsiderHandler::~mfslInterpreterInsiderHandler ()
{}

void mfslInterpreterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createMfslInterpreterComponent ();
}

std::string mfslInterpreterInsiderHandler::usageInformation ()
{
  std::stringstream ss;

  ss <<
R"(Usage: mfsl [option]*
)" <<
    endl;

  return ss.str ();
}

std::string mfslInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    mfslInterpreterAboutInformation ();
}

std::string mfslInterpreterInsiderHandler::mfslInterpreterAboutInformation () const
{
  std::string result =
R"(What mfsl does:

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
void mfslInterpreterInsiderHandler::createTheMfslInterpreterPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the mfsl prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::createTheMfslInterpreterOptionGroups (
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

  // create the mfsl OAH group
  appendGroupToHandler (
    createGlobalMfslInterpreterOahGroup ());

  // create the mfsl insider OAH group
  appendGroupToHandler (
    createGlobalMfslInterpreterInsiderOahGroup ());
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::checkOptionsAndArguments () const
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
void mfslInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalMfslInterpreterInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "mfslInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void mfslInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfslInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterInsiderOahGroup>*> (v)) {
        S_mfslInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mfslInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mfslInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfslInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterInsiderOahGroup>*> (v)) {
        S_mfslInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mfslInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mfslInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "mfslInterpreterInsiderHandler:" <<
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

std::ostream& operator<< (std::ostream& os, const S_mfslInterpreterInsiderHandler& elt)
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
S_mfslInterpreterInsiderOahGroup gGlobalMfslInterpreterInsiderOahGroup;

S_mfslInterpreterInsiderOahGroup mfslInterpreterInsiderOahGroup::create ()
{
  mfslInterpreterInsiderOahGroup* obj = new mfslInterpreterInsiderOahGroup ();
  assert (obj != nullptr);

  return obj;
}

mfslInterpreterInsiderOahGroup::mfslInterpreterInsiderOahGroup ()
  : oahGroup (
      "mfsl",
      "help-mfsl-group", "hmfslg",
R"(Options that are used by mfsl are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMfslInterpreterInsiderOahGroup ();
}

mfslInterpreterInsiderOahGroup::~mfslInterpreterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::initializeMfslInterpreterInsiderOahGroup ()
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
void mfslInterpreterInsiderOahGroup::printMfslInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The mfsl options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_mfslInterpreterInsiderOahGroup createGlobalMfslInterpreterInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global mfsl insider OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMfslInterpreterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMfslInterpreterInsiderOahGroup =
      mfslInterpreterInsiderOahGroup::create ();
    assert (gGlobalMfslInterpreterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMfslInterpreterInsiderOahGroup;
}


}
