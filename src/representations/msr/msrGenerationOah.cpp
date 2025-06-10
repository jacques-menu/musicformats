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

#include "mfConstants.h"

#include "oahOah.h"
#include "msrOah.h"

#include "oahEarlyOptions.h"

#include "msrGenerationOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_msrGeneratorsOahGroup gGlobalMsrGeneratorsOahGroup;

S_msrGeneratorsOahGroup msrGeneratorsOahGroup::create ()
{
  msrGeneratorsOahGroup* obj = new msrGeneratorsOahGroup ();
  assert (obj != nullptr);
  return obj;
}

msrGeneratorsOahGroup::msrGeneratorsOahGroup ()
  : oahGroup (
      "MSR generators",
      "help-msr generators", "hmsrgen",
R"(These options control the way the translators to MSR work.)",
      oahElementVisibilityKind::kElementVisibilityWhole)
{
  fGenerationAPIKind = msrGenerationAPIKind::kMsrFunctionsAPIKind;

  // initialize it
  initializeMsrGeneratorsOahGroup ();
}

msrGeneratorsOahGroup::~msrGeneratorsOahGroup ()
{}

void msrGeneratorsOahGroup::initializeGenerationAPIOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generation API",
        "help-generation-api", "hga",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const msrGenerationAPIKind
    msrGenerationAPIKindDefaultValue =
      msrGenerationAPIKind::kMsrFunctionsAPIKind; // default value

  fGenerationAPIKindAtom =
    msrGenerationAPIKindAtom::create (
      K_MSR_GENERATION_API_KIND_LONG_NAME, K_MSR_GENERATION_API_KIND_SHORT_NAME,
      regex_replace (
        regex_replace (
          regex_replace (
R"(Generate GENERATION_API code to the output.
The NUMBER generation API kinds available are:
GENERATION_API_KINDS.
The default is 'DEFAULT_VALUE'.)",
            std::regex ("NUMBER"),
            std::to_string (gGlobalGenerationAPIKindsMap.size ())),
          std::regex ("GENERATION_API_KINDS"),
          availableGenerationAPIKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        std::regex ("DEFAULT_VALUE"),
        msrGenerationAPIKindAsString (
          msrGenerationAPIKindDefaultValue)),
      "GENERATION_API",
      "fGenerationAPIKind",
      fGenerationAPIKind);

  subGroup->
    appendAtomToSubGroup (
      fGenerationAPIKindAtom);
}

void msrGeneratorsOahGroup::initializeMsrGeneratorsOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsrGeneratorsTraceOah ();
#endif // MF_TRACE_IS_ENABLED

  // generation API
  // --------------------------------------
  initializeGenerationAPIOptions ();
}

//______________________________________________________________________________
void msrGeneratorsOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msrGeneratorsOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msrGeneratorsOahGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrGeneratorsOahGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrGeneratorsOahGroup>*
    p =
      dynamic_cast<visitor<S_msrGeneratorsOahGroup>*> (v)) {
        S_msrGeneratorsOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrGeneratorsOahGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrGeneratorsOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrGeneratorsOahGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrGeneratorsOahGroup>*
    p =
      dynamic_cast<visitor<S_msrGeneratorsOahGroup>*> (v)) {
        S_msrGeneratorsOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching msrGeneratorsOahGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrGeneratorsOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> msrGeneratorsOahGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void msrGeneratorsOahGroup::displayMsrGeneratorsOahValues (
  int fieldWidth)
{
  gLog <<
    "The msrGenerators options are:" <<
    std::endl;

  ++gIndenter;

  // generation API kind
  // --------------------------------------

  gLog <<
    "Generation API:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) << "gnerationAPIKind" << ": " <<
      msrGenerationAPIKindAsString (fGenerationAPIKind) <<
      std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrGeneratorsOahGroup& elt)
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
S_msrGeneratorsOahGroup createGlobalMsrGeneratorsOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global msrGenerators OAH group";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalMsrGeneratorsOahGroup) {

    // initialize the generation API kinds map
    // ------------------------------------------------------

    initializeMsrGenerationAPI ();

    // create the msrGenerators options
    // ------------------------------------------------------

    gGlobalMsrGeneratorsOahGroup =
      msrGeneratorsOahGroup::create ();
    assert (gGlobalMsrGeneratorsOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsrGeneratorsOahGroup;
}


}
