/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "mfPreprocessorSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

// early options
#include "oahEarlyOptions.h"

// OAH
#include "oahOah.h"
#include "waeOah.h"
#include "oahDisplayOah.h"

// WAE
#include "waeHandlers.h"
#include "oahWae.h"

// components
#include "ischemeInterpreterComponent.h"

#include "ischemeInterpreterInterface.h"

#include "ischemeInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_ischemeInterpreterInsiderHandler ischemeInterpreterInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  ischemeInterpreterInsiderHandler* obj = new
    ischemeInterpreterInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj != nullptr);

  return obj;
}

ischemeInterpreterInsiderHandler::ischemeInterpreterInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
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

std::string ischemeInterpreterInsiderHandler::usageInformation ()
{
  std::stringstream ss;

  ss <<
R"(Usage: ischeme [option]*
)" <<
    endl;

  return ss.str ();
}

std::string ischemeInterpreterInsiderHandler::handlerServiceAboutInformation () const
{
  return
    ischemeInterpreterAboutInformation ();
}

std::string ischemeInterpreterInsiderHandler::ischemeInterpreterAboutInformation () const
{
  std::string result =
R"(What ischeme does:

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
void ischemeInterpreterInsiderHandler::createTheIschemeInterpreterPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating the ischeme prefixes in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void ischemeInterpreterInsiderHandler::createTheIschemeInterpreterOptionGroups (
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
void ischemeInterpreterInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void ischemeInterpreterInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

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
    std::stringstream ss;

    ss <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    std::stringstream ss;

    ss <<
      "ischemeInterpreterInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

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
void ischemeInterpreterInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> ischemeInterpreterInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_ischemeInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterInsiderOahGroup>*> (v)) {
        S_ischemeInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching ischemeInterpreterInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void ischemeInterpreterInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> ischemeInterpreterInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_ischemeInterpreterInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterInsiderOahGroup>*> (v)) {
        S_ischemeInterpreterInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching ischemeInterpreterInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void ischemeInterpreterInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> ischemeInterpreterInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void ischemeInterpreterInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

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

std::ostream& operator<< (std::ostream& os, const S_ischemeInterpreterInsiderHandler& elt)
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
  ischemeInterpreterInsiderOahGroup* obj = new ischemeInterpreterInsiderOahGroup ();
  assert (obj != nullptr);

  return obj;
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
void ischemeInterpreterInsiderOahGroup::printIschemeInterpreterInsiderOahGroupValues (int fieldWidth)
{
  gLog <<
    "The ischeme options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_ischemeInterpreterInsiderOahGroup createGlobalIschemeInterpreterInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global ischeme insider OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

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
