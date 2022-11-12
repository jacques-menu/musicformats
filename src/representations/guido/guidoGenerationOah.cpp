/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "enableHarmoniesExtraOahIfDesired.h"

#include "mfStringsHandling.h"

#include "oahOah.h"

#include "outputFileOah.h"
#include "harmoniesExtraOah.h"

// MSR
#include "msrOah.h"
#include "msr2msrOah.h"


// mxsr

#include "guidoGenerationOah.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_guidoGenerationOahGroup gGlobalGuidoGenerationOahGroup;

S_guidoGenerationOahGroup guidoGenerationOahGroup::create ()
{
  guidoGenerationOahGroup* o = new guidoGenerationOahGroup ();
  assert (o != nullptr);

  return o;
}

guidoGenerationOahGroup::guidoGenerationOahGroup ()
  : oahGroup (
    "generators",
    "help-generators", "hgens",
R"(Options that are used by generators are grouped here.)",
    oahElementVisibilityKind::kElementVisibilityWhole)
{
  fGenerateGuidoComments = false;
  fGenerateGuidoStem = false;
  fGenerateGuidoBars = false;

  initializeGuidoGenerationOahGroup ();
}

guidoGenerationOahGroup::~guidoGenerationOahGroup ()
{}

//_______________________________________________________________________________
void guidoGenerationOahGroup::initializeGuidoGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << std::left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    std::endl;
  }
#endif

  // Guido
  // --------------------------------------

  createGuidoSubGroup ();
}

void guidoGenerationOahGroup::createGuidoSubGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
  gLogStream << std::left <<
    "Creating insider Guido subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    std::endl;
  }
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Guido",
        "help-generator-guido", "generator-guido",
R"()",
      oahElementVisibilityKind::kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // generate guido comments

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-guido-comments", "",
R"(Generate comments in the Guido output.)",
        "generateGuidoComments",
        fGenerateGuidoComments));

  // generate guido stem

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-guido-stem", "",
R"(Generate stem in the Guido output.)",
        "generateGuidoStem",
        fGenerateGuidoStem));

  // generate guido bars

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-guido-bars", "",
R"(Generate barLines in the Guido output.)",
        "generateGuidoBars",
        fGenerateGuidoBars));
}

//______________________________________________________________________________
void guidoGenerationOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> guidoGenerationOahGroup::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_guidoGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_guidoGenerationOahGroup>*> (v)) {
        S_guidoGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching guidoGenerationOahGroup::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void guidoGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> guidoGenerationOahGroup::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_guidoGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_guidoGenerationOahGroup>*> (v)) {
        S_guidoGenerationOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching guidoGenerationOahGroup::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void guidoGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> guidoGenerationOahGroup::browseData ()" <<
      std::endl;
  }
#endif
}

//______________________________________________________________________________
void guidoGenerationOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void guidoGenerationOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void guidoGenerationOahGroup::printGuidoGenerationOahGroupValues (
  int fieldWidth)
{
  gLogStream <<
    "The generators options are:" <<
    std::endl;

  ++gIndenter;

  // Guido
  // --------------------------------------

  gLogStream << std::left <<
    std::setw (fieldWidth) << "Guido:" <<
    std::endl;

  ++gIndenter;

  gLogStream << std::left <<
    std::setw (fieldWidth) <<
    "fGenerateGuidoComments" << " : " <<
    fGenerateGuidoComments <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGenerateGuidoStem" << " : " <<
    fGenerateGuidoStem <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGenerateGuidoBars" << " : " <<
    fGenerateGuidoBars <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_guidoGenerationOahGroup createGlobalGuidoGenerationOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "Creating global generators insider OAH group" <<
      std::endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalGuidoGenerationOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalGuidoGenerationOahGroup =
      guidoGenerationOahGroup::create ();
    assert (gGlobalGuidoGenerationOahGroup != 0);

/* JMI ???
  fHandlerMultiComponent->
    appendVersionDescrToComponent (
        mfcVersionDescr::create (
          mfcVersionNumber::createFromString ("0.9.50"),
          "October 6, 2021",
          std::list<std::string> {
            "Start of sequential versions numbering"
          }
    ));
*/
  }

  // return the global OAH group
  return gGlobalGuidoGenerationOahGroup;
}


}
