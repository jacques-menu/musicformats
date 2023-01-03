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

#include "oahWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "oahOah.h"
#include "msrOah.h"

#include "mfConstants.h"

#include "Mikrokosmos3WanderingOah.h"

#include "oahEarlyOptions.h"

#include "Mikrokosmos3WanderingInsiderHandler.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_Mikrokosmos3WanderingOahGroup gGlobalMikrokosmos3WanderingOahGroup;

S_Mikrokosmos3WanderingOahGroup Mikrokosmos3WanderingOahGroup::create ()
{
  Mikrokosmos3WanderingOahGroup* o = new Mikrokosmos3WanderingOahGroup ();
  assert (o != nullptr);
  return o;
}

Mikrokosmos3WanderingOahGroup::Mikrokosmos3WanderingOahGroup ()
  : oahGroup (
    "Mikrokosmos3Wandering",
    "help-mikrokosmos-group", "hmkk-group",
R"(These options control the way Mikrokosmos3Wandering works.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  fGenerationAPIKind =
    msrGenerationAPIKind::kMsrFunctionsAPIKind;

  fMultiGenerationOutputKind =
    mfMultiGenerationOutputKind::kGeneration_UNKNOWN;

  // initialize it
  initializeMikrokosmos3WanderingOahGroup ();
}

Mikrokosmos3WanderingOahGroup::~Mikrokosmos3WanderingOahGroup ()
{}

void Mikrokosmos3WanderingOahGroup::initializeGenerationAPIOptions ()
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
    generationAPIKindDefaultValue =
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
          existingGenerationAPIKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
        std::regex ("DEFAULT_VALUE"),
        msrGenerationAPIKindAsString (
          generationAPIKindDefaultValue)),
      "GENERATION_API",
      "msrGenerationAPIKind",
      fGenerationAPIKind);

  subGroup->
    appendAtomToSubGroup (
      fGenerationAPIKindAtom);
}

void Mikrokosmos3WanderingOahGroup::initializeMultiGenerationOutputOptions ()
{
//   S_oahSubGroup
//     subGroup =
//       oahSubGroup::create (
//         "Multi-generation output",
//         "help- multi-generation-output", "hmgo",
// R"()",
//       oahElementVisibilityKind::kElementVisibilityWhole,
//       this);
//
//   appendSubGroupToGroup (subGroup);
//
//   const mfMultiGenerationOutputKind
//     multiGenerationKindDefaultValue =
//       mfMultiGenerationOutputKind::kGeneration_UNKNOWN; // default value
//
//   fMultiGenerationOutputKindAtom =
//     mfMultiGenerationOutputKindAtom::create (
//       K_MSR_GENERATION_API_KIND_LONG_NAME, K_MSR_GENERATION_API_KIND_SHORT_NAME,
//       regex_replace (
//         regex_replace (
//           regex_replace (
// R"(Generate GENERATION_API code to the output.
// The NUMBER generation API kinds available are:
// GENERATION_API_KINDS.
// The default is 'DEFAULT_VALUE'.)",
//             std::regex ("NUMBER"),
//             std::to_string (gGlobalMultiGenerationOutputKindsMap.size ())),
//           std::regex ("GENERATION_API_KINDS"),
//           existingMultiGenerationOutputKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
//         std::regex ("DEFAULT_VALUE"),
//         mfMultiGenerationOutputKindAsString (
//           multiGenerationKindDefaultValue)),
//       "fMultiGenerationOutputKind",
//       fMultiGenerationOutputKind);
//
//   subGroup->
//     appendAtomToSubGroup (
//       fMultiGenerationOutputKindAtom);
}

void Mikrokosmos3WanderingOahGroup::initializeMikrokosmos3WanderingOahGroup ()
{
#ifdef OAH_TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMikrokosmos3WanderingtracingOah ();
#endif

  // generation API
  // --------------------------------------
  // initializeGenerationAPIOptions ();

  //  multi-generation output kind
  // --------------------------------------
  initializeMultiGenerationOutputOptions ();
}

void Mikrokosmos3WanderingOahGroup::enforceGroupQuietness ()
{}

void Mikrokosmos3WanderingOahGroup::checkGroupOptionsConsistency ()
{
  switch (fMultiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN:
      {
        std::stringstream s;

        s <<
          fUpLinkToHandler->getHandlerServiceName () <<
          " needs an generate code option chosen among:" <<
          std::endl;

        ++gIndenter;

        s <<
          existingMultiGenerationOutputKinds (K_MF_NAMES_LIST_MAX_LENGTH);

        --gIndenter;

        oahError (s.str ());
      }
      break;
    default:
      ;
  } // switch
}

void Mikrokosmos3WanderingOahGroup::acceptIn (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void Mikrokosmos3WanderingOahGroup::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingOahGroup elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void Mikrokosmos3WanderingOahGroup::browseData (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

void Mikrokosmos3WanderingOahGroup::printMikrokosmos3WanderingOahValues (
  int fieldWidth)
{
  gLogStream <<
    "The Mikrokosmos3Wandering options are:" <<
    std::endl;

  ++gIndenter;

  // generation API kind
  // --------------------------------------

  gLogStream <<
    "Generation API:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fGenerationAPIKind" << ": " <<
      msrGenerationAPIKindAsString (fGenerationAPIKind) <<
      std::endl;

  --gIndenter;

  // multi-generation output kind
  // --------------------------------------

  gLogStream <<
    "multi-generation output:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) << "fMultiGenerationOutputKind" << ": " <<
      mfMultiGenerationOutputKindAsString (fMultiGenerationOutputKind) <<
      std::endl;

  --gIndenter;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_Mikrokosmos3WanderingOahGroup& elt)
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
S_Mikrokosmos3WanderingOahGroup createGlobalMikrokosmos3WanderingOahGroup ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global Mikrokosmos3Wandering OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMikrokosmos3WanderingOahGroup) {

    // initialize the generation API kinds map
    // ------------------------------------------------------

    initializeMsrGenerationAPI ();

    // initialize the generated output kinds map
    // ------------------------------------------------------

    initializeMultiGenerationOutputKindsMap ();

    // create the Mikrokosmos3Wandering options
    // ------------------------------------------------------

    gGlobalMikrokosmos3WanderingOahGroup =
      Mikrokosmos3WanderingOahGroup::create ();
    assert (gGlobalMikrokosmos3WanderingOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMikrokosmos3WanderingOahGroup;
}


}
