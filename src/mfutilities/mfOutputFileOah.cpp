/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <regex>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "mfOutputFileOah.h"

#include "oahEarlyOptions.h"

#include "oahAtomsCollection.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_outputFileOahGroup gGlobalOutputFileOahGroup;

S_outputFileOahGroup outputFileOahGroup::create ()
{
  outputFileOahGroup* obj = new outputFileOahGroup ();
  assert (obj != nullptr);
  return obj;
}

outputFileOahGroup::outputFileOahGroup ()
  : oahGroup (
      "OutputFile",
      "help-output-file-group", "hofg",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  initializeOutputFileOah ();
}

outputFileOahGroup::~outputFileOahGroup ()
{}

void outputFileOahGroup::initializeOutputFileOah ()
{
  // output file name
  // --------------------------------------
  initializeOutputFileNameOptions ();
}

void outputFileOahGroup::initializeOutputFileNameOptions ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  gLog << std::left <<
    "Creating insider output file subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Output file",
        "help-output-file", "hof",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // output filename

  fOutputFileNameStringAtom =
    oahStringAtom::create (
      "output-file-name", "o",
R"(Write output to file FILENAME instead of standard output.)",
      "FILENAME",
      "fOutputFileName",
      fOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fOutputFileNameStringAtom);

  // auto output filename

  fAutoOutputFileNameAtom =
    oahBooleanAtom::create (
      "auto-output-file-name", "aofn",
R"(This option can only be used when writing to a file.
Write the output to a file in the current working directory.
The file name is derived from that of the input file:
any suffix after the '.' is replaced by one suited for the output format,
or such a suffix is adde if no '.' is present.)",
      "fAutoOutputFileName",
      fAutoOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fAutoOutputFileNameAtom);
}

//______________________________________________________________________________
void outputFileOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void outputFileOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void outputFileOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> outputFileOahGroup::acceptIn ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_outputFileOahGroup>*
    p =
      dynamic_cast<visitor<S_outputFileOahGroup>*> (v)) {
        S_outputFileOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching outputFileOahGroup::visitStart ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void outputFileOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> outputFileOahGroup::acceptOut ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_outputFileOahGroup>*
    p =
      dynamic_cast<visitor<S_outputFileOahGroup>*> (v)) {
        S_outputFileOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching outputFileOahGroup::visitEnd ()";

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void outputFileOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> outputFileOahGroup::browseData ()";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void outputFileOahGroup::displayOutputFileOahValues (int fieldWidth)
{
  gLog <<
    "The outputFile options are:" <<
    std::endl;

  ++gIndenter;

  // output file
  // --------------------------------------

  gLog << std::left <<
    std::setw (fieldWidth) << "Output file:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "fOutputFileName" << ": \"" <<
    fOutputFileName <<
    "\"" <<
    std::endl <<
    std::setw (fieldWidth) << "fAutoOutputFileName" << ": \"" <<
    fAutoOutputFileName <<
    "\"" <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_outputFileOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_outputFileOahGroup createGlobalOutputFileOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global outputFile OAH group";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalOutputFileOahGroup) {
    // create the global outputFile options group
    gGlobalOutputFileOahGroup =
      outputFileOahGroup::create ();
    assert (gGlobalOutputFileOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalOutputFileOahGroup;
}


}
