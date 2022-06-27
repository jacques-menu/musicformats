/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

// WAE
#include "oahWae.h"

#include "enableHarmoniesExtraOahIfDesired.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

// OAH
#include "oahOah.h"
#include "waeOah.h"
#include "displayOah.h"

// early options
#include "oahEarlyOptions.h"

// components
#include "mfslInterpreterComponent.h"

#include "mfslInterpreterInterface.h"

#include "mfslInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfslInterpreterInsiderHandler mfslInterpreterInsiderHandler::create (
  const string& serviceName,
  const string& handlerHeader)
{
  // create the insider handler
  mfslInterpreterInsiderHandler* o = new
    mfslInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o != nullptr);

  return o;
}

mfslInterpreterInsiderHandler::mfslInterpreterInsiderHandler (
  const string& serviceName,
  const string& handlerHeader)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" regular options handler" <<
      endl;
  }
#endif

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

string mfslInterpreterInsiderHandler::usageInformation ()
{
  stringstream s;

  s <<
R"(Usage: mfsl [option]*
)" <<
    endl;

  return s.str ();
}

string mfslInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    mfslInterpreterAboutInformation ();
}

string mfslInterpreterInsiderHandler::mfslInterpreterAboutInformation () const
{
  string result =
R"(What mfsl does:

    This interpreter reads text input containing
    a tool name, an input source name, keywords and options,
    and launches the specified tool
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the mfsl prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::createTheMfslInterpreterOptionGroups (
  const string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the \"" <<
      fHandlerHeader <<
      "\" insider option groups" <<
      endl;
  }
#endif

  // initialize options handling, phase 1
  // ------------------------------------------------------

  // create the OAH OAH group first
  appendGroupToHandler (
    createGlobalOahOahGroup (
      serviceName));

  // create the WAE OAH group
  appendGroupToHandler (
    createGlobalWaeOahGroup ());

#ifdef TRACING_IS_ENABLED
  // create the tracing OAH group
  appendGroupToHandler (
    createGlobalTracingOahGroup (
      this));
#endif

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

// JMI

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  if (MSDR_STANDARD_INPUT_NAME == string ("-")) {
    checkSingleInputSourceInArgumentsVector ();
  }
  else if (MSDR_STANDARD_INPUT_NAME == string ("")) {
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
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

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
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    stringstream s;

    s <<
      "mfslInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gGlobalServiceRunData->getInputSourceName ()) {
    stringstream s;

    s <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mfslInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterInsiderOahGroup>*> (v)) {
        S_mfslInterpreterInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mfslInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mfslInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterInsiderOahGroup>*> (v)) {
        S_mfslInterpreterInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mfslInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void mfslInterpreterInsiderHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

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

ostream& operator<< (ostream& os, const S_mfslInterpreterInsiderHandler& elt)
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
  mfslInterpreterInsiderOahGroup* o = new mfslInterpreterInsiderOahGroup ();
  assert (o != nullptr);

  return o;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream << left <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group" <<
      endl;
  }
#endif

  // JMI ???
}

//______________________________________________________________________________
void mfslInterpreterInsiderOahGroup::printMfslInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The mfsl options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_mfslInterpreterInsiderOahGroup createGlobalMfslInterpreterInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global mfsl insider OAH group" <<
      endl;
  }
#endif

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
