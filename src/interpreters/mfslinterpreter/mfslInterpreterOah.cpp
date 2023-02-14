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

// libmusicxml2
#include "visitor.h"

#include "mfStaticSettings.h"

#include "oahOah.h"

#include "mfslWae.h"

#include "mfslInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "mfslInterpreterInsiderHandler.h"

#include "waeHandlers.h"


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
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializemfslInterpreterTraceOah ();
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> mfslInterpreterOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfslInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterOahGroup>*> (v)) {
        S_mfslInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mfslInterpreterOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void mfslInterpreterOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> mfslInterpreterOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_mfslInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_mfslInterpreterOahGroup>*> (v)) {
        S_mfslInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching mfslInterpreterOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void mfslInterpreterOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> mfslInterpreterOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void mfslInterpreterOahGroup::displayMfslInterpreterOahValues (
  int fieldWidth)
{
  gLog <<
    "The mfsl options are:" <<
    std::endl;

  ++gIndenter;

  // trace
  // --------------------------------------

  gLog <<
    "Trace:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fTraceScanning" << ": " <<
      fTraceScanning <<
      std::endl <<
    std::setw (fieldWidth) << "fDisplayTokens" << ": " <<
      fDisplayTokens <<
      std::endl <<

    std::setw (fieldWidth) << "fTraceParsing" << ": " <<
      fTraceParsing <<
      std::endl <<

    std::setw (fieldWidth) << "fDisplayToolAndInput" << ": " <<
      fDisplayToolAndInput <<
      std::endl <<

    std::setw (fieldWidth) << "fDisplayOptions" << ": " <<
      fDisplayOptions <<
      std::endl <<

    std::setw (fieldWidth) << "fTraceInputs" << ": " <<
      fTraceInputs <<
      std::endl <<
    std::setw (fieldWidth) << "fTraceCaseInputStatements" << ": " <<
      fTraceCaseInputStatements <<
      std::endl <<

    std::setw (fieldWidth) << "fTraceChoices" << ": " <<
      fTraceChoices <<
      std::endl <<
    std::setw (fieldWidth) << "fTraceCaseChoiceStatements" << ": " <<
      fTraceCaseChoiceStatements <<
      std::endl <<

    std::setw (fieldWidth) << "fTraceCaseChoiceStatements" << ": " <<
      fTraceCaseChoiceStatements <<
      std::endl <<
    std::setw (fieldWidth) << "fTraceCaseInputStatements" << ": " <<
      fTraceCaseInputStatements <<
      std::endl <<
    std::setw (fieldWidth) << "fNoLaunch" << ": " <<
      fNoLaunch <<
      std::endl <<

    std::setw (fieldWidth) << "fTraceOptionsBlocks" << ": " <<
      fTraceOptionsBlocks <<
      std::endl;

  --gIndenter;

  // choice
  // --------------------------------------

  gLog <<
    "Choice:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fSelectChoiceToLabelsMultiMap" << ": ";

  ++gIndenter;

  if (fSelectChoiceToLabelsMultiMap.size ()) {
    for (std::pair<std::string, std::string> thePair : fSelectChoiceToLabelsMultiMap) {
      gLog <<
        thePair.first << ": " << thePair.second <<
        std::endl;
    } // for
  }

  --gIndenter;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_mfslInterpreterOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_mfslInterpreterOahGroup createGlobalMfslInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
		std::stringstream ss;

    ss <<
      "Creating global mfsl OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
