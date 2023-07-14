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

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfInitialization.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrInitialization.h"

#include "lpsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "mfOutputFileOah.h"
#include "oahDisplayOah.h"
#include "oahHarmoniesExtraOah.h"

#include "msdl2msrOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "oahWae.h"

#include "oahEarlyOptions.h"

#include "msdl2lilypondComponent.h"

#include "msdl2lilypondInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2lilypondInsiderHandler msdl2lilypondInsiderHandler::create (
  const std::string&      serviceName,
  const std::string&      handlerHeader,
  oahHandlerUsedThruKind  handlerUsedThruKind)
{
  // create the insider handler
  msdl2lilypondInsiderHandler* obj = new
    msdl2lilypondInsiderHandler (
      serviceName,
      handlerHeader,
      handlerUsedThruKind);
  assert (obj!=0);

  return obj;
}

msdl2lilypondInsiderHandler::msdl2lilypondInsiderHandler (
  const std::string&      serviceName,
  const std::string&      handlerHeader,
  oahHandlerUsedThruKind  handlerUsedThruKind)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
            Welcome to the MSDL to LilyPond converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
/* JMI
R"(
Usage: msdl2lilypond [option]* [MSDLFile] [option]*
)"
*/
      usageFromUsedThruKind (handlerUsedThruKind))
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing msdl2lilypond insider options handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize the history
  initializeHandlerMultiComponent ();

  // create the msdl2lilypond prefixes
  createTheMsdl2lyPrefixes ();

  // create the msdl2musicxml option groups
  createTheMsdl2lyOptionGroups (
    serviceName);
}

msdl2lilypondInsiderHandler::~msdl2lilypondInsiderHandler ()
{}

void msdl2lilypondInsiderHandler::initializeHandlerMultiComponent ()
{
  // create the component
  fHandlerMultiComponent =
    createMsdl2lilypondConverterComponent ();
}

std::string msdl2lilypondInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msdl2lilypond does:

    This multi-pass converter basically performs 5 passes:
        Pass 1: reads the contents of MusicXMLFile or stdin ('-')
                and converts it to a MusicXML tree;
        Pass 2: converts that MusicXML tree into
                a Music Score Representation (MSR) skeleton;
        Pass 2b: populates the MSR skeleton from the MusicXML tree
                to get a full MSR;
        Pass 3: converts the MSR into a
                LilyPond Score Representation (LPSR);
        Pass 4: converts the LPSR to LilyPond code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

std::string msdl2lilypondInsiderHandler::usageFromUsedThruKind (
   oahHandlerUsedThruKind handlerUsedThruKind) const
{
  std::string result;

  switch (handlerUsedThruKind) {
    case oahHandlerUsedThruKind::kHandlerUsedThruUnknown:
      {
        std::stringstream ss;

        ss <<
          "oahHandlerUsedThruKind::kHandlerUsedThruUnknown found in usageFromUsedThruKind() in handler \"" <<
          fHandlerHeader <<
          "\"";

        oahInternalError (ss.str ());
      }
      break;
    case oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv:
      result = fHandlerServiceName + " ([options] | [MSDLFile])+";
      break;
    case oahHandlerUsedThruKind::kHandlerUsedThruOptionsAndArguments:
      result = "Usage: [options]";
      break;
  } // switch

  result = "Usage: " + result;

  return result;
}

void msdl2lilypondInsiderHandler::createTheMsdl2lyPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the msdl2lilypond prefixes";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

void msdl2lilypondInsiderHandler::createTheMsdl2lyOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the msdl2lilypond insider option groups" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
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
  // create the trace OAH group
  appendGroupToHandler (
    createGlobalTraceOahGroup (
      this));
