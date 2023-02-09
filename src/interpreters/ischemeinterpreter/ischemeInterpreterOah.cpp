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

#include "ischemeWae.h"

#include "ischemeInterpreterOah.h"

#include "oahEarlyOptions.h"

#include "ischemeInterpreterInsiderHandler.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_ischemeInterpreterOahGroup gGlobalIschemeInterpreterOahGroup;

S_ischemeInterpreterOahGroup ischemeInterpreterOahGroup::create ()
{
  ischemeInterpreterOahGroup* o = new ischemeInterpreterOahGroup ();
  assert (o != nullptr);
  return o;
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
// JMI  initializeischemeInterpreterTraceOah ();
#endif

  // user options
  // --------------------------------------
  initializeIschemeOptions ();

  // maintaince options
  // --------------------------------------
//   initializeIschemeUserOptions (); JMI
}
//     void                  initializeIschemeUserOptions ();
//
//     void                  initializeIschemeMaintenanceOptions ();

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

  fDisplayToolAndInputAtom =
    oahBooleanAtom::create (
      "display-tool-and-input", "dtai",
R"(Write iScheme tool and input analysis activity to standard output.)",
      "fDisplayToolAndInput",
      fDisplayToolAndInput);

  subGroup->
    appendAtomToSubGroup (
      fDisplayToolAndInputAtom);

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
R"(Analyze the iScheme script, but don't launch the tool actually.
This is useful to check the options gathered by the iScheme interpreter,
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
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> ischemeInterpreterOahGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_ischemeInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterOahGroup>*> (v)) {
        S_ischemeInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching ischemeInterpreterOahGroup::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void ischemeInterpreterOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> ischemeInterpreterOahGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_ischemeInterpreterOahGroup>*
    p =
      dynamic_cast<visitor<S_ischemeInterpreterOahGroup>*> (v)) {
        S_ischemeInterpreterOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching ischemeInterpreterOahGroup::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void ischemeInterpreterOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      ".\\\" ==> ischemeInterpreterOahGroup::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}

void ischemeInterpreterOahGroup::printIschemeInterpreterOahValues (
  int fieldWidth)
{
  gLog <<
    "The ischeme options are:" <<
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

std::ostream& operator << (std::ostream& os, const S_ischemeInterpreterOahGroup& elt)
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
S_ischemeInterpreterOahGroup createGlobalIschemeInterpreterOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
		std::stringstream ss;

    ss <<
      "Creating global ischeme OAH group" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

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
