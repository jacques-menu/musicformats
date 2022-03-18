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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "mfslInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "mfslInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________

S_mfslInterpreterOahGroup gGlobalMfslInterpreterOahGroup;

S_mfslInterpreterOahGroup mfslInterpreterOahGroup::create ()
{
  mfslInterpreterOahGroup* o = new mfslInterpreterOahGroup ();
  assert (o != nullptr);
  return o;
}

mfslInterpreterOahGroup::mfslInterpreterOahGroup ()
  : oahGroup (
    "mfslInterpreter",
     "help-mfsl-interpreter-group", "hmi-group",
R"(These options control the way MFSL interpreter works.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  // initialize it
  initializeMfslInterpreterOahGroup ();
}

mfslInterpreterOahGroup::~mfslInterpreterOahGroup ()
{}

void mfslInterpreterOahGroup::initializeMfslInterpreterOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializemfslInterpretertracingOah ();
#endif

  // midi
  // --------------------------------------
  initializeMfslOptions ();
}

void mfslInterpreterOahGroup::initializeMfslOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MFSL",
        "help-mfsl", "hmfsl",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // verbose mode
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "verbose-mode", "verbose",
R"(Don't execute the MFSL input, merely display the tokens it contains.)",
        "fVerboseMode",
        fVerboseMode));

  // trace
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-scanning", "tscan",
R"(Write a trace of the MFSL scanning activity to standard error.)",
        "fTraceScanning",
        fTraceScanning));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-parsing", "tparse",
R"(Write a trace of the MFSL parsing activity to standard error.)",
        "fTraceParsing",
        fTraceParsing));

  // display
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-tokens", "dtoks",
R"(Display the MFSL tokens after they’re accepted.)",
        "fDisplayTokens",
        fDisplayTokens));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-non-terminals", "dnterms",
R"(Display the MFSL non-terminals after they’re accepted.)",
        "fDisplayNonTerminals",
        fDisplayNonTerminals));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-semantics", "tsem",
R"(Write MFSL semantics analysis activity to standard output.)",
        "fTraceSemantics",
        fTraceSemantics));

  // setting choices
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      msrRenamePartAtom::create (
        "set-choice", "set",
        regex_replace (
R"(Set the value of CHOICE_NAME to VALUE.
SET_CHOICE_SPEC should be of the form ORIGINAL_NAME:VALUE .
There can be spaces around the ':'.
There can be several occurrences of this option.)",
          regex ("EXECUTABLE"),
          gGlobalOahOahGroup->getOahOahGroupServiceName ()),
        "SET_CHOICE_SPEC",
        "fSetChoiceToValueMap",
        fSetChoiceToValueMap));
}

void mfslInterpreterOahGroup::enforceGroupQuietness ()
{}

void mfslInterpreterOahGroup::checkGroupOptionsConsistency ()
{}

void mfslInterpreterOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mfslInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterOahGroup>*> (v)) {
        S_mfslInterpreterOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mfslInterpreterOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mfslInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterOahGroup>*> (v)) {
        S_mfslInterpreterOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mfslInterpreterOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mfslInterpreterOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mfslInterpreterOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void mfslInterpreterOahGroup::printMfslInterpreterOahValues (
  int fieldWidth)
{
  gLogStream <<
    "The mfslInterpreter options are:" <<
    endl;

  ++gIndenter;

  // verbose mode
  // --------------------------------------

  gLogStream <<
    "VerboseMode:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fVerboseMode" << " : " <<
      fVerboseMode <<
      endl;

  --gIndenter;

  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fTraceScanning" << " : " <<
      fTraceScanning <<
      endl <<
    setw (fieldWidth) << "fTraceParsing" << " : " <<
      fTraceParsing <<
      endl;

  --gIndenter;

  // display
  // --------------------------------------

  gLogStream <<
    "Display:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fDisplayTokens" << " : " <<
      fDisplayTokens <<
      endl <<
    setw (fieldWidth) << "fDisplayNonTerminals" << " : " <<
      fDisplayNonTerminals <<
      endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_mfslInterpreterOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mfslInterpreterOahGroup createGlobalMfslInterpreterOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global mfslInterpreter OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMfslInterpreterOahGroup) {
    // create the mfslInterpreter options
    // ------------------------------------------------------

    gGlobalMfslInterpreterOahGroup =
      mfslInterpreterOahGroup::create ();
    assert (gGlobalMfslInterpreterOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMfslInterpreterOahGroup;
}


}
