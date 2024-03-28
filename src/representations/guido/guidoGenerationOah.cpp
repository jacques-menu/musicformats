/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfOutputFileOah.h"
#include "mfStringsHandling.h"

#include "oahOah.h"
#include "oahHarmoniesExtraOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "guidoGenerationOah.h"

#include "oahEarlyOptions.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_guidoGenerationOahGroup gGlobalGuidoGenerationOahGroup;

S_guidoGenerationOahGroup guidoGenerationOahGroup::create ()
{
  guidoGenerationOahGroup* obj = new guidoGenerationOahGroup ();
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  gLog << std::left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // Guido
  // --------------------------------------

  createGuidoSubGroup ();
}

void guidoGenerationOahGroup::createGuidoSubGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
  gLog << std::left <<
    "Creating insider Guido subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> guidoGenerationOahGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_guidoGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_guidoGenerationOahGroup>*> (v)) {
        S_guidoGenerationOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching guidoGenerationOahGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void guidoGenerationOahGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> guidoGenerationOahGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_guidoGenerationOahGroup>*
    p =
      dynamic_cast<visitor<S_guidoGenerationOahGroup>*> (v)) {
        S_guidoGenerationOahGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gOahOahGroup->getTraceOahVisitors ()) {
          std::stringstream ss;

          ss <<
            ".\\\" ==> Launching guidoGenerationOahGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void guidoGenerationOahGroup::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      ".\\\" ==> guidoGenerationOahGroup::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
void guidoGenerationOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void guidoGenerationOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void guidoGenerationOahGroup::displayGuidoGenerationOahGroupValues (
  int fieldWidth)
{
  gLog <<
    "The generators options are:" <<
    std::endl;

  ++gIndenter;

  // Guido
  // --------------------------------------

  gLog << std::left <<
    std::setw (fieldWidth) << "Guido:" <<
    std::endl;

  ++gIndenter;

  gLog << std::left <<
    std::setw (fieldWidth) <<
    "fGenerateGuidoComments" << ": " <<
    fGenerateGuidoComments <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGenerateGuidoStem" << ": " <<
    fGenerateGuidoStem <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGenerateGuidoBars" << ": " <<
    fGenerateGuidoBars <<
    std::endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_guidoGenerationOahGroup createGlobalGuidoGenerationOahGroup ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "Creating global generators insider OAH group";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // protect library against multiple initializations
  if (! gGlobalGuidoGenerationOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalGuidoGenerationOahGroup =
      guidoGenerationOahGroup::create ();
    assert (gGlobalGuidoGenerationOahGroup != 0);

/* JMI ???
  fHandlerMultiComponent->
    appendVersionToComponent (
        mfcVersion::create (
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
