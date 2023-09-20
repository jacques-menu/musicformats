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

#include "oahOah.h"

#include "stringFilterWae.h"

#include "stringFilterInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "stringFilterInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________

S_stringFilterInterpreterOahGroup gGlobalstringFilterInterpreterOahGroup;

S_stringFilterInterpreterOahGroup stringFilterInterpreterOahGroup::create ()
{
  stringFilterInterpreterOahGroup* obj = new stringFilterInterpreterOahGroup ();
  assert (obj != nullptr);
  return obj;
}

stringFilterInterpreterOahGroup::stringFilterInterpreterOahGroup ()
  : oahGroup (
      "stringFilter",
      "help-stringFilter-interpreter-group", "hmi-group",
R"(These options control the way stringFilter interpreter works.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  // initialize it
  initializestringFilterInterpreterOahGroup ();
}

stringFilterInterpreterOahGroup::~stringFilterInterpreterOahGroup ()
{}

void stringFilterInterpreterOahGroup::initializestringFilterInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializestringFilterInterpretertracingOah ();
#endif // MF_TRACE_IS_ENABLED

  // user options
  // --------------------------------------
  initializestringFilterOptions ();

  // maintaince options
  // --------------------------------------
//   initializestringFilterUserOptions (); JMI
}
//     void                  initializestringFilterUserOptions ();
//
//     void                  initializestringFilterMaintainanceOptions ();

void stringFilterInterpreterOahGroup::initializestringFilterOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "stringFilter",
        "help-stringFilter", "hstringFilter",
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
R"(Write a trace of the stringFilter tokens to standard error.)",
        "fDisplayTokens",
        fDisplayTokens));

  fDisplayServiceAndInputAtom =
    oahBooleanAtom::create (
      "display-service-and-input", "dservandinput",
R"(Write stringFilter service and input analysis activity to standard output.)",
      "fDisplayServiceAndInput",
      fDisplayServiceAndInput);

  subGroup->
    appendAtomToSubGroup (
      fDisplayServiceAndInputAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options", "dopts",
R"(Write stringFilter options analysis activity to standard output.)",
        "fDisplayOptions",
        fDisplayOptions));

  // no launch

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "no-launch", "nol",
R"(Analyze the stringFilter script, but don't launch the service actually.
This is useful to check the options gathered by the stringFilter interpreter,
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
R"(Write a trace of the stringFilter scanning by Flex-generated code to standard error.)",
        "fTraceScanning",
        fTraceScanning));

  // trace scanning is done by the Bison-generated code
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-parsing", "tparse",
R"(Write a trace of the stringFilter parsing by Bison-generated code to standard error.)",
        "fTraceParsing",
        fTraceParsing));

  // trace inputs
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-inputs", "tinputs",
R"(Write stringFilter case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-input-statements", "tinputsstats",
R"(Write stringFilter case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  // trace choices
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choices", "tchoices",
R"(Write stringFilter choice analysis activity to standard output.)",
        "fTraceChoices",
        fTraceChoices));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choice-statements", "tchoicesstats",
R"(Write stringFilter choice statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-choice-statements", "tcasechoices",
R"(Write stringFilter case statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  // trace options blocks
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-options-blocks", "toblocks",
R"(Write stringFilter options blocks analysis activity to standard output.)",
        "fTraceOptionsBlocks",
        fTraceOptionsBlocks));
}

void stringFilterInterpreterOahGroup::enforceGroupQuietness ()
{}

void stringFilterInterpreterOahGroup::checkGroupOptionsConsistency ()
{}

void stringFilterInterpreterOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringFilterInterpreterOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_stringFilterInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_stringFilterInterpreterOahGroup>*> (v)) {
        S_stringFilterInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching stringFilterInterpreterOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void stringFilterInterpreterOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringFilterInterpreterOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_stringFilterInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_stringFilterInterpreterOahGroup>*> (v)) {
        S_stringFilterInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching stringFilterInterpreterOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void stringFilterInterpreterOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> stringFilterInterpreterOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void stringFilterInterpreterOahGroup::printstringFilterInterpreterOahValues (
  int fieldWidth)
{
  gLog <<
    "The stringFilter options are:" <<
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

std::ostream& operator<< (std::ostream& os, const S_stringFilterInterpreterOahGroup& elt)
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
S_stringFilterInterpreterOahGroup createGlobalstringFilterInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global stringFilter OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalstringFilterInterpreterOahGroup) {
    // create the stringFilter options
    // ------------------------------------------------------

    gGlobalstringFilterInterpreterOahGroup =
      stringFilterInterpreterOahGroup::create ();
    assert (gGlobalstringFilterInterpreterOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalstringFilterInterpreterOahGroup;
}


}
