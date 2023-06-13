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
#include "mfFindInterpreterComponent.h"

#include "mfFindInterpreterInterface.h"

#include "mfFindInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_mfFindInterpreterInsiderHandler mfFindInterpreterInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  mfFindInterpreterInsiderHandler* obj = new
    mfFindInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj != nullptr);

  return obj;
}

mfFindInterpreterInsiderHandler::mfFindInterpreterInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
                   Welcome to the mfFind interpreter,
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

  // create the mfFind prefixes
  createThemfFindInterpreterPrefixes ();

  // create the mfFind option groups
  createThemfFindInterpreterOptionGroups (
    serviceName);
}

mfFindInterpreterInsiderHandler::~mfFindInterpreterInsiderHandler ()
{}

void mfFindInterpreterInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createmfFindInterpreterComponent ();
}

std::string mfFindInterpreterInsiderHandler::usageInformation ()
{
  stringstream s;

  s <<
R"(Usage: mfFind [option]*
)" <<
    endl;

  return s.str ();
}

std::string mfFindInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    mfFindInterpreterAboutInformation ();
}

std::string mfFindInterpreterInsiderHandler::mfFindInterpreterAboutInformation () const
{
  std::string result =
R"(What mfFind does:

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
void mfFindInterpreterInsiderHandler::createThemfFindInterpreterPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the mfFind prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void mfFindInterpreterInsiderHandler::createThemfFindInterpreterOptionGroups (
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

  // create the mfFind OAH group
  appendGroupToHandler (
    createGlobalmfFindInterpreterOahGroup ());

  // create the mfFind insider OAH group
  appendGroupToHandler (
    createGlobalmfFindInterpreterInsiderOahGroup ());
}

//______________________________________________________________________________
void mfFindInterpreterInsiderHandler::checkOptionsAndArguments () const
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
void mfFindInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void mfFindInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalmfFindInterpreterInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void mfFindInterpreterInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void mfFindInterpreterInsiderOahGroup::checkGroupOptionsConsistency ()
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
      "mfFindInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void mfFindInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mfFindInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfFindInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfFindInterpreterInsiderOahGroup>*> (v)) {
        S_mfFindInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mfFindInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mfFindInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mfFindInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfFindInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_mfFindInterpreterInsiderOahGroup>*> (v)) {
        S_mfFindInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mfFindInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mfFindInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mfFindInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void mfFindInterpreterInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "mfFindInterpreterInsiderHandler:" <<
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

std::ostream& operator<< (std::ostream& os, const S_mfFindInterpreterInsiderHandler& elt)
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
S_mfFindInterpreterInsiderOahGroup gGlobalmfFindInterpreterInsiderOahGroup;

S_mfFindInterpreterInsiderOahGroup mfFindInterpreterInsiderOahGroup::create ()
{
  mfFindInterpreterInsiderOahGroup* obj = new mfFindInterpreterInsiderOahGroup ();
  assert (obj != nullptr);

  return obj;
}

mfFindInterpreterInsiderOahGroup::mfFindInterpreterInsiderOahGroup ()
  : oahGroup (
    "mfFind",
    "help-mfFind-group", "hmfFindg",
R"(Options that are used by mfFind are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializemfFindInterpreterInsiderOahGroup ();
}

mfFindInterpreterInsiderOahGroup::~mfFindInterpreterInsiderOahGroup ()
{}

//_______________________________________________________________________________
void mfFindInterpreterInsiderOahGroup::initializemfFindInterpreterInsiderOahGroup ()
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
void mfFindInterpreterInsiderOahGroup::printmfFindInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The mfFind options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_mfFindInterpreterInsiderOahGroup createGlobalmfFindInterpreterInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global mfFind insider OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalmfFindInterpreterInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalmfFindInterpreterInsiderOahGroup =
      mfFindInterpreterInsiderOahGroup::create ();
    assert (gGlobalmfFindInterpreterInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalmfFindInterpreterInsiderOahGroup;
}


}
