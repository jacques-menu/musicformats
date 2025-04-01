/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "oahOah.h"

#include "mnx2mnxsrWae.h"

#include "mnx2mnxsrInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "mnx2mnxsrInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________

S_mnx2mnxsrInterpreterOahGroup gGlobalMnx2mnxsrInterpreterOahGroup;

S_mnx2mnxsrInterpreterOahGroup mnx2mnxsrInterpreterOahGroup::create ()
{
  mnx2mnxsrInterpreterOahGroup* obj = new mnx2mnxsrInterpreterOahGroup ();
  assert (obj != nullptr);
  return obj;
}

mnx2mnxsrInterpreterOahGroup::mnx2mnxsrInterpreterOahGroup ()
  : oahGroup (
      "mnx2mnxsr",
      "help-mnx2mnxsr-interpreter-group", "hmi-group",
R"(These options control the way MNX2MNXSR interpreter works.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  // initialize it
  initializeMnx2mnxsrInterpreterOahGroup ();
}

mnx2mnxsrInterpreterOahGroup::~mnx2mnxsrInterpreterOahGroup ()
{}

void mnx2mnxsrInterpreterOahGroup::initializeMnx2mnxsrInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializemnx2mnxsrInterpretertracingOah ();
#endif // MF_TRACE_IS_ENABLED

  // user options
  // --------------------------------------
  initializeMnx2mnxsrOptions ();

  // maintaince options
  // --------------------------------------
//   initializeMnx2mnxsrUserOptions (); JMI
}
//     void                  initializeMnx2mnxsrUserOptions ();
//
//     void                  initializeMnx2mnxsrMaintainanceOptions ();

void mnx2mnxsrInterpreterOahGroup::initializeMnx2mnxsrOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MNX2MNXSR",
        "help-mnx2mnxsr", "hmnx2mnxsr",
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
R"(Write a trace of the MNX2MNXSR tokens to standard error.)",
        "fDisplayTokens",
        fDisplayTokens));

  fDisplayServiceAndInputAtom =
    oahBooleanAtom::create (
      "display-service-and-input", "dservandinput",
R"(Write MNX2MNXSR service and input analysis activity to standard output.)",
      "fDisplayServiceAndInput",
      fDisplayServiceAndInput);

  subGroup->
    appendAtomToSubGroup (
      fDisplayServiceAndInputAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options", "dopts",
R"(Write MNX2MNXSR options analysis activity to standard output.)",
        "fDisplayOptions",
        fDisplayOptions));

  // no launch

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "no-launch", "nol",
R"(Analyze the MNX2MNXSR script, but don't launch the service actually.
This is useful to check the options gathered by the MNX2MNXSR interpreter,
and what command(s) would be launched.
This option implies the '-display-service-and-input, -dservandinput' option.)",
        "fNoLaunch",
        fNoLaunch,
        fDisplayServiceAndInputAtom));

  // input

  fInputSourcesSetAtom =
    oahStringSetElementAtom::create (
      "input", "",
R"(Use INPUT_SOURCE_NAME as input to launch the service.
This option overrides the 'input' value(s) specified in the script.
There can be several occurrences of this option,
in which case the service will be run several times.)",
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
The service will be run using the corresponding options block(s).
There can be several occurrences of this option,
in which case the service will be run several times.
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
R"(Write a trace of the MNX2MNXSR scanning by Flex-generated code to standard error.)",
        "fTraceScanning",
        fTraceScanning));

  // trace scanning is done by the Bison-generated code
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-parsing", "tparse",
R"(Write a trace of the MNX2MNXSR parsing by Bison-generated code to standard error.)",
        "fTraceParsing",
        fTraceParsing));

  // trace inputs
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-inputs", "tinputs",
R"(Write MNX2MNXSR case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-input-statements", "tinputsstats",
R"(Write MNX2MNXSR case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  // trace choices
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choices", "tchoices",
R"(Write MNX2MNXSR choice analysis activity to standard output.)",
        "fTraceChoices",
        fTraceChoices));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choice-statements", "tchoicesstats",
