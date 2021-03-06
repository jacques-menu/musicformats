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

#include "mfslWae.h"

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
    "mfsl",
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

  // user options
  // --------------------------------------
  initializeMfslOptions ();

  // maintaince options
  // --------------------------------------
//   initializeMfslUserOptions (); JMI
}
//     void                  initializeMfslUserOptions ();
//
//     void                  initializeMfslMaintenanceOptions ();

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

  // display
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-tokens", "dtoks",
R"(Write a trace of the MFSL tokens to standard error.)",
        "fDisplayTokens",
        fDisplayTokens));

  fDisplayToolAndInputAtom =
    oahBooleanAtom::create (
      "display-tool-and-input", "dtai",
R"(Write MFSL tool and input analysis activity to standard output.)",
      "fDisplayToolAndInput",
      fDisplayToolAndInput);

  subGroup->
    appendAtomToSubGroup (
      fDisplayToolAndInputAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options", "dopts",
R"(Write MFSL options analysis activity to standard output.)",
        "fDisplayOptions",
        fDisplayOptions));

  // no launch

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "no-launch", "nol",
R"(Analyze the MFSL script, but don't launch the tool actually.
This is useful to check the options gathered by the MFSL interpreter,
and what command(s) would be launched.
This option implies the '-display-tool-and-input, -ttai' option.)",
        "fNoLaunch",
        fNoLaunch,
        fDisplayToolAndInputAtom));

  // input

  fInputSourcesSetAtom =
    oahStringSetElementAtom::create (
      "input", "",
R"(Use INPUT_SOURCE_NAME as input to launch the tool.
This option overrides the 'input' value(s) specified in the script.
There can be several occurrences of this option,
in which case the tool will be run several times.)",
      "INPUT_SOURCE_NAME",
      "fInputSourcesSet",
      fInputSourcesSet);

  fInputSourcesSetAtom->
      setMultipleOccurrencesAllowed ();

  subGroup->
    appendAtomToSubGroup (
      fInputSourcesSetAtom);

  // select

  fSelectChoiceToLabelsMultiMapAtom =
    oahStringToStringMultiMapElementAtom::create (
      "select", "sel",
R"(Select LABEL for choice CHOICE.
The tool will be run using the corresponding options block(s).
There can be several occurrences of this option,
in which case the tool will be run several times.
The LABEL can be 'ALL', in which case
all the labels of the given choice are selected.)",
      "CHOICE:LABEL",
      "fSelectChoiceToLabelsMultiMap",
      fSelectChoiceToLabelsMultiMap);

  fSelectChoiceToLabelsMultiMapAtom->
      setMultipleOccurrencesAllowed ();

  subGroup->
    appendAtomToSubGroup (
      fSelectChoiceToLabelsMultiMapAtom);

  // maintainance options
  // --------------------------------------

  // trace scanning is done by the Flex-generated code
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-scanning", "tscan",
R"(Write a trace of the MFSL scanning by Flex-generated code to standard error.)",
        "fTraceScanning",
        fTraceScanning));

  // trace scanning is done by the Bison-generated code
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-parsing", "tparse",
R"(Write a trace of the MFSL parsing by Bison-generated code to standard error.)",
        "fTraceParsing",
        fTraceParsing));

  // trace inputs
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-inputs", "tinputs",
R"(Write MFSL case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-input-statements", "tinputsstats",
R"(Write MFSL case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  // trace choices
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choices", "tchoices",
R"(Write MFSL choice analysis activity to standard output.)",
        "fTraceChoices",
        fTraceChoices));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choice-statements", "tchoicesstats",
R"(Write MFSL choice statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-choice-statements", "tcasechoices",
R"(Write MFSL case statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  // trace options blocks
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-options-blocks", "toblocks",
R"(Write MFSL options blocks analysis activity to standard output.)",
        "fTraceOptionsBlocks",
        fTraceOptionsBlocks));
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
    "The mfsl options are:" <<
    endl;

  ++gIndenter;

  // tracing
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fTraceScanning" << " : " <<
      fTraceScanning <<
      endl <<
    setw (fieldWidth) << "fDisplayTokens" << " : " <<
      fDisplayTokens <<
      endl <<

    setw (fieldWidth) << "fTraceParsing" << " : " <<
      fTraceParsing <<
      endl <<

    setw (fieldWidth) << "fDisplayToolAndInput" << " : " <<
      fDisplayToolAndInput <<
      endl <<

    setw (fieldWidth) << "fDisplayOptions" << " : " <<
      fDisplayOptions <<
      endl <<

    setw (fieldWidth) << "fTraceInputs" << " : " <<
      fTraceInputs <<
      endl <<
    setw (fieldWidth) << "fTraceCaseInputStatements" << " : " <<
      fTraceCaseInputStatements <<
      endl <<

    setw (fieldWidth) << "fTraceChoices" << " : " <<
      fTraceChoices <<
      endl <<
    setw (fieldWidth) << "fTraceCaseChoiceStatements" << " : " <<
      fTraceCaseChoiceStatements <<
      endl <<

    setw (fieldWidth) << "fTraceCaseChoiceStatements" << " : " <<
      fTraceCaseChoiceStatements <<
      endl <<
    setw (fieldWidth) << "fTraceCaseInputStatements" << " : " <<
      fTraceCaseInputStatements <<
      endl <<
    setw (fieldWidth) << "fNoLaunch" << " : " <<
      fNoLaunch <<
      endl <<

    setw (fieldWidth) << "fTraceOptionsBlocks" << " : " <<
      fTraceOptionsBlocks <<
      endl;

  --gIndenter;

  // choice
  // --------------------------------------

  gLogStream <<
    "Choice:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "fSelectChoiceToLabelsMultiMap" << " : ";

  ++gIndenter;

  if (fSelectChoiceToLabelsMultiMap.size ()) {
    for (pair<string, string> thePair : fSelectChoiceToLabelsMultiMap) {
      gLogStream <<
        thePair.first << " : " << thePair.second <<
        endl;
    } // for
  }

  --gIndenter;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_mfslInterpreterOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_mfslInterpreterOahGroup createGlobalMfslInterpreterOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global mfsl OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMfslInterpreterOahGroup) {
    // create the mfsl options
    // ------------------------------------------------------

    gGlobalMfslInterpreterOahGroup =
      mfslInterpreterOahGroup::create ();
    assert (gGlobalMfslInterpreterOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMfslInterpreterOahGroup;
}


}
