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
#include "ischemeInterpreterComponent.h"

#include "ischemeInterpreterInterface.h"

#include "ischemeInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_ischemeInterpreterInsiderHandler ischemeInterpreterInsiderHandler::create (
  const string& serviceName,
  const string& handlerHeader)
{
  // create the insider handler
  ischemeInterpreterInsiderHandler* o = new
    ischemeInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o != nullptr);

  return o;
}

ischemeInterpreterInsiderHandler::ischemeInterpreterInsiderHandler (
  const string& serviceName,
  const string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the iScheme interpreter,
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

  // create the ischeme prefixes
  createTheIschemeInterpreterPrefixes ();

  // create the ischeme option groups
  createTheIschemeInterpreterOptionGroups (
    serviceName);
}

ischemeInterpreterInsiderHandler::~ischemeInterpreterInsiderHandler ()
{}

void ischemeInterpreterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createIschemeInterpreterComponent ();
}

string ischemeInterpreterInsiderHandler::usageInformation ()
{
  stringstream s;

  s <<
R"(Usage: ischeme [option]*
)" <<
    endl;

  return s.str ();
}

string ischemeInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    ischemeInterpreterAboutInformation ();
}

string ischemeInterpreterInsiderHandler::ischemeInterpreterAboutInformation () const
{
  string result =
R"(What ischeme does:

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
void ischemeInterpreterInsiderHandler::createTheIschemeInterpreterPrefixes ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the ischeme prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void ischemeInterpreterInsiderHandler::createTheIschemeInterpreterOptionGroups (
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

  // create the ischeme OAH group
  appendGroupToHandler (
    createGlobalIschemeInterpreterOahGroup ());

  // create the ischeme insider OAH group
  appendGroupToHandler (
    createGlobalIschemeInterpreterInsiderOahGroup ());
}

//______________________________________________________________________________
void ischemeInterpreterInsiderHandler::checkOptionsAndArguments () const
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
void ischemeInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void ischemeInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalIschemeInterpreterInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void ischemeInterpreterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void ischemeInterpreterInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "ischemeInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void ischemeInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> ischemeInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_ischemeInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterInsiderOahGroup>*> (v)) {
        S_ischemeInterpreterInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching ischemeInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void ischemeInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> ischemeInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_ischemeInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterInsiderOahGroup>*> (v)) {
        S_ischemeInterpreterInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching ischemeInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void ischemeInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> ischemeInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void ischemeInterpreterInsiderHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "ischemeInterpreterInsiderHandler:" <<
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

ostream& operator<< (ostream& os, const S_ischemeInterpreterInsiderHandler& elt)
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
S_ischemeInterpreterInsiderOahGroup gGlobalIschemeInterpreterInsiderOahGroup;

S_ischemeInterpreterInsiderOahGroup ischemeInterpreterInsiderOahGroup::create ()
{
  ischemeInterpreterInsiderOahGroup* o = new ischemeInterpreterInsiderOahGroup ();
  assert (o != nullptr);

  return o;
}

ischemeInterpreterInsiderOahGroup::ischemeInterpreterInsiderOahGroup ()
  : oahGroup (
    "ischeme",
    "help-ischeme-group", "hischemeg",
R"(Options that are used by ischeme are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeIschemeInterpreterInsiderOahGroup ();
}

ischemeInterpreterInsiderOahGroup::~ischemeInterpreterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void ischemeInterpreterInsiderOahGroup::initializeIschemeInterpreterInsiderOahGroup ()
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
void ischemeInterpreterInsiderOahGroup::printIschemeInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLogStream <<
    "The ischeme options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_ischemeInterpreterInsiderOahGroup createGlobalIschemeInterpreterInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global ischeme insider OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalIschemeInterpreterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalIschemeInterpreterInsiderOahGroup =
      ischemeInterpreterInsiderOahGroup::create ();
    assert (gGlobalIschemeInterpreterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalIschemeInterpreterInsiderOahGroup;
}


}
