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

#include "oahEarlyOptions.h"

#include "msrGenerationOah.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________

S_msrGeneratorsOahGroup gGlobalMsrGeneratorsOahGroup;

S_msrGeneratorsOahGroup msrGeneratorsOahGroup::create ()
{
  msrGeneratorsOahGroup* o = new msrGeneratorsOahGroup ();
  assert (o != nullptr);
  return o;
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
            regex ("NUMBER"),
            to_string (gGlobalGenerationAPIKindsMap.size ())),
          regex ("GENERATION_API_KINDS"),
          existingGenerationAPIKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
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
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsrGeneratorstracingOah ();
#endif

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGeneratorsOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrGeneratorsOahGroup>*
    p =
      dynamic_cast<visitor<S_msrGeneratorsOahGroup>*> (v)) {
        S_msrGeneratorsOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGeneratorsOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrGeneratorsOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGeneratorsOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrGeneratorsOahGroup>*
    p =
      dynamic_cast<visitor<S_msrGeneratorsOahGroup>*> (v)) {
        S_msrGeneratorsOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrGeneratorsOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrGeneratorsOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrGeneratorsOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msrGeneratorsOahGroup::printMsrGeneratorsOahValues (
  int fieldWidth)
{
  gLogStream <<
    "The msrGenerators options are:" <<
    endl;

  ++gIndenter;

  // generation API kind
  // --------------------------------------

  gLogStream <<
    "Generation API:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "gnerationAPIKind" << " : " <<
      msrGenerationAPIKindAsString (fGenerationAPIKind) <<
      endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrGeneratorsOahGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msrGeneratorsOahGroup createGlobalMsrGeneratorsOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global msrGenerators OAH group" <<
      endl;
  }
#endif

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