R"(Write MNX2MNXSR choice statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-choice-statements", "tcasechoices",
R"(Write MNX2MNXSR case statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  // trace options blocks
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-options-blocks", "toblocks",
R"(Write MNX2MNXSR options blocks analysis activity to standard output.)",
        "fTraceOptionsBlocks",
        fTraceOptionsBlocks));
}

void mnx2mnxsrInterpreterOahGroup::enforceGroupQuietness ()
{}

void mnx2mnxsrInterpreterOahGroup::checkGroupOptionsConsistency ()
{}

void mnx2mnxsrInterpreterOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mnx2mnxsrInterpreterOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mnx2mnxsrInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mnx2mnxsrInterpreterOahGroup>*> (v)) {
        S_mnx2mnxsrInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mnx2mnxsrInterpreterOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mnx2mnxsrInterpreterOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mnx2mnxsrInterpreterOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mnx2mnxsrInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mnx2mnxsrInterpreterOahGroup>*> (v)) {
        S_mnx2mnxsrInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching mnx2mnxsrInterpreterOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mnx2mnxsrInterpreterOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> mnx2mnxsrInterpreterOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void mnx2mnxsrInterpreterOahGroup::printMnx2mnxsrInterpreterOahValues (
  int fieldWidth)
{
  gLog <<
    "The mnx2mnxsr options are:" <<
    endl;

  ++gIndenter;

  // tracing
  // --------------------------------------

  gLog <<
    "Trace:" <<
    endl;

  ++gIndenter;

  gLog << left <<
    setw (fieldWidth) << "fTraceScanning" << ": " <<
      fTraceScanning <<
      endl <<
    setw (fieldWidth) << "fDisplayTokens" << ": " <<
      fDisplayTokens <<
      endl <<

    setw (fieldWidth) << "fTraceParsing" << ": " <<
      fTraceParsing <<
      endl <<

    setw (fieldWidth) << "fDisplayServiceAndInput" << ": " <<
      fDisplayServiceAndInput <<
      endl <<

    setw (fieldWidth) << "fDisplayOptions" << ": " <<
      fDisplayOptions <<
      endl <<

    setw (fieldWidth) << "fTraceInputs" << ": " <<
      fTraceInputs <<
      endl <<
    setw (fieldWidth) << "fTraceCaseInputStatements" << ": " <<
      fTraceCaseInputStatements <<
      endl <<

    setw (fieldWidth) << "fTraceChoices" << ": " <<
      fTraceChoices <<
      endl <<
    setw (fieldWidth) << "fTraceCaseChoiceStatements" << ": " <<
      fTraceCaseChoiceStatements <<
      endl <<

    setw (fieldWidth) << "fTraceCaseChoiceStatements" << ": " <<
      fTraceCaseChoiceStatements <<
      endl <<
    setw (fieldWidth) << "fTraceCaseInputStatements" << ": " <<
      fTraceCaseInputStatements <<
      endl <<
    setw (fieldWidth) << "fNoLaunch" << ": " <<
      fNoLaunch <<
      endl <<

    setw (fieldWidth) << "fTraceOptionsBlocks" << ": " <<
      fTraceOptionsBlocks <<
      endl;

  --gIndenter;

  // choice
  // --------------------------------------

  gLog <<
    "Choice:" <<
    endl;

  ++gIndenter;

  gLog << left <<
    setw (fieldWidth) << "fSelectChoiceToLabelsMultiMap" << " : ";

  ++gIndenter;

  if (fSelectChoiceToLabelsMultiMap.size ()) {
    for (pair<std::string, std::string> thePair : fSelectChoiceToLabelsMultiMap) {
      gLog <<
        thePair.first << ": " << thePair.second <<
        endl;
    } // for
  }

  --gIndenter;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator<< (std::ostream& os, const S_mnx2mnxsrInterpreterOahGroup& elt)
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
S_mnx2mnxsrInterpreterOahGroup createGlobalMnx2mnxsrInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global mnx2mnxsr OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMnx2mnxsrInterpreterOahGroup) {
    // create the mnx2mnxsr options
    // ------------------------------------------------------

    gGlobalMnx2mnxsrInterpreterOahGroup =
      mnx2mnxsrInterpreterOahGroup::create ();
    assert (gGlobalMnx2mnxsrInterpreterOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMnx2mnxsrInterpreterOahGroup;
}


}