#endif // MF_TRACE_IS_ENABLED

  // create the output file OAH group
  appendGroupToHandler (
    createGlobalOutputFileOahGroup ());

  // create the display OAH group
  appendGroupToHandler (
    createGlobalDisplayOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();
  initializeLPSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msdl2lilypond OAH group
  appendGroupToHandler (
    createGlobalMsdl2lyInsiderOahGroup ());

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2lpsr OAH group
  appendGroupToHandler (
    createGlobalMsr2lpsrOahGroup ());

  // create the LPSR OAH group
  appendGroupToHandler (
    createGlobalLpsrOahGroup ());

  // create the LilyPond generation OAH group
  appendGroupToHandler (
    createGlobalLpsr2lilypondOahGroup ());

  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    // print the options handler initial state
    std::stringstream ss;

    ss <<
      "msdl2lilypondInsiderHandler has been initialized as:" <<
      std::endl;

    ++gIndenter;

    print (gLog);

    gLog <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    gLog <<
      "msdl2lilypondInsiderHandler help:" <<
      std::endl;

    this->printHelp (gOutput);
  }
#endif // MF_TRACE_IS_ENABLED
}

void msdl2lilypondInsiderHandler::checkOptionsAndArguments () const
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  checkSingleInputSourceInArgumentsVector ();
}

std::string msdl2lilypondInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
  return ""; // JMI
}

void msdl2lilypondInsiderHandler::checkHandlerOptionsConsistency ()
{}

void msdl2lilypondInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalMsdl2lyInsiderOahGroup->
    enforceGroupQuietness ();

  gMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2lpsrOahGroup->
    enforceGroupQuietness ();

  gLpsrOahGroup->
    enforceGroupQuietness ();

  gGlobalLpsr2lilypondOahGroup->
    enforceGroupQuietness ();

  gHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
}

void msdl2lilypondInsiderOahGroup::enforceGroupQuietness ()
{}

void msdl2lilypondInsiderOahGroup::checkGroupOptionsConsistency ()
{}

void msdl2lilypondInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondInsiderOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2lilypondInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lilypondInsiderOahGroup>*> (v)) {
        S_msdl2lilypondInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2lilypondInsiderOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdl2lilypondInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondInsiderOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2lilypondInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2lilypondInsiderOahGroup>*> (v)) {
        S_msdl2lilypondInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2lilypondInsiderOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdl2lilypondInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2lilypondInsiderOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

void msdl2lilypondInsiderHandler::print (std::ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msdl2lilypondInsiderHandler '" << fHandlerHeader << "':" <<
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

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdl2lilypondInsiderHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
S_msdl2lilypondInsiderOahGroup gGlobalMsdl2lyInsiderOahGroup;

S_msdl2lilypondInsiderOahGroup msdl2lilypondInsiderOahGroup::create ()
{
  msdl2lilypondInsiderOahGroup* obj = new msdl2lilypondInsiderOahGroup ();
  assert (obj!=0);

  return obj;
}

msdl2lilypondInsiderOahGroup::msdl2lilypondInsiderOahGroup ()
  : oahGroup (
      "msdl2lilypond",
      "hx2l", "help-msdl2lilypond",
R"(Options that are used by msdl2lilypond are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2lyInsiderOahGroup ();
}

msdl2lilypondInsiderOahGroup::~msdl2lilypondInsiderOahGroup ()
{}

void msdl2lilypondInsiderOahGroup::initializeMsdl2lyInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing \"" <<
      fGroupHeader <<
      "\" group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

void msdl2lilypondInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  gLog << std::left <<
    "Creating insider quit subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "help-msdl2lilypond-quit", "hmsdl2lilypondquit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 1

  S_oahBooleanAtom
    quit1OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-1", "qap1",
R"(Quit after pass 1, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "fQuitAfterPass1",
        fQuitAfterPass1);

  subGroup->
    appendAtomToSubGroup (
      quit1OahBooleanAtom);
}

void msdl2lilypondInsiderOahGroup::printMsdl2lyInsiderOahGroupValues (
  int fieldWidth)
{
  gLog <<
    "The msdl2lilypond options are:" <<
    std::endl;

  ++gIndenter;

  // quit after some passes
  // --------------------------------------

  gLog <<
    "Quit after some passes:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fQuitAfterPass1" << ": " <<
    fQuitAfterPass1 <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_msdl2lilypondInsiderOahGroup createGlobalMsdl2lyInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msdl2lilypond OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsdl2lyInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsdl2lyInsiderOahGroup =
      msdl2lilypondInsiderOahGroup::create ();
    assert (gGlobalMsdl2lyInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2lyInsiderOahGroup;
}


}
