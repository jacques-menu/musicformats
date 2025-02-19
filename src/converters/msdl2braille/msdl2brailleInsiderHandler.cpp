/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "bsrInitialization.h"

#include "oahOah.h"
#include "waeOah.h"
#include "mfOutputFileOah.h"
#include "oahDisplayOah.h"
#include "oahHarmoniesExtraOah.h"

#include "msdl2msrOah.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"

#include "msdl2brailleComponent.h"

#include "msdl2brailleInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2brailleInsiderHandler msdl2brailleInsiderHandler::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  // create the insider handler
  msdl2brailleInsiderHandler* obj = new
    msdl2brailleInsiderHandler (
      serviceName,
      handlerHeader);
  assert (obj!=0);

  return obj;
}

msdl2brailleInsiderHandler::msdl2brailleInsiderHandler (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahInsiderHandler (
      serviceName,
      handlerHeader,
R"(
         Welcome to the MSDL  to Braille converter
          delivered as part of the MusicFormats library.

      --- https://github.com/jacques-menu/musicformats ---
)",
R"(
Usage: msdl2braille [option]* [MSDLFile] [option]*
)")
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Initializing msdl2braille insider options handler \"" <<
      fHandlerHeader <<
      "\"";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // initialize the history
  initializeHandlerMultiComponent ();

// create the msdl2braille prefixes
  createTheMsdl2brlPrefixes ();

  // create the msdl2braille option groups
  createTheMsdl2brlOptionGroups (serviceName);
}

msdl2brailleInsiderHandler::~msdl2brailleInsiderHandler ()
{}

void msdl2brailleInsiderHandler::initializeHandlerMultiComponent ()
{
  fHandlerMultiComponent =
    createMsdl2brailleConverterComponent ();
}

//_______________________________________________________________________________
std::string msdl2brailleInsiderHandler::handlerServiceAboutInformation () const
{
  return
R"(What msdl2braille does:

    This multi-pass converter basically performs 6 passes:
        Pass 1:  reads the contents of MSDLFile or stdin ('-')
                 and converts it to a first Music Score Representation (MSR);
        Pass 2:  converts that first MSR into a second MSR;
                 to get a full MSR;
        Pass 3:  converts the second MSR into a first
                 Braille Score Representation (BSR);
                 containing one Braille page per MusicXML page;
        Pass 4:  converts the first BSR into a second BSR
                 with as many Braille pages as needed
                 to fit the line and page lengths;
        Pass 5:  converts the second BSR to Braille text
                 and writes it to standard output.

    In this preliminary version, pass 2 merely clones the BSR it receives.

    Other passes are performed according to the options, such as
    displaying views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::createTheMsdl2brlPrefixes ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the msdl2braille prefixes";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::createTheMsdl2brlOptionGroups (
  const std::string& serviceName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating the msdl2braille insider option groups" <<
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
  initializeBSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the msdl2braille OAH group
  appendGroupToHandler (
    createGlobalMsdl2brlOahGroup (
      serviceName,
      fHandlerHeader));

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2bsr OAH group
  appendGroupToHandler (
    createGlobalMsr2bsrOahGroup ());

  // create the BSR OAH group
  appendGroupToHandler (
    createGlobalBsrOahGroup ());

  // create the braille generation OAH group
  appendGroupToHandler (
    createGlobalBrailleGenerationOahGroup ());

  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
    // print the options handler initial state
    std::stringstream ss;

    ss <<
      "msdl2brailleInsiderHandler has been initialized as:" <<
      std::endl;

    ++gIndenter;

    print (gLog);

    gLog <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::checkOptionsAndArguments () const
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

//______________________________________________________________________________
std::string msdl2brailleInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
  return ""; // JMI
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::enforceHandlerQuietness ()
{
#ifdef MF_TRACE_IS_ENABLED
  gTraceOahGroup->
    enforceGroupQuietness ();
#endif // MF_TRACE_IS_ENABLED

  gGlobalMsdl2brlInsiderOahGroup->
    enforceGroupQuietness ();

  gMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2bsrOahGroup->
    enforceGroupQuietness ();

  gBsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBrailleGenerationOahGroup->
    enforceGroupQuietness ();

  gHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void msdl2brailleInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msdl2brailleInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2brailleInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2brailleInsiderOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brailleInsiderOahGroup>*> (v)) {
        S_msdl2brailleInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2brailleInsiderOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdl2brailleInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2brailleInsiderOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdl2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brailleInsiderOahGroup>*> (v)) {
        S_msdl2brailleInsiderOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msdl2brailleInsiderOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdl2brailleInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msdl2brailleInsiderOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::print (std::ostream& os) const
{
  constexpr int fieldWidth = 27;

  os <<
    "msdl2brailleInsiderHandler:" <<
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

    std::list <S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msdl2brailleInsiderHandler& elt)
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
S_msdl2brailleInsiderOahGroup gGlobalMsdl2brlInsiderOahGroup;

S_msdl2brailleInsiderOahGroup msdl2brailleInsiderOahGroup::create (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
  msdl2brailleInsiderOahGroup* obj = new msdl2brailleInsiderOahGroup (
    serviceName,
    handlerHeader);
  assert (obj!=0);

  return obj;
}

msdl2brailleInsiderOahGroup::msdl2brailleInsiderOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader)
  : oahGroup (
      serviceName,
      "hx2b", "help-msdl2braille",
R"(Options that are used by msdl2braille are grouped here.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeMsdl2brlInsiderOahGroup ();
}

msdl2brailleInsiderOahGroup::~msdl2brailleInsiderOahGroup ()
{}

//_______________________________________________________________________________
void msdl2brailleInsiderOahGroup::initializeMsdl2brlInsiderOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  gLog << std::left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

void msdl2brailleInsiderOahGroup::createInsiderQuitSubGroup ()
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
        "help-msdl2braille-quit", "hmsdl2braillequit",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // quit after pass 1

  S_oahBooleanAtom
    quit1OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-1", "qap1",
R"(Quit after pass 1, i.e. after creation of the first MSR.)",
        "fQuitAfterPass1",
        fQuitAfterPass1);

  // quit after pass 2

  S_oahBooleanAtom
    quit2OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-2", "qap2",
