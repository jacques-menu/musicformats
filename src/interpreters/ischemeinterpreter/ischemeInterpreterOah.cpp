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

#include "visitor.h"

#include "oahOah.h"

#include "ischemeWae.h"

#include "ischemeInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "ischemeInterpreterInsiderHandler.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________

S_ischemeInterpreterOahGroup gGlobalIschemeInterpreterOahGroup;

S_ischemeInterpreterOahGroup ischemeInterpreterOahGroup::create ()
{
  ischemeInterpreterOahGroup* obj = new ischemeInterpreterOahGroup ();
  assert (obj != nullptr);
  return obj;
}

ischemeInterpreterOahGroup::ischemeInterpreterOahGroup ()
  : oahGroup (
      "ischeme",
      "help-ischeme-interpreter-group", "hmi-group",
R"(These options control the way iScheme interpreter works.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  // initialize it
  initializeIschemeInterpreterOahGroup ();
}

ischemeInterpreterOahGroup::~ischemeInterpreterOahGroup ()
{}

void ischemeInterpreterOahGroup::initializeIschemeInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeischemeInterpretertracingOah ();
#endif // MF_TRACE_IS_ENABLED

  // user options
  // --------------------------------------
  initializeIschemeOptions ();

  // maintaince options
  // --------------------------------------
//   initializeIschemeUserOptions (); JMI
}
//     void                  initializeIschemeUserOptions ();
//
//     void                  initializeIschemeMaintainanceOptions ();

void ischemeInterpreterOahGroup::initializeIschemeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "iScheme",
        "help-ischeme", "hischeme",
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
R"(Write a trace of the iScheme tokens to standard error.)",
        "fDisplayTokens",
        fDisplayTokens));

  fDisplayServiceAndInputAtom =
    oahBooleanAtom::create (
      "display-service-and-input", "dservandinput",
R"(Write iScheme service and input analysis activity to standard output.)",
      "fDisplayServiceAndInput",
      fDisplayServiceAndInput);

  subGroup->
    appendAtomToSubGroup (
      fDisplayServiceAndInputAtom);

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "display-options", "dopts",
R"(Write iScheme options analysis activity to standard output.)",
        "fDisplayOptions",
        fDisplayOptions));

  // no launch

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "no-launch", "nol",
R"(Analyze the iScheme script, but don't launch the service actually.
This is useful to check the options gathered by the iScheme interpreter,
and what command(s) would be launched.
This option implies the '-display-service-and-input, -dsai' option.)",
        "fNoLaunch",
        fNoLaunch,
        fDisplayServiceAndInputAtom));

  // input

  fInputSourcesSetAtom =
    oahStringSetAtom::create (
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
    oahStringToStringMultiMapAtom::create (
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
R"(Write a trace of the iScheme scanning by Flex-generated code to standard error.)",
        "fTraceScanning",
        fTraceScanning));

  // trace scanning is done by the Bison-generated code
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-parsing", "tparse",
R"(Write a trace of the iScheme parsing by Bison-generated code to standard error.)",
        "fTraceParsing",
        fTraceParsing));

  // trace inputs
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-inputs", "tinputs",
R"(Write iScheme case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-input-statements", "tinputsstats",
R"(Write iScheme case statements handling activity to standard output.)",
        "fTraceCaseInputStatements",
        fTraceCaseInputStatements));

  // trace choices
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choices", "tchoices",
R"(Write iScheme choice analysis activity to standard output.)",
        "fTraceChoices",
        fTraceChoices));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-choice-statements", "tchoicesstats",
R"(Write iScheme choice statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-case-choice-statements", "tcasechoices",
R"(Write iScheme case statements handling activity to standard output.)",
        "fTraceCaseChoiceStatements",
        fTraceCaseChoiceStatements));

  // trace options blocks
  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "trace-options-blocks", "toblocks",
R"(Write iScheme options blocks analysis activity to standard output.)",
        "fTraceOptionsBlocks",
        fTraceOptionsBlocks));
}

void ischemeInterpreterOahGroup::enforceGroupQuietness ()
{}

void ischemeInterpreterOahGroup::checkGroupOptionsConsistency ()
{}

void ischemeInterpreterOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> ischemeInterpreterOahGroup::acceptIn ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_ischemeInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterOahGroup>*> (v)) {
        S_ischemeInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching ischemeInterpreterOahGroup::visitStart ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void ischemeInterpreterOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> ischemeInterpreterOahGroup::acceptOut ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_ischemeInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterOahGroup>*> (v)) {
        S_ischemeInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          gLog <<
            ".\\\" ==> Launching ischemeInterpreterOahGroup::visitEnd ()" <<
            endl;
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void ischemeInterpreterOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    gLog <<
      ".\\\" ==> ischemeInterpreterOahGroup::browseData ()" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void ischemeInterpreterOahGroup::printIschemeInterpreterOahValues (
  int fieldWidth)
{
  gLog <<
    "The ischeme options are:" <<
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

std::ostream& operator<< (std::ostream& os, const S_ischemeInterpreterOahGroup& elt)
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
S_ischemeInterpreterOahGroup createGlobalIschemeInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
    gLog <<
      "Creating global ischeme OAH group" <<
      endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalIschemeInterpreterOahGroup) {
    // create the ischeme options
    // ------------------------------------------------------

    gGlobalIschemeInterpreterOahGroup =
      ischemeInterpreterOahGroup::create ();
    assert (gGlobalIschemeInterpreterOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalIschemeInterpreterOahGroup;
}


}
