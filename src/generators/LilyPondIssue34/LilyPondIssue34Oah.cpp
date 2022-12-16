/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include <regex>

// libmusicxml2
#include "visitor.h"

#include "oahWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "msrOah.h"

#include "mfConstants.h"

#include "LilyPondIssue34Oah.h"

#include "oahEarlyOptions.h"

#include "LilyPondIssue34InsiderHandler.h"


namespace MusicFormats
{

//_______________________________________________________________________________

S_LilyPondIssue34OahGroup gGlobalLilyPondIssue34OahGroup;

S_LilyPondIssue34OahGroup LilyPondIssue34OahGroup::create ()
{
  LilyPondIssue34OahGroup* o = new LilyPondIssue34OahGroup ();
  assert (o != nullptr);
  return o;
}

LilyPondIssue34OahGroup::LilyPondIssue34OahGroup ()
  : oahGroup (
    "LilyPondIssue34",
    "help-lilypond-issue-34-group", "hli34-group",
R"(These options control the way LilyPondIssue34 works.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  fGenerationAPIKind =
    msrGenerationAPIKind::kMsrFunctionsAPIKind;

  fMultiGenerationOutputKind =
    mfMultiGenerationOutputKind::kGeneration_NO_;

  // initialize it
  initializeLilyPondIssue34OahGroup ();
}

LilyPondIssue34OahGroup::~LilyPondIssue34OahGroup ()
{}

void LilyPondIssue34OahGroup::initializeGenerationAPIOptions ()
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
          existingGenerationAPIKinds (K_MF_NAMES_LIST_MAX_LENGTH)),
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

void LilyPondIssue34OahGroup::initializeMultiGenerationOutputOptions ()
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
//       mfMultiGenerationOutputKind::kGeneration_NO_; // default value
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

void LilyPondIssue34OahGroup::initializeLilyPondIssue34OahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeLilyPondIssue34tracingOah ();
#endif

  // generation API
  // --------------------------------------
  // initializeGenerationAPIOptions ();

  //  multi-generation output kind
  // --------------------------------------
  initializeMultiGenerationOutputOptions ();
}

void LilyPondIssue34OahGroup::enforceGroupQuietness ()
{}

void LilyPondIssue34OahGroup::checkGroupOptionsConsistency ()
{
  switch (fMultiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_NO_:
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

void LilyPondIssue34OahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> LilyPondIssue34OahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_LilyPondIssue34OahGroup>*
    p =
      dynamic_cast<visitor<S_LilyPondIssue34OahGroup>*> (v)) {
        S_LilyPondIssue34OahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching LilyPondIssue34OahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void LilyPondIssue34OahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> LilyPondIssue34OahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_LilyPondIssue34OahGroup>*
    p =
      dynamic_cast<visitor<S_LilyPondIssue34OahGroup>*> (v)) {
        S_LilyPondIssue34OahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching LilyPondIssue34OahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void LilyPondIssue34OahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> LilyPondIssue34OahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

void LilyPondIssue34OahGroup::printLilyPondIssue34OahValues (
  int fieldWidth)
{
  gLogStream <<
    "The LilyPondIssue34 options are:" <<
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

std::ostream& operator << (std::ostream& os, const S_LilyPondIssue34OahGroup& elt)
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
S_LilyPondIssue34OahGroup createGlobalLilyPondIssue34OahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global LilyPondIssue34 OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalLilyPondIssue34OahGroup) {

    // initialize the generation API kinds map
    // ------------------------------------------------------

    initializeMsrGenerationAPI ();


    // initialize the multi-generation output kinds map
    // ------------------------------------------------------

    initializeMultiGenerationOutputKindsMap ();

    // create the LilyPondIssue34 options
    // ------------------------------------------------------

    gGlobalLilyPondIssue34OahGroup =
      LilyPondIssue34OahGroup::create ();
    assert (gGlobalLilyPondIssue34OahGroup != 0);
  }

  // return the global OAH group
  return gGlobalLilyPondIssue34OahGroup;
}


}