R"(Quit after pass 2, i.e. after creation of the second MSR.)",
        "fQuitAfterPass2",
        fQuitAfterPass2);

  subGroup->
    appendAtomToSubGroup (
      quit2OahBooleanAtom);

  // quit after pass 3

  S_oahBooleanAtom
    quit2bOahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-3", "qap3",
R"(Quit after pass 3, i.e. after creation of the first BSR.)",
        "fQuitAfterPass3",
        fQuitAfterPass3);

  subGroup->
    appendAtomToSubGroup (
      quit2bOahBooleanAtom);

  // quit after pass 4

  S_oahBooleanAtom
    quit4OahBooleanAtom =
      oahBooleanAtom::create (
        "quit-after-pass-4", "qap4",
R"(Quit after pass 4, i.e. after creation of the second BSR.)",
        "fQuitAfterPass4",
        fQuitAfterPass4);

  subGroup->
    appendAtomToSubGroup (
      quit4OahBooleanAtom);
}

//______________________________________________________________________________
void msdl2brailleInsiderOahGroup::printMsdl2brlInsiderOahGroupValues (
  int fieldWidth)
{
  gLog <<
    "The msdl2braille options are:" <<
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
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass2" << ": " <<
    fQuitAfterPass2 <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass3" << ": " <<
    fQuitAfterPass3 <<
    std::endl <<
    std::setw (fieldWidth) << "fQuitAfterPass4" << ": " <<
    fQuitAfterPass4 <<
    std::endl;

  --gIndenter;


  --gIndenter;
}

//______________________________________________________________________________
S_msdl2brailleInsiderOahGroup createGlobalMsdl2brlOahGroup (
  const std::string& serviceName,
  const std::string& handlerHeader)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msdl2braille OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsdl2brlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMsdl2brlInsiderOahGroup =
      msdl2brailleInsiderOahGroup::create (
        serviceName,
        handlerHeader);
    assert (gGlobalMsdl2brlInsiderOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsdl2brlInsiderOahGroup;
}


}

