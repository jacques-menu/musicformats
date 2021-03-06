/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...
#include <regex>

#include "enableHarmoniesExtraOahIfDesired.h"
#ifdef EXTRA_OAH_IS_ENABLED
  #include "harmoniesExtraOah.h"
#endif

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msr.h"
#include "bsr.h"

#include "oahOah.h"
#include "waeOah.h"
#include "outputFileOah.h"
#include "displayOah.h"

#include "msdl2msrOah.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "oahEarlyOptions.h"

#include "msdl2brailleComponent.h"

#include "msdl2brailleInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msdl2brailleInsiderHandler msdl2brailleInsiderHandler::create (
  const string& serviceName,
  const string& handlerHeader)
{
  // create the insider handler
  msdl2brailleInsiderHandler* o = new
    msdl2brailleInsiderHandler (
      serviceName,
      handlerHeader);
  assert (o!=0);

  return o;
}

msdl2brailleInsiderHandler::msdl2brailleInsiderHandler (
  const string& serviceName,
  const string& handlerHeader)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Initializing msdl2braille insider options handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

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
string msdl2brailleInsiderHandler::handlerServiceAboutInformation () const
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
                 to fit the line and page lengthes;
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msdl2braille prefixes" <<
      endl;
  }
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::createTheMsdl2brlOptionGroups (
  const string& serviceName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating the msdl2braille insider option groups" <<
      fHandlerHeader <<
      "\"" <<
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

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalHarmoniesExtraOahGroup ());
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOahDetails ()) {
    // print the options handler initial state
    gLogStream <<
      "msdl2brailleInsiderHandler has been initialized as:" <<
      endl;

    ++gIndenter;

    print (gLogStream);

    gLogStream <<
      endl;

    --gIndenter;
  }
#endif
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::checkOptionsAndArguments () const
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

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
string msdl2brailleInsiderHandler::fetchOutputFileNameFromTheOptions () const
{
  return ""; // JMI
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTracingOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMsdl2brlInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2bsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBrailleGenerationOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalHarmoniesExtraOahGroup->
    enforceGroupQuietness ();
#endif
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brailleInsiderOahGroup>*> (v)) {
        S_msdl2brailleInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brailleInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2brailleInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2brailleInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2brailleInsiderOahGroup>*> (v)) {
        S_msdl2brailleInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2brailleInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2brailleInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2brailleInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void msdl2brailleInsiderHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "msdl2brailleInsiderHandler:" <<
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

ostream& operator<< (ostream& os, const S_msdl2brailleInsiderHandler& elt)
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
S_msdl2brailleInsiderOahGroup gGlobalMsdl2brlInsiderOahGroup;

S_msdl2brailleInsiderOahGroup msdl2brailleInsiderOahGroup::create (
  const string& serviceName,
  const string& handlerHeader)
{
  msdl2brailleInsiderOahGroup* o = new msdl2brailleInsiderOahGroup (
    serviceName,
    handlerHeader);
  assert (o!=0);

  return o;
}

msdl2brailleInsiderOahGroup::msdl2brailleInsiderOahGroup (
  const string& serviceName,
  const string& handlerHeader)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    endl;
  }
#endif

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

void msdl2brailleInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << left <<
    "Creating insider quit subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
  }
#endif

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
    quit2aOahBooleanAtom =
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
  gLogStream <<
    "The msdl2braille options are:" <<
    endl;

  ++gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fQuitAfterPass1" << " : " <<
    fQuitAfterPass1 <<
    endl <<
    setw (fieldWidth) << "fQuitAfterPass2" << " : " <<
    fQuitAfterPass2 <<
    endl <<
    setw (fieldWidth) << "fQuitAfterPass3" << " : " <<
    fQuitAfterPass3 <<
    endl <<
    setw (fieldWidth) << "fQuitAfterPass4" << " : " <<
    fQuitAfterPass4 <<
    endl;

  --gIndenter;


  --gIndenter;
}

//______________________________________________________________________________
S_msdl2brailleInsiderOahGroup createGlobalMsdl2brlOahGroup (
  const string& serviceName,
  const string& handlerHeader)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msdl2braille OAH group" <<
      endl;
  }
#endif

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

